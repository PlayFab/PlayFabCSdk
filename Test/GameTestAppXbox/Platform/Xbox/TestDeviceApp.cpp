// Copyright (C) Microsoft Corporation. All rights reserved.
// TestDeviceApp.cpp - ATG-based test device application implementation
#include "pch.h"
#include "TestDeviceApp.h"
#include "DeviceGameSaveState.h"
#include "DeviceGameSave.h"
#include "DeviceWebSocketConnection.h"
#include "DeviceCommandProcessor.h"
#include "DeviceApplication.h"

#include "ATGTelemetry.h"
#include "ATGColors.h"
#include "TextConsole.h"
#include "FindMedia.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

TestDeviceApp::TestDeviceApp() noexcept(false) :
    m_frame(0),
    m_state(nullptr),
    m_initialized(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

TestDeviceApp::~TestDeviceApp()
{
    if (m_state && m_state->keepAliveUser)
    {
        XUserCloseHandle(m_state->keepAliveUser);
        m_state->keepAliveUser = nullptr;
    }
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }
}

void TestDeviceApp::Initialize(HWND window, int width, int height, LPCWSTR cmdLine)
{
    m_gamePad = std::make_unique<GamePad>();
    m_keyboard = std::make_unique<Keyboard>();

    m_deviceResources->SetWindow(window, width, height);
    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();
    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Initialize test device infrastructure
    m_state = GetSampleGameSaveState();

    InitializeHCTraceToVerboseLog();

#ifdef _DEBUG
    const char* buildConfig = "Debug";
#else
    const char* buildConfig = "Release";
#endif

    LogToWindowFormat("Game Test Device [Xbox] [%s] starting...", buildConfig);

    // Log and validate sandbox — mismatched sandbox causes silent auth failures
    {
        char sandboxId[XSystemXboxLiveSandboxIdMaxBytes]{};
        size_t used = 0;
        HRESULT sbHr = XSystemGetXboxLiveSandboxId(sizeof(sandboxId), sandboxId, &used);
        if (SUCCEEDED(sbHr))
        {
            LogToWindowFormat("Xbox Live Sandbox: %s", sandboxId);
            if (strcmp(sandboxId, "XDKS.1") != 0)
            {
                LogToWindowFormat("WARNING: Expected sandbox 'XDKS.1' but got '%s' — PlayFab CustomID auth and two-device tests will fail", sandboxId);
            }
        }
        else
        {
            LogToWindowFormat("WARNING: Failed to query sandbox (hr=0x%08X)", static_cast<uint32_t>(sbHr));
        }
    }

    // Log the HTTP stack actually bound at runtime. This is deliberately verbose because a
    // silent mis-binding here invalidates every console HTTP test: the GDK ships an
    // Xbox.LibHttpClient extension whose prebuilt libHttpClient.GDK.dll is WinHttp-only (no
    // CurlProvider/XCurl). If the EXE binds that instead of the repo-built libHttpClient.dll,
    // console HTTP silently runs on WinHttp and any Curl/XCurl work is unreachable.
    {
        XSystemDeviceType deviceType = XSystemGetDeviceType();
        LogToWindowFormat("HTTP stack: XSystemGetDeviceType()=%d (Pc=%d) -> expected provider: %s",
            static_cast<int>(deviceType),
            static_cast<int>(XSystemDeviceType::Pc),
            (deviceType != XSystemDeviceType::Pc) ? "XCurl/CurlProvider" : "WinHttp");

        const char* kModules[] = { "libHttpClient.dll", "libHttpClient.GDK.dll", "XCurl.dll", "winhttp.dll" };
        for (const char* moduleName : kModules)
        {
            HMODULE hMod = GetModuleHandleA(moduleName);
            if (hMod != nullptr)
            {
                char modulePath[MAX_PATH]{};
                if (GetModuleFileNameA(hMod, modulePath, ARRAYSIZE(modulePath)) > 0)
                {
                    LogToWindowFormat("HTTP stack: LOADED %s -> %s", moduleName, modulePath);
                }
                else
                {
                    LogToWindowFormat("HTTP stack: LOADED %s (path unavailable)", moduleName);
                }
            }
            else
            {
                LogToWindowFormat("HTTP stack: not loaded: %s", moduleName);
            }
        }

        if (GetModuleHandleA("libHttpClient.GDK.dll") != nullptr)
        {
            LogToWindow("HTTP stack: WARNING - the GDK prebuilt libHttpClient.GDK.dll is loaded. "
                        "It is WinHttp-only; Curl/XCurl provider code will NOT run. "
                        "Remove Xbox.LibHttpClient from GDKExtLibNames in GameTestAppXbox.vcxproj.");
        }
    }

    // Pre-flight check: verify an Xbox user is available (silent login)
    // Two-device scenarios require both devices to have a signed-in user.
    {
        XAsyncBlock async{};
        HRESULT userHr = XUserAddAsync(XUserAddOptions::AddDefaultUserSilently, &async);
        if (SUCCEEDED(userHr))
        {
            userHr = XAsyncGetStatus(&async, true);
        }
        if (SUCCEEDED(userHr))
        {
            XUserHandle tempUser = nullptr;
            HRESULT resultHr = XUserAddResult(&async, &tempUser);
            if (SUCCEEDED(resultHr) && tempUser)
            {
                LogToWindow("Xbox user available (silent login OK)");
                // Keep this handle open for the lifetime of the app. Closing the only
                // handle to the default user tears down GDK's silent resolution, which
                // makes the scenario's later XUserAddAsync(AddDefaultUserSilently) call
                // time out (E_ABORT after 5s). Holding one handle keeps resolution warm.
                if (m_state->keepAliveUser)
                {
                    XUserCloseHandle(m_state->keepAliveUser);
                }
                m_state->keepAliveUser = tempUser;
            }
            else
            {
                LogToWindowFormat("WARNING: No Xbox user available (result hr=0x%08X) — two-device scenarios will fail", static_cast<uint32_t>(resultHr));
            }
        }
        else
        {
            LogToWindowFormat("WARNING: No Xbox user available (hr=0x%08X) — two-device scenarios will fail. Ensure a user is signed in on the console.", static_cast<uint32_t>(userHr));
        }
    }

    SetSampleDeviceEngineType(DetectSampleDeviceEngineType());

    HRESULT wsInitHr = m_state->websocketClient.Initialize();
    LogToWindowFormat("DeviceWebSocketClient init (hr=0x%08X)", static_cast<uint32_t>(wsInitHr));
    Sample_GameSave_ConfigureWebSocketLogging(m_state);

    // Parse command line (cmdLine excludes exe name, matching Windows WinMain convention)
    char narrowCmdLine[4096] = {};
    if (cmdLine && *cmdLine)
    {
        WideCharToMultiByte(CP_UTF8, 0, cmdLine, -1, narrowCmdLine, sizeof(narrowCmdLine), nullptr, nullptr);
    }
    if (!Sample_GameSave_ParseArgs(narrowCmdLine, m_state))
    {
        LogToWindow("Failed to parse command line arguments");
    }

    HRESULT hr = XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &m_state->taskQueue);
    if (FAILED(hr))
    {
        LogToWindowFormat("Failed to create task queue (hr=0x%08X)", static_cast<uint32_t>(hr));
    }
    else
    {
        XTaskQueueSetCurrentProcessTaskQueue(m_state->taskQueue);
        LogToWindow("XTaskQueueSetCurrentProcessTaskQueue invoked");
    }

    m_initialized = true;
}

void TestDeviceApp::Tick()
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Frame %llu", m_frame);

#ifdef _GAMING_XBOX
    m_deviceResources->WaitForOrigin();
#endif

    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();

    PIXEndEvent();
    m_frame++;
}

void TestDeviceApp::Update(DX::StepTimer const&)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    auto pad = m_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        m_gamePadButtons.Update(pad);

        if (pad.IsViewPressed() && pad.IsMenuPressed())
        {
            ExitGame();
            return;
        }
    }
    else
    {
        m_gamePadButtons.Reset();
    }

    auto kb = m_keyboard->GetState();
    m_keyboardButtons.Update(kb);

    if (kb.Escape)
    {
        ExitGame();
    }

    if (m_initialized && m_state != nullptr && !m_state->quit)
    {
        PumpWebSocketAutoConnect(m_state);
    }

    PIXEndEvent();
}

void TestDeviceApp::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    ID3D12DescriptorHeap* heap = m_resourceDescriptors->Heap();
    commandList->SetDescriptorHeaps(1, &heap);

    m_console->Render(commandList);

    PIXEndEvent(commandList);

    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
    PIXEndEvent();
}

void TestDeviceApp::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, ATG::Colors::Background, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}

void TestDeviceApp::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

#ifdef _GAMING_DESKTOP
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = { D3D_SHADER_MODEL_6_0 };
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel)))
        || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }
#endif

    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    m_resourceDescriptors = std::make_unique<DirectX::DescriptorPile>(device,
        Descriptors::Count,
        Descriptors::Reserve);

    const RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());

    // Create text console
    {
        ResourceUploadBatch resourceUpload(device);
        resourceUpload.Begin();

        wchar_t fontPath[MAX_PATH] = {};
        DX::FindMediaFile(fontPath, MAX_PATH, L"SegoeUI_18.spritefont");

        m_console = std::make_unique<DX::TextConsole>(
            device,
            resourceUpload,
            rtState,
            fontPath,
            m_resourceDescriptors->GetCpuHandle(Descriptors::Font),
            m_resourceDescriptors->GetGpuHandle(Descriptors::Font));
        m_console->SetDebugOutput(true);
        m_console->SetForegroundColor(DirectX::Colors::White);

        auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
        uploadResourcesFinished.wait();
    }

    // Register console with logging system
    DeviceLoggingInitializePlatform(m_console.get());
}

void TestDeviceApp::CreateWindowSizeDependentResources()
{
    auto const size = m_deviceResources->GetOutputSize();

    // Set console viewport to cover most of the screen
    RECT consoleRect = { 10, 10, size.right - 10, size.bottom - 10 };
    if (m_console)
    {
        m_console->SetWindow(consoleRect);
    }

    auto const viewport = m_deviceResources->GetScreenViewport();
    m_console->SetViewport(viewport);
}

void TestDeviceApp::OnDeviceLost()
{
    m_graphicsMemory.reset();
    m_console.reset();
    m_resourceDescriptors.reset();
}

void TestDeviceApp::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

void TestDeviceApp::OnActivated()
{
}

void TestDeviceApp::OnDeactivated()
{
}

void TestDeviceApp::OnSuspending()
{
    // Model the PlayFabMultiplayer PubSub/WebRequestManager teardown from bug 63050439: a
    // synchronous, blocking wait performed ON THE SUSPEND PATH against a task queue whose ports
    // XAsync has already suspended. If LHC still needs queued work to finish an in-flight request,
    // that work can no longer run (SuspendPort blocks Manual, ThreadPool and SerializedThreadPool
    // alike), so this call never returns, suspend never completes, and the PLM watchdog terminates
    // the title -- exactly the Forza signature. Armed by the ArmSuspendQueueTerminate command.
    if (m_state && m_state->terminateQueueOnSuspend && m_state->taskQueue)
    {
        LogToWindow("OnSuspending: XTaskQueueTerminate(wait=true) - modeling PFMultiplayer suspend teardown");
        XTaskQueueTerminate(m_state->taskQueue, true /*wait*/, nullptr, nullptr);
        LogToWindow("OnSuspending: XTaskQueueTerminate returned");
        m_state->terminateQueueOnSuspend = false;
    }

    m_deviceResources->Suspend();
}

void TestDeviceApp::OnResuming()
{
    m_deviceResources->Resume();
    m_timer.ResetElapsedTime();
}

void TestDeviceApp::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void TestDeviceApp::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

void TestDeviceApp::GetDefaultSize(int& width, int& height) const noexcept
{
    width = 1440;
    height = 810;
}

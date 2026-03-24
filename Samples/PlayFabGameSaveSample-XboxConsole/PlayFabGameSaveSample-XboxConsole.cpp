//--------------------------------------------------------------------------------------
// PlayFabGameSaveSample-XboxConsole.cpp
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayFabGameSaveSample-XboxConsole.h"

#include "ATGColors.h"
#include "FindMedia.h"

#include <fstream>
#include <chrono>

extern void ExitSample() noexcept;

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Sample::Sample() noexcept(false) : m_frame(0)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->SetClearColor(ATG::Colors::Background);
    m_deviceResources->RegisterDeviceNotify(this);
    m_liveResources = std::make_shared<ATG::LiveResources>();
    m_liveInfoHUD = std::make_unique<ATG::LiveInfoHUD>("PF Game Saves GDK Sample");

    DX::ThrowIfFailed(
        XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::Manual, &m_mainAsyncQueue));

    ATG::UIConfig uiconfig;
    m_ui = std::make_unique<ATG::UIManager>(uiconfig);
    m_log = std::make_unique<DX::TextConsoleImage>();

    // Seed RNG based on time
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq seed{static_cast<uint32_t>(now & 0xFFFFFFFF), static_cast<uint32_t>((now >> 32) & 0xFFFFFFFF)};
    m_rng.seed(seed);
}

Sample::~Sample()
{
    if (m_deviceResources)
    {
        m_deviceResources->WaitForGpu();
    }

    if (m_mainAsyncQueue)
    {
        XTaskQueueCloseHandle(m_mainAsyncQueue);
        m_mainAsyncQueue = nullptr;
    }
}

// Initialize the Direct3D resources required to run.
void Sample::Initialize(HWND window, int width, int height)
{
    m_gamePad = std::make_unique<GamePad>();
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    m_deviceResources->SetWindow(window, width, height);

    wchar_t result[MAX_PATH];
    DX::FindMediaFile(result, MAX_PATH, L".\\Assets\\SampleUI.csv");
    m_ui->LoadLayout(result, L".\\Assets\\");

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_liveResources->SetUserChangedCallback([this](XUserHandle user)
                                            {
        m_liveInfoHUD->SetUser(user, m_liveResources->GetAsyncQueue());
        m_ui->FindPanel<ATG::IPanel>(2000)->Show();
        m_signInInProgress = false; // completed
        PostLog(L"Sign-in completed"); });

    m_liveResources->SetUserSignOutCompletedCallback([this](XUserHandle /*user*/)
                                                     {
        m_liveInfoHUD->SetUser(nullptr, m_liveResources->GetAsyncQueue());
        m_ui->FindPanel<ATG::IPanel>(2000)->Close();
        m_signInInProgress = false; // completed
        PostLog(L"Sign-out completed"); });

    m_liveResources->SetErrorHandler([this](HRESULT error)
                                     {
        if (error == E_GAMEUSER_NO_DEFAULT_USER || error == E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED)
        {
            m_liveResources->SignInWithUI();
        }
        else
        {
            m_log->Format(DirectX::Colors::Red, L"Xbox Live Error: 0x%08X\n", error);
        } });

    while (!m_liveResources->IsNetworkAvailable())
    {
        SwitchToThread();
    }

    m_liveResources->Initialize();
    m_liveInfoHUD->Initialize();

    SetupUI();

    // Initialize PlayFab and Game Saves minimally
    if (SUCCEEDED(InitializePlayFab()))
    {
        InitializeGameSaves();
    }
}

void Sample::SetupUI()
{
    // Ensure on-screen legend reflects PF Game Save button mappings even if a stale CSV is deployed
    try
    {
        // Ensure the UI control panel is visible
        auto panel = m_ui->FindPanel<ATG::IPanel>(2000);
        panel->Show();

        // Add one multi-line legend with one button per line and position it higher

        // Halve the width of the legend area so it doesn't span too far to the right
        // Original: left=82, right=1700 -> width=1618. New width=809 => right=82+809=891
        RECT r{82, 600, 891, 1000};
        // Desktop: include keyboard shortcuts and use a larger legend font; Xbox: controller-only with small font
        unsigned style = ATG::Legend::c_StyleTransparent;
#ifndef _GAMING_DESKTOP
        style |= ATG::Legend::c_StyleFontSmall;
        auto text =
            L"[Menu] or Tab - Sign in\n"
            L"[A] - Add user to Game Saves\n"
            L"[X] - Save 10MB file\n"
            L"[Y] - Load/verify save\n"
            L"[B] - Delete save\n"
            L"[RB] - Set short save description\n"
            L"[View] - Upload (release device)\n"
            L"[LB] - Upload (keep active)\n"
            L"[RThumb] - Exit";
#else
        auto text =
            L"[Menu] or Tab - Sign in\n"
            L"[A] or 'A' - Add user to Game Saves\n"
            L"[X] or 'X' - Save 10MB file\n"
            L"[Y] or 'Y' - Load/verify save\n"
            L"[B] or 'B' - Delete save\n"
            L"[RB] or 'D' - Set short save description\n"
            L"[View] or 'R' - Upload (release device)\n"
            L"[LB] or 'U' - Upload (keep active)\n"
            L"[RThumb] or Esc - Exit";
#endif
        auto legend = new ATG::Legend(2010, text, r, style);
        panel->Add(legend);

        // Hide the original single-line legend to avoid clipping (after adding our override)
        if (auto orig = m_ui->FindControl<ATG::IControl>(2000, 2001))
        {
            orig->SetVisible(false);
        }

        // Hide the old 'Results' label if present (ID 2003)
        if (auto results = m_ui->FindControl<ATG::IControl>(2000, 2003))
        {
            results->SetVisible(false);
        }
    }
    catch (...)
    {
        // If the legend/control ids change or layout not loaded yet, skip without failing
    }
}

void Sample::PostLog(const wchar_t *text)
{
    if (!text)
        return;
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logPending.emplace_back(text);
}

void Sample::PostLogf(const wchar_t *fmt, ...)
{
    if (!fmt)
        return;
    va_list args;
    va_start(args, fmt);
    wchar_t buf[1024]{};
    _vsnwprintf_s(buf, _TRUNCATE, fmt, args);
    va_end(args);
    PostLog(buf);
}

#pragma region Frame Update
// Executes basic render loop.
void Sample::Tick()
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Frame %llu", m_frame);

#ifdef _GAMING_XBOX
    m_deviceResources->WaitForOrigin();
#endif

    m_timer.Tick([&]()
                 { Update(m_timer); });

    m_mouse->EndOfInputFrame();

    Render();

    PIXEndEvent();
    m_frame++;
}

// Updates the world.
void Sample::Update(DX::StepTimer const &timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    auto pad = m_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        m_gamePadButtons.Update(pad);

        // Exit on right thumbstick press (R3)
        if (m_gamePadButtons.rightStick == GamePad::ButtonStateTracker::PRESSED)
        {
            ExitSample();
        }

        if (m_gamePadButtons.menu == GamePad::ButtonStateTracker::PRESSED && !m_signInInProgress)
        {
            m_signInInProgress = true;
            if (!m_liveResources->IsUserSignedIn())
            {
                m_liveResources->SignInSilently();
            }
            else
            {
                m_liveResources->SignInWithUI();
            }
            if (!m_localUser)
            {
                (void)SignInAndCreateLocalUser();
            }
        }

        // A = Add user to GameSaves (download)
        if (m_gamePadButtons.a == GamePad::ButtonStateTracker::PRESSED && !m_addUserToGSInProgress && !m_userAddedToGS)
        {
            (void)AddUserToGameSaves();
        }
        // X = Save 10MB file
        if (m_gamePadButtons.x == GamePad::ButtonStateTracker::PRESSED)
        {
            DoSave();
        }
        // Y = Load verify
        if (m_gamePadButtons.y == GamePad::ButtonStateTracker::PRESSED)
        {
            DoLoad();
        }
        // B = Delete file
        if (m_gamePadButtons.b == GamePad::ButtonStateTracker::PRESSED)
        {
            DoDelete();
        }
        // View = Upload and release device as active
        if (m_gamePadButtons.view == GamePad::ButtonStateTracker::PRESSED)
        {
            DoUpload(false);
        }
        // LeftShoulder = Upload keep device active
        if (m_gamePadButtons.leftShoulder == GamePad::ButtonStateTracker::PRESSED)
        {
            DoUpload(true);
        }
        // RightShoulder = Set short save description
        if (m_gamePadButtons.rightShoulder == GamePad::ButtonStateTracker::PRESSED)
        {
            DoSetSaveDescription();
        }

        m_ui->Update(elapsedTime, pad);
    }
    else
    {
        m_gamePadButtons.Reset();
    }

    auto kb = m_keyboard->GetState();
    m_keyboardButtons.Update(kb);

    // No keyboard exit; use controller R3 to exit per sample guidance

    if (m_keyboardButtons.IsKeyReleased(Keyboard::Keys::Tab) && !m_signInInProgress)
    {
        m_signInInProgress = true;
        if (!m_liveResources->IsUserSignedIn())
        {
            m_liveResources->SignInSilently();
        }
        else
        {
            m_liveResources->SignInWithUI();
        }
    }

    m_ui->Update(elapsedTime, *m_mouse, *m_keyboard);

    m_liveInfoHUD->Update(m_deviceResources->GetCommandQueue());

    // Flush pending logs on main thread
    {
        std::deque<std::wstring> local;
        {
            std::lock_guard<std::mutex> lock(m_logMutex);
            if (!m_logPending.empty())
            {
                local.swap(m_logPending);
            }
        }
        for (auto &s : local)
        {
            m_log->WriteLine(s.c_str());
        }
    }

    while (XTaskQueueDispatch(m_mainAsyncQueue, XTaskQueuePort::Completion, 0))
    {
        SwitchToThread();
    }

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Sample::Render()
{
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    ID3D12DescriptorHeap *heap = m_resourceDescriptors->Heap();
    commandList->SetDescriptorHeaps(1, &heap);

    m_liveInfoHUD->Render(commandList);
    m_log->Render(commandList);
    // m_display removed
    m_ui->Render(commandList);

    PIXEndEvent(commandList);

    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();
    m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Sample::Clear()
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
#pragma endregion

#pragma region Message Handlers
void Sample::OnActivated() {}
void Sample::OnDeactivated() {}
void Sample::OnSuspending() { m_deviceResources->Suspend(); }
void Sample::OnResuming()
{
    m_deviceResources->Resume();
    m_timer.ResetElapsedTime();
    m_gamePadButtons.Reset();
    m_keyboardButtons.Reset();
    m_liveResources->Refresh();
}
void Sample::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}
void Sample::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;
    CreateWindowSizeDependentResources();
}
void Sample::GetDefaultSize(int &width, int &height) const noexcept
{
    width = 1840;
    height = 1035;
}
#ifdef _GAMING_DESKTOP
void Sample::OnKeyDown(WPARAM vk)
{
    switch (vk)
    {
    case VK_TAB:
        if (!m_signInInProgress)
        {
            m_signInInProgress = true;
            PostLog(L"Sign-in started");
            if (!m_liveResources->IsUserSignedIn())
                m_liveResources->SignInSilently();
            else
                m_liveResources->SignInWithUI();
        }
        break;
    case 'A':
        if (!m_addUserToGSInProgress && !m_userAddedToGS)
        {
            (void)AddUserToGameSaves();
        }
        else if (m_userAddedToGS)
        {
            PostLog(L"User already added to Game Saves");
        }
        break;
    case 'X':
        DoSave();
        break;
    case 'Y':
        DoLoad();
        break;
    case 'B':
        DoDelete();
        break;
    case 'D':
        DoSetSaveDescription();
        break;
    case 'R':
        (void)DoUpload(false);
        break;
    case 'U':
        (void)DoUpload(true);
        break;
    case VK_ESCAPE:
        ExitSample();
        break;
    default:
        break;
    }
}
#endif
#pragma endregion

#pragma region Direct3D Resources
void Sample::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

#ifdef _GAMING_DESKTOP
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel = {D3D_SHADER_MODEL_6_0};
    if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel))) || (shaderModel.HighestShaderModel < D3D_SHADER_MODEL_6_0))
    {
        throw std::runtime_error("Shader Model 6.0 is not supported!");
    }
#endif

    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());

    m_resourceDescriptors = std::make_unique<DirectX::DescriptorPile>(device,
                                                                      Descriptors::Count,
                                                                      Descriptors::Reserve);

    ResourceUploadBatch resourceUpload(device);
    resourceUpload.Begin();

    m_liveInfoHUD->RestoreDevice(device, rtState, resourceUpload, *m_resourceDescriptors);

    wchar_t font[260];
    wchar_t background[260];

    // Use a larger font on Desktop to make text 2x bigger
#ifdef _GAMING_DESKTOP
    DX::FindMediaFile(font, 260, L"SegoeUI_30.spritefont");
#else
    DX::FindMediaFile(font, 260, L"courier_16.spritefont");
#endif
    DX::FindMediaFile(background, 260, L"ATGSampleBackground.DDS");

    m_log->RestoreDevice(
        device,
        resourceUpload,
        rtState,
        font,
        background,
        m_resourceDescriptors->GetCpuHandle(Descriptors::Font),
        m_resourceDescriptors->GetGpuHandle(Descriptors::Font),
        m_resourceDescriptors->GetCpuHandle(Descriptors::Background),
        m_resourceDescriptors->GetGpuHandle(Descriptors::Background));

    // Results console removed

    m_ui->RestoreDevice(device, rtState, resourceUpload, *m_resourceDescriptors);

    auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
    uploadResourcesFinished.wait();
}

void ScaleRect(const RECT &originalDisplayRect, const RECT &displayRect, const RECT &originalSubRect, RECT &scaledSubRect)
{
    const float widthScale = ((float)displayRect.right - (float)displayRect.left) / ((float)originalDisplayRect.right - (float)originalDisplayRect.left);
    const float heightScale = ((float)displayRect.bottom - (float)displayRect.top) / ((float)originalDisplayRect.bottom - (float)originalDisplayRect.top);

    scaledSubRect.top = (LONG)((float)originalSubRect.top * heightScale);
    scaledSubRect.left = (LONG)((float)originalSubRect.left * widthScale);
    scaledSubRect.bottom = (LONG)((float)originalSubRect.bottom * heightScale);
    scaledSubRect.right = (LONG)((float)originalSubRect.right * widthScale);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Sample::CreateWindowSizeDependentResources()
{
    auto const fullscreen = m_deviceResources->GetOutputSize();
    auto const viewport = m_deviceResources->GetScreenViewport();

    m_liveInfoHUD->SetViewport(viewport);

    static const RECT originalScale = {0, 0, 1920, 1080};
    // Right-side text area (Log) - expanded to cover former Results area
    static const RECT screenDisplay = {960, 200, 1780, 950};
    RECT scaledDisplay;
    ScaleRect(originalScale, fullscreen, screenDisplay, scaledDisplay);

    m_log->SetWindow(scaledDisplay, false);
    m_log->SetViewport(viewport);

    m_ui->SetWindow(fullscreen);
}

void Sample::OnDeviceLost()
{
    m_ui->ReleaseDevice();
    m_graphicsMemory.reset();
    m_liveInfoHUD->ReleaseDevice();
    m_resourceDescriptors.reset();
}

void Sample::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

HRESULT Sample::InitializePlayFab()
{
    if (m_pfInitialized)
        return S_OK;

    // XGame runtime
    HRESULT hr = XGameRuntimeInitialize();
    if (FAILED(hr))
        return hr;

    // PF Core & Services
    RETURN_IF_FAILED(PFInitialize(nullptr));
    RETURN_IF_FAILED(PFServicesInitialize(nullptr));

    // Create service config (TODO: replace with your title values)
    RETURN_IF_FAILED(PFServiceConfigCreateHandle(
        "https://E18D7.playfabapi.com",
        "E18D7",
        &m_serviceConfig));

    // Dedicated queue for async work
    RETURN_IF_FAILED(XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &m_queue));

    m_pfInitialized = true;
    return S_OK;
}

HRESULT Sample::InitializeGameSaves()
{
    if (m_gsInitialized)
        return S_OK;

    PFGameSaveInitArgs args{};
    args.backgroundQueue = m_queue;
    args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    args.saveFolder = nullptr; // GDK ignores saveFolder; path is provided by platform
    RETURN_IF_FAILED(PFGameSaveFilesInitialize(&args));
    m_gsInitialized = true;
    return S_OK;
}

HRESULT Sample::SignInAndCreateLocalUser()
{
    if (m_localUserCreateInProgress)
    {
        return E_PENDING;
    }

    if (!m_liveResources->IsUserSignedIn())
    {
        // Surface sign-in start even when triggered via other actions (e.g., keyboard 'A')
        if (!m_signInInProgress)
        {
            m_signInInProgress = true;
            PostLog(L"Sign-in started");
        }
        m_liveResources->SignInWithUI();
        return E_PENDING;
    }

    m_localUserCreateInProgress = true;

    if (!m_xuser)
    {
        // Grab the XUser from LiveResources callback
        // LiveResources doesn't expose handle directly; request a new default user handle for PF
        XAsyncBlock async{};
        RETURN_IF_FAILED(XUserAddAsync(XUserAddOptions::AddDefaultUserAllowingUI, &async));
        RETURN_IF_FAILED(XAsyncGetStatus(&async, true));
        RETURN_IF_FAILED(XUserAddResult(&async, &m_xuser));
    }

    if (!m_localUser)
    {
        RETURN_IF_FAILED(PFLocalUserCreateHandleWithXboxUser(m_serviceConfig, m_xuser, nullptr, &m_localUser));
    }
    m_localUserCreateInProgress = false;
    PostLog(L"Created PFLocalUserHandle");
    return S_OK;
}

HRESULT Sample::AddUserToGameSaves()
{
    if (m_userAddedToGS)
        return S_OK;
    if (m_addUserToGSInProgress)
        return E_PENDING;
    if (!m_localUser)
    {
        RETURN_IF_FAILED(SignInAndCreateLocalUser());
    }
    if (!m_gsInitialized)
    {
        RETURN_IF_FAILED(InitializeGameSaves());
    }

    auto async = std::make_unique<XAsyncBlock>();
    async->queue = m_queue;
    async->context = this;
    async->callback = [](_Inout_ XAsyncBlock *a)
    {
        std::unique_ptr<XAsyncBlock> hold(a);
        auto self = static_cast<Sample *>(a->context);
        HRESULT hr = XAsyncGetStatus(a, false);
        if (SUCCEEDED(hr))
        {
            self->m_userAddedToGS = true;
            char folder[MAX_PATH] = {};
            if (SUCCEEDED(PFGameSaveFilesGetFolder(self->m_localUser, MAX_PATH, folder, nullptr)))
            {
                self->m_saveRoot = folder;
            }
            (void)PFGameSaveFilesGetRemainingQuota(self->m_localUser, &self->m_remainingQuota);
            self->PostLog(L"PFGameSaveFilesAddUserWithUiAsync completed");
            self->DoLoad();
        }
        else
        {
            self->PostLogf(L"Add user to Game Saves failed: 0x%08X", hr);
        }
        self->m_addUserToGSInProgress = false;
    };

    m_addUserToGSInProgress = true;
    PostLog(L"PFGameSaveFilesAddUserWithUiAsync started");
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(m_localUser, PFGameSaveFilesAddUserOptions::None, async.get());
    if (FAILED(hr))
        return hr;
    async.release();
    return S_OK;
}

void Sample::DoSave()
{
    if (!m_userAddedToGS || m_saveRoot.empty())
        return;
    const size_t fileSize = (size_t)(1 * 512 * 1024); // 512k
    std::vector<uint8_t> buf(fileSize);
    // Fill with random bytes
    std::uniform_int_distribution<int> dist(0, 255);
    for (size_t i = 0; i < fileSize; ++i)
    {
        buf[i] = static_cast<uint8_t>(dist(m_rng));
    }
    std::string path = m_saveRoot + "\\savegame.dat";
    std::ofstream f(path, std::ios::binary);
    if (f.is_open())
    {
        f.write(reinterpret_cast<const char *>(buf.data()), (std::streamsize)buf.size());
        // Log first 4 bytes of what we saved (in hex)
        PostLogf(L"Saved game data. Header: %02X %02X %02X %02X", buf[0], buf[1], buf[2], buf[3]);
    }
    else
    {
        PostLog(L"Save failed: could not open file for writing");
        return;
    }
}

void Sample::DoLoad()
{
    if (m_saveRoot.empty())
    {
        PostLog(L"Load failed: No save root yet");
        return;
    }
    std::string path = m_saveRoot + "\\savegame.dat";
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open())
    {
        PostLog(L"Load failed: file not found");
        return;
    }
    uint8_t header[4]{};
    f.read(reinterpret_cast<char *>(header), 4);
    if (f.gcount() == 4)
    {
        // Log first 4 bytes read (in hex)
        PostLogf(L"Loaded game data. Header: %02X %02X %02X %02X", header[0], header[1], header[2], header[3]);
    }
    else
    {
        PostLog(L"Load failed: file too small to read header");
    }
}

void Sample::DoDelete()
{
    if (m_saveRoot.empty())
        return;
    std::string path = m_saveRoot + "\\savegame.dat";
    DWORD attrs = GetFileAttributesA(path.c_str());
    if (attrs != INVALID_FILE_ATTRIBUTES)
    {
        if (DeleteFileA(path.c_str()))
        {
            PostLog(L"Deleted local save game file");
        }
        else
        {
            PostLog(L"Failed to delete local save game file");
        }
    }
    else
    {
        PostLog(L"Local save game file not found");
    }
}

HRESULT Sample::DoUpload(bool keepDeviceActive)
{
    if (!m_userAddedToGS)
        return E_FAIL;
    auto async = std::make_unique<XAsyncBlock>();
    async->queue = m_queue;
    async->context = this;
    async->callback = [](_Inout_ XAsyncBlock *a)
    {
        std::unique_ptr<XAsyncBlock> hold(a);
        auto self = static_cast<Sample *>(a->context);
        const HRESULT hr = XAsyncGetStatus(a, false);
        if (SUCCEEDED(hr))
        {
            self->PostLog(L"PFGameSaveFilesUploadWithUiAsync completed");
        }
        else
        {
            self->PostLogf(L"PFGameSaveFilesUploadWithUiAsync failed: 0x%08X", hr);
        }
    };
    auto opt = keepDeviceActive ? PFGameSaveFilesUploadOption::KeepDeviceActive : PFGameSaveFilesUploadOption::ReleaseDeviceAsActive;
    PostLog(keepDeviceActive ? L"PFGameSaveFilesUploadWithUiAsync started (keep active)" : L"PFGameSaveFilesUploadWithUiAsync started (release device)");
    HRESULT hr = PFGameSaveFilesUploadWithUiAsync(m_localUser, opt, async.get());
    if (FAILED(hr))
        return hr;
    async.release();
    return S_OK;
}

void Sample::DoSetSaveDescription()
{
    // Require a valid local save root to proceed (user added and folder retrieved)
    if (!m_userAddedToGS || m_saveRoot.empty())
    {
        PostLog(L"SetSaveDescription failed: No local save root yet");
        return;
    }

    // Create a simple short description; include a timestamp so it changes between calls
    wchar_t wbuf[128]{};
    SYSTEMTIME st{};
    GetLocalTime(&st);
    swprintf_s(wbuf, L"Saved %04u-%02u-%02u %02u:%02u:%02u", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    // Convert to UTF-8 for API
    char utf8[256]{};
    WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, utf8, static_cast<int>(sizeof(utf8)), nullptr, nullptr);

    auto async = std::make_unique<XAsyncBlock>();
    async->queue = m_queue;
    async->context = this;
    async->callback = [](_Inout_ XAsyncBlock* a)
    {
        std::unique_ptr<XAsyncBlock> hold(a);
        auto self = static_cast<Sample*>(a->context);
        HRESULT hr = XAsyncGetStatus(a, false);
        if (SUCCEEDED(hr))
        {
            // Retrieve the result per API contract
            (void)PFGameSaveFilesSetSaveDescriptionResult(a);
            self->PostLog(L"PFGameSaveFilesSetSaveDescriptionAsync completed");
        }
        else
        {
            self->PostLogf(L"PFGameSaveFilesSetSaveDescriptionAsync failed: 0x%08X", hr);
        }
    };

    PostLog(L"PFGameSaveFilesSetSaveDescriptionAsync started");
    HRESULT hr = PFGameSaveFilesSetSaveDescriptionAsync(m_localUser, utf8, async.get());
    if (FAILED(hr))
    {
        PostLogf(L"PFGameSaveFilesSetSaveDescriptionAsync start failed: 0x%08X", hr);
        return;
    }
    (void)async.release();
}
#pragma endregion

// Copyright (C) Microsoft Corporation. All rights reserved.
// Main_Xbox.cpp - Entry point for GameTestAppXbox
//
// Based on PlayFabGameSaveSample-XboxConsole/Main.cpp
#include "pch.h"
#include "TestDeviceApp.h"
#include "DeviceGameSaveState.h"
#include "DeviceGameSave.h"
#include "DeviceLogging.h"

#include "ATGTelemetry.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
    std::unique_ptr<TestDeviceApp> g_app;
#ifdef _GAMING_XBOX
    HANDLE g_plmSuspendComplete = nullptr;
    HANDLE g_plmSignalResume = nullptr;
#endif
}

// Unhandled exception filter to log crash info before terminating
static LONG WINAPI CrashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
    const DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
    const void* exceptionAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;
    
    LogToWindowFormat("[CRASH] Unhandled exception 0x%08X at address %p", exceptionCode, exceptionAddress);
    
    // Log exception-specific info
    switch (exceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            LogToWindow("[CRASH] Access violation");
            break;
        case EXCEPTION_STACK_OVERFLOW:
            LogToWindow("[CRASH] Stack overflow");
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            LogToWindow("[CRASH] Integer divide by zero");
            break;
        default:
            break;
    }
    
    // Flush logs
    FlushDeviceLogFile();
    
    // Let the process terminate without showing a dialog
    return EXCEPTION_EXECUTE_HANDLER;
}

// Configure error handling for headless/automated operation
// Note: Xbox/GDK doesn't support SetErrorMode - crash dialogs are handled differently
static void ConfigureHeadlessErrorHandling()
{
    // Set unhandled exception filter to log crashes
    SetUnhandledExceptionFilter(CrashHandler);
    
#ifdef _DEBUG
    // Redirect CRT assertions to debug output instead of dialog box
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    
    // Disable abort() message box
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
}

LPCWSTR g_szAppName = L"GameTestAppXbox";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ExitGame() noexcept
{
    PostQuitMessage(0);
}

int SampleMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // Configure error handling FIRST before any other code runs
    ConfigureHeadlessErrorHandling();

    if (!XMVerifyCPUSupport())
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: This hardware does not support the required instruction set.\n");
#if defined(_GAMING_XBOX) && defined(__AVX2__)
        OutputDebugStringA("This may indicate a Gaming.Xbox.Scarlett.x64 binary is being run on an Xbox One.\n");
#endif
#endif
        return 1;
    }

    if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))
        return 1;

#ifdef _GAMING_DESKTOP
    char dir[_MAX_PATH] = {};
    if (GetModuleFileNameA(nullptr, dir, _MAX_PATH) > 0)
    {
        std::string exe = dir;
        exe = exe.substr(0, exe.find_last_of("\\"));
        std::ignore = SetCurrentDirectoryA(exe.c_str());
    }
#endif

    HRESULT hr = XGameRuntimeInitialize();
    if (FAILED(hr))
    {
        if (hr == E_GAMERUNTIME_DLL_NOT_FOUND || hr == E_GAMERUNTIME_VERSION_MISMATCH)
        {
#ifdef _GAMING_DESKTOP
            std::ignore = MessageBoxW(nullptr, L"Game Runtime is not installed on this system or needs updating.", g_szAppName, MB_ICONERROR | MB_OK);
#endif
        }
        return 1;
    }

#ifdef _GAMING_XBOX
    SetThreadAffinityMask(GetCurrentThread(), 0x1);
#endif

    g_app = std::make_unique<TestDeviceApp>();

#ifdef _GAMING_XBOX
    PAPPSTATE_REGISTRATION hPLM = {};
#endif
    {
        WNDCLASSEXW wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszClassName = L"GameTestAppXboxWindowClass";
        if (!RegisterClassExW(&wcex))
            return 1;

#ifdef _GAMING_XBOX
        RECT rc = { 0, 0, 1920, 1080 };
#else
        int w, h;
        g_app->GetDefaultSize(w, h);

        RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
#endif

        HWND hwnd = CreateWindowExW(0, L"GameTestAppXboxWindowClass", g_szAppName, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
            nullptr, nullptr, hInstance,
            g_app.get());
        if (!hwnd)
            return 1;

        ShowWindow(hwnd, nCmdShow);

        ATG::SendLaunchTelemetry();

        GetClientRect(hwnd, &rc);

        g_app->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top, lpCmdLine);

#ifdef _GAMING_XBOX
        g_plmSuspendComplete = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
        g_plmSignalResume = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
        if (!g_plmSuspendComplete || !g_plmSignalResume)
            return 1;

        if (RegisterAppStateChangeNotification([](BOOLEAN quiesced, PVOID context)
            {
                if (quiesced)
                {
                    ResetEvent(g_plmSuspendComplete);
                    ResetEvent(g_plmSignalResume);
                    PostMessage(reinterpret_cast<HWND>(context), WM_USER, 0, 0);
                    std::ignore = WaitForSingleObject(g_plmSuspendComplete, INFINITE);
                }
                else
                {
                    SetEvent(g_plmSignalResume);
                }
            }, hwnd, &hPLM))
            return 1;
#endif
    }

    MSG msg = {};
    OutputDebugStringA("INFO: GameTestAppXbox started.\n");
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_app->Tick();
        }
    }

    // Cleanup before shutdown
    DeviceGameSaveState* state = GetSampleGameSaveState();
    Sample_GameSave_Cleanup(state);

    LogToWindow("Game Test Device [Xbox] shutting down...");

    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
    }

    g_app.reset();

#ifdef _GAMING_XBOX
    UnregisterAppStateChangeNotification(hPLM);

    CloseHandle(g_plmSuspendComplete);
    CloseHandle(g_plmSignalResume);
#endif

    XGameRuntimeUninitialize();

    CoUninitialize();

    return static_cast<int>(msg.wParam);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        return SampleMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    }
    catch (const std::exception& e)
    {
        OutputDebugStringA("*** ERROR: Unhandled C++ exception thrown: ");
        OutputDebugStringA(e.what());
        OutputDebugStringA(" *** \n");
        return 1;
    }
    catch (...)
    {
        OutputDebugStringA("*** ERROR: Unknown unhandled C++ exception thrown ***\n");
        return 1;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _GAMING_DESKTOP
    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    static bool s_fullscreen = false;
#endif

    auto app = reinterpret_cast<TestDeviceApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
        if (lParam)
        {
            auto params = reinterpret_cast<LPCREATESTRUCTW>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(params->lpCreateParams));
        }
        break;

    case WM_ACTIVATEAPP:
        if (app)
        {
            Keyboard::ProcessMessage(message, wParam, lParam);

            if (wParam)
            {
                app->OnActivated();
            }
            else
            {
                app->OnDeactivated();
            }
        }
        break;

    case WM_ACTIVATE:
        Keyboard::ProcessMessage(message, wParam, lParam);
        break;

#ifdef _GAMING_XBOX
    case WM_USER:
        if (app)
        {
            app->OnSuspending();
            SetEvent(g_plmSuspendComplete);
            std::ignore = WaitForSingleObject(g_plmSignalResume, INFINITE);
            app->OnResuming();
        }
        break;
#else
    case WM_PAINT:
        if (s_in_sizemove && app)
        {
            app->Tick();
        }
        else
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_MOVE:
        if (app)
        {
            app->OnWindowMoved();
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend && app)
                    app->OnSuspending();
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend && app)
                app->OnResuming();
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && app)
        {
            app->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (app)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend && app)
                app->OnSuspending();
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend && app)
                    app->OnResuming();
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        Keyboard::ProcessMessage(message, wParam, lParam);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                int width = 800;
                int height = 600;
                if (app)
                    app->GetDefaultSize(width, height);

                ShowWindow(hWnd, SW_SHOWNORMAL);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }
        Keyboard::ProcessMessage(message, wParam, lParam);
        break;

    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
#endif
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

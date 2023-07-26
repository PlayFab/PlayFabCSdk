//
// Main.cpp
//

#include "TestAppPch.h"
#include <XGameRuntimeInit.h>
#include <XDisplay.h>
#include "Keyboard.h"

#if !defined(_GAMING_DESKTOP)
#include "GameXbox.h"

#include <appnotify.h>

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
    std::unique_ptr<Game> g_game;
    HANDLE g_plmSuspendComplete = nullptr;
    HANDLE g_plmSignalResume = nullptr;
}

bool g_HDRMode = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetDisplayMode() noexcept;
void ExitGame() noexcept;

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!XMVerifyCPUSupport())
    {
#ifdef _DEBUG
        OutputDebugStringA("ERROR: This hardware does not support the required instruction set.\n");
#ifdef __AVX2__
        OutputDebugStringA("This may indicate a Gaming.Xbox.Scarlett.x64 binary is being run on an Xbox One.\n");
#endif
#endif
        return 1;
    }

    if (FAILED(XGameRuntimeInitialize()))
        return 1;

    // Microsoft GDKX supports UTF-8 everywhere
    assert(GetACP() == CP_UTF8);

    g_game = std::make_unique<Game>();

    // Register class and create window
    PAPPSTATE_REGISTRATION hPLM = {};
    PAPPCONSTRAIN_REGISTRATION hPLM2 = {};

    {
        // Register class
        WNDCLASSEXA wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXA);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = hInstance;
        wcex.lpszClassName = u8"PFTestAppWindowClass";
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        if (!RegisterClassExA(&wcex))
            return 1;

        // Create window
        HWND hwnd = CreateWindowExA(0, u8"PFTestAppWindowClass", u8"PFTestApp", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, nullptr, nullptr, hInstance,
            nullptr);
        if (!hwnd)
            return 1;

        ShowWindow(hwnd, nCmdShow);

        SetDisplayMode();

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()));

        g_game->Initialize(hwnd);

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

                // To ensure we use the main UI thread to process the notification, we self-post a message
                PostMessage(reinterpret_cast<HWND>(context), WM_USER, 0, 0);

                // To defer suspend, you must wait to exit this callback
                std::ignore = WaitForSingleObject(g_plmSuspendComplete, INFINITE);
            }
            else
            {
                SetEvent(g_plmSignalResume);
            }
        }, hwnd, &hPLM))
            return 1;

        if (RegisterAppConstrainedChangeNotification([](BOOLEAN constrained, PVOID context)
        {
            // To ensure we use the main UI thread to process the notification, we self-post a message
            SendMessage(reinterpret_cast<HWND>(context), WM_USER + 1, (constrained) ? 1u : 0u, 0);
        }, hwnd, &hPLM2))
            return 1;
    }

    // Main message loop
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_game->Tick();
        }
    }

    g_game.reset();

    UnregisterAppStateChangeNotification(hPLM);
    UnregisterAppConstrainedChangeNotification(hPLM2);

    CloseHandle(g_plmSuspendComplete);
    CloseHandle(g_plmSignalResume);

    XGameRuntimeUninitialize();

    return static_cast<int>(msg.wParam);
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
        Keyboard::ProcessMessage(message, wParam, lParam);
        break;

    case WM_USER:
        if (game)
        {
            game->OnSuspending();

            // Complete deferral
            SetEvent(g_plmSuspendComplete);

            std::ignore = WaitForSingleObject(g_plmSignalResume, INFINITE);

            SetDisplayMode();

            game->OnResuming();
        }
        break;

    case WM_USER + 1:
        if (game)
        {
            if (wParam)
            {
                game->OnConstrained();
            }
            else
            {
                SetDisplayMode();

                game->OnUnConstrained();
            }
        }
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// HDR helper
void SetDisplayMode() noexcept
{
    if (g_game && g_game->RequestHDRMode())
    {
        // Request HDR mode.
        auto result = XDisplayTryEnableHdrMode(XDisplayHdrModePreference::PreferHdr, nullptr);

        g_HDRMode = (result == XDisplayHdrModeResult::Enabled);

#ifdef _DEBUG
        OutputDebugStringA((g_HDRMode) ? "INFO: Display in HDR Mode\n" : "INFO: Display in SDR Mode\n");
#endif
    }
}

// Exit helper
void ExitGame() noexcept
{
    PostQuitMessage(0);
}

#endif // !defined(_GAMING_DESKTOP)
// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "Common/DeviceApplication.h"
#include "Common/DeviceLogging.h"
#include "Common/DeviceWebSocketConnection.h"
#include "DeviceWindow.h"
#include "Common/DeviceGameSaveState.h"
#include "Common/DeviceGameSave.h"
#include "Common/DeviceCommandProcessor.h"

#include <chrono>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

int RunDeviceApplication(HINSTANCE instance, LPSTR commandLine, int showCommand)
{
    UNREFERENCED_PARAMETER(commandLine);

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    InitializeHCTraceToVerboseLog();

#ifdef _DEBUG
    const char* buildConfig = "Debug";
#else
    const char* buildConfig = "Release";
#endif

    LogToWindowFormat("PlayFab Game Save Test [%s] starting...", buildConfig);

    DeviceGameSaveState* state = GetSampleGameSaveState();
    SetSampleDeviceEngineType(DetectSampleDeviceEngineType());

    HRESULT hr = XGameRuntimeInitialize();
    LogToWindowFormat("XGameRuntimeInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    if (FAILED(hr))
    {
        if (hr == E_GAMERUNTIME_DLL_NOT_FOUND || hr == E_GAMERUNTIME_VERSION_MISMATCH)
        {
            LogToWindow("Game Runtime is not installed on this system");
        }

        return 1;
    }

    HRESULT wsInitHr = state->websocketClient.Initialize();
    LogToWindowFormat("DeviceWebSocketClient init (hr=0x%08X)", static_cast<uint32_t>(wsInitHr));
    Sample_GameSave_ConfigureWebSocketLogging(state);

    if (!Sample_GameSave_ParseArgs(commandLine, state))
    {
        return 1;
    }

    hr = XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &state->taskQueue);
    if (FAILED(hr))
    {
        LogToWindow("Failed to create task queue");
        return 1;
    }

    if (state->taskQueue != nullptr)
    {
        XTaskQueueSetCurrentProcessTaskQueue(state->taskQueue);
        LogToWindow("XTaskQueueSetCurrentProcessTaskQueue invoked");
    }

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DeviceWindowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"PlayFabGameSaveWindowClass";
    RegisterClassEx(&wc);

#ifdef _DEBUG
    const wchar_t* windowTitle = L"PlayFab Game Save Test [Debug]";
#else
    const wchar_t* windowTitle = L"PlayFab Game Save Test [Release]";
#endif

    HWND hwnd = CreateWindowEx(
        0,
        L"PlayFabGameSaveWindowClass",
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (hwnd == nullptr)
    {
        LogToWindow("Failed to create window");
        return 1;
    }

    ShowWindow(hwnd, showCommand);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (!GetSampleGameSaveState()->quit)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                GetSampleGameSaveState()->quit = true;
                break;
            }
        }

        if (!GetSampleGameSaveState()->quit)
        {
            // Placeholder for future update loop work.
        }

        PumpWebSocketAutoConnect(state);
    }

    Sample_GameSave_Cleanup(state);

    LogToWindow("PlayFab Game Save Test shutting down...");

    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
    }

    Sleep(2000);
    XGameRuntimeUninitialize();

    return 0;
}

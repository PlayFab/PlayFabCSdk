// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "DeviceApplication.h"
#include "DeviceLogging.h"
#include "DeviceWebSocketConnection.h"
#include "DeviceWindow.h"
#include "DeviceGameSaveState.h"
#include "DeviceGameSave.h"
#include "DeviceCommandProcessor.h"
#include "libHttpClient/HCGlobalHandlers.h"

#include <chrono>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

// For stack trace capture
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

// Capture and log a stack trace
static void LogStackTrace(CONTEXT* context)
{
    // Initialize symbol handler
    HANDLE process = GetCurrentProcess();
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    
    if (!SymInitialize(process, NULL, TRUE))
    {
        LogToWindowFormat("[CRASH] SymInitialize failed: 0x%08X", GetLastError());
        return;
    }

    LogToWindow("[CRASH] Stack trace:");

    // Set up stack frame
    STACKFRAME64 stackFrame = {};
    DWORD machineType;

#ifdef _M_X64
    machineType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = context->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#else
    machineType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = context->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

    // Buffer for symbol info
    char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuffer;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    // Walk the stack
    const int maxFrames = 32;
    for (int frameNum = 0; frameNum < maxFrames; frameNum++)
    {
        if (!StackWalk64(machineType, process, GetCurrentThread(), &stackFrame, context,
            NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            break;
        }

        if (stackFrame.AddrPC.Offset == 0)
        {
            break;
        }

        DWORD64 address = stackFrame.AddrPC.Offset;
        DWORD64 displacement = 0;

        // Try to get symbol name
        if (SymFromAddr(process, address, &displacement, symbol))
        {
            // Try to get source file and line number
            IMAGEHLP_LINE64 line = {};
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD lineDisplacement = 0;

            if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line))
            {
                LogToWindowFormat("[CRASH]   [%2d] %s+0x%llX (%s:%lu)",
                    frameNum, symbol->Name, displacement, line.FileName, line.LineNumber);
            }
            else
            {
                LogToWindowFormat("[CRASH]   [%2d] %s+0x%llX (0x%llX)",
                    frameNum, symbol->Name, displacement, address);
            }
        }
        else
        {
            // Couldn't resolve symbol, just log the address
            LogToWindowFormat("[CRASH]   [%2d] 0x%llX", frameNum, address);
        }
    }

    SymCleanup(process);
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
            if (exceptionInfo->ExceptionRecord->NumberParameters >= 2)
            {
                ULONG_PTR accessType = exceptionInfo->ExceptionRecord->ExceptionInformation[0];
                ULONG_PTR accessAddress = exceptionInfo->ExceptionRecord->ExceptionInformation[1];
                LogToWindowFormat("[CRASH] %s address 0x%p",
                    accessType == 0 ? "Reading from" : (accessType == 1 ? "Writing to" : "Executing"),
                    (void*)accessAddress);
            }
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
    
    // Log the stack trace
    LogStackTrace(exceptionInfo->ContextRecord);
    
    // Flush logs
    FlushDeviceLogFile();
    
    // Let the process terminate without showing a dialog
    return EXCEPTION_EXECUTE_HANDLER;
}

// Configure error handling for headless/automated operation
static void ConfigureHeadlessErrorHandling()
{
    // Suppress Windows Error Reporting dialogs
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    
    // Set unhandled exception filter to log crashes
    SetUnhandledExceptionFilter(CrashHandler);
    
#ifdef _DEBUG
    // Redirect CRT assertions to stderr instead of dialog box
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
    
    // Disable abort() message box
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
}

int RunDeviceApplication(HINSTANCE instance, LPSTR commandLine, int showCommand)
{
    UNREFERENCED_PARAMETER(commandLine);

    // Configure error handling FIRST before any other code runs
    ConfigureHeadlessErrorHandling();

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    InitializeHCTraceToVerboseLog();

#ifdef _DEBUG
    const char* buildConfig = "Debug";
#else
    const char* buildConfig = "Release";
#endif

    LogToWindowFormat("Game Test [%s] starting...", buildConfig);

    DeviceGameSaveState* state = GetSampleGameSaveState();
    SetSampleDeviceEngineType(DetectSampleDeviceEngineType());

    // Install custom memory hooks before any HCInitialize call so that
    // libHttpClient routes all allocations through our counters.
    HRESULT hr = HCMemSetFunctions(CustomHCMemAlloc, CustomHCMemFree);
    LogToWindowFormat("HCMemSetFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));

    hr = XGameRuntimeInitialize();
    LogToWindowFormat("XGameRuntimeInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    if (FAILED(hr))
    {
        if (hr == E_GAMERUNTIME_DLL_NOT_FOUND || hr == E_GAMERUNTIME_VERSION_MISMATCH)
        {
            LogToWindow("Game Runtime is not installed on this system");
        }

        return 1;
    }

    // Register XUserPlatformRemoteConnect handlers immediately after runtime init.
    // On PC, the GRTS overlay may not inject into loose-layout registered apps,
    // which prevents the native Xbox account picker from appearing. Remote Connect
    // provides a web-based sign-in fallback: the GDK provides a URL + code that
    // the user enters in a browser to authenticate.
    {
        XUserPlatformRemoteConnectEventHandlers remoteConnect{};
        remoteConnect.context = nullptr;
        remoteConnect.show = [](_In_opt_ void* /*context*/, _In_ uint32_t /*userIdentifier*/,
            _In_ XUserPlatformOperation /*operation*/, _In_z_ char const* url,
            _In_z_ char const* code, _In_ size_t /*codeSize*/, _In_reads_bytes_(0) void const* /*codeChallenge*/)
        {
            LogToWindowFormat("XUser Remote Connect: Open %s and enter code: %s", url, code);
        };
        remoteConnect.close = [](_In_opt_ void* /*context*/, _In_ uint32_t userIdentifier,
            _In_ XUserPlatformOperation /*operation*/)
        {
            LogToWindowFormat("XUser Remote Connect: Sign-in completed (userIdentifier=%u)", userIdentifier);
        };
        HRESULT rcHr = XUserPlatformRemoteConnectSetEventHandlers(nullptr, &remoteConnect);
        LogToWindowFormat("XUserPlatformRemoteConnectSetEventHandlers (hr=0x%08X)", static_cast<uint32_t>(rcHr));
    }

    // Log and validate sandbox AFTER runtime init — GDK APIs require XGameRuntimeInitialize first
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
            LogToWindowFormat("WARNING: Failed to query sandbox (hr=0x%08X) — is GDK Gaming Runtime installed?", static_cast<uint32_t>(sbHr));
        }
    }

    // On GDK, libHttpClient gates HTTP/WebSocket on XNetworking connectivity.
    // When running with ForceUseLocalServices (CI agents), the connectivity hint
    // callback may never fire, leaving the network gate closed.  Poll briefly so
    // that the in-proc xgameruntime has time to report network-ready.
#if defined(_GAMING_DESKTOP)
    if (XGameRuntimeIsFeatureAvailable(XGameRuntimeFeature::XNetworking))
    {
        LogToWindow("Waiting for XNetworking to report network ready...");
        constexpr int kMaxNetworkWaitMs = 5000;
        constexpr int kPollIntervalMs = 100;
        int waited = 0;
        bool networkReady = false;
        while (waited < kMaxNetworkWaitMs)
        {
            XNetworkingConnectivityHint hint{};
            HRESULT netHr = XNetworkingGetConnectivityHint(&hint);
            if (SUCCEEDED(netHr) && hint.networkInitialized)
            {
                networkReady = true;
                break;
            }
            Sleep(kPollIntervalMs);
            waited += kPollIntervalMs;
        }
        LogToWindowFormat("XNetworking ready=%d after %dms", networkReady, waited);
        if (!networkReady)
        {
            LogToWindow("WARNING: XNetworking never reported ready — WebSocket connections may fail");
        }
    }
#endif

    HRESULT wsInitHr = state->websocketClient.Initialize();
    LogToWindowFormat("DeviceWebSocketClient init (hr=0x%08X)", static_cast<uint32_t>(wsInitHr));
    Sample_GameSave_ConfigureWebSocketLogging(state);

    if (!Sample_GameSave_ParseArgs(commandLine, state))
    {
        return 1;
    }

    // Override engine type if /forceinproc was passed on the command line.
    // DetectSampleDeviceEngineType() runs before arg parsing, so we fix up here.
    if (state->forceInproc)
    {
        SetSampleDeviceEngineType(DeviceEngineType::PcInprocGameSaves);
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
    const wchar_t* windowTitle = L"Game Test [Debug]";
#else
    const wchar_t* windowTitle = L"Game Test [Release]";
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

    LogToWindow("Game Test shutting down...");

    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
    }

    XGameRuntimeUninitialize();

    return 0;
}

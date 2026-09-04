// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "DeviceGameSave.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpclient/httpClient.h>
#include <fstream>
#include <sstream>

HC_DEFINE_TRACE_AREA(Sample, HCTraceLevel::Verbose);
HC_DECLARE_TRACE_AREA(Sample);

namespace
{
[[maybe_unused]] void MyDebugTrace(
    _In_z_ const char* areaName,
    _In_ HCTraceLevel level,
    _In_ uint64_t threadId,
    _In_ uint64_t timestamp,
    _In_z_ const char* message)
{
    UNREFERENCED_PARAMETER(level);
    UNREFERENCED_PARAMETER(threadId);
    UNREFERENCED_PARAMETER(timestamp);
    LogToWindow(std::string("[") + areaName + "] " + message);
}

std::vector<std::string> SplitStringBySpace(const std::string& input)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}
}

HRESULT Sample_GameSave_Cleanup(DeviceGameSaveState* state)
{
    LogToWindow("[Lifecycle] Sample_GameSave_Cleanup called — tearing down");
    state->websocketClient.Cleanup();
    state->websocketConnectInProgress.store(false);
    state->websocketLastAttempt = {};

    if (state->localUserHandle)
    {
        PFLocalUserCloseHandle(state->localUserHandle);
        state->localUserHandle = nullptr;
    }

    if (state->serviceConfigHandle)
    {
        PFServiceConfigCloseHandle(state->serviceConfigHandle);
        state->serviceConfigHandle = nullptr;
    }

    if (!state->statusFilePath.empty())
    {
        std::remove(state->statusFilePath.c_str());
    }

    XAsyncBlock async1{};
    HRESULT hr = PFGameSaveFilesUninitializeAsync(&async1);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async1, true);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    hr = PFGameSaveFilesUninitializeResult(&async1);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeResult");
    RETURN_IF_FAILED(hr);

    XAsyncBlock async2{};
    hr = PFServicesUninitializeAsync(&async2);
    HC_TRACE_WARNING_HR(Sample, hr, "PFServicesUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async2, true);
    HC_TRACE_WARNING_HR(Sample, hr, "PFServicesUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    XAsyncBlock async3{};
    hr = PFUninitializeAsync(&async3);
    HC_TRACE_WARNING_HR(Sample, hr, "PFUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async3, true);
    HC_TRACE_WARNING_HR(Sample, hr, "PFUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
        state->taskQueueOwnedByCommand = false;
    }

#ifdef _WIN32
    if (state->xuser)
    {
        XUserCloseHandle(state->xuser);
        state->xuser = nullptr;
    }
#endif

    return S_OK;
}

bool Sample_GameSave_ParseArgs(_In_z_ _Printf_format_string_ const char* cmdLineStr, DeviceGameSaveState* gameState)
{
    std::string cmdLine = cmdLineStr;
    std::vector<std::string> argv = SplitStringBySpace(cmdLine);
    int argc = static_cast<int>(argv.size());

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i].c_str(), "/controller") == 0)
        {
            if (i + 1 >= argc)
            {
                LogToWindow("Missing value for argument: /controller");

                std::string usage =
                    "Usage: game.exe [options]\r\n"
                    "  /controller [ip:port]  WebSocket controller address\r\n"
                    "  /forceinproc           Force in-process (Win32) provider\r\n";

                LogToWindow(usage);
                return false;
            }

            gameState->controllerIpAddress = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/forceinproc") == 0)
        {
            gameState->forceInproc = true;
            LogToWindow("Force in-proc mode enabled via /forceinproc");
        }
        else
        {
            // Ignore unknown arguments (e.g., /notinteractive from pipeline)
            LogToWindow("Ignoring unknown argument: " + argv[i]);
        }
    }

    // If no /controller arg was provided, try reading from controllerip.txt.
    // This is useful on Xbox console where passing command-line params is difficult.
    if (gameState->controllerIpAddress == "localhost")
    {
        std::ifstream file("controllerip.txt");
        if (file.is_open())
        {
            std::string ip;
            if (std::getline(file, ip))
            {
                // Trim whitespace
                while (!ip.empty() && (ip.back() == ' ' || ip.back() == '\t' || ip.back() == '\r' || ip.back() == '\n'))
                {
                    ip.pop_back();
                }
                while (!ip.empty() && (ip.front() == ' ' || ip.front() == '\t'))
                {
                    ip.erase(ip.begin());
                }

                if (!ip.empty())
                {
                    gameState->controllerIpAddress = ip;
                    LogToWindow("Controller IP from controllerip.txt: " + ip);
                }
            }
        }
    }

    return true;
}

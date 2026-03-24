// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "DeviceGameSave.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpclient/httpClient.h>
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
        if (strcmp(argv[i].c_str(), "/gdktestapp") == 0)
        {
        }
        else if (strcmp(argv[i].c_str(), "/v") == 0)
        {
            gameState->verboseLogs = true;
        }
        else if (strcmp(argv[i].c_str(), "/setui") == 0)
        {
            gameState->setUICallBacks = true;
        }
        else if (strcmp(argv[i].c_str(), "/p") == 0)
        {
            gameState->inputSaveFolder = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/u") == 0)
        {
            gameState->inputCustomUserId = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/linkcustomidwithxbox") == 0)
        {
            gameState->linkCustomIdWithXbox = true;
        }
        else if (strcmp(argv[i].c_str(), "/statusfile") == 0)
        {
            gameState->statusFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/pathfile") == 0)
        {
            gameState->pathFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/xuidfile") == 0)
        {
            gameState->xuidFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/waitforackat") == 0)
        {
            gameState->waitForAckAt = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/debugbreakat") == 0)
        {
            gameState->debugBreakAt = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/controller") == 0)
        {
            gameState->controllerIpAddress = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/idonly") == 0)
        {
            gameState->idOnly = true;
        }
        else if (strcmp(argv[i].c_str(), "/notinteractive") == 0)
        {
            gameState->interactive = false;
        }
        else if (strcmp(argv[i].c_str(), "/activedevicepromptsynclast") == 0)
        {
            gameState->activeDeviceContentionSyncLast = true;
        }
        else if (strcmp(argv[i].c_str(), "/keepdeviceactive") == 0)
        {
            gameState->keepDeviceActive = true;
        }
        else if (strcmp(argv[i].c_str(), "/takelocal") == 0)
        {
            gameState->takeLocal = true;
        }
        else if (strcmp(argv[i].c_str(), "/takeremote") == 0)
        {
            gameState->takeRemote = true;
        }
        else
        {
            LogToWindow("Unknown argument: " + argv[i]);

            std::string usage =
                "Usage: game.exe [see other options below]\r\n"
                "If /v is given then show verbose logs\r\n"
                "If /p [path] is given then it will use the path as the save folder.  Only applies when using inproc client\r\n"
                "If /statusfile [file] is given then it will create the file while the device is active and delete it upon exit\r\n"
                "If /pathfile [file] is given then it will create the file containing the save game root\r\n"
                "If /xuidfile [file] is given then it will create the file containing the xuid\r\n"
                "If /waitforackat [act list] is given then it will wait for the ack at the given list\r\n"
                "If /debugbreakat [act list] is given then it will debug break at the given list\r\n"
                "If /u is given, its the custom ID to use\r\n"
                "If /linkcustomidwithxbox is given, then it will link the customId with the Xbox account\r\n";
            usage += "If /idonly is given, then it write the xuid file and exit\r\n";
            usage += "If /interactive is given, then it turn on interactive mode\r\n";
            usage += "If /setui is given, then it will set the UI callbacks\r\n";
            usage += "If /takeLocal is given then it will take local if there's a conflict\r\n";
            usage += "If /takeRemote is given then it will take remote if there's a conflict\r\n";
            usage += "If /keepdeviceactive is given then it will keep the device active when in /ioff mode\r\n";

            LogToWindow(usage);
            return false;
        }
    }

    return true;
}

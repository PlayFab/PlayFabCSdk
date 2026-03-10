// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

HRESULT DownloadFile(const std::string& url, const std::string& fileName)
{
    // Trivial example download using LHC
    HCCallHandle call = nullptr;
    RETURN_IF_FAILED(HCHttpCallCreate(&call));
    RETURN_IF_FAILED(HCHttpCallRequestSetUrl(call, "GET", url.c_str()));
    XAsyncBlock async{};
    RETURN_IF_FAILED(HCHttpCallPerformAsync(call, &async));
    RETURN_IF_FAILED(XAsyncGetStatus(&async, true));
    HRESULT networkErrorCode = S_OK;
    uint32_t platErrCode = 0;
    uint32_t statusCode = 0;
    RETURN_IF_FAILED(HCHttpCallResponseGetNetworkErrorCode(call, &networkErrorCode, &platErrCode));
    RETURN_IF_FAILED(HCHttpCallResponseGetStatusCode(call, &statusCode));
    if (FAILED(networkErrorCode))
    {
        return networkErrorCode;
    }
    if (statusCode != 200)
    {
        return E_FAIL;
    }
    size_t bufferSize = 0;
    RETURN_IF_FAILED(HCHttpCallResponseGetResponseBodyBytesSize(call, &bufferSize));
    std::vector<uint8_t> buffer(bufferSize);
    size_t bufferUsed = 0;
    RETURN_IF_FAILED(HCHttpCallResponseGetResponseBodyBytes(call, bufferSize, buffer.data(), &bufferUsed));
    HANDLE hFile = CreateFileA(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bufferWritten = 0;
    WriteFile(hFile, buffer.data(), (DWORD)bufferUsed, &bufferWritten, NULL);
    CloseHandle(hFile);
    HCHttpCallCloseHandle(call);
    return S_OK;
}

void LogDescriptor(PFGameSaveDescriptor* descriptor)
{
    char gameTime[80]{ 0 };
#ifdef _WIN32
    struct tm localTime;
    if (localtime_s(&localTime, &descriptor->time) == 0) {
        strftime(gameTime, sizeof(gameTime), "%Y-%m-%d %H:%M:%S", &localTime);
    }
#else
    struct tm* localTime = localtime(&descriptor->time);
    strftime(gameTime, sizeof(gameTime), "%Y-%m-%d %H:%M:%S", localTime);
#endif
    std::cout << "GameSave Metadata:" << std::endl;
    std::cout << "  Time: " << gameTime << std::endl;
    std::cout << "  Total Size: " << descriptor->totalBytes << std::endl;
    std::cout << "  Uploaded Size: " << descriptor->uploadedBytes << std::endl;
    std::cout << "  Device Type: " << descriptor->deviceType << std::endl;
    std::cout << "  Device ID: " << descriptor->deviceId << std::endl;
    std::cout << "  Device Name: " << descriptor->deviceFriendlyName << std::endl;
    std::cout << "  Description: " << descriptor->shortSaveDescription << std::endl;

    std::string thumbnail = descriptor->thumbnailUri;
    if (thumbnail.find("http") != std::string::npos)
    {
        // Download the thumbnail
        std::vector<char> tempPath(MAX_PATH);
        GetTempPathA(MAX_PATH, tempPath.data());
        std::string fileName = tempPath.data();
        fileName += "\\thumbnail.png";

        HRESULT hr = DownloadFile(thumbnail, fileName);
        if (SUCCEEDED(hr))
        {
            std::cout << "  Thumbnail File: " << fileName << std::endl;
        }
        else
        {
            std::cout << "  Thumbnail URL: " << thumbnail << std::endl;
        }
    }
    else
    {
        std::cout << "  Thumbnail File: " << descriptor->thumbnailUri << std::endl;
    }
}


void MyPFGameSaveFilesUiProgressCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesSyncState syncState, _In_ void* context)
{
    UNREFERENCED_PARAMETER(context);
    //Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    PFGameSaveFilesSyncState state{};
    uint64_t current{};
    uint64_t total{};
    PFGameSaveFilesUiProgressGetProgress(localUserHandle, &state, &current, &total);
    if (syncState == PFGameSaveFilesSyncState::Downloading)
    {
        std::cout << "Downloading game save data..." << std::endl;
    }
    else
    {
        std::cout << "Uploading game save data..." << std::endl;
    }
    std::cout << "Progress: " << current << " / " << total << std::endl;
    std::cout << "[Cancel]" << std::endl;

    // if user chooses [Cancel], call PFGameSaveFilesSetUIProgressResponse(localUserHandle, PFGameSaveFilesUIProgressUserAction::Cancel);
    // These API calls can happen inside or outside of this callback
}

void MyPFGameSaveFilesUiSyncFailedCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesSyncState syncState,
    _In_ HRESULT error,
    _In_ void* context)
{
    UNREFERENCED_PARAMETER(error);
    UNREFERENCED_PARAMETER(syncState);
    Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    // Tell the user something like this:
    std::cout << "We couldn't sync your data with the cloud just now" << std::endl;
    std::cout << "Try syncing again or use this game or app offline [error]" << std::endl;
    std::cout << "[Try Again]" << std::endl;
    std::cout << "[Use Offline]" << std::endl;

    // if user chooses [Try Again], call PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Retry);
    // if user chooses [Use Offline], call PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::UseOffline);
    // if user chooses [Cancel], call PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
    // These API calls can happen inside or outside of this callback

    if (!gameState->interactive)
    {
        PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::UseOffline);
    }
}

void MyPFGameSaveFilesUiActiveDeviceContentionCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveDescriptor* localGameSave, _In_ PFGameSaveDescriptor* remoteGameSave, _In_ void* context)
{
    UNREFERENCED_PARAMETER(localGameSave);

    Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    if (gameState->activeDeviceContentionSyncLast)
    {
        // Automatically sync last data
        PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
        LogDescriptor(remoteGameSave);
    }
    else
    {
        // Tell the user something like this:
        std::cout << "Your other device is taking a long time to sync to the cloud" << std::endl;
        std::cout << "Do you want to sync the last saved data we have in the cloud? [y/n]" << std::endl;
        LogDescriptor(remoteGameSave);

        char response = 'y';
        // TODO: In interactive mode, get the response from the user

        if (response == 'y' || response == 'Y')
        {
            // Simple test app has no UI buttons, and will automatically click "sync last saved data" by calling PFGameSaveFilesUiActiveDeviceContentionSyncLastSavedData().
            PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
        }
        else
        {
            PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
        }
    }
}

void MyPFGameSaveFilesUiConflictCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave,
    _In_ PFGameSaveDescriptor* remoteGameSave,
    _In_ void* context)
{
    Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    // Tell the user something like this:
    std::cout << "Which one do you want to use?" << std::endl;
    std::cout << "The save data we have on the network is different than the data on this device." << std::endl;
    std::cout << "L: [This device <insert localModifiedTime> with [localSize] bytes]" << std::endl;
    std::cout << "R: [The Xbox[?] network (cloud data) <insert remoteModifiedTime> with [remoteSize] bytes]" << std::endl;
    std::cout << "C: [Cancel]" << std::endl;
    LogDescriptor(localGameSave);
    LogDescriptor(remoteGameSave);

    // Save just to verify later 
    gameState->savedLocalGameSave = *localGameSave;
    gameState->savedRemoteGameSave = *remoteGameSave;

    // if the users chooses [This device], call PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeLocal);
    // if the users chooses [Cloud data], call PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeRemote);
    // if the users chooses [Cancel], call PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
    // These API calls can happen inside or outside of this callback

    if (!gameState->interactive)
    {
        gameState->gotConflict = true;
        if (gameState->takeLocal)
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeLocal);
        }
        else if (gameState->takeRemote)
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeRemote);
        }
        else
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
        }
    }
    else
    {
        char response = 'l';
        // TODO: In interactive mode, get the response from the user

        if (response == 'l' || response == 'L')
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeLocal);
        }
        else if (response == 'r' || response == 'R')
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeRemote);
        }
        else
        {
            PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
        }
    }
}

void MyPFGameSaveFilesUiOutOfStorageCallback(_In_ PFLocalUserHandle localUserHandle, _In_ uint64_t requiredBytes, _In_ void* context)
{
    UNREFERENCED_PARAMETER(requiredBytes);
    Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    // Tell the user something like this:
    std::cout << "You are out of space on this device." << std::endl;
    std::cout << "You need on [requiredBytes] bytes free." << std::endl;
    std::cout << "[Try again]" << std::endl;
    std::cout << "[Cancel]" << std::endl;

    // if user chooses [Try Again], call PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Retry);
    // if user chooses [Cancel], call PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
    // These API calls can happen inside or outside of this callback

    if (!gameState->interactive)
    {
        PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
    }
}

void MyPFGameSaveFilesActiveDeviceChangedCallback(
    PFLocalUserHandle localUserHandle,
    PFGameSaveDescriptor* activeDevice,
    void* context)
{
    UNREFERENCED_PARAMETER(localUserHandle);
    Sample_GameSaveState* gameState = static_cast<Sample_GameSaveState*>(context);

    // Tell the user something like this:
    std::cout << "The active device has changed.  Going back to main menu." << std::endl;
    std::cout << "New device type: " << activeDevice->deviceType << std::endl;
    LogDescriptor(activeDevice);

    gameState->gotActiveDeviceChangeNotification = true;
}

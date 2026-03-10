// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

struct GameSaveUiCallbackInfo
{
    PFGameSaveFilesUiProgressCallback* progressCallback;
    void* progressContext;

    PFGameSaveFilesUiSyncFailedCallback* syncFailedCallback;
    void* syncFailedContext;

    PFGameSaveFilesUiActiveDeviceContentionCallback* activeDeviceContentionCallback;
    void* activeDeviceContentionContext;

    PFGameSaveFilesUiConflictCallback* conflictCallback;
    void* conflictContext;

    PFGameSaveFilesUiOutOfStorageCallback* outOfStorageCallback;
    void* outOfStorageContext;

    XTaskQueueHandle activeDeviceChangedCallbackQueue;
    PFGameSaveFilesActiveDeviceChangedCallback* activeDeviceChangedCallback;
    void* activeDeviceChangedContext;
};
GameSaveUiCallbackInfo& GetGameSaveUiCallbackInfo() noexcept;

} // namespace GameSave
} // namespace PlayFab
// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

struct GameSaveUiCallbackInfo
{
    PFGameSaveFilesUiProgressCallback* progressCallback{ nullptr };
    void* progressContext{ nullptr };

    PFGameSaveFilesUiSyncFailedCallback* syncFailedCallback{ nullptr };
    void* syncFailedContext{ nullptr };

    PFGameSaveFilesUiActiveDeviceContentionCallback* activeDeviceContentionCallback{ nullptr };
    void* activeDeviceContentionContext{ nullptr };

    PFGameSaveFilesUiConflictCallback* conflictCallback{ nullptr };
    void* conflictContext{ nullptr };

    PFGameSaveFilesUiOutOfStorageCallback* outOfStorageCallback{ nullptr };
    void* outOfStorageContext{ nullptr };

    XTaskQueueHandle activeDeviceChangedCallbackQueue{ nullptr };
    PFGameSaveFilesActiveDeviceChangedCallback* activeDeviceChangedCallback{ nullptr };
    void* activeDeviceChangedContext{ nullptr };
};
GameSaveUiCallbackInfo& GetGameSaveUiCallbackInfo() noexcept;
std::mutex& GetGameSaveUiCallbackMutex() noexcept;

} // namespace GameSave
} // namespace PlayFab
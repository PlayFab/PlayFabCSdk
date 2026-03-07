// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

extern "C"
{

// These functions can be used only for debugging.
PF_API PFGameSaveFilesSetMockDeviceIdForDebug(_In_ const char* deviceId);
PF_API PFGameSaveFilesSetMockRootFolderForDebug(_In_ const char* rootFolder);
PF_API PFGameSaveFilesSetMockManifestOffsetForDebug(_In_ size_t offset);
PF_API PFGameSaveFilesSetMockDataFolderForDebug(_In_ const char* mockDataFolder);
PF_API PFGameSaveFilesSetForceOutOfStorageErrorForDebug(_In_ bool forceError);
PF_API PFGameSaveFilesSetForceSyncFailedErrorForDebug(_In_ bool forceError);
PF_API PFGameSaveFilesSetWriteManifestsToDiskForDebug(_In_ bool writeManifests);
PF_API PFGameSaveFilesSetActiveDevicePollForceChangeForDebug();
PF_API PFGameSaveFilesSetActiveDevicePollIntervalForDebug(_In_ uint32_t interval);

PF_API PFGameSaveFilesPauseUploadForDebug();
PF_API PFGameSaveFilesResumeUploadForDebug();

enum class GameSaveServiceMockForcedOffline
{
    None = 0,

    // Download endpoints
    ListManifests,
    InitializeManifest,
    GetManifestDownloadDetails,
    DownloadFile,

    // Upload endpoints
    InitiateUpload,
    FinalizeManifest,
    UpdateManifest,
    UploadFile,
};

PF_API PFGameSaveFilesSetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode);

// These functions can be used only for debugging and will only return data in DEBUG builds
PF_API PFGameSaveFilesGetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept;
PF_API PFGameSaveFilesGetStatsJsonForDebug(_In_ PFLocalUserHandle localUserHandle, _In_ size_t jsonSize, _Out_writes_(jsonSize) char* jsonBuffer, _Out_opt_ size_t* jsonSizeUsed);

}
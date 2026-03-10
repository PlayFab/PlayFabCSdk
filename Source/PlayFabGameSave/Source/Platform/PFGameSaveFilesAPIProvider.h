// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "stdafx.h"
#include "ApiHelpers.h"
#include "DownloadAsyncProvider.h"
#include "UploadAsyncProvider.h"
#include "FileResetCloudAsyncProvider.h"
#include "PFGameSaveFilesForDebug.h"
#include "GameSaveServiceMock.h"

namespace PlayFab
{
namespace GameSave
{

class GameSaveAPIProvider
{
public:
    virtual ~GameSaveAPIProvider() = default;
    virtual HRESULT Initialize(_In_ PFGameSaveInitArgs* args) noexcept = 0;
    virtual HRESULT UninitializeAsync(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT UninitializeResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetActiveDeviceChangedCallback(
        _In_opt_ XTaskQueueHandle callbackQueue,
        _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback,
        _In_opt_ void* context
    ) noexcept = 0;
    virtual HRESULT SetUiCallbacks(
        _In_ PFGameSaveUICallbacks* callbacks
    ) noexcept = 0;
    virtual HRESULT UiProgressGetProgress(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_opt_ PFGameSaveFilesSyncState* syncState,
        _Out_opt_ uint64_t* current,
        _Out_opt_ uint64_t* total
    ) noexcept = 0;
    virtual HRESULT GetFolderSize(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ size_t* saveRootFolderSize
    ) noexcept = 0;
    virtual HRESULT GetFolder(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t saveRootFolderSize,
        _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
        _Out_opt_ size_t* saveRootFolderUsed
    ) noexcept = 0;
    virtual HRESULT AddUserWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesAddUserOptions options,
        _Inout_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT AddUserWithUiResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT GetRemainingQuota(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ int64_t* remainingQuota
    ) noexcept = 0;
    virtual HRESULT IsConnectedToCloud(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ bool* isConnectedToCloud
    ) noexcept = 0;
    virtual HRESULT UploadWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUploadOption option,
        _Inout_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT UploadWithUiResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetUiProgressResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiProgressUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiSyncFailedResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiSyncFailedUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiActiveDeviceContentionResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiConflictResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiConflictUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiOutOfStorageResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiOutOfStorageUserAction action
    ) noexcept = 0;
    virtual HRESULT SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept = 0;
    virtual HRESULT SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept = 0;
    virtual HRESULT SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept = 0;
    virtual HRESULT GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept = 0;
    virtual HRESULT GetStatsJsonForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t jsonSize,
        _Out_writes_(jsonSize) char* jsonBuffer,
        _Out_opt_ size_t* jsonSizeUsed
    ) noexcept = 0;
    virtual HRESULT GetSaveDescriptionSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* descriptionSize) noexcept = 0;
    virtual HRESULT GetSaveDescriptionForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t descriptionSize,
        _Out_writes_(descriptionSize) char* descriptionBuffer,
        _Out_opt_ size_t* descriptionSizeUsed
    ) noexcept = 0;
    virtual HRESULT SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept = 0;
    virtual HRESULT SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept = 0;
    virtual HRESULT SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept = 0;
    virtual HRESULT PauseUploadForDebug() noexcept = 0;
    virtual HRESULT SetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode) noexcept = 0;
    virtual HRESULT ResumeUploadForDebug() noexcept = 0;
    virtual HRESULT ResetCloudAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT ResetCloudResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetSaveDescriptionAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ const char* shortSaveDescription,
        _In_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept = 0;

    // Optional callbacks for in-process download/upload scenarios - only some platforms need these to be defined
    virtual HRESULT DownloadProcessingComplete() noexcept { return S_OK; }
    virtual HRESULT UploadProcessingComplete() noexcept { return S_OK; }
};

} // namespace GameSave
} // namespace PlayFab
// Copyright (C) Microsoft Corporation. All rights reserved.
#include "Platform/PFGameSaveFilesAPIProvider.h"

namespace PlayFab
{
namespace GameSave
{

class GameSaveAPIProviderWin32 : public GameSaveAPIProvider
{
public:
    GameSaveAPIProviderWin32() = default;
    HRESULT Initialize(_In_ PFGameSaveInitArgs* args) noexcept override;
    HRESULT UninitializeAsync(_Inout_ XAsyncBlock* async) noexcept override;
    HRESULT UninitializeResult(_Inout_ XAsyncBlock* async) noexcept override;
    HRESULT SetActiveDeviceChangedCallback(
        _In_opt_ XTaskQueueHandle callbackQueue,
        _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback,
        _In_opt_ void* context
    ) noexcept override;
    HRESULT SetUiCallbacks(
        _In_ PFGameSaveUICallbacks* callbacks
    ) noexcept override;
    HRESULT UiProgressGetProgress(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_opt_ PFGameSaveFilesSyncState* syncState,
        _Out_opt_ uint64_t* current,
        _Out_opt_ uint64_t* total
    ) noexcept override;
    HRESULT GetFolderSize(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ size_t* saveRootFolderSize
    ) noexcept override;
    HRESULT GetFolder(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t saveRootFolderSize,
        _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
        _Out_opt_ size_t* saveRootFolderUsed
    ) noexcept override;
    HRESULT AddUserWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesAddUserOptions options,
        _Inout_ XAsyncBlock* async
    ) noexcept override;
    HRESULT AddUserWithUiResult(_Inout_ XAsyncBlock* async) noexcept override;
    HRESULT GetRemainingQuota(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ int64_t* remainingQuota
    ) noexcept override;
    HRESULT IsConnectedToCloud(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ bool* isConnectedToCloud
    ) noexcept override;
    HRESULT UploadWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUploadOption option,
        _Inout_ XAsyncBlock* async
    ) noexcept override;
    HRESULT UploadWithUiResult(_Inout_ XAsyncBlock* async) noexcept override;
    HRESULT SetUiProgressResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiProgressUserAction action
    ) noexcept override;
    HRESULT SetUiSyncFailedResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiSyncFailedUserAction action
    ) noexcept override;
    HRESULT SetUiActiveDeviceContentionResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action
    ) noexcept override;
    HRESULT SetUiConflictResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiConflictUserAction action
    ) noexcept override;
    HRESULT SetUiOutOfStorageResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiOutOfStorageUserAction action
    ) noexcept override;
    HRESULT SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept override;
    HRESULT SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept override;
    HRESULT SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept override;
    HRESULT GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept override;
    HRESULT GetStatsJsonForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t jsonSize,
        _Out_writes_(jsonSize) char* jsonBuffer,
        _Out_opt_ size_t* jsonSizeUsed
    ) noexcept override;
    HRESULT GetSaveDescriptionSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* descriptionSize) noexcept override;
    HRESULT GetSaveDescriptionForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t descriptionSize,
        _Out_writes_(descriptionSize) char* descriptionBuffer,
        _Out_opt_ size_t* descriptionSizeUsed
    ) noexcept override;
    HRESULT SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept override;
    HRESULT SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept override;
    HRESULT SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept override;
    HRESULT PauseUploadForDebug() noexcept override;
    HRESULT SetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode) noexcept override;
    HRESULT ResumeUploadForDebug() noexcept override;
    HRESULT ResetCloudAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ XAsyncBlock* async
    ) noexcept override;
    HRESULT ResetCloudResult(_Inout_ XAsyncBlock* async) noexcept override;
    HRESULT SetSaveDescriptionAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ const char* shortSaveDescription,
        _In_ XAsyncBlock* async
    ) noexcept override;
    HRESULT SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept override;
};

} // namespace GameSave
} // namespace PlayFab
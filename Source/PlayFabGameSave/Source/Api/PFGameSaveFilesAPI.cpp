// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "ApiHelpers.h"
#include "DownloadAsyncProvider.h"
#include "UploadAsyncProvider.h"
#include "FileResetCloudAsyncProvider.h"
#include "PFGameSaveFilesForDebug.h"
#include "GameSaveServiceMock.h"

using namespace PlayFab;
using namespace PlayFab::GameSave;

template<typename TWork>
inline HRESULT GSApiImpl(const char* apiIdentity, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

        return work(*state);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFGameSaveFilesInitialize(_In_ PFGameSaveInitArgs* args) noexcept
{
    try
    {
        RETURN_HR_INVALIDARG_IF_NULL(args);
        RETURN_IF_FAILED(GameSaveGlobalState::Create(nullptr, args->options, args->backgroundQueue));
        SharedPtr<GameSaveGlobalState> state;
        RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

        return state->ApiProvider().Initialize(args);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary PFGameSaveFilesInitialize\n    %s:%u", E_FAIL, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFGameSaveFilesUninitializeAsync(
    _Inout_ XAsyncBlock* async
    ) noexcept
{
    return GSApiImpl("PFGameSaveFilesUninitializeAsync", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(async);
        return state.ApiProvider().UninitializeAsync(async);
    });    
}

PF_API PFGameSaveFilesUninitializeResult(
    _Inout_ XAsyncBlock* async
    ) noexcept
{
    return XAsyncGetStatus(async, false);
}

PF_API PFGameSaveFilesSetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback, _In_opt_ void* context
    ) noexcept
{
    return GSApiImpl("PFGameSaveFilesSetActiveDeviceChangedCallback", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetActiveDeviceChangedCallback(callbackQueue, callback, context);
    });
}

PF_API PFGameSaveFilesSetUiCallbacks(
    _In_ PFGameSaveUICallbacks* callbacks
) noexcept
{
    return GSApiImpl("PFGameSaveFilesSetUiCallbacks", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(callbacks);
        return state.ApiProvider().SetUiCallbacks(callbacks);
    });
}

PF_API PFGameSaveFilesUiProgressGetProgress(
    _In_ PFLocalUserHandle localUserHandle, 
    _Out_opt_ PFGameSaveFilesSyncState* syncState, 
    _Out_opt_ uint64_t* current, 
    _Out_opt_ uint64_t* total)
{
    return GSApiImpl("PFGameSaveFilesUiProgressGetProgress", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().UiProgressGetProgress(localUserHandle, syncState, current, total);
    });
    
}

PF_API PFGameSaveFilesGetFolderSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* saveRootFolderSize
) noexcept
{
    return GSApiImpl("PFGameSaveFilesGetFolderSize", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().GetFolderSize(localUserHandle, saveRootFolderSize);
    });
}

PF_API PFGameSaveFilesGetFolder(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t saveRootFolderSize,
    _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
    _Out_opt_ size_t* saveRootFolderUsed
) noexcept
{
    return GSApiImpl("PFGameSaveFilesGetFolder", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().GetFolder(localUserHandle, saveRootFolderSize, saveRootFolderBuffer, saveRootFolderUsed);
    });
}

PF_API PFGameSaveFilesAddUserWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesAddUserOptions options,
    _Inout_ XAsyncBlock* async) noexcept
{
    return GSApiImpl("PFGameSaveFilesAddUserWithUiAsync", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
        RETURN_HR_INVALIDARG_IF_NULL(async);

        // Validate that at most one rollback flag is specified; both rollback bits together are invalid
        uint32_t optionBits = static_cast<uint32_t>(options);
        const uint32_t rollbackMask = static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) |
            static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastConflict);
        uint32_t selectedRollback = optionBits & rollbackMask;
        if (selectedRollback == rollbackMask)
        {
            return E_INVALIDARG; // both rollback flags set
        }
        
        return state.ApiProvider().AddUserWithUiAsync(localUserHandle, options, async);
    });
}

PF_API PFGameSaveFilesAddUserWithUiResult(
    _Inout_ XAsyncBlock* async
    ) noexcept
{
    return GSApiImpl("PFGameSaveFilesAddUserWithUiResult", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().AddUserWithUiResult(async);
    });
}

PF_API PFGameSaveFilesGetRemainingQuota(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ int64_t* remainingQuota) noexcept
{
    return GSApiImpl("PFGameSaveFilesGetRemainingQuota", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().GetRemainingQuota(localUserHandle, remainingQuota);
    });
}

PF_API PFGameSaveFilesIsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud) noexcept
{
    return GSApiImpl("PFGameSaveFilesIsConnectedToCloud", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().IsConnectedToCloud(localUserHandle, isConnectedToCloud);
    });
}

PF_API PFGameSaveFilesUploadWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUploadOption option,
    _Inout_ XAsyncBlock* async
    ) noexcept
{
    return GSApiImpl("PFGameSaveFilesUploadWithUiAsync", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
        RETURN_HR_INVALIDARG_IF_NULL(async);
        return state.ApiProvider().UploadWithUiAsync(localUserHandle, option, async);
    });
}

PF_API PFGameSaveFilesUploadWithUiResult(
    _Inout_ XAsyncBlock* async
    ) noexcept
{
    return GSApiImpl("PFGameSaveFilesUploadWithUiResult", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().UploadWithUiResult(async);
    });
}

PF_API PFGameSaveFilesSetSaveDescriptionAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ const char* shortSaveDescription,
    _In_ XAsyncBlock* async
) noexcept
{
    return GSApiImpl("PFGameSaveFilesSetSaveDescriptionAsync", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
        RETURN_HR_INVALIDARG_IF_NULL(shortSaveDescription);
        RETURN_HR_INVALIDARG_IF_NULL(async);
        return state.ApiProvider().SetSaveDescriptionAsync(localUserHandle, shortSaveDescription, async);
    }); 
}

PF_API PFGameSaveFilesSetSaveDescriptionResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return GSApiImpl("PFGameSaveFilesSetSaveDescriptionResult", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetSaveDescriptionResult(async);
    });
}

PF_API PFGameSaveFilesSetUiProgressResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiProgressUserAction action)
{
    return GSApiImpl("PFGameSaveFilesSetUiProgressResponse", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetUiProgressResponse(localUserHandle, action);
    });
}

PF_API PFGameSaveFilesSetUiSyncFailedResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiSyncFailedUserAction action)
{
    return GSApiImpl("PFGameSaveFilesSetUiSyncFailedResponse", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetUiSyncFailedResponse(localUserHandle, action);
    });
}

PF_API PFGameSaveFilesSetUiActiveDeviceContentionResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action)
{
    return GSApiImpl("PFGameSaveFilesSetUiActiveDeviceContentionResponse", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetUiActiveDeviceContentionResponse(localUserHandle, action);
    });
}

PF_API PFGameSaveFilesSetUiConflictResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiConflictUserAction action)
{
    return GSApiImpl("PFGameSaveFilesSetUiConflictResponse", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetUiConflictResponse(localUserHandle, action);
    });
}

PF_API PFGameSaveFilesSetUiOutOfStorageResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiOutOfStorageUserAction action)
{
    return GSApiImpl("PFGameSaveFilesSetUiOutOfStorageResponse", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetUiOutOfStorageResponse(localUserHandle, action);
    });
}

PF_API PFGameSaveFilesSetMockDeviceIdForDebug(_In_ const char* deviceId)
{
    return GSApiImpl("PFGameSaveFilesSetMockDeviceIdForDebug", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(deviceId);
        return state.ApiProvider().SetMockDeviceIdForDebug(deviceId);
    });
}

PF_API PFGameSaveFilesSetMockRootFolderForDebug(_In_ const char* rootFolder)
{
    return GSApiImpl("PFGameSaveFilesSetMockRootFolderForDebug", [&](GameSaveGlobalState& state) {
        state.SetDebugRootFolderOverride(rootFolder);
        return S_OK;
    });
}

PF_API PFGameSaveFilesSetMockManifestOffsetForDebug(_In_ size_t offset)
{
    return GSApiImpl("PFGameSaveFilesSetMockManifestOffsetForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetMockManifestOffsetForDebug(offset);
    });
}

PF_API PFGameSaveFilesSetMockDataFolderForDebug(_In_ const char* mockDataFolder)
{
    return GSApiImpl("PFGameSaveFilesSetMockDataFolderForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetMockDataFolderForDebug(mockDataFolder);
    }); 
}

PF_API PFGameSaveFilesGetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept
{
    return GSApiImpl("PFGameSaveFilesGetStatsJsonSizeForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().GetStatsJsonSizeForDebug(localUserHandle, jsonSize);
    }); 
}

PF_API PFGameSaveFilesGetStatsJsonForDebug(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t jsonSize,
    _Out_writes_(jsonSize) char* jsonBuffer,
    _Out_opt_ size_t* jsonSizeUsed)
{
    return GSApiImpl("PFGameSaveFilesGetStatsJsonForDebug", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(jsonBuffer);
        return state.ApiProvider().GetStatsJsonForDebug(localUserHandle, jsonSize, jsonBuffer, jsonSizeUsed);
    }); 
}

PF_API PFGameSaveFilesSetForceOutOfStorageErrorForDebug(_In_ bool forceError)
{
    return GSApiImpl("PFGameSaveFilesSetForceOutOfStorageErrorForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetForceOutOfStorageErrorForDebug(forceError);
    }); 
}

PF_API PFGameSaveFilesSetForceSyncFailedErrorForDebug(_In_ bool forceError)
{
    return GSApiImpl("PFGameSaveFilesSetForceSyncFailedErrorForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetForceSyncFailedErrorForDebug(forceError);
    }); 
}

PF_API PFGameSaveFilesSetWriteManifestsToDiskForDebug(_In_ bool writeManifests)
{
    return GSApiImpl("PFGameSaveFilesSetWriteManifestsToDiskForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetWriteManifestsToDiskForDebug(writeManifests);
    });
}

PF_API PFGameSaveFilesPauseUploadForDebug()
{
    return GSApiImpl("PFGameSaveFilesPauseUploadForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().PauseUploadForDebug();
    });
}

PF_API PFGameSaveFilesSetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode)
{
    return GSApiImpl("PFGameSaveFilesSetMockForceOfflineForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().SetMockForceOfflineForDebug(mode);
    });
}

PF_API PFGameSaveFilesResumeUploadForDebug()
{
    return GSApiImpl("PFGameSaveFilesResumeUploadForDebug", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().ResumeUploadForDebug();
    });
}

PF_API PFGameSaveFilesResetCloudAsync(_In_ PFLocalUserHandle localUserHandle, _In_ XAsyncBlock* async) noexcept
{
    return GSApiImpl("PFGameSaveFilesResetCloudAsync", [&](GameSaveGlobalState& state) {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
        RETURN_HR_INVALIDARG_IF_NULL(async);
        return state.ApiProvider().ResetCloudAsync(localUserHandle, async);
    });
}

PF_API PFGameSaveFilesResetCloudResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return GSApiImpl("PFGameSaveFilesResetCloudResult", [&](GameSaveGlobalState& state) {
        return state.ApiProvider().ResetCloudResult(async);
    });
}


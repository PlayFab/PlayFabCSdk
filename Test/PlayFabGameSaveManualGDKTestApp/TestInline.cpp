// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "TestInline.h"

TestInlineClass::TestInlineClass() noexcept(false)
{
}

TestInlineClass::~TestInlineClass()
{
}


HRESULT TestInlineClass::TestGameSaveAPIAddUser(XUserHandle xuser)
{
    HRESULT hr = PFInitialize(nullptr);
    RETURN_IF_FAILED(hr);

    PFGameSaveInitArgs args = {};
    hr = PFGameSaveFilesInitialize(&args);
    RETURN_IF_FAILED(hr);

    PFLocalUserHandle localUserHandle;

    PFServiceConfigHandle serviceConfigHandle{ nullptr };
    hr = PFServiceConfigCreateHandle(
        "https://E18D7.playfabapi.com",
        "E18D7",
        &serviceConfigHandle);
    RETURN_IF_FAILED(hr);

    hr = PFLocalUserCreateHandleWithXboxUser(serviceConfigHandle, xuser, nullptr, &localUserHandle);
    RETURN_IF_FAILED(hr);
    XAsyncBlock async{};
    XTaskQueueHandle queue{};
    hr = XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &queue);
    RETURN_IF_FAILED(hr);
    async.queue = queue;
    hr = PFGameSaveFilesAddUserWithUiAsync(localUserHandle, PFGameSaveFilesAddUserOptions::None, &async);
    RETURN_IF_FAILED(hr);
    hr = XAsyncGetStatus(&async, true); // Simple synchronous wait, not recommended for production code
    RETURN_IF_FAILED(hr);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    RETURN_IF_FAILED(hr);

    size_t folderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(localUserHandle, &folderSize);
    RETURN_IF_FAILED(hr);
    char folder[1024];
    hr = PFGameSaveFilesGetFolder(localUserHandle, 1024, folder, nullptr);
    RETURN_IF_FAILED(hr);

    int64_t remainingQuota = 0;
    hr = PFGameSaveFilesGetRemainingQuota(localUserHandle, &remainingQuota);
    // this returns E_NOTIMPL for now
    //RETURN_IF_FAILED(hr);

    XAsyncBlock async2{};
    hr = PFGameSaveFilesUninitializeAsync(&async2);
    RETURN_IF_FAILED(hr);
    hr = XAsyncGetStatus(&async2, true); // Simple synchronous wait, not recommended for production code
    RETURN_IF_FAILED(hr);
    hr = PFGameSaveFilesUninitializeResult(&async2);
    RETURN_IF_FAILED(hr);

    return S_OK;
}

HRESULT TestInlineClass::TestLinkGDKGameSaveAPI()
{
    // Not proper calling patterns
    // Just to test the linking of the GDK PFGameSave APIs

    PFGameSaveInitArgs args = {};
    PFGameSaveFilesInitialize(&args);

    PFLocalUserHandle localUserHandle = nullptr;
    XAsyncBlock async{};
    PFGameSaveFilesAddUserWithUiAsync(localUserHandle, PFGameSaveFilesAddUserOptions::None, &async);
    XAsyncGetStatus(&async, true);
    PFGameSaveFilesAddUserWithUiResult(&async);

    size_t folderSize = 0;
    PFGameSaveFilesGetFolderSize(localUserHandle, &folderSize);
    char folder[1024];
    PFGameSaveFilesGetFolder(localUserHandle, 1024, folder, nullptr);

    int64_t remainingQuota = 0;
    PFGameSaveFilesGetRemainingQuota(localUserHandle, &remainingQuota);

    XAsyncBlock async2{};
    PFGameSaveFilesUninitializeAsync(&async2);
    PFGameSaveFilesUninitializeResult(&async2);

    XAsyncBlock async3{};
    PFGameSaveFilesUploadWithUiAsync(localUserHandle, PFGameSaveFilesUploadOption::KeepDeviceActive, &async3);
    PFGameSaveFilesUploadWithUiResult(&async3);

    PFGameSaveFilesSetActiveDeviceChangedCallback(
        nullptr, // callbackQueue
        [](PFLocalUserHandle localUserHandle,
            PFGameSaveDescriptor* activeDevice,
            void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(activeDevice);
            UNREFERENCED_PARAMETER(context);
        },
        nullptr); // contextPtr

    PFGameSaveUICallbacks callbacks = {};
    callbacks.progressCallback = [](PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(syncState);
            UNREFERENCED_PARAMETER(context);
        };
    callbacks.progressContext = nullptr;

    callbacks.syncFailedCallback = [](PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        HRESULT error,
        void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(syncState);
            UNREFERENCED_PARAMETER(error);
            UNREFERENCED_PARAMETER(context);
        };
    callbacks.syncFailedContext = nullptr;

    callbacks.activeDeviceContentionCallback = [](PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveDescriptor* localGameSave,
        _In_ PFGameSaveDescriptor* remoteGameSave,
        void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(localGameSave);
            UNREFERENCED_PARAMETER(remoteGameSave);
            UNREFERENCED_PARAMETER(context);
        };
    callbacks.activeDeviceContentionContext = nullptr;

    callbacks.conflictCallback = [](PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveDescriptor* localGameSave,
        _In_ PFGameSaveDescriptor* remoteGameSave,
        void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(localGameSave);
            UNREFERENCED_PARAMETER(remoteGameSave);
            UNREFERENCED_PARAMETER(context);
        };
    callbacks.conflictContext = nullptr;

    callbacks.outOfStorageCallback = [](PFLocalUserHandle localUserHandle,
        uint64_t requiredBytes,
        void* context)
        {
            UNREFERENCED_PARAMETER(localUserHandle);
            UNREFERENCED_PARAMETER(requiredBytes);
            UNREFERENCED_PARAMETER(context);
        };
    callbacks.outOfStorageContext = nullptr;
    PFGameSaveFilesSetUiCallbacks(&callbacks);

    PFGameSaveFilesSyncState syncState;
    uint64_t current;
    uint64_t total;
    PFGameSaveFilesUiProgressGetProgress(localUserHandle, &syncState, &current, &total);

    PFGameSaveFilesSetUiProgressResponse(localUserHandle, PFGameSaveFilesUiProgressUserAction::Cancel);
    PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
    PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Retry);
    PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::UseOffline);
    PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
    PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry);
    PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
    PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
    PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeLocal);
    PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeRemote);
    PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
    PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
    PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Retry);

    PFGameSaveFilesResetCloudAsync(localUserHandle, &async2);

    return S_OK;
}
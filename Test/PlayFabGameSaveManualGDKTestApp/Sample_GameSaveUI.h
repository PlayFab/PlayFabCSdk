// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

HRESULT DownloadFile(const std::string& url, const std::string& fileName);
void LogDescriptor(PFGameSaveDescriptor* descriptor);

void MyPFGameSaveFilesUiProgressCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesSyncState syncState,
    _In_ void* context);

void MyPFGameSaveFilesUiSyncFailedCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesSyncState syncState,
    _In_ HRESULT error,
    _In_ void* context);

void MyPFGameSaveFilesUiActiveDeviceContentionCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave,
    _In_ PFGameSaveDescriptor* remoteGameSave,
    _In_ void* context);

void MyPFGameSaveFilesUiConflictCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave,
    _In_ PFGameSaveDescriptor* remoteGameSave,
    _In_ void* context);

void MyPFGameSaveFilesUiOutOfStorageCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ uint64_t requiredBytes,
    _In_ void* context);

void MyPFGameSaveFilesActiveDeviceChangedCallback(
    PFLocalUserHandle localUserHandle,
    PFGameSaveDescriptor* activeDevice,
    void* context);

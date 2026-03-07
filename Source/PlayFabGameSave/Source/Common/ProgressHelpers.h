// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "stdafx.h"

namespace PlayFab
{
namespace GameSave
{

typedef void CALLBACK ProgressCallback(
    _In_ PFGameSaveFilesSyncState syncState,
    _In_ uint64_t current,
    _In_ uint64_t total,
    _In_ void* context);

struct InnerProgressContext
{
    ProgressCallback* callback;
    void* callbackContext;
    ISchedulableTask& task;
    LocalUser& localUser;
    PFGameSaveFilesSyncState syncState;

    InnerProgressContext(
        ProgressCallback* callback,
        void* callbackContext,
        ISchedulableTask& task,
        LocalUser& localUser,
        PFGameSaveFilesSyncState syncState) :
        callback{ callback },
        callbackContext{ callbackContext },
        task{ task },
        localUser{ localUser },
        syncState{ syncState }
    {
    }
};

HRESULT InnerProgressCallback(
    _In_ HCCallHandle call,
    _In_ uint64_t current,
    _In_ uint64_t total,
    _In_opt_ void* innerContext // InnerProgressContext
);

} // namespace GameSave
} // namespace PlayFab
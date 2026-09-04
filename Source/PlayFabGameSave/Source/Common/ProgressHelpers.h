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
    PFGameSaveFilesSyncState syncState;

    // When both are non-zero, InnerProgressCallback converts compressed transfer
    // progress to uncompressed units using the global ratio (totalUncompressed / totalCompressed).
    uint64_t totalUncompressedBytes{ 0 };
    uint64_t totalCompressedBytes{ 0 };

    InnerProgressContext(
        ProgressCallback* callback,
        void* callbackContext,
        ISchedulableTask& /*task*/,
        LocalUser& /*localUser*/,
        PFGameSaveFilesSyncState syncState) :
        callback{ callback },
        callbackContext{ callbackContext },
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
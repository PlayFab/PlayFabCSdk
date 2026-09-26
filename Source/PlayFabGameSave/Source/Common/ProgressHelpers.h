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

    // Operation totals, plus the offsets and sizes InnerProgressCallback needs to place this
    // file's transfer inside them. The completed offsets are the bytes already finished by
    // earlier files, in each unit; the file sizes are the current file's own.
    //
    // These exist because a single global compressed:uncompressed ratio is not monotonic across
    // files that compress differently - see InnerProgressCallback for the arithmetic.
    uint64_t totalUncompressedBytes{ 0 };
    uint64_t totalCompressedBytes{ 0 };
    uint64_t completedUncompressedBytes{ 0 };
    uint64_t completedCompressedBytes{ 0 };
    uint64_t fileUncompressedBytes{ 0 };
    uint64_t fileCompressedBytes{ 0 };

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
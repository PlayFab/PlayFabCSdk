// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "GameSaveGlobalState.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"
#include "UploadAsyncProvider.h"

using namespace PlayFab;

namespace PlayFab
{
namespace GameSave
{



HRESULT UploadAsyncProvider::DoWork(RunContext runContext)
{
    std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
#if _DEBUG
    SingleThreadProviderValidationScope threadScope(m_singleThreadProvider);
#endif

    HRESULT hr = m_folderSync->DoWorkFolderUpload(runContext, *this, m_folderSyncMutex, m_option);
    if (hr != E_PENDING)
    {
        TRACE_TASK(FormatString("UploadAsyncProvider::DoWork HR:0x%0.8x", hr));

        // Upload processing is complete (success or failure) since we're no longer pending.
        // Notify platform provider to perform cleanup (e.g., unmount Sony SaveData).
        // The GlobalState check ensures the state still exists before accessing the API provider.
        SharedPtr<GameSaveGlobalState> globalState;
        if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
        {
            HRESULT cleanupHr = globalState->ApiProvider().UploadProcessingComplete();
            if (FAILED(cleanupHr))
            {
                TRACE_ERROR("UploadProcessingComplete failed: 0x%0.8x\n", cleanupHr);
            }
        }

        if (SUCCEEDED(hr))
        {
            this->Complete(0);
        }
        else
        {
            // Reset syncState to SyncComplete so:
            // 1. Subsequent SetSaveDescriptionAsync calls don't think an upload is still in progress
            //    (if we left it as Uploading/PreparingForUpload, deferred descriptions could be lost).
            // 2. Subsequent Upload/Download operations don't return E_PF_GAMESAVE_USER_NOT_ADDED
            //    (NotStarted is treated as "user never added" by the API layer).
            m_folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0);
            this->Fail(hr);
        }
    }

    return hr;
}

} // namespace GameSave
} // namespace PlayFab

// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "DownloadAsyncProvider.h"
#include "GameSaveGlobalState.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"

using namespace PlayFab;

namespace PlayFab
{
namespace GameSave
{

HRESULT DownloadAsyncProvider::DoWork(RunContext runContext)
{
    std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
#if _DEBUG
    SingleThreadProviderValidationScope threadScope(m_singleThreadProviderValidation);
#endif

    HRESULT hr = m_folderSync->DoWorkFolderDownload(runContext, *this, m_folderSyncMutex);
    if (hr != E_PENDING)
    {
        TRACE_TASK(FormatString("DownloadAsyncProvider::DoWork HR:0x%0.8x", hr));

        // Upload processing is complete (success or failure) since we're no longer pending.
        // Notify platform provider to perform cleanup (e.g., unmount Sony SaveData).
        // The GlobalState check ensures the state still exists before accessing the API provider.
        SharedPtr<GameSaveGlobalState> globalState;
        if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
        {
            HRESULT cleanupHr = globalState->ApiProvider().DownloadProcessingComplete();
            if (FAILED(cleanupHr))
            {
                TRACE_ERROR("DownloadProcessingComplete failed: 0x%0.8x\n", cleanupHr);
            }
        }

        if (SUCCEEDED(hr))
        {
            this->Complete(0);
        }
        else
        {
            // Reset syncState so the SDK doesn't remain in a stale Downloading state.
            m_folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
            this->Fail(hr);
        }
    }

    return hr;
}

} // namespace GameSave
} // namespace PlayFab

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
#if defined(_DEBUG)
    SingleThreadProviderValidationScope threadScope(m_singleThreadProviderValidation);
#endif

    HRESULT hr = m_folderSync->DoWorkFolderDownload(runContext, *this, m_folderSyncMutex);
    if (hr != E_PENDING)
    {
        TRACE_TASK(FormatString("DownloadAsyncProvider::DoWork HR:0x%0.8x", hr));

        // The workflow has reached a terminal state and set the sync state itself (below on
        // failure), so the reservation is consumed rather than restored.
        ReleaseReservationOnce(false);

        // AddUser can drive an upload itself: the conflict-resolution path calls
        // UploadStep::Upload, which allocates a per-attempt staging directory. On the success path
        // UploadStep releases it at ListManifestsAfterUpload, but a terminal failure returns
        // straight out of DoWorkFolderDownload and would otherwise leave the directory behind
        // until the next AddUser sweep. Released here rather than at that one failure site so
        // every terminal exit from the download path is covered, mirroring what
        // UploadAsyncProvider::DoWork does for the ordinary upload path.
        //
        // Safe at this point specifically because the operation is over: DoWorkFolderDownload
        // returns E_PENDING for as long as the conflict upload is still pumping, and no
        // UploadStep failure path schedules async work before returning. A no-op when the
        // download never started an upload.
        m_folderSync->ReleaseUploadStaging();

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
            // A failed or cancelled reconnect leaves the user as it found them: added and offline
            // (CDS §Offline Support: Cancel acts as if AddUser was never called). TryReserveDownload
            // cleared the forced-disconnect flag to admit the reconnect; left cleared, the user read
            // as connected to the cloud while every upload was refused as not added.
            if (m_previousForcedDisconnectFromCloud)
            {
                m_folderSync->SetForcedDisconnectFromCloud(true);
            }
            // Reset syncState so the SDK doesn't remain in a stale Downloading state. Compare-and-set
            // (H17): only while this AddUser still owns the state. Its conflict upload reports the
            // upload states, which it owns too: no upload can be admitted while an AddUser runs.
            m_folderSync->TrySetSyncStateIfOwnedBy({ SyncOperationKind::AddUser, SyncOperationKind::Upload }, PFGameSaveFilesSyncState::NotStarted);
            this->Fail(hr);
        }
    }

    return hr;
}

} // namespace GameSave
} // namespace PlayFab

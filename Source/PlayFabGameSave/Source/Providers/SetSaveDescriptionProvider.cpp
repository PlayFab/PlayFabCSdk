// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "FolderSyncManager.h"
#include "SetSaveDescriptionProvider.h"

using namespace PlayFab;

namespace PlayFab
{
namespace GameSave
{

HRESULT SetSaveDescriptionProvider::DoWork(RunContext runContext)
{
    std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
#if _DEBUG
    SingleThreadProviderValidationScope threadScope(m_singleThreadProvider);
#endif

    // If finalize is in progress, wait asynchronously (reschedule) until it completes or timeout.
    FolderSyncManagerProgress prog = m_folderSync->GetSyncProgress();
    if ((prog.syncState == PFGameSaveFilesSyncState::Uploading || prog.syncState == PFGameSaveFilesSyncState::PreparingForUpload) &&
        m_folderSync->HasStartedFinalizeManifest())
    {
        if (!m_waitingForFinalize)
        {
            m_waitingForFinalize = true;
            m_waitElapsedMs = 0;
            TRACE_INFORMATION("SetSaveDescriptionProvider waiting for in-flight FinalizeManifest to complete");
        }

        if (m_waitElapsedMs < c_waitMaxMs)
        {
            m_waitElapsedMs += c_waitSliceMs;
            // Reschedule after slice to check again
            Schedule(c_waitSliceMs);
            return E_PENDING;
        }
        else
        {
            // Timed out; treat as deferred for next upload - just cache description and complete.
            m_folderSync->SetLastShortSaveDescription(m_shortSaveDescription);
            TRACE_WARNING("SetSaveDescriptionProvider timed out waiting for FinalizeManifest; description cached for next upload");
            m_waitingForFinalize = false; // reset stale wait state after timeout
            this->Complete(0);
            return S_OK;
        }
    }

    // If we reach here, either finalize wasn't in progress or has completed; ensure wait flag is cleared.
    if (m_waitingForFinalize)
    {
        m_waitingForFinalize = false;
    }

    HRESULT hr = m_folderSync->DoWorkSetSaveDescription(runContext, *this, m_folderSyncMutex, m_shortSaveDescription);
    if (hr != E_PENDING)
    {
        TRACE_TASK(FormatString("SetSaveDescriptionProvider::DoWork HR:0x%0.8x", hr));

        if (SUCCEEDED(hr))
        {
            m_folderSync->SetLastShortSaveDescription(m_shortSaveDescription);
            this->Complete(0);
        }
        else
        {
            this->Fail(hr);
        }
        // In any terminal path ensure wait flag is cleared
        m_waitingForFinalize = false;
    }

    return hr;
}

} // namespace GameSave
} // namespace PlayFab
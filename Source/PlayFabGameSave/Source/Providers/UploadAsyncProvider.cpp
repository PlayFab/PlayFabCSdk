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
HRESULT UploadAsyncProvider::Begin(RunContext runContext)
{
    if (!m_globalState)
    {
        RETURN_IF_FAILED(GameSaveGlobalState::Get(m_globalState));
    }
    m_cancellationToken.emplace(runContext.CancellationToken());
    (void)m_cancellationToken->RegisterForNotificationAndCheck(*this);
    HRESULT hr = GameSaveAsyncProvider::Begin(std::move(runContext));
    if (FAILED(hr))
    {
        // No work was queued. XAsyncBegin reports Begin failures asynchronously, so release
        // storage here rather than relying on the API's synchronous-start rollback.
        std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex);
        return FinishUpload(hr);
    }
    return hr;
}

HRESULT UploadAsyncProvider::DoWork(RunContext runContext)
{
    std::lock_guard<std::recursive_mutex> lock(m_folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active

    HRESULT hr = m_folderSync->DoWorkFolderUpload(runContext, *this, m_folderSyncMutex, m_option);
    if (hr != E_PENDING)
    {
        TRACE_TASK(FormatString("UploadAsyncProvider::DoWork HR:0x%0.8x", hr));
        hr = FinishUpload(hr);

        if (SUCCEEDED(hr))
        {
            this->Complete(0);
        }
        else
        {
            // Only now that the upload no longer owns admission: a title that re-adds the user from
            // the active-device-changed callback (the documented recovery after a disconnect) must
            // not be rejected because this upload is still finishing.
            m_folderSync->FirePendingActiveDeviceChangedCallback(runContext);
            this->Fail(hr);
        }
    }

    return hr;
}

HRESULT UploadAsyncProvider::FinishUpload(HRESULT hr)
{
    const bool uploadFailed = FAILED(hr);
    ReleaseReservationOnce(false);
    m_folderSync->GetUIManager().ReleaseUIWaitForTask(*this);
    m_folderSync->ReleaseUploadStaging();

    // Storage handoff failure takes precedence, including over cancellation. A successful
    // cleanup preserves the operation's original result (in particular E_ABORT).
    HRESULT cleanupHr = m_uploadProcessingComplete ?
        m_uploadProcessingComplete() : m_globalState->ApiProvider().UploadProcessingComplete();
    if (FAILED(cleanupHr))
    {
        TRACE_ERROR("UploadProcessingComplete failed: 0x%0.8x (upload result: 0x%0.8x)", cleanupHr, hr);
        hr = cleanupHr;
    }
    if (uploadFailed)
    {
        // A successful upload already released admission. Its cleanup failure must not reset
        // a subsequent upload's reservation.
        m_folderSync->TrySetSyncStateIfOwnedBy({ SyncOperationKind::Upload }, PFGameSaveFilesSyncState::SyncComplete);
    }
    return hr;
}

} // namespace GameSave
} // namespace PlayFab

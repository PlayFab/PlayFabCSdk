// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "DownloadAsyncProvider.h"

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

        if (SUCCEEDED(hr))
        {
            this->Complete(0);
        }
        else
        {
            this->Fail(hr);
        }
    }

    return hr;
}

} // namespace GameSave
} // namespace PlayFab

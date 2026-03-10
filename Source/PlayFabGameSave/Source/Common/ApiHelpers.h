// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "stdafx.h"

#include "GameSaveGlobalState.h"

namespace PlayFab
{
namespace GameSave
{

namespace Detail
{
    HRESULT CompleteAsyncWithError(XAsyncBlock* async, const char* apiIdentity, HRESULT hr);
}

template<typename TWork>
inline HRESULT GameSaveEntityApiImpl(PFLocalUserHandle localUserHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

        SharedPtr<FolderSyncManager> folderSync = state->GetFolderSyncManagerFromLocalUser(localUserHandle, false);
        if (folderSync == nullptr)
        {
            return E_PF_GAMESAVE_USER_NOT_ADDED;
        }
        return work(std::move(folderSync));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary\n    %s:%u", E_FAIL, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT GameSaveEntityAsyncApiImpl(XAsyncBlock* async, const char* apiIdentity, bool createOnDemand, PFLocalUserHandle localUserHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        SharedPtr<FolderSyncManager> folderSync = state->GetFolderSyncManagerFromLocalUser(localUserHandle, createOnDemand);
        if (folderSync == nullptr)
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, E_PF_GAMESAVE_USER_NOT_ADDED);
        }
        return work(std::move(folderSync), state->RunContext().DeriveOnQueue(async->queue));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

} // namespace GameSave
} // namespace PlayFab
// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalUserLoginOperation.h"

namespace PlayFab
{
namespace GameSave
{
 
LocalUserLoginOperation::LocalUserLoginOperation(
    LocalUser&& localUser,
    PlayFab::RunContext&& rc
) :
    XAsyncOperation{ std::move(rc) },
    m_localUser{ std::move(localUser) }
{
}

PlayFab::AsyncOp<PlayFab::GameSave::LoginResult> LocalUserLoginOperation::Run(LocalUser localUser, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<LocalUserLoginOperation>(std::move(localUser), std::move(rc)));
}

HRESULT LocalUserLoginOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLocalUserLoginAsync(m_localUser.Handle(), true, async);
}

Result<LoginResult> LocalUserLoginOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFLocalUserLoginGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFLocalUserLoginGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

} // namespace GameSave
} // namespace PlayFab
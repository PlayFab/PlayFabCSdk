// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>

namespace PlayFab
{
namespace GameSave
{

struct LoginResult
{
    Entity entity;
    Wrappers::PFAuthenticationLoginResultWrapper<Allocator> loginResult;
};

class LocalUserLoginOperation : public XAsyncOperation<LoginResult>
{
public:
    LocalUserLoginOperation(LocalUser&& localUser, PlayFab::RunContext&& rc);
    static AsyncOp<LoginResult> Run(LocalUser localUser, PlayFab::RunContext rc) noexcept;

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;

    LocalUser m_localUser;
};

} // namespace GameSave
} // namespace PlayFab
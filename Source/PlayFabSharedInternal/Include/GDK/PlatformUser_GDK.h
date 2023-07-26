#pragma once

#include "AsyncOp.h"
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>

namespace PlayFab
{
namespace Platform
{

struct TokenAndSignature
{
    String token;
    String signature;
};

// RAII wrapper around XUserHandle
class User
{
public:
    // XUsers must be copied with User::Duplicate
    User(const User& other) = delete;
    User(User&& other) noexcept = default;
    User& operator=(User&& other) noexcept = default;
    ~User() noexcept = default;

    static Result<User> Wrap(XUserHandle userHandle) noexcept;
    static Result<User> Duplicate(XUserHandle userHandle) noexcept;
    static AsyncOp<User> Add(
        XUserAddOptions options,
        RunContext rc
    ) noexcept;

    XUserHandle Handle() const noexcept;
    uint64_t Id() const noexcept;

    AsyncOp<TokenAndSignature> GetTokenAndSignature(
        RunContext runContext
    ) const noexcept;

private:
    User(XUserHandle userHandle) noexcept;

    HRESULT Initialize() noexcept;

    PlayFab::Wrappers::XUser m_user;
    mutable uint64_t m_id{ 0 };
};

} // namespace Platform

using XUser = Platform::User;

} // namespace PlayFab

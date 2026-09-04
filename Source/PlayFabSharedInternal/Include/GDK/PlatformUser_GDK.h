#pragma once

#include <atomic>
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
    User(User&& other) noexcept
        : m_user(std::move(other.m_user))
        , m_id(other.m_id.load())
    {
        other.m_id.store(0);
    }
    User& operator=(User&& other) noexcept
    {
        if (this != &other)
        {
            m_user = std::move(other.m_user);
            m_id.store(other.m_id.load());
            other.m_id.store(0);
        }
        return *this;
    }
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
    mutable std::atomic<uint64_t> m_id{ 0 };
};

} // namespace Platform

using XUser = Platform::User;

} // namespace PlayFab

#pragma once

#include "AsyncOp.h"
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>

namespace PlayFab
{

struct TokenAndSignature
{
    String token;
    String signature;
};

// Wrapper of GDK XUser. Compared to the public PlayFab::Weappers::XUser class, this internal only
// class uses exception free error handling, and has wrappers of additional XUser APIs rather than just
// managing XUserHandle duplication.
class XUser
{
public:
    // XUsers must be copied with User::Duplicate
    XUser(const XUser& other) = delete;
    XUser(XUser&& other) noexcept = default;
    XUser& operator=(XUser&& other) noexcept = default;
    ~XUser() noexcept = default;

    static Result<XUser> Wrap(XUserHandle userHandle) noexcept;
    static Result<XUser> Duplicate(XUserHandle userHandle) noexcept;
    static AsyncOp<XUser> Add(
        XUserAddOptions options,
        RunContext rc
    ) noexcept;

    XUserHandle Handle() const noexcept;
    uint64_t Id() const noexcept;

    AsyncOp<TokenAndSignature> GetTokenAndSignature(
        RunContext runContext
    ) const noexcept;

private:
    XUser(XUserHandle userHandle) noexcept;

    HRESULT Initialize() noexcept;

    PlayFab::Wrappers::XUser m_user;
    mutable uint64_t m_id{ 0 };
};

} // namespace PlayFab

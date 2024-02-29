#pragma once

#include "PlatformUser.h"

namespace PlayFab
{
namespace Platform
{

#ifdef __cplusplus
extern "C"
{
#endif

// This is a Swift method that we call to get the Identity Token when using Apple Sign-In
extern const char* GetIdentityToken();

#ifdef __cplusplus
}
#endif

class User : public std::enable_shared_from_this<User>
{
public:
    User(User const&) = delete;
    User(User&&) noexcept;
    User& operator=(User const&) = delete;
    ~User() noexcept;

    AsyncOp<String> GetUserIdentityToken(RunContext rc) const noexcept;
private:
    Result<String> GetUserIdentityToken() const noexcept;

    String m_identityToken;
    bool m_closeUser{ false };
};

}
}

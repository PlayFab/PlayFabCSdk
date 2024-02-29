#pragma once

#include "PlatformUser.h"

namespace PlayFab
{
namespace Platform
{

class User : public std::enable_shared_from_this<User>
{
public:
    User(User const&) = delete;
    User(User&&) noexcept;
    User& operator=(User const&) = delete;
    ~User() noexcept;
    User() noexcept;
};

using UserPtr = SharedPtr<User>;

}
}

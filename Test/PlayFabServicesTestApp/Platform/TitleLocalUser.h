#pragma once

#include <playfab/core/cpp/LocalUser.h>

namespace PlayFab
{
namespace Test
{

using LocalUserHandleWrapper = Wrappers::LocalUser<Allocator>;

// Base class for title's platform specific LocalUser context
struct TitleLocalUserContext
{
    virtual ~TitleLocalUserContext() = default;
};

// Title's representation of a LocalUser, including a PFLocalUserHandle wrapper as well as a platform specific context.
struct TitleLocalUser
{
    LocalUserHandleWrapper const handleWrapper;
    SharedPtr<TitleLocalUserContext> const context;
};

}
}

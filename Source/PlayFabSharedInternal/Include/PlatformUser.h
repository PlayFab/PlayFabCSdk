#pragma once

#include <XAsyncOperation.h>

namespace PlayFab
{
namespace Platform
{
// Opaque forward declaration of a Platform User. This is currently used in the PlayFabServicesTestApp to cache a generic reference
// to a platform user to be used by Platform specific tests. A definition of this class doesn't necessarily need to exist for every
// Platform.
// 
// It may make sense to define a common interface for this, but keep it completely opaque for now.
class User;

using UserPtr = SharedPtr<User>;

}
}

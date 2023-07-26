#pragma once

#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
using Entity = Wrappers::Entity<Allocator>;
using ServiceConfig = Wrappers::ServiceConfig<Allocator>;

// Shared Classes
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

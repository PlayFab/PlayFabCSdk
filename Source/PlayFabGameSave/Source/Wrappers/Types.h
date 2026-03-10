#pragma once

#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>
#include <playfab/core/PFTypes.h>
#include "playfab/core/cpp/EventPipeline.h"

namespace PlayFab
{

using Entity = Wrappers::Entity<Allocator>;
using ServiceConfig = Wrappers::ServiceConfig<Allocator>;
using EventPipeline = Wrappers::EventPipeline;

} // namespace PlayFab

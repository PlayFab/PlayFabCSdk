// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>
#include <playfab/core/cpp/EventPipeline.h>
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#include "Types.h"

namespace PlayFab
{
namespace Test
{

using ServiceConfig = Wrappers::ServiceConfig<Allocator>;
using EventPipeline = Wrappers::EventPipeline;
using Entity = Wrappers::Entity<Allocator>;
using EntityKey = Wrappers::PFEntityKeyWrapper<Allocator>;
template<typename ModelWrapperT>
using ModelVector = Wrappers::ModelVector<ModelWrapperT, Allocator>;

struct TestTitleData
{
    String titleId;
    String secretKey;
    String connectionString;
};

struct LoginResult
{
    Entity entity;
    Wrappers::PFAuthenticationLoginResultWrapper<Allocator> loginResult;
};

}
}

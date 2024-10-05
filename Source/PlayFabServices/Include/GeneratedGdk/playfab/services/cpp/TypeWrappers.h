// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFVersionConfigurationWrapper : public ModelWrapper<PFVersionConfiguration, Alloc>
{
public:
    using ModelType = PFVersionConfiguration;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFVersionConfiguration, Alloc>::ModelWrapper;

    void SetMaxQueryableVersions(int32_t value)
    {
        this->m_model.maxQueryableVersions = value;
    }

    void SetResetInterval(PFResetInterval value)
    {
        this->m_model.resetInterval = value;
    }

private:
};

} // namespace Wrappers
} // namespace PlayFab

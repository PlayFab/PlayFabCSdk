// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFCore.h>
#include <string>
#include <assert.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class ServiceConfig
{
public:
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T>
    using Vector = typename std::vector<T, Allocator<T>>;

    // Creates a ServiceConfig by wrapping a PFServiceConfigHandle (transfers ownership of the handle)
    static ServiceConfig Wrap(PFServiceConfigHandle handle)
    {
        return ServiceConfig{ handle };
    }

    // Creates a TitlePlayer by duplicating a PFServiceConfigHandle (caller still owns provided handle)
    static ServiceConfig Duplicate(PFServiceConfigHandle handle)
    {
        PFServiceConfigHandle duplicatedHandle;
        THROW_IF_FAILED(PFServiceConfigDuplicateHandle(handle, &duplicatedHandle));
        return ServiceConfig{ duplicatedHandle };
    }

    ServiceConfig(_In_z_ const char* apiEndpoint, _In_z_ const char* titleId)
    {
        THROW_IF_FAILED(PFServiceConfigCreateHandle(apiEndpoint, titleId, &m_handle));
    }

    ServiceConfig(ServiceConfig const& other)
    {
        THROW_IF_FAILED(PFServiceConfigDuplicateHandle(other.m_handle, &m_handle));
    }

    ServiceConfig(ServiceConfig&& other)
    {
        std::swap(m_handle, other.m_handle);
    }

    ServiceConfig& operator=(ServiceConfig other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    virtual ~ServiceConfig()
    {
        if (m_handle)
        {
            PFServiceConfigCloseHandle(m_handle);
        }
    }

    PFServiceConfigHandle Handle() const noexcept
    {
        return m_handle;
    }

    String APIEndpoint() const
    {
        size_t size{ 0 };
        THROW_IF_FAILED(PFServiceConfigGetAPIEndpointSize(m_handle, &size));

        Vector<char> apiEndpoint(size, '\0');
        THROW_IF_FAILED(PFServiceConfigGetAPIEndpoint(m_handle, size, &apiEndpoint[0], nullptr));

        return String{ apiEndpoint.data() };
    }

    String TitleId() const
    {
        size_t size{ 0 };
        THROW_IF_FAILED(PFServiceConfigGetTitleIdSize(m_handle, &size));

        Vector<char> titleId(size, '\0');
        THROW_IF_FAILED(PFServiceConfigGetTitleId(m_handle, titleId.size(), &titleId[0], nullptr));

        return String{ titleId.data() };
    }

private:
    ServiceConfig(PFServiceConfigHandle handle) : m_handle{ handle }
    {
    }

    PFServiceConfigHandle m_handle{ nullptr };
};

}
}

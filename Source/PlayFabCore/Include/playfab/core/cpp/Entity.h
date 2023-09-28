// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include "PlayFabException.h"
#include <string>


namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class Entity
{
public:
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T>
    using Vector = typename std::vector<T, Alloc<T>>;

    // Creates a Entity by wrapping a PFEntityHandle (transfers ownership of the handle)
    static Entity Wrap(PFEntityHandle handle)
    {
        return Entity{ handle };
    }

    // Creates a Entity by duplicating a PFEntityHandle (caller still owns provided handle)
    static Entity Duplicate(PFEntityHandle handle)
    {
        PFEntityHandle duplicatedHandle;
        THROW_IF_FAILED(PFEntityDuplicateHandle(handle, &duplicatedHandle));
        return Entity{ duplicatedHandle };
    }

    Entity(Entity const& other)
    {
        THROW_IF_FAILED(PFEntityDuplicateHandle(other.m_handle, &m_handle)); 
    }

    Entity(Entity&& other)
    {
        std::swap(m_handle, other.m_handle);
    }

    Entity& operator=(Entity other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    virtual ~Entity()
    {
        if (m_handle)
        {
            PFEntityCloseHandle(m_handle);
        }
    }

    PFEntityHandle Handle() const noexcept
    {
        return m_handle;
    }

    PFEntityKeyWrapper<Alloc> EntityKey() const
    {
        size_t size{};
        THROW_IF_FAILED(PFEntityGetEntityKeySize(m_handle, &size));

        PFEntityKey const* pEntityKey{};
        Vector<char> buffer(size);
        THROW_IF_FAILED(PFEntityGetEntityKey(m_handle, buffer.size(), buffer.data(), &pEntityKey, nullptr));

        return *pEntityKey;
    }

    // Gets secretKey if available and returns E_PF_NOSECRETKEY otherwise
    HRESULT TryGetSecretKey(String& secretKey) const
    {
        try
        {
#if HC_PLATFORM == HC_PLATFORM_WIN32
            size_t size{ 0 };
            THROW_IF_FAILED(PFEntityGetSecretKeySize(m_handle, &size));

            Vector<char> secretKeyBuffer(size, '\0');
            THROW_IF_FAILED(PFEntityGetSecretKey(m_handle, secretKeyBuffer.size(), secretKeyBuffer.data(), nullptr));

            secretKey = secretKeyBuffer.data();
            return S_OK;
#else   
            UNREFERENCED_PARAMETER(secretKey);
            assert(false);
            return S_OK;
#endif
        }
        catch (Wrappers::Exception& e)
        {
            if (e.hr == E_PF_NOSECRETKEY)
            {
                return e.hr;
            }
            throw e;
        }
    }

    String APIEndpoint() const
    {
        size_t size{ 0 };
        THROW_IF_FAILED(PFEntityGetAPIEndpointSize(m_handle, &size));

        Vector<char> apiEndpoint(size, '\0');
        THROW_IF_FAILED(PFEntityGetAPIEndpoint(m_handle, size, &apiEndpoint[0], nullptr));

        return String{ apiEndpoint.data() };
    }

    bool IsTitlePlayer() const
    {
        bool isTitlePlayer;
        THROW_IF_FAILED(PFEntityIsTitlePlayer(m_handle, &isTitlePlayer));
        return isTitlePlayer;
    }

private:
    Entity(PFEntityHandle handle) : m_handle{ handle }
    {
    }

    PFEntityHandle m_handle{ nullptr };
};

}
}

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include <playfab/core/PFLocalUser.h>
#include "PlayFabException.h"
#include <string>
#include <assert.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class LocalUser
{
public:
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T>
    using Vector = typename std::vector<T, Alloc<T>>;

    // Creates a LocalUser by wrapping a PFLocalUserHandle (transfers ownership of the handle)
    static LocalUser Wrap(PFLocalUserHandle handle)
    {
        return LocalUser{ handle };
    }

    // Creates a Entity by duplicating a PFLocalUserHandle (caller still owns provided handle)
    static LocalUser Duplicate(PFLocalUserHandle handle)
    {
        PFLocalUserHandle duplicatedHandle;
        THROW_IF_FAILED(PFLocalUserDuplicateHandle(handle, &duplicatedHandle));
        return LocalUser{ duplicatedHandle };
    }

    LocalUser(LocalUser const& other)
    {
        THROW_IF_FAILED(PFLocalUserDuplicateHandle(other.m_handle, &m_handle));
    }

    LocalUser(LocalUser&& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
    }

    LocalUser& operator=(LocalUser other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    virtual ~LocalUser()
    {
        if (m_handle)
        {
            PFLocalUserCloseHandle(m_handle);
        }
    }

    PFLocalUserHandle Handle() const noexcept
    {
        return m_handle;
    }

    String LocalId() const
    {
        size_t size{ 0 };
        THROW_IF_FAILED(PFLocalUserGetLocalIdSize(m_handle, &size));

        Vector<char> localId(size, '\0');
        THROW_IF_FAILED(PFLocalUserGetLocalId(m_handle, size, &localId[0], nullptr));

        return String{ localId.data() };
    }

private:
    LocalUser(PFLocalUserHandle handle) : m_handle{ handle }
    {
    }

    PFLocalUserHandle m_handle{ nullptr };
};

}
}

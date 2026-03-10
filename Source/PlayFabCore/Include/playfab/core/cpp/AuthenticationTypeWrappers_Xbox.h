// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/cpp/TypeWrappers.h>
#include <playfab/core/PFAuthenticationTypes.h>

namespace PlayFab
{
namespace Wrappers
{

class XUser
{
public:
    static XUser Wrap(XUserHandle handle)
    {
        return XUser{ handle };
    }

    static XUser Duplicate(XUserHandle handle)
    {
        XUserHandle duplicatedHandle;
        THROW_IF_FAILED(XUserDuplicateHandle(handle, &duplicatedHandle));
        return XUser{ duplicatedHandle };
    }

    XUser(XUser const& other)
    {
        THROW_IF_FAILED(XUserDuplicateHandle(other.m_handle, &m_handle));
    }

    XUser(XUser&& other)
    {
        std::swap(other.m_handle, m_handle);
    }

    XUser& operator=(XUser other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    ~XUser()
    {
        if (m_handle)
        {
            XUserCloseHandle(m_handle);
        }
    }

    XUserHandle Handle() const noexcept
    {
        return m_handle;
    }

private:
    XUser(XUserHandle handle) : m_handle{ handle }
    {
    }

    XUserHandle m_handle{ nullptr };
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithXUserRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithXUserRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithXUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithXUserRequestWrapper(XUser user)
        : m_user{ std::move(user) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXUserRequestWrapper(const PFAuthenticationLoginWithXUserRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithXUserRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_user{ XUser::Duplicate(model.user) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXUserRequestWrapper(const PFAuthenticationLoginWithXUserRequestWrapper& src) :
        PFAuthenticationLoginWithXUserRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithXUserRequestWrapper(PFAuthenticationLoginWithXUserRequestWrapper&& src) :
        m_user{ XUser::Wrap(nullptr) }
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithXUserRequestWrapper& operator=(PFAuthenticationLoginWithXUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithXUserRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithXUserRequestWrapper& lhs, PFAuthenticationLoginWithXUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_user, rhs.m_user);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    void SetUser(XUser value)
    {
        m_user = std::move(value);
        this->m_model.user = m_user.Handle();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.user = m_user.Handle();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    XUser m_user;
};

} // namespace Wrappers
} // namespace PlayFab
#endif // HC_PLATFORM_GDK
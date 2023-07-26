// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFGroupsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsAcceptGroupApplicationRequestWrapper : public ModelWrapper<PFGroupsAcceptGroupApplicationRequest, Alloc>
{
public:
    using ModelType = PFGroupsAcceptGroupApplicationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsAcceptGroupApplicationRequestWrapper() = default;

    PFGroupsAcceptGroupApplicationRequestWrapper(const PFGroupsAcceptGroupApplicationRequest& model) :
        ModelWrapper<PFGroupsAcceptGroupApplicationRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsAcceptGroupApplicationRequestWrapper(const PFGroupsAcceptGroupApplicationRequestWrapper& src) :
        PFGroupsAcceptGroupApplicationRequestWrapper{ src.Model() }
    {
    }

    PFGroupsAcceptGroupApplicationRequestWrapper(PFGroupsAcceptGroupApplicationRequestWrapper&& src) :
        PFGroupsAcceptGroupApplicationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsAcceptGroupApplicationRequestWrapper& operator=(PFGroupsAcceptGroupApplicationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsAcceptGroupApplicationRequestWrapper() = default;

    friend void swap(PFGroupsAcceptGroupApplicationRequestWrapper& lhs, PFGroupsAcceptGroupApplicationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsAcceptGroupInvitationRequestWrapper : public ModelWrapper<PFGroupsAcceptGroupInvitationRequest, Alloc>
{
public:
    using ModelType = PFGroupsAcceptGroupInvitationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsAcceptGroupInvitationRequestWrapper() = default;

    PFGroupsAcceptGroupInvitationRequestWrapper(const PFGroupsAcceptGroupInvitationRequest& model) :
        ModelWrapper<PFGroupsAcceptGroupInvitationRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsAcceptGroupInvitationRequestWrapper(const PFGroupsAcceptGroupInvitationRequestWrapper& src) :
        PFGroupsAcceptGroupInvitationRequestWrapper{ src.Model() }
    {
    }

    PFGroupsAcceptGroupInvitationRequestWrapper(PFGroupsAcceptGroupInvitationRequestWrapper&& src) :
        PFGroupsAcceptGroupInvitationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsAcceptGroupInvitationRequestWrapper& operator=(PFGroupsAcceptGroupInvitationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsAcceptGroupInvitationRequestWrapper() = default;

    friend void swap(PFGroupsAcceptGroupInvitationRequestWrapper& lhs, PFGroupsAcceptGroupInvitationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsAddMembersRequestWrapper : public ModelWrapper<PFGroupsAddMembersRequest, Alloc>
{
public:
    using ModelType = PFGroupsAddMembersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsAddMembersRequestWrapper() = default;

    PFGroupsAddMembersRequestWrapper(const PFGroupsAddMembersRequest& model) :
        ModelWrapper<PFGroupsAddMembersRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_members{ model.members, model.members + model.membersCount },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsAddMembersRequestWrapper(const PFGroupsAddMembersRequestWrapper& src) :
        PFGroupsAddMembersRequestWrapper{ src.Model() }
    {
    }

    PFGroupsAddMembersRequestWrapper(PFGroupsAddMembersRequestWrapper&& src) :
        PFGroupsAddMembersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsAddMembersRequestWrapper& operator=(PFGroupsAddMembersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsAddMembersRequestWrapper() = default;

    friend void swap(PFGroupsAddMembersRequestWrapper& lhs, PFGroupsAddMembersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_members, rhs.m_members);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetMembers(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_members = std::move(value);
        this->m_model.members =  m_members.empty() ? nullptr : m_members.data();
        this->m_model.membersCount =  static_cast<uint32_t>(m_members.size());
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
        this->m_model.members = m_members.empty() ? nullptr : m_members.data();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_members;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsApplyToGroupRequestWrapper : public ModelWrapper<PFGroupsApplyToGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsApplyToGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsApplyToGroupRequestWrapper() = default;

    PFGroupsApplyToGroupRequestWrapper(const PFGroupsApplyToGroupRequest& model) :
        ModelWrapper<PFGroupsApplyToGroupRequest, Alloc>{ model },
        m_autoAcceptOutstandingInvite{ model.autoAcceptOutstandingInvite ? std::optional<bool>{ *model.autoAcceptOutstandingInvite } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsApplyToGroupRequestWrapper(const PFGroupsApplyToGroupRequestWrapper& src) :
        PFGroupsApplyToGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsApplyToGroupRequestWrapper(PFGroupsApplyToGroupRequestWrapper&& src) :
        PFGroupsApplyToGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsApplyToGroupRequestWrapper& operator=(PFGroupsApplyToGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsApplyToGroupRequestWrapper() = default;

    friend void swap(PFGroupsApplyToGroupRequestWrapper& lhs, PFGroupsApplyToGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_autoAcceptOutstandingInvite, rhs.m_autoAcceptOutstandingInvite);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAutoAcceptOutstandingInvite(std::optional<bool> value)
    {
        m_autoAcceptOutstandingInvite = std::move(value);
        this->m_model.autoAcceptOutstandingInvite = m_autoAcceptOutstandingInvite ? m_autoAcceptOutstandingInvite.operator->() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.autoAcceptOutstandingInvite = m_autoAcceptOutstandingInvite ? m_autoAcceptOutstandingInvite.operator->() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.group = &m_group.Model();
    }

    std::optional<bool> m_autoAcceptOutstandingInvite;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsEntityWithLineageWrapper : public ModelWrapper<PFGroupsEntityWithLineage, Alloc>
{
public:
    using ModelType = PFGroupsEntityWithLineage;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsEntityWithLineageWrapper() = default;

    PFGroupsEntityWithLineageWrapper(const PFGroupsEntityWithLineage& model) :
        ModelWrapper<PFGroupsEntityWithLineage, Alloc>{ model },
        m_key{ model.key ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.key } : std::nullopt },
        m_lineage{ model.lineage, model.lineage + model.lineageCount }
    {
        SetModelPointers();
    }

    PFGroupsEntityWithLineageWrapper(const PFGroupsEntityWithLineageWrapper& src) :
        PFGroupsEntityWithLineageWrapper{ src.Model() }
    {
    }

    PFGroupsEntityWithLineageWrapper(PFGroupsEntityWithLineageWrapper&& src) :
        PFGroupsEntityWithLineageWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsEntityWithLineageWrapper& operator=(PFGroupsEntityWithLineageWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsEntityWithLineageWrapper() = default;

    friend void swap(PFGroupsEntityWithLineageWrapper& lhs, PFGroupsEntityWithLineageWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_key, rhs.m_key);
        swap(lhs.m_lineage, rhs.m_lineage);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKey(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_key = std::move(value);
        this->m_model.key = m_key ? &m_key->Model() : nullptr;
    }

    void SetLineage(ModelDictionaryEntryVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_lineage = std::move(value);
        this->m_model.lineage =  m_lineage.empty() ? nullptr : m_lineage.data();
        this->m_model.lineageCount =  static_cast<uint32_t>(m_lineage.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.key = m_key ?  &m_key->Model() : nullptr;
        this->m_model.lineage = m_lineage.empty() ? nullptr : m_lineage.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_key;
    ModelDictionaryEntryVector<PFEntityKeyWrapper<Alloc>, Alloc> m_lineage;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsApplyToGroupResponseWrapper : public ModelWrapper<PFGroupsApplyToGroupResponse, Alloc>
{
public:
    using ModelType = PFGroupsApplyToGroupResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsApplyToGroupResponseWrapper() = default;

    PFGroupsApplyToGroupResponseWrapper(const PFGroupsApplyToGroupResponse& model) :
        ModelWrapper<PFGroupsApplyToGroupResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsApplyToGroupResponseWrapper(const PFGroupsApplyToGroupResponseWrapper& src) :
        PFGroupsApplyToGroupResponseWrapper{ src.Model() }
    {
    }

    PFGroupsApplyToGroupResponseWrapper(PFGroupsApplyToGroupResponseWrapper&& src) :
        PFGroupsApplyToGroupResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsApplyToGroupResponseWrapper& operator=(PFGroupsApplyToGroupResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsApplyToGroupResponseWrapper() = default;

    friend void swap(PFGroupsApplyToGroupResponseWrapper& lhs, PFGroupsApplyToGroupResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetExpires(time_t value)
    {
        this->m_model.expires = value;
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
    }

    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_entity;
    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsBlockEntityRequestWrapper : public ModelWrapper<PFGroupsBlockEntityRequest, Alloc>
{
public:
    using ModelType = PFGroupsBlockEntityRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsBlockEntityRequestWrapper() = default;

    PFGroupsBlockEntityRequestWrapper(const PFGroupsBlockEntityRequest& model) :
        ModelWrapper<PFGroupsBlockEntityRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsBlockEntityRequestWrapper(const PFGroupsBlockEntityRequestWrapper& src) :
        PFGroupsBlockEntityRequestWrapper{ src.Model() }
    {
    }

    PFGroupsBlockEntityRequestWrapper(PFGroupsBlockEntityRequestWrapper&& src) :
        PFGroupsBlockEntityRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsBlockEntityRequestWrapper& operator=(PFGroupsBlockEntityRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsBlockEntityRequestWrapper() = default;

    friend void swap(PFGroupsBlockEntityRequestWrapper& lhs, PFGroupsBlockEntityRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsChangeMemberRoleRequestWrapper : public ModelWrapper<PFGroupsChangeMemberRoleRequest, Alloc>
{
public:
    using ModelType = PFGroupsChangeMemberRoleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsChangeMemberRoleRequestWrapper() = default;

    PFGroupsChangeMemberRoleRequestWrapper(const PFGroupsChangeMemberRoleRequest& model) :
        ModelWrapper<PFGroupsChangeMemberRoleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_destinationRoleId{ SafeString(model.destinationRoleId) },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_members{ model.members, model.members + model.membersCount },
        m_originRoleId{ SafeString(model.originRoleId) }
    {
        SetModelPointers();
    }

    PFGroupsChangeMemberRoleRequestWrapper(const PFGroupsChangeMemberRoleRequestWrapper& src) :
        PFGroupsChangeMemberRoleRequestWrapper{ src.Model() }
    {
    }

    PFGroupsChangeMemberRoleRequestWrapper(PFGroupsChangeMemberRoleRequestWrapper&& src) :
        PFGroupsChangeMemberRoleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsChangeMemberRoleRequestWrapper& operator=(PFGroupsChangeMemberRoleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsChangeMemberRoleRequestWrapper() = default;

    friend void swap(PFGroupsChangeMemberRoleRequestWrapper& lhs, PFGroupsChangeMemberRoleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_destinationRoleId, rhs.m_destinationRoleId);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_members, rhs.m_members);
        swap(lhs.m_originRoleId, rhs.m_originRoleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDestinationRoleId(String value)
    {
        m_destinationRoleId = std::move(value);
        this->m_model.destinationRoleId =  m_destinationRoleId.empty() ? nullptr : m_destinationRoleId.data();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetMembers(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_members = std::move(value);
        this->m_model.members =  m_members.empty() ? nullptr : m_members.data();
        this->m_model.membersCount =  static_cast<uint32_t>(m_members.size());
    }

    void SetOriginRoleId(String value)
    {
        m_originRoleId = std::move(value);
        this->m_model.originRoleId =  m_originRoleId.empty() ? nullptr : m_originRoleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.destinationRoleId = m_destinationRoleId.empty() ? nullptr : m_destinationRoleId.data();
        this->m_model.group = &m_group.Model();
        this->m_model.members = m_members.empty() ? nullptr : m_members.data();
        this->m_model.originRoleId = m_originRoleId.empty() ? nullptr : m_originRoleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_destinationRoleId;
    PFEntityKeyWrapper<Alloc> m_group;
    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_members;
    String m_originRoleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsCreateGroupRequestWrapper : public ModelWrapper<PFGroupsCreateGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsCreateGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsCreateGroupRequestWrapper() = default;

    PFGroupsCreateGroupRequestWrapper(const PFGroupsCreateGroupRequest& model) :
        ModelWrapper<PFGroupsCreateGroupRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_groupName{ SafeString(model.groupName) }
    {
        SetModelPointers();
    }

    PFGroupsCreateGroupRequestWrapper(const PFGroupsCreateGroupRequestWrapper& src) :
        PFGroupsCreateGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsCreateGroupRequestWrapper(PFGroupsCreateGroupRequestWrapper&& src) :
        PFGroupsCreateGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsCreateGroupRequestWrapper& operator=(PFGroupsCreateGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsCreateGroupRequestWrapper() = default;

    friend void swap(PFGroupsCreateGroupRequestWrapper& lhs, PFGroupsCreateGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_groupName, rhs.m_groupName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_groupName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsCreateGroupResponseWrapper : public ModelWrapper<PFGroupsCreateGroupResponse, Alloc>
{
public:
    using ModelType = PFGroupsCreateGroupResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsCreateGroupResponseWrapper() = default;

    PFGroupsCreateGroupResponseWrapper(const PFGroupsCreateGroupResponse& model) :
        ModelWrapper<PFGroupsCreateGroupResponse, Alloc>{ model },
        m_adminRoleId{ SafeString(model.adminRoleId) },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_groupName{ SafeString(model.groupName) },
        m_memberRoleId{ SafeString(model.memberRoleId) },
        m_roles{ model.roles, model.roles + model.rolesCount }
    {
        SetModelPointers();
    }

    PFGroupsCreateGroupResponseWrapper(const PFGroupsCreateGroupResponseWrapper& src) :
        PFGroupsCreateGroupResponseWrapper{ src.Model() }
    {
    }

    PFGroupsCreateGroupResponseWrapper(PFGroupsCreateGroupResponseWrapper&& src) :
        PFGroupsCreateGroupResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsCreateGroupResponseWrapper& operator=(PFGroupsCreateGroupResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsCreateGroupResponseWrapper() = default;

    friend void swap(PFGroupsCreateGroupResponseWrapper& lhs, PFGroupsCreateGroupResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adminRoleId, rhs.m_adminRoleId);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_groupName, rhs.m_groupName);
        swap(lhs.m_memberRoleId, rhs.m_memberRoleId);
        swap(lhs.m_roles, rhs.m_roles);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdminRoleId(String value)
    {
        m_adminRoleId = std::move(value);
        this->m_model.adminRoleId =  m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

    void SetMemberRoleId(String value)
    {
        m_memberRoleId = std::move(value);
        this->m_model.memberRoleId =  m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetRoles(StringDictionaryEntryVector<Alloc> value)
    {
        m_roles = std::move(value);
        this->m_model.roles =  m_roles.empty() ? nullptr : m_roles.data();
        this->m_model.rolesCount =  static_cast<uint32_t>(m_roles.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.adminRoleId = m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
        this->m_model.group = &m_group.Model();
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
        this->m_model.memberRoleId = m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
        this->m_model.roles = m_roles.empty() ? nullptr : m_roles.data();
    }

    String m_adminRoleId;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_groupName;
    String m_memberRoleId;
    StringDictionaryEntryVector<Alloc> m_roles;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsCreateGroupRoleRequestWrapper : public ModelWrapper<PFGroupsCreateGroupRoleRequest, Alloc>
{
public:
    using ModelType = PFGroupsCreateGroupRoleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsCreateGroupRoleRequestWrapper() = default;

    PFGroupsCreateGroupRoleRequestWrapper(const PFGroupsCreateGroupRoleRequest& model) :
        ModelWrapper<PFGroupsCreateGroupRoleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_roleId{ SafeString(model.roleId) },
        m_roleName{ SafeString(model.roleName) }
    {
        SetModelPointers();
    }

    PFGroupsCreateGroupRoleRequestWrapper(const PFGroupsCreateGroupRoleRequestWrapper& src) :
        PFGroupsCreateGroupRoleRequestWrapper{ src.Model() }
    {
    }

    PFGroupsCreateGroupRoleRequestWrapper(PFGroupsCreateGroupRoleRequestWrapper&& src) :
        PFGroupsCreateGroupRoleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsCreateGroupRoleRequestWrapper& operator=(PFGroupsCreateGroupRoleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsCreateGroupRoleRequestWrapper() = default;

    friend void swap(PFGroupsCreateGroupRoleRequestWrapper& lhs, PFGroupsCreateGroupRoleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_roleId, rhs.m_roleId);
        swap(lhs.m_roleName, rhs.m_roleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

    void SetRoleName(String value)
    {
        m_roleName = std::move(value);
        this->m_model.roleName =  m_roleName.empty() ? nullptr : m_roleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
        this->m_model.roleName = m_roleName.empty() ? nullptr : m_roleName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_roleId;
    String m_roleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsCreateGroupRoleResponseWrapper : public ModelWrapper<PFGroupsCreateGroupRoleResponse, Alloc>
{
public:
    using ModelType = PFGroupsCreateGroupRoleResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsCreateGroupRoleResponseWrapper() = default;

    PFGroupsCreateGroupRoleResponseWrapper(const PFGroupsCreateGroupRoleResponse& model) :
        ModelWrapper<PFGroupsCreateGroupRoleResponse, Alloc>{ model },
        m_roleId{ SafeString(model.roleId) },
        m_roleName{ SafeString(model.roleName) }
    {
        SetModelPointers();
    }

    PFGroupsCreateGroupRoleResponseWrapper(const PFGroupsCreateGroupRoleResponseWrapper& src) :
        PFGroupsCreateGroupRoleResponseWrapper{ src.Model() }
    {
    }

    PFGroupsCreateGroupRoleResponseWrapper(PFGroupsCreateGroupRoleResponseWrapper&& src) :
        PFGroupsCreateGroupRoleResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsCreateGroupRoleResponseWrapper& operator=(PFGroupsCreateGroupRoleResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsCreateGroupRoleResponseWrapper() = default;

    friend void swap(PFGroupsCreateGroupRoleResponseWrapper& lhs, PFGroupsCreateGroupRoleResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_roleId, rhs.m_roleId);
        swap(lhs.m_roleName, rhs.m_roleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

    void SetRoleName(String value)
    {
        m_roleName = std::move(value);
        this->m_model.roleName =  m_roleName.empty() ? nullptr : m_roleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
        this->m_model.roleName = m_roleName.empty() ? nullptr : m_roleName.data();
    }

    String m_roleId;
    String m_roleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsDeleteGroupRequestWrapper : public ModelWrapper<PFGroupsDeleteGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsDeleteGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsDeleteGroupRequestWrapper() = default;

    PFGroupsDeleteGroupRequestWrapper(const PFGroupsDeleteGroupRequest& model) :
        ModelWrapper<PFGroupsDeleteGroupRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsDeleteGroupRequestWrapper(const PFGroupsDeleteGroupRequestWrapper& src) :
        PFGroupsDeleteGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsDeleteGroupRequestWrapper(PFGroupsDeleteGroupRequestWrapper&& src) :
        PFGroupsDeleteGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsDeleteGroupRequestWrapper& operator=(PFGroupsDeleteGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsDeleteGroupRequestWrapper() = default;

    friend void swap(PFGroupsDeleteGroupRequestWrapper& lhs, PFGroupsDeleteGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsDeleteRoleRequestWrapper : public ModelWrapper<PFGroupsDeleteRoleRequest, Alloc>
{
public:
    using ModelType = PFGroupsDeleteRoleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsDeleteRoleRequestWrapper() = default;

    PFGroupsDeleteRoleRequestWrapper(const PFGroupsDeleteRoleRequest& model) :
        ModelWrapper<PFGroupsDeleteRoleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsDeleteRoleRequestWrapper(const PFGroupsDeleteRoleRequestWrapper& src) :
        PFGroupsDeleteRoleRequestWrapper{ src.Model() }
    {
    }

    PFGroupsDeleteRoleRequestWrapper(PFGroupsDeleteRoleRequestWrapper&& src) :
        PFGroupsDeleteRoleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsDeleteRoleRequestWrapper& operator=(PFGroupsDeleteRoleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsDeleteRoleRequestWrapper() = default;

    friend void swap(PFGroupsDeleteRoleRequestWrapper& lhs, PFGroupsDeleteRoleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGetGroupRequestWrapper : public ModelWrapper<PFGroupsGetGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsGetGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGetGroupRequestWrapper() = default;

    PFGroupsGetGroupRequestWrapper(const PFGroupsGetGroupRequest& model) :
        ModelWrapper<PFGroupsGetGroupRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt },
        m_groupName{ SafeString(model.groupName) }
    {
        SetModelPointers();
    }

    PFGroupsGetGroupRequestWrapper(const PFGroupsGetGroupRequestWrapper& src) :
        PFGroupsGetGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsGetGroupRequestWrapper(PFGroupsGetGroupRequestWrapper&& src) :
        PFGroupsGetGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGetGroupRequestWrapper& operator=(PFGroupsGetGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGetGroupRequestWrapper() = default;

    friend void swap(PFGroupsGetGroupRequestWrapper& lhs, PFGroupsGetGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_groupName, rhs.m_groupName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
    String m_groupName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGetGroupResponseWrapper : public ModelWrapper<PFGroupsGetGroupResponse, Alloc>
{
public:
    using ModelType = PFGroupsGetGroupResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGetGroupResponseWrapper() = default;

    PFGroupsGetGroupResponseWrapper(const PFGroupsGetGroupResponse& model) :
        ModelWrapper<PFGroupsGetGroupResponse, Alloc>{ model },
        m_adminRoleId{ SafeString(model.adminRoleId) },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_groupName{ SafeString(model.groupName) },
        m_memberRoleId{ SafeString(model.memberRoleId) },
        m_roles{ model.roles, model.roles + model.rolesCount }
    {
        SetModelPointers();
    }

    PFGroupsGetGroupResponseWrapper(const PFGroupsGetGroupResponseWrapper& src) :
        PFGroupsGetGroupResponseWrapper{ src.Model() }
    {
    }

    PFGroupsGetGroupResponseWrapper(PFGroupsGetGroupResponseWrapper&& src) :
        PFGroupsGetGroupResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGetGroupResponseWrapper& operator=(PFGroupsGetGroupResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGetGroupResponseWrapper() = default;

    friend void swap(PFGroupsGetGroupResponseWrapper& lhs, PFGroupsGetGroupResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adminRoleId, rhs.m_adminRoleId);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_groupName, rhs.m_groupName);
        swap(lhs.m_memberRoleId, rhs.m_memberRoleId);
        swap(lhs.m_roles, rhs.m_roles);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdminRoleId(String value)
    {
        m_adminRoleId = std::move(value);
        this->m_model.adminRoleId =  m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

    void SetMemberRoleId(String value)
    {
        m_memberRoleId = std::move(value);
        this->m_model.memberRoleId =  m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetRoles(StringDictionaryEntryVector<Alloc> value)
    {
        m_roles = std::move(value);
        this->m_model.roles =  m_roles.empty() ? nullptr : m_roles.data();
        this->m_model.rolesCount =  static_cast<uint32_t>(m_roles.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.adminRoleId = m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
        this->m_model.group = &m_group.Model();
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
        this->m_model.memberRoleId = m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
        this->m_model.roles = m_roles.empty() ? nullptr : m_roles.data();
    }

    String m_adminRoleId;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_groupName;
    String m_memberRoleId;
    StringDictionaryEntryVector<Alloc> m_roles;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsInviteToGroupRequestWrapper : public ModelWrapper<PFGroupsInviteToGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsInviteToGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsInviteToGroupRequestWrapper() = default;

    PFGroupsInviteToGroupRequestWrapper(const PFGroupsInviteToGroupRequest& model) :
        ModelWrapper<PFGroupsInviteToGroupRequest, Alloc>{ model },
        m_autoAcceptOutstandingApplication{ model.autoAcceptOutstandingApplication ? std::optional<bool>{ *model.autoAcceptOutstandingApplication } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsInviteToGroupRequestWrapper(const PFGroupsInviteToGroupRequestWrapper& src) :
        PFGroupsInviteToGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsInviteToGroupRequestWrapper(PFGroupsInviteToGroupRequestWrapper&& src) :
        PFGroupsInviteToGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsInviteToGroupRequestWrapper& operator=(PFGroupsInviteToGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsInviteToGroupRequestWrapper() = default;

    friend void swap(PFGroupsInviteToGroupRequestWrapper& lhs, PFGroupsInviteToGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_autoAcceptOutstandingApplication, rhs.m_autoAcceptOutstandingApplication);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAutoAcceptOutstandingApplication(std::optional<bool> value)
    {
        m_autoAcceptOutstandingApplication = std::move(value);
        this->m_model.autoAcceptOutstandingApplication = m_autoAcceptOutstandingApplication ? m_autoAcceptOutstandingApplication.operator->() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.autoAcceptOutstandingApplication = m_autoAcceptOutstandingApplication ? m_autoAcceptOutstandingApplication.operator->() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    std::optional<bool> m_autoAcceptOutstandingApplication;
    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsInviteToGroupResponseWrapper : public ModelWrapper<PFGroupsInviteToGroupResponse, Alloc>
{
public:
    using ModelType = PFGroupsInviteToGroupResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsInviteToGroupResponseWrapper() = default;

    PFGroupsInviteToGroupResponseWrapper(const PFGroupsInviteToGroupResponse& model) :
        ModelWrapper<PFGroupsInviteToGroupResponse, Alloc>{ model },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt },
        m_invitedByEntity{ model.invitedByEntity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.invitedByEntity } : std::nullopt },
        m_invitedEntity{ model.invitedEntity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.invitedEntity } : std::nullopt },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsInviteToGroupResponseWrapper(const PFGroupsInviteToGroupResponseWrapper& src) :
        PFGroupsInviteToGroupResponseWrapper{ src.Model() }
    {
    }

    PFGroupsInviteToGroupResponseWrapper(PFGroupsInviteToGroupResponseWrapper&& src) :
        PFGroupsInviteToGroupResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsInviteToGroupResponseWrapper& operator=(PFGroupsInviteToGroupResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsInviteToGroupResponseWrapper() = default;

    friend void swap(PFGroupsInviteToGroupResponseWrapper& lhs, PFGroupsInviteToGroupResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_invitedByEntity, rhs.m_invitedByEntity);
        swap(lhs.m_invitedEntity, rhs.m_invitedEntity);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetExpires(time_t value)
    {
        this->m_model.expires = value;
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

    void SetInvitedByEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_invitedByEntity = std::move(value);
        this->m_model.invitedByEntity = m_invitedByEntity ? &m_invitedByEntity->Model() : nullptr;
    }

    void SetInvitedEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_invitedEntity = std::move(value);
        this->m_model.invitedEntity = m_invitedEntity ? &m_invitedEntity->Model() : nullptr;
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
        this->m_model.invitedByEntity = m_invitedByEntity ?  &m_invitedByEntity->Model() : nullptr;
        this->m_model.invitedEntity = m_invitedEntity ?  &m_invitedEntity->Model() : nullptr;
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_invitedByEntity;
    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_invitedEntity;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsIsMemberRequestWrapper : public ModelWrapper<PFGroupsIsMemberRequest, Alloc>
{
public:
    using ModelType = PFGroupsIsMemberRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsIsMemberRequestWrapper() = default;

    PFGroupsIsMemberRequestWrapper(const PFGroupsIsMemberRequest& model) :
        ModelWrapper<PFGroupsIsMemberRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsIsMemberRequestWrapper(const PFGroupsIsMemberRequestWrapper& src) :
        PFGroupsIsMemberRequestWrapper{ src.Model() }
    {
    }

    PFGroupsIsMemberRequestWrapper(PFGroupsIsMemberRequestWrapper&& src) :
        PFGroupsIsMemberRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsIsMemberRequestWrapper& operator=(PFGroupsIsMemberRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsIsMemberRequestWrapper() = default;

    friend void swap(PFGroupsIsMemberRequestWrapper& lhs, PFGroupsIsMemberRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsIsMemberResponseWrapper : public ModelWrapper<PFGroupsIsMemberResponse, Alloc>
{
public:
    using ModelType = PFGroupsIsMemberResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFGroupsIsMemberResponse, Alloc>::ModelWrapper;

    void SetIsMember(bool value)
    {
        this->m_model.isMember = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupApplicationsRequestWrapper : public ModelWrapper<PFGroupsListGroupApplicationsRequest, Alloc>
{
public:
    using ModelType = PFGroupsListGroupApplicationsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupApplicationsRequestWrapper() = default;

    PFGroupsListGroupApplicationsRequestWrapper(const PFGroupsListGroupApplicationsRequest& model) :
        ModelWrapper<PFGroupsListGroupApplicationsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsListGroupApplicationsRequestWrapper(const PFGroupsListGroupApplicationsRequestWrapper& src) :
        PFGroupsListGroupApplicationsRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupApplicationsRequestWrapper(PFGroupsListGroupApplicationsRequestWrapper&& src) :
        PFGroupsListGroupApplicationsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupApplicationsRequestWrapper& operator=(PFGroupsListGroupApplicationsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupApplicationsRequestWrapper() = default;

    friend void swap(PFGroupsListGroupApplicationsRequestWrapper& lhs, PFGroupsListGroupApplicationsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGroupApplicationWrapper : public ModelWrapper<PFGroupsGroupApplication, Alloc>
{
public:
    using ModelType = PFGroupsGroupApplication;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGroupApplicationWrapper() = default;

    PFGroupsGroupApplicationWrapper(const PFGroupsGroupApplication& model) :
        ModelWrapper<PFGroupsGroupApplication, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsGroupApplicationWrapper(const PFGroupsGroupApplicationWrapper& src) :
        PFGroupsGroupApplicationWrapper{ src.Model() }
    {
    }

    PFGroupsGroupApplicationWrapper(PFGroupsGroupApplicationWrapper&& src) :
        PFGroupsGroupApplicationWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGroupApplicationWrapper& operator=(PFGroupsGroupApplicationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGroupApplicationWrapper() = default;

    friend void swap(PFGroupsGroupApplicationWrapper& lhs, PFGroupsGroupApplicationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetExpires(time_t value)
    {
        this->m_model.expires = value;
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
    }

    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_entity;
    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupApplicationsResponseWrapper : public ModelWrapper<PFGroupsListGroupApplicationsResponse, Alloc>
{
public:
    using ModelType = PFGroupsListGroupApplicationsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupApplicationsResponseWrapper() = default;

    PFGroupsListGroupApplicationsResponseWrapper(const PFGroupsListGroupApplicationsResponse& model) :
        ModelWrapper<PFGroupsListGroupApplicationsResponse, Alloc>{ model },
        m_applications{ model.applications, model.applications + model.applicationsCount }
    {
        SetModelPointers();
    }

    PFGroupsListGroupApplicationsResponseWrapper(const PFGroupsListGroupApplicationsResponseWrapper& src) :
        PFGroupsListGroupApplicationsResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupApplicationsResponseWrapper(PFGroupsListGroupApplicationsResponseWrapper&& src) :
        PFGroupsListGroupApplicationsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupApplicationsResponseWrapper& operator=(PFGroupsListGroupApplicationsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupApplicationsResponseWrapper() = default;

    friend void swap(PFGroupsListGroupApplicationsResponseWrapper& lhs, PFGroupsListGroupApplicationsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_applications, rhs.m_applications);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetApplications(ModelVector<PFGroupsGroupApplicationWrapper<Alloc>, Alloc> value)
    {
        m_applications = std::move(value);
        this->m_model.applications =  m_applications.empty() ? nullptr : m_applications.data();
        this->m_model.applicationsCount =  static_cast<uint32_t>(m_applications.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.applications = m_applications.empty() ? nullptr : m_applications.data();
    }

    ModelVector<PFGroupsGroupApplicationWrapper<Alloc>, Alloc> m_applications;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupBlocksRequestWrapper : public ModelWrapper<PFGroupsListGroupBlocksRequest, Alloc>
{
public:
    using ModelType = PFGroupsListGroupBlocksRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupBlocksRequestWrapper() = default;

    PFGroupsListGroupBlocksRequestWrapper(const PFGroupsListGroupBlocksRequest& model) :
        ModelWrapper<PFGroupsListGroupBlocksRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsListGroupBlocksRequestWrapper(const PFGroupsListGroupBlocksRequestWrapper& src) :
        PFGroupsListGroupBlocksRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupBlocksRequestWrapper(PFGroupsListGroupBlocksRequestWrapper&& src) :
        PFGroupsListGroupBlocksRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupBlocksRequestWrapper& operator=(PFGroupsListGroupBlocksRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupBlocksRequestWrapper() = default;

    friend void swap(PFGroupsListGroupBlocksRequestWrapper& lhs, PFGroupsListGroupBlocksRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGroupBlockWrapper : public ModelWrapper<PFGroupsGroupBlock, Alloc>
{
public:
    using ModelType = PFGroupsGroupBlock;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGroupBlockWrapper() = default;

    PFGroupsGroupBlockWrapper(const PFGroupsGroupBlock& model) :
        ModelWrapper<PFGroupsGroupBlock, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsGroupBlockWrapper(const PFGroupsGroupBlockWrapper& src) :
        PFGroupsGroupBlockWrapper{ src.Model() }
    {
    }

    PFGroupsGroupBlockWrapper(PFGroupsGroupBlockWrapper&& src) :
        PFGroupsGroupBlockWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGroupBlockWrapper& operator=(PFGroupsGroupBlockWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGroupBlockWrapper() = default;

    friend void swap(PFGroupsGroupBlockWrapper& lhs, PFGroupsGroupBlockWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.group = &m_group.Model();
    }

    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupBlocksResponseWrapper : public ModelWrapper<PFGroupsListGroupBlocksResponse, Alloc>
{
public:
    using ModelType = PFGroupsListGroupBlocksResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupBlocksResponseWrapper() = default;

    PFGroupsListGroupBlocksResponseWrapper(const PFGroupsListGroupBlocksResponse& model) :
        ModelWrapper<PFGroupsListGroupBlocksResponse, Alloc>{ model },
        m_blockedEntities{ model.blockedEntities, model.blockedEntities + model.blockedEntitiesCount }
    {
        SetModelPointers();
    }

    PFGroupsListGroupBlocksResponseWrapper(const PFGroupsListGroupBlocksResponseWrapper& src) :
        PFGroupsListGroupBlocksResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupBlocksResponseWrapper(PFGroupsListGroupBlocksResponseWrapper&& src) :
        PFGroupsListGroupBlocksResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupBlocksResponseWrapper& operator=(PFGroupsListGroupBlocksResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupBlocksResponseWrapper() = default;

    friend void swap(PFGroupsListGroupBlocksResponseWrapper& lhs, PFGroupsListGroupBlocksResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_blockedEntities, rhs.m_blockedEntities);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBlockedEntities(ModelVector<PFGroupsGroupBlockWrapper<Alloc>, Alloc> value)
    {
        m_blockedEntities = std::move(value);
        this->m_model.blockedEntities =  m_blockedEntities.empty() ? nullptr : m_blockedEntities.data();
        this->m_model.blockedEntitiesCount =  static_cast<uint32_t>(m_blockedEntities.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.blockedEntities = m_blockedEntities.empty() ? nullptr : m_blockedEntities.data();
    }

    ModelVector<PFGroupsGroupBlockWrapper<Alloc>, Alloc> m_blockedEntities;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupInvitationsRequestWrapper : public ModelWrapper<PFGroupsListGroupInvitationsRequest, Alloc>
{
public:
    using ModelType = PFGroupsListGroupInvitationsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupInvitationsRequestWrapper() = default;

    PFGroupsListGroupInvitationsRequestWrapper(const PFGroupsListGroupInvitationsRequest& model) :
        ModelWrapper<PFGroupsListGroupInvitationsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsListGroupInvitationsRequestWrapper(const PFGroupsListGroupInvitationsRequestWrapper& src) :
        PFGroupsListGroupInvitationsRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupInvitationsRequestWrapper(PFGroupsListGroupInvitationsRequestWrapper&& src) :
        PFGroupsListGroupInvitationsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupInvitationsRequestWrapper& operator=(PFGroupsListGroupInvitationsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupInvitationsRequestWrapper() = default;

    friend void swap(PFGroupsListGroupInvitationsRequestWrapper& lhs, PFGroupsListGroupInvitationsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGroupInvitationWrapper : public ModelWrapper<PFGroupsGroupInvitation, Alloc>
{
public:
    using ModelType = PFGroupsGroupInvitation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGroupInvitationWrapper() = default;

    PFGroupsGroupInvitationWrapper(const PFGroupsGroupInvitation& model) :
        ModelWrapper<PFGroupsGroupInvitation, Alloc>{ model },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt },
        m_invitedByEntity{ model.invitedByEntity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.invitedByEntity } : std::nullopt },
        m_invitedEntity{ model.invitedEntity ? std::optional<PFGroupsEntityWithLineageWrapper<Alloc>>{ *model.invitedEntity } : std::nullopt },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsGroupInvitationWrapper(const PFGroupsGroupInvitationWrapper& src) :
        PFGroupsGroupInvitationWrapper{ src.Model() }
    {
    }

    PFGroupsGroupInvitationWrapper(PFGroupsGroupInvitationWrapper&& src) :
        PFGroupsGroupInvitationWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGroupInvitationWrapper& operator=(PFGroupsGroupInvitationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGroupInvitationWrapper() = default;

    friend void swap(PFGroupsGroupInvitationWrapper& lhs, PFGroupsGroupInvitationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_invitedByEntity, rhs.m_invitedByEntity);
        swap(lhs.m_invitedEntity, rhs.m_invitedEntity);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetExpires(time_t value)
    {
        this->m_model.expires = value;
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

    void SetInvitedByEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_invitedByEntity = std::move(value);
        this->m_model.invitedByEntity = m_invitedByEntity ? &m_invitedByEntity->Model() : nullptr;
    }

    void SetInvitedEntity(std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> value)
    {
        m_invitedEntity = std::move(value);
        this->m_model.invitedEntity = m_invitedEntity ? &m_invitedEntity->Model() : nullptr;
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
        this->m_model.invitedByEntity = m_invitedByEntity ?  &m_invitedByEntity->Model() : nullptr;
        this->m_model.invitedEntity = m_invitedEntity ?  &m_invitedEntity->Model() : nullptr;
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_invitedByEntity;
    std::optional<PFGroupsEntityWithLineageWrapper<Alloc>> m_invitedEntity;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupInvitationsResponseWrapper : public ModelWrapper<PFGroupsListGroupInvitationsResponse, Alloc>
{
public:
    using ModelType = PFGroupsListGroupInvitationsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupInvitationsResponseWrapper() = default;

    PFGroupsListGroupInvitationsResponseWrapper(const PFGroupsListGroupInvitationsResponse& model) :
        ModelWrapper<PFGroupsListGroupInvitationsResponse, Alloc>{ model },
        m_invitations{ model.invitations, model.invitations + model.invitationsCount }
    {
        SetModelPointers();
    }

    PFGroupsListGroupInvitationsResponseWrapper(const PFGroupsListGroupInvitationsResponseWrapper& src) :
        PFGroupsListGroupInvitationsResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupInvitationsResponseWrapper(PFGroupsListGroupInvitationsResponseWrapper&& src) :
        PFGroupsListGroupInvitationsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupInvitationsResponseWrapper& operator=(PFGroupsListGroupInvitationsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupInvitationsResponseWrapper() = default;

    friend void swap(PFGroupsListGroupInvitationsResponseWrapper& lhs, PFGroupsListGroupInvitationsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_invitations, rhs.m_invitations);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetInvitations(ModelVector<PFGroupsGroupInvitationWrapper<Alloc>, Alloc> value)
    {
        m_invitations = std::move(value);
        this->m_model.invitations =  m_invitations.empty() ? nullptr : m_invitations.data();
        this->m_model.invitationsCount =  static_cast<uint32_t>(m_invitations.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.invitations = m_invitations.empty() ? nullptr : m_invitations.data();
    }

    ModelVector<PFGroupsGroupInvitationWrapper<Alloc>, Alloc> m_invitations;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupMembersRequestWrapper : public ModelWrapper<PFGroupsListGroupMembersRequest, Alloc>
{
public:
    using ModelType = PFGroupsListGroupMembersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupMembersRequestWrapper() = default;

    PFGroupsListGroupMembersRequestWrapper(const PFGroupsListGroupMembersRequest& model) :
        ModelWrapper<PFGroupsListGroupMembersRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsListGroupMembersRequestWrapper(const PFGroupsListGroupMembersRequestWrapper& src) :
        PFGroupsListGroupMembersRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupMembersRequestWrapper(PFGroupsListGroupMembersRequestWrapper&& src) :
        PFGroupsListGroupMembersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupMembersRequestWrapper& operator=(PFGroupsListGroupMembersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupMembersRequestWrapper() = default;

    friend void swap(PFGroupsListGroupMembersRequestWrapper& lhs, PFGroupsListGroupMembersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsEntityMemberRoleWrapper : public ModelWrapper<PFGroupsEntityMemberRole, Alloc>
{
public:
    using ModelType = PFGroupsEntityMemberRole;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsEntityMemberRoleWrapper() = default;

    PFGroupsEntityMemberRoleWrapper(const PFGroupsEntityMemberRole& model) :
        ModelWrapper<PFGroupsEntityMemberRole, Alloc>{ model },
        m_members{ model.members, model.members + model.membersCount },
        m_roleId{ SafeString(model.roleId) },
        m_roleName{ SafeString(model.roleName) }
    {
        SetModelPointers();
    }

    PFGroupsEntityMemberRoleWrapper(const PFGroupsEntityMemberRoleWrapper& src) :
        PFGroupsEntityMemberRoleWrapper{ src.Model() }
    {
    }

    PFGroupsEntityMemberRoleWrapper(PFGroupsEntityMemberRoleWrapper&& src) :
        PFGroupsEntityMemberRoleWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsEntityMemberRoleWrapper& operator=(PFGroupsEntityMemberRoleWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsEntityMemberRoleWrapper() = default;

    friend void swap(PFGroupsEntityMemberRoleWrapper& lhs, PFGroupsEntityMemberRoleWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_members, rhs.m_members);
        swap(lhs.m_roleId, rhs.m_roleId);
        swap(lhs.m_roleName, rhs.m_roleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetMembers(ModelVector<PFGroupsEntityWithLineageWrapper<Alloc>, Alloc> value)
    {
        m_members = std::move(value);
        this->m_model.members =  m_members.empty() ? nullptr : m_members.data();
        this->m_model.membersCount =  static_cast<uint32_t>(m_members.size());
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

    void SetRoleName(String value)
    {
        m_roleName = std::move(value);
        this->m_model.roleName =  m_roleName.empty() ? nullptr : m_roleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.members = m_members.empty() ? nullptr : m_members.data();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
        this->m_model.roleName = m_roleName.empty() ? nullptr : m_roleName.data();
    }

    ModelVector<PFGroupsEntityWithLineageWrapper<Alloc>, Alloc> m_members;
    String m_roleId;
    String m_roleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListGroupMembersResponseWrapper : public ModelWrapper<PFGroupsListGroupMembersResponse, Alloc>
{
public:
    using ModelType = PFGroupsListGroupMembersResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListGroupMembersResponseWrapper() = default;

    PFGroupsListGroupMembersResponseWrapper(const PFGroupsListGroupMembersResponse& model) :
        ModelWrapper<PFGroupsListGroupMembersResponse, Alloc>{ model },
        m_members{ model.members, model.members + model.membersCount }
    {
        SetModelPointers();
    }

    PFGroupsListGroupMembersResponseWrapper(const PFGroupsListGroupMembersResponseWrapper& src) :
        PFGroupsListGroupMembersResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListGroupMembersResponseWrapper(PFGroupsListGroupMembersResponseWrapper&& src) :
        PFGroupsListGroupMembersResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListGroupMembersResponseWrapper& operator=(PFGroupsListGroupMembersResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListGroupMembersResponseWrapper() = default;

    friend void swap(PFGroupsListGroupMembersResponseWrapper& lhs, PFGroupsListGroupMembersResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_members, rhs.m_members);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetMembers(ModelVector<PFGroupsEntityMemberRoleWrapper<Alloc>, Alloc> value)
    {
        m_members = std::move(value);
        this->m_model.members =  m_members.empty() ? nullptr : m_members.data();
        this->m_model.membersCount =  static_cast<uint32_t>(m_members.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.members = m_members.empty() ? nullptr : m_members.data();
    }

    ModelVector<PFGroupsEntityMemberRoleWrapper<Alloc>, Alloc> m_members;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListMembershipRequestWrapper : public ModelWrapper<PFGroupsListMembershipRequest, Alloc>
{
public:
    using ModelType = PFGroupsListMembershipRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListMembershipRequestWrapper() = default;

    PFGroupsListMembershipRequestWrapper(const PFGroupsListMembershipRequest& model) :
        ModelWrapper<PFGroupsListMembershipRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsListMembershipRequestWrapper(const PFGroupsListMembershipRequestWrapper& src) :
        PFGroupsListMembershipRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListMembershipRequestWrapper(PFGroupsListMembershipRequestWrapper&& src) :
        PFGroupsListMembershipRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListMembershipRequestWrapper& operator=(PFGroupsListMembershipRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListMembershipRequestWrapper() = default;

    friend void swap(PFGroupsListMembershipRequestWrapper& lhs, PFGroupsListMembershipRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGroupRoleWrapper : public ModelWrapper<PFGroupsGroupRole, Alloc>
{
public:
    using ModelType = PFGroupsGroupRole;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGroupRoleWrapper() = default;

    PFGroupsGroupRoleWrapper(const PFGroupsGroupRole& model) :
        ModelWrapper<PFGroupsGroupRole, Alloc>{ model },
        m_roleId{ SafeString(model.roleId) },
        m_roleName{ SafeString(model.roleName) }
    {
        SetModelPointers();
    }

    PFGroupsGroupRoleWrapper(const PFGroupsGroupRoleWrapper& src) :
        PFGroupsGroupRoleWrapper{ src.Model() }
    {
    }

    PFGroupsGroupRoleWrapper(PFGroupsGroupRoleWrapper&& src) :
        PFGroupsGroupRoleWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGroupRoleWrapper& operator=(PFGroupsGroupRoleWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGroupRoleWrapper() = default;

    friend void swap(PFGroupsGroupRoleWrapper& lhs, PFGroupsGroupRoleWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_roleId, rhs.m_roleId);
        swap(lhs.m_roleName, rhs.m_roleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

    void SetRoleName(String value)
    {
        m_roleName = std::move(value);
        this->m_model.roleName =  m_roleName.empty() ? nullptr : m_roleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
        this->m_model.roleName = m_roleName.empty() ? nullptr : m_roleName.data();
    }

    String m_roleId;
    String m_roleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsGroupWithRolesWrapper : public ModelWrapper<PFGroupsGroupWithRoles, Alloc>
{
public:
    using ModelType = PFGroupsGroupWithRoles;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsGroupWithRolesWrapper() = default;

    PFGroupsGroupWithRolesWrapper(const PFGroupsGroupWithRoles& model) :
        ModelWrapper<PFGroupsGroupWithRoles, Alloc>{ model },
        m_group{ model.group ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.group } : std::nullopt },
        m_groupName{ SafeString(model.groupName) },
        m_roles{ model.roles, model.roles + model.rolesCount }
    {
        SetModelPointers();
    }

    PFGroupsGroupWithRolesWrapper(const PFGroupsGroupWithRolesWrapper& src) :
        PFGroupsGroupWithRolesWrapper{ src.Model() }
    {
    }

    PFGroupsGroupWithRolesWrapper(PFGroupsGroupWithRolesWrapper&& src) :
        PFGroupsGroupWithRolesWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsGroupWithRolesWrapper& operator=(PFGroupsGroupWithRolesWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsGroupWithRolesWrapper() = default;

    friend void swap(PFGroupsGroupWithRolesWrapper& lhs, PFGroupsGroupWithRolesWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_groupName, rhs.m_groupName);
        swap(lhs.m_roles, rhs.m_roles);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGroup(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_group = std::move(value);
        this->m_model.group = m_group ? &m_group->Model() : nullptr;
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetRoles(ModelVector<PFGroupsGroupRoleWrapper<Alloc>, Alloc> value)
    {
        m_roles = std::move(value);
        this->m_model.roles =  m_roles.empty() ? nullptr : m_roles.data();
        this->m_model.rolesCount =  static_cast<uint32_t>(m_roles.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.group = m_group ?  &m_group->Model() : nullptr;
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
        this->m_model.roles = m_roles.empty() ? nullptr : m_roles.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_group;
    String m_groupName;
    ModelVector<PFGroupsGroupRoleWrapper<Alloc>, Alloc> m_roles;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListMembershipResponseWrapper : public ModelWrapper<PFGroupsListMembershipResponse, Alloc>
{
public:
    using ModelType = PFGroupsListMembershipResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListMembershipResponseWrapper() = default;

    PFGroupsListMembershipResponseWrapper(const PFGroupsListMembershipResponse& model) :
        ModelWrapper<PFGroupsListMembershipResponse, Alloc>{ model },
        m_groups{ model.groups, model.groups + model.groupsCount }
    {
        SetModelPointers();
    }

    PFGroupsListMembershipResponseWrapper(const PFGroupsListMembershipResponseWrapper& src) :
        PFGroupsListMembershipResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListMembershipResponseWrapper(PFGroupsListMembershipResponseWrapper&& src) :
        PFGroupsListMembershipResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListMembershipResponseWrapper& operator=(PFGroupsListMembershipResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListMembershipResponseWrapper() = default;

    friend void swap(PFGroupsListMembershipResponseWrapper& lhs, PFGroupsListMembershipResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_groups, rhs.m_groups);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGroups(ModelVector<PFGroupsGroupWithRolesWrapper<Alloc>, Alloc> value)
    {
        m_groups = std::move(value);
        this->m_model.groups =  m_groups.empty() ? nullptr : m_groups.data();
        this->m_model.groupsCount =  static_cast<uint32_t>(m_groups.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.groups = m_groups.empty() ? nullptr : m_groups.data();
    }

    ModelVector<PFGroupsGroupWithRolesWrapper<Alloc>, Alloc> m_groups;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListMembershipOpportunitiesRequestWrapper : public ModelWrapper<PFGroupsListMembershipOpportunitiesRequest, Alloc>
{
public:
    using ModelType = PFGroupsListMembershipOpportunitiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListMembershipOpportunitiesRequestWrapper() = default;

    PFGroupsListMembershipOpportunitiesRequestWrapper(const PFGroupsListMembershipOpportunitiesRequest& model) :
        ModelWrapper<PFGroupsListMembershipOpportunitiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsListMembershipOpportunitiesRequestWrapper(const PFGroupsListMembershipOpportunitiesRequestWrapper& src) :
        PFGroupsListMembershipOpportunitiesRequestWrapper{ src.Model() }
    {
    }

    PFGroupsListMembershipOpportunitiesRequestWrapper(PFGroupsListMembershipOpportunitiesRequestWrapper&& src) :
        PFGroupsListMembershipOpportunitiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListMembershipOpportunitiesRequestWrapper& operator=(PFGroupsListMembershipOpportunitiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListMembershipOpportunitiesRequestWrapper() = default;

    friend void swap(PFGroupsListMembershipOpportunitiesRequestWrapper& lhs, PFGroupsListMembershipOpportunitiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsListMembershipOpportunitiesResponseWrapper : public ModelWrapper<PFGroupsListMembershipOpportunitiesResponse, Alloc>
{
public:
    using ModelType = PFGroupsListMembershipOpportunitiesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsListMembershipOpportunitiesResponseWrapper() = default;

    PFGroupsListMembershipOpportunitiesResponseWrapper(const PFGroupsListMembershipOpportunitiesResponse& model) :
        ModelWrapper<PFGroupsListMembershipOpportunitiesResponse, Alloc>{ model },
        m_applications{ model.applications, model.applications + model.applicationsCount },
        m_invitations{ model.invitations, model.invitations + model.invitationsCount }
    {
        SetModelPointers();
    }

    PFGroupsListMembershipOpportunitiesResponseWrapper(const PFGroupsListMembershipOpportunitiesResponseWrapper& src) :
        PFGroupsListMembershipOpportunitiesResponseWrapper{ src.Model() }
    {
    }

    PFGroupsListMembershipOpportunitiesResponseWrapper(PFGroupsListMembershipOpportunitiesResponseWrapper&& src) :
        PFGroupsListMembershipOpportunitiesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsListMembershipOpportunitiesResponseWrapper& operator=(PFGroupsListMembershipOpportunitiesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsListMembershipOpportunitiesResponseWrapper() = default;

    friend void swap(PFGroupsListMembershipOpportunitiesResponseWrapper& lhs, PFGroupsListMembershipOpportunitiesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_applications, rhs.m_applications);
        swap(lhs.m_invitations, rhs.m_invitations);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetApplications(ModelVector<PFGroupsGroupApplicationWrapper<Alloc>, Alloc> value)
    {
        m_applications = std::move(value);
        this->m_model.applications =  m_applications.empty() ? nullptr : m_applications.data();
        this->m_model.applicationsCount =  static_cast<uint32_t>(m_applications.size());
    }

    void SetInvitations(ModelVector<PFGroupsGroupInvitationWrapper<Alloc>, Alloc> value)
    {
        m_invitations = std::move(value);
        this->m_model.invitations =  m_invitations.empty() ? nullptr : m_invitations.data();
        this->m_model.invitationsCount =  static_cast<uint32_t>(m_invitations.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.applications = m_applications.empty() ? nullptr : m_applications.data();
        this->m_model.invitations = m_invitations.empty() ? nullptr : m_invitations.data();
    }

    ModelVector<PFGroupsGroupApplicationWrapper<Alloc>, Alloc> m_applications;
    ModelVector<PFGroupsGroupInvitationWrapper<Alloc>, Alloc> m_invitations;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsRemoveGroupApplicationRequestWrapper : public ModelWrapper<PFGroupsRemoveGroupApplicationRequest, Alloc>
{
public:
    using ModelType = PFGroupsRemoveGroupApplicationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsRemoveGroupApplicationRequestWrapper() = default;

    PFGroupsRemoveGroupApplicationRequestWrapper(const PFGroupsRemoveGroupApplicationRequest& model) :
        ModelWrapper<PFGroupsRemoveGroupApplicationRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsRemoveGroupApplicationRequestWrapper(const PFGroupsRemoveGroupApplicationRequestWrapper& src) :
        PFGroupsRemoveGroupApplicationRequestWrapper{ src.Model() }
    {
    }

    PFGroupsRemoveGroupApplicationRequestWrapper(PFGroupsRemoveGroupApplicationRequestWrapper&& src) :
        PFGroupsRemoveGroupApplicationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsRemoveGroupApplicationRequestWrapper& operator=(PFGroupsRemoveGroupApplicationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsRemoveGroupApplicationRequestWrapper() = default;

    friend void swap(PFGroupsRemoveGroupApplicationRequestWrapper& lhs, PFGroupsRemoveGroupApplicationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsRemoveGroupInvitationRequestWrapper : public ModelWrapper<PFGroupsRemoveGroupInvitationRequest, Alloc>
{
public:
    using ModelType = PFGroupsRemoveGroupInvitationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsRemoveGroupInvitationRequestWrapper() = default;

    PFGroupsRemoveGroupInvitationRequestWrapper(const PFGroupsRemoveGroupInvitationRequest& model) :
        ModelWrapper<PFGroupsRemoveGroupInvitationRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsRemoveGroupInvitationRequestWrapper(const PFGroupsRemoveGroupInvitationRequestWrapper& src) :
        PFGroupsRemoveGroupInvitationRequestWrapper{ src.Model() }
    {
    }

    PFGroupsRemoveGroupInvitationRequestWrapper(PFGroupsRemoveGroupInvitationRequestWrapper&& src) :
        PFGroupsRemoveGroupInvitationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsRemoveGroupInvitationRequestWrapper& operator=(PFGroupsRemoveGroupInvitationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsRemoveGroupInvitationRequestWrapper() = default;

    friend void swap(PFGroupsRemoveGroupInvitationRequestWrapper& lhs, PFGroupsRemoveGroupInvitationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsRemoveMembersRequestWrapper : public ModelWrapper<PFGroupsRemoveMembersRequest, Alloc>
{
public:
    using ModelType = PFGroupsRemoveMembersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsRemoveMembersRequestWrapper() = default;

    PFGroupsRemoveMembersRequestWrapper(const PFGroupsRemoveMembersRequest& model) :
        ModelWrapper<PFGroupsRemoveMembersRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_members{ model.members, model.members + model.membersCount },
        m_roleId{ SafeString(model.roleId) }
    {
        SetModelPointers();
    }

    PFGroupsRemoveMembersRequestWrapper(const PFGroupsRemoveMembersRequestWrapper& src) :
        PFGroupsRemoveMembersRequestWrapper{ src.Model() }
    {
    }

    PFGroupsRemoveMembersRequestWrapper(PFGroupsRemoveMembersRequestWrapper&& src) :
        PFGroupsRemoveMembersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsRemoveMembersRequestWrapper& operator=(PFGroupsRemoveMembersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsRemoveMembersRequestWrapper() = default;

    friend void swap(PFGroupsRemoveMembersRequestWrapper& lhs, PFGroupsRemoveMembersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_members, rhs.m_members);
        swap(lhs.m_roleId, rhs.m_roleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetMembers(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_members = std::move(value);
        this->m_model.members =  m_members.empty() ? nullptr : m_members.data();
        this->m_model.membersCount =  static_cast<uint32_t>(m_members.size());
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.group = &m_group.Model();
        this->m_model.members = m_members.empty() ? nullptr : m_members.data();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_group;
    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_members;
    String m_roleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsUnblockEntityRequestWrapper : public ModelWrapper<PFGroupsUnblockEntityRequest, Alloc>
{
public:
    using ModelType = PFGroupsUnblockEntityRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsUnblockEntityRequestWrapper() = default;

    PFGroupsUnblockEntityRequestWrapper(const PFGroupsUnblockEntityRequest& model) :
        ModelWrapper<PFGroupsUnblockEntityRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} },
        m_group{ model.group ? *model.group : decltype(*model.group){} }
    {
        SetModelPointers();
    }

    PFGroupsUnblockEntityRequestWrapper(const PFGroupsUnblockEntityRequestWrapper& src) :
        PFGroupsUnblockEntityRequestWrapper{ src.Model() }
    {
    }

    PFGroupsUnblockEntityRequestWrapper(PFGroupsUnblockEntityRequestWrapper&& src) :
        PFGroupsUnblockEntityRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsUnblockEntityRequestWrapper& operator=(PFGroupsUnblockEntityRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsUnblockEntityRequestWrapper() = default;

    friend void swap(PFGroupsUnblockEntityRequestWrapper& lhs, PFGroupsUnblockEntityRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_group, rhs.m_group);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
        this->m_model.group = &m_group.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
    PFEntityKeyWrapper<Alloc> m_group;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsUpdateGroupRequestWrapper : public ModelWrapper<PFGroupsUpdateGroupRequest, Alloc>
{
public:
    using ModelType = PFGroupsUpdateGroupRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsUpdateGroupRequestWrapper() = default;

    PFGroupsUpdateGroupRequestWrapper(const PFGroupsUpdateGroupRequest& model) :
        ModelWrapper<PFGroupsUpdateGroupRequest, Alloc>{ model },
        m_adminRoleId{ SafeString(model.adminRoleId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedProfileVersion{ model.expectedProfileVersion ? std::optional<int32_t>{ *model.expectedProfileVersion } : std::nullopt },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_groupName{ SafeString(model.groupName) },
        m_memberRoleId{ SafeString(model.memberRoleId) }
    {
        SetModelPointers();
    }

    PFGroupsUpdateGroupRequestWrapper(const PFGroupsUpdateGroupRequestWrapper& src) :
        PFGroupsUpdateGroupRequestWrapper{ src.Model() }
    {
    }

    PFGroupsUpdateGroupRequestWrapper(PFGroupsUpdateGroupRequestWrapper&& src) :
        PFGroupsUpdateGroupRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsUpdateGroupRequestWrapper& operator=(PFGroupsUpdateGroupRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsUpdateGroupRequestWrapper() = default;

    friend void swap(PFGroupsUpdateGroupRequestWrapper& lhs, PFGroupsUpdateGroupRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adminRoleId, rhs.m_adminRoleId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedProfileVersion, rhs.m_expectedProfileVersion);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_groupName, rhs.m_groupName);
        swap(lhs.m_memberRoleId, rhs.m_memberRoleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdminRoleId(String value)
    {
        m_adminRoleId = std::move(value);
        this->m_model.adminRoleId =  m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetExpectedProfileVersion(std::optional<int32_t> value)
    {
        m_expectedProfileVersion = std::move(value);
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetGroupName(String value)
    {
        m_groupName = std::move(value);
        this->m_model.groupName =  m_groupName.empty() ? nullptr : m_groupName.data();
    }

    void SetMemberRoleId(String value)
    {
        m_memberRoleId = std::move(value);
        this->m_model.memberRoleId =  m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.adminRoleId = m_adminRoleId.empty() ? nullptr : m_adminRoleId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
        this->m_model.group = &m_group.Model();
        this->m_model.groupName = m_groupName.empty() ? nullptr : m_groupName.data();
        this->m_model.memberRoleId = m_memberRoleId.empty() ? nullptr : m_memberRoleId.data();
    }

    String m_adminRoleId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedProfileVersion;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_groupName;
    String m_memberRoleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsUpdateGroupResponseWrapper : public ModelWrapper<PFGroupsUpdateGroupResponse, Alloc>
{
public:
    using ModelType = PFGroupsUpdateGroupResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsUpdateGroupResponseWrapper() = default;

    PFGroupsUpdateGroupResponseWrapper(const PFGroupsUpdateGroupResponse& model) :
        ModelWrapper<PFGroupsUpdateGroupResponse, Alloc>{ model },
        m_operationReason{ SafeString(model.operationReason) },
        m_setResult{ model.setResult ? std::optional<PFOperationTypes>{ *model.setResult } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsUpdateGroupResponseWrapper(const PFGroupsUpdateGroupResponseWrapper& src) :
        PFGroupsUpdateGroupResponseWrapper{ src.Model() }
    {
    }

    PFGroupsUpdateGroupResponseWrapper(PFGroupsUpdateGroupResponseWrapper&& src) :
        PFGroupsUpdateGroupResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsUpdateGroupResponseWrapper& operator=(PFGroupsUpdateGroupResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsUpdateGroupResponseWrapper() = default;

    friend void swap(PFGroupsUpdateGroupResponseWrapper& lhs, PFGroupsUpdateGroupResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_operationReason, rhs.m_operationReason);
        swap(lhs.m_setResult, rhs.m_setResult);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetOperationReason(String value)
    {
        m_operationReason = std::move(value);
        this->m_model.operationReason =  m_operationReason.empty() ? nullptr : m_operationReason.data();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetSetResult(std::optional<PFOperationTypes> value)
    {
        m_setResult = std::move(value);
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.operationReason = m_operationReason.empty() ? nullptr : m_operationReason.data();
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

    String m_operationReason;
    std::optional<PFOperationTypes> m_setResult;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsUpdateGroupRoleRequestWrapper : public ModelWrapper<PFGroupsUpdateGroupRoleRequest, Alloc>
{
public:
    using ModelType = PFGroupsUpdateGroupRoleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsUpdateGroupRoleRequestWrapper() = default;

    PFGroupsUpdateGroupRoleRequestWrapper(const PFGroupsUpdateGroupRoleRequest& model) :
        ModelWrapper<PFGroupsUpdateGroupRoleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedProfileVersion{ model.expectedProfileVersion ? std::optional<int32_t>{ *model.expectedProfileVersion } : std::nullopt },
        m_group{ model.group ? *model.group : decltype(*model.group){} },
        m_roleId{ SafeString(model.roleId) },
        m_roleName{ SafeString(model.roleName) }
    {
        SetModelPointers();
    }

    PFGroupsUpdateGroupRoleRequestWrapper(const PFGroupsUpdateGroupRoleRequestWrapper& src) :
        PFGroupsUpdateGroupRoleRequestWrapper{ src.Model() }
    {
    }

    PFGroupsUpdateGroupRoleRequestWrapper(PFGroupsUpdateGroupRoleRequestWrapper&& src) :
        PFGroupsUpdateGroupRoleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsUpdateGroupRoleRequestWrapper& operator=(PFGroupsUpdateGroupRoleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsUpdateGroupRoleRequestWrapper() = default;

    friend void swap(PFGroupsUpdateGroupRoleRequestWrapper& lhs, PFGroupsUpdateGroupRoleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedProfileVersion, rhs.m_expectedProfileVersion);
        swap(lhs.m_group, rhs.m_group);
        swap(lhs.m_roleId, rhs.m_roleId);
        swap(lhs.m_roleName, rhs.m_roleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetExpectedProfileVersion(std::optional<int32_t> value)
    {
        m_expectedProfileVersion = std::move(value);
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
    }

    void SetGroup(PFEntityKeyWrapper<Alloc> value)
    {
        m_group = std::move(value);
        this->m_model.group = &m_group.Model();
    }

    void SetRoleId(String value)
    {
        m_roleId = std::move(value);
        this->m_model.roleId =  m_roleId.empty() ? nullptr : m_roleId.data();
    }

    void SetRoleName(String value)
    {
        m_roleName = std::move(value);
        this->m_model.roleName =  m_roleName.empty() ? nullptr : m_roleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedProfileVersion = m_expectedProfileVersion ? m_expectedProfileVersion.operator->() : nullptr;
        this->m_model.group = &m_group.Model();
        this->m_model.roleId = m_roleId.empty() ? nullptr : m_roleId.data();
        this->m_model.roleName = m_roleName.empty() ? nullptr : m_roleName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedProfileVersion;
    PFEntityKeyWrapper<Alloc> m_group;
    String m_roleId;
    String m_roleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGroupsUpdateGroupRoleResponseWrapper : public ModelWrapper<PFGroupsUpdateGroupRoleResponse, Alloc>
{
public:
    using ModelType = PFGroupsUpdateGroupRoleResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGroupsUpdateGroupRoleResponseWrapper() = default;

    PFGroupsUpdateGroupRoleResponseWrapper(const PFGroupsUpdateGroupRoleResponse& model) :
        ModelWrapper<PFGroupsUpdateGroupRoleResponse, Alloc>{ model },
        m_operationReason{ SafeString(model.operationReason) },
        m_setResult{ model.setResult ? std::optional<PFOperationTypes>{ *model.setResult } : std::nullopt }
    {
        SetModelPointers();
    }

    PFGroupsUpdateGroupRoleResponseWrapper(const PFGroupsUpdateGroupRoleResponseWrapper& src) :
        PFGroupsUpdateGroupRoleResponseWrapper{ src.Model() }
    {
    }

    PFGroupsUpdateGroupRoleResponseWrapper(PFGroupsUpdateGroupRoleResponseWrapper&& src) :
        PFGroupsUpdateGroupRoleResponseWrapper{}
    {
        swap(*this, src);
    }

    PFGroupsUpdateGroupRoleResponseWrapper& operator=(PFGroupsUpdateGroupRoleResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGroupsUpdateGroupRoleResponseWrapper() = default;

    friend void swap(PFGroupsUpdateGroupRoleResponseWrapper& lhs, PFGroupsUpdateGroupRoleResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_operationReason, rhs.m_operationReason);
        swap(lhs.m_setResult, rhs.m_setResult);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetOperationReason(String value)
    {
        m_operationReason = std::move(value);
        this->m_model.operationReason =  m_operationReason.empty() ? nullptr : m_operationReason.data();
    }

    void SetProfileVersion(int32_t value)
    {
        this->m_model.profileVersion = value;
    }

    void SetSetResult(std::optional<PFOperationTypes> value)
    {
        m_setResult = std::move(value);
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.operationReason = m_operationReason.empty() ? nullptr : m_operationReason.data();
        this->m_model.setResult = m_setResult ? m_setResult.operator->() : nullptr;
    }

    String m_operationReason;
    std::optional<PFOperationTypes> m_setResult;
};

} // namespace Wrappers
} // namespace PlayFab

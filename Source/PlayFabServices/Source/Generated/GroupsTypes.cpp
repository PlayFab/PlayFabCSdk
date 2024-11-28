#include "stdafx.h"
#include "GroupsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Groups
{

JsonValue AcceptGroupApplicationRequest::ToJson() const
{
    return AcceptGroupApplicationRequest::ToJson(this->Model());
}

JsonValue AcceptGroupApplicationRequest::ToJson(const PFGroupsAcceptGroupApplicationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue AcceptGroupInvitationRequest::ToJson() const
{
    return AcceptGroupInvitationRequest::ToJson(this->Model());
}

JsonValue AcceptGroupInvitationRequest::ToJson(const PFGroupsAcceptGroupInvitationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue AddMembersRequest::ToJson() const
{
    return AddMembersRequest::ToJson(this->Model());
}

JsonValue AddMembersRequest::ToJson(const PFGroupsAddMembersRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "Members", input.members, input.membersCount);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    return output;
}

JsonValue ApplyToGroupRequest::ToJson() const
{
    return ApplyToGroupRequest::ToJson(this->Model());
}

JsonValue ApplyToGroupRequest::ToJson(const PFGroupsApplyToGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AutoAcceptOutstandingInvite", input.autoAcceptOutstandingInvite);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

HRESULT EntityWithLineage::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> key{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Key", key));
    if (key)
    {
        this->SetKey(std::move(*key));
    }

    ModelDictionaryEntryVector<EntityKey> lineage{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityKey>(input, "Lineage", lineage));
    this->SetLineage(std::move(lineage));

    return S_OK;
}

size_t EntityWithLineage::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsEntityWithLineage const*> EntityWithLineage::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityWithLineage>(&this->Model());
}

size_t EntityWithLineage::RequiredBufferSize(const PFGroupsEntityWithLineage& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.key)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.key);
    }
    requiredSize += (alignof(PFEntityKeyDictionaryEntry) + sizeof(PFEntityKeyDictionaryEntry) * model.lineageCount);
    for (size_t i = 0; i < model.lineageCount; ++i)
    {
        requiredSize += (std::strlen(model.lineage[i].key) + 1);
        requiredSize += EntityKey::RequiredBufferSize(*model.lineage[i].value);
    }
    return requiredSize;
}

HRESULT EntityWithLineage::Copy(const PFGroupsEntityWithLineage& input, PFGroupsEntityWithLineage& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.key);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.key = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityKey>(input.lineage, input.lineageCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lineage = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ApplyToGroupResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityWithLineage> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expires", this->m_model.expires));

    std::optional<EntityKey> group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    if (group)
    {
        this->SetGroup(std::move(*group));
    }

    return S_OK;
}

size_t ApplyToGroupResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsApplyToGroupResponse const*> ApplyToGroupResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ApplyToGroupResponse>(&this->Model());
}

size_t ApplyToGroupResponse::RequiredBufferSize(const PFGroupsApplyToGroupResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.entity);
    }
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    return requiredSize;
}

HRESULT ApplyToGroupResponse::Copy(const PFGroupsApplyToGroupResponse& input, PFGroupsApplyToGroupResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue BlockEntityRequest::ToJson() const
{
    return BlockEntityRequest::ToJson(this->Model());
}

JsonValue BlockEntityRequest::ToJson(const PFGroupsBlockEntityRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue ChangeMemberRoleRequest::ToJson() const
{
    return ChangeMemberRoleRequest::ToJson(this->Model());
}

JsonValue ChangeMemberRoleRequest::ToJson(const PFGroupsChangeMemberRoleRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DestinationRoleId", input.destinationRoleId);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "Members", input.members, input.membersCount);
    JsonUtils::ObjectAddMember(output, "OriginRoleId", input.originRoleId);
    return output;
}

JsonValue CreateGroupRequest::ToJson() const
{
    return CreateGroupRequest::ToJson(this->Model());
}

JsonValue CreateGroupRequest::ToJson(const PFGroupsCreateGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "GroupName", input.groupName);
    return output;
}

HRESULT CreateGroupResponse::FromJson(const JsonValue& input)
{
    String adminRoleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AdminRoleId", adminRoleId));
    this->SetAdminRoleId(std::move(adminRoleId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    EntityKey group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    this->SetGroup(std::move(group));

    String groupName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GroupName", groupName));
    this->SetGroupName(std::move(groupName));

    String memberRoleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MemberRoleId", memberRoleId));
    this->SetMemberRoleId(std::move(memberRoleId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    StringDictionaryEntryVector roles{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Roles", roles));
    this->SetRoles(std::move(roles));

    return S_OK;
}

size_t CreateGroupResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsCreateGroupResponse const*> CreateGroupResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CreateGroupResponse>(&this->Model());
}

size_t CreateGroupResponse::RequiredBufferSize(const PFGroupsCreateGroupResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.adminRoleId)
    {
        requiredSize += (std::strlen(model.adminRoleId) + 1);
    }
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    if (model.groupName)
    {
        requiredSize += (std::strlen(model.groupName) + 1);
    }
    if (model.memberRoleId)
    {
        requiredSize += (std::strlen(model.memberRoleId) + 1);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.rolesCount);
    for (size_t i = 0; i < model.rolesCount; ++i)
    {
        requiredSize += (std::strlen(model.roles[i].key) + 1);
        requiredSize += (std::strlen(model.roles[i].value) + 1);
    }
    return requiredSize;
}

HRESULT CreateGroupResponse::Copy(const PFGroupsCreateGroupResponse& input, PFGroupsCreateGroupResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.adminRoleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.adminRoleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.groupName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.groupName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.memberRoleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.memberRoleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.roles, input.rolesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roles = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CreateGroupRoleRequest::ToJson() const
{
    return CreateGroupRoleRequest::ToJson(this->Model());
}

JsonValue CreateGroupRoleRequest::ToJson(const PFGroupsCreateGroupRoleRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    JsonUtils::ObjectAddMember(output, "RoleName", input.roleName);
    return output;
}

HRESULT CreateGroupRoleResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    String roleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleId", roleId));
    this->SetRoleId(std::move(roleId));

    String roleName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleName", roleName));
    this->SetRoleName(std::move(roleName));

    return S_OK;
}

size_t CreateGroupRoleResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsCreateGroupRoleResponse const*> CreateGroupRoleResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CreateGroupRoleResponse>(&this->Model());
}

size_t CreateGroupRoleResponse::RequiredBufferSize(const PFGroupsCreateGroupRoleResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.roleId)
    {
        requiredSize += (std::strlen(model.roleId) + 1);
    }
    if (model.roleName)
    {
        requiredSize += (std::strlen(model.roleName) + 1);
    }
    return requiredSize;
}

HRESULT CreateGroupRoleResponse::Copy(const PFGroupsCreateGroupRoleResponse& input, PFGroupsCreateGroupRoleResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.roleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeleteGroupRequest::ToJson() const
{
    return DeleteGroupRequest::ToJson(this->Model());
}

JsonValue DeleteGroupRequest::ToJson(const PFGroupsDeleteGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue DeleteRoleRequest::ToJson() const
{
    return DeleteRoleRequest::ToJson(this->Model());
}

JsonValue DeleteRoleRequest::ToJson(const PFGroupsDeleteRoleRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    return output;
}

JsonValue GetGroupRequest::ToJson() const
{
    return GetGroupRequest::ToJson(this->Model());
}

JsonValue GetGroupRequest::ToJson(const PFGroupsGetGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "GroupName", input.groupName);
    return output;
}

HRESULT GetGroupResponse::FromJson(const JsonValue& input)
{
    String adminRoleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AdminRoleId", adminRoleId));
    this->SetAdminRoleId(std::move(adminRoleId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    EntityKey group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    this->SetGroup(std::move(group));

    String groupName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GroupName", groupName));
    this->SetGroupName(std::move(groupName));

    String memberRoleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MemberRoleId", memberRoleId));
    this->SetMemberRoleId(std::move(memberRoleId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    StringDictionaryEntryVector roles{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Roles", roles));
    this->SetRoles(std::move(roles));

    return S_OK;
}

size_t GetGroupResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGetGroupResponse const*> GetGroupResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetGroupResponse>(&this->Model());
}

size_t GetGroupResponse::RequiredBufferSize(const PFGroupsGetGroupResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.adminRoleId)
    {
        requiredSize += (std::strlen(model.adminRoleId) + 1);
    }
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    if (model.groupName)
    {
        requiredSize += (std::strlen(model.groupName) + 1);
    }
    if (model.memberRoleId)
    {
        requiredSize += (std::strlen(model.memberRoleId) + 1);
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.rolesCount);
    for (size_t i = 0; i < model.rolesCount; ++i)
    {
        requiredSize += (std::strlen(model.roles[i].key) + 1);
        requiredSize += (std::strlen(model.roles[i].value) + 1);
    }
    return requiredSize;
}

HRESULT GetGroupResponse::Copy(const PFGroupsGetGroupResponse& input, PFGroupsGetGroupResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.adminRoleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.adminRoleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.groupName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.groupName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.memberRoleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.memberRoleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.roles, input.rolesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roles = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue InviteToGroupRequest::ToJson() const
{
    return InviteToGroupRequest::ToJson(this->Model());
}

JsonValue InviteToGroupRequest::ToJson(const PFGroupsInviteToGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AutoAcceptOutstandingApplication", input.autoAcceptOutstandingApplication);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    return output;
}

HRESULT InviteToGroupResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expires", this->m_model.expires));

    std::optional<EntityKey> group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    if (group)
    {
        this->SetGroup(std::move(*group));
    }

    std::optional<EntityWithLineage> invitedByEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InvitedByEntity", invitedByEntity));
    if (invitedByEntity)
    {
        this->SetInvitedByEntity(std::move(*invitedByEntity));
    }

    std::optional<EntityWithLineage> invitedEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InvitedEntity", invitedEntity));
    if (invitedEntity)
    {
        this->SetInvitedEntity(std::move(*invitedEntity));
    }

    String roleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleId", roleId));
    this->SetRoleId(std::move(roleId));

    return S_OK;
}

size_t InviteToGroupResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsInviteToGroupResponse const*> InviteToGroupResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InviteToGroupResponse>(&this->Model());
}

size_t InviteToGroupResponse::RequiredBufferSize(const PFGroupsInviteToGroupResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    if (model.invitedByEntity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.invitedByEntity);
    }
    if (model.invitedEntity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.invitedEntity);
    }
    if (model.roleId)
    {
        requiredSize += (std::strlen(model.roleId) + 1);
    }
    return requiredSize;
}

HRESULT InviteToGroupResponse::Copy(const PFGroupsInviteToGroupResponse& input, PFGroupsInviteToGroupResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.invitedByEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitedByEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.invitedEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitedEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue IsMemberRequest::ToJson() const
{
    return IsMemberRequest::ToJson(this->Model());
}

JsonValue IsMemberRequest::ToJson(const PFGroupsIsMemberRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    return output;
}

HRESULT IsMemberResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsMember", this->m_model.isMember));

    return S_OK;
}

size_t IsMemberResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsIsMemberResponse const*> IsMemberResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<IsMemberResponse>(&this->Model());
}

size_t IsMemberResponse::RequiredBufferSize(const PFGroupsIsMemberResponse& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT IsMemberResponse::Copy(const PFGroupsIsMemberResponse& input, PFGroupsIsMemberResponse& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ListGroupApplicationsRequest::ToJson() const
{
    return ListGroupApplicationsRequest::ToJson(this->Model());
}

JsonValue ListGroupApplicationsRequest::ToJson(const PFGroupsListGroupApplicationsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

HRESULT GroupApplication::FromJson(const JsonValue& input)
{
    std::optional<EntityWithLineage> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expires", this->m_model.expires));

    std::optional<EntityKey> group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    if (group)
    {
        this->SetGroup(std::move(*group));
    }

    return S_OK;
}

size_t GroupApplication::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGroupApplication const*> GroupApplication::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GroupApplication>(&this->Model());
}

size_t GroupApplication::RequiredBufferSize(const PFGroupsGroupApplication& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.entity);
    }
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    return requiredSize;
}

HRESULT GroupApplication::Copy(const PFGroupsGroupApplication& input, PFGroupsGroupApplication& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListGroupApplicationsResponse::FromJson(const JsonValue& input)
{
    ModelVector<GroupApplication> applications{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupApplication>(input, "Applications", applications));
    this->SetApplications(std::move(applications));

    return S_OK;
}

size_t ListGroupApplicationsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListGroupApplicationsResponse const*> ListGroupApplicationsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListGroupApplicationsResponse>(&this->Model());
}

size_t ListGroupApplicationsResponse::RequiredBufferSize(const PFGroupsListGroupApplicationsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsGroupApplication*) + sizeof(PFGroupsGroupApplication*) * model.applicationsCount);
    for (size_t i = 0; i < model.applicationsCount; ++i)
    {
        requiredSize += GroupApplication::RequiredBufferSize(*model.applications[i]);
    }
    return requiredSize;
}

HRESULT ListGroupApplicationsResponse::Copy(const PFGroupsListGroupApplicationsResponse& input, PFGroupsListGroupApplicationsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GroupApplication>(input.applications, input.applicationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.applications = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListGroupBlocksRequest::ToJson() const
{
    return ListGroupBlocksRequest::ToJson(this->Model());
}

JsonValue ListGroupBlocksRequest::ToJson(const PFGroupsListGroupBlocksRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

HRESULT GroupBlock::FromJson(const JsonValue& input)
{
    std::optional<EntityWithLineage> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    EntityKey group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    this->SetGroup(std::move(group));

    return S_OK;
}

size_t GroupBlock::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGroupBlock const*> GroupBlock::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GroupBlock>(&this->Model());
}

size_t GroupBlock::RequiredBufferSize(const PFGroupsGroupBlock& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.entity);
    }
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    return requiredSize;
}

HRESULT GroupBlock::Copy(const PFGroupsGroupBlock& input, PFGroupsGroupBlock& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListGroupBlocksResponse::FromJson(const JsonValue& input)
{
    ModelVector<GroupBlock> blockedEntities{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupBlock>(input, "BlockedEntities", blockedEntities));
    this->SetBlockedEntities(std::move(blockedEntities));

    return S_OK;
}

size_t ListGroupBlocksResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListGroupBlocksResponse const*> ListGroupBlocksResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListGroupBlocksResponse>(&this->Model());
}

size_t ListGroupBlocksResponse::RequiredBufferSize(const PFGroupsListGroupBlocksResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsGroupBlock*) + sizeof(PFGroupsGroupBlock*) * model.blockedEntitiesCount);
    for (size_t i = 0; i < model.blockedEntitiesCount; ++i)
    {
        requiredSize += GroupBlock::RequiredBufferSize(*model.blockedEntities[i]);
    }
    return requiredSize;
}

HRESULT ListGroupBlocksResponse::Copy(const PFGroupsListGroupBlocksResponse& input, PFGroupsListGroupBlocksResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GroupBlock>(input.blockedEntities, input.blockedEntitiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.blockedEntities = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListGroupInvitationsRequest::ToJson() const
{
    return ListGroupInvitationsRequest::ToJson(this->Model());
}

JsonValue ListGroupInvitationsRequest::ToJson(const PFGroupsListGroupInvitationsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

HRESULT GroupInvitation::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Expires", this->m_model.expires));

    std::optional<EntityKey> group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    if (group)
    {
        this->SetGroup(std::move(*group));
    }

    std::optional<EntityWithLineage> invitedByEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InvitedByEntity", invitedByEntity));
    if (invitedByEntity)
    {
        this->SetInvitedByEntity(std::move(*invitedByEntity));
    }

    std::optional<EntityWithLineage> invitedEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InvitedEntity", invitedEntity));
    if (invitedEntity)
    {
        this->SetInvitedEntity(std::move(*invitedEntity));
    }

    String roleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleId", roleId));
    this->SetRoleId(std::move(roleId));

    return S_OK;
}

size_t GroupInvitation::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGroupInvitation const*> GroupInvitation::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GroupInvitation>(&this->Model());
}

size_t GroupInvitation::RequiredBufferSize(const PFGroupsGroupInvitation& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    if (model.invitedByEntity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.invitedByEntity);
    }
    if (model.invitedEntity)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.invitedEntity);
    }
    if (model.roleId)
    {
        requiredSize += (std::strlen(model.roleId) + 1);
    }
    return requiredSize;
}

HRESULT GroupInvitation::Copy(const PFGroupsGroupInvitation& input, PFGroupsGroupInvitation& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.invitedByEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitedByEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityWithLineage>(input.invitedEntity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitedEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListGroupInvitationsResponse::FromJson(const JsonValue& input)
{
    ModelVector<GroupInvitation> invitations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupInvitation>(input, "Invitations", invitations));
    this->SetInvitations(std::move(invitations));

    return S_OK;
}

size_t ListGroupInvitationsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListGroupInvitationsResponse const*> ListGroupInvitationsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListGroupInvitationsResponse>(&this->Model());
}

size_t ListGroupInvitationsResponse::RequiredBufferSize(const PFGroupsListGroupInvitationsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsGroupInvitation*) + sizeof(PFGroupsGroupInvitation*) * model.invitationsCount);
    for (size_t i = 0; i < model.invitationsCount; ++i)
    {
        requiredSize += GroupInvitation::RequiredBufferSize(*model.invitations[i]);
    }
    return requiredSize;
}

HRESULT ListGroupInvitationsResponse::Copy(const PFGroupsListGroupInvitationsResponse& input, PFGroupsListGroupInvitationsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GroupInvitation>(input.invitations, input.invitationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitations = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListGroupMembersRequest::ToJson() const
{
    return ListGroupMembersRequest::ToJson(this->Model());
}

JsonValue ListGroupMembersRequest::ToJson(const PFGroupsListGroupMembersRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

HRESULT EntityMemberRole::FromJson(const JsonValue& input)
{
    ModelVector<EntityWithLineage> members{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityWithLineage>(input, "Members", members));
    this->SetMembers(std::move(members));

    String roleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleId", roleId));
    this->SetRoleId(std::move(roleId));

    String roleName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleName", roleName));
    this->SetRoleName(std::move(roleName));

    return S_OK;
}

size_t EntityMemberRole::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsEntityMemberRole const*> EntityMemberRole::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityMemberRole>(&this->Model());
}

size_t EntityMemberRole::RequiredBufferSize(const PFGroupsEntityMemberRole& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsEntityWithLineage*) + sizeof(PFGroupsEntityWithLineage*) * model.membersCount);
    for (size_t i = 0; i < model.membersCount; ++i)
    {
        requiredSize += EntityWithLineage::RequiredBufferSize(*model.members[i]);
    }
    if (model.roleId)
    {
        requiredSize += (std::strlen(model.roleId) + 1);
    }
    if (model.roleName)
    {
        requiredSize += (std::strlen(model.roleName) + 1);
    }
    return requiredSize;
}

HRESULT EntityMemberRole::Copy(const PFGroupsEntityMemberRole& input, PFGroupsEntityMemberRole& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EntityWithLineage>(input.members, input.membersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.members = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListGroupMembersResponse::FromJson(const JsonValue& input)
{
    ModelVector<EntityMemberRole> members{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityMemberRole>(input, "Members", members));
    this->SetMembers(std::move(members));

    return S_OK;
}

size_t ListGroupMembersResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListGroupMembersResponse const*> ListGroupMembersResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListGroupMembersResponse>(&this->Model());
}

size_t ListGroupMembersResponse::RequiredBufferSize(const PFGroupsListGroupMembersResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsEntityMemberRole*) + sizeof(PFGroupsEntityMemberRole*) * model.membersCount);
    for (size_t i = 0; i < model.membersCount; ++i)
    {
        requiredSize += EntityMemberRole::RequiredBufferSize(*model.members[i]);
    }
    return requiredSize;
}

HRESULT ListGroupMembersResponse::Copy(const PFGroupsListGroupMembersResponse& input, PFGroupsListGroupMembersResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EntityMemberRole>(input.members, input.membersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.members = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListMembershipRequest::ToJson() const
{
    return ListMembershipRequest::ToJson(this->Model());
}

JsonValue ListMembershipRequest::ToJson(const PFGroupsListMembershipRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GroupRole::FromJson(const JsonValue& input)
{
    String roleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleId", roleId));
    this->SetRoleId(std::move(roleId));

    String roleName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoleName", roleName));
    this->SetRoleName(std::move(roleName));

    return S_OK;
}

size_t GroupRole::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGroupRole const*> GroupRole::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GroupRole>(&this->Model());
}

size_t GroupRole::RequiredBufferSize(const PFGroupsGroupRole& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.roleId)
    {
        requiredSize += (std::strlen(model.roleId) + 1);
    }
    if (model.roleName)
    {
        requiredSize += (std::strlen(model.roleName) + 1);
    }
    return requiredSize;
}

HRESULT GroupRole::Copy(const PFGroupsGroupRole& input, PFGroupsGroupRole& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.roleId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.roleName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roleName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GroupWithRoles::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> group{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Group", group));
    if (group)
    {
        this->SetGroup(std::move(*group));
    }

    String groupName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GroupName", groupName));
    this->SetGroupName(std::move(groupName));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    ModelVector<GroupRole> roles{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupRole>(input, "Roles", roles));
    this->SetRoles(std::move(roles));

    return S_OK;
}

size_t GroupWithRoles::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsGroupWithRoles const*> GroupWithRoles::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GroupWithRoles>(&this->Model());
}

size_t GroupWithRoles::RequiredBufferSize(const PFGroupsGroupWithRoles& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.group)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.group);
    }
    if (model.groupName)
    {
        requiredSize += (std::strlen(model.groupName) + 1);
    }
    requiredSize += (alignof(PFGroupsGroupRole*) + sizeof(PFGroupsGroupRole*) * model.rolesCount);
    for (size_t i = 0; i < model.rolesCount; ++i)
    {
        requiredSize += GroupRole::RequiredBufferSize(*model.roles[i]);
    }
    return requiredSize;
}

HRESULT GroupWithRoles::Copy(const PFGroupsGroupWithRoles& input, PFGroupsGroupWithRoles& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.group);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.group = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.groupName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.groupName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<GroupRole>(input.roles, input.rolesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.roles = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListMembershipResponse::FromJson(const JsonValue& input)
{
    ModelVector<GroupWithRoles> groups{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupWithRoles>(input, "Groups", groups));
    this->SetGroups(std::move(groups));

    return S_OK;
}

size_t ListMembershipResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListMembershipResponse const*> ListMembershipResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListMembershipResponse>(&this->Model());
}

size_t ListMembershipResponse::RequiredBufferSize(const PFGroupsListMembershipResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsGroupWithRoles*) + sizeof(PFGroupsGroupWithRoles*) * model.groupsCount);
    for (size_t i = 0; i < model.groupsCount; ++i)
    {
        requiredSize += GroupWithRoles::RequiredBufferSize(*model.groups[i]);
    }
    return requiredSize;
}

HRESULT ListMembershipResponse::Copy(const PFGroupsListMembershipResponse& input, PFGroupsListMembershipResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GroupWithRoles>(input.groups, input.groupsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.groups = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListMembershipOpportunitiesRequest::ToJson() const
{
    return ListMembershipOpportunitiesRequest::ToJson(this->Model());
}

JsonValue ListMembershipOpportunitiesRequest::ToJson(const PFGroupsListMembershipOpportunitiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT ListMembershipOpportunitiesResponse::FromJson(const JsonValue& input)
{
    ModelVector<GroupApplication> applications{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupApplication>(input, "Applications", applications));
    this->SetApplications(std::move(applications));

    ModelVector<GroupInvitation> invitations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GroupInvitation>(input, "Invitations", invitations));
    this->SetInvitations(std::move(invitations));

    return S_OK;
}

size_t ListMembershipOpportunitiesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsListMembershipOpportunitiesResponse const*> ListMembershipOpportunitiesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListMembershipOpportunitiesResponse>(&this->Model());
}

size_t ListMembershipOpportunitiesResponse::RequiredBufferSize(const PFGroupsListMembershipOpportunitiesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGroupsGroupApplication*) + sizeof(PFGroupsGroupApplication*) * model.applicationsCount);
    for (size_t i = 0; i < model.applicationsCount; ++i)
    {
        requiredSize += GroupApplication::RequiredBufferSize(*model.applications[i]);
    }
    requiredSize += (alignof(PFGroupsGroupInvitation*) + sizeof(PFGroupsGroupInvitation*) * model.invitationsCount);
    for (size_t i = 0; i < model.invitationsCount; ++i)
    {
        requiredSize += GroupInvitation::RequiredBufferSize(*model.invitations[i]);
    }
    return requiredSize;
}

HRESULT ListMembershipOpportunitiesResponse::Copy(const PFGroupsListMembershipOpportunitiesResponse& input, PFGroupsListMembershipOpportunitiesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<GroupApplication>(input.applications, input.applicationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.applications = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<GroupInvitation>(input.invitations, input.invitationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitations = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RemoveGroupApplicationRequest::ToJson() const
{
    return RemoveGroupApplicationRequest::ToJson(this->Model());
}

JsonValue RemoveGroupApplicationRequest::ToJson(const PFGroupsRemoveGroupApplicationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue RemoveGroupInvitationRequest::ToJson() const
{
    return RemoveGroupInvitationRequest::ToJson(this->Model());
}

JsonValue RemoveGroupInvitationRequest::ToJson(const PFGroupsRemoveGroupInvitationRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue RemoveMembersRequest::ToJson() const
{
    return RemoveMembersRequest::ToJson(this->Model());
}

JsonValue RemoveMembersRequest::ToJson(const PFGroupsRemoveMembersRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "Members", input.members, input.membersCount);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    return output;
}

JsonValue UnblockEntityRequest::ToJson() const
{
    return UnblockEntityRequest::ToJson(this->Model());
}

JsonValue UnblockEntityRequest::ToJson(const PFGroupsUnblockEntityRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    return output;
}

JsonValue UpdateGroupRequest::ToJson() const
{
    return UpdateGroupRequest::ToJson(this->Model());
}

JsonValue UpdateGroupRequest::ToJson(const PFGroupsUpdateGroupRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AdminRoleId", input.adminRoleId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedProfileVersion", input.expectedProfileVersion);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "GroupName", input.groupName);
    JsonUtils::ObjectAddMember(output, "MemberRoleId", input.memberRoleId);
    return output;
}

HRESULT UpdateGroupResponse::FromJson(const JsonValue& input)
{
    String operationReason{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OperationReason", operationReason));
    this->SetOperationReason(std::move(operationReason));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    std::optional<PFOperationTypes> setResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SetResult", setResult));
    this->SetSetResult(std::move(setResult));

    return S_OK;
}

size_t UpdateGroupResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsUpdateGroupResponse const*> UpdateGroupResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateGroupResponse>(&this->Model());
}

size_t UpdateGroupResponse::RequiredBufferSize(const PFGroupsUpdateGroupResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.operationReason)
    {
        requiredSize += (std::strlen(model.operationReason) + 1);
    }
    if (model.setResult)
    {
        requiredSize += (alignof(PFOperationTypes) + sizeof(PFOperationTypes));
    }
    return requiredSize;
}

HRESULT UpdateGroupResponse::Copy(const PFGroupsUpdateGroupResponse& input, PFGroupsUpdateGroupResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.operationReason);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operationReason = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.setResult);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.setResult = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateGroupRoleRequest::ToJson() const
{
    return UpdateGroupRoleRequest::ToJson(this->Model());
}

JsonValue UpdateGroupRoleRequest::ToJson(const PFGroupsUpdateGroupRoleRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedProfileVersion", input.expectedProfileVersion);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Group", input.group);
    JsonUtils::ObjectAddMember(output, "RoleId", input.roleId);
    JsonUtils::ObjectAddMember(output, "RoleName", input.roleName);
    return output;
}

HRESULT UpdateGroupRoleResponse::FromJson(const JsonValue& input)
{
    String operationReason{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OperationReason", operationReason));
    this->SetOperationReason(std::move(operationReason));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    std::optional<PFOperationTypes> setResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SetResult", setResult));
    this->SetSetResult(std::move(setResult));

    return S_OK;
}

size_t UpdateGroupRoleResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGroupsUpdateGroupRoleResponse const*> UpdateGroupRoleResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateGroupRoleResponse>(&this->Model());
}

size_t UpdateGroupRoleResponse::RequiredBufferSize(const PFGroupsUpdateGroupRoleResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.operationReason)
    {
        requiredSize += (std::strlen(model.operationReason) + 1);
    }
    if (model.setResult)
    {
        requiredSize += (alignof(PFOperationTypes) + sizeof(PFOperationTypes));
    }
    return requiredSize;
}

HRESULT UpdateGroupRoleResponse::Copy(const PFGroupsUpdateGroupRoleResponse& input, PFGroupsUpdateGroupRoleResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.operationReason);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operationReason = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.setResult);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.setResult = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Groups

// Json serialization helpers

} // namespace PlayFab

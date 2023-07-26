#pragma once

#include <playfab/services/cpp/GroupsTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Groups
{

// Groups Classes
class AcceptGroupApplicationRequest : public Wrappers::PFGroupsAcceptGroupApplicationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsAcceptGroupApplicationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsAcceptGroupApplicationRequest& input);
};

class AcceptGroupInvitationRequest : public Wrappers::PFGroupsAcceptGroupInvitationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsAcceptGroupInvitationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsAcceptGroupInvitationRequest& input);
};

class AddMembersRequest : public Wrappers::PFGroupsAddMembersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsAddMembersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsAddMembersRequest& input);
};

class ApplyToGroupRequest : public Wrappers::PFGroupsApplyToGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsApplyToGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsApplyToGroupRequest& input);
};

class EntityWithLineage : public Wrappers::PFGroupsEntityWithLineageWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsEntityWithLineage>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsEntityWithLineageWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsEntityWithLineage const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsEntityWithLineage& model);
    static HRESULT Copy(const PFGroupsEntityWithLineage& input, PFGroupsEntityWithLineage& output, ModelBuffer& buffer);
};

class ApplyToGroupResponse : public Wrappers::PFGroupsApplyToGroupResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsApplyToGroupResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsApplyToGroupResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsApplyToGroupResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsApplyToGroupResponse& model);
    static HRESULT Copy(const PFGroupsApplyToGroupResponse& input, PFGroupsApplyToGroupResponse& output, ModelBuffer& buffer);
};

class BlockEntityRequest : public Wrappers::PFGroupsBlockEntityRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsBlockEntityRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsBlockEntityRequest& input);
};

class ChangeMemberRoleRequest : public Wrappers::PFGroupsChangeMemberRoleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsChangeMemberRoleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsChangeMemberRoleRequest& input);
};

class CreateGroupRequest : public Wrappers::PFGroupsCreateGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsCreateGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsCreateGroupRequest& input);
};

class CreateGroupResponse : public Wrappers::PFGroupsCreateGroupResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsCreateGroupResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsCreateGroupResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsCreateGroupResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsCreateGroupResponse& model);
    static HRESULT Copy(const PFGroupsCreateGroupResponse& input, PFGroupsCreateGroupResponse& output, ModelBuffer& buffer);
};

class CreateGroupRoleRequest : public Wrappers::PFGroupsCreateGroupRoleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsCreateGroupRoleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsCreateGroupRoleRequest& input);
};

class CreateGroupRoleResponse : public Wrappers::PFGroupsCreateGroupRoleResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsCreateGroupRoleResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsCreateGroupRoleResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsCreateGroupRoleResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsCreateGroupRoleResponse& model);
    static HRESULT Copy(const PFGroupsCreateGroupRoleResponse& input, PFGroupsCreateGroupRoleResponse& output, ModelBuffer& buffer);
};

class DeleteGroupRequest : public Wrappers::PFGroupsDeleteGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsDeleteGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsDeleteGroupRequest& input);
};

class DeleteRoleRequest : public Wrappers::PFGroupsDeleteRoleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsDeleteRoleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsDeleteRoleRequest& input);
};

class GetGroupRequest : public Wrappers::PFGroupsGetGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGetGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsGetGroupRequest& input);
};

class GetGroupResponse : public Wrappers::PFGroupsGetGroupResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGetGroupResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGetGroupResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGetGroupResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGetGroupResponse& model);
    static HRESULT Copy(const PFGroupsGetGroupResponse& input, PFGroupsGetGroupResponse& output, ModelBuffer& buffer);
};

class InviteToGroupRequest : public Wrappers::PFGroupsInviteToGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsInviteToGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsInviteToGroupRequest& input);
};

class InviteToGroupResponse : public Wrappers::PFGroupsInviteToGroupResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsInviteToGroupResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsInviteToGroupResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsInviteToGroupResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsInviteToGroupResponse& model);
    static HRESULT Copy(const PFGroupsInviteToGroupResponse& input, PFGroupsInviteToGroupResponse& output, ModelBuffer& buffer);
};

class IsMemberRequest : public Wrappers::PFGroupsIsMemberRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsIsMemberRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsIsMemberRequest& input);
};

class IsMemberResponse : public Wrappers::PFGroupsIsMemberResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsIsMemberResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsIsMemberResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsIsMemberResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsIsMemberResponse& model);
    static HRESULT Copy(const PFGroupsIsMemberResponse& input, PFGroupsIsMemberResponse& output, ModelBuffer& buffer);
};

class ListGroupApplicationsRequest : public Wrappers::PFGroupsListGroupApplicationsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupApplicationsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListGroupApplicationsRequest& input);
};

class GroupApplication : public Wrappers::PFGroupsGroupApplicationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGroupApplication>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGroupApplicationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGroupApplication const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGroupApplication& model);
    static HRESULT Copy(const PFGroupsGroupApplication& input, PFGroupsGroupApplication& output, ModelBuffer& buffer);
};

class ListGroupApplicationsResponse : public Wrappers::PFGroupsListGroupApplicationsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListGroupApplicationsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupApplicationsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListGroupApplicationsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListGroupApplicationsResponse& model);
    static HRESULT Copy(const PFGroupsListGroupApplicationsResponse& input, PFGroupsListGroupApplicationsResponse& output, ModelBuffer& buffer);
};

class ListGroupBlocksRequest : public Wrappers::PFGroupsListGroupBlocksRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupBlocksRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListGroupBlocksRequest& input);
};

class GroupBlock : public Wrappers::PFGroupsGroupBlockWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGroupBlock>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGroupBlockWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGroupBlock const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGroupBlock& model);
    static HRESULT Copy(const PFGroupsGroupBlock& input, PFGroupsGroupBlock& output, ModelBuffer& buffer);
};

class ListGroupBlocksResponse : public Wrappers::PFGroupsListGroupBlocksResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListGroupBlocksResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupBlocksResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListGroupBlocksResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListGroupBlocksResponse& model);
    static HRESULT Copy(const PFGroupsListGroupBlocksResponse& input, PFGroupsListGroupBlocksResponse& output, ModelBuffer& buffer);
};

class ListGroupInvitationsRequest : public Wrappers::PFGroupsListGroupInvitationsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupInvitationsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListGroupInvitationsRequest& input);
};

class GroupInvitation : public Wrappers::PFGroupsGroupInvitationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGroupInvitation>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGroupInvitationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGroupInvitation const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGroupInvitation& model);
    static HRESULT Copy(const PFGroupsGroupInvitation& input, PFGroupsGroupInvitation& output, ModelBuffer& buffer);
};

class ListGroupInvitationsResponse : public Wrappers::PFGroupsListGroupInvitationsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListGroupInvitationsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupInvitationsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListGroupInvitationsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListGroupInvitationsResponse& model);
    static HRESULT Copy(const PFGroupsListGroupInvitationsResponse& input, PFGroupsListGroupInvitationsResponse& output, ModelBuffer& buffer);
};

class ListGroupMembersRequest : public Wrappers::PFGroupsListGroupMembersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupMembersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListGroupMembersRequest& input);
};

class EntityMemberRole : public Wrappers::PFGroupsEntityMemberRoleWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsEntityMemberRole>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsEntityMemberRoleWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsEntityMemberRole const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsEntityMemberRole& model);
    static HRESULT Copy(const PFGroupsEntityMemberRole& input, PFGroupsEntityMemberRole& output, ModelBuffer& buffer);
};

class ListGroupMembersResponse : public Wrappers::PFGroupsListGroupMembersResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListGroupMembersResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListGroupMembersResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListGroupMembersResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListGroupMembersResponse& model);
    static HRESULT Copy(const PFGroupsListGroupMembersResponse& input, PFGroupsListGroupMembersResponse& output, ModelBuffer& buffer);
};

class ListMembershipRequest : public Wrappers::PFGroupsListMembershipRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListMembershipRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListMembershipRequest& input);
};

class GroupRole : public Wrappers::PFGroupsGroupRoleWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGroupRole>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGroupRoleWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGroupRole const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGroupRole& model);
    static HRESULT Copy(const PFGroupsGroupRole& input, PFGroupsGroupRole& output, ModelBuffer& buffer);
};

class GroupWithRoles : public Wrappers::PFGroupsGroupWithRolesWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsGroupWithRoles>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsGroupWithRolesWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsGroupWithRoles const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsGroupWithRoles& model);
    static HRESULT Copy(const PFGroupsGroupWithRoles& input, PFGroupsGroupWithRoles& output, ModelBuffer& buffer);
};

class ListMembershipResponse : public Wrappers::PFGroupsListMembershipResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListMembershipResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListMembershipResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListMembershipResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListMembershipResponse& model);
    static HRESULT Copy(const PFGroupsListMembershipResponse& input, PFGroupsListMembershipResponse& output, ModelBuffer& buffer);
};

class ListMembershipOpportunitiesRequest : public Wrappers::PFGroupsListMembershipOpportunitiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListMembershipOpportunitiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsListMembershipOpportunitiesRequest& input);
};

class ListMembershipOpportunitiesResponse : public Wrappers::PFGroupsListMembershipOpportunitiesResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsListMembershipOpportunitiesResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsListMembershipOpportunitiesResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsListMembershipOpportunitiesResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsListMembershipOpportunitiesResponse& model);
    static HRESULT Copy(const PFGroupsListMembershipOpportunitiesResponse& input, PFGroupsListMembershipOpportunitiesResponse& output, ModelBuffer& buffer);
};

class RemoveGroupApplicationRequest : public Wrappers::PFGroupsRemoveGroupApplicationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsRemoveGroupApplicationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsRemoveGroupApplicationRequest& input);
};

class RemoveGroupInvitationRequest : public Wrappers::PFGroupsRemoveGroupInvitationRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsRemoveGroupInvitationRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsRemoveGroupInvitationRequest& input);
};

class RemoveMembersRequest : public Wrappers::PFGroupsRemoveMembersRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsRemoveMembersRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsRemoveMembersRequest& input);
};

class UnblockEntityRequest : public Wrappers::PFGroupsUnblockEntityRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsUnblockEntityRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsUnblockEntityRequest& input);
};

class UpdateGroupRequest : public Wrappers::PFGroupsUpdateGroupRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsUpdateGroupRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsUpdateGroupRequest& input);
};

class UpdateGroupResponse : public Wrappers::PFGroupsUpdateGroupResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsUpdateGroupResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsUpdateGroupResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsUpdateGroupResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsUpdateGroupResponse& model);
    static HRESULT Copy(const PFGroupsUpdateGroupResponse& input, PFGroupsUpdateGroupResponse& output, ModelBuffer& buffer);
};

class UpdateGroupRoleRequest : public Wrappers::PFGroupsUpdateGroupRoleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsUpdateGroupRoleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGroupsUpdateGroupRoleRequest& input);
};

class UpdateGroupRoleResponse : public Wrappers::PFGroupsUpdateGroupRoleResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGroupsUpdateGroupRoleResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGroupsUpdateGroupRoleResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGroupsUpdateGroupRoleResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGroupsUpdateGroupRoleResponse& model);
    static HRESULT Copy(const PFGroupsUpdateGroupRoleResponse& input, PFGroupsUpdateGroupRoleResponse& output, ModelBuffer& buffer);
};

} // namespace Groups
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

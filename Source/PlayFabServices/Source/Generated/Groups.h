#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "GroupsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Groups
{

class GroupsAPI
{
public:
    GroupsAPI() = delete;
    GroupsAPI(const GroupsAPI& source) = delete;
    GroupsAPI& operator=(const GroupsAPI& source) = delete;
    ~GroupsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> AcceptGroupApplication(Entity const& entity, const AcceptGroupApplicationRequest& request, RunContext rc);
    static AsyncOp<void> AcceptGroupInvitation(Entity const& entity, const AcceptGroupInvitationRequest& request, RunContext rc);
    static AsyncOp<void> AddMembers(Entity const& entity, const AddMembersRequest& request, RunContext rc);
    static AsyncOp<ApplyToGroupResponse> ApplyToGroup(Entity const& entity, const ApplyToGroupRequest& request, RunContext rc);
    static AsyncOp<void> BlockEntity(Entity const& entity, const BlockEntityRequest& request, RunContext rc);
    static AsyncOp<void> ChangeMemberRole(Entity const& entity, const ChangeMemberRoleRequest& request, RunContext rc);
    static AsyncOp<CreateGroupResponse> CreateGroup(Entity const& entity, const CreateGroupRequest& request, RunContext rc);
    static AsyncOp<CreateGroupRoleResponse> CreateRole(Entity const& entity, const CreateGroupRoleRequest& request, RunContext rc);
    static AsyncOp<void> DeleteGroup(Entity const& entity, const DeleteGroupRequest& request, RunContext rc);
    static AsyncOp<void> DeleteRole(Entity const& entity, const DeleteRoleRequest& request, RunContext rc);
    static AsyncOp<GetGroupResponse> GetGroup(Entity const& entity, const GetGroupRequest& request, RunContext rc);
    static AsyncOp<InviteToGroupResponse> InviteToGroup(Entity const& entity, const InviteToGroupRequest& request, RunContext rc);
    static AsyncOp<IsMemberResponse> IsMember(Entity const& entity, const IsMemberRequest& request, RunContext rc);
    static AsyncOp<ListGroupApplicationsResponse> ListGroupApplications(Entity const& entity, const ListGroupApplicationsRequest& request, RunContext rc);
    static AsyncOp<ListGroupBlocksResponse> ListGroupBlocks(Entity const& entity, const ListGroupBlocksRequest& request, RunContext rc);
    static AsyncOp<ListGroupInvitationsResponse> ListGroupInvitations(Entity const& entity, const ListGroupInvitationsRequest& request, RunContext rc);
    static AsyncOp<ListGroupMembersResponse> ListGroupMembers(Entity const& entity, const ListGroupMembersRequest& request, RunContext rc);
    static AsyncOp<ListMembershipResponse> ListMembership(Entity const& entity, const ListMembershipRequest& request, RunContext rc);
    static AsyncOp<ListMembershipOpportunitiesResponse> ListMembershipOpportunities(Entity const& entity, const ListMembershipOpportunitiesRequest& request, RunContext rc);
    static AsyncOp<void> RemoveGroupApplication(Entity const& entity, const RemoveGroupApplicationRequest& request, RunContext rc);
    static AsyncOp<void> RemoveGroupInvitation(Entity const& entity, const RemoveGroupInvitationRequest& request, RunContext rc);
    static AsyncOp<void> RemoveMembers(Entity const& entity, const RemoveMembersRequest& request, RunContext rc);
    static AsyncOp<void> UnblockEntity(Entity const& entity, const UnblockEntityRequest& request, RunContext rc);
    static AsyncOp<UpdateGroupResponse> UpdateGroup(Entity const& entity, const UpdateGroupRequest& request, RunContext rc);
    static AsyncOp<UpdateGroupRoleResponse> UpdateRole(Entity const& entity, const UpdateGroupRoleRequest& request, RunContext rc);
};

} // namespace Groups
} // namespace PlayFab

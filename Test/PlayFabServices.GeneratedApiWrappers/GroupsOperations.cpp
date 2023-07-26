#include "TestAppPch.h"
#include "GroupsOperations.h"
#include <playfab/services/PFGroups.h>

namespace PlayFab
{
namespace Test
{


AcceptGroupApplicationOperation::AcceptGroupApplicationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> AcceptGroupApplicationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<AcceptGroupApplicationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT AcceptGroupApplicationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsAcceptGroupApplicationAsync(m_entity.Handle(), &m_request.Model(), async);
}



AcceptGroupInvitationOperation::AcceptGroupInvitationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> AcceptGroupInvitationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<AcceptGroupInvitationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT AcceptGroupInvitationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsAcceptGroupInvitationAsync(m_entity.Handle(), &m_request.Model(), async);
}



AddMembersOperation::AddMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> AddMembersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<AddMembersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT AddMembersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsAddMembersAsync(m_entity.Handle(), &m_request.Model(), async);
}



ApplyToGroupOperation::ApplyToGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ApplyToGroupOperation::ResultType> ApplyToGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ApplyToGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ApplyToGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsApplyToGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ApplyToGroupOperation::ResultType> ApplyToGroupOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsApplyToGroupGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsApplyToGroupResponse* result;
    RETURN_IF_FAILED(PFGroupsApplyToGroupGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


BlockEntityOperation::BlockEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> BlockEntityOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<BlockEntityOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT BlockEntityOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsBlockEntityAsync(m_entity.Handle(), &m_request.Model(), async);
}



ChangeMemberRoleOperation::ChangeMemberRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ChangeMemberRoleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ChangeMemberRoleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ChangeMemberRoleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsChangeMemberRoleAsync(m_entity.Handle(), &m_request.Model(), async);
}



CreateGroupOperation::CreateGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<CreateGroupOperation::ResultType> CreateGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsCreateGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<CreateGroupOperation::ResultType> CreateGroupOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsCreateGroupGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsCreateGroupResponse* result;
    RETURN_IF_FAILED(PFGroupsCreateGroupGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


CreateRoleOperation::CreateRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<CreateRoleOperation::ResultType> CreateRoleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateRoleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateRoleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsCreateRoleAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<CreateRoleOperation::ResultType> CreateRoleOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsCreateRoleGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsCreateGroupRoleResponse* result;
    RETURN_IF_FAILED(PFGroupsCreateRoleGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


DeleteGroupOperation::DeleteGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsDeleteGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}



DeleteRoleOperation::DeleteRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteRoleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteRoleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteRoleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsDeleteRoleAsync(m_entity.Handle(), &m_request.Model(), async);
}



GetGroupOperation::GetGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetGroupOperation::ResultType> GetGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsGetGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetGroupOperation::ResultType> GetGroupOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsGetGroupGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsGetGroupResponse* result;
    RETURN_IF_FAILED(PFGroupsGetGroupGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


InviteToGroupOperation::InviteToGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<InviteToGroupOperation::ResultType> InviteToGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<InviteToGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT InviteToGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsInviteToGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<InviteToGroupOperation::ResultType> InviteToGroupOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsInviteToGroupGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsInviteToGroupResponse* result;
    RETURN_IF_FAILED(PFGroupsInviteToGroupGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


IsMemberOperation::IsMemberOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<IsMemberOperation::ResultType> IsMemberOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<IsMemberOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT IsMemberOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsIsMemberAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<IsMemberOperation::ResultType> IsMemberOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFGroupsIsMemberResponse result{};
    RETURN_IF_FAILED(PFGroupsIsMemberGetResult(async, &result));
    return ResultType{ result };
}


ListGroupApplicationsOperation::ListGroupApplicationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListGroupApplicationsOperation::ResultType> ListGroupApplicationsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListGroupApplicationsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListGroupApplicationsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListGroupApplicationsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListGroupApplicationsOperation::ResultType> ListGroupApplicationsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListGroupApplicationsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListGroupApplicationsResponse* result;
    RETURN_IF_FAILED(PFGroupsListGroupApplicationsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListGroupBlocksOperation::ListGroupBlocksOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListGroupBlocksOperation::ResultType> ListGroupBlocksOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListGroupBlocksOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListGroupBlocksOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListGroupBlocksAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListGroupBlocksOperation::ResultType> ListGroupBlocksOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListGroupBlocksGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListGroupBlocksResponse* result;
    RETURN_IF_FAILED(PFGroupsListGroupBlocksGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListGroupInvitationsOperation::ListGroupInvitationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListGroupInvitationsOperation::ResultType> ListGroupInvitationsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListGroupInvitationsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListGroupInvitationsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListGroupInvitationsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListGroupInvitationsOperation::ResultType> ListGroupInvitationsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListGroupInvitationsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListGroupInvitationsResponse* result;
    RETURN_IF_FAILED(PFGroupsListGroupInvitationsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListGroupMembersOperation::ListGroupMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListGroupMembersOperation::ResultType> ListGroupMembersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListGroupMembersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListGroupMembersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListGroupMembersAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListGroupMembersOperation::ResultType> ListGroupMembersOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListGroupMembersGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListGroupMembersResponse* result;
    RETURN_IF_FAILED(PFGroupsListGroupMembersGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListMembershipOperation::ListMembershipOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListMembershipOperation::ResultType> ListMembershipOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListMembershipOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListMembershipOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListMembershipAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListMembershipOperation::ResultType> ListMembershipOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListMembershipGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListMembershipResponse* result;
    RETURN_IF_FAILED(PFGroupsListMembershipGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListMembershipOpportunitiesOperation::ListMembershipOpportunitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListMembershipOpportunitiesOperation::ResultType> ListMembershipOpportunitiesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListMembershipOpportunitiesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListMembershipOpportunitiesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsListMembershipOpportunitiesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListMembershipOpportunitiesOperation::ResultType> ListMembershipOpportunitiesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsListMembershipOpportunitiesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsListMembershipOpportunitiesResponse* result;
    RETURN_IF_FAILED(PFGroupsListMembershipOpportunitiesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


RemoveGroupApplicationOperation::RemoveGroupApplicationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> RemoveGroupApplicationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RemoveGroupApplicationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RemoveGroupApplicationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsRemoveGroupApplicationAsync(m_entity.Handle(), &m_request.Model(), async);
}



RemoveGroupInvitationOperation::RemoveGroupInvitationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> RemoveGroupInvitationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RemoveGroupInvitationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RemoveGroupInvitationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsRemoveGroupInvitationAsync(m_entity.Handle(), &m_request.Model(), async);
}



RemoveMembersOperation::RemoveMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> RemoveMembersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RemoveMembersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RemoveMembersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsRemoveMembersAsync(m_entity.Handle(), &m_request.Model(), async);
}



UnblockEntityOperation::UnblockEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> UnblockEntityOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UnblockEntityOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UnblockEntityOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsUnblockEntityAsync(m_entity.Handle(), &m_request.Model(), async);
}



UpdateGroupOperation::UpdateGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<UpdateGroupOperation::ResultType> UpdateGroupOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateGroupOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateGroupOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsUpdateGroupAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<UpdateGroupOperation::ResultType> UpdateGroupOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsUpdateGroupGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsUpdateGroupResponse* result;
    RETURN_IF_FAILED(PFGroupsUpdateGroupGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


UpdateRoleOperation::UpdateRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<UpdateRoleOperation::ResultType> UpdateRoleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateRoleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateRoleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFGroupsUpdateRoleAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<UpdateRoleOperation::ResultType> UpdateRoleOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFGroupsUpdateRoleGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFGroupsUpdateGroupRoleResponse* result;
    RETURN_IF_FAILED(PFGroupsUpdateRoleGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

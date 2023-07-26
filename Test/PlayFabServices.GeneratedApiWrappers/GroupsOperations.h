#pragma once

#include <playfab/services/cpp/GroupsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class AcceptGroupApplicationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsAcceptGroupApplicationRequestWrapper<Allocator>;

    AcceptGroupApplicationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class AcceptGroupInvitationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsAcceptGroupInvitationRequestWrapper<Allocator>;

    AcceptGroupInvitationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class AddMembersOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsAddMembersRequestWrapper<Allocator>;

    AddMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ApplyToGroupOperation : public XAsyncOperation<Wrappers::PFGroupsApplyToGroupResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsApplyToGroupRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsApplyToGroupResponseWrapper<Allocator>;

    ApplyToGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsApplyToGroupResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class BlockEntityOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsBlockEntityRequestWrapper<Allocator>;

    BlockEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ChangeMemberRoleOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsChangeMemberRoleRequestWrapper<Allocator>;

    ChangeMemberRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class CreateGroupOperation : public XAsyncOperation<Wrappers::PFGroupsCreateGroupResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsCreateGroupRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsCreateGroupResponseWrapper<Allocator>;

    CreateGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsCreateGroupResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class CreateRoleOperation : public XAsyncOperation<Wrappers::PFGroupsCreateGroupRoleResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsCreateGroupRoleRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsCreateGroupRoleResponseWrapper<Allocator>;

    CreateRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsCreateGroupRoleResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteGroupOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsDeleteGroupRequestWrapper<Allocator>;

    DeleteGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteRoleOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsDeleteRoleRequestWrapper<Allocator>;

    DeleteRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetGroupOperation : public XAsyncOperation<Wrappers::PFGroupsGetGroupResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsGetGroupRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsGetGroupResponseWrapper<Allocator>;

    GetGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsGetGroupResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class InviteToGroupOperation : public XAsyncOperation<Wrappers::PFGroupsInviteToGroupResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsInviteToGroupRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsInviteToGroupResponseWrapper<Allocator>;

    InviteToGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsInviteToGroupResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class IsMemberOperation : public XAsyncOperation<Wrappers::PFGroupsIsMemberResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsIsMemberRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsIsMemberResponseWrapper<Allocator>;

    IsMemberOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsIsMemberResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListGroupApplicationsOperation : public XAsyncOperation<Wrappers::PFGroupsListGroupApplicationsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListGroupApplicationsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListGroupApplicationsResponseWrapper<Allocator>;

    ListGroupApplicationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListGroupApplicationsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListGroupBlocksOperation : public XAsyncOperation<Wrappers::PFGroupsListGroupBlocksResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListGroupBlocksRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListGroupBlocksResponseWrapper<Allocator>;

    ListGroupBlocksOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListGroupBlocksResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListGroupInvitationsOperation : public XAsyncOperation<Wrappers::PFGroupsListGroupInvitationsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListGroupInvitationsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListGroupInvitationsResponseWrapper<Allocator>;

    ListGroupInvitationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListGroupInvitationsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListGroupMembersOperation : public XAsyncOperation<Wrappers::PFGroupsListGroupMembersResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListGroupMembersRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListGroupMembersResponseWrapper<Allocator>;

    ListGroupMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListGroupMembersResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListMembershipOperation : public XAsyncOperation<Wrappers::PFGroupsListMembershipResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListMembershipRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListMembershipResponseWrapper<Allocator>;

    ListMembershipOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListMembershipResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListMembershipOpportunitiesOperation : public XAsyncOperation<Wrappers::PFGroupsListMembershipOpportunitiesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsListMembershipOpportunitiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsListMembershipOpportunitiesResponseWrapper<Allocator>;

    ListMembershipOpportunitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsListMembershipOpportunitiesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class RemoveGroupApplicationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsRemoveGroupApplicationRequestWrapper<Allocator>;

    RemoveGroupApplicationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class RemoveGroupInvitationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsRemoveGroupInvitationRequestWrapper<Allocator>;

    RemoveGroupInvitationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class RemoveMembersOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsRemoveMembersRequestWrapper<Allocator>;

    RemoveMembersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class UnblockEntityOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFGroupsUnblockEntityRequestWrapper<Allocator>;

    UnblockEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class UpdateGroupOperation : public XAsyncOperation<Wrappers::PFGroupsUpdateGroupResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsUpdateGroupRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsUpdateGroupResponseWrapper<Allocator>;

    UpdateGroupOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsUpdateGroupResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class UpdateRoleOperation : public XAsyncOperation<Wrappers::PFGroupsUpdateGroupRoleResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFGroupsUpdateGroupRoleRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFGroupsUpdateGroupRoleResponseWrapper<Allocator>;

    UpdateRoleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFGroupsUpdateGroupRoleResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

#include "TestAppPch.h"
#include "GroupsTests.h"
#include "GroupsOperations.h"

namespace PlayFab
{
namespace Test
{

constexpr char joinerCustomId[]{ "GroupJoinerCustomId" };

struct GroupsTestsState
{
    String groupName;
    EntityKey group;
    std::optional<Entity> groupJoiner;

    // Helper to remove groupJoiner from the group
    AsyncOp<void> RemoveMember(Entity const& entity, RunContext rc)
    {
        RemoveMembersOperation::RequestType request;
        request.SetGroup(group);
        ModelVector<EntityKey> members;
        members.push_back(groupJoiner->EntityKey());
        request.SetMembers(std::move(members));
        return RemoveMembersOperation::Run(entity, std::move(request), rc);
    }
};

AsyncOp<void> GroupsTests::Initialize()
{
    m_state = MakeShared<GroupsTestsState>();

    // Use a unique group name each run
    Stringstream groupName;
    groupName << "TestGroup" << time(nullptr);
    m_state->groupName = groupName.str();

    return ServicesTestClass::Initialize().Then([&](Result<void> result) -> AsyncOp<CreateGroupOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        CreateGroupOperation::RequestType request;
        request.SetGroupName(m_state->groupName);
        return CreateGroupOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<CreateGroupOperation::ResultType> result) -> AsyncOp<Entity>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_state->group = *result.Payload().Model().group;
        return GetTitlePlayer(joinerCustomId);
    })
    .Then([&](Result<Entity> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_state->groupJoiner.emplace(result.ExtractPayload());
        return S_OK;
    });
}

AsyncOp<void> GroupsTests::Uninitialize()
{
    DeleteGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    return DeleteGroupOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result)
    {
        UNREFERENCED_PARAMETER(result);// Continue with uninitialize regardless of success deleting group
        m_state.reset();
        return ServicesTestClass::Uninitialize();
    });
}

void GroupsTests::TestAcceptGroupApplication(TestContext& tc)
{
    ApplyToGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    ApplyToGroupOperation::Run(*m_state->groupJoiner, request, RunContext()).Then([&](Result<ApplyToGroupOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        AcceptGroupApplicationOperation::RequestType request;
        request.SetEntity(*model.entity->key);
        request.SetGroup(*model.group);
        return AcceptGroupApplicationOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
        // Remove member regardless of success
        return m_state->RemoveMember(DefaultTitlePlayer(), RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestAcceptGroupInvitation(TestContext& tc)
{
    InviteToGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetEntity(m_state->groupJoiner->EntityKey());
    InviteToGroupOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<InviteToGroupOperation::ResultType> result) -> AsyncOp<ListGroupInvitationsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ListGroupInvitationsOperation::RequestType request;
        request.SetGroup(m_state->group);
        return ListGroupInvitationsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ListGroupInvitationsOperation::ResultType> result) ->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.invitationsCount, "invitationsCount");
        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, model.invitations[0]->invitedByEntity->key->id, "invitations[0]->invitedByEntity->key->id");
        tc.AssertEqual<String>(m_state->groupJoiner->EntityKey().Model().id, model.invitations[0]->invitedEntity->key->id, "invitations[0]->invitedEntity->key->id");

        AcceptGroupInvitationOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetEntity(m_state->groupJoiner->EntityKey());
        return AcceptGroupInvitationOperation::Run(*m_state->groupJoiner, request, RunContext());
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Remove member regardless of success
        return m_state->RemoveMember(DefaultTitlePlayer(), RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestAddMembers(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddMembersOperation::RequestType request;
    request.SetGroup(m_state->group);
    ModelVector<EntityKey> members;
    members.push_back(m_state->groupJoiner->EntityKey());
    request.SetMembers(std::move(members));

    AddMembersOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Remove member regardless of success
        return m_state->RemoveMember(DefaultTitlePlayer(), RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else 
    // Unsupported (Title Entity required)
    tc.Skip();
#endif
}

void GroupsTests::TestApplyToGroup(TestContext& tc)
{
    // Covered by GroupsTests::TestAcceptGroupApplication
    tc.EndTest(S_OK);
}

void GroupsTests::TestBlockEntity(TestContext& tc)
{
    // Covered by GroupsTests::TestListGroupBlocks
    tc.EndTest(S_OK);
}

constexpr char testRoleId[]{ "testRole" };
constexpr char testRoleName[]{ "Test Role" };

void GroupsTests::TestChangeMemberRole(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    CreateRoleOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetRoleId(testRoleId);
    request.SetRoleName(testRoleName);

    CreateRoleOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<CreateRoleOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(testRoleId, result.Payload().Model().roleId, "roleId");
        tc.AssertEqual<String>(testRoleName, result.Payload().Model().roleName, "roleName");

        AddMembersOperation::RequestType request;
        request.SetGroup(m_state->group);
        ModelVector<EntityKey> members;
        members.push_back(m_state->groupJoiner->EntityKey());
        request.SetMembers(std::move(members));
        return AddMembersOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ChangeMemberRoleOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetOriginRoleId("members");
        request.SetDestinationRoleId(testRoleId);
        ModelVector<EntityKey> members;
        members.push_back(m_state->groupJoiner->EntityKey());
        request.SetMembers(std::move(members));

        return ChangeMemberRoleOperation::Run(DefaultTitlePlayer(), std::move(request), RunContext());
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        RemoveMembersOperation::RequestType request;
        request.SetGroup(m_state->group);
        ModelVector<EntityKey> members;
        members.push_back(m_state->groupJoiner->EntityKey());
        request.SetMembers(std::move(members));
        return RemoveMembersOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        DeleteRoleOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetRoleId(testRoleId);
        return DeleteRoleOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // Unsupported (Title Entity required)
    tc.Skip();
#endif
}

void GroupsTests::TestCreateGroup(TestContext& tc)
{
    // Covered by GroupsTests::Initialize
    tc.EndTest(S_OK);
}

void GroupsTests::TestCreateRole(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    // Covered by GroupsTests::TestChangeMemberRole
    tc.EndTest(S_OK);
#else
    // Unsupported (Title Entity required)
    tc.Skip();
#endif
}

void GroupsTests::TestDeleteGroup(TestContext& tc)
{
    // Covered by GroupsTests::Uninitialize
    tc.EndTest(S_OK);
}

void GroupsTests::TestDeleteRole(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    // Covered by GroupsTests::TestChangeMemberRole
    tc.EndTest(S_OK);
#else
    // Unsupported (Title Entity required)
    tc.Skip();
#endif
}

void GroupsTests::TestGetGroup(TestContext& tc)
{
    GetGroupOperation::RequestType request;
    request.SetGroupName(m_state->groupName);
    GetGroupOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetGroupOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(m_state->group.Model().id, result.Payload().Model().group->id, "group.id");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestInviteToGroup(TestContext& tc)
{
    // Covered by GroupsTests::TestAcceptGroupInvitation
    tc.EndTest(S_OK);
}

void GroupsTests::TestIsMember(TestContext& tc)
{
    IsMemberOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetEntity(DefaultTitlePlayer().EntityKey());

    IsMemberOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<IsMemberOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(true, result.Payload().Model().isMember, "isMember");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestListGroupApplications(TestContext& tc)
{
    ApplyToGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    ApplyToGroupOperation::Run(*m_state->groupJoiner, request, RunContext()).Then([&](Result<ApplyToGroupOperation::ResultType> result) -> AsyncOp<ListGroupApplicationsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ListGroupApplicationsOperation::RequestType request;
        request.SetGroup(m_state->group);
        return ListGroupApplicationsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ListGroupApplicationsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.applicationsCount, "applicationsCount");
        tc.AssertEqual<String>(m_state->groupJoiner->EntityKey().Model().id, model.applications[0]->entity->key->id, "applications[0]->entity->key->id");
        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        RemoveGroupApplicationOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetEntity(m_state->groupJoiner->EntityKey());
        return RemoveGroupApplicationOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestListGroupBlocks(TestContext& tc)
{
    BlockEntityOperation::RequestType request;
    request.SetEntity(m_state->groupJoiner->EntityKey());
    request.SetGroup(m_state->group);

    BlockEntityOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<ListGroupBlocksOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ListGroupBlocksOperation::RequestType request;
        request.SetGroup(m_state->group);
        return ListGroupBlocksOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ListGroupBlocksOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.blockedEntitiesCount, "blockedEntitiesCount");
        tc.AssertEqual<String>(m_state->groupJoiner->EntityKey().Model().id, model.blockedEntities[0]->entity->key->id, "model.blockedEntities[0]->entity->key->id");
        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup step
        UnblockEntityOperation::RequestType request;
        request.SetEntity(m_state->groupJoiner->EntityKey());
        request.SetGroup(m_state->group);
        return UnblockEntityOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestListGroupInvitations(TestContext& tc)
{
    // Covered by GroupsTests::TestAcceptGroupInvitation
    tc.EndTest(S_OK);
}

void GroupsTests::TestListGroupMembers(TestContext& tc)
{
    ListGroupMembersOperation::RequestType request;
    request.SetGroup(m_state->group);
    ListGroupMembersOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ListGroupMembersOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        uint32_t totalMembers{ 0 };
        for (auto i = 0u; i < model.membersCount; ++i)
        {
            totalMembers += model.members[i]->membersCount;
        }
        tc.AssertEqual(1u, totalMembers, "totalMembers");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestListMembership(TestContext& tc)
{
    ListMembershipOperation::Run(DefaultTitlePlayer(), {}, RunContext()).Then([&](Result<ListMembershipOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertTrue(model.groupsCount >= 1, "No group memberships found");

        bool groupFound = false;
        for (size_t i = 0; i < model.groupsCount; ++i)
        {
            if (m_state->groupName == model.groups[i]->groupName)
            {
                groupFound = true;
                break;
            }
        }
        tc.AssertTrue(groupFound, "Expected group not found");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestListMembershipOpportunities(TestContext& tc)
{
    InviteToGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetEntity(m_state->groupJoiner->EntityKey());

    InviteToGroupOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<InviteToGroupOperation::ResultType> result) -> AsyncOp<ListMembershipOpportunitiesOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return ListMembershipOpportunitiesOperation::Run(*m_state->groupJoiner, {}, RunContext());
    })
    .Then([&](Result<ListMembershipOpportunitiesOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        for (size_t i = 0; i < model.invitationsCount; ++i)
        {
            if (!std::strcmp(m_state->group.Model().id, model.invitations[i]->group->id))
            {
                tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, model.invitations[i]->invitedByEntity->key->id, "model.invitations[0]->invitedByEntity->key->id");
                return S_OK;
            }
        }
        return E_FAIL; // invite not found
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        RemoveGroupInvitationOperation::RequestType request;
        request.SetEntity(m_state->groupJoiner->EntityKey());
        request.SetGroup(m_state->group);
        return RemoveGroupInvitationOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestRemoveGroupApplication(TestContext& tc)
{
    // Covered by GroupsTests::TestListGroupApplications
    tc.EndTest(S_OK);
}

void GroupsTests::TestRemoveGroupInvitation(TestContext& tc)
{
    // Covered by GroupsTests::TestListMembershipOpportunities
    tc.EndTest(S_OK);
}

void GroupsTests::TestRemoveMembers(TestContext& tc)
{
    // Covered by GroupsTests::TestAcceptGroupInvitation
    tc.EndTest(S_OK);
}

void GroupsTests::TestUnblockEntity(TestContext& tc)
{
    // Covered by GroupsTests::TestListGroupBlocks
    tc.EndTest(S_OK);
}

void GroupsTests::TestUpdateGroup(TestContext& tc)
{
    constexpr char newGroupName[]{ "newGroupName" };

    UpdateGroupOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetGroupName(newGroupName);
    UpdateGroupOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<UpdateGroupOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(PFOperationTypes::Updated, *model.setResult, "setResult");
        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        UpdateGroupOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetGroupName(m_state->groupName);
        return UpdateGroupOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<UpdateGroupOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(PFOperationTypes::Updated, *result.Payload().Model().setResult, "setResult");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void GroupsTests::TestUpdateRole(TestContext& tc)
{
    CreateRoleOperation::RequestType request;
    request.SetGroup(m_state->group);
    request.SetRoleId(testRoleId);
    request.SetRoleName(testRoleName);

    CreateRoleOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<CreateRoleOperation::ResultType> result) -> AsyncOp<UpdateRoleOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        UpdateRoleOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetRoleName("updatedRoleName");
        request.SetRoleId(testRoleId);
        return UpdateRoleOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<UpdateRoleOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(PFOperationTypes::Updated, *model.setResult, "setResult");
        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        DeleteRoleOperation::RequestType request;
        request.SetGroup(m_state->group);
        request.SetRoleId(testRoleId);
        return DeleteRoleOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

}
}

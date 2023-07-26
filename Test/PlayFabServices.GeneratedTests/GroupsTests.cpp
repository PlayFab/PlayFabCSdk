#include "TestAppPch.h"
#include "GroupsTests.h"

namespace PlayFab
{
namespace Test
{

void GroupsTests::AddTests()
{
    AddTest("TestAcceptGroupApplication", &GroupsTests::TestAcceptGroupApplication);
    AddTest("TestAcceptGroupInvitation", &GroupsTests::TestAcceptGroupInvitation);
    AddTest("TestAddMembers", &GroupsTests::TestAddMembers);
    AddTest("TestApplyToGroup", &GroupsTests::TestApplyToGroup);
    AddTest("TestBlockEntity", &GroupsTests::TestBlockEntity);
    AddTest("TestChangeMemberRole", &GroupsTests::TestChangeMemberRole);
    AddTest("TestCreateGroup", &GroupsTests::TestCreateGroup);
    AddTest("TestCreateRole", &GroupsTests::TestCreateRole);
    AddTest("TestDeleteGroup", &GroupsTests::TestDeleteGroup);
    AddTest("TestDeleteRole", &GroupsTests::TestDeleteRole);
    AddTest("TestGetGroup", &GroupsTests::TestGetGroup);
    AddTest("TestInviteToGroup", &GroupsTests::TestInviteToGroup);
    AddTest("TestIsMember", &GroupsTests::TestIsMember);
    AddTest("TestListGroupApplications", &GroupsTests::TestListGroupApplications);
    AddTest("TestListGroupBlocks", &GroupsTests::TestListGroupBlocks);
    AddTest("TestListGroupInvitations", &GroupsTests::TestListGroupInvitations);
    AddTest("TestListGroupMembers", &GroupsTests::TestListGroupMembers);
    AddTest("TestListMembership", &GroupsTests::TestListMembership);
    AddTest("TestListMembershipOpportunities", &GroupsTests::TestListMembershipOpportunities);
    AddTest("TestRemoveGroupApplication", &GroupsTests::TestRemoveGroupApplication);
    AddTest("TestRemoveGroupInvitation", &GroupsTests::TestRemoveGroupInvitation);
    AddTest("TestRemoveMembers", &GroupsTests::TestRemoveMembers);
    AddTest("TestUnblockEntity", &GroupsTests::TestUnblockEntity);
    AddTest("TestUpdateGroup", &GroupsTests::TestUpdateGroup);
    AddTest("TestUpdateRole", &GroupsTests::TestUpdateRole);

}

}
}

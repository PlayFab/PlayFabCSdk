#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class GroupsTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct GroupsTestsState> m_state;

private:
    // Autogenerated tests
    void TestAcceptGroupApplication(TestContext& testContext);
    void TestAcceptGroupInvitation(TestContext& testContext);
    void TestAddMembers(TestContext& testContext);
    void TestApplyToGroup(TestContext& testContext);
    void TestBlockEntity(TestContext& testContext);
    void TestChangeMemberRole(TestContext& testContext);
    void TestCreateGroup(TestContext& testContext);
    void TestCreateRole(TestContext& testContext);
    void TestDeleteGroup(TestContext& testContext);
    void TestDeleteRole(TestContext& testContext);
    void TestGetGroup(TestContext& testContext);
    void TestInviteToGroup(TestContext& testContext);
    void TestIsMember(TestContext& testContext);
    void TestListGroupApplications(TestContext& testContext);
    void TestListGroupBlocks(TestContext& testContext);
    void TestListGroupInvitations(TestContext& testContext);
    void TestListGroupMembers(TestContext& testContext);
    void TestListMembership(TestContext& testContext);
    void TestListMembershipOpportunities(TestContext& testContext);
    void TestRemoveGroupApplication(TestContext& testContext);
    void TestRemoveGroupInvitation(TestContext& testContext);
    void TestRemoveMembers(TestContext& testContext);
    void TestUnblockEntity(TestContext& testContext);
    void TestUpdateGroup(TestContext& testContext);
    void TestUpdateRole(TestContext& testContext);

};

} // namespace Test
} // namespace PlayFab

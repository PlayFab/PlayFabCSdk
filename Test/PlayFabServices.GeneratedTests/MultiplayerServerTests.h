#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class MultiplayerServerTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct MultiplayerServerTestsState> m_state;

private:
    // Autogenerated tests
#if 0
    void TestDeleteSecret(TestContext& testContext);
#endif
    void TestListBuildAliases(TestContext& testContext);
    void TestListBuildSummariesV2(TestContext& testContext);
    void TestListQosServersForTitle(TestContext& testContext);
#if 0
    void TestListSecretSummaries(TestContext& testContext);
#endif
    void TestRequestMultiplayerServer(TestContext& testContext);
#if 0
    void TestRequestPartyService(TestContext& testContext);
#endif
#if 0
    void TestUploadSecret(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

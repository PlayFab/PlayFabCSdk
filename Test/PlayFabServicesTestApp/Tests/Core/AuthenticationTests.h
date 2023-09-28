#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
namespace Test
{

class AuthenticationTests : public CoreTestClass
{
public:
    using CoreTestClass::CoreTestClass;

private:
    // TestClass overrides
    void AddTests() override;

    // Test Methods
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestAuthenticateGameServerWithCustomId(TestContext& testContext);
    void TestDelete(TestContext& testContext);
    void TestGetEntity(TestContext& testContext);
    void TestGetEntityMasterPlayerAccount(TestContext& testContext);
    void TestGetEntityWithSecretKey(TestContext& testContext);
    void TestServerLoginWithServerCustomId(TestContext& testContext);
    void TestServerLoginWithSteamId(TestContext& testContext);
    void TestServerLoginWithXbox(TestContext& testContext);
    void TestServerLoginWithXboxId(TestContext& testContext);
    void TestValidateEntityToken(TestContext& testContext);
#endif

    AsyncOp<LoginResult> LoginDefaultTitlePlayer();
};

}
}

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
#if 0 // removed until GetEntity is enabled
    void TestGetEntity(TestContext& testContext);
    void TestGetEntityMasterPlayerAccount(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestGetEntityWithSecretKey(TestContext& testContext);
#endif

    AsyncOp<LoginResult> LoginDefaultTitlePlayer();
};

}
}

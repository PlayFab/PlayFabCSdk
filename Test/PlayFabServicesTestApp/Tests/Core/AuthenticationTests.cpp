#include "TestAppPch.h"
#include "AuthenticationTests.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/PlayFabPal.h"

using namespace PlayFab::Platform;

namespace PlayFab
{
namespace Test
{

void AuthenticationTests::AddTests()
{
#if 0 // removed until GetEntity is enabled
    AddTest("TestGetEntity", &AuthenticationTests::TestGetEntity);
    AddTest("TestGetEntityMasterPlayerAccount", &AuthenticationTests::TestGetEntityMasterPlayerAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestGetEntityWithSecretKey", &AuthenticationTests::TestGetEntityWithSecretKey);
#endif
}

AsyncOp<LoginResult> AuthenticationTests::LoginDefaultTitlePlayer()
{
    return Platform::GetDefaultPlatformUser(RunContext()).Then([&](Result<UserPtr> result) -> AsyncOp<LoginResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return Platform::LoginDefaultTitlePlayer(ServiceConfig(), result.ExtractPayload(), RunContext());
    });
}

#if 0
void AuthenticationTests::TestGetEntity(TestContext& tc)
{
    SharedPtr<std::optional<LoginResult>> defaultTitlePlayer = MakeShared<std::optional<LoginResult>>();

    LoginDefaultTitlePlayer().Then([&, defaultTitlePlayer](Result<LoginResult> result)->AsyncOp<Entity>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *defaultTitlePlayer = result.ExtractPayload();
        Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator> request{};
        return RunOperation(MakeUnique<GetEntityOperation>(defaultTitlePlayer->value().entity, std::move(request), RunContext()));
    })
    .Then([&, defaultTitlePlayer](Result<Entity> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(defaultTitlePlayer->value().entity.EntityKey().Model().id, result.Payload().EntityKey().Model().id, "entityKeyId");
        tc.AssertEqual(defaultTitlePlayer->value().entity.EntityKey().Model().type, result.Payload().EntityKey().Model().type, "entityKeyType");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

constexpr char kMasterAccountType[]{ "master_player_account" };

void AuthenticationTests::TestGetEntityMasterPlayerAccount(TestContext& tc)
{
    SharedPtr<std::optional<LoginResult>> defaultTitlePlayer = MakeShared<std::optional<LoginResult>>();

    LoginDefaultTitlePlayer().Then([&, defaultTitlePlayer](Result<LoginResult> result)->AsyncOp<Entity>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *defaultTitlePlayer = result.ExtractPayload();
        Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator> request{};
        Wrappers::PFEntityKeyWrapper<Allocator> key{};
        key.SetId(defaultTitlePlayer->value().loginResult.Model().playFabId);
        key.SetType(kMasterAccountType);
        request.SetEntity(key);
        return RunOperation(MakeUnique<GetEntityOperation>(defaultTitlePlayer->value().entity, std::move(request), RunContext()));
    })
    .Then([&, defaultTitlePlayer](Result<Entity> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(defaultTitlePlayer->value().loginResult.Model().playFabId, result.Payload().EntityKey().Model().id, "entityKeyId");
        tc.AssertEqual(kMasterAccountType, result.Payload().EntityKey().Model().type, "entityKeyType");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
constexpr char kTitleType[]{ "title" };

void AuthenticationTests::TestGetEntityWithSecretKey(TestContext& tc)
{
    Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator> request{};
    RunOperation(MakeUnique<GetEntityWithSecretKeyOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), std::move(request), RunContext())).Then([&](Result<Entity> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(m_testTitleData.titleId.data(), result.Payload().EntityKey().Model().id, "titleId");
        tc.AssertEqual(kTitleType, result.Payload().EntityKey().Model().type, "titleType");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif // HC_PLATFORM == HC_PLATFORM_WIN32

}
}

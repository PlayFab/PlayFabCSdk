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
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestAuthenticateGameServerWithCustomId", &AuthenticationTests::TestAuthenticateGameServerWithCustomId);
    AddTest("TestDelete", &AuthenticationTests::TestDelete);
    AddTest("TestGetEntity", &AuthenticationTests::TestGetEntity);
    AddTest("TestGetEntityMasterPlayerAccount", &AuthenticationTests::TestGetEntityMasterPlayerAccount);
    AddTest("TestGetEntityWithSecretKey", &AuthenticationTests::TestGetEntityWithSecretKey);
    AddTest("TestServerLoginWithServerCustomId", &AuthenticationTests::TestServerLoginWithServerCustomId);
    AddTest("TestServerLoginWithSteamId", &AuthenticationTests::TestServerLoginWithSteamId);
    AddTest("TestServerLoginWithXbox", &AuthenticationTests::TestServerLoginWithXbox);
    AddTest("TestServerLoginWithXboxId", &AuthenticationTests::TestServerLoginWithXboxId);
    AddTest("TestValidateEntityToken", &AuthenticationTests::TestValidateEntityToken);
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

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestAuthenticateGameServerWithCustomId(TestContext& tc)
{
    SharedPtr<std::optional<Entity>> titleEntity = MakeShared<std::optional<Entity>>();
    Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator> request{};

    RunOperation(MakeUnique<GetEntityWithSecretKeyOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), std::move(request), RunContext())).Then([&, titleEntity](Result<Entity> result) -> AsyncOp<Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *titleEntity = result.ExtractPayload();
        AuthenticateGameServerWithCustomIdOperation::RequestType request;
        request.SetCustomId("TestAuthenticateGameServerWithCustomId");

        return RunOperation(MakeUnique<AuthenticateGameServerWithCustomIdOperation>(titleEntity->value(), request, RunContext()));
    })
    .Then([&, titleEntity](Result<Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>> result) -> AsyncOp<Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ValidateEntityTokenOperation::RequestType request;
        request.SetEntityToken(result.Payload().Model().entityToken->entityToken);

        return RunOperation(MakeUnique<ValidateEntityTokenOperation>(titleEntity->value(), request, RunContext()));
    })
    .Then([&, titleEntity](Result<Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        DeleteOperation::RequestType request;
        request.SetEntity(*result.Payload().Model().entity);

        return RunOperation(MakeUnique<DeleteOperation>(titleEntity->value(), request, RunContext()));
    })
    .Then([&](Result<void> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestDelete(TestContext& tc)
{
    // Covered by TestAuthenticateGameServerWithCustomId
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
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
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
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
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestServerLoginWithServerCustomId(TestContext& tc)
{
    ServerLoginWithServerCustomIdOperation::RequestType request;
    request.SetServerCustomId("TestServerLoginWithCustomId");

    RunOperation(MakeUnique<ServerLoginWithServerCustomIdOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), request, RunContext())).Then([&](Result<ServerLoginResult> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("A5CE1CC3C44401AD", result.Payload().loginResult.Model().playFabId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestServerLoginWithSteamId(TestContext& tc)
{
    ServerLoginWithSteamIdOperation::RequestType request;
    request.SetSteamId("76561199383523607");
    
    RunOperation(MakeUnique<ServerLoginWithSteamIdOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), request, RunContext())).Then([&](Result<ServerLoginResult> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("161316EBD27587FA", result.Payload().loginResult.Model().playFabId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestServerLoginWithXbox(TestContext& tc)
{
    ServerLoginWithXboxOperation::RequestType request;
    request.SetXboxToken("testToken");

    RunOperation(MakeUnique<ServerLoginWithXboxOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), request, RunContext())).Then([&](Result<ServerLoginResult> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_XBOX_LIVE_TOKEN, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestServerLoginWithXboxId(TestContext& tc)
{
    ServerLoginWithXboxIdOperation::RequestType request;
    request.SetXboxId("2814639779201810");
    request.SetSandbox("XDKS.1");

    RunOperation(MakeUnique<ServerLoginWithXboxIdOperation>(ServiceConfig(), m_testTitleData.secretKey.data(), request, RunContext())).Then([&](Result<ServerLoginResult> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("A13E73660B38980B", result.Payload().loginResult.Model().playFabId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AuthenticationTests::TestValidateEntityToken(TestContext& tc)
{
    // Covered by TestAuthenticateGameServerWithCustomId
    tc.Skip();
}
#endif

}
}

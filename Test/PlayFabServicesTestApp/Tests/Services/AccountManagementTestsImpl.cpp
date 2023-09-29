#include "TestAppPch.h"
#include "AccountManagementTests.h"
#include "AccountManagementOperations.h"
#include "Operations/Core/AuthenticationOperations.h"
#if HC_PLATFORM == HC_PLATFORM_GDK
#include "GDK/PlatformUser_GDK.h"
#endif

namespace PlayFab
{
namespace Test
{

struct AccountManagementTestsState
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    String defaultXUserXuidString;
#endif
};

AsyncOp<void> AccountManagementTests::Initialize()
{
    return ServicesTestClass::Initialize().Then([&](Result<void> result) -> Result<void>
    {
#if HC_PLATFORM == HC_PLATFORM_GDK
        auto user = DefaultPlatformUser();
        assert(user);
        Stringstream xuidString;
        xuidString << user->Id();
        m_state = MakeShared<AccountManagementTestsState>(AccountManagementTestsState{ xuidString.str() });
        return S_OK;
#else
        return S_OK;
#endif
    });
}

AsyncOp<void> AccountManagementTests::Uninitialize()
{
    m_state.reset();
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientAddOrUpdateContactEmail(TestContext& tc)
{
    ClientAddOrUpdateContactEmailOperation::RequestType request;
    request.SetEmailAddress(DefaultTitlePlayerId() + "@email.com");

    ClientAddOrUpdateContactEmailOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerSendEmailFromTemplateOperation::RequestType request;
        request.SetEmailTemplateId("F803122B2E31744A");
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerSendEmailFromTemplateOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        tc.AssertEqual(E_PF_SMTP_ADDON_NOT_ENABLED, result.hr, "errorName");

        ClientRemoveContactEmailOperation::RequestType request;
        return ClientRemoveContactEmailOperation::Run(DefaultTitlePlayer(), request, RunContext());
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
void AccountManagementTests::TestClientAddUsernamePassword(TestContext& tc)
{
    LoginWithCustomIDOperation::RequestType request;
    request.SetCreateAccount(true);
    request.SetCustomId("AddUserPassCustomId");
    char* playfabId = new char[30u];

    RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext())).Then([&, playfabId](Result<LoginResult> result) -> AsyncOp<ClientAddUsernamePasswordOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        strcpy_s(playfabId, 30u, result.Payload().loginResult.Model().playFabId);

        ClientAddUsernamePasswordOperation::RequestType request;
        request.SetEmail("usernamepassword@email.com");
        request.SetUsername(playfabId);
        request.SetPassword("password");

        return ClientAddUsernamePasswordOperation::Run(result.Payload().entity, request, RunContext());
    })
    .Then([&, playfabId](Result<ClientAddUsernamePasswordOperation::ResultType> result) -> AsyncOp<void>
    {   
        tc.AssertEqual(E_PF_ACCOUNT_ALREADY_LINKED, result.hr, "errorName");

        ServerSendCustomAccountRecoveryEmailOperation::RequestType request;
        request.SetEmailTemplateId("F803122B2E31744A");
        request.SetEmail("usernamepassword@email.com");

        return ServerSendCustomAccountRecoveryEmailOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, playfabId](Result<void> result) -> AsyncOp<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_FOUND, result.hr, "errorName");

        ServerDeletePlayerOperation::RequestType request;
        request.SetPlayFabId(playfabId);

        return ServerDeletePlayerOperation::Run(TitleEntity(), request, RunContext());
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

#if HC_PLATFORM == HC_PLATFORM_GDK
void VerifyAccountInfoResultPlatformDependent(TestContext& tc, AccountManagementTestsState& testState, ClientGetAccountInfoOperation::ResultType const& result)
{
    auto& model = result.Model();
    tc.AssertEqual<String>(testState.defaultXUserXuidString, model.accountInfo->xboxInfo->xboxUserId, "accountInfo->xboxInfo->xboxUserId");
}
#else
void VerifyAccountInfoResultPlatformDependent(TestContext& tc, AccountManagementTestsState& testState, ClientGetAccountInfoOperation::ResultType const& result)
{
    UNREFERENCED_PARAMETER(tc);
    UNREFERENCED_PARAMETER(testState);
    UNREFERENCED_PARAMETER(result);
}
#endif

void AccountManagementTests::TestClientGetAccountInfo(TestContext& tc)
{
    ClientGetAccountInfoOperation::Run(DefaultTitlePlayer(), {}, RunContext()).Then([&](Result<ClientGetAccountInfoOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().Model().accountInfo->titleInfo->titlePlayerAccount->id, "accountInfo->titleInfo->titlePlayerAccount->id");
        // Verify platform specific fields
        VerifyAccountInfoResultPlatformDependent(tc, *this->m_state, result.Payload());
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void AccountManagementTests::TestClientGetPlayerCombinedInfo(TestContext& tc)
{
    ClientGetPlayerCombinedInfoOperation::RequestType request;
    Wrappers::PFGetPlayerCombinedInfoRequestParamsWrapper<Allocator> requestParams;
    requestParams.SetGetTitleData(true);
    requestParams.SetGetPlayerProfile(true);
    requestParams.SetGetUserData(true);
    requestParams.SetGetUserAccountInfo(true);
    request.SetInfoRequestParameters(requestParams);

    ClientGetPlayerCombinedInfoOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientGetPlayerCombinedInfoOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(DefaultTitlePlayerId(), result.Payload().Model().playFabId, "playFabId");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void AccountManagementTests::TestClientGetPlayerProfile(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
    // CustomId of the player whose profile we'll retreive
    constexpr char customId[]{ "ProfileCustomId" };

    auto playFabId = MakeShared<String>();

    GetPlayFabIdFromCustomId(customId).Then([&, playFabId](Result<String> result) -> AsyncOp<ClientGetPlayerProfileOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *playFabId = result.ExtractPayload();

        ClientGetPlayerProfileOperation::RequestType request;
        request.SetPlayFabId(*playFabId);
        return ClientGetPlayerProfileOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
        .Then([&, playFabId](Result<ClientGetPlayerProfileOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(*playFabId, result.Payload().Model().playerProfile->playerId, "playerProfile->playerId");
        return S_OK;
    })
        .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // GDK doesn't support GetPlayFabIdFromCustomId
    tc.Skip();
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromFacebookIDsOperation::RequestType request;
    request.SetFacebookIDs({ "testId" });

    ClientGetPlayFabIDsFromFacebookIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromFacebookIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds(TestContext& tc)
{
    ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::RequestType request;
    request.SetFacebookInstantGamesIds({ "testId" });

    ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromGameCenterIDsOperation::RequestType request;
    request.SetGameCenterIDs({ "testId" });

    ClientGetPlayFabIDsFromGameCenterIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromGameCenterIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromGoogleIDsOperation::RequestType request;
    request.SetGoogleIDs({ "testId" });

    ClientGetPlayFabIDsFromGoogleIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromGoogleIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::RequestType request;
    request.SetGooglePlayGamesPlayerIDs({ "testId" });

    ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromKongregateIDsOperation::RequestType request;
    request.SetKongregateIDs({ "testId" });

    ClientGetPlayFabIDsFromKongregateIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromKongregateIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds(TestContext& tc)
{
    ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::RequestType request;
    request.SetNintendoAccountIds({ "testId" });

    ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds(TestContext& tc)
{
    ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::RequestType request;
    request.SetNintendoSwitchDeviceIds({ "testId" });

    ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromPSNAccountIDsOperation::RequestType request;
    request.SetPSNAccountIDs({ "testId" });

    ClientGetPlayFabIDsFromPSNAccountIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromSteamIDsOperation::RequestType request;
    request.SetSteamStringIDs({ "76561199383523607" });

    ClientGetPlayFabIDsFromSteamIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientGetPlayFabIDsFromSteamIDsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("161316EBD27587FA", result.Payload().Model().data[0]->playFabId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromTwitchIDsOperation::RequestType request;
    request.SetTwitchIds({ "testId" });

    ClientGetPlayFabIDsFromTwitchIDsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<ClientGetPlayFabIDsFromTwitchIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
constexpr char xboxLiveSandboxId[]{ "XDKS.1" };

void AccountManagementTests::TestClientGetPlayFabIDsFromXboxLiveIDs(TestContext& tc)
{
    ClientGetPlayFabIDsFromXboxLiveIDsOperation::RequestType request;
    request.SetSandbox(xboxLiveSandboxId);
#if HC_PLATFORM == HC_PLATFORM_GDK
    request.SetXboxLiveAccountIDs({ m_state->defaultXUserXuidString });
#else
    request.SetXboxLiveAccountIDs({ "2814639779201810" });
#endif

    ClientGetPlayFabIDsFromXboxLiveIDsOperation::Run(DefaultTitlePlayer(), request, RunContext())
        .Then([&](Result<ClientGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.dataCount, "dataCount");
        tc.AssertEqual<uint32_t>(1, model.dataCount, "model.dataCount");
        tc.AssertEqual<bool>(true, model.data[0]->playFabId != nullptr, "model.data[0]->playFabId null");
#if HC_PLATFORM == HC_PLATFORM_GDK
        tc.AssertEqual<String>(DefaultTitlePlayerId(), model.data[0]->playFabId, "data[0]->playFabId");
#else
        tc.AssertEqual<String>("A13E73660B38980B", model.data[0]->playFabId, "data[0]->playFabId");
#endif
        return S_OK;
    })
        .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkAndroidDeviceID(TestContext& tc)
{
    ClientLinkAndroidDeviceIDOperation::RequestType request;
    request.SetAndroidDeviceId("testId");
    request.SetForceLink(true);

    ClientLinkAndroidDeviceIDOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkApple(TestContext& tc)
{
    ClientLinkAppleOperation::RequestType request;
    request.SetIdentityToken("testId");
    request.SetForceLink(true);

    ClientLinkAppleOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_APPLE_NOT_ENABLED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void AccountManagementTests::TestClientLinkCustomID(TestContext& tc)
{
    static constexpr char defaultUserNewCustomId[]{ "defaultUserNewCustomId2" };

    ClientLinkCustomIDOperation::RequestType request;
    request.SetCustomId(defaultUserNewCustomId);

    ClientLinkCustomIDOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<LoginResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        LoginWithCustomIDOperation::RequestType request;
        request.SetCustomId(defaultUserNewCustomId);
        return RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext()));
    })
    .Then([&](Result<LoginResult> result)-> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // basic sanity check that link worked
        tc.AssertEqual<String>(DefaultTitlePlayerId(), result.Payload().loginResult.Model().playFabId, "playFabId");
        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().entity.EntityKey().Model().id, "EntityKey.id");
        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        return ClientUnlinkCustomIDOperation::Run(DefaultTitlePlayer(), {}, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<LoginResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        LoginWithCustomIDOperation::RequestType request;
        request.SetCustomId(defaultUserNewCustomId);
        return RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext()));
    })
    .Then([&](Result<LoginResult> result) -> Result<void>
    {
        // Login should fail after unlink since we specified createAccount=false in the request
        tc.AssertFailed(std::move(result), "Login Succeeded unexpectedly after UnlinkCustomID");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkFacebookAccount(TestContext& tc)
{
    ClientLinkFacebookAccountOperation::RequestType request;
    request.SetAccessToken("testId");
    request.SetForceLink(true);

    ClientLinkFacebookAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_FACEBOOK_TOKEN, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkFacebookInstantGamesId(TestContext& tc)
{
    ClientLinkFacebookInstantGamesIdOperation::RequestType request;
    request.SetFacebookInstantGamesSignature("testId");
    request.SetForceLink(true);

    ClientLinkFacebookInstantGamesIdOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_FACEBOOK_INSTANT_GAMES_AUTH_NOT_CONFIGURED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkGameCenterAccount(TestContext& tc)
{
    ClientLinkGameCenterAccountOperation::RequestType request;
    request.SetGameCenterId("testId");
    request.SetForceLink(true);

    ClientLinkGameCenterAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkGoogleAccount(TestContext& tc)
{
    ClientLinkGoogleAccountOperation::RequestType request;
    request.SetServerAuthCode("testId");
    request.SetForceLink(true);

    ClientLinkGoogleAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_GOOGLE_O_AUTH_NOT_CONFIGURED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount(TestContext& tc)
{
    ClientLinkGooglePlayGamesServicesAccountOperation::RequestType request;
    request.SetServerAuthCode("testId");
    request.SetForceLink(true);

    ClientLinkGooglePlayGamesServicesAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_GOOGLE_O_AUTH_NOT_CONFIGURED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkIOSDeviceID(TestContext& tc)
{
    ClientLinkIOSDeviceIDOperation::RequestType request;
    request.SetDeviceId("testId");
    request.SetForceLink(true);

    ClientLinkIOSDeviceIDOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkKongregate(TestContext& tc)
{
    ClientLinkKongregateOperation::RequestType request;
    request.SetKongregateId("testId");
    request.SetAuthTicket("testTicket");
    request.SetForceLink(true);

    ClientLinkKongregateOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_FEATURE_NOT_CONFIGURED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientLinkNintendoServiceAccount(TestContext& tc)
{
    ClientLinkNintendoServiceAccountOperation::RequestType request;
    request.SetIdentityToken("eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjM0NTY3ODkwIn0.r43DtzFd6FGE_QGYECRIMKXma8YnwAD0WnPdMVwQRTE");
    request.SetForceLink(true);

    ClientLinkNintendoServiceAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_NOT_AUTHORIZED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkNintendoSwitchDeviceId(TestContext& tc)
{
    ClientLinkNintendoSwitchDeviceIdOperation::RequestType request;
    request.SetNintendoSwitchDeviceId("testId");
    request.SetForceLink(true);

    ClientLinkNintendoSwitchDeviceIdOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void AccountManagementTests::TestClientLinkOpenIdConnect(TestContext& tc)
{
    ClientLinkOpenIdConnectOperation::RequestType request;
    request.SetConnectionId("testId");
    request.SetIdToken("testToken");
    request.SetForceLink(true);

    ClientLinkOpenIdConnectOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_IDENTITY_PROVIDER_ID, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkPSNAccount(TestContext& tc)
{
    ClientLinkPSNAccountOperation::RequestType request;
    request.SetAuthCode("testId");
    request.SetRedirectUri("dummyUrl");
    request.SetForceLink(true);

    ClientLinkPSNAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_PSN_AUTH_CODE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientLinkSteamAccount(TestContext& tc)
{
    ClientLinkSteamAccountOperation::RequestType request;
    request.SetSteamTicket("testId");
    request.SetForceLink(true);

    ClientLinkSteamAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_STEAM_TICKET, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientLinkTwitch(TestContext& tc)
{
    ClientLinkTwitchOperation::RequestType request;
    request.SetAccessToken("testId");
    request.SetForceLink(true);

    ClientLinkTwitchOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_FEATURE_NOT_CONFIGURED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientLinkXboxAccount(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    constexpr char customIdToLink[]{ "CustomIdToLinkXboxAccount" };

    auto customIdEntityToLink = MakeShared<std::optional<Entity>>();

    GetTitlePlayer(customIdToLink).Then([&, customIdEntityToLink](Result<Entity> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        customIdEntityToLink->emplace(result.ExtractPayload());

        assert(DefaultPlatformUser());
        ClientLinkXboxAccountOperation::RequestType request{ Wrappers::XUser::Duplicate(DefaultPlatformUser()->Handle()) };
        request.SetForceLink(true);

        return ClientLinkXboxAccountOperation::Run(**customIdEntityToLink, request, RunContext());
    })
    .Then([&, customIdEntityToLink](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        return ClientUnlinkXboxAccountOperation::Run(**customIdEntityToLink, {}, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // This test would be doable on Win32 but would need to bring in a XAL dependency to get an Xbox token.
    // For now, this will be GDK only test.
    tc.Skip();
#endif
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientRemoveContactEmail(TestContext& tc)
{
    // This is already covered by TestClientAddOrUpdateContactEmail
    tc.Skip();
}
#endif

void AccountManagementTests::TestClientReportPlayer(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
    constexpr char customIdToReport[]{ "customIdToReport" };

    GetPlayFabIdFromCustomId(customIdToReport).Then([&](Result<String> result) -> AsyncOp<ClientReportPlayerOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ClientReportPlayerOperation::RequestType request;
        request.SetReporteeId(result.ExtractPayload());
        return ClientReportPlayerOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
        .Finally([&](Result<ClientReportPlayerOperation::ResultType> result)
    {
        if (result.hr == E_PF_OVER_LIMIT)
        {
            // This API can only be called 5 times a day. Because of this low limit, don't treat E_PF_OVERGAMEMANAGERLIMIT results
            // as test failures
            tc.EndTest(S_OK);
        }
        else
        {
            tc.EndTest(std::move(result));
        }
    });
#else
    // GDK doesn't support GetPlayFabIdFromCustomId
    tc.Skip();
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientSendAccountRecoveryEmail(TestContext& tc)
{
    ClientSendAccountRecoveryEmailOperation::RequestType request;
    request.SetEmail("foo123@bar123.com");
    request.SetTitleId(m_testTitleData.titleId);

    ClientSendAccountRecoveryEmailOperation::Run(ServiceConfig(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_EMAIL_ADDRESS, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkAndroidDeviceID(TestContext& tc)
{
    ClientUnlinkAndroidDeviceIDOperation::RequestType request;
    
    ClientUnlinkAndroidDeviceIDOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkApple(TestContext& tc)
{
    ClientUnlinkAppleOperation::RequestType request;
    
    ClientUnlinkAppleOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_APPLE_NOT_ENABLED_FOR_TITLE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void AccountManagementTests::TestClientUnlinkCustomID(TestContext& tc)
{
    // Covered by TestClientLinkCustomID
    tc.EndTest(S_OK);
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkFacebookAccount(TestContext& tc)
{
    ClientUnlinkFacebookAccountOperation::RequestType request;
    
    ClientUnlinkFacebookAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkFacebookInstantGamesId(TestContext& tc)
{
    ClientUnlinkFacebookInstantGamesIdOperation::RequestType request;
    request.SetFacebookInstantGamesId("testId");
    
    ClientUnlinkFacebookInstantGamesIdOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_FACEBOOK_INSTANT_GAMES_ID_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkGameCenterAccount(TestContext& tc)
{
    ClientUnlinkGameCenterAccountOperation::RequestType request;
    
    ClientUnlinkGameCenterAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkGoogleAccount(TestContext& tc)
{
    ClientUnlinkGoogleAccountOperation::RequestType request;

    ClientUnlinkGoogleAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount(TestContext& tc)
{
    ClientUnlinkGooglePlayGamesServicesAccountOperation::RequestType request;

    ClientUnlinkGooglePlayGamesServicesAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkIOSDeviceID(TestContext& tc)
{
    ClientUnlinkIOSDeviceIDOperation::RequestType request;

    ClientUnlinkIOSDeviceIDOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkKongregate(TestContext& tc)
{
    ClientUnlinkKongregateOperation::RequestType request;

    ClientUnlinkKongregateOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
void AccountManagementTests::TestClientUnlinkNintendoServiceAccount(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    ClientUnlinkNintendoServiceAccountOperation::RequestType request;
    ClientUnlinkNintendoServiceAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // Test not yet implemented on Switch
    tc.Skip();
#endif
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId(TestContext& tc)
{
    ClientUnlinkNintendoSwitchDeviceIdOperation::RequestType request;
    request.SetNintendoSwitchDeviceId("testId");

    ClientUnlinkNintendoSwitchDeviceIdOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void AccountManagementTests::TestClientUnlinkOpenIdConnect(TestContext& tc)
{
    ClientUnlinkOpenIdConnectOperation::RequestType request;
    request.SetConnectionId("testId");

    ClientUnlinkOpenIdConnectOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual((HRESULT)0x801901f4, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkPSNAccount(TestContext& tc)
{
    ClientUnlinkPSNAccountOperation::RequestType request;

    ClientUnlinkPSNAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientUnlinkSteamAccount(TestContext& tc)
{
    ClientUnlinkSteamAccountOperation::RequestType request;

    ClientUnlinkSteamAccountOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUnlinkTwitch(TestContext& tc)
{
    ClientUnlinkTwitchOperation::RequestType request;

    ClientUnlinkTwitchOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void AccountManagementTests::TestClientUnlinkXboxAccount(TestContext& tc)
{
    // Covered by TestClientLinkXboxAccount
    tc.EndTest(S_OK);
}
#endif

void AccountManagementTests::TestClientUpdateAvatarUrl(TestContext& tc)
{
    ClientUpdateAvatarUrlOperation::RequestType request;
    request.SetImageUrl("dummyUrl");
    ClientUpdateAvatarUrlOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestClientUpdateUserTitleDisplayName(TestContext& tc)
{
    ClientUpdateUserTitleDisplayNameOperation::RequestType request;
    request.SetDisplayName("dummyName" + DefaultTitlePlayerId());
    ClientUpdateUserTitleDisplayNameOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientUpdateUserTitleDisplayNameOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual<String>(model.displayName, "dummyName" + DefaultTitlePlayerId(), "displayName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerBanUsers(TestContext& tc)
{
    // Covered by TestServerRevokeBans and TestServerRevokeAllBansForUser
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerDeletePlayer(TestContext& tc)
{
    // Covered by TestClientAddUsernamePassword
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayerCombinedInfo(TestContext& tc)
{
    ServerGetPlayerCombinedInfoOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());
    Wrappers::PFGetPlayerCombinedInfoRequestParamsWrapper<Allocator> requestParams;
    requestParams.SetGetTitleData(true);
    requestParams.SetGetPlayerProfile(true);
    requestParams.SetGetUserData(true);
    requestParams.SetGetUserAccountInfo(true);
    request.SetInfoRequestParameters(requestParams);

    ServerGetPlayerCombinedInfoOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayerCombinedInfoOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual<String>(DefaultTitlePlayerId(), result.Payload().Model().playFabId, "playFabId");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayerProfile(TestContext& tc)
{
    ServerGetPlayerProfileOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerGetPlayerProfileOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayerProfileOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>(DefaultTitlePlayerId(), result.Payload().Model().playerProfile->playerId, "playerProfile->playerId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs(TestContext& tc)
{
    ServerGetPlayFabIDsFromFacebookIDsOperation::RequestType request;
    request.SetFacebookIDs({ "testId" });

    ServerGetPlayFabIDsFromFacebookIDsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromFacebookIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds(TestContext& tc)
{
    ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::RequestType request;
    request.SetFacebookInstantGamesIds({ "testId" });

    ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds(TestContext& tc)
{
    ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::RequestType request;
    request.SetNintendoAccountIds({ "testId" });

    ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds(TestContext& tc)
{
    ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::RequestType request;
    request.SetNintendoSwitchDeviceIds({ "testId" });

    ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs(TestContext& tc)
{
    ServerGetPlayFabIDsFromPSNAccountIDsOperation::RequestType request;
    request.SetPSNAccountIDs({ "testId" });

    ServerGetPlayFabIDsFromPSNAccountIDsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs(TestContext& tc)
{
    ServerGetPlayFabIDsFromSteamIDsOperation::RequestType request;
    request.SetSteamStringIDs({ "76561199383523607" });

    ServerGetPlayFabIDsFromSteamIDsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayFabIDsFromSteamIDsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("161316EBD27587FA", result.Payload().Model().data[0]->playFabId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs(TestContext& tc)
{
    ServerGetPlayFabIDsFromTwitchIDsOperation::RequestType request;
    request.SetTwitchIds({ "testId" });

    ServerGetPlayFabIDsFromTwitchIDsOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<ServerGetPlayFabIDsFromTwitchIDsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs(TestContext& tc)
{
    ServerGetPlayFabIDsFromXboxLiveIDsOperation::RequestType request;
    request.SetSandbox(xboxLiveSandboxId);
    request.SetXboxLiveAccountIDs({ "2814639779201810" });

    ServerGetPlayFabIDsFromXboxLiveIDsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.dataCount, "dataCount");
        tc.AssertEqual<uint32_t>(1, model.dataCount, "model.dataCount");
        tc.AssertEqual<bool>(true, model.data[0]->playFabId != nullptr, "model.data[0]->playFabId null");
        tc.AssertEqual<String>("A13E73660B38980B", model.data[0]->playFabId, "data[0]->playFabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs(TestContext& tc)
{
    constexpr char customId[]{ "testServerGetServerCustomId" };

    ServerLinkServerCustomIdOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetServerCustomId(customId);
    request.SetForceLink(true);

    ServerLinkServerCustomIdOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<ServerGetServerCustomIDsFromPlayFabIDsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetServerCustomIDsFromPlayFabIDsOperation::RequestType request;
        request.SetPlayFabIDs({ DefaultTitlePlayerId() });

        return ServerGetServerCustomIDsFromPlayFabIDsOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, customId](Result<ServerGetServerCustomIDsFromPlayFabIDsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>(customId, result.Payload().Model().data[0]->serverCustomId, "notEqual");

        ServerUnlinkServerCustomIdOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());
        request.SetServerCustomId(customId);

        return ServerUnlinkServerCustomIdOperation::Run(TitleEntity(), request, RunContext());
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
void AccountManagementTests::TestServerGetUserAccountInfo(TestContext& tc)
{
    ServerGetUserAccountInfoOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerGetUserAccountInfoOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetUserAccountInfoOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, result.Payload().Model().userInfo->titleInfo->titlePlayerAccount->id, "userInfo->titleInfo->titlePlayerAccount->id");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerGetUserBans(TestContext& tc)
{
    // Covered by TestServerRevokeBans
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkNintendoServiceAccount(TestContext& tc)
{
    ServerLinkNintendoServiceAccountOperation::RequestType request;
    request.SetIdentityToken("eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiIxMjM0NTY3ODkwIn0.r43DtzFd6FGE_QGYECRIMKXma8YnwAD0WnPdMVwQRTE");
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetForceLink(true);

    ServerLinkNintendoServiceAccountOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_NOT_AUTHORIZED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkNintendoServiceAccountSubject(TestContext& tc)
{
    ServerLinkNintendoServiceAccountSubjectOperation::RequestType request;
    request.SetSubject("testSubject");
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetForceLink(true);

    ServerLinkNintendoServiceAccountSubjectOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerUnlinkNintendoServiceAccountOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUnlinkNintendoServiceAccountOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkNintendoSwitchDeviceId(TestContext& tc)
{
    ServerLinkNintendoSwitchDeviceIdOperation::RequestType request;
    request.SetNintendoSwitchDeviceId("testId");
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetForceLink(true);

    ServerLinkNintendoSwitchDeviceIdOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkPSNAccount(TestContext& tc)
{
    ServerLinkPSNAccountOperation::RequestType request;
    request.SetAuthCode("testId");
    request.SetRedirectUri("dummyUrl");
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetForceLink(true);

    ServerLinkPSNAccountOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_INVALID_PSN_AUTH_CODE, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkServerCustomId(TestContext& tc)
{
    // Covered by TestServerGetServerCustomIDsFromPlayFabIDs
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkSteamId(TestContext& tc)
{
    ServerLinkSteamIdOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetSteamId("123456");
    request.SetForceLink(true);

    ServerLinkSteamIdOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_STEAM_USER_NOT_FOUND, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerLinkXboxAccount(TestContext& tc)
{
    // This test would be doable on Win32 but would need to bring in a XAL dependency to get an Xbox token.
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerRevokeAllBansForUser(TestContext& tc)
{   
    LoginWithCustomIDOperation::RequestType request;
    request.SetCreateAccount(true);
    request.SetCustomId("RevokeAllBansCustomId");

    RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext())).Then([&](Result<LoginResult> result) -> AsyncOp<ServerBanUsersOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerBanUsersOperation::RequestType request;
        Wrappers::PFAccountManagementBanRequestWrapper<Allocator> banRequest;
        banRequest.SetPlayFabId(result.Payload().loginResult.Model().playFabId);
        ModelVector<Wrappers::PFAccountManagementBanRequestWrapper<Allocator>> bans;
        bans.push_back(banRequest);
        request.SetBans(bans);

        return ServerBanUsersOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerBanUsersOperation::ResultType> result) -> AsyncOp<ServerUpdateBansOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        ServerUpdateBansOperation::RequestType request;
        Wrappers::PFAccountManagementUpdateBanRequestWrapper<Allocator> updateRequest;
        updateRequest.SetBanId(result.Payload().Model().banData[0]->banId);
        updateRequest.SetReason("test");
        ModelVector<Wrappers::PFAccountManagementUpdateBanRequestWrapper<Allocator>> updates;
        updates.push_back(updateRequest);
        request.SetBans(updates);

        return ServerUpdateBansOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerUpdateBansOperation::ResultType> result) -> AsyncOp<ServerRevokeAllBansForUserOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        ServerRevokeAllBansForUserOperation::RequestType request;
        request.SetPlayFabId(result.Payload().Model().banData[0]->playFabId);

        return ServerRevokeAllBansForUserOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerRevokeAllBansForUserOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("test", result.Payload().Model().banData[0]->reason, "banReason");
        tc.AssertTrue(!result.Payload().Model().banData[0]->active, "Error: ban is still active");

        ServerDeletePlayerOperation::RequestType request;
        request.SetPlayFabId(result.Payload().Model().banData[0]->playFabId);

        return ServerDeletePlayerOperation::Run(TitleEntity(), request, RunContext());
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
void AccountManagementTests::TestServerRevokeBans(TestContext& tc)
{
    LoginWithCustomIDOperation::RequestType request;
    request.SetCreateAccount(true);
    request.SetCustomId("RevokeBansCustomId");
    char* banId = new char[30u];

    RunOperation(MakeUnique<LoginWithCustomIDOperation>(ServiceConfig(), request, RunContext())).Then([&](Result<LoginResult> result) -> AsyncOp<ServerBanUsersOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerBanUsersOperation::RequestType request;
        Wrappers::PFAccountManagementBanRequestWrapper<Allocator> banRequest;
        banRequest.SetPlayFabId(result.Payload().loginResult.Model().playFabId);
        ModelVector<Wrappers::PFAccountManagementBanRequestWrapper<Allocator>> bans;
        bans.push_back(banRequest);
        request.SetBans(bans);

        return ServerBanUsersOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerBanUsersOperation::ResultType> result) -> AsyncOp<ServerRevokeBansOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().banDataCount, "newBans");
        
        ServerRevokeBansOperation::RequestType request;
        request.SetBanIds({result.Payload().Model().banData[0]->banId});

        return ServerRevokeBansOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, banId](Result<ServerRevokeBansOperation::ResultType> result) -> AsyncOp<ServerGetUserBansOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        ServerGetUserBansOperation::RequestType request;
        request.SetPlayFabId(result.Payload().Model().banData[0]->playFabId);
        strcpy_s(banId, 30u, result.Payload().Model().banData[0]->banId);

        return ServerGetUserBansOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, banId](Result<ServerGetUserBansOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        bool banRevoked = false;
        for (uint32_t i = 0; i < result.Payload().Model().banDataCount; ++i) {
            if (!strcmp(banId, result.Payload().Model().banData[i]->banId)) {
                banRevoked = !result.Payload().Model().banData[i]->active;
                break;
            }
        }
        tc.AssertTrue(banRevoked, "Error: ban is still active");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });

}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerSendCustomAccountRecoveryEmail(TestContext& tc)
{
    // Covered by TestClientAddUsernamePassword
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerSendEmailFromTemplate(TestContext& tc)
{
    // Covered by TestClientAddOrUpdateContactEmail
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkNintendoServiceAccount(TestContext& tc)
{
    // Covered by TestServerLinkNintendoServiceAccountSubject
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId(TestContext& tc)
{
    ServerUnlinkNintendoSwitchDeviceIdOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUnlinkNintendoSwitchDeviceIdOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual((HRESULT)0x801901f4, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkPSNAccount(TestContext& tc)
{
    ServerUnlinkPSNAccountOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUnlinkPSNAccountOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkServerCustomId(TestContext& tc)
{
    // Covered by TestServerGetServerCustomIDsFromPlayFabIDs
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkSteamId(TestContext& tc)
{
    ServerUnlinkSteamIdOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUnlinkSteamIdOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_ACCOUNT_NOT_LINKED, result.hr, "errorName");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUnlinkXboxAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUpdateAvatarUrl(TestContext& tc)
{
    ServerUpdateAvatarUrlOperation::RequestType request;
    request.SetImageUrl("dummyUrl");
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateAvatarUrlOperation::Run(TitleEntity(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestServerUpdateBans(TestContext& tc)
{
    // Covered by TestServerRevokeAllBansForUser
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs(TestContext& tc)
{
    GetTitlePlayersFromXboxLiveIDsOperation::RequestType request;
    request.SetSandbox(xboxLiveSandboxId);
    request.SetXboxLiveIds({ "2814639779201810" });

    GetTitlePlayersFromXboxLiveIDsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetTitlePlayersFromXboxLiveIDsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual<String>("A13E73660B38980B", result.Payload().Model().titlePlayerAccounts->value->masterPlayerAccountId, "playfabId");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

}
}

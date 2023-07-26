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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientAddOrUpdateContactEmail(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientAddUsernamePassword(TestContext& tc)
{
    tc.Skip();
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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH
void AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs(TestContext& tc)
{
    tc.Skip();
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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkAndroidDeviceID(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkApple(TestContext& tc)
{
    tc.Skip();
}
#endif

void AccountManagementTests::TestClientLinkCustomID(TestContext& tc)
{
    static constexpr char defaultUserNewCustomId[]{ "defaultUserNewCustomId" };

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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkFacebookAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkFacebookInstantGamesId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkGameCenterAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkGoogleAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkIOSDeviceID(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkKongregate(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkNintendoServiceAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkNintendoSwitchDeviceId(TestContext& tc)
{
    tc.Skip();
}
#endif

void AccountManagementTests::TestClientLinkOpenIdConnect(TestContext& tc)
{
    tc.Skip();
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkPSNAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH
void AccountManagementTests::TestClientLinkSteamAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientLinkTwitch(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH
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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientRemoveContactEmail(TestContext& tc)
{
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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientSendAccountRecoveryEmail(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkAndroidDeviceID(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkApple(TestContext& tc)
{
    tc.Skip();
}
#endif

void AccountManagementTests::TestClientUnlinkCustomID(TestContext& tc)
{
    // Covered by TestClientLinkCustomID
    tc.EndTest(S_OK);
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkFacebookAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkFacebookInstantGamesId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkGameCenterAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkGoogleAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkIOSDeviceID(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkKongregate(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkNintendoServiceAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId(TestContext& tc)
{
    tc.Skip();
}
#endif

void AccountManagementTests::TestClientUnlinkOpenIdConnect(TestContext& tc)
{
    tc.Skip();
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkPSNAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH
void AccountManagementTests::TestClientUnlinkSteamAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUnlinkTwitch(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH
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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestClientUpdateUserTitleDisplayName(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerBanUsers(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerDeletePlayer(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayerCombinedInfo(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayerProfile(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetUserAccountInfo(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerGetUserBans(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkNintendoServiceAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkNintendoSwitchDeviceId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkPSNAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkServerCustomId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkSteamId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerLinkXboxAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerRevokeAllBansForUser(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerRevokeBans(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerSendCustomAccountRecoveryEmail(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerSendEmailFromTemplate(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkNintendoServiceAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkPSNAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkServerCustomId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkSteamId(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUnlinkXboxAccount(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUpdateAvatarUrl(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestServerUpdateBans(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs(TestContext& tc)
{
    tc.Skip();
}
#endif

}
}

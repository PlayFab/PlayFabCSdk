#include "TestAppPch.h"
#include "ProfilesTests.h"
#include "ProfilesOperations.h"
#include "FriendsOperations.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> ProfilesTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> ProfilesTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void ProfilesTests::TestGetProfile(TestContext& tc)
{
    GetProfileOperation::Run(DefaultTitlePlayer(), {}, RunContext()).Then([&](Result<GetProfileOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, model.profile->entity->id, "profile->entity->id");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });

}

void ProfilesTests::TestGetProfiles(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    // AddFriend -> SetProfilePolicy -> GetProfiles -> Cleanup

    static constexpr char friendCustomId[] = "ProfileCustomId";

    struct TestState
    {
        std::optional<Entity> friendEntity;
        String friendPlayFabId;
    };

    auto testState = MakeShared<TestState>();

    GetTitlePlayer(friendCustomId).Then([&, testState](Result<Entity> result) -> AsyncOp<ClientAddFriendOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        testState->friendEntity.emplace(result.ExtractPayload());

        auto getFriendPlayFabIdResult = GetPlayFabIdFromCustomId(friendCustomId).Wait();
        RETURN_IF_FAILED_PLAYFAB(getFriendPlayFabIdResult);
        testState->friendPlayFabId = getFriendPlayFabIdResult.ExtractPayload();

        // AddFriend
        ClientAddFriendOperation::RequestType request;
        request.SetFriendPlayFabId(testState->friendPlayFabId);
        return ClientAddFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, testState](Result<ClientAddFriendOperation::ResultType> result) -> AsyncOp<ClientAddFriendOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(true, result.Payload().Model().created, "created");

        // AddFriend in the other direction
        ClientAddFriendOperation::RequestType request;
        request.SetFriendPlayFabId(DefaultTitlePlayerId());
        return ClientAddFriendOperation::Run(*testState->friendEntity, request, RunContext());
    })
    .Then([&, testState](Result<ClientAddFriendOperation::ResultType> result) -> AsyncOp<SetProfilePolicyOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.AssertEqual(true, result.Payload().Model().created, "created");

        // Set Profile Policy of friend so that friends can read profile
        SetProfilePolicyOperation::RequestType request;
        request.SetEntity(testState->friendEntity->EntityKey());
        ModelVector<Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator>> statements;
        Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator> statement;
        statement.SetEffect(PFProfilesEffectType::Allow);
        statement.SetAction("Read");
        JsonDocument principalJson{ rapidjson::kObjectType };
        principalJson.AddMember("Friend", "true", principalJson.GetAllocator());
        statement.SetPrincipal(JsonUtils::WriteToString(principalJson));
        Stringstream resource;
        resource << "pfrn:data--*!*/Profile/*";
        statement.SetResource(resource.str());
        statements.push_back(std::move(statement));
        request.SetStatements(std::move(statements));

        return SetProfilePolicyOperation::Run(*testState->friendEntity, request, RunContext());
    })
    .Then([&, testState](Result<SetProfilePolicyOperation::ResultType> result) -> AsyncOp<GetProfilesOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // DefaultTitlePlayer should be able to read the friends profile now
        GetProfilesOperation::RequestType request;
        ModelVector<EntityKey> profilesToGet;
        profilesToGet.push_back(DefaultTitlePlayer().EntityKey());
        profilesToGet.push_back(testState->friendEntity->EntityKey());
        request.SetEntities(std::move(profilesToGet));
        return GetProfilesOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, testState](Result<GetProfilesOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        Map<String, Entity> expectedProfilesRetreived;
        auto defaultTitlePlayer = DefaultTitlePlayer();
        expectedProfilesRetreived.emplace(defaultTitlePlayer.EntityKey().Model().id, defaultTitlePlayer);
        expectedProfilesRetreived.emplace(testState->friendEntity->EntityKey().Model().id, *testState->friendEntity);

        auto& model = result.Payload().Model();
        tc.AssertEqual<size_t>(expectedProfilesRetreived.size(), model.profilesCount, "profilesCount");
        tc.AssertTrue(expectedProfilesRetreived.find(model.profiles[0]->entity->id) != expectedProfilesRetreived.end(), "Unexpected profile returned");
        tc.AssertTrue(expectedProfilesRetreived.find(model.profiles[1]->entity->id) != expectedProfilesRetreived.end(), "Unexpected profile returned");

        return S_OK;
    })
    .Then([&, testState](Result<void> result) -> AsyncOp<SetProfilePolicyOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup by removing friend and resetting profile policy
        SetProfilePolicyOperation::RequestType request;
        request.SetEntity(testState->friendEntity->EntityKey());
        ModelVector<Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator>> statements;
        Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator> statement;
        statement.SetEffect(PFProfilesEffectType::Deny);
        statement.SetAction("Read");
        JsonDocument principalJson{ rapidjson::kObjectType };
        principalJson.AddMember("Friend", "true", principalJson.GetAllocator());
        statement.SetPrincipal(JsonUtils::WriteToString(principalJson));
        Stringstream resource;
        resource << "pfrn:data--*!*/Profile/*";
        statement.SetResource(resource.str());
        statements.push_back(std::move(statement));
        request.SetStatements(std::move(statements));

        return SetProfilePolicyOperation::Run(*testState->friendEntity, request, RunContext());
    })
    .Then([&, testState](Result<SetProfilePolicyOperation::ResultType> result)
    {
        tc.RecordResult(std::move(result));

        ClientRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(testState->friendPlayFabId);
        return ClientRemoveFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, testState](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        ClientRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(DefaultTitlePlayerId());
        return ClientRemoveFriendOperation::Run(*testState->friendEntity, request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // GDK doesn't support GetTitlePlayer
    tc.Skip();
#endif
}

void ProfilesTests::TestGetTitlePlayersFromMasterPlayerAccountIds(TestContext& tc)
{
    GetTitlePlayersFromMasterPlayerAccountIdsOperation::RequestType request;
    request.SetMasterPlayerAccountIds({ DefaultTitlePlayerId() });
    request.SetTitleId(ServiceConfig().TitleId());
    GetTitlePlayersFromMasterPlayerAccountIdsOperation::Run(DefaultTitlePlayer(), request, RunContext())
        .Then([&](Result<GetTitlePlayersFromMasterPlayerAccountIdsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.titlePlayerAccountsCount, "titlePlayerAccountsCount");
        tc.AssertEqual<String>(DefaultTitlePlayerId(), model.titlePlayerAccounts[0].key, "model.titlePlayerAccounts[0].key");
        tc.AssertEqual<String>(DefaultTitlePlayer().EntityKey().Model().id, model.titlePlayerAccounts[0].value->id, "titlePlayerAccounts[0].value->id");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void ProfilesTests::TestSetProfileLanguage(TestContext& tc)
{
    SetProfileLanguageOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetLanguage("en");
    SetProfileLanguageOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<SetProfileLanguageOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void ProfilesTests::TestSetProfilePolicy(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    // Covered by TestGetProfiles
    tc.EndTest(S_OK);
#else
    // GDK doesn't support GetTitlePlayer
    tc.Skip();
#endif
}


}
}

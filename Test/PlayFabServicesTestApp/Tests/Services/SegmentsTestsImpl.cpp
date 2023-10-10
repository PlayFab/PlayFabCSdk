#include "TestAppPch.h"
#include "SegmentsTests.h"
#include "SegmentsOperations.h"
#include "Platform/PlatformUtils.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32
constexpr char testTag[] = { "testTag" };
#endif

AsyncOp<void> SegmentsTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> SegmentsTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void SegmentsTests::TestClientGetPlayerSegments(TestContext& tc)
{
    ClientGetPlayerSegmentsOperation::Run(DefaultTitlePlayer(), RunContext()).Then([&](Result<ClientGetPlayerSegmentsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto model = result.Payload().Model();

        // We expect every player to be at least part of the "All Players" segment. Depending on the player, they may be part of other segments
        // as well.
        tc.AssertTrue(model.segmentsCount >= 1u, "No segments returned");
        bool allPlayersFound = false;
        for (uint32_t i = 0; i < model.segmentsCount; ++i)
        {
            if (!std::strcmp("All Players", model.segments[i]->name))
            {
                allPlayersFound = true;
                break;
            }
        }
        tc.AssertTrue(allPlayersFound, "\"All Players\" segment not found");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void SegmentsTests::TestClientGetPlayerTags(TestContext& tc)
{
    ClientGetPlayerTagsOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());
    
    ClientGetPlayerTagsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientGetPlayerTagsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual<String>(DefaultTitlePlayerId(), model.playFabId, "playFabId");
        tc.AssertEqual(0u, model.tagsCount, "tagsCount");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerAddPlayerTag(TestContext& tc)
{
    ServerAddPlayerTagOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());
    request.SetTagName(testTag);

    ServerAddPlayerTagOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<ServerGetPlayerTagsOperation::ResultType>
    {
        ServerGetPlayerTagsOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());

        // Delay before getting player tags to let them load in fully, otherwise it may return no tags.
        Platform::Sleep(5000);

        return ServerGetPlayerTagsOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetPlayerTagsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual<String>(DefaultTitlePlayerId(), model.playFabId, "playFabId");
        tc.AssertEqual(1u, model.tagsCount, "tagsCount");
        // Tag should be title.titleId.testTag
        tc.AssertEqual<String>("title." + m_testTitleData.titleId + "." + testTag, model.tags[0], "tags");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));

        ServerRemovePlayerTagOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());
        request.SetTagName(testTag);

        return ServerRemovePlayerTagOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetAllSegments(TestContext& tc)
{
    ServerGetAllSegmentsOperation::Run(TitleEntity(), RunContext()).Then([&](Result<ServerGetAllSegmentsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();

        // The "All Players" segment must exist at least.
        tc.AssertTrue(model.segmentsCount >= 1u, "No segments returned");
        bool allPlayersFound = false;
        for (uint32_t i = 0; i < model.segmentsCount; ++i)
        {
            if (!std::strcmp("All Players", model.segments[i]->name))
            {
                allPlayersFound = true;
                break;
            }
        }
        tc.AssertTrue(allPlayersFound, "\"All Players\" segment not found");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayerSegments(TestContext& tc)
{
    ServerGetPlayerSegmentsOperation::RequestType request;
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerGetPlayerSegmentsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayerSegmentsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto& model = result.Payload().Model();

        // We expect every player to be at least part of the "All Players" segment. Depending on the player, they may be part of other segments
        // as well.
        tc.AssertTrue(model.segmentsCount >= 1u, "No segments returned");
        bool allPlayersFound = false;
        for (uint32_t i = 0; i < model.segmentsCount; ++i)
        {
            if (!std::strcmp("All Players", model.segments[i]->name))
            {
                allPlayersFound = true;
                break;
            }
        }
        tc.AssertTrue(allPlayersFound, "\"All Players\" segment not found");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayersInSegment(TestContext& tc)
{
    ServerGetPlayersInSegmentOperation::RequestType request;
    request.SetSegmentId("DC1731C7F6F3D1E4");

    ServerGetPlayersInSegmentOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerGetPlayersInSegmentOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertTrue(model.profilesInSegment > 0, "No players in segment");
        bool defaultTitlePlayerFound = false;
        const char* defaultPlayerId = "D1522C6BB456B845";
        std::vector<const char*> ids;
        for (uint32_t i = 0; i < model.playerProfilesCount; ++i)
        {
            ids.push_back(model.playerProfiles[i]->playerId);
            if (std::strcmp(model.playerProfiles[i]->playerId, defaultPlayerId) == 0)
            {
                defaultTitlePlayerFound = true;
                break;
            }
        }
        tc.AssertTrue(defaultTitlePlayerFound, "DefaultTitlePlayer not found in \"All Players\"");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayerTags(TestContext& tc)
{
    // Already covered in TestServerAddPlayerTag
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void SegmentsTests::TestServerRemovePlayerTag(TestContext& tc)
{
    // Already covered in TestServerAddPlayerTag
    tc.EndTest(S_OK);
}
#endif


}
}

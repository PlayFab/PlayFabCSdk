#include "TestAppPch.h"
#include "SegmentsTests.h"
#include "SegmentsOperations.h"

namespace PlayFab
{
namespace Test
{

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

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerAddPlayerTag(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetAllSegments(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayerSegments(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayersInSegment(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerGetPlayerTags(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void SegmentsTests::TestServerRemovePlayerTag(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

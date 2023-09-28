#include "TestAppPch.h"
#include "SegmentsTests.h"

namespace PlayFab
{
namespace Test
{

void SegmentsTests::AddTests()
{
    AddTest("TestClientGetPlayerSegments", &SegmentsTests::TestClientGetPlayerSegments);
    AddTest("TestClientGetPlayerTags", &SegmentsTests::TestClientGetPlayerTags);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerAddPlayerTag", &SegmentsTests::TestServerAddPlayerTag);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetAllSegments", &SegmentsTests::TestServerGetAllSegments);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayerSegments", &SegmentsTests::TestServerGetPlayerSegments);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayersInSegment", &SegmentsTests::TestServerGetPlayersInSegment);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayerTags", &SegmentsTests::TestServerGetPlayerTags);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerRemovePlayerTag", &SegmentsTests::TestServerRemovePlayerTag);
#endif

}

}
}

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
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerAddPlayerTag", &SegmentsTests::TestServerAddPlayerTag);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetAllSegments", &SegmentsTests::TestServerGetAllSegments);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayerSegments", &SegmentsTests::TestServerGetPlayerSegments);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayersInSegment", &SegmentsTests::TestServerGetPlayersInSegment);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayerTags", &SegmentsTests::TestServerGetPlayerTags);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerRemovePlayerTag", &SegmentsTests::TestServerRemovePlayerTag);
#endif

}

}
}

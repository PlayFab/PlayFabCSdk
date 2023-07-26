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
#if 0
    AddTest("TestServerAddPlayerTag", &SegmentsTests::TestServerAddPlayerTag);
#endif
#if 0
    AddTest("TestServerGetAllSegments", &SegmentsTests::TestServerGetAllSegments);
#endif
#if 0
    AddTest("TestServerGetPlayerSegments", &SegmentsTests::TestServerGetPlayerSegments);
#endif
#if 0
    AddTest("TestServerGetPlayersInSegment", &SegmentsTests::TestServerGetPlayersInSegment);
#endif
#if 0
    AddTest("TestServerGetPlayerTags", &SegmentsTests::TestServerGetPlayerTags);
#endif
#if 0
    AddTest("TestServerRemovePlayerTag", &SegmentsTests::TestServerRemovePlayerTag);
#endif

}

}
}

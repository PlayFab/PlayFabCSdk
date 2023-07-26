#include "TestAppPch.h"
#include "ProfilesTests.h"

namespace PlayFab
{
namespace Test
{

void ProfilesTests::AddTests()
{
    AddTest("TestGetProfile", &ProfilesTests::TestGetProfile);
    AddTest("TestGetProfiles", &ProfilesTests::TestGetProfiles);
    AddTest("TestGetTitlePlayersFromMasterPlayerAccountIds", &ProfilesTests::TestGetTitlePlayersFromMasterPlayerAccountIds);
    AddTest("TestSetProfileLanguage", &ProfilesTests::TestSetProfileLanguage);
    AddTest("TestSetProfilePolicy", &ProfilesTests::TestSetProfilePolicy);

}

}
}

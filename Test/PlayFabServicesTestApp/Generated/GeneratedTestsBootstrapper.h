#pragma once

#include "TestTypes.h"
#include "FriendsTests.h"
#include "AccountManagementTests.h"
#include "PlatformSpecificTests.h"
#include "CloudScriptTests.h"
#include "SegmentsTests.h"
#include "TitleDataManagementTests.h"
#include "PlayerDataManagementTests.h"
#include "PushNotificationsTests.h"
#include "DataTests.h"
#include "InventoryTests.h"
#include "CatalogTests.h"
#include "ExperimentationTests.h"
#include "GroupsTests.h"
#include "LeaderboardsTests.h"
#include "StatisticsTests.h"
#include "LocalizationTests.h"
#include "MultiplayerServerTests.h"
#include "ProfilesTests.h"

namespace PlayFab
{
namespace Test
{

/// <summary>
/// GetGeneratedTestClasses facilitates adding all generated tests to the TestRunner. This pattern allows us to keep generated code 
/// contained in one place and not pollute other files that are changed more frequently.
/// </summary>
List<SharedPtr<TestClass>> GetGeneratedTestClasses(TestTitleData& testTitleData)
{
    List<SharedPtr<TestClass>> generatedTestClasses
    {
        MakeShared<FriendsTests>(testTitleData),
        MakeShared<AccountManagementTests>(testTitleData),
        MakeShared<PlatformSpecificTests>(testTitleData),
        MakeShared<CloudScriptTests>(testTitleData),
        MakeShared<SegmentsTests>(testTitleData),
        MakeShared<TitleDataManagementTests>(testTitleData),
        MakeShared<PlayerDataManagementTests>(testTitleData),
        MakeShared<PushNotificationsTests>(testTitleData),
        MakeShared<DataTests>(testTitleData),
        MakeShared<InventoryTests>(testTitleData),
        MakeShared<CatalogTests>(testTitleData),
        MakeShared<ExperimentationTests>(testTitleData),
        MakeShared<GroupsTests>(testTitleData),
        MakeShared<LeaderboardsTests>(testTitleData),
        MakeShared<StatisticsTests>(testTitleData),
        MakeShared<LocalizationTests>(testTitleData),
        MakeShared<MultiplayerServerTests>(testTitleData),
        MakeShared<ProfilesTests>(testTitleData),
    };

    return generatedTestClasses;
}

}
}

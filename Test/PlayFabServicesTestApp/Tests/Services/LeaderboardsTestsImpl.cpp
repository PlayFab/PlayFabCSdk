#include "TestAppPch.h"
#include "LeaderboardsTests.h"
#include "LeaderboardsOperations.h"
#include "StatisticsOperations.h"
#include "Platform/PlatformUtils.h"

namespace PlayFab
{
namespace Test
{

struct LeaderboardsTestsState
{
    String leaderboardName;
    String existingLeaderboardName;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
AsyncOp<void> CreateStatisticDefinitionFromLeaderboardsTests(Entity entity, RunContext rc, String statisticName)
{
    CreateStatisticDefinitionOperation::RequestType request;
    request.SetName(statisticName);

    Wrappers::PFStatisticsStatisticColumnWrapper<Allocator> column;
    column.SetName("PlayerScoreStatistic");

    ModelVector<Wrappers::PFStatisticsStatisticColumnWrapper<Allocator>> columns;
    columns.push_back(column);

    request.SetColumns(columns);
    request.SetEntityType("title");

    Wrappers::PFVersionConfigurationWrapper<Allocator> versionConfiguration;
    versionConfiguration.SetMaxQueryableVersions(1);
    versionConfiguration.SetResetInterval(PFResetInterval::Manual);

    request.SetVersionConfiguration(versionConfiguration);

    return CreateStatisticDefinitionOperation::Run(entity, request, rc);
}

AsyncOp<void> CreateLeaderboardDefinition(Entity entity, RunContext rc, String leaderboardName, String linkedStatisticName = "", String linkedStatisticColumnName = "")
{
    CreateLeaderboardDefinitionOperation::RequestType request;
    request.SetName(leaderboardName);

    ModelVector<Wrappers::PFLeaderboardsLeaderboardColumnWrapper<Allocator>> columns;
    Wrappers::PFLeaderboardsLeaderboardColumnWrapper<Allocator> column;

    if (linkedStatisticName != "" && linkedStatisticColumnName != "")
    {
        Wrappers::PFLeaderboardsLinkedStatisticColumnWrapper<Allocator> linkedStatisticColumn;
        linkedStatisticColumn.SetLinkedStatisticColumnName(linkedStatisticColumnName);
        linkedStatisticColumn.SetLinkedStatisticName(linkedStatisticName);

        column.SetLinkedStatisticColumn(linkedStatisticColumn);
        column.SetName("PlayerScore");
        column.SetSortDirection(PFLeaderboardsLeaderboardSortDirection::Descending);

        columns.push_back(column);
    }
    else
    {
        column.SetName("PlayerScore");
        column.SetSortDirection(PFLeaderboardsLeaderboardSortDirection::Descending);

        columns.push_back(column);
    }

    request.SetColumns(columns);
    request.SetSizeLimit(100);
    request.SetEntityType("title");

    Wrappers::PFVersionConfigurationWrapper<Allocator> versionConfiguration;
    versionConfiguration.SetMaxQueryableVersions(1);
    versionConfiguration.SetResetInterval(PFResetInterval::Manual);

    request.SetVersionConfiguration(versionConfiguration);

    return CreateLeaderboardDefinitionOperation::Run(entity, request, rc);
}

AsyncOp<void> DeleteLeaderboardDefinition(Entity entity, RunContext rc, String leaderboardName)
{
    DeleteLeaderboardDefinitionOperation::RequestType request;
    request.SetName(leaderboardName);
    return DeleteLeaderboardDefinitionOperation::Run(entity, request, rc);
}
#endif

AsyncOp<void> LeaderboardsTests::Initialize()
{
    m_state = MakeShared<LeaderboardsTestsState>();

    Stringstream leaderboardName;
    leaderboardName << "PFCSDK_Leaderboard_" << time(nullptr);
    m_state->leaderboardName = leaderboardName.str();
    m_state->existingLeaderboardName = "PFCSDK_Leaderboard_DO_NOT_DELETE";

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    return ServicesTestClass::Initialize().Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return CreateLeaderboardDefinition(TitleEntity(), RunContext(), m_state->leaderboardName);
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        UpdateLeaderboardEntriesOperation::RequestType request;
        request.SetLeaderboardName(m_state->leaderboardName);

        Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator> entry;
        entry.SetEntityId(m_testTitleData.titleId);
        entry.SetMetadata("Arbitrary Metadata");
        entry.SetScores({ "100" });

        ModelVector<Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator>> entries;
        entries.push_back(entry);

        request.SetEntries(entries);

        return UpdateLeaderboardEntriesOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([](Result<void> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        Platform::Sleep(5000); // Wait for leaderboard to update
        
        return S_OK;
    });
#else
    return ServicesTestClass::Initialize();
#endif
}

AsyncOp<void> LeaderboardsTests::Uninitialize()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    return DeleteLeaderboardDefinition(TitleEntity(), RunContext(), m_state->leaderboardName).Then([&](Result<void> result) -> AsyncOp<void>
    {
        UNREFERENCED_PARAMETER(result);// Continue with uninitialize regardless of success deleting leaderboard
        m_state.reset();
        return ServicesTestClass::Uninitialize();
    });
#else
    m_state.reset();
    return ServicesTestClass::Uninitialize();
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestCreateLeaderboardDefinition(TestContext& tc)
{
    // Already covered in LeaderboardsTests::Initialize() 
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestDeleteLeaderboardDefinition(TestContext& tc)
{
    // Already covered in LeaderboardsTests::Uninitialize() 
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestDeleteLeaderboardEntries(TestContext& tc)
{
    UpdateLeaderboardEntriesOperation::RequestType request;
    request.SetLeaderboardName(m_state->leaderboardName);

    Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator> entry;
    entry.SetEntityId(m_testTitleData.titleId);
    entry.SetMetadata("Arbitrary Metadata");
    entry.SetScores({ "200" });

    ModelVector<Wrappers::PFLeaderboardsLeaderboardEntryUpdateWrapper<Allocator>> entries;
    entries.push_back(entry);

    request.SetEntries(entries);

    return UpdateLeaderboardEntriesOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result)->AsyncOp<GetLeaderboardOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetLeaderboardOperation::RequestType request;
        request.SetLeaderboardName(m_state->leaderboardName);
        request.SetPageSize(10);

        return GetLeaderboardOperation::Run(TitleEntity(), request, RunContext());
     })
    .Then([&](Result<GetLeaderboardOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // These checks can be a little flaky, as the leaderboard may not have been updated yet.
        // PFLeaderboardsEntityLeaderboardEntry const* const* rankings = result.Payload().Model().rankings;
        // tc.AssertTrue(result.Payload().Model().rankingsCount > 0, "Found unexpected rankings count after inserting entries.");
        // tc.AssertEqual(rankings[0]->scores[0], "200", "Found unexpected score.");

        DeleteLeaderboardEntriesOperation::RequestType request;
        request.SetName(m_state->leaderboardName);
        request.SetEntityIds({ m_testTitleData.titleId });

        return DeleteLeaderboardEntriesOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<GetLeaderboardOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetLeaderboardOperation::RequestType request;
        request.SetLeaderboardName(m_state->leaderboardName);
        request.SetPageSize(10);

        return GetLeaderboardOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<GetLeaderboardOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // This check can be a little flaky, as the leaderboard may not have been updated yet.
        // tc.AssertTrue(result.Payload().Model().rankingsCount == 0, "Found unexpected rankings count after deleting entries.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void LeaderboardsTests::TestGetFriendLeaderboardForEntity(TestContext& tc)
{
    tc.Skip(); // TODO - Service call is returning 500 error code, need to check with service team to see why
#if 0
    GetFriendLeaderboardForEntityOperation::RequestType request;
    request.SetLeaderboardName(m_state->leaderboardName);
    request.SetExternalFriendSources(PFExternalFriendSources::All);

    return GetFriendLeaderboardForEntityOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetFriendLeaderboardForEntityOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int columnsCount = result.Payload().Model().columnsCount;
        tc.AssertTrue(columnsCount > 0, "Found unexpected columns count.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#endif
}

void LeaderboardsTests::TestGetLeaderboard(TestContext& tc)
{
    GetLeaderboardOperation::RequestType request;
    request.SetLeaderboardName(m_state->existingLeaderboardName);
    request.SetPageSize(10);

    return GetLeaderboardOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetLeaderboardOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int columnsCount = result.Payload().Model().columnsCount;
        tc.AssertTrue(columnsCount > 0, "Found unexpected columns count.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void LeaderboardsTests::TestGetLeaderboardAroundEntity(TestContext& tc)
{
    GetLeaderboardAroundEntityOperation::RequestType request;
    request.SetLeaderboardName(m_state->existingLeaderboardName);
    request.SetMaxSurroundingEntries(2);

    return GetLeaderboardAroundEntityOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetLeaderboardAroundEntityOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int columnsCount = result.Payload().Model().columnsCount;
        tc.AssertTrue(columnsCount > 0, "Found unexpected columns count.");
        // TODO - Assertion around rankings

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestGetLeaderboardDefinition(TestContext& tc)
{
    GetLeaderboardDefinitionOperation::RequestType request;
    request.SetName(m_state->leaderboardName);

    return GetLeaderboardDefinitionOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetLeaderboardDefinitionOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(m_state->leaderboardName.c_str(), result.Payload().Model().name, "Found unexpected leaderboard name.");
        int columnsCount = result.Payload().Model().columnsCount;
        tc.AssertTrue(columnsCount > 0, "Found unexpected columns count.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestGetLeaderboardForEntities(TestContext& tc)
{
    tc.Skip(); // TODO - Service call is returning 500 error code, need to check with service team to see why
#if 0
    GetLeaderboardForEntitiesOperation::RequestType request;
    request.SetLeaderboardName(m_state->leaderboardName);
    request.SetEntityIds({ m_testTitleData.titleId });

    return GetLeaderboardForEntitiesOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetLeaderboardForEntitiesOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int columnsCount = result.Payload().Model().columnsCount;
        tc.AssertTrue(columnsCount > 0, "Found unexpected columns count.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#endif
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestIncrementLeaderboardVersion(TestContext& tc)
{
    IncrementLeaderboardVersionOperation::RequestType request;
    request.SetName(m_state->leaderboardName);

    return IncrementLeaderboardVersionOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<IncrementLeaderboardVersionOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int version = result.Payload().Model().version;
        tc.AssertTrue(version > 0, "Found unexpected version on Leaderboard.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestListLeaderboardDefinitions(TestContext& tc)
{
    ListLeaderboardDefinitionsOperation::RequestType request;

    return ListLeaderboardDefinitionsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ListLeaderboardDefinitionsOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int count = result.Payload().Model().leaderboardDefinitionsCount;
        tc.AssertTrue(count > 0, "No leaderboards were found.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestUnlinkLeaderboardFromStatistic(TestContext& tc)
{
    Stringstream leaderboardName;
    leaderboardName << "PFCSDK_Leaderboard_StatisticLinked_" << time(nullptr);
    String leaderboardNameStr = leaderboardName.str();

    Stringstream statisticName;
    statisticName << "PFCSDK_Statistic_LeaderboardLinked_" << time(nullptr);
    String statisticNameStr = statisticName.str();

    CreateStatisticDefinitionOperation::RequestType request;
    request.SetName(statisticNameStr);

    Wrappers::PFStatisticsStatisticColumnWrapper<Allocator> column;
    column.SetName("PlayerScoreStatistic");

    ModelVector<Wrappers::PFStatisticsStatisticColumnWrapper<Allocator>> columns;
    columns.push_back(column);

    request.SetColumns(columns);
    request.SetEntityType("title");

    Wrappers::PFVersionConfigurationWrapper<Allocator> versionConfiguration;
    versionConfiguration.SetMaxQueryableVersions(1);
    versionConfiguration.SetResetInterval(PFResetInterval::Manual);

    request.SetVersionConfiguration(versionConfiguration);

    return CreateStatisticDefinitionOperation::Run(TitleEntity(), request, RunContext()).Then([&, statisticNameStr, leaderboardNameStr](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return CreateLeaderboardDefinition(TitleEntity(), RunContext(), leaderboardNameStr, statisticNameStr, "PlayerScoreStatistic");
    })
    .Then([&, statisticNameStr, leaderboardNameStr](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        UnlinkLeaderboardFromStatisticOperation::RequestType request;
        request.SetName(leaderboardNameStr);
        request.SetStatisticName(statisticNameStr);

        return UnlinkLeaderboardFromStatisticOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, statisticNameStr, leaderboardNameStr](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        tc.RecordResult(std::move(result));

        // Cleanup
        DeleteLeaderboardDefinitionOperation::RequestType request;
        request.SetName(leaderboardNameStr);

        return DeleteLeaderboardDefinitionOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, statisticNameStr](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Cleanup
        DeleteStatisticDefinitionOperation::RequestType request;
        request.SetName(statisticNameStr);

        return DeleteStatisticDefinitionOperation::Run(TitleEntity(), request, RunContext());
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

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void LeaderboardsTests::TestUpdateLeaderboardEntries(TestContext& tc)
{
    // Already covered in TestDeleteLeaderboardEntries
    tc.EndTest(S_OK);
}
#endif


}
}

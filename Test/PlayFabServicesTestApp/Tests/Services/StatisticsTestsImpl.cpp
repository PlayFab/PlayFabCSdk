#include "TestAppPch.h"
#include "StatisticsTests.h"
#include "StatisticsOperations.h"

namespace PlayFab
{
namespace Test
{

struct StatisticsTestsState
{
    String statisticName;
    String existingStatisticName;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
AsyncOp<void> CreateStatisticDefinition(Entity entity, RunContext rc, String statisticName)
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

AsyncOp<void> DeleteStatisticDefinition(Entity entity, RunContext rc, String statisticName)
{
    DeleteStatisticDefinitionOperation::RequestType request;
    request.SetName(statisticName);
    return DeleteStatisticDefinitionOperation::Run(entity, request, rc);
}
#endif

AsyncOp<void> StatisticsTests::Initialize()
{
    m_state = MakeShared<StatisticsTestsState>();

    Stringstream statisticName;
    statisticName << "PFCSDK_Statistic_" << time(nullptr);
    m_state->statisticName = statisticName.str();
    m_state->existingStatisticName = "PFCSDK_Statistic_DO_NOT_DELETE";
    
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    return ServicesTestClass::Initialize().Then([&](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return CreateStatisticDefinition(TitleEntity(), RunContext(), m_state->statisticName);
    })
    .Then([](Result<void> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return S_OK;
    });
#else
    return ServicesTestClass::Initialize();
#endif
}

AsyncOp<void> StatisticsTests::Uninitialize()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    return DeleteStatisticDefinition(TitleEntity(), RunContext(), m_state->statisticName).Then([&](Result<void> result) -> AsyncOp<void>
    {
        UNREFERENCED_PARAMETER(result);// Continue with uninitialize regardless of success deleting statistic
        m_state.reset();
        return ServicesTestClass::Uninitialize();
    });
#else
    m_state.reset();
    return ServicesTestClass::Uninitialize();
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void StatisticsTests::TestCreateStatisticDefinition(TestContext& tc)
{
    // Already covered in StatisticsTests::Initialize() 
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void StatisticsTests::TestDeleteStatisticDefinition(TestContext& tc)
{
    // Already covered in StatisticsTests::Uninitialize() 
    tc.EndTest(S_OK);
}
#endif

void StatisticsTests::TestDeleteStatistics(TestContext& tc)
{
    UpdateStatisticsOperation::RequestType request;

    Wrappers::PFStatisticsStatisticUpdateWrapper<Allocator> statistic;
    statistic.SetMetadata("Arbitrary Metadata");
    statistic.SetName(m_state->existingStatisticName);
    statistic.SetScores({ "100" });

    ModelVector<Wrappers::PFStatisticsStatisticUpdateWrapper<Allocator>> statistics;
    statistics.push_back(statistic);

    request.SetStatistics(statistics);

    return UpdateStatisticsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<UpdateStatisticsOperation::ResultType > result)->AsyncOp<GetStatisticsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetStatisticsOperation::RequestType request;

        return GetStatisticsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetStatisticsOperation::ResultType> result)->AsyncOp<DeleteStatisticsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertTrue(result.Payload().Model().statisticsCount > 0, "Found unexpected statistics count after inserting entries.");

        PFStatisticsEntityStatisticValue const* foundStatistic = {};
        
        auto statisticsCount = result.ExtractPayload().Model().statisticsCount;
        auto statistics = result.ExtractPayload().Model().statistics;

        for (size_t i = 0; i < statisticsCount; i++)
        {
            const char* key = statistics[i].key;
            if (key == m_state->existingStatisticName)
            {
                foundStatistic = statistics[i].value;
                break;
            }
        }

        tc.AssertTrue(foundStatistic != nullptr, "Did not find expected statistic.");

        if (foundStatistic)
        {
            if (foundStatistic->scoresCount > 0)
            {
                tc.AssertEqual(foundStatistic->scores[0], "100", "Found unexpected score.");
            }
        }

        DeleteStatisticsOperation::RequestType request;

        Wrappers::PFStatisticsStatisticDeleteWrapper<Allocator> deleteStatistic;
        deleteStatistic.SetName(m_state->existingStatisticName);

        ModelVector<Wrappers::PFStatisticsStatisticDeleteWrapper<Allocator>> deleteStatistics;
        deleteStatistics.push_back(deleteStatistic);

        request.SetStatistics(deleteStatistics);

        return DeleteStatisticsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<DeleteStatisticsOperation::ResultType> result)->AsyncOp<GetStatisticsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetStatisticsOperation::RequestType request;

        return GetStatisticsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetStatisticsOperation::ResultType> result)->Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        PFStatisticsEntityStatisticValue const* foundStatistic = {};
        
        auto statisticsCount = result.ExtractPayload().Model().statisticsCount;
        auto statistics = result.ExtractPayload().Model().statistics;

        for (size_t i = 0; i < statisticsCount; i++)
        {
            const char* key = statistics[i].key;
            if (key == m_state->existingStatisticName)
            {
                foundStatistic = statistics[i].value;
                break;
            }
        }
        
        tc.AssertTrue(foundStatistic == nullptr, "Found unexpected statistic.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void StatisticsTests::TestGetStatisticDefinition(TestContext& tc)
{
    GetStatisticDefinitionOperation::RequestType request;
    request.SetName(m_state->statisticName);

    return GetStatisticDefinitionOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetStatisticDefinitionOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(m_state->statisticName.c_str(), result.Payload().Model().name, "Found unexpected statistic name.");
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

void StatisticsTests::TestGetStatistics(TestContext& tc)
{
    tc.Skip(); // TODO - Service call is returning 500 error code, need to check with service team to see why
#if 0
    GetStatisticsOperation::RequestType request;

    return GetStatisticsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetStatisticsOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int statisticsCount = result.Payload().Model().statisticsCount;
        tc.AssertTrue(statisticsCount > 0, "Found unexpected statistics count.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void StatisticsTests::TestGetStatisticsForEntities(TestContext& tc)
{
    tc.Skip(); // TODO - Service call is returning 500 error code, need to check with service team to see why
#if 0
    GetStatisticsForEntitiesOperation::RequestType request;

    ModelVector<Wrappers::PFEntityKeyWrapper<Allocator>> entityKeys;
    entityKeys.push_back(TitleEntity().EntityKey());

    request.SetEntities(entityKeys);

    return GetStatisticsForEntitiesOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetStatisticsForEntitiesOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int entitiesStatisticsCount = result.Payload().Model().entitiesStatisticsCount;
        tc.AssertTrue(entitiesStatisticsCount > 0, "Found unexpected entitiesStatisticsCount count.");

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
void StatisticsTests::TestIncrementStatisticVersion(TestContext& tc)
{
    IncrementStatisticVersionOperation::RequestType request;
    request.SetName(m_state->statisticName);

    return IncrementStatisticVersionOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<IncrementStatisticVersionOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int version = result.Payload().Model().version;
        tc.AssertTrue(version > 0, "Found unexpected version on Statistic.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void StatisticsTests::TestListStatisticDefinitions(TestContext& tc)
{
    ListStatisticDefinitionsOperation::RequestType request;

    return ListStatisticDefinitionsOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ListStatisticDefinitionsOperation::ResultType> result)->AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        int count = result.Payload().Model().statisticDefinitionsCount;
        tc.AssertTrue(count > 0, "No statistics were found.");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void StatisticsTests::TestUpdateStatistics(TestContext& tc)
{
    // Already covered in TestDeleteStatistics
    tc.EndTest(S_OK);
}

}
}

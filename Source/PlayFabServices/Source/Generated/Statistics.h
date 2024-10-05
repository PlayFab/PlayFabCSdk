#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "StatisticsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Statistics
{

class StatisticsAPI
{
public:
    StatisticsAPI() = delete;
    StatisticsAPI(const StatisticsAPI& source) = delete;
    StatisticsAPI& operator=(const StatisticsAPI& source) = delete;
    ~StatisticsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> CreateStatisticDefinition(Entity const& entity, const CreateStatisticDefinitionRequest& request, RunContext rc);
    static AsyncOp<void> DeleteStatisticDefinition(Entity const& entity, const DeleteStatisticDefinitionRequest& request, RunContext rc);
    static AsyncOp<DeleteStatisticsResponse> DeleteStatistics(Entity const& entity, const DeleteStatisticsRequest& request, RunContext rc);
    static AsyncOp<GetStatisticDefinitionResponse> GetStatisticDefinition(Entity const& entity, const GetStatisticDefinitionRequest& request, RunContext rc);
    static AsyncOp<GetStatisticsResponse> GetStatistics(Entity const& entity, const GetStatisticsRequest& request, RunContext rc);
    static AsyncOp<GetStatisticsForEntitiesResponse> GetStatisticsForEntities(Entity const& entity, const GetStatisticsForEntitiesRequest& request, RunContext rc);
    static AsyncOp<IncrementStatisticVersionResponse> IncrementStatisticVersion(Entity const& entity, const IncrementStatisticVersionRequest& request, RunContext rc);
    static AsyncOp<ListStatisticDefinitionsResponse> ListStatisticDefinitions(Entity const& entity, const ListStatisticDefinitionsRequest& request, RunContext rc);
    static AsyncOp<UpdateStatisticsResponse> UpdateStatistics(Entity const& entity, const UpdateStatisticsRequest& request, RunContext rc);
};

} // namespace Statistics
} // namespace PlayFab

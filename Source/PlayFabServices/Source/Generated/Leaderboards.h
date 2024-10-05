#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "LeaderboardsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Leaderboards
{

class LeaderboardsAPI
{
public:
    LeaderboardsAPI() = delete;
    LeaderboardsAPI(const LeaderboardsAPI& source) = delete;
    LeaderboardsAPI& operator=(const LeaderboardsAPI& source) = delete;
    ~LeaderboardsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> CreateLeaderboardDefinition(Entity const& entity, const CreateLeaderboardDefinitionRequest& request, RunContext rc);
    static AsyncOp<void> DeleteLeaderboardDefinition(Entity const& entity, const DeleteLeaderboardDefinitionRequest& request, RunContext rc);
    static AsyncOp<void> DeleteLeaderboardEntries(Entity const& entity, const DeleteLeaderboardEntriesRequest& request, RunContext rc);
    static AsyncOp<GetEntityLeaderboardResponse> GetFriendLeaderboardForEntity(Entity const& entity, const GetFriendLeaderboardForEntityRequest& request, RunContext rc);
    static AsyncOp<GetEntityLeaderboardResponse> GetLeaderboard(Entity const& entity, const GetEntityLeaderboardRequest& request, RunContext rc);
    static AsyncOp<GetEntityLeaderboardResponse> GetLeaderboardAroundEntity(Entity const& entity, const GetLeaderboardAroundEntityRequest& request, RunContext rc);
    static AsyncOp<GetLeaderboardDefinitionResponse> GetLeaderboardDefinition(Entity const& entity, const GetLeaderboardDefinitionRequest& request, RunContext rc);
    static AsyncOp<GetEntityLeaderboardResponse> GetLeaderboardForEntities(Entity const& entity, const GetLeaderboardForEntitiesRequest& request, RunContext rc);
    static AsyncOp<IncrementLeaderboardVersionResponse> IncrementLeaderboardVersion(Entity const& entity, const IncrementLeaderboardVersionRequest& request, RunContext rc);
    static AsyncOp<ListLeaderboardDefinitionsResponse> ListLeaderboardDefinitions(Entity const& entity, const ListLeaderboardDefinitionsRequest& request, RunContext rc);
    static AsyncOp<void> UnlinkLeaderboardFromStatistic(Entity const& entity, const UnlinkLeaderboardFromStatisticRequest& request, RunContext rc);
    static AsyncOp<void> UpdateLeaderboardEntries(Entity const& entity, const UpdateLeaderboardEntriesRequest& request, RunContext rc);
};

} // namespace Leaderboards
} // namespace PlayFab

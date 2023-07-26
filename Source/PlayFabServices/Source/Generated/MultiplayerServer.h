#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "MultiplayerServerTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace MultiplayerServer
{

class MultiplayerServerAPI
{
public:
    MultiplayerServerAPI() = delete;
    MultiplayerServerAPI(const MultiplayerServerAPI& source) = delete;
    MultiplayerServerAPI& operator=(const MultiplayerServerAPI& source) = delete;
    ~MultiplayerServerAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<ListBuildAliasesResponse> ListBuildAliases(Entity const& entity, const ListBuildAliasesRequest& request, RunContext rc);
    static AsyncOp<ListBuildSummariesResponse> ListBuildSummariesV2(Entity const& entity, const ListBuildSummariesRequest& request, RunContext rc);
    static AsyncOp<ListQosServersForTitleResponse> ListQosServersForTitle(Entity const& entity, const ListQosServersForTitleRequest& request, RunContext rc);
    static AsyncOp<RequestMultiplayerServerResponse> RequestMultiplayerServer(Entity const& entity, const RequestMultiplayerServerRequest& request, RunContext rc);
};

} // namespace MultiplayerServer
} // namespace PlayFab

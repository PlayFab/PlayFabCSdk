#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "PlayStreamTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace PlayStream
{

class PlayStreamAPI
{
public:
    PlayStreamAPI() = delete;
    PlayStreamAPI(const PlayStreamAPI& source) = delete;
    PlayStreamAPI& operator=(const PlayStreamAPI& source) = delete;
    ~PlayStreamAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<ExportPlayersInSegmentResult> ServerExportPlayersInSegment(Entity const& entity, const ExportPlayersInSegmentRequest& request, RunContext rc);
    static AsyncOp<GetPlayersInSegmentExportResponse> ServerGetSegmentExport(Entity const& entity, const GetPlayersInSegmentExportRequest& request, RunContext rc);
    static AsyncOp<GetSegmentPlayerCountResult> ServerGetSegmentPlayerCount(Entity const& entity, const GetSegmentPlayerCountRequest& request, RunContext rc);
};

} // namespace PlayStream
} // namespace PlayFab

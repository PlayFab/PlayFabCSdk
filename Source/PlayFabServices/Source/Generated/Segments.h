#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "SegmentsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Segments
{

class SegmentsAPI
{
public:
    SegmentsAPI() = delete;
    SegmentsAPI(const SegmentsAPI& source) = delete;
    SegmentsAPI& operator=(const SegmentsAPI& source) = delete;
    ~SegmentsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<GetPlayerSegmentsResult> ClientGetPlayerSegments(Entity const& entity, RunContext rc);
    static AsyncOp<GetPlayerTagsResult> ClientGetPlayerTags(Entity const& entity, const GetPlayerTagsRequest& request, RunContext rc);
    static AsyncOp<void> ServerAddPlayerTag(Entity const& entity, const AddPlayerTagRequest& request, RunContext rc);
    static AsyncOp<GetAllSegmentsResult> ServerGetAllSegments(Entity const& entity, RunContext rc);
    static AsyncOp<GetPlayerSegmentsResult> ServerGetPlayerSegments(Entity const& entity, const GetPlayersSegmentsRequest& request, RunContext rc);
    static AsyncOp<GetPlayersInSegmentResult> ServerGetPlayersInSegment(Entity const& entity, const GetPlayersInSegmentRequest& request, RunContext rc);
    static AsyncOp<GetPlayerTagsResult> ServerGetPlayerTags(Entity const& entity, const GetPlayerTagsRequest& request, RunContext rc);
    static AsyncOp<void> ServerRemovePlayerTag(Entity const& entity, const RemovePlayerTagRequest& request, RunContext rc);
};

} // namespace Segments
} // namespace PlayFab

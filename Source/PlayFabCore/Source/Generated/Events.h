#pragma once

#include "EventsTypes.h"
#include "Common/Entity.h"
#include "Common/PFCoreGlobalState.h"

namespace PlayFab
{
namespace Events
{

class EventsAPI
{
public:
    EventsAPI() = delete;
    EventsAPI(const EventsAPI& source) = delete;
    EventsAPI& operator=(const EventsAPI& source) = delete;
    ~EventsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<DeleteDataConnectionResponse> DeleteDataConnection(SharedPtr<Entity> entity, const DeleteDataConnectionRequest& request, RunContext rc);
    static AsyncOp<GetDataConnectionResponse> GetDataConnection(SharedPtr<Entity> entity, const GetDataConnectionRequest& request, RunContext rc);
    static AsyncOp<ListDataConnectionsResponse> ListDataConnections(SharedPtr<Entity> entity, const ListDataConnectionsRequest& request, RunContext rc);
    static AsyncOp<SetDataConnectionResponse> SetDataConnection(SharedPtr<Entity> entity, const SetDataConnectionRequest& request, RunContext rc);
    static AsyncOp<SetDataConnectionActiveResponse> SetDataConnectionActive(SharedPtr<Entity> entity, const SetDataConnectionActiveRequest& request, RunContext rc);
    static AsyncOp<WriteEventsResponse> WriteEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
    static AsyncOp<WriteEventsResponse> WriteTelemetryEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
};

} // namespace Events
} // namespace PlayFab

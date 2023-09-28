#pragma once

#include "EventsTypes.h"
#include "Common/Entity.h"
#include "Common/GlobalState.h"

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
    static AsyncOp<WriteEventsResponse> WriteEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
    static AsyncOp<WriteEventsResponse> WriteTelemetryEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
};

} // namespace Events
} // namespace PlayFab

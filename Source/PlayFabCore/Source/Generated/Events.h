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
    static AsyncOp<CreateTelemetryKeyResponse> CreateTelemetryKey(SharedPtr<Entity> entity, const CreateTelemetryKeyRequest& request, RunContext rc);
    static AsyncOp<DeleteTelemetryKeyResponse> DeleteTelemetryKey(SharedPtr<Entity> entity, const DeleteTelemetryKeyRequest& request, RunContext rc);
    static AsyncOp<GetTelemetryKeyResponse> GetTelemetryKey(SharedPtr<Entity> entity, const GetTelemetryKeyRequest& request, RunContext rc);
    static AsyncOp<ListTelemetryKeysResponse> ListTelemetryKeys(SharedPtr<Entity> entity, const ListTelemetryKeysRequest& request, RunContext rc);
    static AsyncOp<SetTelemetryKeyActiveResponse> SetTelemetryKeyActive(SharedPtr<Entity> entity, const SetTelemetryKeyActiveRequest& request, RunContext rc);
    static AsyncOp<WriteEventsResponse> WriteEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
    static AsyncOp<WriteEventsResponse> WriteTelemetryEvents(SharedPtr<Entity> entity, const WriteEventsRequest& request, RunContext rc);
};

} // namespace Events
} // namespace PlayFab

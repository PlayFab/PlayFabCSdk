#include "stdafx.h"
#include "Events.h"
#include "PFCoreGlobalState.h"

namespace PlayFab
{
namespace Events
{


AsyncOp<WriteEventsResponse> EventsAPI::WriteEvents(
    SharedPtr<Entity> entity,
    const WriteEventsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/WriteEvents" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsWriteEvents;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            WriteEventsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<WriteEventsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<WriteEventsResponse> EventsAPI::WriteTelemetryEvents(
    SharedPtr<Entity> entity,
    const WriteEventsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/WriteTelemetryEvents" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsWriteTelemetryEvents;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            WriteEventsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<WriteEventsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Events
} // namespace PlayFab

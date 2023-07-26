#include "stdafx.h"
#include "Events.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Events
{


AsyncOp<CreateTelemetryKeyResponse> EventsAPI::CreateTelemetryKey(
    SharedPtr<Entity> entity,
    const CreateTelemetryKeyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/CreateTelemetryKey" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsCreateTelemetryKey;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<CreateTelemetryKeyResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            CreateTelemetryKeyResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<CreateTelemetryKeyResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<DeleteTelemetryKeyResponse> EventsAPI::DeleteTelemetryKey(
    SharedPtr<Entity> entity,
    const DeleteTelemetryKeyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/DeleteTelemetryKey" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsDeleteTelemetryKey;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<DeleteTelemetryKeyResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            DeleteTelemetryKeyResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<DeleteTelemetryKeyResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTelemetryKeyResponse> EventsAPI::GetTelemetryKey(
    SharedPtr<Entity> entity,
    const GetTelemetryKeyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/GetTelemetryKey" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsGetTelemetryKey;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTelemetryKeyResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTelemetryKeyResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTelemetryKeyResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ListTelemetryKeysResponse> EventsAPI::ListTelemetryKeys(
    SharedPtr<Entity> entity,
    const ListTelemetryKeysRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/ListTelemetryKeys" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsListTelemetryKeys;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListTelemetryKeysResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListTelemetryKeysResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListTelemetryKeysResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SetTelemetryKeyActiveResponse> EventsAPI::SetTelemetryKeyActive(
    SharedPtr<Entity> entity,
    const SetTelemetryKeyActiveRequest& request,
    RunContext rc
)
{
    const char* path{ "/Event/SetTelemetryKeyActive" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::EventsSetTelemetryKeyActive;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SetTelemetryKeyActiveResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SetTelemetryKeyActiveResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SetTelemetryKeyActiveResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

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

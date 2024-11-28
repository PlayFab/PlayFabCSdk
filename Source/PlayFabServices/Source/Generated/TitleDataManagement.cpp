#include "stdafx.h"
#include "TitleDataManagement.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace TitleDataManagement
{


AsyncOp<GetPublisherDataResult> TitleDataManagementAPI::ClientGetPublisherData(
    Entity const& entity,
    const GetPublisherDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::TitleDataManagementClientGetPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPublisherDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPublisherDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPublisherDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTimeResult> TitleDataManagementAPI::ClientGetTime(
    Entity const& entity,
    RunContext rc
)
{
    const char* path{ "/Client/GetTime" };
    JsonValue requestBody { JsonValue::object() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::TitleDataManagementClientGetTime,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTimeResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTimeResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTimeResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitleDataResult> TitleDataManagementAPI::ClientGetTitleData(
    Entity const& entity,
    const GetTitleDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetTitleData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::TitleDataManagementClientGetTitleData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitleDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitleDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitleDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitleNewsResult> TitleDataManagementAPI::ClientGetTitleNews(
    Entity const& entity,
    const GetTitleNewsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetTitleNews" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::TitleDataManagementClientGetTitleNews,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitleNewsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitleNewsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitleNewsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPublisherDataResult> TitleDataManagementAPI::ServerGetPublisherData(
    Entity const& entity,
    const GetPublisherDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerGetPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPublisherDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPublisherDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPublisherDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTimeResult> TitleDataManagementAPI::ServerGetTime(
    Entity const& entity,
    RunContext rc
)
{
    const char* path{ "/Server/GetTime" };
    JsonValue requestBody { JsonValue::object() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerGetTime,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTimeResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTimeResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTimeResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitleDataResult> TitleDataManagementAPI::ServerGetTitleData(
    Entity const& entity,
    const GetTitleDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetTitleData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerGetTitleData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitleDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitleDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitleDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitleDataResult> TitleDataManagementAPI::ServerGetTitleInternalData(
    Entity const& entity,
    const GetTitleDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetTitleInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerGetTitleInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitleDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitleDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitleDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitleNewsResult> TitleDataManagementAPI::ServerGetTitleNews(
    Entity const& entity,
    const GetTitleNewsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetTitleNews" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerGetTitleNews,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitleNewsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitleNewsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitleNewsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> TitleDataManagementAPI::ServerSetPublisherData(
    Entity const& entity,
    const SetPublisherDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SetPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerSetPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> TitleDataManagementAPI::ServerSetTitleData(
    Entity const& entity,
    const SetTitleDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SetTitleData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerSetTitleData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> TitleDataManagementAPI::ServerSetTitleInternalData(
    Entity const& entity,
    const SetTitleDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SetTitleInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::TitleDataManagementServerSetTitleInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace TitleDataManagement
} // namespace PlayFab

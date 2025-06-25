#include "stdafx.h"
#include "PlayerDataManagement.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace PlayerDataManagement
{


AsyncOp<ClientDeletePlayerCustomPropertiesResult> PlayerDataManagementAPI::ClientDeletePlayerCustomProperties(
    Entity const& entity,
    const ClientDeletePlayerCustomPropertiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/DeletePlayerCustomProperties" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientDeletePlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientDeletePlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientDeletePlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientDeletePlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetPlayerCustomPropertyResult> PlayerDataManagementAPI::ClientGetPlayerCustomProperty(
    Entity const& entity,
    const ClientGetPlayerCustomPropertyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayerCustomProperty" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientGetPlayerCustomProperty,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetPlayerCustomPropertyResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetPlayerCustomPropertyResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetPlayerCustomPropertyResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetUserDataResult> PlayerDataManagementAPI::ClientGetUserData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetUserData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientGetUserData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetUserDataResult> PlayerDataManagementAPI::ClientGetUserPublisherData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetUserPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientGetUserPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetUserDataResult> PlayerDataManagementAPI::ClientGetUserPublisherReadOnlyData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetUserPublisherReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientGetUserPublisherReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetUserDataResult> PlayerDataManagementAPI::ClientGetUserReadOnlyData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetUserReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientGetUserReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientListPlayerCustomPropertiesResult> PlayerDataManagementAPI::ClientListPlayerCustomProperties(
    Entity const& entity,
    RunContext rc
)
{
    const char* path{ "/Client/ListPlayerCustomProperties" };
    JsonValue requestBody { JsonValue::object() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientListPlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientListPlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientListPlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientListPlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientUpdatePlayerCustomPropertiesResult> PlayerDataManagementAPI::ClientUpdatePlayerCustomProperties(
    Entity const& entity,
    const ClientUpdatePlayerCustomPropertiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdatePlayerCustomProperties" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientUpdatePlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientUpdatePlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientUpdatePlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientUpdatePlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ClientUpdateUserData(
    Entity const& entity,
    const ClientUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdateUserData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientUpdateUserData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ClientUpdateUserPublisherData(
    Entity const& entity,
    const ClientUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdateUserPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlayerDataManagementClientUpdateUserPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerDeletePlayerCustomPropertiesResult> PlayerDataManagementAPI::ServerDeletePlayerCustomProperties(
    Entity const& entity,
    const ServerDeletePlayerCustomPropertiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/DeletePlayerCustomProperties" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerDeletePlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerDeletePlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerDeletePlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerDeletePlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetPlayerCustomPropertyResult> PlayerDataManagementAPI::ServerGetPlayerCustomProperty(
    Entity const& entity,
    const ServerGetPlayerCustomPropertyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayerCustomProperty" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetPlayerCustomProperty,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetPlayerCustomPropertyResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetPlayerCustomPropertyResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetPlayerCustomPropertyResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserInternalData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserPublisherData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserPublisherInternalData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserPublisherInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserPublisherInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserPublisherReadOnlyData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserPublisherReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserPublisherReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetUserDataResult> PlayerDataManagementAPI::ServerGetUserReadOnlyData(
    Entity const& entity,
    const GetUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerGetUserReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerListPlayerCustomPropertiesResult> PlayerDataManagementAPI::ServerListPlayerCustomProperties(
    Entity const& entity,
    const ListPlayerCustomPropertiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/ListPlayerCustomProperties" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerListPlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerListPlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerListPlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerListPlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerUpdatePlayerCustomPropertiesResult> PlayerDataManagementAPI::ServerUpdatePlayerCustomProperties(
    Entity const& entity,
    const ServerUpdatePlayerCustomPropertiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdatePlayerCustomProperties" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdatePlayerCustomProperties,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerUpdatePlayerCustomPropertiesResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerUpdatePlayerCustomPropertiesResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerUpdatePlayerCustomPropertiesResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserData(
    Entity const& entity,
    const ServerUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserInternalData(
    Entity const& entity,
    const UpdateUserInternalDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserPublisherData(
    Entity const& entity,
    const ServerUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserPublisherData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserPublisherData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserPublisherInternalData(
    Entity const& entity,
    const UpdateUserInternalDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserPublisherInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserPublisherInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserPublisherReadOnlyData(
    Entity const& entity,
    const ServerUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserPublisherReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserPublisherReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateUserDataResult> PlayerDataManagementAPI::ServerUpdateUserReadOnlyData(
    Entity const& entity,
    const ServerUpdateUserDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateUserReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayerDataManagementServerUpdateUserReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace PlayerDataManagement
} // namespace PlayFab

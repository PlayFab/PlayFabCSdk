#include "stdafx.h"
#include "Character.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Character
{


AsyncOp<ListUsersCharactersResult> CharacterAPI::ClientGetAllUsersCharacters(
    Entity const& entity,
    const ListUsersCharactersRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetAllUsersCharacters" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CharacterClientGetAllUsersCharacters,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListUsersCharactersResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListUsersCharactersResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListUsersCharactersResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetCharacterDataResult> CharacterAPI::ClientGetCharacterData(
    Entity const& entity,
    const GetCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetCharacterData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CharacterClientGetCharacterData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGetCharacterDataResult> CharacterAPI::ClientGetCharacterReadOnlyData(
    Entity const& entity,
    const GetCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetCharacterReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CharacterClientGetCharacterReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGetCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGetCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGetCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ClientGrantCharacterToUserResult> CharacterAPI::ClientGrantCharacterToUser(
    Entity const& entity,
    const ClientGrantCharacterToUserRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GrantCharacterToUser" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CharacterClientGrantCharacterToUser,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ClientGrantCharacterToUserResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ClientGrantCharacterToUserResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ClientGrantCharacterToUserResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateCharacterDataResult> CharacterAPI::ClientUpdateCharacterData(
    Entity const& entity,
    const ClientUpdateCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdateCharacterData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::CharacterClientUpdateCharacterData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> CharacterAPI::ServerDeleteCharacterFromUser(
    Entity const& entity,
    const DeleteCharacterFromUserRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/DeleteCharacterFromUser" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerDeleteCharacterFromUser,
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

AsyncOp<ListUsersCharactersResult> CharacterAPI::ServerGetAllUsersCharacters(
    Entity const& entity,
    const ListUsersCharactersRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetAllUsersCharacters" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerGetAllUsersCharacters,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListUsersCharactersResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListUsersCharactersResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListUsersCharactersResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetCharacterDataResult> CharacterAPI::ServerGetCharacterData(
    Entity const& entity,
    const GetCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetCharacterData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerGetCharacterData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetCharacterDataResult> CharacterAPI::ServerGetCharacterInternalData(
    Entity const& entity,
    const GetCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetCharacterInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerGetCharacterInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGetCharacterDataResult> CharacterAPI::ServerGetCharacterReadOnlyData(
    Entity const& entity,
    const GetCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetCharacterReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerGetCharacterReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGetCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGetCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGetCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerGrantCharacterToUserResult> CharacterAPI::ServerGrantCharacterToUser(
    Entity const& entity,
    const ServerGrantCharacterToUserRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GrantCharacterToUser" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerGrantCharacterToUser,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ServerGrantCharacterToUserResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ServerGrantCharacterToUserResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ServerGrantCharacterToUserResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateCharacterDataResult> CharacterAPI::ServerUpdateCharacterData(
    Entity const& entity,
    const ServerUpdateCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateCharacterData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerUpdateCharacterData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateCharacterDataResult> CharacterAPI::ServerUpdateCharacterInternalData(
    Entity const& entity,
    const ServerUpdateCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateCharacterInternalData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerUpdateCharacterInternalData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateCharacterDataResult> CharacterAPI::ServerUpdateCharacterReadOnlyData(
    Entity const& entity,
    const ServerUpdateCharacterDataRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateCharacterReadOnlyData" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::CharacterServerUpdateCharacterReadOnlyData,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateCharacterDataResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateCharacterDataResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateCharacterDataResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Character
} // namespace PlayFab

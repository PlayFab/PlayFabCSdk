#include "stdafx.h"
#include "Leaderboards.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Leaderboards
{


AsyncOp<void> LeaderboardsAPI::CreateLeaderboardDefinition(
    Entity const& entity,
    const CreateLeaderboardDefinitionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/CreateLeaderboardDefinition" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsCreateLeaderboardDefinition,
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

AsyncOp<void> LeaderboardsAPI::DeleteLeaderboardDefinition(
    Entity const& entity,
    const DeleteLeaderboardDefinitionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/DeleteLeaderboardDefinition" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsDeleteLeaderboardDefinition,
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

AsyncOp<void> LeaderboardsAPI::DeleteLeaderboardEntries(
    Entity const& entity,
    const DeleteLeaderboardEntriesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/DeleteLeaderboardEntries" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsDeleteLeaderboardEntries,
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

AsyncOp<GetEntityLeaderboardResponse> LeaderboardsAPI::GetFriendLeaderboardForEntity(
    Entity const& entity,
    const GetFriendLeaderboardForEntityRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/GetFriendLeaderboardForEntity" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsGetFriendLeaderboardForEntity,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityLeaderboardResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityLeaderboardResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityLeaderboardResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetEntityLeaderboardResponse> LeaderboardsAPI::GetLeaderboard(
    Entity const& entity,
    const GetEntityLeaderboardRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/GetLeaderboard" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsGetLeaderboard,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityLeaderboardResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityLeaderboardResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityLeaderboardResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetEntityLeaderboardResponse> LeaderboardsAPI::GetLeaderboardAroundEntity(
    Entity const& entity,
    const GetLeaderboardAroundEntityRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/GetLeaderboardAroundEntity" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsGetLeaderboardAroundEntity,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityLeaderboardResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityLeaderboardResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityLeaderboardResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetLeaderboardDefinitionResponse> LeaderboardsAPI::GetLeaderboardDefinition(
    Entity const& entity,
    const GetLeaderboardDefinitionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/GetLeaderboardDefinition" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsGetLeaderboardDefinition,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetLeaderboardDefinitionResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetLeaderboardDefinitionResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetLeaderboardDefinitionResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetEntityLeaderboardResponse> LeaderboardsAPI::GetLeaderboardForEntities(
    Entity const& entity,
    const GetLeaderboardForEntitiesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/GetLeaderboardForEntities" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsGetLeaderboardForEntities,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityLeaderboardResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityLeaderboardResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityLeaderboardResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<IncrementLeaderboardVersionResponse> LeaderboardsAPI::IncrementLeaderboardVersion(
    Entity const& entity,
    const IncrementLeaderboardVersionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/IncrementLeaderboardVersion" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsIncrementLeaderboardVersion,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<IncrementLeaderboardVersionResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            IncrementLeaderboardVersionResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<IncrementLeaderboardVersionResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ListLeaderboardDefinitionsResponse> LeaderboardsAPI::ListLeaderboardDefinitions(
    Entity const& entity,
    const ListLeaderboardDefinitionsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/ListLeaderboardDefinitions" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsListLeaderboardDefinitions,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListLeaderboardDefinitionsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListLeaderboardDefinitionsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ListLeaderboardDefinitionsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> LeaderboardsAPI::UnlinkLeaderboardFromStatistic(
    Entity const& entity,
    const UnlinkLeaderboardFromStatisticRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/UnlinkLeaderboardFromStatistic" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsUnlinkLeaderboardFromStatistic,
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

AsyncOp<void> LeaderboardsAPI::UpdateLeaderboardEntries(
    Entity const& entity,
    const UpdateLeaderboardEntriesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Leaderboard/UpdateLeaderboardEntries" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::LeaderboardsUpdateLeaderboardEntries,
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

} // namespace Leaderboards
} // namespace PlayFab

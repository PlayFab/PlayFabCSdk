#include "stdafx.h"
#include "Segments.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Segments
{


AsyncOp<GetPlayerSegmentsResult> SegmentsAPI::ClientGetPlayerSegments(
    Entity const& entity,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayerSegments" };
    JsonValue requestBody{ rapidjson::kNullType };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::SegmentsClientGetPlayerSegments,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerSegmentsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerSegmentsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerSegmentsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerTagsResult> SegmentsAPI::ClientGetPlayerTags(
    Entity const& entity,
    const GetPlayerTagsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayerTags" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::SegmentsClientGetPlayerTags,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerTagsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerTagsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerTagsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> SegmentsAPI::ServerAddPlayerTag(
    Entity const& entity,
    const AddPlayerTagRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/AddPlayerTag" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerAddPlayerTag,
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

AsyncOp<GetAllSegmentsResult> SegmentsAPI::ServerGetAllSegments(
    Entity const& entity,
    RunContext rc
)
{
    const char* path{ "/Server/GetAllSegments" };
    JsonValue requestBody{ rapidjson::kNullType };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerGetAllSegments,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetAllSegmentsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetAllSegmentsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetAllSegmentsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerSegmentsResult> SegmentsAPI::ServerGetPlayerSegments(
    Entity const& entity,
    const GetPlayersSegmentsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayerSegments" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerGetPlayerSegments,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerSegmentsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerSegmentsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerSegmentsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayersInSegmentResult> SegmentsAPI::ServerGetPlayersInSegment(
    Entity const& entity,
    const GetPlayersInSegmentRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayersInSegment" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerGetPlayersInSegment,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayersInSegmentResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayersInSegmentResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayersInSegmentResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerTagsResult> SegmentsAPI::ServerGetPlayerTags(
    Entity const& entity,
    const GetPlayerTagsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayerTags" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerGetPlayerTags,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerTagsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerTagsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerTagsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> SegmentsAPI::ServerRemovePlayerTag(
    Entity const& entity,
    const RemovePlayerTagRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/RemovePlayerTag" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::SegmentsServerRemovePlayerTag,
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

} // namespace Segments
} // namespace PlayFab

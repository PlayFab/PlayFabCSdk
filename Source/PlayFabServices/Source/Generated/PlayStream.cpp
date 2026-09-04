#include "stdafx.h"
#include "PlayStream.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace PlayStream
{


AsyncOp<ExportPlayersInSegmentResult> PlayStreamAPI::ServerExportPlayersInSegment(
    Entity const& entity,
    const ExportPlayersInSegmentRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/ExportPlayersInSegment" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayStreamServerExportPlayersInSegment,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExportPlayersInSegmentResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExportPlayersInSegmentResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExportPlayersInSegmentResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayersInSegmentExportResponse> PlayStreamAPI::ServerGetSegmentExport(
    Entity const& entity,
    const GetPlayersInSegmentExportRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetSegmentExport" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayStreamServerGetSegmentExport,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayersInSegmentExportResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayersInSegmentExportResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayersInSegmentExportResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetSegmentPlayerCountResult> PlayStreamAPI::ServerGetSegmentPlayerCount(
    Entity const& entity,
    const GetSegmentPlayerCountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetSegmentPlayerCount" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlayStreamServerGetSegmentPlayerCount,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetSegmentPlayerCountResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetSegmentPlayerCountResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetSegmentPlayerCountResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace PlayStream
} // namespace PlayFab

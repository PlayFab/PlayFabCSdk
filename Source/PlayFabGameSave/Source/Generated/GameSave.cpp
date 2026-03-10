#include "stdafx.h"
#include "GameSave.h"
#include "GameSaveGlobalState.h"

using namespace PlayFab::GameSave;

namespace PlayFab
{
namespace GameSaveWrapper
{


AsyncOp<void> GameSaveAPI::DeleteManifest(
    Entity const& entity,
    const DeleteManifestRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/DeleteManifest" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveDeleteManifest,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        if (FAILED(result.hr))
        {
            return Result<void>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return { S_OK, std::move(result.httpResult) };
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<FinalizeManifestResponse> GameSaveAPI::FinalizeManifest(
    Entity const& entity,
    const FinalizeManifestRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/FinalizeManifest" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveFinalizeManifest,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<FinalizeManifestResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<FinalizeManifestResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            FinalizeManifestResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<FinalizeManifestResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<FinalizeManifestResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<GetConfigForTitleResponse> GameSaveAPI::GetConfigForTitle(
    Entity const& entity,
    const GetConfigForTitleRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/GetConfigForTitle" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveGetConfigForTitle,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetConfigForTitleResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<GetConfigForTitleResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetConfigForTitleResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<GetConfigForTitleResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<GetConfigForTitleResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<GetManifestDownloadDetailsResponse> GameSaveAPI::GetManifestDownloadDetails(
    Entity const& entity,
    const GetManifestDownloadDetailsRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/GetManifestDownloadDetails" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveGetManifestDownloadDetails,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetManifestDownloadDetailsResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<GetManifestDownloadDetailsResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetManifestDownloadDetailsResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<GetManifestDownloadDetailsResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<GetManifestDownloadDetailsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<GetQuotaForPlayerResponse> GameSaveAPI::GetQuotaForPlayer(
    Entity const& entity,
    const GetQuotaForPlayerRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/GetQuotaForPlayer" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveGetQuotaForPlayer,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetQuotaForPlayerResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<GetQuotaForPlayerResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetQuotaForPlayerResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<GetQuotaForPlayerResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<GetQuotaForPlayerResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<InitializeManifestResponse> GameSaveAPI::InitializeManifest(
    Entity const& entity,
    const InitializeManifestRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/InitializeManifest" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveInitializeManifest,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<InitializeManifestResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<InitializeManifestResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            InitializeManifestResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<InitializeManifestResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<InitializeManifestResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<InitiateUploadResponse> GameSaveAPI::InitiateUpload(
    Entity const& entity,
    const InitiateUploadRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/InitiateUpload" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveInitiateUpload,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<InitiateUploadResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<InitiateUploadResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            InitiateUploadResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<InitiateUploadResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<InitiateUploadResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<ListManifestsResponse> GameSaveAPI::ListManifests(
    Entity const& entity,
    const ListManifestsRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/ListManifests" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveListManifests,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ListManifestsResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<ListManifestsResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ListManifestsResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<ListManifestsResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<ListManifestsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<void> GameSaveAPI::RequestOnboarding(
    Entity const& entity,
    const RequestOnboardingRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/RequestOnboarding" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveRequestOnboarding,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        if (FAILED(result.hr))
        {
            return Result<void>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return { S_OK, std::move(result.httpResult) };
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<RollbackToManifestResponse> GameSaveAPI::RollbackToManifest(
    Entity const& entity,
    const RollbackToManifestRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/RollbackToManifest" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveRollbackToManifest,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RollbackToManifestResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<RollbackToManifestResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RollbackToManifestResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<RollbackToManifestResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<RollbackToManifestResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<UpdateConfigForTitleResponse> GameSaveAPI::UpdateConfigForTitle(
    Entity const& entity,
    const UpdateConfigForTitleRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/UpdateConfigForTitle" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveUpdateConfigForTitle,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateConfigForTitleResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<UpdateConfigForTitleResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateConfigForTitleResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<UpdateConfigForTitleResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<UpdateConfigForTitleResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

AsyncOp<UpdateManifestResponse> GameSaveAPI::UpdateManifest(
    Entity const& entity,
    const UpdateManifestRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameSave/UpdateManifest" };
    JsonValue requestBody = request.ToJson();

    auto requestOp = GameSaveHttpClient::MakeEntityRequest(
        ServicesCacheId::GameSaveUpdateManifest,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateManifestResponse>
    {
        if (FAILED(result.hr))
        {
            return Result<UpdateManifestResponse>{ result.hr, std::move(result.httpResult) };
        }

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateManifestResponse resultModel;
            HRESULT hr = resultModel.FromJson(serviceResponse.Data);
            if (FAILED(hr))
            {
                return Result<UpdateManifestResponse>{ hr, std::move(result.httpResult) };
            }

            return { std::move(resultModel), std::move(result.httpResult) };
        }
        else
        {
            return Result<UpdateManifestResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage), std::move(result.httpResult) };
        }
    });
}

} // namespace GameSave
} // namespace PlayFab

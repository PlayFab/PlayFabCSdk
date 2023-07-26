#include "stdafx.h"
#include "Profiles.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Profiles
{


AsyncOp<GetEntityProfileResponse> ProfilesAPI::GetProfile(
    Entity const& entity,
    const GetEntityProfileRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/GetProfile" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ProfilesGetProfile,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityProfileResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityProfileResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityProfileResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetEntityProfilesResponse> ProfilesAPI::GetProfiles(
    Entity const& entity,
    const GetEntityProfilesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/GetProfiles" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ProfilesGetProfiles,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetEntityProfilesResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityProfilesResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetEntityProfilesResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitlePlayersFromMasterPlayerAccountIdsResponse> ProfilesAPI::GetTitlePlayersFromMasterPlayerAccountIds(
    Entity const& entity,
    const GetTitlePlayersFromMasterPlayerAccountIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/GetTitlePlayersFromMasterPlayerAccountIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ProfilesGetTitlePlayersFromMasterPlayerAccountIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitlePlayersFromMasterPlayerAccountIdsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitlePlayersFromMasterPlayerAccountIdsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitlePlayersFromMasterPlayerAccountIdsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SetProfileLanguageResponse> ProfilesAPI::SetProfileLanguage(
    Entity const& entity,
    const SetProfileLanguageRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/SetProfileLanguage" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ProfilesSetProfileLanguage,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SetProfileLanguageResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SetProfileLanguageResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SetProfileLanguageResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SetEntityProfilePolicyResponse> ProfilesAPI::SetProfilePolicy(
    Entity const& entity,
    const SetEntityProfilePolicyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/SetProfilePolicy" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::ProfilesSetProfilePolicy,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SetEntityProfilePolicyResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SetEntityProfilePolicyResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SetEntityProfilePolicyResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Profiles
} // namespace PlayFab

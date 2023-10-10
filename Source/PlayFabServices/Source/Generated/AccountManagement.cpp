#include "stdafx.h"
#include "AccountManagement.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace AccountManagement
{


AsyncOp<void> AccountManagementAPI::ClientAddOrUpdateContactEmail(
    Entity const& entity,
    const AddOrUpdateContactEmailRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/AddOrUpdateContactEmail" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientAddOrUpdateContactEmail,
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

AsyncOp<AddUsernamePasswordResult> AccountManagementAPI::ClientAddUsernamePassword(
    Entity const& entity,
    const AddUsernamePasswordRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/AddUsernamePassword" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientAddUsernamePassword,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<AddUsernamePasswordResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            AddUsernamePasswordResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<AddUsernamePasswordResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetAccountInfoResult> AccountManagementAPI::ClientGetAccountInfo(
    Entity const& entity,
    const GetAccountInfoRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetAccountInfo" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetAccountInfo,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetAccountInfoResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetAccountInfoResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetAccountInfoResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerCombinedInfoResult> AccountManagementAPI::ClientGetPlayerCombinedInfo(
    Entity const& entity,
    const GetPlayerCombinedInfoRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayerCombinedInfo" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayerCombinedInfo,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerCombinedInfoResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerCombinedInfoResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerCombinedInfoResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerProfileResult> AccountManagementAPI::ClientGetPlayerProfile(
    Entity const& entity,
    const GetPlayerProfileRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayerProfile" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayerProfile,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerProfileResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerProfileResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerProfileResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromFacebookIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromFacebookIDs(
    Entity const& entity,
    const GetPlayFabIDsFromFacebookIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromFacebookIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromFacebookIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromFacebookIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromFacebookIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromFacebookIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromFacebookInstantGamesIdsResult> AccountManagementAPI::ClientGetPlayFabIDsFromFacebookInstantGamesIds(
    Entity const& entity,
    const GetPlayFabIDsFromFacebookInstantGamesIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromFacebookInstantGamesIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromFacebookInstantGamesIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromFacebookInstantGamesIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromFacebookInstantGamesIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromGameCenterIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromGameCenterIDs(
    Entity const& entity,
    const GetPlayFabIDsFromGameCenterIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromGameCenterIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromGameCenterIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromGameCenterIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromGameCenterIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromGameCenterIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromGoogleIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromGoogleIDs(
    Entity const& entity,
    const GetPlayFabIDsFromGoogleIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromGoogleIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromGoogleIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromGoogleIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromGoogleIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromGoogleIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs(
    Entity const& entity,
    const GetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromGooglePlayGamesPlayerIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromKongregateIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromKongregateIDs(
    Entity const& entity,
    const GetPlayFabIDsFromKongregateIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromKongregateIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromKongregateIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromKongregateIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromKongregateIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromKongregateIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromNintendoServiceAccountIdsResult> AccountManagementAPI::ClientGetPlayFabIDsFromNintendoServiceAccountIds(
    Entity const& entity,
    const GetPlayFabIDsFromNintendoServiceAccountIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromNintendoServiceAccountIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromNintendoServiceAccountIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromNintendoServiceAccountIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromNintendoServiceAccountIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult> AccountManagementAPI::ClientGetPlayFabIDsFromNintendoSwitchDeviceIds(
    Entity const& entity,
    const GetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromNintendoSwitchDeviceIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromNintendoSwitchDeviceIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromPSNAccountIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromPSNAccountIDs(
    Entity const& entity,
    const GetPlayFabIDsFromPSNAccountIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromPSNAccountIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromPSNAccountIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromPSNAccountIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromPSNAccountIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromPSNAccountIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromSteamIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromSteamIDs(
    Entity const& entity,
    const GetPlayFabIDsFromSteamIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromSteamIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromSteamIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromSteamIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromSteamIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromSteamIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromTwitchIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromTwitchIDs(
    Entity const& entity,
    const GetPlayFabIDsFromTwitchIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromTwitchIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromTwitchIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromTwitchIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromTwitchIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromTwitchIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromXboxLiveIDsResult> AccountManagementAPI::ClientGetPlayFabIDsFromXboxLiveIDs(
    Entity const& entity,
    const GetPlayFabIDsFromXboxLiveIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPlayFabIDsFromXboxLiveIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientGetPlayFabIDsFromXboxLiveIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromXboxLiveIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromXboxLiveIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromXboxLiveIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AccountManagementAPI::ClientLinkAndroidDeviceID(
    Entity const& entity,
    const LinkAndroidDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkAndroidDeviceID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkAndroidDeviceID,
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

AsyncOp<void> AccountManagementAPI::ClientLinkApple(
    Entity const& entity,
    const LinkAppleRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkApple" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkApple,
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

AsyncOp<void> AccountManagementAPI::ClientLinkCustomID(
    Entity const& entity,
    const LinkCustomIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkCustomID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkCustomID,
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

AsyncOp<void> AccountManagementAPI::ClientLinkFacebookAccount(
    Entity const& entity,
    const LinkFacebookAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkFacebookAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkFacebookAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkFacebookInstantGamesId(
    Entity const& entity,
    const LinkFacebookInstantGamesIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkFacebookInstantGamesId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkFacebookInstantGamesId,
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

AsyncOp<void> AccountManagementAPI::ClientLinkGameCenterAccount(
    Entity const& entity,
    const LinkGameCenterAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkGameCenterAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkGameCenterAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkGoogleAccount(
    Entity const& entity,
    const LinkGoogleAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkGoogleAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkGoogleAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkGooglePlayGamesServicesAccount(
    Entity const& entity,
    const LinkGooglePlayGamesServicesAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkGooglePlayGamesServicesAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkGooglePlayGamesServicesAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkIOSDeviceID(
    Entity const& entity,
    const LinkIOSDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkIOSDeviceID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkIOSDeviceID,
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

AsyncOp<void> AccountManagementAPI::ClientLinkKongregate(
    Entity const& entity,
    const LinkKongregateAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkKongregate" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkKongregate,
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

AsyncOp<void> AccountManagementAPI::ClientLinkNintendoServiceAccount(
    Entity const& entity,
    const ClientLinkNintendoServiceAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkNintendoServiceAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkNintendoSwitchDeviceId(
    Entity const& entity,
    const ClientLinkNintendoSwitchDeviceIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkNintendoSwitchDeviceId,
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

AsyncOp<void> AccountManagementAPI::ClientLinkOpenIdConnect(
    Entity const& entity,
    const LinkOpenIdConnectRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkOpenIdConnect" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkOpenIdConnect,
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

AsyncOp<void> AccountManagementAPI::ClientLinkPSNAccount(
    Entity const& entity,
    const ClientLinkPSNAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkPSNAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkPSNAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkSteamAccount(
    Entity const& entity,
    const LinkSteamAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkSteamAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkSteamAccount,
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

AsyncOp<void> AccountManagementAPI::ClientLinkTwitch(
    Entity const& entity,
    const LinkTwitchAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkTwitch" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkTwitch,
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

#if HC_PLATFORM != HC_PLATFORM_GDK
AsyncOp<void> AccountManagementAPI::ClientLinkXboxAccount(
    Entity const& entity,
    const ClientLinkXboxAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LinkXboxAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientLinkXboxAccount,
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
#endif

AsyncOp<void> AccountManagementAPI::ClientRemoveContactEmail(
    Entity const& entity,
    const RemoveContactEmailRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/RemoveContactEmail" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientRemoveContactEmail,
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

AsyncOp<ReportPlayerClientResult> AccountManagementAPI::ClientReportPlayer(
    Entity const& entity,
    const ReportPlayerClientRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/ReportPlayer" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientReportPlayer,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ReportPlayerClientResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ReportPlayerClientResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ReportPlayerClientResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AccountManagementAPI::ClientSendAccountRecoveryEmail(
    ServiceConfig const& serviceConfig,
    const SendAccountRecoveryEmailRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/SendAccountRecoveryEmail" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakePostRequest(
        ServicesCacheId::AccountManagementClientSendAccountRecoveryEmail,
        serviceConfig.APIEndpoint(),
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkAndroidDeviceID(
    Entity const& entity,
    const UnlinkAndroidDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkAndroidDeviceID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkAndroidDeviceID,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkApple(
    Entity const& entity,
    const UnlinkAppleRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkApple" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkApple,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkCustomID(
    Entity const& entity,
    const UnlinkCustomIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkCustomID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkCustomID,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkFacebookAccount(
    Entity const& entity,
    const UnlinkFacebookAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkFacebookAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkFacebookAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkFacebookInstantGamesId(
    Entity const& entity,
    const UnlinkFacebookInstantGamesIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkFacebookInstantGamesId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkFacebookInstantGamesId,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkGameCenterAccount(
    Entity const& entity,
    const UnlinkGameCenterAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkGameCenterAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkGameCenterAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkGoogleAccount(
    Entity const& entity,
    const UnlinkGoogleAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkGoogleAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkGoogleAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkGooglePlayGamesServicesAccount(
    Entity const& entity,
    const UnlinkGooglePlayGamesServicesAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkGooglePlayGamesServicesAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkGooglePlayGamesServicesAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkIOSDeviceID(
    Entity const& entity,
    const UnlinkIOSDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkIOSDeviceID" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkIOSDeviceID,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkKongregate(
    Entity const& entity,
    const UnlinkKongregateAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkKongregate" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkKongregate,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkNintendoServiceAccount(
    Entity const& entity,
    const ClientUnlinkNintendoServiceAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkNintendoServiceAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkNintendoSwitchDeviceId(
    Entity const& entity,
    const ClientUnlinkNintendoSwitchDeviceIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkNintendoSwitchDeviceId,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkOpenIdConnect(
    Entity const& entity,
    const UnlinkOpenIdConnectRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkOpenIdConnect" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkOpenIdConnect,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkPSNAccount(
    Entity const& entity,
    const ClientUnlinkPSNAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkPSNAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkPSNAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkSteamAccount(
    Entity const& entity,
    const UnlinkSteamAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkSteamAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkSteamAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkTwitch(
    Entity const& entity,
    const UnlinkTwitchAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkTwitch" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkTwitch,
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

AsyncOp<void> AccountManagementAPI::ClientUnlinkXboxAccount(
    Entity const& entity,
    const ClientUnlinkXboxAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UnlinkXboxAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUnlinkXboxAccount,
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

AsyncOp<void> AccountManagementAPI::ClientUpdateAvatarUrl(
    Entity const& entity,
    const ClientUpdateAvatarUrlRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdateAvatarUrl" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUpdateAvatarUrl,
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

AsyncOp<UpdateUserTitleDisplayNameResult> AccountManagementAPI::ClientUpdateUserTitleDisplayName(
    Entity const& entity,
    const UpdateUserTitleDisplayNameRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/UpdateUserTitleDisplayName" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementClientUpdateUserTitleDisplayName,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateUserTitleDisplayNameResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateUserTitleDisplayNameResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateUserTitleDisplayNameResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<BanUsersResult> AccountManagementAPI::ServerBanUsers(
    Entity const& entity,
    const BanUsersRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/BanUsers" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerBanUsers,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<BanUsersResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            BanUsersResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<BanUsersResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AccountManagementAPI::ServerDeletePlayer(
    Entity const& entity,
    const DeletePlayerRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/DeletePlayer" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerDeletePlayer,
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

AsyncOp<GetPlayerCombinedInfoResult> AccountManagementAPI::ServerGetPlayerCombinedInfo(
    Entity const& entity,
    const GetPlayerCombinedInfoRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayerCombinedInfo" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayerCombinedInfo,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerCombinedInfoResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerCombinedInfoResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerCombinedInfoResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayerProfileResult> AccountManagementAPI::ServerGetPlayerProfile(
    Entity const& entity,
    const GetPlayerProfileRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayerProfile" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayerProfile,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayerProfileResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayerProfileResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayerProfileResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromFacebookIDsResult> AccountManagementAPI::ServerGetPlayFabIDsFromFacebookIDs(
    Entity const& entity,
    const GetPlayFabIDsFromFacebookIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromFacebookIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromFacebookIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromFacebookIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromFacebookIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromFacebookIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromFacebookInstantGamesIdsResult> AccountManagementAPI::ServerGetPlayFabIDsFromFacebookInstantGamesIds(
    Entity const& entity,
    const GetPlayFabIDsFromFacebookInstantGamesIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromFacebookInstantGamesIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromFacebookInstantGamesIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromFacebookInstantGamesIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromFacebookInstantGamesIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromNintendoServiceAccountIdsResult> AccountManagementAPI::ServerGetPlayFabIDsFromNintendoServiceAccountIds(
    Entity const& entity,
    const GetPlayFabIDsFromNintendoServiceAccountIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromNintendoServiceAccountIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromNintendoServiceAccountIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromNintendoServiceAccountIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromNintendoServiceAccountIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult> AccountManagementAPI::ServerGetPlayFabIDsFromNintendoSwitchDeviceIds(
    Entity const& entity,
    const GetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromNintendoSwitchDeviceIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromNintendoSwitchDeviceIdsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromPSNAccountIDsResult> AccountManagementAPI::ServerGetPlayFabIDsFromPSNAccountIDs(
    Entity const& entity,
    const GetPlayFabIDsFromPSNAccountIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromPSNAccountIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromPSNAccountIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromPSNAccountIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromPSNAccountIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromPSNAccountIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromSteamIDsResult> AccountManagementAPI::ServerGetPlayFabIDsFromSteamIDs(
    Entity const& entity,
    const GetPlayFabIDsFromSteamIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromSteamIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromSteamIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromSteamIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromSteamIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromSteamIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromTwitchIDsResult> AccountManagementAPI::ServerGetPlayFabIDsFromTwitchIDs(
    Entity const& entity,
    const GetPlayFabIDsFromTwitchIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromTwitchIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromTwitchIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromTwitchIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromTwitchIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromTwitchIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetPlayFabIDsFromXboxLiveIDsResult> AccountManagementAPI::ServerGetPlayFabIDsFromXboxLiveIDs(
    Entity const& entity,
    const GetPlayFabIDsFromXboxLiveIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetPlayFabIDsFromXboxLiveIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetPlayFabIDsFromXboxLiveIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPlayFabIDsFromXboxLiveIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPlayFabIDsFromXboxLiveIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPlayFabIDsFromXboxLiveIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetServerCustomIDsFromPlayFabIDsResult> AccountManagementAPI::ServerGetServerCustomIDsFromPlayFabIDs(
    Entity const& entity,
    const GetServerCustomIDsFromPlayFabIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetServerCustomIDsFromPlayFabIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetServerCustomIDsFromPlayFabIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetServerCustomIDsFromPlayFabIDsResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetServerCustomIDsFromPlayFabIDsResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetServerCustomIDsFromPlayFabIDsResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetUserAccountInfoResult> AccountManagementAPI::ServerGetUserAccountInfo(
    Entity const& entity,
    const GetUserAccountInfoRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserAccountInfo" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetUserAccountInfo,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetUserAccountInfoResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetUserAccountInfoResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetUserAccountInfoResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetUserBansResult> AccountManagementAPI::ServerGetUserBans(
    Entity const& entity,
    const GetUserBansRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/GetUserBans" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerGetUserBans,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetUserBansResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetUserBansResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetUserBansResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AccountManagementAPI::ServerLinkNintendoServiceAccount(
    Entity const& entity,
    const ServerLinkNintendoServiceAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkNintendoServiceAccount,
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

AsyncOp<void> AccountManagementAPI::ServerLinkNintendoServiceAccountSubject(
    Entity const& entity,
    const LinkNintendoServiceAccountSubjectRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkNintendoServiceAccountSubject" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkNintendoServiceAccountSubject,
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

AsyncOp<void> AccountManagementAPI::ServerLinkNintendoSwitchDeviceId(
    Entity const& entity,
    const ServerLinkNintendoSwitchDeviceIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkNintendoSwitchDeviceId,
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

AsyncOp<void> AccountManagementAPI::ServerLinkPSNAccount(
    Entity const& entity,
    const ServerLinkPSNAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkPSNAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkPSNAccount,
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

AsyncOp<void> AccountManagementAPI::ServerLinkPSNId(
    Entity const& entity,
    const LinkPSNIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkPSNId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkPSNId,
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

AsyncOp<void> AccountManagementAPI::ServerLinkServerCustomId(
    Entity const& entity,
    const LinkServerCustomIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkServerCustomId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkServerCustomId,
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

AsyncOp<void> AccountManagementAPI::ServerLinkSteamId(
    Entity const& entity,
    const LinkSteamIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkSteamId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkSteamId,
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

AsyncOp<void> AccountManagementAPI::ServerLinkXboxAccount(
    Entity const& entity,
    const ServerLinkXboxAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LinkXboxAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerLinkXboxAccount,
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

AsyncOp<RevokeAllBansForUserResult> AccountManagementAPI::ServerRevokeAllBansForUser(
    Entity const& entity,
    const RevokeAllBansForUserRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/RevokeAllBansForUser" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerRevokeAllBansForUser,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RevokeAllBansForUserResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RevokeAllBansForUserResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RevokeAllBansForUserResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RevokeBansResult> AccountManagementAPI::ServerRevokeBans(
    Entity const& entity,
    const RevokeBansRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/RevokeBans" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerRevokeBans,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RevokeBansResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RevokeBansResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RevokeBansResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AccountManagementAPI::ServerSendCustomAccountRecoveryEmail(
    Entity const& entity,
    const SendCustomAccountRecoveryEmailRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SendCustomAccountRecoveryEmail" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerSendCustomAccountRecoveryEmail,
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

AsyncOp<void> AccountManagementAPI::ServerSendEmailFromTemplate(
    Entity const& entity,
    const SendEmailFromTemplateRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SendEmailFromTemplate" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerSendEmailFromTemplate,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkNintendoServiceAccount(
    Entity const& entity,
    const ServerUnlinkNintendoServiceAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkNintendoServiceAccount,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkNintendoSwitchDeviceId(
    Entity const& entity,
    const ServerUnlinkNintendoSwitchDeviceIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkNintendoSwitchDeviceId,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkPSNAccount(
    Entity const& entity,
    const ServerUnlinkPSNAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkPSNAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkPSNAccount,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkServerCustomId(
    Entity const& entity,
    const UnlinkServerCustomIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkServerCustomId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkServerCustomId,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkSteamId(
    Entity const& entity,
    const UnlinkSteamIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkSteamId" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkSteamId,
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

AsyncOp<void> AccountManagementAPI::ServerUnlinkXboxAccount(
    Entity const& entity,
    const ServerUnlinkXboxAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UnlinkXboxAccount" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUnlinkXboxAccount,
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

AsyncOp<void> AccountManagementAPI::ServerUpdateAvatarUrl(
    Entity const& entity,
    const ServerUpdateAvatarUrlRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateAvatarUrl" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUpdateAvatarUrl,
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

AsyncOp<UpdateBansResult> AccountManagementAPI::ServerUpdateBans(
    Entity const& entity,
    const UpdateBansRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/UpdateBans" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::AccountManagementServerUpdateBans,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateBansResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateBansResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateBansResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitlePlayersFromProviderIDsResponse> AccountManagementAPI::GetTitlePlayersFromXboxLiveIDs(
    Entity const& entity,
    const GetTitlePlayersFromXboxLiveIDsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Profile/GetTitlePlayersFromXboxLiveIDs" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::AccountManagementGetTitlePlayersFromXboxLiveIDs,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitlePlayersFromProviderIDsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitlePlayersFromProviderIDsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitlePlayersFromProviderIDsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace AccountManagement
} // namespace PlayFab

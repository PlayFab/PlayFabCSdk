#include "TestAppPch.h"
#include "AccountManagementOperations.h"
#include <playfab/services/PFAccountManagement.h>

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientAddOrUpdateContactEmailOperation::ClientAddOrUpdateContactEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientAddOrUpdateContactEmailOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientAddOrUpdateContactEmailOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientAddOrUpdateContactEmailOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientAddOrUpdateContactEmailAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientAddUsernamePasswordOperation::ClientAddUsernamePasswordOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientAddUsernamePasswordOperation::ResultType> ClientAddUsernamePasswordOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientAddUsernamePasswordOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientAddUsernamePasswordOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientAddUsernamePasswordAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientAddUsernamePasswordOperation::ResultType> ClientAddUsernamePasswordOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientAddUsernamePasswordGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementAddUsernamePasswordResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientAddUsernamePasswordGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


ClientGetAccountInfoOperation::ClientGetAccountInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetAccountInfoOperation::ResultType> ClientGetAccountInfoOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetAccountInfoOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetAccountInfoOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetAccountInfoAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetAccountInfoOperation::ResultType> ClientGetAccountInfoOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetAccountInfoGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetAccountInfoResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetAccountInfoGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetPlayerCombinedInfoOperation::ClientGetPlayerCombinedInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayerCombinedInfoOperation::ResultType> ClientGetPlayerCombinedInfoOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayerCombinedInfoOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayerCombinedInfoOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayerCombinedInfoAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayerCombinedInfoOperation::ResultType> ClientGetPlayerCombinedInfoOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayerCombinedInfoGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayerCombinedInfoResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayerCombinedInfoGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetPlayerProfileOperation::ClientGetPlayerProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayerProfileOperation::ResultType> ClientGetPlayerProfileOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayerProfileOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayerProfileOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayerProfileAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayerProfileOperation::ResultType> ClientGetPlayerProfileOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayerProfileGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayerProfileResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayerProfileGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromFacebookIDsOperation::ClientGetPlayFabIDsFromFacebookIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromFacebookIDsOperation::ResultType> ClientGetPlayFabIDsFromFacebookIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromFacebookIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromFacebookIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromFacebookIDsOperation::ResultType> ClientGetPlayFabIDsFromFacebookIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromFacebookIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> ClientGetPlayFabIDsFromFacebookInstantGamesIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromGameCenterIDsOperation::ClientGetPlayFabIDsFromGameCenterIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromGameCenterIDsOperation::ResultType> ClientGetPlayFabIDsFromGameCenterIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromGameCenterIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromGameCenterIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromGameCenterIDsOperation::ResultType> ClientGetPlayFabIDsFromGameCenterIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromGoogleIDsOperation::ClientGetPlayFabIDsFromGoogleIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromGoogleIDsOperation::ResultType> ClientGetPlayFabIDsFromGoogleIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromGoogleIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromGoogleIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromGoogleIDsOperation::ResultType> ClientGetPlayFabIDsFromGoogleIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromGoogleIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::ResultType> ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::ResultType> ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromKongregateIDsOperation::ClientGetPlayFabIDsFromKongregateIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromKongregateIDsOperation::ResultType> ClientGetPlayFabIDsFromKongregateIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromKongregateIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromKongregateIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromKongregateIDsOperation::ResultType> ClientGetPlayFabIDsFromKongregateIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromKongregateIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH

ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> ClientGetPlayFabIDsFromNintendoServiceAccountIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> ClientGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5

ClientGetPlayFabIDsFromPSNAccountIDsOperation::ClientGetPlayFabIDsFromPSNAccountIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> ClientGetPlayFabIDsFromPSNAccountIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromPSNAccountIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromPSNAccountIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> ClientGetPlayFabIDsFromPSNAccountIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromSteamIDsOperation::ClientGetPlayFabIDsFromSteamIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromSteamIDsOperation::ResultType> ClientGetPlayFabIDsFromSteamIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromSteamIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromSteamIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromSteamIDsOperation::ResultType> ClientGetPlayFabIDsFromSteamIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromSteamIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientGetPlayFabIDsFromTwitchIDsOperation::ClientGetPlayFabIDsFromTwitchIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromTwitchIDsOperation::ResultType> ClientGetPlayFabIDsFromTwitchIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromTwitchIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromTwitchIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromTwitchIDsOperation::ResultType> ClientGetPlayFabIDsFromTwitchIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromTwitchIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

ClientGetPlayFabIDsFromXboxLiveIDsOperation::ClientGetPlayFabIDsFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> ClientGetPlayFabIDsFromXboxLiveIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayFabIDsFromXboxLiveIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayFabIDsFromXboxLiveIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> ClientGetPlayFabIDsFromXboxLiveIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkAndroidDeviceIDOperation::ClientLinkAndroidDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkAndroidDeviceIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkAndroidDeviceIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkAndroidDeviceIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkAndroidDeviceIDAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkAppleOperation::ClientLinkAppleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkAppleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkAppleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkAppleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkAppleAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientLinkCustomIDOperation::ClientLinkCustomIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkCustomIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkCustomIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkCustomIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkCustomIDAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkFacebookAccountOperation::ClientLinkFacebookAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkFacebookAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkFacebookAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkFacebookAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkFacebookAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkFacebookInstantGamesIdOperation::ClientLinkFacebookInstantGamesIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkFacebookInstantGamesIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkFacebookInstantGamesIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkFacebookInstantGamesIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkFacebookInstantGamesIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkGameCenterAccountOperation::ClientLinkGameCenterAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkGameCenterAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkGameCenterAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkGameCenterAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkGameCenterAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkGoogleAccountOperation::ClientLinkGoogleAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkGoogleAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkGoogleAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkGoogleAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkGoogleAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkGooglePlayGamesServicesAccountOperation::ClientLinkGooglePlayGamesServicesAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkGooglePlayGamesServicesAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkGooglePlayGamesServicesAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkGooglePlayGamesServicesAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkIOSDeviceIDOperation::ClientLinkIOSDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkIOSDeviceIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkIOSDeviceIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkIOSDeviceIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkIOSDeviceIDAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkKongregateOperation::ClientLinkKongregateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkKongregateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkKongregateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkKongregateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkKongregateAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH

ClientLinkNintendoServiceAccountOperation::ClientLinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkNintendoServiceAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkNintendoServiceAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkNintendoServiceAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkNintendoServiceAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkNintendoSwitchDeviceIdOperation::ClientLinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkNintendoSwitchDeviceIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkNintendoSwitchDeviceIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkNintendoSwitchDeviceIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientLinkOpenIdConnectOperation::ClientLinkOpenIdConnectOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkOpenIdConnectOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkOpenIdConnectOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkOpenIdConnectOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkOpenIdConnectAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5

ClientLinkPSNAccountOperation::ClientLinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkPSNAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkPSNAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkPSNAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkPSNAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkSteamAccountOperation::ClientLinkSteamAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkSteamAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkSteamAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkSteamAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkSteamAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientLinkTwitchOperation::ClientLinkTwitchOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkTwitchOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkTwitchOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkTwitchOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkTwitchAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

ClientLinkXboxAccountOperation::ClientLinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientLinkXboxAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientLinkXboxAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientLinkXboxAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientLinkXboxAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientRemoveContactEmailOperation::ClientRemoveContactEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientRemoveContactEmailOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientRemoveContactEmailOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientRemoveContactEmailOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientRemoveContactEmailAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientReportPlayerOperation::ClientReportPlayerOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientReportPlayerOperation::ResultType> ClientReportPlayerOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientReportPlayerOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientReportPlayerOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientReportPlayerAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientReportPlayerOperation::ResultType> ClientReportPlayerOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFAccountManagementReportPlayerClientResult result{};
    RETURN_IF_FAILED(PFAccountManagementClientReportPlayerGetResult(async, &result));
    return ResultType{ result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientSendAccountRecoveryEmailOperation::ClientSendAccountRecoveryEmailOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientSendAccountRecoveryEmailOperation::Run(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientSendAccountRecoveryEmailOperation>(std::move(serviceConfig), std::move(request), std::move(rc)));
}

HRESULT ClientSendAccountRecoveryEmailOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientSendAccountRecoveryEmailAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkAndroidDeviceIDOperation::ClientUnlinkAndroidDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkAndroidDeviceIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkAndroidDeviceIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkAndroidDeviceIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkAndroidDeviceIDAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkAppleOperation::ClientUnlinkAppleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkAppleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkAppleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkAppleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkAppleAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientUnlinkCustomIDOperation::ClientUnlinkCustomIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkCustomIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkCustomIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkCustomIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkCustomIDAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkFacebookAccountOperation::ClientUnlinkFacebookAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkFacebookAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkFacebookAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkFacebookAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkFacebookAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkFacebookInstantGamesIdOperation::ClientUnlinkFacebookInstantGamesIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkFacebookInstantGamesIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkFacebookInstantGamesIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkFacebookInstantGamesIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkGameCenterAccountOperation::ClientUnlinkGameCenterAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkGameCenterAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkGameCenterAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkGameCenterAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkGameCenterAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkGoogleAccountOperation::ClientUnlinkGoogleAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkGoogleAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkGoogleAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkGoogleAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkGoogleAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkGooglePlayGamesServicesAccountOperation::ClientUnlinkGooglePlayGamesServicesAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkGooglePlayGamesServicesAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkGooglePlayGamesServicesAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkGooglePlayGamesServicesAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkIOSDeviceIDOperation::ClientUnlinkIOSDeviceIDOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkIOSDeviceIDOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkIOSDeviceIDOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkIOSDeviceIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkIOSDeviceIDAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkKongregateOperation::ClientUnlinkKongregateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkKongregateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkKongregateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkKongregateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkKongregateAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH

ClientUnlinkNintendoServiceAccountOperation::ClientUnlinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkNintendoServiceAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkNintendoServiceAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkNintendoServiceAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkNintendoServiceAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkNintendoSwitchDeviceIdOperation::ClientUnlinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkNintendoSwitchDeviceIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkNintendoSwitchDeviceIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkNintendoSwitchDeviceIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientUnlinkOpenIdConnectOperation::ClientUnlinkOpenIdConnectOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkOpenIdConnectOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkOpenIdConnectOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkOpenIdConnectOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkOpenIdConnectAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5

ClientUnlinkPSNAccountOperation::ClientUnlinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkPSNAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkPSNAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkPSNAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkPSNAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkSteamAccountOperation::ClientUnlinkSteamAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkSteamAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkSteamAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkSteamAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkSteamAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUnlinkTwitchOperation::ClientUnlinkTwitchOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkTwitchOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkTwitchOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkTwitchOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkTwitchAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

ClientUnlinkXboxAccountOperation::ClientUnlinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUnlinkXboxAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUnlinkXboxAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUnlinkXboxAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUnlinkXboxAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


ClientUpdateAvatarUrlOperation::ClientUpdateAvatarUrlOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientUpdateAvatarUrlOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUpdateAvatarUrlOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUpdateAvatarUrlOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUpdateAvatarUrlAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32

ClientUpdateUserTitleDisplayNameOperation::ClientUpdateUserTitleDisplayNameOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientUpdateUserTitleDisplayNameOperation::ResultType> ClientUpdateUserTitleDisplayNameOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUpdateUserTitleDisplayNameOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUpdateUserTitleDisplayNameOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementClientUpdateUserTitleDisplayNameAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientUpdateUserTitleDisplayNameOperation::ResultType> ClientUpdateUserTitleDisplayNameOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementUpdateUserTitleDisplayNameResult* result;
    RETURN_IF_FAILED(PFAccountManagementClientUpdateUserTitleDisplayNameGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerBanUsersOperation::ServerBanUsersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerBanUsersOperation::ResultType> ServerBanUsersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerBanUsersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerBanUsersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerBanUsersAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerBanUsersOperation::ResultType> ServerBanUsersOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerBanUsersGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementBanUsersResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerBanUsersGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerDeletePlayerOperation::ServerDeletePlayerOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerDeletePlayerOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerDeletePlayerOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerDeletePlayerOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerDeletePlayerAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayerCombinedInfoOperation::ServerGetPlayerCombinedInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayerCombinedInfoOperation::ResultType> ServerGetPlayerCombinedInfoOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayerCombinedInfoOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayerCombinedInfoOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayerCombinedInfoAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayerCombinedInfoOperation::ResultType> ServerGetPlayerCombinedInfoOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayerCombinedInfoGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayerCombinedInfoResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayerCombinedInfoGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayerProfileOperation::ServerGetPlayerProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayerProfileOperation::ResultType> ServerGetPlayerProfileOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayerProfileOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayerProfileOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayerProfileAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayerProfileOperation::ResultType> ServerGetPlayerProfileOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayerProfileGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayerProfileResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayerProfileGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromFacebookIDsOperation::ServerGetPlayFabIDsFromFacebookIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromFacebookIDsOperation::ResultType> ServerGetPlayFabIDsFromFacebookIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromFacebookIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromFacebookIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromFacebookIDsOperation::ResultType> ServerGetPlayFabIDsFromFacebookIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromFacebookIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::ResultType> ServerGetPlayFabIDsFromFacebookInstantGamesIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::ResultType> ServerGetPlayFabIDsFromNintendoServiceAccountIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::ResultType> ServerGetPlayFabIDsFromNintendoSwitchDeviceIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromPSNAccountIDsOperation::ServerGetPlayFabIDsFromPSNAccountIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> ServerGetPlayFabIDsFromPSNAccountIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromPSNAccountIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromPSNAccountIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromPSNAccountIDsOperation::ResultType> ServerGetPlayFabIDsFromPSNAccountIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromSteamIDsOperation::ServerGetPlayFabIDsFromSteamIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromSteamIDsOperation::ResultType> ServerGetPlayFabIDsFromSteamIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromSteamIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromSteamIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromSteamIDsOperation::ResultType> ServerGetPlayFabIDsFromSteamIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromSteamIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromTwitchIDsOperation::ServerGetPlayFabIDsFromTwitchIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromTwitchIDsOperation::ResultType> ServerGetPlayFabIDsFromTwitchIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromTwitchIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromTwitchIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromTwitchIDsOperation::ResultType> ServerGetPlayFabIDsFromTwitchIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromTwitchIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetPlayFabIDsFromXboxLiveIDsOperation::ServerGetPlayFabIDsFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> ServerGetPlayFabIDsFromXboxLiveIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayFabIDsFromXboxLiveIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayFabIDsFromXboxLiveIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayFabIDsFromXboxLiveIDsOperation::ResultType> ServerGetPlayFabIDsFromXboxLiveIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetServerCustomIDsFromPlayFabIDsOperation::ServerGetServerCustomIDsFromPlayFabIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetServerCustomIDsFromPlayFabIDsOperation::ResultType> ServerGetServerCustomIDsFromPlayFabIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetServerCustomIDsFromPlayFabIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetServerCustomIDsFromPlayFabIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetServerCustomIDsFromPlayFabIDsOperation::ResultType> ServerGetServerCustomIDsFromPlayFabIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetUserAccountInfoOperation::ServerGetUserAccountInfoOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserAccountInfoOperation::ResultType> ServerGetUserAccountInfoOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserAccountInfoOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserAccountInfoOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetUserAccountInfoAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserAccountInfoOperation::ResultType> ServerGetUserAccountInfoOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetUserAccountInfoGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetUserAccountInfoResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetUserAccountInfoGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetUserBansOperation::ServerGetUserBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserBansOperation::ResultType> ServerGetUserBansOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserBansOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserBansOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerGetUserBansAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserBansOperation::ResultType> ServerGetUserBansOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerGetUserBansGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetUserBansResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerGetUserBansGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkNintendoServiceAccountOperation::ServerLinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkNintendoServiceAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkNintendoServiceAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkNintendoServiceAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkNintendoServiceAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkNintendoSwitchDeviceIdOperation::ServerLinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkNintendoSwitchDeviceIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkNintendoSwitchDeviceIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkNintendoSwitchDeviceIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkPSNAccountOperation::ServerLinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkPSNAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkPSNAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkPSNAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkPSNAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

ServerLinkPSNIdOperation::ServerLinkPSNIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkPSNIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkPSNIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkPSNIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkPSNIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkServerCustomIdOperation::ServerLinkServerCustomIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkServerCustomIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkServerCustomIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkServerCustomIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkServerCustomIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkSteamIdOperation::ServerLinkSteamIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkSteamIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkSteamIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkSteamIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkSteamIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerLinkXboxAccountOperation::ServerLinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerLinkXboxAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerLinkXboxAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerLinkXboxAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerLinkXboxAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerRevokeAllBansForUserOperation::ServerRevokeAllBansForUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerRevokeAllBansForUserOperation::ResultType> ServerRevokeAllBansForUserOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerRevokeAllBansForUserOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerRevokeAllBansForUserOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerRevokeAllBansForUserAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerRevokeAllBansForUserOperation::ResultType> ServerRevokeAllBansForUserOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerRevokeAllBansForUserGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementRevokeAllBansForUserResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerRevokeAllBansForUserGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerRevokeBansOperation::ServerRevokeBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerRevokeBansOperation::ResultType> ServerRevokeBansOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerRevokeBansOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerRevokeBansOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerRevokeBansAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerRevokeBansOperation::ResultType> ServerRevokeBansOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerRevokeBansGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementRevokeBansResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerRevokeBansGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerSendCustomAccountRecoveryEmailOperation::ServerSendCustomAccountRecoveryEmailOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSendCustomAccountRecoveryEmailOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSendCustomAccountRecoveryEmailOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSendCustomAccountRecoveryEmailOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerSendCustomAccountRecoveryEmailAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerSendEmailFromTemplateOperation::ServerSendEmailFromTemplateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSendEmailFromTemplateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSendEmailFromTemplateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSendEmailFromTemplateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerSendEmailFromTemplateAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkNintendoServiceAccountOperation::ServerUnlinkNintendoServiceAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkNintendoServiceAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkNintendoServiceAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkNintendoServiceAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkNintendoServiceAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkNintendoSwitchDeviceIdOperation::ServerUnlinkNintendoSwitchDeviceIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkNintendoSwitchDeviceIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkNintendoSwitchDeviceIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkNintendoSwitchDeviceIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkPSNAccountOperation::ServerUnlinkPSNAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkPSNAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkPSNAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkPSNAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkPSNAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkServerCustomIdOperation::ServerUnlinkServerCustomIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkServerCustomIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkServerCustomIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkServerCustomIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkServerCustomIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkSteamIdOperation::ServerUnlinkSteamIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkSteamIdOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkSteamIdOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkSteamIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkSteamIdAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUnlinkXboxAccountOperation::ServerUnlinkXboxAccountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUnlinkXboxAccountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUnlinkXboxAccountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUnlinkXboxAccountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUnlinkXboxAccountAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUpdateAvatarUrlOperation::ServerUpdateAvatarUrlOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerUpdateAvatarUrlOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateAvatarUrlOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateAvatarUrlOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUpdateAvatarUrlAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerUpdateBansOperation::ServerUpdateBansOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateBansOperation::ResultType> ServerUpdateBansOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateBansOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateBansOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementServerUpdateBansAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateBansOperation::ResultType> ServerUpdateBansOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementServerUpdateBansGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementUpdateBansResult* result;
    RETURN_IF_FAILED(PFAccountManagementServerUpdateBansGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

GetTitlePlayersFromXboxLiveIDsOperation::GetTitlePlayersFromXboxLiveIDsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetTitlePlayersFromXboxLiveIDsOperation::ResultType> GetTitlePlayersFromXboxLiveIDsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetTitlePlayersFromXboxLiveIDsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetTitlePlayersFromXboxLiveIDsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetTitlePlayersFromXboxLiveIDsOperation::ResultType> GetTitlePlayersFromXboxLiveIDsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFAccountManagementGetTitlePlayersFromProviderIDsResponse* result;
    RETURN_IF_FAILED(PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

}
}

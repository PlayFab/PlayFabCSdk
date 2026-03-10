#include "stdafx.h"
#include <playfab/services/PFAccountManagement.h>
#include "Generated/AccountManagement.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::AccountManagement;

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientAddOrUpdateContactEmailAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementAddOrUpdateContactEmailRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientAddOrUpdateContactEmailAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientAddOrUpdateContactEmailAsync),
            std::bind(&AccountManagementAPI::ClientAddOrUpdateContactEmail, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientAddUsernamePasswordAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementAddUsernamePasswordRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientAddUsernamePasswordAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientAddUsernamePasswordAsync),
            std::bind(&AccountManagementAPI::ClientAddUsernamePassword, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientAddUsernamePasswordGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientAddUsernamePasswordGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientAddUsernamePasswordGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementAddUsernamePasswordResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientAddUsernamePasswordGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementAddUsernamePasswordResult*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFAccountManagementClientGetAccountInfoAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetAccountInfoRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetAccountInfoAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetAccountInfoAsync),
            std::bind(&AccountManagementAPI::ClientGetAccountInfo, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetAccountInfoGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetAccountInfoGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetAccountInfoGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetAccountInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetAccountInfoGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetAccountInfoResult*>(buffer);

        return S_OK;
    });
}

PF_API PFAccountManagementClientGetPlayerCombinedInfoAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayerCombinedInfoRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayerCombinedInfoAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayerCombinedInfoAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayerCombinedInfo, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayerCombinedInfoGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayerCombinedInfoGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayerCombinedInfoGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerCombinedInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayerCombinedInfoGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayerCombinedInfoResult*>(buffer);

        return S_OK;
    });
}

PF_API PFAccountManagementClientGetPlayerProfileAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayerProfileRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayerProfileAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayerProfileAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayerProfile, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayerProfileGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayerProfileGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayerProfileGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerProfileResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayerProfileGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayerProfileResult*>(buffer);

        return S_OK;
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromBattleNetAccountIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromFacebookIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromFacebookInstantGamesIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromGameCenterIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromGoogleIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGoogleIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromGoogleIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromKongregateIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromKongregateIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromKongregateIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromNintendoServiceAccountIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromNintendoSwitchDeviceIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromOpenIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromOpenIdSubjectIdentifiers, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromOpenIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromOpenIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromPSNAccountIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromPSNOnlineIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromSteamIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromSteamIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamNamesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromSteamNames, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamNamesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromSteamNamesResult*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromTwitchIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromTwitchIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync),
            std::bind(&AccountManagementAPI::ClientGetPlayFabIDsFromXboxLiveIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkAndroidDeviceIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkAndroidDeviceIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkAndroidDeviceIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkAndroidDeviceIDAsync),
            std::bind(&AccountManagementAPI::ClientLinkAndroidDeviceID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkAppleAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkAppleRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkAppleAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkAppleAsync),
            std::bind(&AccountManagementAPI::ClientLinkApple, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementClientLinkBattleNetAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkBattleNetAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkBattleNetAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkBattleNetAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkBattleNetAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientLinkCustomIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkCustomIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkCustomIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkCustomIDAsync),
            std::bind(&AccountManagementAPI::ClientLinkCustomID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkFacebookAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkFacebookAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkFacebookAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkFacebookAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkFacebookAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkFacebookInstantGamesIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkFacebookInstantGamesIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkFacebookInstantGamesIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkFacebookInstantGamesIdAsync),
            std::bind(&AccountManagementAPI::ClientLinkFacebookInstantGamesId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkGameCenterAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkGameCenterAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkGameCenterAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkGameCenterAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkGameCenterAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkGoogleAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkGoogleAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkGoogleAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkGoogleAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkGoogleAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkGooglePlayGamesServicesAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkGooglePlayGamesServicesAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkIOSDeviceIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkIOSDeviceIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkIOSDeviceIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkIOSDeviceIDAsync),
            std::bind(&AccountManagementAPI::ClientLinkIOSDeviceID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkKongregateAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkKongregateAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkKongregateAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkKongregateAsync),
            std::bind(&AccountManagementAPI::ClientLinkKongregate, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkNintendoServiceAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkNintendoServiceAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkNintendoServiceAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkNintendoServiceAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync),
            std::bind(&AccountManagementAPI::ClientLinkNintendoSwitchDeviceId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientLinkOpenIdConnectAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkOpenIdConnectRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkOpenIdConnectAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkOpenIdConnectAsync),
            std::bind(&AccountManagementAPI::ClientLinkOpenIdConnect, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkPSNAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkPSNAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkPSNAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkPSNAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkPSNAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkSteamAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkSteamAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkSteamAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkSteamAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkSteamAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientLinkTwitchAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkTwitchAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkTwitchAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkTwitchAsync),
            std::bind(&AccountManagementAPI::ClientLinkTwitch, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientLinkXboxAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientLinkXboxAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientLinkXboxAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientLinkXboxAccountAsync),
            std::bind(&AccountManagementAPI::ClientLinkXboxAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientRemoveContactEmailAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementRemoveContactEmailRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientRemoveContactEmailAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientRemoveContactEmailAsync),
            std::bind(&AccountManagementAPI::ClientRemoveContactEmail, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientReportPlayerAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementReportPlayerClientRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientReportPlayerAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientReportPlayerAsync),
            std::bind(&AccountManagementAPI::ClientReportPlayer, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientReportPlayerGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFAccountManagementReportPlayerClientResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientReportPlayerGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFAccountManagementReportPlayerClientResult), result, nullptr);
    });
}

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientSendAccountRecoveryEmailAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAccountManagementSendAccountRecoveryEmailRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientSendAccountRecoveryEmailAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientSendAccountRecoveryEmailAsync),
            std::bind(&AccountManagementAPI::ClientSendAccountRecoveryEmail, ServiceConfig::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkAndroidDeviceIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkAndroidDeviceIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkAndroidDeviceIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkAndroidDeviceIDAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkAndroidDeviceID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkAppleAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkAppleRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkAppleAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkAppleAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkApple, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementClientUnlinkBattleNetAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkBattleNetAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkBattleNetAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkBattleNetAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkBattleNetAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientUnlinkCustomIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkCustomIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkCustomIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkCustomIDAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkCustomID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkFacebookAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkFacebookAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkFacebookAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkFacebookAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkFacebookAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkFacebookInstantGamesIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkFacebookInstantGamesId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkGameCenterAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkGameCenterAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkGameCenterAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkGameCenterAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkGameCenterAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkGoogleAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkGoogleAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkGoogleAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkGoogleAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkGoogleAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkGooglePlayGamesServicesAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkIOSDeviceIDAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkIOSDeviceIDRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkIOSDeviceIDAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkIOSDeviceIDAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkIOSDeviceID, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkKongregateAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkKongregateAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkKongregateAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkKongregateAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkKongregate, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkNintendoServiceAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkNintendoServiceAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkNintendoServiceAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkNintendoServiceAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkNintendoSwitchDeviceId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientUnlinkOpenIdConnectAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkOpenIdConnectRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkOpenIdConnectAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkOpenIdConnectAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkOpenIdConnect, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkPSNAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkPSNAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkPSNAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkPSNAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkPSNAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkSteamAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkSteamAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkSteamAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkSteamAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkSteamAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUnlinkTwitchAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkTwitchAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkTwitchAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkTwitchAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkTwitch, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAccountManagementClientUnlinkXboxAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUnlinkXboxAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUnlinkXboxAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUnlinkXboxAccountAsync),
            std::bind(&AccountManagementAPI::ClientUnlinkXboxAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientUpdateAvatarUrlAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementClientUpdateAvatarUrlRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUpdateAvatarUrlAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUpdateAvatarUrlAsync),
            std::bind(&AccountManagementAPI::ClientUpdateAvatarUrl, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementClientUpdateUserTitleDisplayNameAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUpdateUserTitleDisplayNameRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementClientUpdateUserTitleDisplayNameAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementClientUpdateUserTitleDisplayNameAsync),
            std::bind(&AccountManagementAPI::ClientUpdateUserTitleDisplayName, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementClientUpdateUserTitleDisplayNameGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementUpdateUserTitleDisplayNameResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementClientUpdateUserTitleDisplayNameGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementUpdateUserTitleDisplayNameResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerBanUsersAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementBanUsersRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerBanUsersAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerBanUsersAsync),
            std::bind(&AccountManagementAPI::ServerBanUsers, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerBanUsersGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerBanUsersGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerBanUsersGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementBanUsersResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerBanUsersGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementBanUsersResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerDeletePlayerAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementDeletePlayerRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerDeletePlayerAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerDeletePlayerAsync),
            std::bind(&AccountManagementAPI::ServerDeletePlayer, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayerCombinedInfoAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayerCombinedInfoRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayerCombinedInfoAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayerCombinedInfoAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayerCombinedInfo, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayerCombinedInfoGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayerCombinedInfoGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayerCombinedInfoGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerCombinedInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayerCombinedInfoGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayerCombinedInfoResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayerProfileAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayerProfileRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayerProfileAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayerProfileAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayerProfile, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayerProfileGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayerProfileGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayerProfileGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerProfileResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayerProfileGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayerProfileResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromBattleNetAccountIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromFacebookIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromFacebookInstantGamesIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromNintendoServiceAccountIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromNintendoSwitchDeviceIds, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromOpenIdsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromOpenIdSubjectIdentifiers, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromOpenIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromOpenIdsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromPSNAccountIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromPSNOnlineIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromSteamIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromSteamIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamNamesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromSteamNames, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamNamesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromSteamNamesResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromTwitchIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromTwitchIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetPlayFabIDsFromXboxLiveIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync),
            std::bind(&AccountManagementAPI::ServerGetServerCustomIDsFromPlayFabIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetUserAccountInfoAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetUserAccountInfoRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetUserAccountInfoAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetUserAccountInfoAsync),
            std::bind(&AccountManagementAPI::ServerGetUserAccountInfo, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetUserAccountInfoGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetUserAccountInfoGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetUserAccountInfoGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetUserAccountInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetUserAccountInfoGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetUserAccountInfoResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerGetUserBansAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetUserBansRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerGetUserBansAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerGetUserBansAsync),
            std::bind(&AccountManagementAPI::ServerGetUserBans, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerGetUserBansGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetUserBansGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerGetUserBansGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetUserBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerGetUserBansGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetUserBansResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerLinkBattleNetAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkBattleNetAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkBattleNetAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkBattleNetAccountAsync),
            std::bind(&AccountManagementAPI::ServerLinkBattleNetAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkNintendoServiceAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoServiceAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoServiceAccountAsync),
            std::bind(&AccountManagementAPI::ServerLinkNintendoServiceAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkNintendoServiceAccountSubjectRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync),
            std::bind(&AccountManagementAPI::ServerLinkNintendoServiceAccountSubject, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync),
            std::bind(&AccountManagementAPI::ServerLinkNintendoSwitchDeviceId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkPSNAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkPSNAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkPSNAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkPSNAccountAsync),
            std::bind(&AccountManagementAPI::ServerLinkPSNAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerLinkPSNIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkPSNIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkPSNIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkPSNIdAsync),
            std::bind(&AccountManagementAPI::ServerLinkPSNId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkServerCustomIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkServerCustomIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkServerCustomIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkServerCustomIdAsync),
            std::bind(&AccountManagementAPI::ServerLinkServerCustomId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkSteamIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkSteamIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkSteamIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkSteamIdAsync),
            std::bind(&AccountManagementAPI::ServerLinkSteamId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAccountManagementServerLinkTwitchAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkTwitchAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkTwitchAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkTwitchAccountAsync),
            std::bind(&AccountManagementAPI::ServerLinkTwitchAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerLinkXboxAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerLinkXboxAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkXboxAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkXboxAccountAsync),
            std::bind(&AccountManagementAPI::ServerLinkXboxAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAccountManagementServerLinkXboxIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementLinkXboxIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerLinkXboxIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerLinkXboxIdAsync),
            std::bind(&AccountManagementAPI::ServerLinkXboxId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerRevokeAllBansForUserAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementRevokeAllBansForUserRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerRevokeAllBansForUserAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerRevokeAllBansForUserAsync),
            std::bind(&AccountManagementAPI::ServerRevokeAllBansForUser, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerRevokeAllBansForUserGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerRevokeAllBansForUserGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerRevokeAllBansForUserGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementRevokeAllBansForUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerRevokeAllBansForUserGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementRevokeAllBansForUserResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerRevokeBansAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementRevokeBansRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerRevokeBansAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerRevokeBansAsync),
            std::bind(&AccountManagementAPI::ServerRevokeBans, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerRevokeBansGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerRevokeBansGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerRevokeBansGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementRevokeBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerRevokeBansGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementRevokeBansResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerSendCustomAccountRecoveryEmailAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementSendCustomAccountRecoveryEmailRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerSendCustomAccountRecoveryEmailAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerSendCustomAccountRecoveryEmailAsync),
            std::bind(&AccountManagementAPI::ServerSendCustomAccountRecoveryEmail, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerSendEmailFromTemplateAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementSendEmailFromTemplateRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerSendEmailFromTemplateAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerSendEmailFromTemplateAsync),
            std::bind(&AccountManagementAPI::ServerSendEmailFromTemplate, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAccountManagementServerUnlinkBattleNetAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkBattleNetAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkBattleNetAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkBattleNetAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkBattleNetAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAccountManagementServerUnlinkFacebookAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkFacebookAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkFacebookAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkFacebookAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkFacebookAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAccountManagementServerUnlinkFacebookInstantGamesIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkFacebookInstantGamesIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkFacebookInstantGamesIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkFacebookInstantGamesIdAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkFacebookInstantGamesId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkNintendoServiceAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkNintendoServiceAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkNintendoServiceAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkNintendoServiceAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkNintendoSwitchDeviceId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkPSNAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkPSNAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkPSNAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkPSNAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkPSNAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkServerCustomIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkServerCustomIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkServerCustomIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkServerCustomIdAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkServerCustomId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkSteamIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUnlinkSteamIdRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkSteamIdAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkSteamIdAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkSteamId, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAccountManagementServerUnlinkTwitchAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkTwitchAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkTwitchAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkTwitchAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkTwitchAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUnlinkXboxAccountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUnlinkXboxAccountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUnlinkXboxAccountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUnlinkXboxAccountAsync),
            std::bind(&AccountManagementAPI::ServerUnlinkXboxAccount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUpdateAvatarUrlAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementServerUpdateAvatarUrlRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUpdateAvatarUrlAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUpdateAvatarUrlAsync),
            std::bind(&AccountManagementAPI::ServerUpdateAvatarUrl, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAccountManagementServerUpdateBansAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementUpdateBansRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementServerUpdateBansAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementServerUpdateBansAsync),
            std::bind(&AccountManagementAPI::ServerUpdateBans, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementServerUpdateBansGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerUpdateBansGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementServerUpdateBansGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementUpdateBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementServerUpdateBansGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementUpdateBansResult*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync),
            std::bind(&AccountManagementAPI::GetTitlePlayersFromXboxLiveIDs, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetTitlePlayersFromProviderIDsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementGetTitlePlayersFromProviderIDsResponse*>(buffer);

        return S_OK;
    });
}

PF_API PFAccountManagementSetDisplayNameAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAccountManagementSetDisplayNameRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFAccountManagementSetDisplayNameAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAccountManagementSetDisplayNameAsync),
            std::bind(&AccountManagementAPI::SetDisplayName, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAccountManagementSetDisplayNameGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementSetDisplayNameGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAccountManagementSetDisplayNameGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementSetDisplayNameResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAccountManagementSetDisplayNameGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAccountManagementSetDisplayNameResponse*>(buffer);

        return S_OK;
    });
}

}
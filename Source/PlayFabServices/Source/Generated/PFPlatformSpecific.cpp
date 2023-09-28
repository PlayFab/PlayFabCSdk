#include "stdafx.h"
#include <playfab/services/PFPlatformSpecific.h>
#include "Generated/PlatformSpecific.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::PlatformSpecific;

#if 0
PF_API PFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync),
        std::bind(&PlatformSpecificAPI::ClientAndroidDevicePushNotificationRegistration, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if 0
PF_API PFPlatformSpecificClientRefreshPSNAuthTokenAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlatformSpecificRefreshPSNAuthTokenRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlatformSpecificClientRefreshPSNAuthTokenAsync),
        std::bind(&PlatformSpecificAPI::ClientRefreshPSNAuthToken, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if 0
PF_API PFPlatformSpecificClientRegisterForIOSPushNotificationAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlatformSpecificRegisterForIOSPushNotificationRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlatformSpecificClientRegisterForIOSPushNotificationAsync),
        std::bind(&PlatformSpecificAPI::ClientRegisterForIOSPushNotification, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFPlatformSpecificServerAwardSteamAchievementAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlatformSpecificAwardSteamAchievementRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlatformSpecificServerAwardSteamAchievementAsync),
        std::bind(&PlatformSpecificAPI::ServerAwardSteamAchievement, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlatformSpecificServerAwardSteamAchievementGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlatformSpecificServerAwardSteamAchievementGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlatformSpecificAwardSteamAchievementResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlatformSpecificAwardSteamAchievementResult*>(buffer);

    return S_OK;
}
#endif


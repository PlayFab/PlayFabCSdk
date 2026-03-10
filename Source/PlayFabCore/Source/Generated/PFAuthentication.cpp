#include "stdafx.h"
#include <playfab/core/PFAuthentication.h>
#include "Authentication.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"
#include "Authentication/LoginHandler.h"
#include "Authentication/GetEntityAsyncProvider.h"

using namespace PlayFab;
using namespace PlayFab::Authentication;

extern "C"
{

HRESULT PFAuthenticationLoginGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);

    Vector<char> tempBuffer;
    if (!bufferSize || !buffer)
    {
        // The buffer for the extended LoginResult is optional, but we report the result size to XAsync when the async
        // call completes (not when the client requests the result). XAsyncGetResult requires the specified buffer size
        // so create and pass it a temporary one if the client didn't provide one

        RETURN_HR_IF(E_INVALIDARG, result);
        RETURN_IF_FAILED(XAsyncGetResultSize(async, &bufferSize));
        tempBuffer.resize(bufferSize);
        buffer = tempBuffer.data();
    }

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    PFCombinedLoginResult* combinedLoginResult = reinterpret_cast<PFCombinedLoginResult*>(buffer);
    *entityHandle = combinedLoginResult->entityHandle;

    if (result)
    {
        *result = combinedLoginResult->loginResult;
    }

    return S_OK;
}

HRESULT PFAuthenticationServerLoginGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityTokenResponse);
    
    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    PFServerCombinedLoginResult* combinedLoginResult = reinterpret_cast<PFServerCombinedLoginResult*>(buffer);
    *entityTokenResponse = combinedLoginResult->entityTokenResponse;

    if (result)
    {
        *result = combinedLoginResult->loginResult;
    }

    return S_OK;
}

#if 0
PF_API PFAuthenticationLoginWithAndroidDeviceIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithAndroidDeviceIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithAndroidDeviceID",
            LoginWithAndroidDeviceIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithAndroidDeviceID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithAndroidDeviceIDAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithAndroidDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithAndroidDeviceIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithAndroidDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithAndroidDeviceIDGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithAndroidDeviceIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithAndroidDeviceIDAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithAndroidDeviceID",
            LoginWithAndroidDeviceIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithAndroidDeviceID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithAndroidDeviceIDAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationLoginWithAppleAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithAppleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithAppleAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithApple",
            LoginWithAppleRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithApple
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithAppleAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithAppleGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithAppleGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithAppleGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithAppleGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithAppleAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithAppleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithAppleAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithApple",
            LoginWithAppleRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithApple
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithAppleAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationLoginWithBattleNetAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithBattleNetAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithBattleNet",
            LoginWithBattleNetRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithBattleNet
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithBattleNetAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithBattleNetGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithBattleNetGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithBattleNetGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithBattleNetGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithBattleNetAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithBattleNetAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithBattleNet",
            LoginWithBattleNetRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithBattleNet
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithBattleNetAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAuthenticationLoginWithCustomIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithCustomIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithCustomID",
            LoginWithCustomIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithCustomID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithCustomIDAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithCustomIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithCustomIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithCustomIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithCustomIDGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithCustomIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithCustomIDAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithCustomID",
            LoginWithCustomIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithCustomID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithCustomIDAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if 0
PF_API PFAuthenticationLoginWithEmailAddressAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithEmailAddressRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithEmailAddressAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithEmailAddress",
            LoginWithEmailAddressRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithEmailAddress
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithEmailAddressAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithEmailAddressGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithEmailAddressGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithEmailAddressGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithEmailAddressGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithEmailAddressAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithEmailAddressRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithEmailAddressAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithEmailAddress",
            LoginWithEmailAddressRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithEmailAddress
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithEmailAddressAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS
PF_API PFAuthenticationLoginWithFacebookAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithFacebookRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithFacebook",
            LoginWithFacebookRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithFacebook
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithFacebookGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithFacebookGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithFacebookAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithFacebookRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithFacebookAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithFacebook",
            LoginWithFacebookRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithFacebook
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithFacebookAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAuthenticationLoginWithFacebookInstantGamesIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithFacebookInstantGamesIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookInstantGamesIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithFacebookInstantGamesId",
            LoginWithFacebookInstantGamesIdRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithFacebookInstantGamesId
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookInstantGamesIdAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithFacebookInstantGamesIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookInstantGamesIdGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithFacebookInstantGamesIdAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithFacebookInstantGamesIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithFacebookInstantGamesIdAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithFacebookInstantGamesId",
            LoginWithFacebookInstantGamesIdRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithFacebookInstantGamesId
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithFacebookInstantGamesIdAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_IOS
PF_API PFAuthenticationLoginWithGameCenterAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithGameCenterRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithGameCenterAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGameCenter",
            LoginWithGameCenterRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGameCenter
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithGameCenterAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithGameCenterGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGameCenterGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithGameCenterGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGameCenterGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithGameCenterAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGameCenterRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithGameCenterAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGameCenter",
            LoginWithGameCenterRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGameCenter
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithGameCenterAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFAuthenticationLoginWithGoogleAccountAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithGoogleAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithGoogleAccountAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGoogleAccount",
            LoginWithGoogleAccountRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGoogleAccount
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithGoogleAccountAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithGoogleAccountGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGoogleAccountGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithGoogleAccountGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGoogleAccountGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithGoogleAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGoogleAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithGoogleAccountAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGoogleAccount",
            LoginWithGoogleAccountRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGoogleAccount
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithGoogleAccountAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFAuthenticationLoginWithGooglePlayGamesServicesAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithGooglePlayGamesServicesRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithGooglePlayGamesServicesAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGooglePlayGamesServices",
            LoginWithGooglePlayGamesServicesRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGooglePlayGamesServices
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithGooglePlayGamesServicesAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithGooglePlayGamesServicesGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithGooglePlayGamesServicesGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithGooglePlayGamesServicesAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGooglePlayGamesServicesRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithGooglePlayGamesServicesAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithGooglePlayGamesServices",
            LoginWithGooglePlayGamesServicesRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithGooglePlayGamesServices
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithGooglePlayGamesServicesAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAuthenticationLoginWithIOSDeviceIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithIOSDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithIOSDeviceIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithIOSDeviceID",
            LoginWithIOSDeviceIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithIOSDeviceID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithIOSDeviceIDAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithIOSDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithIOSDeviceIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithIOSDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithIOSDeviceIDGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithIOSDeviceIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithIOSDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithIOSDeviceIDAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithIOSDeviceID",
            LoginWithIOSDeviceIDRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithIOSDeviceID
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithIOSDeviceIDAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAuthenticationLoginWithKongregateAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithKongregateRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithKongregateAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithKongregate",
            LoginWithKongregateRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithKongregate
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithKongregateAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithKongregateGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithKongregateGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithKongregateGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithKongregateGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithKongregateAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithKongregateRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithKongregateAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithKongregate",
            LoginWithKongregateRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithKongregate
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithKongregateAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
PF_API PFAuthenticationLoginWithNintendoServiceAccountAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithNintendoServiceAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoServiceAccountAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithNintendoServiceAccount",
            LoginWithNintendoServiceAccountRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithNintendoServiceAccount
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoServiceAccountAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithNintendoServiceAccountGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoServiceAccountGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithNintendoServiceAccountGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoServiceAccountGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithNintendoServiceAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithNintendoServiceAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithNintendoServiceAccountAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithNintendoServiceAccount",
            LoginWithNintendoServiceAccountRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithNintendoServiceAccount
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithNintendoServiceAccountAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithNintendoSwitchDeviceId",
            LoginWithNintendoSwitchDeviceIdRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithNintendoSwitchDeviceId
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithNintendoSwitchDeviceId",
            LoginWithNintendoSwitchDeviceIdRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithNintendoSwitchDeviceId
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAuthenticationLoginWithOpenIdConnectAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithOpenIdConnectAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithOpenIdConnect",
            LoginWithOpenIdConnectRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithOpenIdConnect
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithOpenIdConnectAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithOpenIdConnectGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithOpenIdConnectGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithOpenIdConnectGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithOpenIdConnectGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithOpenIdConnectAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithOpenIdConnect",
            LoginWithOpenIdConnectRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithOpenIdConnect
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithOpenIdConnectAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if 0
PF_API PFAuthenticationLoginWithPlayFabAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithPlayFabRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithPlayFabAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithPlayFab",
            LoginWithPlayFabRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithPlayFab
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithPlayFabAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithPlayFabGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithPlayFabGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithPlayFabGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithPlayFabGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithPlayFabAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithPlayFabRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithPlayFabAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithPlayFab",
            LoginWithPlayFabRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithPlayFab
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithPlayFabAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFAuthenticationLoginWithPSNAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithPSNRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithPSNAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithPSN",
            LoginWithPSNRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithPSN
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithPSNAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithPSNGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithPSNGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithPSNGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithPSNGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithPSNAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithPSNRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithPSNAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithPSN",
            LoginWithPSNRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithPSN
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithPSNAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationLoginWithSteamAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithSteamAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithSteam",
            LoginWithSteamRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithSteam
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithSteamAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithSteamGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithSteamGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithSteamGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithSteamGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithSteamAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithSteamAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithSteam",
            LoginWithSteamRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithSteam
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithSteamAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if 0
PF_API PFAuthenticationLoginWithTwitchAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithTwitchRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithTwitchAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithTwitch",
            LoginWithTwitchRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithTwitch
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithTwitchAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithTwitchGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithTwitchGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithTwitchGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithTwitchGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithTwitchAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithTwitchRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithTwitchAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithTwitch",
            LoginWithTwitchRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithTwitch
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithTwitchAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFAuthenticationLoginWithXboxAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationLoginWithXboxAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithXbox",
            LoginWithXboxRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithXbox
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationLoginWithXboxAsync),
            std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationLoginWithXboxGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithXboxGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationLoginWithXboxGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationLoginWithXboxGetResult), [&]()
    {
        return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
    });
}

PF_API PFAuthenticationReLoginWithXboxAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationReLoginWithXboxAsync), entityHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity)
    {
        SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithRequestHandler>(
            "/Client/LoginWithXbox",
            LoginWithXboxRequest{ *request }.ToJson(),
            CacheId::AuthenticationLoginWithXbox
        );

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFAuthenticationReLoginWithXboxAsync), 
            std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

#if 0
PF_API PFAuthenticationRegisterPlayFabUserAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationRegisterPlayFabUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationRegisterPlayFabUserAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationRegisterPlayFabUserAsync),
            std::bind(&AuthenticationAPI::RegisterPlayFabUser, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationRegisterPlayFabUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationRegisterPlayFabUserGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationRegisterPlayFabUserGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationRegisterPlayFabUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationRegisterPlayFabUserGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAuthenticationRegisterPlayFabUserResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationServerLoginWithAndroidDeviceIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithAndroidDeviceIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithAndroidDeviceIDAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithAndroidDeviceID, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithAndroidDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithAndroidDeviceIDGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationServerLoginWithBattleNetAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithBattleNetAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithBattleNetAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithBattleNet, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithBattleNetGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithBattleNetGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithBattleNetGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithBattleNetGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationServerLoginWithCustomIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithCustomIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithCustomIDAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithCustomID, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithCustomIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithCustomIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithCustomIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithCustomIDGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationServerLoginWithIOSDeviceIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithIOSDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithIOSDeviceIDAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithIOSDeviceIDAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithIOSDeviceID, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithIOSDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithIOSDeviceIDGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationServerLoginWithPSNAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithPSNRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithPSNAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithPSNAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithPSN, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithPSNGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithPSNGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithPSNGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithPSNGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationServerLoginWithServerCustomIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithServerCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithServerCustomIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithServerCustomIdAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithServerCustomId, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithServerCustomIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithServerCustomIdGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithServerCustomIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithServerCustomIdGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationServerLoginWithSteamIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithSteamIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithSteamIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithSteamIdAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithSteamId, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithSteamIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithSteamIdGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithSteamIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithSteamIdGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if 0
PF_API PFAuthenticationServerLoginWithTwitchAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithTwitchRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithTwitchAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithTwitchAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithTwitch, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithTwitchGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithTwitchGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithTwitchGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithTwitchGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationServerLoginWithXboxAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithXbox, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithXboxGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithXboxGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationServerLoginWithXboxIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithXboxIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxIdAsync),
            std::bind(&AuthenticationAPI::ServerLoginWithXboxId, state, std::move(context), secretKey, *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationServerLoginWithXboxIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxIdGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationServerLoginWithXboxIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxIdGetResult), [&]()
    {
        return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationAuthenticateGameServerWithCustomIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationAuthenticateCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationAuthenticateGameServerWithCustomIdAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationAuthenticateGameServerWithCustomIdAsync),
            std::bind(&AuthenticationAPI::AuthenticateGameServerWithCustomId, state, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationAuthenticateGameServerWithCustomIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _Out_opt_ bool* newlyCreated
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationAuthenticateGameServerWithCustomIdGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(entityHandle);

        PFAuthenticationAuthenticateGameServerResult result{};
        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, sizeof(PFAuthenticationAuthenticateGameServerResult), &result, nullptr));

        *entityHandle = result.entityHandle;
        if (newlyCreated)
        {
            *newlyCreated = result.newlyCreated;
        }
        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationDeleteAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationDeleteRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationDeleteAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationDeleteAsync),
            std::bind(&AuthenticationAPI::Delete, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationGetEntityAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationGetEntityAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeGetEntityProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationGetEntityAsync),
            std::bind(&AuthenticationAPI::GetEntity, state, std::move(context), *request, std::placeholders::_1),
        state
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationGetEntityGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationGetEntityGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFEntityHandle), entityHandle, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationGetEntityWithSecretKeyAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return ServiceConfigAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationGetEntityWithSecretKeyAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig> context)
    {

        auto provider = MakeGetEntityProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationGetEntityWithSecretKeyAsync),
            std::bind(&AuthenticationAPI::GetEntityWithSecretKey, state, std::move(context), secretKey, *request, std::placeholders::_1),
        state
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationGetEntityWithSecretKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationGetEntityWithSecretKeyGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFEntityHandle), entityHandle, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFAuthenticationValidateEntityTokenAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationValidateEntityTokenRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFAuthenticationValidateEntityTokenAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFAuthenticationValidateEntityTokenAsync),
            std::bind(&AuthenticationAPI::ValidateEntityToken, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFAuthenticationValidateEntityTokenGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationValidateEntityTokenGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFAuthenticationValidateEntityTokenGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationValidateEntityTokenResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFAuthenticationValidateEntityTokenGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFAuthenticationValidateEntityTokenResponse*>(buffer);

        return S_OK;
    });
}
#endif

}
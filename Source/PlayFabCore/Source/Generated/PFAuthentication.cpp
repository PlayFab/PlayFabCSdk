#include "stdafx.h"
#include <playfab/core/PFAuthentication.h>
#include "Authentication.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"
#include "Authentication/GetEntityAsyncProvider.h"

using namespace PlayFab;
using namespace PlayFab::Authentication;

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

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFAuthenticationLoginWithAndroidDeviceIDAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithAndroidDeviceIDAsync),
        std::bind(&AuthenticationAPI::LoginWithAndroidDeviceID, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithAndroidDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithAndroidDeviceIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithAndroidDeviceID, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if 0
PF_API PFAuthenticationLoginWithAppleAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithAppleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithAppleAsync),
        std::bind(&AuthenticationAPI::LoginWithApple, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithAppleGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithAppleAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithAppleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithApple, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithCustomIDAsync),
        std::bind(&AuthenticationAPI::LoginWithCustomID, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithCustomIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithCustomIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithCustomID, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithEmailAddressAsync),
        std::bind(&AuthenticationAPI::LoginWithEmailAddress, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithEmailAddressGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithEmailAddressAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithEmailAddressRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithEmailAddress, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFAuthenticationLoginWithFacebookAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithFacebookRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookAsync),
        std::bind(&AuthenticationAPI::LoginWithFacebook, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithFacebookGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithFacebookAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithFacebookRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithFacebook, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithFacebookInstantGamesIdAsync),
        std::bind(&AuthenticationAPI::LoginWithFacebookInstantGamesId, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithFacebookInstantGamesIdAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithFacebookInstantGamesIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithFacebookInstantGamesId, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if 0
PF_API PFAuthenticationLoginWithGameCenterAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithGameCenterRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithGameCenterAsync),
        std::bind(&AuthenticationAPI::LoginWithGameCenter, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithGameCenterGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithGameCenterAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGameCenterRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithGameCenter, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithGoogleAccountAsync),
        std::bind(&AuthenticationAPI::LoginWithGoogleAccount, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithGoogleAccountGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithGoogleAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGoogleAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithGoogleAccount, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithGooglePlayGamesServicesAsync),
        std::bind(&AuthenticationAPI::LoginWithGooglePlayGamesServices, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithGooglePlayGamesServicesAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithGooglePlayGamesServicesRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithGooglePlayGamesServices, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithIOSDeviceIDAsync),
        std::bind(&AuthenticationAPI::LoginWithIOSDeviceID, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithIOSDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithIOSDeviceIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithIOSDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithIOSDeviceID, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithKongregateAsync),
        std::bind(&AuthenticationAPI::LoginWithKongregate, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithKongregateGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithKongregateAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithKongregateRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithKongregate, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoServiceAccountAsync),
        std::bind(&AuthenticationAPI::LoginWithNintendoServiceAccount, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithNintendoServiceAccountGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithNintendoServiceAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithNintendoServiceAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithNintendoServiceAccount, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync),
        std::bind(&AuthenticationAPI::LoginWithNintendoSwitchDeviceId, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithNintendoSwitchDeviceId, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithOpenIdConnectAsync),
        std::bind(&AuthenticationAPI::LoginWithOpenIdConnect, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithOpenIdConnectGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithOpenIdConnect, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithPlayFabAsync),
        std::bind(&AuthenticationAPI::LoginWithPlayFab, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithPlayFabGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithPlayFabAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithPlayFabRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithPlayFab, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithPSNAsync),
        std::bind(&AuthenticationAPI::LoginWithPSN, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithPSNGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithPSNAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithPSNRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithPSN, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationLoginWithSteamAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithSteamAsync),
        std::bind(&AuthenticationAPI::LoginWithSteam, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithSteamGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithSteamAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithSteam, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
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

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithTwitchAsync),
        std::bind(&AuthenticationAPI::LoginWithTwitch, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithTwitchGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithTwitchAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithTwitchRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithTwitch, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationLoginWithXboxAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithXboxAsync),
        std::bind(&AuthenticationAPI::LoginWithXbox, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithXboxGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithXboxAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithXbox, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFAuthenticationLoginWithXUserAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithXUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationLoginWithXUserAsync),
        std::bind(&AuthenticationAPI::LoginWithXUser, state, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationLoginWithXUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFAuthenticationLoginWithXUserGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return PFAuthenticationLoginGetResult(async, entityHandle, bufferSize, buffer, result, bufferUsed);
}

PF_API PFAuthenticationReLoginWithXUserAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithXUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto provider = MakeProvider(state->RunContext().DeriveOnQueue(async->queue), async, __FUNCTION__, std::bind(&AuthenticationAPI::ReLoginWithXUser, std::move(entity), *request, std::placeholders::_1));
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if 0
PF_API PFAuthenticationRegisterPlayFabUserAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationRegisterPlayFabUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationRegisterPlayFabUserAsync),
        std::bind(&AuthenticationAPI::RegisterPlayFabUser, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationRegisterPlayFabUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFAuthenticationRegisterPlayFabUserGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationRegisterPlayFabUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFAuthenticationRegisterPlayFabUserResult*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationServerLoginWithServerCustomIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithServerCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationServerLoginWithServerCustomIdAsync),
        std::bind(&AuthenticationAPI::ServerLoginWithServerCustomId, state, std::move(context), secretKey, *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationServerLoginWithServerCustomIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationServerLoginWithSteamIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithSteamIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationServerLoginWithSteamIdAsync),
        std::bind(&AuthenticationAPI::ServerLoginWithSteamId, state, std::move(context), secretKey, *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationServerLoginWithSteamIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationServerLoginWithXboxAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxAsync),
        std::bind(&AuthenticationAPI::ServerLoginWithXbox, state, std::move(context), secretKey, *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationServerLoginWithXboxGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationServerLoginWithXboxIdAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithXboxIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationServerLoginWithXboxIdAsync),
        std::bind(&AuthenticationAPI::ServerLoginWithXboxId, state, std::move(context), secretKey, *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationServerLoginWithXboxIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
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
    return PFAuthenticationServerLoginGetResult(async, entityTokenResponse, bufferSize, buffer, result, bufferUsed);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationAuthenticateGameServerWithCustomIdAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationAuthenticateCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationAuthenticateGameServerWithCustomIdAsync),
        std::bind(&AuthenticationAPI::AuthenticateGameServerWithCustomId, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationAuthenticateGameServerWithCustomIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFAuthenticationAuthenticateGameServerWithCustomIdGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationAuthenticateCustomIdResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFAuthenticationAuthenticateCustomIdResult*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationDeleteAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationDeleteRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationDeleteAsync),
        std::bind(&AuthenticationAPI::Delete, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationGetEntityAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeGetEntityProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationGetEntityAsync),
        std::bind(&AuthenticationAPI::GetEntity, state, std::move(context), *request, std::placeholders::_1),
        state
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationGetEntityGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFEntityHandle), result, nullptr);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationGetEntityWithSecretKeyAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto provider = MakeGetEntityProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationGetEntityWithSecretKeyAsync),
        std::bind(&AuthenticationAPI::GetEntityWithSecretKey, state, std::move(context), secretKey, *request, std::placeholders::_1),
        state
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationGetEntityWithSecretKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFEntityHandle), result, nullptr);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
PF_API PFAuthenticationValidateEntityTokenAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFAuthenticationValidateEntityTokenRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFAuthenticationValidateEntityTokenAsync),
        std::bind(&AuthenticationAPI::ValidateEntityToken, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFAuthenticationValidateEntityTokenGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFAuthenticationValidateEntityTokenGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationValidateEntityTokenResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFAuthenticationValidateEntityTokenResponse*>(buffer);

    return S_OK;
}
#endif


#include "stdafx.h"
#include <playfab/core/PFEntity.h>
#include "GlobalState.h"
#include "ApiHelpers.h"
#include <ApiXAsyncProvider.h>

using namespace PlayFab;

PF_API PFEntityDuplicateHandle(
    _In_ PFEntityHandle entityHandle,
    _Out_ PFEntityHandle* duplicatedEntityHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEntityDuplicateHandle), [&](GlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(duplicatedEntityHandle);

        SharedPtr<Entity> entity;
        RETURN_IF_FAILED(state.Entities().FromHandle(entityHandle, entity));
        return state.Entities().MakeHandle(std::move(entity), *duplicatedEntityHandle);
    });
}

PF_API_(void) PFEntityCloseHandle(
    _In_ PFEntityHandle entityHandle
) noexcept
{
    ApiImpl(XASYNC_IDENTITY(PFEntityCloseHandle), [&](GlobalState& state)
    {
        state.Entities().CloseHandle(entityHandle);
        return S_OK;
    });
}

PF_API PFEntityGetEntityTokenAsync(
    _In_ PFEntityHandle entityHandle,
    _Inout_ XAsyncBlock* async
) noexcept
{
    // Although the internal method to retreive an EntityToken from and Entity object is synchronous, the public API is asynchronous for
    // a somewhat subtle reason. Because EntityTokens are not fixed size, clients must first call an API to query the size of the buffer required to
    // store the token, and only then request the token. At the same time, the SDK is updating EntityTokens in the backround. If PFEntityGetEntityToken
    // is were synchronous, we could run into a sequence where the client queries the size of the EntityToken, allocates a buffer, the SDK refreshes the token,
    // and then the client calls GetEntityToken only to find out that the newly refreshed Token doesn't fit in their buffer. This would make error handling
    // somewhat problemetic for callers. Instead, we make the public GetEntityToken call asynchronous, effectively associating a Token "snapshot" with an
    // XAsync operation. This functor wraps the internal syncronous Entity::GetEntityToken in an AsyncOp, allowing us to easily reuse the generic ApiXAsyncProvider

    struct GetEntityTokenAsyncWrapper
    {
        SharedPtr<Entity> const entity;

        AsyncOp<EntityToken> operator()(RunContext) noexcept
        {
            return entity->GetEntityToken();
        }
    };

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEntityGetEntityTokenAsync), entityHandle, [&](SharedPtr<Entity> entity, RunContext&& rc)
    {
        auto provider = MakeProvider(std::move(rc), async, XASYNC_IDENTITY(PFEntityGetEntityTokenAsync), GetEntityTokenAsyncWrapper{ std::move(entity) });
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEntityGetEntityTokenResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEntityGetEntityTokenResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ const PFEntityToken** entityToken,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityToken);

    HRESULT hr = XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed);
    if (SUCCEEDED(hr))
    {
        *entityToken = static_cast<PFEntityToken*>(buffer);
    }
    return hr;
}

#if HC_PLATFORM != HC_PLATFORM_GDK
PF_API PFEntityGetSecretKeySize(
    _In_  PFEntityHandle handle,
    _Out_ size_t* secretKeySize
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityGetSecretKeySize), handle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(secretKeySize);
        auto result{ entity->GetSecretKey() };
        RETURN_IF_FAILED(result.hr);

        *secretKeySize = result.ExtractPayload().size() + 1;
        return S_OK;
    });
}

PF_API PFEntityGetSecretKey(
    _In_ PFEntityHandle handle,
    _In_ size_t secretKeySize,
    _Out_writes_(secretKeySize) char* secretKeyBuffer,
    _Out_opt_ size_t* secretKeyUsed
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFServiceConfigGetTitleId), handle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(secretKeyBuffer);
        auto result{ entity->GetSecretKey() };
        RETURN_IF_FAILED(result.hr);

        String const& secretKey{ result.Payload() };
        RETURN_HR_IF(E_INVALIDARG, secretKeySize < secretKey.size() + 1);

        memcpy(secretKeyBuffer, secretKey.data(), secretKey.size() + 1);

        if (secretKeyUsed)
        {
            *secretKeyUsed = secretKey.size() + 1;
        }

        return S_OK;
    });
}
#endif

PF_API PFEntityGetEntityKeySize(
    _In_ PFEntityHandle entityHandle,
    _Out_ size_t* bufferSize
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityGetEntityKeySize), entityHandle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(bufferSize);
        *bufferSize = entity->EntityKey().RequiredBufferSize();
        return S_OK;
    });
}

PF_API PFEntityGetEntityKey(
    _In_ PFEntityHandle entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ const PFEntityKey** entityKey,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityGetEntityKey), entityHandle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(buffer);
        RETURN_HR_INVALIDARG_IF_NULL(entityKey);

        ModelBuffer b{ buffer, bufferSize };
        auto copyResult = entity->EntityKey().Copy(b);
        RETURN_IF_FAILED(copyResult.hr);
        *entityKey = copyResult.ExtractPayload();
        if (bufferUsed)
        {
            *bufferUsed = bufferSize - b.RemainingSpace();
        }
        return S_OK;
    });
}

PF_API PFEntityIsTitlePlayer(
    _In_ PFEntityHandle entityHandle,
    _Out_ bool* isTitlePlayer
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityIsTitlePlayer), entityHandle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(isTitlePlayer);
        *isTitlePlayer = std::strcmp(entity->EntityKey().Model().type, PFEntityTitlePlayerEntityType) == 0;
        return S_OK;
    });
}

PF_API PFEntityGetAPIEndpointSize(
    _In_  PFEntityHandle entityHandle,
    _Out_ size_t* apiEndpointSize
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityGetAPIEndpointSize), entityHandle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(apiEndpointSize);
        *apiEndpointSize = entity->ServiceConfig()->APIEndpoint().size() + 1;
        return S_OK;
    });
}

PF_API PFEntityGetAPIEndpoint(
    _In_ PFEntityHandle entityHandle,
    _In_ size_t apiEndpointSize,
    _Out_writes_(apiEndpointSize) char* apiEndpointBuffer,
    _Out_opt_ size_t* apiEndpointUsed
) noexcept
{
    return EntityApiImpl(XASYNC_IDENTITY(PFEntityGetAPIEndpoint), entityHandle, [&](SharedPtr<Entity> entity)
    {
        RETURN_HR_INVALIDARG_IF_NULL(apiEndpointBuffer);

        String const& apiEndpoint = entity->ServiceConfig()->APIEndpoint();
        RETURN_HR_IF(E_INVALIDARG, apiEndpointSize < apiEndpoint.size() + 1);

        memcpy(apiEndpointBuffer, apiEndpoint.data(), apiEndpoint.size() + 1);

        if (apiEndpointUsed)
        {
            *apiEndpointUsed = apiEndpoint.size() + 1;
        }

        return S_OK;
    });
}

PF_API PFEntityRegisterTokenExpiredEventHandler(
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ PFEntityTokenExpiredEventHandler* handler,
    _Out_ PFRegistrationToken* token
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEntityRegisterTokenExpiredEventHandler), [&](GlobalState& state)
    {
        return state.TokenExpiredHandler().RegisterClientHandler(state.RunContext().DeriveOnQueue(queue), context, handler, token);
    });
}

PF_API_(void) PFEntityUnregisterTokenExpiredEventHandler(
    _In_ PFRegistrationToken token
) noexcept
{
    SharedPtr<GlobalState> state;
    GlobalState::Get(state);
    if (state)
    {
        state->TokenExpiredHandler().UnregisterClientHandler(token);
    }
}

PF_API PFEntityRegisterTokenRefreshedEventHandler(
    _In_ XTaskQueueHandle queue,
    _In_opt_ void* context,
    _In_ PFEntityTokenRefreshedEventHandler* handler,
    _Out_ PFRegistrationToken* token
) noexcept
{
    SharedPtr<GlobalState> state;
    RETURN_IF_FAILED(GlobalState::Get(state));

    return state->TokenRefreshedHandler().RegisterClientHandler(
        state->RunContext().DeriveOnQueue(queue),
        context,
        handler,
        token
    );
}

PF_API_(void) PFEntityUnregisterTokenRefreshedEventHandler(
    _In_ PFRegistrationToken token
) noexcept
{

    SharedPtr<GlobalState> state;
    GlobalState::Get(state);
    if (state)
    {
        state->TokenRefreshedHandler().UnregisterClientHandler(token);
    }
}

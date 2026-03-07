#include "stdafx.h"
#include <playfab/core/PFLocalUser.h>
#include "LocalUser.h"
#include "ApiHelpers.h"
#include "ApiXAsyncProvider.h"
#include "LocalUserCache.h"

using namespace PlayFab;

PF_API PFLocalUserCreateHandleWithPersistedLocalId(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ const char* localId,
    _In_ PFLocalUserLoginHandler* loginHandler,
    _In_opt_ void* customContext,
    _Out_ PFLocalUserHandle* localUserHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserCreateHandleWithPersistedLocalId), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localId);
        RETURN_HR_INVALIDARG_IF_NULL(loginHandler);
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

        SharedPtr<ServiceConfig> serviceConfig;
        RETURN_IF_FAILED(state.ServiceConfigs().FromHandle(serviceConfigHandle, serviceConfig));
        
        // Create custom user to get the user object (similar to platform user pattern)
        Result<SharedPtr<CustomLocalUser>> result = CustomLocalUser::Make(serviceConfig, customContext, localId, state, *loginHandler);
        RETURN_IF_FAILED(result.hr);
        
        SharedPtr<LocalUser> newUser = result.ExtractPayload();
        
        // Use unified cache helper with known LocalId for "cache first" pattern
        auto& cache = Core::LocalUserCache::Instance();
        SharedPtr<LocalUser> localUser = cache.GetOrStoreLocalUser(newUser);
        RETURN_HR_IF(E_PF_CORE_LOCAL_USER_MISSING_ID, !localUser);
        
        // Create handle for the user (whether cached or newly created)
        return state.LocalUsers().MakeHandle(localUser, *localUserHandle);
    });
}

PF_API PFLocalUserDuplicateHandle(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ PFLocalUserHandle* duplicatedHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserDuplicateHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(duplicatedHandle);

        SharedPtr<LocalUser> localUser;
        RETURN_IF_FAILED(state.LocalUsers().FromHandle(localUserHandle, localUser));
        return state.LocalUsers().MakeHandle(std::move(localUser), *duplicatedHandle);
    });
}

PF_API_(void) PFLocalUserCloseHandle(
    _In_ PFLocalUserHandle localUserHandle
) noexcept
{
    ApiImpl(XASYNC_IDENTITY(PFLocalUserCloseHandle), [&](PFCoreGlobalState& state)
    {
        state.LocalUsers().CloseHandle(localUserHandle);
        return S_OK;
    });
}

PF_API_(int32_t) PFLocalUserHandleCompare(
    _In_opt_ PFLocalUserHandle user1,
    _In_opt_ PFLocalUserHandle user2
    ) noexcept
{
    if (user1 == user2)
    {
        return 0;
    }
    if (user1 == nullptr)
    {
        return 1;
    }
    if (user2 == nullptr)
    {
        return -1;
    }

    size_t size1{ 0 };
    if( SUCCEEDED(PFLocalUserGetLocalIdSize(user1, &size1)) && size1 > 0 )
    {
        Vector<char> localId1(size1, '\0');
        if (SUCCEEDED(PFLocalUserGetLocalId(user1, size1, &localId1[0], nullptr)))
        {
            size_t size2{ 0 };
            if( SUCCEEDED(PFLocalUserGetLocalIdSize(user2, &size2)) && size2 > 0)
            {
                Vector<char> localId2(size2, '\0');
                if (SUCCEEDED(PFLocalUserGetLocalId(user2, size2, &localId2[0], nullptr)))
                {
                    if (localId1.size() != localId2.size())
                    {
                        return localId1.size() < localId2.size() ? -1 : 1;
                    }
                    return strcmp(localId1.data(), localId2.data());
                }
            }
        }
    }
    
    return 1;  // something failed so just report that they don't match
}

PF_API PFLocalUserGetServiceConfigHandle(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ PFServiceConfigHandle* serviceConfigHandle
) noexcept
{
    return LocalUserApiImpl(XASYNC_IDENTITY(PFLocalUserGetServiceConfigHandle), localUserHandle, [&](SharedPtr<LocalUser> localUser)
    {
        SharedPtr<PFCoreGlobalState> state;
        RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

        SharedPtr<ServiceConfig> serviceConfig = localUser->GetServiceConfig();
        return state->ServiceConfigs().MakeHandle(std::move(serviceConfig), *serviceConfigHandle);
    });
}

PF_API PFLocalUserGetLocalIdSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* localIdSize
) noexcept
{
    return LocalUserApiImpl(XASYNC_IDENTITY(PFLocalUserGetLocalIdSize), localUserHandle, [&](SharedPtr<LocalUser> localUser)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localIdSize);
        *localIdSize = localUser->LocalId().size() + 1;
        return S_OK;
    });
}

PF_API PFLocalUserGetLocalId(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t localIdSize,
    _Out_writes_(localIdSize) char* localIdBuffer,
    _Out_opt_ size_t* localIdUsed
) noexcept
{
    return LocalUserApiImpl(XASYNC_IDENTITY(PFLocalUserGetLocalId), localUserHandle, [&](SharedPtr<LocalUser> localUser)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localIdBuffer);

        String localId = localUser->LocalId();
        RETURN_HR_IF(E_INVALIDARG, localIdSize < localId.size() + 1);

        memcpy(localIdBuffer, localId.data(), localId.size() + 1);

        if (localIdUsed)
        {
            *localIdUsed = localId.size() + 1;
        }

        return S_OK;
    });
}

PF_API PFLocalUserGetCustomContext(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ void** customContext
) noexcept
{
    return LocalUserApiImpl(XASYNC_IDENTITY(PFLocalUserGetCustomContext), localUserHandle, [&](SharedPtr<LocalUser> localUser)
    {
        RETURN_HR_INVALIDARG_IF_NULL(customContext);
        *customContext = localUser->CustomContext();
        return S_OK;
    });
}

PF_API PFLocalUserTryGetEntityHandle(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ PFEntityHandle* entityHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserTryGetEntityHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(entityHandle);

        SharedPtr<LocalUser> localUser;
        RETURN_IF_FAILED(state.LocalUsers().FromHandle(localUserHandle, localUser));

        SharedPtr<Entity> entity = localUser->Entity();
        return state.Entities().MakeHandle(std::move(entity), *entityHandle);
    });
}

PF_API PFLocalUserLoginAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ bool createAccount,
    _Inout_ XAsyncBlock* async
) noexcept
{
    return AsyncApiImpl(async, XASYNC_IDENTITY(PFLocalUserLoginAsync), [&](SharedPtr<PFCoreGlobalState> state, RunContext&& rc)
    {
        SharedPtr<LocalUser> localUser;
        RETURN_IF_FAILED(state->LocalUsers().FromHandle(localUserHandle, localUser));

        auto provider = MakeProvider(
            std::move(rc),
            async,
            XASYNC_IDENTITY(PFLocalUserLoginAsync),
            std::bind(&LocalUser::LoginOrGetCachedLoginResult, localUser.get(), std::move(state), createAccount, std::placeholders::_1)
        );

        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFLocalUserLoginGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

extern "C" PF_API PFLocalUserLoginGetResult(
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
    Authentication::PFCombinedLoginResult* combinedLoginResult = reinterpret_cast<Authentication::PFCombinedLoginResult*>(buffer);
    *entityHandle = combinedLoginResult->entityHandle;

    if (result)
    {
        *result = combinedLoginResult->loginResult;
    }

    return S_OK;
}
#include "stdafx.h"
#include <playfab/core/PFLocalUser_Xbox.h>
#include "LocalUser.h"
#include "LocalUser_Xbox.h"
#include "ApiHelpers.h"
#include "ApiXAsyncProvider.h"
#include "LocalUserCache.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_GDK // Temporary while GDK is still a separate platform

PF_API PFLocalUserCreateHandleWithXboxUser(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ XUserHandle user,
    _In_opt_ void* customContext,
    _Out_ PFLocalUserHandle* localUserHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserCreateHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

        SharedPtr<ServiceConfig> serviceConfig;
        RETURN_IF_FAILED(state.ServiceConfigs().FromHandle(serviceConfigHandle, serviceConfig));
        
        // Create Xbox user to get LocalId
        Result<SharedPtr<LocalUser>> result = MakeXboxUser(serviceConfig, user, customContext);
        RETURN_IF_FAILED(result.hr);
        
        SharedPtr<LocalUser> newUser = result.ExtractPayload();

        auto& cache = Core::LocalUserCache::Instance();
        SharedPtr<LocalUser> localUser = cache.GetOrStoreLocalUser(newUser);
        RETURN_HR_IF(E_PF_CORE_LOCAL_USER_MISSING_ID, !localUser);

        // Create handle for the user (whether cached or newly created)
        return state.LocalUsers().MakeHandle(localUser, *localUserHandle);
    });
}

PF_API PFLocalUserTryGetXUser(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ XUserHandle* user
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserTryGetXUser), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
        RETURN_HR_INVALIDARG_IF_NULL(user);

        SharedPtr<LocalUser> localUser;
        RETURN_IF_FAILED(state.LocalUsers().FromHandle(localUserHandle, localUser));
        SharedPtr<XboxLocalUser> xboxLocalUser = std::dynamic_pointer_cast<XboxLocalUser>(localUser);
        
        if (!xboxLocalUser)
        {
            return E_PF_CORE_LOCAL_USER_NO_PLATFORM_CONTEXT;
        }

        return XUserDuplicateHandle(xboxLocalUser->GetXUserHandle(), user);
    });
}

#endif
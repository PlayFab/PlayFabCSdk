#include "stdafx.h"
#include <playfab/core/PFLocalUser_Steam.h>
#include "LocalUser.h"
#include "LocalUser_Steam.h"
#include "ApiHelpers.h"
#include "ApiXAsyncProvider.h"
#include "LocalUserCache.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_GDK

PF_API PFLocalUserCreateHandleWithSteamUser(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_opt_ void* customContext,
    _Out_ PFLocalUserHandle* localUserHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFLocalUserCreateHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

        SharedPtr<ServiceConfig> serviceConfig;
        RETURN_IF_FAILED(state.ServiceConfigs().FromHandle(serviceConfigHandle, serviceConfig));

        // Create Steam user to get LocalId
        Result<SharedPtr<LocalUser>> result = MakeSteamUser(serviceConfig, customContext);
        RETURN_IF_FAILED(result.hr);

        SharedPtr<LocalUser> newUser = result.ExtractPayload();
        
        auto& cache = Core::LocalUserCache::Instance();
        SharedPtr<LocalUser> localUser = cache.GetOrStoreLocalUser(newUser);
        RETURN_HR_IF(E_PF_CORE_LOCAL_USER_MISSING_ID, !localUser);
        
        // Create handle for the user (cached or newly created)
        return state.LocalUsers().MakeHandle(localUser, *localUserHandle);
    });
}

#endif
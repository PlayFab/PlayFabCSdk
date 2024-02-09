#include "stdafx.h"
#include <playfab/core/PFServiceConfig.h>
#include "PFCoreGlobalState.h"
#include "ServiceConfig.h"
#include "ApiHelpers.h"

using namespace PlayFab;

PF_API PFServiceConfigCreateHandle(
    _In_z_ const char* apiEndpoint,
    _In_z_ const char* titleId,
    _Out_ PFServiceConfigHandle* serviceConfigHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFServiceConfigCreateHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(apiEndpoint);
        RETURN_HR_INVALIDARG_IF_NULL(titleId);
        RETURN_HR_INVALIDARG_IF_NULL(serviceConfigHandle);

        auto serviceConfig = MakeShared<PlayFab::ServiceConfig>(apiEndpoint, titleId, state.HttpRetrySettings());
        return state.ServiceConfigs().MakeHandle(std::move(serviceConfig), *serviceConfigHandle);
    });
}

PF_API PFServiceConfigDuplicateHandle(
    PFServiceConfigHandle handle,
    PFServiceConfigHandle* duplicatedHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFServiceConfigDuplicateHandle), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(duplicatedHandle);

        SharedPtr<ServiceConfig> serviceConfig;
        RETURN_IF_FAILED(state.ServiceConfigs().FromHandle(handle, serviceConfig));
        return state.ServiceConfigs().MakeHandle(std::move(serviceConfig), *duplicatedHandle);
    });
}

PF_API_(void) PFServiceConfigCloseHandle(
    PFServiceConfigHandle handle
) noexcept
{
    ApiImpl(XASYNC_IDENTITY(PFServiceConfigCloseHandle), [&](PFCoreGlobalState& state)
    {
        state.ServiceConfigs().CloseHandle(handle);
        return S_OK;
    });
}

PF_API PFServiceConfigGetAPIEndpointSize(
    _In_  PFServiceConfigHandle handle,
    _Out_ size_t* apiEndpointSize
) noexcept
{
    return ServiceConfigApiImpl(XASYNC_IDENTITY(PFServiceConfigGetAPIEndpointSize), handle, [&](SharedPtr<ServiceConfig> serviceConfig)
    {
        RETURN_HR_INVALIDARG_IF_NULL(apiEndpointSize);

        *apiEndpointSize = serviceConfig->HttpClient()->APIEndpoint().size() + 1;
        return S_OK;
    });
}

PF_API PFServiceConfigGetAPIEndpoint(
    _In_ PFServiceConfigHandle handle,
    _In_ size_t apiEndpointSize,
    _Out_writes_(apiEndpointSize) char* apiEndpointBuffer,
    _Out_opt_ size_t* apiEndpointUsed
) noexcept
{
    return ServiceConfigApiImpl(XASYNC_IDENTITY(PFServiceConfigGetAPIEndpoint), handle, [&](SharedPtr<ServiceConfig> serviceConfig)
    {
        RETURN_HR_INVALIDARG_IF_NULL(apiEndpointBuffer);

        String const& apiEndpoint = serviceConfig->HttpClient()->APIEndpoint();
        RETURN_HR_IF(E_INVALIDARG, apiEndpointSize < apiEndpoint.size() + 1);

        memcpy(apiEndpointBuffer, apiEndpoint.data(), apiEndpoint.size() + 1);

        if (apiEndpointUsed)
        {
            *apiEndpointUsed = apiEndpoint.size() + 1;
        }

        return S_OK;
    });
}

PF_API PFServiceConfigGetTitleIdSize(
    _In_  PFServiceConfigHandle handle,
    _Out_ size_t* titleIdSize
) noexcept
{
    return ServiceConfigApiImpl(XASYNC_IDENTITY(PFServiceConfigGetTitleIdSize), handle, [&](SharedPtr<ServiceConfig> serviceConfig)
    {
        RETURN_HR_INVALIDARG_IF_NULL(titleIdSize);

        *titleIdSize = serviceConfig->TitleId().size() + 1;
        return S_OK;
    });
}

PF_API PFServiceConfigGetTitleId(
    _In_ PFServiceConfigHandle handle,
    _In_ size_t titleIdSize,
    _Out_writes_(titleIdSize) char* titleIdBuffer,
    _Out_opt_ size_t* titleIdUsed
) noexcept
{
    return ServiceConfigApiImpl(XASYNC_IDENTITY(PFServiceConfigGetTitleId), handle, [&](SharedPtr<ServiceConfig> serviceConfig)
    {
        RETURN_HR_INVALIDARG_IF_NULL(titleIdBuffer);

        String const& titleId = serviceConfig->TitleId();
        RETURN_HR_IF(E_INVALIDARG, titleIdSize < titleId.size() + 1);

        memcpy(titleIdBuffer, titleId.data(), titleId.size() + 1);

        if (titleIdUsed)
        {
            *titleIdUsed = titleId.size() + 1;
        }

        return S_OK;
    });
}

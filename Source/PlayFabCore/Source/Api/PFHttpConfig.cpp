#include "stdafx.h"
#include "Common/PFCoreGlobalState.h"
#include "Common/ApiHelpers.h"

using namespace PlayFab;

PF_API PFSetHttpRetrySettings(
    _In_ PFHttpRetrySettings const* settings
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFSetHttpRetrySettings), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(settings);
        *state.HttpRetrySettings() = *settings;
        return S_OK;
    });
}

PF_API PFGetHttpRetrySettings(
    _Out_ PFHttpRetrySettings* settings
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFGetHttpRetrySettings), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(settings);
        *settings = *state.HttpRetrySettings();
        return S_OK;
    });
}

PF_API PFSetHttpSettings(
    _In_ PFHttpSettings const* settings
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFSetHttpSettings), [&](PFCoreGlobalState& state)
        {
            RETURN_HR_INVALIDARG_IF_NULL(settings);
            *state.HttpSettings() = *settings;
            return S_OK;
        });
}

PF_API PFGetHttpSettings(
    _Out_ PFHttpSettings* settings
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFGetHttpSettings), [&](PFCoreGlobalState& state)
        {
            RETURN_HR_INVALIDARG_IF_NULL(settings);
            *settings = *state.HttpSettings();
            return S_OK;
        });
}

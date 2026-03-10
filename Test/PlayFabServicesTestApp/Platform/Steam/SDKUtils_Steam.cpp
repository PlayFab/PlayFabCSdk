#include "TestAppPch.h"
#include "Platform/SDKUtils.h"
#include "steam/steam_api.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

HRESULT SDKInitialize() noexcept
{
    SteamErrMsg errMsg = { 0 };
    if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK)
    {
        TRACE_ERROR("SteamAPI_Init() failed: %s", errMsg);
        return E_FAIL;
    }

    if (!SteamUser()->BLoggedOn())
    {
        TRACE_ERROR("Steam user is not logged in");
        return E_FAIL;
    }

    return S_OK;
}

void SDKUpdate() noexcept
{
	SteamAPI_RunCallbacks();
}

}
}
}
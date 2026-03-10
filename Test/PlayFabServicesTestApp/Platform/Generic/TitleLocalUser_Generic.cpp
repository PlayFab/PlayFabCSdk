#include "TestAppPch.h"
#include "TitleLocalUser_Generic.h"
#include "Operations/Core/AuthenticationOperations.h"

namespace PlayFab
{
namespace Test
{

CustomLocalUserContext::CustomLocalUserContext(String _customId) : customId{ std::move(_customId) }
{
}

HRESULT CALLBACK CustomLocalUserContext::LocalUserLoginHandler(
    PFLocalUserHandle localUserHandle,
    PFServiceConfigHandle serviceConfigHandle,
    PFEntityHandle entityHandle,
    XAsyncBlock* async
)
{
    size_t size{ 0 };
    RETURN_IF_FAILED(PFLocalUserGetLocalIdSize(localUserHandle, &size));

    Vector<char> localId(size, '\0');
    RETURN_IF_FAILED(PFLocalUserGetLocalId(localUserHandle, size, &localId[0], nullptr));

    LoginWithCustomIDOperation::RequestType request;
    request.SetCustomId(localId.data());
    request.SetCreateAccount(true);

    if (entityHandle)
    {
        return PFAuthenticationReLoginWithCustomIDAsync(entityHandle, &request.Model(), async);
    }
    else
    {
        return PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request.Model(), async);
    }
}

} // Test
} // PlayFab
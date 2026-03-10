#include "TestAppPch.h"
#include "TitleLocalUser_Apple.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "PlayFabApple.h"

namespace PlayFab
{
namespace Test
{

AppleLocalUserContext::AppleLocalUserContext(String _customId) : customId{ std::move(_customId) }
{
}

Result<String> AppleLocalUserContext::GetUserIdentityToken()
{
    std::string identityToken = "";
    const char* token = GetIdentityToken();
    identityToken = token;
    return String{ identityToken };
}

HRESULT CALLBACK AppleLocalUserContext::LocalUserLoginHandler(
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

    Result<String> result = GetUserIdentityToken();
    
    if (result.ExtractPayload() != "")
    {
        RETURN_IF_FAILED(result.hr);
        Platform::LoginWithAppleOperation::RequestType request;
        request.SetIdentityToken(result.ExtractPayload());
        request.SetCreateAccount(true);
        
        if (entityHandle)
        {
            return PFAuthenticationReLoginWithAppleAsync(entityHandle, &request.Model(), async);
        }
        else
        {
            return PFAuthenticationLoginWithAppleAsync(serviceConfigHandle, &request.Model(), async);
        }
    }
    else
    {
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
}

}
}

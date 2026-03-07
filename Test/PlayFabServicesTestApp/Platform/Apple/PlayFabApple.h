#include "TestAppPch.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/Apple/TitleLocalUser_Apple.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

class LoginWithAppleOperation : public XAsyncOperation<LoginResult>
{
public:
    
    using RequestType = Wrappers::PFAuthenticationLoginWithAppleRequestWrapper<Allocator>;
    
    LoginWithAppleOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);
    
private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;
    
    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

}
}
}

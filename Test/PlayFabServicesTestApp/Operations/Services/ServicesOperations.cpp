#include "TestAppPch.h"
#include "ServicesOperations.h"

namespace PlayFab
{
namespace Test
{

HRESULT ServicesUninitializeOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFServicesUninitializeAsync(async);
}

}
}

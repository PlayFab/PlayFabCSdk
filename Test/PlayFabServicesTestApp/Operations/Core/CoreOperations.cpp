#include "TestAppPch.h"
#include "CoreOperations.h"

namespace PlayFab
{
namespace Test
{

HRESULT CoreUninitializeOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFUninitializeAsync(async);
}

}
}

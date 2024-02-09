#include "pch.h"
#include "PlatformUser_Apple.h"
#include <unistd.h>

namespace PlayFab
{
namespace Platform
{

Result<String> User::GetUserIdentityToken() const noexcept
{
    std::string identityToken = "";
    const char* token = GetIdentityToken();
    identityToken = token;
    return String{ identityToken };
}

AsyncOp<String> User::GetUserIdentityToken(RunContext rc) const noexcept
{
    auto asyncOpContext = MakeShared<AsyncOpContext<String>>();

    rc.TaskQueueSubmitWork([asyncOpContext, this](void)
    {
        asyncOpContext->Complete(this->GetUserIdentityToken());
    });

    return asyncOpContext;
}

}
}

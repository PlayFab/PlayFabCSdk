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

LocalUserLoginOperation::LocalUserLoginOperation(
    TitleLocalUser localUser,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_localUser{ std::move(localUser) }
{
}

HRESULT LocalUserLoginOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLocalUserLoginAsync(m_localUser.handleWrapper.Handle(), true, async);
}

Result<LoginResult> LocalUserLoginOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFLocalUserLoginGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFLocalUserLoginGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

}
}

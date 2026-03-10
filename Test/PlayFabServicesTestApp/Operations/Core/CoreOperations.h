#pragma once

namespace PlayFab
{
namespace Test
{

class CoreUninitializeOperation : public XAsyncOperation<void>
{
public:
    using XAsyncOperation::XAsyncOperation;

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
};

class LocalUserLoginOperation : public XAsyncOperation<LoginResult>
{
public:
    LocalUserLoginOperation(TitleLocalUser localUser, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;

    TitleLocalUser m_localUser;
};

}
}

#pragma once

namespace PlayFab
{
namespace Test
{

class ServicesUninitializeOperation : public XAsyncOperation<void>
{
public:
    using XAsyncOperation::XAsyncOperation;

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
};

}
}
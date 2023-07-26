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

}
}

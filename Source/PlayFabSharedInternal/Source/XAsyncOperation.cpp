#include "pch.h"
#include "XAsyncOperation.h"

namespace PlayFab
{

IOperation::IOperation(PlayFab::RunContext&& runContext) noexcept : m_rc{ std::move(runContext) }
{
}

void IOperation::Run(UniquePtr<IOperation> operation) noexcept
{
    operation->OnStarted(std::move(operation));
}

RunContext IOperation::RunContext() const noexcept
{
    return m_rc;
}

XAsyncOperationBase::XAsyncOperationBase(PlayFab::RunContext&& runContext) noexcept :
    IOperation{ std::move(runContext) },
    m_asyncBlock{ RunContext().TaskQueueHandle(), this, XAsyncCompletionCallback, {} }
{
}

XAsyncOperationBase::~XAsyncOperationBase() noexcept
{
    RunContext().CancellationToken().UnregisterForNotificationAndCheck(*this);
}

void XAsyncOperationBase::OnStarted(UniquePtr<IOperation> self) noexcept
{
    bool alreadyCancelled = RunContext().CancellationToken().RegisterForNotificationAndCheck(*this);
    if (alreadyCancelled)
    {
        OnFailed(E_ABORT);
        return;
    }

    HRESULT hr = OnStarted(&m_asyncBlock);
    if (FAILED(hr))
    {
        // XAsyncCompletionCallback won't be called in this case. Operation will be destroyed here
        OnFailed(hr);
        return;
    }

    // Release operation. It will be reclaim in XAsyncCompletionCallback
    self.release();
}

void XAsyncOperationBase::OnCancellation() noexcept
{
    XAsyncCancel(&m_asyncBlock);
}

void CALLBACK XAsyncOperationBase::XAsyncCompletionCallback(XAsyncBlock* async)
{
    UniquePtr<XAsyncOperationBase> operation{ static_cast<XAsyncOperationBase*>(async->context) };
    operation->OnCompleted(async);
}

}

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "XAsyncProviderBase.h"
#include "EnumTraits.h"

namespace PlayFab
{

constexpr char defaultProviderIdentity[]{ "UnnamedProvider" };

XAsyncProviderBase::XAsyncProviderBase(_In_ RunContext&& runContext, _In_ XAsyncBlock* async) noexcept
    : identityName{ defaultProviderIdentity },
    m_runContext{ std::move(runContext) },
    m_async{ async }
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProviderBase ctor", threadIdStream.str().c_str());
}

HRESULT XAsyncProviderBase::Run(_In_ UniquePtr<XAsyncProviderBase> provider) noexcept
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProviderBase::Run", threadIdStream.str().c_str());

    RETURN_HR_IF(E_ABORT, provider->m_runContext.RegisterTerminableAndCheck(*provider));
    RETURN_IF_FAILED(XAsyncBegin(provider->m_async, provider.get(), nullptr, provider->identityName, XAsyncProvider));
    provider.release();
    return S_OK;
}

HRESULT XAsyncProviderBase::Begin(RunContext)
{
    return Schedule(0);
}

HRESULT XAsyncProviderBase::DoWork(RunContext)
{
    return E_FAIL;
}

HRESULT XAsyncProviderBase::GetResult(void*, size_t)
{
    assert(false);
    return E_UNEXPECTED;
}

HRESULT XAsyncProviderBase::Schedule(uint32_t delay) const
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProviderBase::Schedule", threadIdStream.str().c_str());
    return XAsyncSchedule(m_async, delay);
}

void XAsyncProviderBase::Complete(size_t resultSize)
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProviderBase::Complete", threadIdStream.str().c_str());
    XAsyncComplete(m_async, S_OK, resultSize);
}

void XAsyncProviderBase::Fail(HRESULT hr)
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProviderBase::Fail HR:0x%08x", threadIdStream.str().c_str(), static_cast<uint32_t>(hr));
    XAsyncComplete(m_async, hr, 0);
}

void XAsyncProviderBase::Terminate(ITerminationListener& listener, void* context) noexcept
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] XAsyncProvider terminated before completion. PFUninitialize may be blocked until result payload is retreived.", threadIdStream.str().c_str());

    assert(!m_terminationListener);
    m_terminationListener = &listener;
    m_terminationListenerContext = context;
}

HRESULT CALLBACK XAsyncProviderBase::XAsyncProvider(_In_ XAsyncOp op, _Inout_ const XAsyncProviderData* data) noexcept
{
    auto provider{ static_cast<XAsyncProviderBase*>(data->context) };

    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[XAsyncProviderBase] [ThreadID %s] Provider[ID=%s] XAsyncOp::%s", threadIdStream.str().c_str(), provider->identityName, EnumName(op));

    switch (op)
    {
    case XAsyncOp::Begin:
        try
        {
            return provider->Begin(provider->m_runContext);
        }
        catch (...)
        {
            return CurrentExceptionToHR();
        }
    case XAsyncOp::DoWork:
        try
        {
            return provider->DoWork(provider->m_runContext);
        }
        catch (...)
        {
            return CurrentExceptionToHR();
        }
    case XAsyncOp::GetResult:
        try
        {
            return provider->GetResult(data->buffer, data->bufferSize);
        }
        catch (...)
        {
            return CurrentExceptionToHR();
        }
    case XAsyncOp::Cancel:
    {
        try
        {
            provider->m_runContext.CancellationToken().Cancel();
            return S_OK;
        }
        catch (...)
        {
            return CurrentExceptionToHR();
        }
    }
    case XAsyncOp::Cleanup:
    {
        // Cleanup should only fail in catastrophic cases. Can't pass result to client 
        // at this point so die with exception.

        // Copy the ITerminationListener locally before destroying the provider, but unregister
        // beforehand to avoid race condition between termination and provider cleanup.
        bool terminated = provider->m_runContext.UnregisterTerminableAndCheck(*provider);
        ITerminationListener* terminationListener = provider->m_terminationListener;
        void* terminationListenerContext = provider->m_terminationListenerContext;

        // Sanity check - if the runContext was terminated, we should have a terminationListener
        assert(!terminated || terminationListener);
        UNREFERENCED_PARAMETER(terminated);

        // Cleanup provider
        UniquePtr<XAsyncProviderBase>{ provider }.reset();

        // Notify listener if needed
        if (terminationListener)
        {
            terminationListener->OnTerminated(terminationListenerContext);
        }

        return S_OK;
    }
    default:
    {
        assert(false);
        return S_OK;
    }
    }
}

} // PlayFab

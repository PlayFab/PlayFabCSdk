// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <XAsyncProvider.h>
#include "RunContext.h"
#include "AsyncOp.h"

namespace PlayFab
{

// XAsyncProviderBase is a helper class that makes implementing XAsync providers easier. It is designed to reduce the 
// amount of boilerplate and edge cases each provider needs to implement.
// 
// Each XAsync API should use a derived provider class and override the relevant operations: Begin, DoWork,
// and GetResult.
class XAsyncProviderBase : public ITerminable
{
public:
    XAsyncProviderBase(const XAsyncProviderBase&) = delete;
    XAsyncProviderBase& operator=(const XAsyncProviderBase&) = delete;
    virtual ~XAsyncProviderBase() noexcept = default;

    // Runs an XAsync Provider. After calling Run, the provider operations Begin, DoWork, and GetResult will 
    // be called by the XAsync framework. The lifetime of the provider will be managed by the Provider
    // base class; it will be destroyed automatically after the operation is complete and the client has gotten
    // the result.
    static HRESULT Run(_In_ UniquePtr<XAsyncProviderBase> provider) noexcept;

protected:
    // Create a Provider from a client provided XAsyncBlock
    XAsyncProviderBase(_In_ RunContext&& runContext, _In_ XAsyncBlock* async) noexcept;

    // Create a Provider from a client provided XAsyncBlock with the specified identityName
    template<size_t n>
    XAsyncProviderBase(_In_ RunContext&& runContext, _In_ XAsyncBlock* async, const char(&_identityName)[n]) noexcept : identityName{ _identityName }, m_runContext{ std::move(runContext) }, m_async{ async } {}

    // Provider Operations to be overridden.

    // The Begin operation should start the asynchronous task, either by calling Schedule(), or by calling
    // another asynchronous API. Begin will be invoked synchronously by Run(), so this operation should never block.
    //
    // Default implementation will call Schedule with no delay.
    virtual HRESULT Begin(RunContext runContext);

    // Perform any long running work. This method is invoked is guaranteed always be invoked on the thread dictated by the Provider's
    // XAsync task queue.
    //
    // Default implementation will return E_FAIL, marking the operation as completed with no result payload.
    virtual HRESULT DoWork(RunContext runContext);

    // The GetResult operation should copy the result payload into a client provided buffer. GetResult will be called
    // synchronously when the client calls the appropriate get result API.
    //
    // Providers implementing XAsync APIs with a result payload MUST override this method.
    // Default implementation will assert and return E_UNEXPECTED (it will never be invoked for XAsync APIs with
    // no result payload).
    virtual HRESULT GetResult(void* buffer, size_t bufferSize);

    // Schedule DoWork operation to the TaskQueue. Can be called multiple times.
    virtual HRESULT Schedule(uint32_t delay) const;

protected:
    // Methods to be called by derived classes to mark XAsyncOp as complete

    // Mark the async operation as successfully completed. Provide the needed buffer size to hold the result object.
    void Complete(size_t resultSize);

    // Marks the operation as complete with a failure code. By design, the client won't get a result payload
    // when the operation fails. The Provider's GetResult method will never be invoked in this case.
    void Fail(HRESULT hr);

    // Provider identity name, used for logging purposes. Typically this is set with the __FUNCTION__ compiler macro
    // or XASYNC_IDENTITY(API)
    const char* const identityName;

private:
    // ITerminable
    void Terminate(ITerminationListener& listener, void* context) noexcept override;

    static HRESULT CALLBACK XAsyncProvider(_In_ XAsyncOp op, _Inout_ const XAsyncProviderData* data) noexcept;

    RunContext m_runContext;
    XAsyncBlock* m_async{ nullptr };

    ITerminationListener* m_terminationListener{ nullptr };
    void* m_terminationListenerContext{ nullptr };
};

} // namespace PlayFab

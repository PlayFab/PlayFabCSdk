#pragma once

#include "XAsyncProviderBase.h"
#include "AsyncOp.h"
#include "Trace.h"
#include "Types.h"

namespace PlayFab
{

// XAsyncProvider for PlayFab API calls
template<typename CallT>
class ApiXAsyncProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    ApiXAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], CallT call) :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_call{ call }
    {
    }

protected:
    using ResultT = typename Detail::UnwrapAsyncT<typename std::invoke_result_t<CallT, RunContext>>;

    // Always kick of the API call during XAsyncOp::Begin
    HRESULT Begin(RunContext runContext) override
    {
        m_call(std::move(runContext)).Finally([this](Result<ResultT> result)
        {
            if (Succeeded(result))
            {
                TRACE_VERBOSE("ApiProvider[ID=%s] Call suceeded (hr=0x%08x)", identityName, result.hr);
                this->Complete(this->StoreResult(std::move(result)));
            }
            else
            {
                TRACE_ERROR("ApiProvider[ID=%s] Call failed with message \"%s\" (hr=0x%08x)", identityName, result.errorMessage.data(), result.hr);
                this->Fail(result.hr);
            }
        });

        return S_OK;
    }

    HRESULT GetResult(void* buffer, size_t bufferSize) override
    {
        return GetResultHelper(buffer, bufferSize);
    }

    // How we deliver the XAsync result will depend on ResultT of m_call:
    // 1. If there is no result payload, there is no result to store & GetResult Op should never be called.
    // 2. If the result payload is serializable, store the result, report the needed result buffer size, and serialize it into
    //    the clients buffer during GetResult.
    // 3. If the result payload isn't serializable, store the result, report the needed buffer size as sizeof(PFResultHandle),
    //    and during GetResult, create a PFResult object return a pointer to that.

    template<typename T = ResultT>
    size_t StoreResult(Result<void>&&, typename std::enable_if_t<std::is_void_v<T>>* = 0)
    {
        // No result to store in this case
        return 0;
    }

    template<typename T = ResultT>
    size_t StoreResult(Result<ResultT>&& result, typename std::enable_if_t<std::is_base_of_v<ClientOutputModel<typename T::ModelType>, T>>* = 0)
    {
        this->m_result.emplace(std::move(result));
        return this->m_result->Payload().RequiredBufferSize();
    }

    template<typename T = ResultT>
    HRESULT GetResultHelper(void*, size_t, typename std::enable_if_t<std::is_void_v<T>>* = 0)
    {
        // Should never be called
        assert(false);
        return E_UNEXPECTED;
    }

    template<typename T = ResultT>
    HRESULT GetResultHelper(void* buffer, size_t bufferSize, typename std::enable_if_t<std::is_base_of_v<ClientOutputModel<typename T::ModelType>, T>>* = 0)
    {
        assert(m_result.has_value());
        ModelBuffer b{ buffer, bufferSize };
        auto copyResult = m_result->Payload().Copy(b);
        if (SUCCEEDED(copyResult.hr))
        {
            // Result should always be written to the beginning of the result buffer
            assert((void*)(copyResult.Payload()) == buffer);
        }
        return copyResult.hr;
    }

    CallT m_call;
    // Storing Result<ResultT> rather than ResultT because Result<void> is valid so we don't need a specialization for that
    std::optional<Result<ResultT>> m_result;

    using XAsyncProviderBase::identityName;
};

template<typename CallT, size_t n>
UniquePtr<ApiXAsyncProvider<CallT>> MakeProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], CallT call)
{
    return MakeUnique<ApiXAsyncProvider<CallT>>(std::move(rc), async, identityName, std::move(call));
}

}

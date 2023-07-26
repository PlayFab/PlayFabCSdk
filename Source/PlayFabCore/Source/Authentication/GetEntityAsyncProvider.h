#pragma once

#include <XAsyncProviderBase.h>
#include "GlobalState.h"

namespace PlayFab
{

// XAsyncProvider for PlayFab GetEntity API calls. This provider wraps an internal call which returns a SharedPtr<Entity> rather than a ClientOutputModel.
// It holds a referece to the GlobalState so that it has access to the EntityTable and can create and provide a PFEntityHandle to the title in GetResult.
template<typename CallT>
class GetEntityAsyncProvider : public XAsyncProviderBase
{
public:
    static_assert(std::is_same_v<SharedPtr<Entity>, typename Detail::UnwrapAsyncT<typename std::invoke_result_t<CallT, RunContext>>>, "CallT must return an Entity");

    template<size_t n>
    GetEntityAsyncProvider(RunContext&& runContext, XAsyncBlock* async, const char(&identityName)[n], CallT authCall, SharedPtr<GlobalState> state)
        : XAsyncProviderBase{ std::move(runContext), async, identityName },
        m_call{ authCall },
        m_state{ std::move(state) }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        m_call(std::move(runContext)).Finally([this](Result<SharedPtr<Entity>> result)
        {
            if (Succeeded(result))
            {
                TRACE_VERBOSE("AuthXAsyncProvider[ID=%s] Call suceeded (hr=0x%08x)", identityName, result.hr);
                m_result = result.ExtractPayload();
                this->Complete(sizeof(PFEntityHandle));
            }
            else
            {
                TRACE_ERROR("AuthXAsyncProvider[ID=%s] Call failed with message \"%s\" (hr=0x%08x)", identityName, result.errorMessage.data(), result.hr);
                this->Fail(result.hr);
            }
        });

        return S_OK;
    }

    HRESULT GetResult(void* buffer, size_t bufferSize) override
    {
        UNREFERENCED_PARAMETER(bufferSize);
        assert(bufferSize == sizeof(PFEntityHandle));
        auto entityHandlePtr = static_cast<PFEntityHandle*>(buffer);
        return m_state->Entities().MakeHandle(m_result, *entityHandlePtr);
    }

private:
    CallT m_call;
    SharedPtr<GlobalState> m_state;
    SharedPtr<Entity> m_result;
};

template<typename CallT, size_t n>
UniquePtr<GetEntityAsyncProvider<CallT>> MakeGetEntityProvider(RunContext&& runContext, XAsyncBlock* async, const char(&identityName)[n], CallT authCall, SharedPtr<GlobalState> state)
{
    return MakeUnique<GetEntityAsyncProvider<CallT>>(std::move(runContext), async, identityName, std::move(authCall), std::move(state));
}

}

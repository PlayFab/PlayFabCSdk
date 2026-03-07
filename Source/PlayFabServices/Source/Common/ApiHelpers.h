#pragma once

#include "Error.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Detail
{
HRESULT CompleteAsyncWithError(XAsyncBlock* async, const char* apiIdentity, HRESULT hr);
}

template<typename TWork>
inline HRESULT ApiImpl(const char* apiIdentity, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GlobalState> state;
        RETURN_IF_FAILED(GlobalState::Get(state));

        return work(*state);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT AsyncApiImpl(XAsyncBlock* async, const char* apiIdentity, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GlobalState> state;
        HRESULT hr = GlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        hr = work(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }
        return S_OK;
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

// Special case for result APIs as they are intentionally not reliate on Global State. This allows them to be called
// after cleanup has started
template<typename TWork>
inline HRESULT ResultApiImpl(const char* apiIdentity, TWork&& work) noexcept
{
    try
    {
        return work();
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

}

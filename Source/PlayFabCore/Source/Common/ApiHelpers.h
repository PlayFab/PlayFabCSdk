#pragma once

#include "Error.h"
#include "PFCoreGlobalState.h"

namespace PlayFab
{
namespace Detail
{
HRESULT CompleteAsyncWithError(XAsyncBlock* async, const char* apiIdentity, HRESULT hr);

HRESULT ExecuteExtensionCallbacksFoo() noexcept;
}

template<typename TWork>
inline HRESULT ApiImpl(const char* apiIdentity, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

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
        SharedPtr<PFCoreGlobalState> state;
        HRESULT hr = PFCoreGlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        hr = work(state, state->RunContext().DeriveOnQueue(async->queue));
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

template<typename TWork>
inline HRESULT ServiceConfigApiImpl(const char* apiIdentity, PFServiceConfigHandle serviceConfigHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

        SharedPtr<ServiceConfig> serviceConfig;
        RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(serviceConfigHandle, serviceConfig));

        return work(serviceConfig);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT ServiceConfigAsyncApiImpl(XAsyncBlock* async, const char* apiIdentity, PFServiceConfigHandle serviceConfigHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        HRESULT hr = PFCoreGlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        SharedPtr<ServiceConfig> serviceConfig;
        hr = state->ServiceConfigs().FromHandle(serviceConfigHandle, serviceConfig);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        hr = work(state, serviceConfig);
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

template<typename TWork>
inline HRESULT EntityApiImpl(const char* apiIdentity, PFEntityHandle entityHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

        SharedPtr<Entity> entity;
        RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

        return work(std::move(entity));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT EntityAsyncApiImpl(XAsyncBlock* async, const char* apiIdentity, PFEntityHandle entityHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        HRESULT hr = PFCoreGlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        SharedPtr<Entity> entity;
        hr = state->Entities().FromHandle(entityHandle, entity);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        return work(state, std::move(entity));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT LocalUserApiImpl(const char* apiIdentity, PFLocalUserHandle localUserHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

        SharedPtr<LocalUser> localUser;
        RETURN_IF_FAILED(state->LocalUsers().FromHandle(localUserHandle, localUser));

        return work(std::move(localUser));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

template<typename TWork>
inline HRESULT LocalUserAsyncApiImpl(XAsyncBlock* async, const char* apiIdentity, PFLocalUserHandle localUserHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        HRESULT hr = PFCoreGlobalState::Get(state);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        SharedPtr<LocalUser> localUser;
        hr = state->LocalUsers().FromHandle(localUserHandle, localUser);
        if (FAILED(hr))
        {
            return Detail::CompleteAsyncWithError(async, apiIdentity, hr);
        }

        return work(std::move(localUser), state->RunContext().DeriveOnQueue(async->queue));
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

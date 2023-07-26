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

        return work(*state);
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
        SharedPtr<GlobalState> state;
        RETURN_IF_FAILED(GlobalState::Get(state));

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
inline HRESULT EntityApiImpl(const char* apiIdentity, PFEntityHandle entityHandle, TWork&& work) noexcept
{
    try
    {
        SharedPtr<GlobalState> state;
        RETURN_IF_FAILED(GlobalState::Get(state));

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
        SharedPtr<GlobalState> state;
        HRESULT hr = GlobalState::Get(state);
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

        return work(std::move(entity), state->RunContext().DeriveOnQueue(async->queue));
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, apiIdentity, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

}

#include "stdafx.h"
#include <playfab/core/PFEvents.h>
#include "Events.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Events;

PF_API PFEventsWriteEventsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFEventsWriteEventsAsync),
        std::bind(&EventsAPI::WriteEvents, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsWriteEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsWriteEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsWriteEventsResponse*>(buffer);

    return S_OK;
}

PF_API PFEventsWriteTelemetryEventsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    SharedPtr<Entity> context;
    RETURN_IF_FAILED(state->Entities().FromHandle(contextHandle, context));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async, 
        XASYNC_IDENTITY(PFEventsWriteTelemetryEventsAsync),
        std::bind(&EventsAPI::WriteTelemetryEvents, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsWriteTelemetryEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsWriteTelemetryEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsWriteEventsResponse*>(buffer);

    return S_OK;
}


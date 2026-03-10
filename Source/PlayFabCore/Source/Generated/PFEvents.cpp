#include "stdafx.h"
#include <playfab/core/PFEvents.h>
#include "Events.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Events;

extern "C"
{

#if 0
PF_API PFEventsDeleteDataConnectionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsDeleteDataConnectionRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsDeleteDataConnectionAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsDeleteDataConnectionAsync),
            std::bind(&EventsAPI::DeleteDataConnection, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsDeleteDataConnectionGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEventsDeleteDataConnectionResponse* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsDeleteDataConnectionGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFEventsDeleteDataConnectionResponse), result, nullptr);
    });
}
#endif

#if 0
PF_API PFEventsGetDataConnectionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsGetDataConnectionRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsGetDataConnectionAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsGetDataConnectionAsync),
            std::bind(&EventsAPI::GetDataConnection, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsGetDataConnectionGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsGetDataConnectionGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsGetDataConnectionGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsGetDataConnectionResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsGetDataConnectionGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsGetDataConnectionResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFEventsListDataConnectionsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsListDataConnectionsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsListDataConnectionsAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsListDataConnectionsAsync),
            std::bind(&EventsAPI::ListDataConnections, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsListDataConnectionsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsListDataConnectionsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsListDataConnectionsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsListDataConnectionsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsListDataConnectionsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsListDataConnectionsResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFEventsSetDataConnectionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsSetDataConnectionRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsSetDataConnectionAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsSetDataConnectionAsync),
            std::bind(&EventsAPI::SetDataConnection, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsSetDataConnectionGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsSetDataConnectionGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsSetDataConnectionGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsSetDataConnectionResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsSetDataConnectionGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsSetDataConnectionResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFEventsSetDataConnectionActiveAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsSetDataConnectionActiveRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsSetDataConnectionActiveAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsSetDataConnectionActiveAsync),
            std::bind(&EventsAPI::SetDataConnectionActive, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsSetDataConnectionActiveGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsSetDataConnectionActiveGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsSetDataConnectionActiveGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsSetDataConnectionActiveResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsSetDataConnectionActiveGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsSetDataConnectionActiveResponse*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFEventsWriteEventsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsWriteEventsAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsWriteEventsAsync),
            std::bind(&EventsAPI::WriteEvents, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsWriteEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsWriteEventsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsWriteEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsWriteEventsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsWriteEventsResponse*>(buffer);

        return S_OK;
    });
}

PF_API PFEventsWriteTelemetryEventsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return EntityAsyncApiImpl(async, XASYNC_IDENTITY(PFEventsWriteTelemetryEventsAsync), contextHandle, [&](SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> context)
    {

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async, 
            XASYNC_IDENTITY(PFEventsWriteTelemetryEventsAsync),
            std::bind(&EventsAPI::WriteTelemetryEvents, std::move(context), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFEventsWriteTelemetryEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsWriteTelemetryEventsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFEventsWriteTelemetryEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFEventsWriteTelemetryEventsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFEventsWriteEventsResponse*>(buffer);

        return S_OK;
    });
}

}
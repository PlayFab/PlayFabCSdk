#include "stdafx.h"
#include <playfab/core/PFEvents.h>
#include "Events.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Events;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFEventsCreateTelemetryKeyAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsCreateTelemetryKeyRequest* request,
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
        XASYNC_IDENTITY(PFEventsCreateTelemetryKeyAsync),
        std::bind(&EventsAPI::CreateTelemetryKey, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsCreateTelemetryKeyGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsCreateTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsCreateTelemetryKeyResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsCreateTelemetryKeyResponse*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFEventsDeleteTelemetryKeyAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsDeleteTelemetryKeyRequest* request,
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
        XASYNC_IDENTITY(PFEventsDeleteTelemetryKeyAsync),
        std::bind(&EventsAPI::DeleteTelemetryKey, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsDeleteTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEventsDeleteTelemetryKeyResponse* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFEventsDeleteTelemetryKeyResponse), result, nullptr);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFEventsGetTelemetryKeyAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsGetTelemetryKeyRequest* request,
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
        XASYNC_IDENTITY(PFEventsGetTelemetryKeyAsync),
        std::bind(&EventsAPI::GetTelemetryKey, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsGetTelemetryKeyGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsGetTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsGetTelemetryKeyResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsGetTelemetryKeyResponse*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFEventsListTelemetryKeysAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsListTelemetryKeysRequest* request,
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
        XASYNC_IDENTITY(PFEventsListTelemetryKeysAsync),
        std::bind(&EventsAPI::ListTelemetryKeys, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsListTelemetryKeysGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsListTelemetryKeysGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsListTelemetryKeysResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsListTelemetryKeysResponse*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
PF_API PFEventsSetTelemetryKeyActiveAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFEventsSetTelemetryKeyActiveRequest* request,
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
        XASYNC_IDENTITY(PFEventsSetTelemetryKeyActiveAsync),
        std::bind(&EventsAPI::SetTelemetryKeyActive, std::move(context), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFEventsSetTelemetryKeyActiveGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFEventsSetTelemetryKeyActiveGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsSetTelemetryKeyActiveResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFEventsSetTelemetryKeyActiveResponse*>(buffer);

    return S_OK;
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


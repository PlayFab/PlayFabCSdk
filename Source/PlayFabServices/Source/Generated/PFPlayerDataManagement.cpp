#include "stdafx.h"
#include <playfab/services/PFPlayerDataManagement.h>
#include "Generated/PlayerDataManagement.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::PlayerDataManagement;

PF_API PFPlayerDataManagementClientGetUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientGetUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientGetUserDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementClientGetUserDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

    return S_OK;
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientGetUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

    return S_OK;
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientGetUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

    return S_OK;
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientGetUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

    return S_OK;
}

PF_API PFPlayerDataManagementClientUpdateUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientUpdateUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientUpdateUserDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}

PF_API PFPlayerDataManagementClientUpdateUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserPublisherDataAsync),
        std::bind(&PlayerDataManagementAPI::ClientUpdateUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementClientUpdateUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}

#if 0
PF_API PFPlayerDataManagementServerGetUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerGetUserInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserInternalDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserInternalDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerGetUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserPublisherDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherInternalDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerGetUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerGetUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFPlayerDataManagementServerGetUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementUpdateUserInternalDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserInternalDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementUpdateUserInternalDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif

#if 0
PF_API PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync),
        std::bind(&PlayerDataManagementAPI::ServerUpdateUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
}
#endif


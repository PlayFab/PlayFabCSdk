#include "stdafx.h"
#include <playfab/services/PFPlayerDataManagement.h>
#include "Generated/PlayerDataManagement.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::PlayerDataManagement;

extern "C"
{

PF_API PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ClientDeletePlayerCustomProperties, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientGetPlayerCustomPropertyAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientGetPlayerCustomPropertyRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientGetPlayerCustomPropertyAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientGetPlayerCustomPropertyAsync),
            std::bind(&PlayerDataManagementAPI::ClientGetPlayerCustomProperty, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientGetPlayerCustomPropertyGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetPlayerCustomPropertyResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetPlayerCustomPropertyGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientGetPlayerCustomPropertyResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientGetUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientGetUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientGetUserDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientGetUserDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientGetUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientGetUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientGetUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientGetUserReadOnlyDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientGetUserDataResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientListPlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ XAsyncBlock* async
) noexcept
{

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientListPlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientListPlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ClientListPlayerCustomProperties, Entity::Duplicate(contextHandle), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementClientListPlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientListPlayerCustomPropertiesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientListPlayerCustomPropertiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementClientListPlayerCustomPropertiesResult*>(buffer);

        return S_OK;
    });
}

PF_API PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ClientUpdatePlayerCustomProperties, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult), result, nullptr);
    });
}

PF_API PFPlayerDataManagementClientUpdateUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientUpdateUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientUpdateUserDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}

PF_API PFPlayerDataManagementClientUpdateUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserPublisherDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserPublisherDataAsync),
            std::bind(&PlayerDataManagementAPI::ClientUpdateUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementClientUpdateUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementClientUpdateUserPublisherDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ServerDeletePlayerCustomProperties, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFPlayerDataManagementServerGetPlayerCustomPropertyAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerGetPlayerCustomPropertyRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetPlayerCustomPropertyAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetPlayerCustomPropertyAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetPlayerCustomProperty, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetPlayerCustomPropertyGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetPlayerCustomPropertyResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetPlayerCustomPropertyGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetPlayerCustomPropertyResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserInternalDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserInternalDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserInternalDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserInternalDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserInternalDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherInternalDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherInternalDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerGetUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerGetUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerGetUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerGetUserReadOnlyDataGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerGetUserDataResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFPlayerDataManagementServerListPlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementListPlayerCustomPropertiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerListPlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerListPlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ServerListPlayerCustomProperties, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerListPlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerListPlayerCustomPropertiesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerListPlayerCustomPropertiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerListPlayerCustomPropertiesResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdatePlayerCustomProperties, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementUpdateUserInternalDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserInternalDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserInternalDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserInternalDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserPublisherDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementUpdateUserInternalDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherInternalData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserPublisherReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayerDataManagementServerUpdateUserDataRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync),
            std::bind(&PlayerDataManagementAPI::ServerUpdateUserReadOnlyData, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayerDataManagementUpdateUserDataResult), result, nullptr);
    });
}
#endif

}
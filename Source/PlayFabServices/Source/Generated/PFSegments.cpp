#include "stdafx.h"
#include <playfab/services/PFSegments.h>
#include "Generated/Segments.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Segments;

extern "C"
{

PF_API PFSegmentsClientGetPlayerSegmentsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ XAsyncBlock* async
) noexcept
{

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsClientGetPlayerSegmentsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsClientGetPlayerSegmentsAsync),
            std::bind(&SegmentsAPI::ClientGetPlayerSegments, Entity::Duplicate(contextHandle), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsClientGetPlayerSegmentsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsClientGetPlayerSegmentsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsClientGetPlayerSegmentsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsClientGetPlayerSegmentsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetPlayerSegmentsResult*>(buffer);

        return S_OK;
    });
}

PF_API PFSegmentsClientGetPlayerTagsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsGetPlayerTagsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsClientGetPlayerTagsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsClientGetPlayerTagsAsync),
            std::bind(&SegmentsAPI::ClientGetPlayerTags, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsClientGetPlayerTagsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsClientGetPlayerTagsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsClientGetPlayerTagsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerTagsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsClientGetPlayerTagsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetPlayerTagsResult*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerAddPlayerTagAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsAddPlayerTagRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerAddPlayerTagAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerAddPlayerTagAsync),
            std::bind(&SegmentsAPI::ServerAddPlayerTag, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerGetAllSegmentsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ XAsyncBlock* async
) noexcept
{

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerGetAllSegmentsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerGetAllSegmentsAsync),
            std::bind(&SegmentsAPI::ServerGetAllSegments, Entity::Duplicate(contextHandle), std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsServerGetAllSegmentsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetAllSegmentsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsServerGetAllSegmentsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetAllSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetAllSegmentsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetAllSegmentsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerGetPlayerSegmentsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsGetPlayersSegmentsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerGetPlayerSegmentsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerGetPlayerSegmentsAsync),
            std::bind(&SegmentsAPI::ServerGetPlayerSegments, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsServerGetPlayerSegmentsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayerSegmentsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsServerGetPlayerSegmentsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayerSegmentsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetPlayerSegmentsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerGetPlayersInSegmentAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsGetPlayersInSegmentRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerGetPlayersInSegmentAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerGetPlayersInSegmentAsync),
            std::bind(&SegmentsAPI::ServerGetPlayersInSegment, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsServerGetPlayersInSegmentGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayersInSegmentGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsServerGetPlayersInSegmentGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayersInSegmentResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayersInSegmentGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetPlayersInSegmentResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerGetPlayerTagsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsGetPlayerTagsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerGetPlayerTagsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerGetPlayerTagsAsync),
            std::bind(&SegmentsAPI::ServerGetPlayerTags, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFSegmentsServerGetPlayerTagsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayerTagsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFSegmentsServerGetPlayerTagsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerTagsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFSegmentsServerGetPlayerTagsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFSegmentsGetPlayerTagsResult*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFSegmentsServerRemovePlayerTagAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFSegmentsRemovePlayerTagRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFSegmentsServerRemovePlayerTagAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFSegmentsServerRemovePlayerTagAsync),
            std::bind(&SegmentsAPI::ServerRemovePlayerTag, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

}
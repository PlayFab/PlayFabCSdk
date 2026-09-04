#include "stdafx.h"
#include <playfab/services/PFPlayStream.h>
#include "Generated/PlayStream.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::PlayStream;

extern "C"
{

#if 0
PF_API PFPlayStreamServerExportPlayersInSegmentAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayStreamExportPlayersInSegmentRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayStreamServerExportPlayersInSegmentAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayStreamServerExportPlayersInSegmentAsync),
            std::bind(&PlayStreamAPI::ServerExportPlayersInSegment, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayStreamServerExportPlayersInSegmentGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayStreamServerExportPlayersInSegmentGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayStreamServerExportPlayersInSegmentGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayStreamExportPlayersInSegmentResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayStreamServerExportPlayersInSegmentGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);
        *result = nullptr;

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayStreamExportPlayersInSegmentResult*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFPlayStreamServerGetSegmentExportAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayStreamGetPlayersInSegmentExportRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayStreamServerGetSegmentExportAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayStreamServerGetSegmentExportAsync),
            std::bind(&PlayStreamAPI::ServerGetSegmentExport, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayStreamServerGetSegmentExportGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayStreamServerGetSegmentExportGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFPlayStreamServerGetSegmentExportGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayStreamGetPlayersInSegmentExportResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayStreamServerGetSegmentExportGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);
        *result = nullptr;

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFPlayStreamGetPlayersInSegmentExportResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFPlayStreamServerGetSegmentPlayerCountAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPlayStreamGetSegmentPlayerCountRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFPlayStreamServerGetSegmentPlayerCountAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFPlayStreamServerGetSegmentPlayerCountAsync),
            std::bind(&PlayStreamAPI::ServerGetSegmentPlayerCount, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFPlayStreamServerGetSegmentPlayerCountGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFPlayStreamGetSegmentPlayerCountResult* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFPlayStreamServerGetSegmentPlayerCountGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFPlayStreamGetSegmentPlayerCountResult), result, nullptr);
    });
}
#endif

}
// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// Public C wrappers for PFMultiplayerServerPingQosServersAsync. Hand-authored counterpart to the
// generated PFFoo.cpp files — follows the same AsyncApiImpl + MakeProvider + ResultApiImpl pattern
// as PFMultiplayerServer.cpp.

#include "stdafx.h"
#include <playfab/services/QoS/PFMultiplayerServerQoS.h>
#include "QoS/QoS.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::QoS;

extern "C"
{

PF_API PFMultiplayerServerPingQosServersAsync(
    _In_ PFEntityHandle entityHandle,
    _In_opt_ const PFMultiplayerServerPingQosServersOptions* options,
    _Inout_ XAsyncBlock* async
) noexcept
{
    return AsyncApiImpl(async, XASYNC_IDENTITY(PFMultiplayerServerPingQosServersAsync), [&](SharedPtr<GlobalState> state)
    {
        PingQosServersOptions cppOpts = PingQosServersOptions::FromC(options);
        Entity entity = Entity::Duplicate(entityHandle);

        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFMultiplayerServerPingQosServersAsync),
            [entity, cppOpts](RunContext rc) -> AsyncOp<PingQosServersResult>
            {
                return QoSAPI::PingQosServers(entity, cppOpts, std::move(rc));
            }
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFMultiplayerServerPingQosServersGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFMultiplayerServerPingQosServersGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFMultiplayerServerPingQosServersGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFMultiplayerServerPingQosServersResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFMultiplayerServerPingQosServersGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFMultiplayerServerPingQosServersResult*>(buffer);

        return S_OK;
    });
}

} // extern "C"

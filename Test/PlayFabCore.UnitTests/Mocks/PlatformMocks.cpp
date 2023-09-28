#include "stdafx.h"
#include "Platform/Platform.h"

namespace PlayFab
{

HRESULT CALLBACK MockProvider(XAsyncOp op, XAsyncProviderData const* data)
{
    if (op == XAsyncOp::Begin)
    {
        XAsyncComplete(data->async, S_OK, 0);
    }
    return S_OK;
}

HRESULT STDAPIVCALLTYPE MockLocalStorageReadAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* /*key*/,
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncBegin(async, nullptr, nullptr, nullptr, MockProvider);
}

HRESULT STDAPIVCALLTYPE MockLocalStorageWriteAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* /*key*/,
    _In_ size_t /*dataSize*/,
    _In_reads_bytes_(dataSize) void const* /*data*/,
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncBegin(async, nullptr, nullptr, nullptr, MockProvider);
}

HRESULT STDAPIVCALLTYPE MockLocalStorageClearAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* /*key*/,
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncBegin(async, nullptr, nullptr, nullptr, MockProvider);
}

HRESULT PlatformInitialize() noexcept
{
    PFLocalStorageHooks storageHooks{ nullptr, MockLocalStorageReadAsync, MockLocalStorageWriteAsync, MockLocalStorageClearAsync, nullptr };
    return SetLocalStorageHandlers(storageHooks);
}

}
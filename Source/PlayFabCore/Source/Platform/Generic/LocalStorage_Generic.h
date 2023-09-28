#pragma once

namespace PlayFab
{
namespace Detail
{

HRESULT STDAPIVCALLTYPE GenericLocalStorageWriteAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _In_ size_t dataSize,
    _In_reads_bytes_(dataSize) void const* data,
    _Inout_ XAsyncBlock* async
) noexcept;

HRESULT STDAPIVCALLTYPE GenericLocalStorageReadAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _Inout_ XAsyncBlock* async
) noexcept;

HRESULT STDAPIVCALLTYPE GenericLocalStorageClearAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _Inout_ XAsyncBlock* async
) noexcept;

}
}

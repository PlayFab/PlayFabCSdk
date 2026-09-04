// XGameSaveEnumAPI.cpp — Implementation wrapping GRTS XGameSaveProviderEnumerator COM interface.

#include "XGameSaveEnumAPI.h"

#include <wrl.h>
#include <winstring.h>
#include <strsafe.h>

#pragma warning(push)
#pragma warning(disable : 4005) // macro redefinitions
#include "GameCoreInternalGrtsInc/gamecore.xgamesave.enumerator.h"
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

struct XGameSaveEnumContext
{
    ComPtr<IXGameSaveProviderEnumerator> Enumerator;
    XGameSaveProviderStatus* CachedProviders;
    uint32_t CachedCount;
    bool ComInitialized;

    XGameSaveEnumContext() : CachedProviders(nullptr), CachedCount(0), ComInitialized(false) {}
    ~XGameSaveEnumContext()
    {
        delete[] CachedProviders;
    }
};

// Copy an HSTRING into a fixed-size wchar_t buffer.
static void CopyHString(HSTRING source, wchar_t* dest, size_t destCount)
{
    const wchar_t* raw = WindowsGetStringRawBuffer(source, nullptr);
    if (raw)
    {
        StringCchCopyW(dest, destCount, raw);
    }
    else
    {
        dest[0] = L'\0';
    }
}

// Convert XGameSaveProviderInfo (HSTRING-based) to XGameSaveProviderStatus (flat C).
static void ConvertProviderInfo(const XGameSaveProviderInfo& info, XGameSaveProviderStatus& out)
{
    memset(&out, 0, sizeof(out));
    CopyHString(info.Xuid, out.Xuid, _countof(out.Xuid));
    CopyHString(info.Scid, out.Scid, _countof(out.Scid));
    CopyHString(info.Aumid, out.Aumid, _countof(out.Aumid));
    CopyHString(info.Location, out.Location, _countof(out.Location));
    out.TotalBytes = info.TotalBytes;
    out.UploadedBytes = info.UploadedBytes;
    out.LastModified = info.LastModified;
    out.LastSyncHr = info.LastSyncHr;
    out.InSync = info.InSync ? 1 : 0;
    out.IsActive = info.IsActive ? 1 : 0;
}

static void CleanupProviderInfo(XGameSaveProviderInfo* info)
{
    WindowsDeleteString(info->Scid);
    WindowsDeleteString(info->Xuid);
    WindowsDeleteString(info->DisplayName);
    WindowsDeleteString(info->Aumid);
    if (info->Location)
    {
        WindowsDeleteString(info->Location);
    }
    WindowsDeleteString(info->LastOwnerChangeId);
}

// Internal: query the enumerator and cache results.
static HRESULT RefreshCache(XGameSaveEnumContext* ctx)
{
    delete[] ctx->CachedProviders;
    ctx->CachedProviders = nullptr;
    ctx->CachedCount = 0;

    uint32_t count = 0;
    HRESULT hr = ctx->Enumerator->GetItemCount(&count);
    if (FAILED(hr))
    {
        return hr;
    }

    if (count == 0)
    {
        return S_OK;
    }

    // Query in batches of MAX_PROVIDERENUMERATOR_GETITEMS_RANGE (50)
    auto* allProviders = new (std::nothrow) XGameSaveProviderStatus[count];
    if (!allProviders)
    {
        return E_OUTOFMEMORY;
    }

    uint32_t totalRead = 0;
    while (totalRead < count)
    {
        uint32_t batchSize = min(count - totalRead, (uint32_t)MAX_PROVIDERENUMERATOR_GETITEMS_RANGE);
        XGameSaveProviderInfo batch[MAX_PROVIDERENUMERATOR_GETITEMS_RANGE];
        uint32_t batchRead = 0;

        hr = ctx->Enumerator->GetItems(totalRead, batchSize, batch, &batchRead);
        if (FAILED(hr))
        {
            for (uint32_t j = 0; j < batchRead; j++)
            {
                CleanupProviderInfo(&batch[j]);
            }
            delete[] allProviders;
            return hr;
        }

        for (uint32_t i = 0; i < batchRead; i++)
        {
            ConvertProviderInfo(batch[i], allProviders[totalRead + i]);
            CleanupProviderInfo(&batch[i]);
        }
        totalRead += batchRead;

        if (batchRead < batchSize)
        {
            break;
        }
    }

    ctx->CachedProviders = allProviders;
    ctx->CachedCount = totalRead;
    return S_OK;
}

static HRESULT CreateHString(const wchar_t* str, HSTRING* hstr)
{
    return WindowsCreateString(str, static_cast<UINT32>(wcslen(str)), hstr);
}

XGSE_API HRESULT XGameSaveEnumCreate(_Out_ XGameSaveEnumHandle* handle)
{
    if (!handle)
    {
        return E_INVALIDARG;
    }
    *handle = nullptr;

    auto ctx = new (std::nothrow) XGameSaveEnumContext();
    if (!ctx)
    {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (SUCCEEDED(hr) || hr == S_FALSE) // S_FALSE means already initialized
    {
        ctx->ComInitialized = (hr == S_OK);
    }
    else
    {
        delete ctx;
        return hr;
    }

    hr = CoCreateInstance(
        __uuidof(::XGameSaveProviderEnumerator),
        nullptr,
        CLSCTX_LOCAL_SERVER,
        IID_PPV_ARGS(&ctx->Enumerator));
    if (FAILED(hr))
    {
        if (ctx->ComInitialized)
        {
            CoUninitialize();
        }
        delete ctx;
        return hr;
    }

    // Initial cache fill
    hr = RefreshCache(ctx);
    if (FAILED(hr))
    {
        ctx->Enumerator = nullptr;
        if (ctx->ComInitialized)
        {
            CoUninitialize();
        }
        delete ctx;
        return hr;
    }

    *handle = ctx;
    return S_OK;
}

XGSE_API void XGameSaveEnumClose(_In_ XGameSaveEnumHandle handle)
{
    if (!handle)
    {
        return;
    }

    auto ctx = handle;
    bool shouldUninit = ctx->ComInitialized;
    ctx->Enumerator = nullptr;
    delete ctx;

    if (shouldUninit)
    {
        CoUninitialize();
    }
}

XGSE_API HRESULT XGameSaveEnumRefresh(_In_ XGameSaveEnumHandle handle)
{
    if (!handle)
    {
        return E_INVALIDARG;
    }
    return RefreshCache(handle);
}

XGSE_API HRESULT XGameSaveEnumGetCount(_In_ XGameSaveEnumHandle handle, _Out_ uint32_t* count)
{
    if (!handle || !count)
    {
        return E_INVALIDARG;
    }
    *count = handle->CachedCount;
    return S_OK;
}

XGSE_API HRESULT XGameSaveEnumGetProviders(
    _In_ XGameSaveEnumHandle handle,
    uint32_t maxCount,
    _Out_writes_to_(maxCount, *actualCount) XGameSaveProviderStatus* providers,
    _Out_ uint32_t* actualCount)
{
    if (!handle || !providers || !actualCount)
    {
        return E_INVALIDARG;
    }

    uint32_t toCopy = min(maxCount, handle->CachedCount);
    memcpy(providers, handle->CachedProviders, toCopy * sizeof(XGameSaveProviderStatus));
    *actualCount = toCopy;
    return S_OK;
}

XGSE_API HRESULT XGameSaveEnumFindProvider(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid,
    _Out_ XGameSaveProviderStatus* status)
{
    if (!handle || !xuid || !scid || !status)
    {
        return E_INVALIDARG;
    }

    for (uint32_t i = 0; i < handle->CachedCount; i++)
    {
        if (_wcsicmp(handle->CachedProviders[i].Xuid, xuid) == 0 &&
            _wcsicmp(handle->CachedProviders[i].Scid, scid) == 0)
        {
            *status = handle->CachedProviders[i];
            return S_OK;
        }
    }

    memset(status, 0, sizeof(*status));
    return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
}

XGSE_API HRESULT XGameSaveEnumDeleteLocalData(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid)
{
    if (!handle || !xuid || !scid)
    {
        return E_INVALIDARG;
    }

    HSTRING hXuid = nullptr;
    HSTRING hScid = nullptr;
    HRESULT hr = CreateHString(xuid, &hXuid);
    if (SUCCEEDED(hr))
    {
        hr = CreateHString(scid, &hScid);
    }
    if (SUCCEEDED(hr))
    {
        hr = handle->Enumerator->DeleteLocalData(hXuid, hScid);
    }
    WindowsDeleteString(hXuid);
    WindowsDeleteString(hScid);
    return hr;
}

XGSE_API HRESULT XGameSaveEnumDeleteLocalAndCloudData(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid)
{
    if (!handle || !xuid || !scid)
    {
        return E_INVALIDARG;
    }

    HSTRING hXuid = nullptr;
    HSTRING hScid = nullptr;
    HRESULT hr = CreateHString(xuid, &hXuid);
    if (SUCCEEDED(hr))
    {
        hr = CreateHString(scid, &hScid);
    }
    if (SUCCEEDED(hr))
    {
        hr = handle->Enumerator->DeleteLocalAndCloudData(hXuid, hScid);
    }
    WindowsDeleteString(hXuid);
    WindowsDeleteString(hScid);
    return hr;
}

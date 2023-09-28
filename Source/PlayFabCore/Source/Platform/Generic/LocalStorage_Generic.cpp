#include "stdafx.h"
#include "LocalStorage_Generic.h"

namespace PlayFab
{
namespace Detail
{

// Synchronous LocalStorage handlers based on cstdio and std::fstream objects
Result<Vector<uint8_t>> GenericLocalStorageRead(String const& path)
{
    std::ifstream file{ path.c_str(), std::ios::binary | std::ios::ate };
    if (!file.is_open())
    {
        // Assume success with no result payload because file does not exist
        return Vector<uint8_t>{};
    }

    int64_t size = file.tellg();
    if (size < 0)
    {
        return E_FAIL;
    }

    file.seekg(0);

    Vector<uint8_t> data(static_cast<size_t>(size));

    file.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));
    if (!file.good())
    {
        return E_FAIL;
    }

    assert(size == file.gcount());
    return data;
}

HRESULT GenericLocalStorageWrite(String const& path, Vector<uint8_t> const& data)
{
    std::ofstream file{ path.data(), std::ios::binary | std::ios::app };
    if (!file.is_open())
    {
        return E_FAIL;
    }

    file.write(reinterpret_cast<char const*>(data.data()), static_cast<std::streamsize>(data.size()));
    if (!file.good())
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT GenericLocalStorageClear(String const& path)
{
    int res = std::remove(path.data());
    if (res != 0)
    {
        // Failing to delete a file that doesn't exist shouldn't be classified
        // as a problem. Unfortunately this result comes back as -1 so to check
        // for it we would have to make a call to explicitly check. Since this
        // seems error prone and unneeded, just swallow the error.
    }

    return S_OK;
}

enum class LocalStorageOperationType
{
    Read,
    Write,
    Clear
};

struct LocalStorageProviderContext
{
    LocalStorageOperationType operationType;
    String path;
    Vector<uint8_t> data;
};


HRESULT CALLBACK LocalStorageOperationAsyncProvider(_In_ XAsyncOp op, _Inout_ const XAsyncProviderData* data) noexcept
{
    auto context = static_cast<LocalStorageProviderContext*>(data->context);

    switch (op)
    {
    case XAsyncOp::Begin:
    {
        return XAsyncSchedule(data->async, 0);
    }
    case XAsyncOp::DoWork:
    try
    {
        switch (context->operationType)
        {
        case LocalStorageOperationType::Read:
        {
            size_t resultSize = 0;
            auto result = GenericLocalStorageRead(context->path);
            if (SUCCEEDED(result.hr))
            {
                context->data = result.ExtractPayload();
                resultSize = context->data.size();
            }
            XAsyncComplete(data->async, result.hr, resultSize);
            break;
        }
        case LocalStorageOperationType::Write:
        {
            XAsyncComplete(data->async, GenericLocalStorageWrite(context->path, context->data), 0);
            break;
        }
        case LocalStorageOperationType::Clear:
        {
            XAsyncComplete(data->async, GenericLocalStorageClear(context->path), 0);
            break;
        }
        default:
        {
            assert(false);
            return E_UNEXPECTED;
        }
        }

        return S_OK;
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
    case XAsyncOp::GetResult:
    {
        assert(context->operationType == LocalStorageOperationType::Read);
        assert(context->data.size() == data->bufferSize);
        memcpy(data->buffer, context->data.data(), data->bufferSize);
        return S_OK;
    }
    case XAsyncOp::Cleanup:
    {
        UniquePtr<LocalStorageProviderContext> reclaim{ context };
        return S_OK;
    }
    default:
    {
        return S_OK;
    }
    }
}

HRESULT STDAPIVCALLTYPE GenericLocalStorageWriteAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _In_ size_t dataSize,
    _In_reads_bytes_(dataSize) void const* data,
    _Inout_ XAsyncBlock* async
) noexcept
{
    assert(key);
    assert(dataSize > 0 && data);

    UniquePtr<LocalStorageProviderContext> providerContext{ new (Allocator<LocalStorageProviderContext>{}.allocate(1)) LocalStorageProviderContext
    {
        LocalStorageOperationType::Write,
        key,
        Vector<uint8_t>((uint8_t*)data, (uint8_t*)data + dataSize)
    } };

    RETURN_IF_FAILED(XAsyncBegin(async, providerContext.get(), nullptr, __FUNCTION__, LocalStorageOperationAsyncProvider));
    providerContext.release();
    return S_OK;
}

HRESULT STDAPIVCALLTYPE GenericLocalStorageReadAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _Inout_ XAsyncBlock* async
) noexcept
{
    assert(key);

    UniquePtr<LocalStorageProviderContext> providerContext{ new (Allocator<LocalStorageProviderContext>{}.allocate(1)) LocalStorageProviderContext
    {
        LocalStorageOperationType::Read,
        key,
        Vector<uint8_t>{}
    } };

    RETURN_IF_FAILED(XAsyncBegin(async, providerContext.get(), nullptr, __FUNCTION__, LocalStorageOperationAsyncProvider));
    providerContext.release();
    return S_OK;
}

HRESULT STDAPIVCALLTYPE GenericLocalStorageClearAsync(
    _In_opt_ void* /*context*/,
    _In_z_ const char* key,
    _Inout_ XAsyncBlock* async
) noexcept
{
    assert(key);

    UniquePtr<LocalStorageProviderContext> providerContext{ new (Allocator<LocalStorageProviderContext>{}.allocate(1)) LocalStorageProviderContext
    {
        LocalStorageOperationType::Clear,
        key,
        Vector<uint8_t>{}
    } };

    RETURN_IF_FAILED(XAsyncBegin(async, providerContext.get(), nullptr, __FUNCTION__, LocalStorageOperationAsyncProvider));
    providerContext.release();
    return S_OK;
}

}
}

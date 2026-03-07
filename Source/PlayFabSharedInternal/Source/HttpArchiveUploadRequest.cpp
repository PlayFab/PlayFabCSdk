#include "pch.h"
#include <regex>
#include "HttpArchiveUploadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"
#include <ApiXAsyncProvider.h>
#include "AsyncOp.h"
#include "XAsyncOperation.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION

namespace PlayFab
{

// function to base64 encode a string from a given string
static String Base64Encode(const String& in)
{
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    String out;
    out.reserve(((in.size() + 2) / 3) * 4);

    size_t i = 0;
    while (i < in.size())
    {
        uint32_t octet_a = i < in.size() ? static_cast<uint8_t>(in[i++]) : 0;
        uint32_t octet_b = i < in.size() ? static_cast<uint8_t>(in[i++]) : 0;
        uint32_t octet_c = i < in.size() ? static_cast<uint8_t>(in[i++]) : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        out.push_back(base64_chars[(triple >> 3 * 6) & 0x3F]);
        out.push_back(base64_chars[(triple >> 2 * 6) & 0x3F]);
        out.push_back(i > in.size() + 1 ? '=' : base64_chars[(triple >> 1 * 6) & 0x3F]);
        out.push_back(i > in.size() ? '=' : base64_chars[(triple >> 0 * 6) & 0x3F]);
    }

    return out;
}

class ArchiveUploadProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    ArchiveUploadProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], String baseUrl, UnorderedMap<String, String> headers, SharedPtr<ArchiveContext> archiveContext, HCHttpCallProgressReportFunction progressCallback, void* progressContext, uint64_t uncompressedTotalSizeBytes, uint64_t uncompressedCurrentSizeBytes) :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_baseUrl{ std::move(baseUrl) },
        m_headers{ std::move(headers) },
        m_archiveContext{ archiveContext },
        m_blockId{ 0 },
        m_progressCallback{ progressCallback },
        m_progressCallbackContext{ progressContext },
        m_uncompressedTotalSizeBytes{ uncompressedTotalSizeBytes },
        m_uncompressedCurrentSizeBytes{ uncompressedCurrentSizeBytes }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);
        TRACE_INFORMATION("ArchiveUploadProvider Begin");
        return Schedule(0);
    }

    HRESULT DoWork(RunContext runContext) override
    {
        TRACE_INFORMATION("ArchiveUploadProvider DoWork");

        String blockContent{};
        size_t uncompressedBytesWritten{};
        bool finishedCompressing{};
        auto hr = m_archiveContext->CompressBytesGetAnyData(blockContent, &uncompressedBytesWritten, &finishedCompressing);

        // If we're waiting for more data, schedule the provider to run again
        if (hr == E_PENDING)
        {
            RETURN_IF_FAILED(Schedule(0));
            return hr;
        }

        if (FAILED(hr))
        {
            TRACE_INFORMATION("ArchiveUploadProvider DoWork HR:0x%0.8x", hr);
            Fail(hr);
            return hr;
        }

        // Done uploading blocks
        if (finishedCompressing && blockContent.empty())
        {
            TRACE_INFORMATION("ArchiveUploadProvider done uploading blocks");
            Complete(sizeof(uint64_t));
            return E_PENDING;
        }

        if (blockContent.empty())
        {
            TRACE_INFORMATION("ArchiveUploadProvider empty");
            Fail(E_FAIL);
            return E_FAIL;
        }

        m_uncompressedCurrentSizeBytes += uncompressedBytesWritten;

        Stringstream fullUrl;
        String blockId = FormatString("%llu", m_blockId++);
        String encodedBlockId = Base64Encode(blockId);
        fullUrl << m_baseUrl << "&comp=block&blockid=" << encodedBlockId;
        String url{ fullUrl.str() };

        auto requestOp = MakeUnique<HCHttpCall>(
            "PUT",
            url,
            m_headers,
            blockContent,
            runContext.Derive()
        );

        requestOp->SetIsPlayfabCall(false);

        // Progress reporting across all requests, not within each one
        m_uncompressedCurrentSizeBytes += m_archiveContext->GetTotalUncompressedSize();
        requestOp->SetProgressReportCallback(true, m_progressCallbackContext, m_progressCallback);
        requestOp->SetDynamicSize(m_uncompressedTotalSizeBytes, m_uncompressedCurrentSizeBytes);
        requestOp->SetHeader("Content-Type", "application/octet-stream");

        TRACE_INFORMATION("ArchiveUploadProvider ServiceResponse Run");
        RunOperation(std::move(requestOp))
        .Finally([&](Result<ServiceResponse> result)
        {
            TRACE_INFORMATION("ArchiveUploadProvider ServiceResponse HR:0x%0.8x HttpSuccess:%d", result.hr, result.Payload().IsHttpSuccess());
            if (SUCCEEDED(result.hr) && result.Payload().IsHttpSuccess())
            {
                // Upload completed successfully. Schedule provider for next upload
                Schedule(0);
            }
            else
            {
                Fail(result.hr);
            }
        });

        return E_PENDING;
    }

    HRESULT GetResult(void* buffer, size_t bufferSize) override
    {
        UNREFERENCED_PARAMETER(bufferSize);
        assert(bufferSize == sizeof(m_blockId));

        auto idBuffer = static_cast<uint64_t*>(buffer);
        *idBuffer = m_blockId;
        return S_OK;
    }

private:
    const String m_baseUrl;
    UnorderedMap<String, String> m_headers;
    SharedPtr<ArchiveContext> m_archiveContext;
    uint64_t m_blockId;
    HCHttpCallProgressReportFunction m_progressCallback{};
    void* m_progressCallbackContext{};
    uint64_t m_uncompressedTotalSizeBytes{};
    uint64_t m_uncompressedCurrentSizeBytes{};
};

HttpArchiveBlockUploadOp::HttpArchiveBlockUploadOp(
    String url,
    UnorderedMap<String, String> headers,
    SharedPtr<ArchiveContext> archiveContext,
    HCHttpCallProgressReportFunction progressCallback,
    void* progressCallbackContext,
    uint64_t uncompressedTotalSizeBytes,
    uint64_t uncompressedCurrentSizeBytes,
    PlayFab::RunContext runContext
) noexcept :
    XAsyncOperation<uint64_t>{ std::move(runContext) },
    m_url{ std::move(url) },
    m_headers{ std::move(headers) },
    m_archiveContext{ archiveContext },
    m_progressCallback{ progressCallback },
    m_progressCallbackContext{ progressCallbackContext },
    m_uncompressedTotalSizeBytes{ uncompressedTotalSizeBytes },
    m_uncompressedCurrentSizeBytes{ uncompressedCurrentSizeBytes }
{
}

HRESULT HttpArchiveBlockUploadOp::OnStarted(XAsyncBlock* async) noexcept
{
    auto provider = MakeUnique<ArchiveUploadProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(HttpArchiveBlockUploadOp), std::move(m_url), std::move(m_headers), m_archiveContext, m_progressCallback, m_progressCallbackContext, m_uncompressedTotalSizeBytes, m_uncompressedCurrentSizeBytes);
    return XAsyncProviderBase::Run(std::move(provider));
}

Result<uint64_t> HttpArchiveBlockUploadOp::GetResult(XAsyncBlock* async) noexcept
{
    RETURN_IF_FAILED(XAsyncGetStatus(async, false));

    uint64_t lastBlockId{};
    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, sizeof(lastBlockId), &lastBlockId, nullptr));

    return lastBlockId;
}

HttpArchiveUploadOp::HttpArchiveUploadOp(
    String url,
    UnorderedMap<String, String> headers,
    SharedPtr<ArchiveContext> archiveContext,
    HCHttpCallProgressReportFunction progressCallback,
    void* progressCallbackContext,
    uint64_t uncompressedTotalSizeBytes,
    uint64_t uncompressedCurrentSizeBytes,
    PlayFab::RunContext runContext
) noexcept :
    m_runContext{ std::move(runContext) },
    m_url{ std::move(url) },
    m_headers{ std::move(headers) },
    m_archiveContext{ archiveContext },
    m_progressCallback{ progressCallback },
    m_progressCallbackContext{ progressCallbackContext },
    m_uncompressedTotalSizeBytes{ uncompressedTotalSizeBytes },
    m_uncompressedCurrentSizeBytes{ uncompressedCurrentSizeBytes }
{
}

AsyncOp<void> HttpArchiveUploadOp::Run() noexcept
{
    TRACE_INFORMATION("[GAME SAVE] HttpArchiveUploadOp::Run");

    auto blockUploadOp = MakeUnique<HttpArchiveBlockUploadOp>(
        m_url,
        m_headers,
        m_archiveContext,
        m_progressCallback,
        m_progressCallbackContext,
        m_uncompressedTotalSizeBytes,
        m_uncompressedCurrentSizeBytes,
        m_runContext
    );

    TRACE_INFORMATION("HttpArchiveUploadOp");
    return RunOperation(std::move(blockUploadOp))
    .Then([baseUrl = m_url, headers = m_headers, runContext = m_runContext, archiveContext = m_archiveContext](Result<uint64_t> result) -> AsyncOp<ServiceResponse>
    {
        TRACE_INFORMATION("HttpArchiveUploadOp HR:0x%0.8x", result.hr);
        RETURN_IF_FAILED(result.hr);
        uint64_t blockId = result.Payload();

        Stringstream fullUrl;
        fullUrl << baseUrl << "&comp=blocklist";
        Stringstream blockList;
        blockList << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<BlockList>\n";
        for (uint64_t i = 0; i < blockId; ++i)
        {
            String blockIdStr = FormatString("%llu", i);
            String encodedBlockId = Base64Encode(blockIdStr);
            blockList << "<Latest>" << encodedBlockId << "</Latest>\n";
        }
        blockList << "</BlockList>";

        String url{ fullUrl.str() };
        String requestBody{ blockList.str() };

        auto requestOp = MakeUnique<HCHttpCall>(
            "PUT",
            url,
            headers,
            requestBody,
            runContext
        );

        requestOp->SetIsPlayfabCall(false);

        requestOp->SetHeader("Content-Type", "application/xml");
        requestOp->SetHeader("Accept", "application/xml");

        archiveContext->Close();

        TRACE_INFORMATION("HCHttpCall Run");
        return RunOperation(std::move(requestOp));
    })
    .Then([](Result<ServiceResponse> result) -> Result<void>
    {
        TRACE_INFORMATION("HCHttpCall HR:0x%0.8x HttpSuccess: %d", result.hr, result.Payload().IsHttpSuccess());
        if (SUCCEEDED(result.hr) && result.Payload().IsHttpSuccess())
        {
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
    });
}

} // namespace PlayFab

#endif
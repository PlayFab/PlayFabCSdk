#pragma once

#include "HttpRequest.h"
#include "Compression.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

namespace PlayFab
{

// Wrapper around HCHttpCallPerformAsync XAsync operation making a PlayFab service call
class HCHttpArchiveDownloadCall : public HCHttpCall
{
public:
    HCHttpArchiveDownloadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        SharedPtr<ArchiveContext> archiveHandle,
        PlayFab::RunContext runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpArchiveDownloadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        SharedPtr<ArchiveContext> archiveHandle,
        uint32_t retryCacheId,
        PFHttpRetrySettings const& retrySettings,
        PlayFab::RunContext runContext,
        PFHttpSettings const& httpSettings,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpArchiveDownloadCall(HCHttpArchiveDownloadCall const&) = delete;
    HCHttpArchiveDownloadCall& operator=(HCHttpArchiveDownloadCall) = delete;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    static HRESULT CALLBACK HCResponseBodyDecompressAndWriteToFile(
        _In_ HCCallHandle callHandle,
        _In_reads_bytes_(bytesAvailable) const uint8_t* source,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context
    );

    SharedPtr<ArchiveContext> m_archiveHandle;
};

}

#endif
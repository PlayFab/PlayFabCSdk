#pragma once

#include "HttpRequest.h"
#include "FilePAL.h"

namespace PlayFab
{

// Wrapper around HCHttpCallPerformAsync XAsync operation making a PlayFab service call
class HCHttpFileDownloadCall : public HCHttpCall
{
public:
    HCHttpFileDownloadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        String responseBodyFilePath,
        PlayFab::RunContext runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpFileDownloadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        String responseBodyFilePath,
        uint32_t retryCacheId,
        PFHttpRetrySettings const& retrySettings,
        PlayFab::RunContext runContext,
        PFHttpSettings const& httpSettings,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpFileDownloadCall(HCHttpFileDownloadCall const&) = delete;
    HCHttpFileDownloadCall& operator=(HCHttpFileDownloadCall) = delete;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServiceResponse> GetResult(XAsyncBlock* async) noexcept override;

    static HRESULT CALLBACK HCResponseBodyWriteToFile(
        _In_ HCCallHandle callHandle,
        _In_reads_bytes_(bytesAvailable) const uint8_t* source,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context
    );

    String m_responseBodyFilePath;
    FileHandle m_responseBodyFileStream;
};

}

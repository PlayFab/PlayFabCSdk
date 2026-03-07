#pragma once

#include "HttpRequest.h"
#include "FilePAL.h"

namespace PlayFab
{

// Wrapper around HCHttpCallPerformAsync XAsync operation making a PlayFab service call
class HCHttpFileUploadCall : public HCHttpCall
{
public:
    HCHttpFileUploadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBodyFilePath,
        PlayFab::RunContext runContext
    ) noexcept;

    HCHttpFileUploadCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBodyFilePath,
        uint32_t retryCacheId,
        PFHttpRetrySettings const& retrySettings,
        PlayFab::RunContext runContext,
        PFHttpSettings const& httpSettings
    ) noexcept;

    HCHttpFileUploadCall(HCHttpFileUploadCall const&) = delete;
    HCHttpFileUploadCall& operator=(HCHttpFileUploadCall) = delete;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServiceResponse> GetResult(XAsyncBlock* async) noexcept override;

    static HRESULT CALLBACK HCRequestBodyReadFromFile(
        _In_ HCCallHandle callHandle,
        _In_ size_t offset,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context,
        _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
        _Out_ size_t* bytesWritten
    );

    String m_requestBodyFilePath;
    FileHandle m_requestBodyFileStream;
};

}

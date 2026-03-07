#include "pch.h"
#include "HttpFileDownloadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

namespace PlayFab
{

HCHttpFileDownloadCall::HCHttpFileDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    String responseBodyFilePath,
    PlayFab::RunContext runContext,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), std::move(runContext), compressionLevel),
    m_responseBodyFilePath{ std::move(responseBodyFilePath) }
{
}

HCHttpFileDownloadCall::HCHttpFileDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    String responseBodyFilePath,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), retryCacheId, retrySettings, std::move(runContext), httpSettings, compressionLevel),
    m_responseBodyFilePath{ std::move(responseBodyFilePath) }
{
}

HRESULT HCHttpFileDownloadCall::OnStarted(XAsyncBlock* async) noexcept
{
    RETURN_IF_FAILED(SetupCall());

    // Override the response body write function to write to a file
    auto fileResult = FilePAL::OpenFile(m_responseBodyFilePath, FileOpenMode::Write);
    RETURN_IF_FAILED(fileResult.hr);
    m_responseBodyFileStream = fileResult.ExtractPayload();

    RETURN_IF_FAILED(HCHttpCallResponseSetResponseBodyWriteFunction(m_callHandle, HCHttpFileDownloadCall::HCResponseBodyWriteToFile, this));

    return HCHttpCallPerformAsync(m_callHandle, async);
}

HRESULT HCHttpFileDownloadCall::HCResponseBodyWriteToFile(
    _In_ HCCallHandle callHandle,
    _In_reads_bytes_(bytesAvailable) const uint8_t* source,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context
)
{
    UNREFERENCED_PARAMETER(callHandle);

    assert(context);
    assert(source);
    assert(bytesAvailable > 0);

    auto call{ static_cast<HCHttpFileDownloadCall*>(context) }; // non-owning
    RETURN_IF_FAILED(FilePAL::WriteFileBytes(call->m_responseBodyFileStream, reinterpret_cast<const char*>(source), bytesAvailable));

    if (call->m_dynamicTotalSize > 0)
    {
        RETURN_IF_FAILED(HCHttpCallResponseAddDynamicBytesWritten(call->m_callHandle, bytesAvailable));
    }

    return S_OK;
}

PlayFab::Result<PlayFab::ServiceResponse> HCHttpFileDownloadCall::GetResult(XAsyncBlock* async) noexcept
{
    FilePAL::CloseFile(m_responseBodyFileStream);
    
    // By design XAsyncBlock should succeed for HttpCall Perform
    RETURN_IF_FAILED(XAsyncGetStatus(async, false));

    // Successful response from service (doesn't always indicate the call was successful, just that the service responded successfully)
    ServiceResponse response{};

    uint32_t callCount{ 1 };
    uint32_t httpCode{ 0 };
    HCHttpCallGetPerformCount(m_callHandle, &callCount);
    RETURN_IF_FAILED(HCHttpCallResponseGetStatusCode(m_callHandle, &httpCode));
    HttpResult httpResult{ callCount - 1, httpCode };
    RETURN_IF_FAILED(HttpStatusToHR(httpCode));

    return response;
}


}

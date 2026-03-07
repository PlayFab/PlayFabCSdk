#include "pch.h"
#include "HttpFileUploadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

namespace PlayFab
{

HCHttpFileUploadCall::HCHttpFileUploadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBodyFilePath,
    PlayFab::RunContext runContext
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), "", std::move(runContext)),
    m_requestBodyFilePath{ std::move(requestBodyFilePath) }
{
}

HCHttpFileUploadCall::HCHttpFileUploadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBodyFilePath,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), "", retryCacheId, retrySettings, std::move(runContext), httpSettings),
    m_requestBodyFilePath{ std::move(requestBodyFilePath) }
{
}

HRESULT HCHttpFileUploadCall::OnStarted(XAsyncBlock* async) noexcept
{
    RETURN_IF_FAILED(SetupCall());

    // Override the request body read function to read from the file
    auto sizeResult = FilePAL::GetFileSize(m_requestBodyFilePath);
    RETURN_IF_FAILED(sizeResult.hr);

    auto result = FilePAL::OpenFile(m_requestBodyFilePath, FileOpenMode::Read);
    RETURN_IF_FAILED(result.hr);
    m_requestBodyFileStream = result.ExtractPayload();

    RETURN_IF_FAILED(HCHttpCallRequestSetRequestBodyReadFunction(m_callHandle, HCHttpFileUploadCall::HCRequestBodyReadFromFile, sizeResult.Payload(), this));

    if (m_retryCacheId.has_value())
    {
        RETURN_IF_FAILED(HCHttpCallRequestSetRetryCacheId(m_callHandle, *m_retryCacheId));
    }

    return HCHttpCallPerformAsync(m_callHandle, async);
}

HRESULT HCHttpFileUploadCall::HCRequestBodyReadFromFile(
    _In_ HCCallHandle callHandle,
    _In_ size_t offset,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context,
    _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
    _Out_ size_t* bytesWritten
)
{
    UNREFERENCED_PARAMETER(callHandle);
    UNREFERENCED_PARAMETER(offset);

    assert(context);
    assert(destination);
    assert(bytesAvailable > 0);
    assert(bytesWritten);

    auto call{ static_cast<HCHttpFileUploadCall*>(context) }; // non-owning

    RETURN_IF_FAILED(FilePAL::ReadFileBytes(call->m_requestBodyFileStream, bytesAvailable, reinterpret_cast<char*>(destination), bytesWritten));

    if (call->m_dynamicTotalSize > 0)
    {
        RETURN_IF_FAILED(HCHttpCallRequestAddDynamicBytesWritten(call->m_callHandle, *bytesWritten));
    }

    return S_OK;
}

PlayFab::Result<PlayFab::ServiceResponse> HCHttpFileUploadCall::GetResult(XAsyncBlock* async) noexcept
{
    FilePAL::CloseFile(m_requestBodyFileStream);

    // By design XAsyncBlock should succeed for HttpCall Perform
    RETURN_IF_FAILED(XAsyncGetStatus(async, false));

    // Successful response from service (doesn't always indicate the call was successful, just that the service responded successfully)
    ServiceResponse response{};

    uint32_t httpCode{ 0 };
    RETURN_IF_FAILED(HCHttpCallResponseGetStatusCode(m_callHandle, &httpCode));
    response.HttpCode = httpCode;
    RETURN_IF_FAILED(HttpStatusToHR(httpCode));

    return response;
}

}

#include "TestAppPch.h"
#include "HttpMock.h"
#include <playfab/httpClient/PFMock.h>

#if _DEBUG
#define ASSERT_SUCCEEDED(hr)                                    do { HRESULT __hrRet = hr; assert(SUCCEEDED(__hrRet)); } while(0, 0)
#else 
#define ASSERT_SUCCEEDED(hr)                                    do { HRESULT __hrRet = hr; UNREFERENCED_PARAMETER(__hrRet); } while(0, 0)
#endif

namespace PlayFab
{
namespace Test
{

HttpMock::HttpMock(const char* method, const char* url, uint32_t statusCode) noexcept
{
    ASSERT_SUCCEEDED(PFHCMockCallCreate(&m_handle));
    ASSERT_SUCCEEDED(PFHCMockAddMock(m_handle, method, url, nullptr, 0));
    ASSERT_SUCCEEDED(PFHCMockResponseSetStatusCode(m_handle, statusCode));
    ASSERT_SUCCEEDED(PFHCMockSetMockMatchedCallback(m_handle, HCMockMatchedCallback, this));
}


HttpMock::~HttpMock()
{
    // avoid race between destruction and callback invocation
    std::unique_lock<std::mutex> lock{ m_mutex };

    ASSERT_SUCCEEDED(PFHCMockRemoveMock(m_handle));
}

void HttpMock::SetResponseHttpStatus(uint32_t httpStatus) const noexcept
{
    ASSERT_SUCCEEDED(PFHCMockResponseSetStatusCode(m_handle, httpStatus));
}

void HttpMock::SetResponseBody(const char* responseBodyString) const noexcept
{
    if (responseBodyString)
    {
        SetResponseBody((uint8_t*)(responseBodyString), std::strlen(responseBodyString) + 1);
    }
}

void HttpMock::SetResponseBody(
    const uint8_t* responseBodyBytes,
    size_t responseBodySize
) const noexcept
{
    if (responseBodyBytes && responseBodySize)
    {
        ASSERT_SUCCEEDED(PFHCMockResponseSetResponseBodyBytes(m_handle, responseBodyBytes, static_cast<uint32_t>(responseBodySize)));
    }
}

void HttpMock::ClearReponseBody() const noexcept
{
    // libHttpClient doesn't allow setting body with nullptr, but it does allow size = 0
    uint8_t body{};
    SetResponseBody(&body, 0);
}

void HttpMock::SetResponseHeaders(const HttpHeaders& responseHeaders) const noexcept
{
    for (const auto& header : responseHeaders)
    {
        ASSERT_SUCCEEDED(PFHCMockResponseSetHeader(m_handle, header.first.data(), header.second.data()));
    }
}

void HttpMock::SetCallback(Callback callback) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    m_callback = std::move(callback);
}

void CALLBACK HttpMock::HCMockMatchedCallback(
    _In_ HCMockCallHandle mockHandle,
    _In_ const char* /*method*/,
    _In_ const char* url,
    _In_ const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize,
    _In_ void* context
) noexcept
{
    UNREFERENCED_PARAMETER(mockHandle);
    auto mock = static_cast<HttpMock*>(context);
    assert(mock->m_handle == mockHandle);

    // avoid race between destruction and callback invocation
    std::unique_lock<std::mutex> lock{ mock->m_mutex };
    ++mock->m_hitCount;
    if (mock->m_callback)
    {
        mock->m_callback(*mock, url, std::string{ requestBodyBytes, requestBodyBytes + requestBodySize }, mock->m_hitCount);
    }
}

}
}

#include "stdafx.h"
#include "HttpMock.h"

namespace PlayFab
{
namespace UnitTests
{

HttpMock::HttpMock(const char* method, const char* url) noexcept
{
    ASSERT_SUCCEEDED(HCMockCallCreate(&m_handle));
    ASSERT_SUCCEEDED(HCMockAddMock(m_handle, method, url, nullptr, 0));
    // Default mock response of 200 with no response headers or body
    ASSERT_SUCCEEDED(HCMockResponseSetStatusCode(m_handle, 200));
    ASSERT_SUCCEEDED(HCMockSetMockMatchedCallback(m_handle, HCMockMatchedCallback, this));
}


HttpMock::~HttpMock()
{
    // avoid race between destruction and callback invocation
    std::unique_lock<std::mutex> lock{ m_mutex };

    ASSERT_SUCCEEDED(HCMockRemoveMock(m_handle));
}

void HttpMock::SetResponseHttpStatus(uint32_t httpStatus) const noexcept
{
    ASSERT_SUCCEEDED(HCMockResponseSetStatusCode(m_handle, httpStatus));
}

void HttpMock::SetResponseBody(const char* responseBodyString) const noexcept
{
    if (responseBodyString)
    {
        SetResponseBody((uint8_t*)(responseBodyString), std::strlen(responseBodyString) + 1);
    }
}

void HttpMock::SetResponseBody(const JsonValue& responseBodyJson) const noexcept
{
    if (!responseBodyJson.is_null())
    {
        SetResponseBody(responseBodyJson.dump());
    }
}

void HttpMock::SetResponseBody(
    const uint8_t* responseBodyBytes,
    size_t responseBodySize
) const noexcept
{
    if (responseBodyBytes && responseBodySize)
    {
        ASSERT_SUCCEEDED(HCMockResponseSetResponseBodyBytes(m_handle, responseBodyBytes, static_cast<uint32_t>(responseBodySize)));
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
        ASSERT_SUCCEEDED(HCMockResponseSetHeader(m_handle, header.first.data(), header.second.data()));
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
    auto mock = static_cast<HttpMock*>(context);
    assert(mock->m_handle == mockHandle);
    UNREFERENCED_PARAMETER(mockHandle);

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

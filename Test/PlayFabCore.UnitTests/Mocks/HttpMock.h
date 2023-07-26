// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <httpClient/httpClient.h>
#include <playfab/core/cpp/ServiceConfig.h> 

namespace PlayFab
{
namespace UnitTests
{

using HttpHeaders = std::map<std::string, std::string>;

// RAII wrapper around HCMockCallHandle
class HttpMock
{
public:
    using Callback = std::function<void(HttpMock const& mock, std::string url, std::string requestBody, uint32_t hitCount)>;

    HttpMock(_In_ const char* httpMethod, _In_ const char* url) noexcept;
    HttpMock(HttpMock const&) = delete;
    HttpMock(HttpMock&&) = delete;
    HttpMock& operator=(HttpMock const&) = delete;
    HttpMock& operator=(HttpMock&&) = delete;
    virtual ~HttpMock();

    void SetResponseHttpStatus(uint32_t httpStatus) const noexcept;
    void SetResponseBody(const char* responseBodyString) const noexcept;
    void SetResponseBody(const rapidjson::Value& responseBodyJson) const noexcept;
    void SetResponseBody(const uint8_t* responseBodyBytes, size_t responseBodySize) const noexcept;
    void ClearReponseBody() const noexcept;
    void SetResponseHeaders(const HttpHeaders& responseHeaders) const noexcept;
    void SetCallback(Callback callback) noexcept;

private:
    static void CALLBACK HCMockMatchedCallback(
        _In_ HCMockCallHandle mockHandle,
        _In_ const char* method,
        _In_ const char* url,
        _In_ const uint8_t* requestBodyBytes,
        _In_ uint32_t requestBodySize,
        _In_ void* context
    ) noexcept;

    std::mutex m_mutex;
    HCMockCallHandle m_handle{ nullptr };
    Callback m_callback;
    uint32_t m_hitCount{ 0 };
};

}
}

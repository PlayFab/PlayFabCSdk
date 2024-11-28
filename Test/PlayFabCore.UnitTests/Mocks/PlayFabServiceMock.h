// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "HttpMock.h"

namespace PlayFab
{
namespace UnitTests
{

// Mocks a PlayFab service API
class PlayFabServiceMock : protected HttpMock
{
public:
    using Callback = std::function<void(PlayFabServiceMock& mock, std::string url, std::string requestBody, uint32_t hitCount)>;

    template<template<typename AllocT> typename Alloc>
    PlayFabServiceMock(Wrappers::ServiceConfig<Alloc> const& serviceConfig, const char* apiName);
    ~PlayFabServiceMock();

    // Response body that will be used when the mock is matched. Will be initialized to a default value if one is configured
    JsonValue& ResponseBody() noexcept;

    // "data" field of a successful service responses. Will cause an assertion if the field isn't present (ex. the response
    // represents a service error
    JsonValue& ResponseBodyPayload() noexcept;

    void SetCallback(Callback callback) noexcept;

    // All configured service responses for this mock API. See MockResponses.json for details.
    JsonValue const& ServiceResponses() const noexcept;

    // General PlayFab Service errors
    static JsonValue const& ServiceErrors() noexcept;

private:
    void HttpMockCallback(HttpMock const& mock, std::string url, std::string requestBody, uint32_t hitCount) noexcept;

    static std::string GetUrl(const char* apiEndpoint, const char* apiName) noexcept;
    static JsonValue const& GetServiceResponses(const char* apiName) noexcept;
    static JsonValue s_serviceResponses;

    std::mutex m_mutex;
    Callback m_callback;
    JsonValue const& m_responses;
    JsonValue m_responseBody;
};

// Template implementations
template<template<typename AllocT> typename Alloc>
PlayFabServiceMock::PlayFabServiceMock(Wrappers::ServiceConfig<Alloc> const& serviceConfig, const char* apiName) :
    HttpMock{ "POST", GetUrl(serviceConfig.APIEndpoint().data(), apiName).data() },
    m_responses{ GetServiceResponses(apiName) }
{
    if (m_responses.contains("default"))
    {
        m_responseBody = m_responses["default"];
    }

    HttpMock::SetCallback(std::bind(&PlayFabServiceMock::HttpMockCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

}
}

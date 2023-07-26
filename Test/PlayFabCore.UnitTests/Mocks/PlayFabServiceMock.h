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
    rapidjson::Value& ResponseBody() noexcept;

    // "data" field of a successful service responses. Will cause an assertion if the field isn't present (ex. the response
    // represents a service error
    rapidjson::Value& ResponseBodyPayload() noexcept;

    void SetCallback(Callback callback) noexcept;

    // All configured service responses for this mock API. See MockResponses.json for details.
    rapidjson::Value const& ServiceResponses() const noexcept;

    // General PlayFab Service errors
    static rapidjson::Value const& ServiceErrors() noexcept;

    // Global allocator to use for rapidjson allocations
    static PlayFab::JsonAllocator& JsonAllocator() noexcept;

private:
    void HttpMockCallback(HttpMock const& mock, std::string url, std::string requestBody, uint32_t hitCount) noexcept;

    static std::string GetUrl(const char* apiEndpoint, const char* apiName) noexcept;
    static rapidjson::Value const& GetServiceResponses(const char* apiName) noexcept;
    static rapidjson::Document s_serviceResponses;

    std::mutex m_mutex;
    Callback m_callback;
    rapidjson::Value const& m_responses;
    rapidjson::Value m_responseBody;
};

// Template implementations
template<template<typename AllocT> typename Alloc>
PlayFabServiceMock::PlayFabServiceMock(Wrappers::ServiceConfig<Alloc> const& serviceConfig, const char* apiName) :
    HttpMock{ "POST", GetUrl(serviceConfig.APIEndpoint().data(), apiName).data() },
    m_responses{ GetServiceResponses(apiName) }
{
    if (m_responses.HasMember("default"))
    {
        m_responseBody.CopyFrom(m_responses["default"], JsonAllocator());
    }

    HttpMock::SetCallback(std::bind(&PlayFabServiceMock::HttpMockCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

}
}

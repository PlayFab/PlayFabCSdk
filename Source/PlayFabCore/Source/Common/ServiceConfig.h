// Configuration of a PlayFab Service connection

#pragma once

#include "HttpClient.h"

namespace PlayFab
{

class ServiceConfig
{
public:
    ServiceConfig(String&& apiEndpoint, String&& titleId, SharedPtr<PFHttpRetrySettings> httpRetrySettings, SharedPtr<PFHttpSettings> httpSettings);

    SharedPtr<HttpClient const> HttpClient() const noexcept;
    String const& APIEndpoint() const noexcept;
    String const& TitleId() const noexcept;

private:
    SharedPtr<PlayFab::HttpClient> m_httpClient;
    String const m_titleId;
};

}

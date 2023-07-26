#include "stdafx.h"
#include "ServiceConfig.h"

namespace PlayFab
{

ServiceConfig::ServiceConfig(String&& apiEndpoint, String&& titleId, SharedPtr<PFHttpRetrySettings> retrySettings)
    : m_httpClient{ MakeShared<PlayFab::HttpClient>(std::move(apiEndpoint), std::move(retrySettings)) },
    m_titleId{ std::move(titleId) }
{
}

SharedPtr<HttpClient const> ServiceConfig::HttpClient() const noexcept
{
    return m_httpClient;
}

String const& ServiceConfig::APIEndpoint() const noexcept
{
    return m_httpClient->APIEndpoint();
}

String const& ServiceConfig::TitleId() const noexcept
{
    return m_titleId;
}

}

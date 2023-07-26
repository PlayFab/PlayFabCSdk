#include "stdafx.h"
#include "JsonUtils.h"
#include "LoginContext.h"

namespace PlayFab
{
namespace Authentication
{

LoginContext::LoginContext(const char* requestPath, CacheId retryCacheId)
    : m_path{ requestPath },
    m_cacheId{ retryCacheId }
{
}

LoginContext::LoginContext(const char* requestPath, JsonValue&& requestBody, CacheId retryCacheId)
    : m_path{ requestPath },
    m_requestBody{ std::move(requestBody) },
    m_cacheId{ retryCacheId }
{
}

LoginContext::LoginContext(const char* requestPath, JsonValue&& requestBody, UnorderedMap<String, String>&& requestHeaders, CacheId retryCacheId)
    : m_path{ requestPath },
    m_requestBody{ std::move(requestBody) },
    m_requestHeaders{ std::move(requestHeaders) },
    m_cacheId{ retryCacheId }
{
}

const char* LoginContext::RequestPath() const
{
    return m_path.data();
}

AsyncOp<JsonValue> LoginContext::GetRequestBody(RunContext runContext) const
{
    UNREFERENCED_PARAMETER(runContext); // No async work to be done in base LoginContext
    JsonValue requestBodyCopy;
    JsonUtils::FromJson(m_requestBody, requestBodyCopy);
    return Result<JsonValue>{ std::move(requestBodyCopy) };
}

const UnorderedMap<String, String>& LoginContext::RequestHeaders() const
{
    return m_requestHeaders;
}

CacheId LoginContext::RetryCacheId() const
{
    return m_cacheId;
}

} // namespace Authentication
} // namespace PlayFab

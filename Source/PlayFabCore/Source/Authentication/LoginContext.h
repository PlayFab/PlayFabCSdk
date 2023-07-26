#pragma once

#include "Generated/CacheId.h"

namespace PlayFab
{
namespace Authentication
{

// Context needed to re-login after auth tokens expire.
// By default, the original auth request will just replayed without modification. For behavior specific to a particular login method, derive
// from this class and override methods as necessary.
class LoginContext
{
public:
    LoginContext(const char* requestPath, CacheId retryCacheId);
    LoginContext(const char* requestPath, JsonValue&& requestBody, CacheId retryCacheId);
    LoginContext(const char* requestPath, JsonValue&& requestBody, UnorderedMap<String, String>&& requestHeaders, CacheId retryCacheId);
    virtual ~LoginContext() = default;

    virtual const char* RequestPath() const;
    virtual AsyncOp<JsonValue> GetRequestBody(RunContext runContext) const;
    virtual const UnorderedMap<String, String>& RequestHeaders() const;
    virtual CacheId RetryCacheId() const;

private:
    String m_path;
    JsonValue m_requestBody;
    UnorderedMap<String, String> m_requestHeaders;
    CacheId m_cacheId;
};

} // namespace Authentication
} // namespace PlayFab

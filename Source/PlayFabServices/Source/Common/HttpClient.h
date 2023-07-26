#pragma once

#include <playfab/httpClient/PFHttpClient.h>
#include <HttpRequest.h>
#include "Generated/CacheId.h"

namespace PlayFab
{

// An Http client for make PlayFab service requests
class ServicesHttpClient
{
public:
    static AsyncOp<ServiceResponse> MakePostRequest(
        ServicesCacheId cacheId,
        String&& endpoint,
        const char* path,
        JsonValue const& requestBody,
        RunContext&& runContext
    );

    static AsyncOp<ServiceResponse> MakeEntityRequest(
        ServicesCacheId cacheId,
        Entity const& entity,
        const char* path,
        JsonValue const& requestBody,
        RunContext&& runContext
    );

    static AsyncOp<ServiceResponse> MakeSecretKeyRequest(
        ServicesCacheId cacheId,
        Entity const& entity,
        const char* path,
        JsonValue const& requestBody,
        RunContext&& runContext
    );

private:
    ServicesHttpClient(); // static only

    static UnorderedMap<String, String> GetDefaultHeaders();
    static String GetUrl(String const& connectionString, const char* path);
};

}

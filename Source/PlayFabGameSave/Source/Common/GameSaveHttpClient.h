#pragma once

namespace PlayFab
{
namespace GameSave
{

// An Http client for make PlayFab service requests
class GameSaveHttpClient
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

    static UnorderedMap<String, String> GetDefaultHeaders();
    static String GetUrl(String const& connectionString, const char* path);
};

} // namespace GameSave
} // namespace PlayFab
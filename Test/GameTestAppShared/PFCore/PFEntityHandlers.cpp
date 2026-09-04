#include "pch.h"
#include "PFEntityHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/core/PFEntity.h>
#include <playfab/core/PFLocalUser.h>
#include "CommandRegistry.h"

static PFRegistrationToken s_tokenExpiredRegistrationToken{ 0 };
static PFRegistrationToken s_tokenRefreshedRegistrationToken{ 0 };

// Helper to obtain a PFEntityHandle from the local user. Caller must close the returned handle.
static HRESULT TryGetEntityHandle(DeviceGameSaveState* state, PFEntityHandle* entityHandle)
{
    if (!state)
    {
        return E_POINTER;
    }
    // Try CustomID entity handle first, then Xbox local user path
    if (state->entityHandle)
    {
        return PFEntityDuplicateHandle(state->entityHandle, entityHandle);
    }
    if (state->localUserHandle)
    {
        return PFLocalUserTryGetEntityHandle(state->localUserHandle, entityHandle);
    }
    return E_POINTER;
}

CommandResultPayload HandlePFEntityDuplicateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            PFEntityHandle duplicatedHandle{ nullptr };
            const HRESULT hr = PFEntityDuplicateHandle(entityHandle, &duplicatedHandle);
            LogToWindowFormat("PFEntityDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                PFEntityCloseHandle(duplicatedHandle);
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetEntityTokenAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            const HRESULT hr = PFEntityGetEntityTokenAsync(entityHandle, &async);
            LogToWindowFormat("PFEntityGetEntityTokenAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFEntityGetEntityTokenResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            const PFEntityToken* entityToken = nullptr;
            RETURN_IF_FAILED(PFEntityGetEntityTokenResult(&async, bufferSize, buffer.data(), &entityToken, nullptr));
            LogToWindowFormat("PFEntityGetEntityTokenAsync result: token=%s, bufferSize=%zu",
                (entityToken && entityToken->token) ? "non-null" : "null", bufferSize);
            if (entityToken)
            {
                payload.result["hasToken"] = (entityToken->token != nullptr);
                payload.result["hasExpiration"] = (entityToken->expiration != nullptr);
            }
            return S_OK;
        });
}

CommandResultPayload HandlePFEntityGetEntityTokenResultSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            const HRESULT hr = PFEntityGetEntityTokenAsync(entityHandle, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            const HRESULT hr = PFEntityGetEntityTokenResultSize(&async, &bufferSize);
            LogToWindowFormat("PFEntityGetEntityTokenResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr))
            {
                payload.result["bufferSize"] = bufferSize;
            }
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetEntityTokenResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            const HRESULT hr = PFEntityGetEntityTokenAsync(entityHandle, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFEntityGetEntityTokenResultSize(&async, &bufferSize));

            std::vector<uint8_t> buffer(bufferSize);
            const PFEntityToken* entityToken = nullptr;
            const HRESULT hr = PFEntityGetEntityTokenResult(&async, bufferSize, buffer.data(), &entityToken, nullptr);
            LogToWindowFormat("PFEntityGetEntityTokenResult (hr=0x%08X, token=%s)", static_cast<uint32_t>(hr),
                (SUCCEEDED(hr) && entityToken && entityToken->token) ? "non-null" : "null");
            if (SUCCEEDED(hr) && entityToken)
            {
                payload.result["hasToken"] = (entityToken->token != nullptr);
                payload.result["hasExpiration"] = (entityToken->expiration != nullptr);
            }
            return hr;
        });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFEntityGetSecretKeySize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t secretKeySize = 0;
            const HRESULT hr = PFEntityGetSecretKeySize(entityHandle, &secretKeySize);
            LogToWindowFormat("PFEntityGetSecretKeySize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), secretKeySize);
            if (SUCCEEDED(hr))
            {
                payload.result["secretKeySize"] = secretKeySize;
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetSecretKey(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t secretKeySize = 0;
            HRESULT hr = PFEntityGetSecretKeySize(entityHandle, &secretKeySize);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEntityGetSecretKey: GetSecretKeySize failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                PFEntityCloseHandle(entityHandle);
                return hr;
            }

            std::vector<char> secretKey(secretKeySize);
            size_t secretKeyUsed = 0;
            hr = PFEntityGetSecretKey(entityHandle, secretKeySize, secretKey.data(), &secretKeyUsed);
            LogToWindowFormat("PFEntityGetSecretKey (hr=0x%08X, used=%zu)", static_cast<uint32_t>(hr), secretKeyUsed);
            if (SUCCEEDED(hr))
            {
                payload.result["secretKey"] = std::string(secretKey.data());
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFEntityGetEntityKeySize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t bufferSize = 0;
            const HRESULT hr = PFEntityGetEntityKeySize(entityHandle, &bufferSize);
            LogToWindowFormat("PFEntityGetEntityKeySize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr))
            {
                payload.result["bufferSize"] = bufferSize;
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetEntityKey(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t bufferSize = 0;
            HRESULT hr = PFEntityGetEntityKeySize(entityHandle, &bufferSize);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEntityGetEntityKey: GetEntityKeySize failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                PFEntityCloseHandle(entityHandle);
                return hr;
            }

            std::vector<uint8_t> buffer(bufferSize);
            const PFEntityKey* entityKey = nullptr;
            hr = PFEntityGetEntityKey(entityHandle, bufferSize, buffer.data(), &entityKey, nullptr);
            LogToWindowFormat("PFEntityGetEntityKey (hr=0x%08X, id=%s, type=%s)", static_cast<uint32_t>(hr),
                (SUCCEEDED(hr) && entityKey && entityKey->id) ? entityKey->id : "null",
                (SUCCEEDED(hr) && entityKey && entityKey->type) ? entityKey->type : "null");
            if (SUCCEEDED(hr) && entityKey)
            {
                payload.result["entityId"] = entityKey->id ? entityKey->id : "";
                payload.result["entityType"] = entityKey->type ? entityKey->type : "";
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityIsTitlePlayer(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            bool isTitlePlayer = false;
            const HRESULT hr = PFEntityIsTitlePlayer(entityHandle, &isTitlePlayer);
            LogToWindowFormat("PFEntityIsTitlePlayer (hr=0x%08X, isTitlePlayer=%s)", static_cast<uint32_t>(hr),
                isTitlePlayer ? "true" : "false");
            if (SUCCEEDED(hr))
            {
                payload.result["isTitlePlayer"] = isTitlePlayer;
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetAPIEndpointSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t apiEndpointSize = 0;
            const HRESULT hr = PFEntityGetAPIEndpointSize(entityHandle, &apiEndpointSize);
            LogToWindowFormat("PFEntityGetAPIEndpointSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), apiEndpointSize);
            if (SUCCEEDED(hr))
            {
                payload.result["apiEndpointSize"] = apiEndpointSize;
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetAPIEndpoint(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t apiEndpointSize = 0;
            HRESULT hr = PFEntityGetAPIEndpointSize(entityHandle, &apiEndpointSize);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEntityGetAPIEndpoint: GetAPIEndpointSize failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                PFEntityCloseHandle(entityHandle);
                return hr;
            }

            std::vector<char> apiEndpoint(apiEndpointSize);
            size_t apiEndpointUsed = 0;
            hr = PFEntityGetAPIEndpoint(entityHandle, apiEndpointSize, apiEndpoint.data(), &apiEndpointUsed);
            LogToWindowFormat("PFEntityGetAPIEndpoint (hr=0x%08X, endpoint=%s)", static_cast<uint32_t>(hr),
                SUCCEEDED(hr) ? apiEndpoint.data() : "null");
            if (SUCCEEDED(hr))
            {
                payload.result["apiEndpoint"] = std::string(apiEndpoint.data());
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetTitleIdSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t titleIdSize = 0;
            const HRESULT hr = PFEntityGetTitleIdSize(entityHandle, &titleIdSize);
            LogToWindowFormat("PFEntityGetTitleIdSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), titleIdSize);
            if (SUCCEEDED(hr))
            {
                payload.result["titleIdSize"] = titleIdSize;
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityGetTitleId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandle(state, &entityHandle));

            size_t titleIdSize = 0;
            HRESULT hr = PFEntityGetTitleIdSize(entityHandle, &titleIdSize);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEntityGetTitleId: GetTitleIdSize failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                PFEntityCloseHandle(entityHandle);
                return hr;
            }

            std::vector<char> titleIdBuffer(titleIdSize);
            size_t titleIdUsed = 0;
            hr = PFEntityGetTitleId(entityHandle, titleIdSize, titleIdBuffer.data(), &titleIdUsed);
            LogToWindowFormat("PFEntityGetTitleId (hr=0x%08X, titleId=%s)", static_cast<uint32_t>(hr),
                SUCCEEDED(hr) ? titleIdBuffer.data() : "null");
            if (SUCCEEDED(hr))
            {
                payload.result["titleId"] = std::string(titleIdBuffer.data());
            }
            PFEntityCloseHandle(entityHandle);
            return hr;
        });
}

CommandResultPayload HandlePFEntityRegisterTokenExpiredEventHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const HRESULT hr = PFEntityRegisterTokenExpiredEventHandler(
                state->taskQueue,
                nullptr,
                [](void* /*context*/, const PFEntityKey* entityKey)
                {
                    LogToWindowFormat("PFEntityTokenExpired event (id=%s, type=%s)",
                        (entityKey && entityKey->id) ? entityKey->id : "null",
                        (entityKey && entityKey->type) ? entityKey->type : "null");
                },
                &s_tokenExpiredRegistrationToken);
            LogToWindowFormat("PFEntityRegisterTokenExpiredEventHandler (hr=0x%08X, token=%llu)",
                static_cast<uint32_t>(hr), static_cast<unsigned long long>(s_tokenExpiredRegistrationToken));
            if (SUCCEEDED(hr))
            {
                payload.result["registrationToken"] = s_tokenExpiredRegistrationToken;
            }
            return hr;
        });
}

CommandResultPayload HandlePFEntityRegisterTokenRefreshedEventHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const HRESULT hr = PFEntityRegisterTokenRefreshedEventHandler(
                state->taskQueue,
                nullptr,
                [](void* /*context*/, const PFEntityKey* entityKey, const PFEntityToken* newToken)
                {
                    LogToWindowFormat("PFEntityTokenRefreshed event (id=%s, type=%s, hasToken=%s)",
                        (entityKey && entityKey->id) ? entityKey->id : "null",
                        (entityKey && entityKey->type) ? entityKey->type : "null",
                        (newToken && newToken->token) ? "true" : "false");
                },
                &s_tokenRefreshedRegistrationToken);
            LogToWindowFormat("PFEntityRegisterTokenRefreshedEventHandler (hr=0x%08X, token=%llu)",
                static_cast<uint32_t>(hr), static_cast<unsigned long long>(s_tokenRefreshedRegistrationToken));
            if (SUCCEEDED(hr))
            {
                payload.result["registrationToken"] = s_tokenRefreshedRegistrationToken;
            }
            return hr;
        });
}

CommandResultPayload HandlePFEntityCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->entityHandle)
            {
                PFEntityCloseHandle(state->entityHandle);
                state->entityHandle = nullptr;
            }
            return S_OK;
        });
}

CommandResultPayload HandlePFEntityUnregisterTokenExpiredEventHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFEntityUnregisterTokenExpiredEventHandler(s_tokenExpiredRegistrationToken);
            LogToWindowFormat("PFEntityUnregisterTokenExpiredEventHandler (token=%llu)",
                static_cast<unsigned long long>(s_tokenExpiredRegistrationToken));
            s_tokenExpiredRegistrationToken = 0;
            return S_OK;
        });
}

CommandResultPayload HandlePFEntityUnregisterTokenRefreshedEventHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFEntityUnregisterTokenRefreshedEventHandler(s_tokenRefreshedRegistrationToken);
            LogToWindowFormat("PFEntityUnregisterTokenRefreshedEventHandler (token=%llu)",
                static_cast<unsigned long long>(s_tokenRefreshedRegistrationToken));
            s_tokenRefreshedRegistrationToken = 0;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFEntityCloseHandle", HandlePFEntityCloseHandle },
    { "PFEntityDuplicateHandle", HandlePFEntityDuplicateHandle },
    { "PFEntityGetAPIEndpoint", HandlePFEntityGetAPIEndpoint },
    { "PFEntityGetAPIEndpointSize", HandlePFEntityGetAPIEndpointSize },
    { "PFEntityGetEntityKey", HandlePFEntityGetEntityKey },
    { "PFEntityGetEntityKeySize", HandlePFEntityGetEntityKeySize },
    { "PFEntityGetEntityTokenAsync", HandlePFEntityGetEntityTokenAsync },
    { "PFEntityGetEntityTokenResult", HandlePFEntityGetEntityTokenResult },
    { "PFEntityGetEntityTokenResultSize", HandlePFEntityGetEntityTokenResultSize },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFEntityGetSecretKey", HandlePFEntityGetSecretKey },
    { "PFEntityGetSecretKeySize", HandlePFEntityGetSecretKeySize },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFEntityGetTitleId", HandlePFEntityGetTitleId },
    { "PFEntityGetTitleIdSize", HandlePFEntityGetTitleIdSize },
    { "PFEntityIsTitlePlayer", HandlePFEntityIsTitlePlayer },
    { "PFEntityRegisterTokenExpiredEventHandler", HandlePFEntityRegisterTokenExpiredEventHandler },
    { "PFEntityRegisterTokenRefreshedEventHandler", HandlePFEntityRegisterTokenRefreshedEventHandler },
    { "PFEntityUnregisterTokenExpiredEventHandler", HandlePFEntityUnregisterTokenExpiredEventHandler },
    { "PFEntityUnregisterTokenRefreshedEventHandler", HandlePFEntityUnregisterTokenRefreshedEventHandler }
});

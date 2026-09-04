#include "pch.h"
#include "PFAuthenticationHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <httpClient/config.h>
#include <playfab/core/PFAuthentication.h>
#include <playfab/core/PFAuthenticationTypes.h>
#include <vector>
#include <playfab/core/PFLocalUser.h>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

// Try CustomID entity handle first, then Xbox local user path
static HRESULT TryGetEntityHandleFromState(DeviceGameSaveState* state, PFEntityHandle* entityHandle)
{
    if (!state) return E_POINTER;
    if (state->entityHandle)
        return PFEntityDuplicateHandle(state->entityHandle, entityHandle);
    if (state->localUserHandle)
        return PFLocalUserTryGetEntityHandle(state->localUserHandle, entityHandle);
    return E_POINTER;
}

CommandResultPayload HandlePFAuthenticationLoginWithAndroidDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithAndroidDeviceIDAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithAndroidDeviceIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithAndroidDeviceIDGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithAndroidDeviceIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithAndroidDeviceIDGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithAndroidDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithAndroidDeviceIDAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithAppleAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
            PFAuthenticationLoginWithAppleRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithAppleAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithAppleAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithAppleGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithAppleGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithApple: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithAppleGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithAppleGetResultSize: called inline by LoginWithApple handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithAppleGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithAppleGetResult: called inline by LoginWithApple handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithAppleAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithAppleRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            const HRESULT hr = PFAuthenticationReLoginWithAppleAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithAppleAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationLoginWithBattleNetAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithBattleNetRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithBattleNetAsync(state->serviceConfigHandle, &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithBattleNetGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithBattleNetGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithBattleNet: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithBattleNetGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithBattleNetGetResultSize: called inline by LoginWithBattleNet handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithBattleNetGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithBattleNetGetResult: called inline by LoginWithBattleNet handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithBattleNetAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithBattleNetRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            const HRESULT hr = PFAuthenticationReLoginWithBattleNetAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFAuthenticationLoginWithCustomIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithCustomIDRequest request{};
            std::string error;
            std::string param_customId;
            if (TryGetStringParameter(parameters, "customId", param_customId, error))
            {
                request.customId = param_customId.c_str();
            }
            else
            {
                request.customId = "test-custom-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithCustomIDAsync(state->serviceConfigHandle, &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithCustomID: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle)
            {
                // Store on state so PFServices handlers can use it
                if (state->entityHandle) { PFEntityCloseHandle(state->entityHandle); }
                state->entityHandle = entityHandle;

                // Extract and store entity key for handlers that need it
                size_t ekSize = 0;
                if (SUCCEEDED(PFEntityGetEntityKeySize(entityHandle, &ekSize)))
                {
                    std::vector<uint8_t> ekBuffer(ekSize);
                    const PFEntityKey* ek = nullptr;
                    if (SUCCEEDED(PFEntityGetEntityKey(entityHandle, ekBuffer.size(), ekBuffer.data(), &ek, nullptr)))
                    {
                        state->entityId = ek->id ? ek->id : "";
                        state->entityType = ek->type ? ek->type : "";
                        LogToWindowFormat("LoginWithCustomID: entityId=%s, entityType=%s", state->entityId.c_str(), state->entityType.c_str());
                    }
                }
            }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithCustomIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithCustomIDGetResultSize: called inline by LoginWithCustomID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithCustomIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithCustomIDGetResult: called inline by LoginWithCustomID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithCustomIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithCustomIDRequest request{};
            std::string error;
            std::string param_customId;
            if (TryGetStringParameter(parameters, "customId", param_customId, error))
            {
                request.customId = param_customId.c_str();
            }
            else
            {
                request.customId = "test-custom-id";
            }
            const HRESULT hr = PFAuthenticationReLoginWithCustomIDAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithEmailAddressAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithEmailAddressAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithEmailAddressGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithEmailAddressGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithEmailAddressGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithEmailAddressGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithEmailAddressAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithEmailAddressAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS
            PFAuthenticationLoginWithFacebookRequest request{};
            std::string error;
            std::string param_accessToken;
            if (TryGetStringParameter(parameters, "accessToken", param_accessToken, error))
            {
                request.accessToken = param_accessToken.c_str();
            }
            else
            {
                request.accessToken = "test-access-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithFacebookAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithFacebookAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithFacebookGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithFacebookGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithFacebook: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithFacebookGetResultSize: called inline by LoginWithFacebook handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithFacebookGetResult: called inline by LoginWithFacebook handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithFacebookAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithFacebookRequest request{};
            std::string error;
            std::string param_accessToken;
            if (TryGetStringParameter(parameters, "accessToken", param_accessToken, error))
            {
                request.accessToken = param_accessToken.c_str();
            }
            else
            {
                request.accessToken = "test-access-token";
            }
            const HRESULT hr = PFAuthenticationReLoginWithFacebookAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithFacebookAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookInstantGamesIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithFacebookInstantGamesIdAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithFacebookInstantGamesIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithFacebookInstantGamesIdGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithFacebookInstantGamesIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithFacebookInstantGamesIdAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGameCenterAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS
            PFAuthenticationLoginWithGameCenterRequest request{};
            std::string error;
            std::string param_playerId;
            if (TryGetStringParameter(parameters, "playerId", param_playerId, error))
            {
                request.playerId = param_playerId.c_str();
            }
            else
            {
                request.playerId = "test-player-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithGameCenterAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithGameCenterAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithGameCenterGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithGameCenterGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithGameCenter: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGameCenterGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGameCenterGetResultSize: called inline by LoginWithGameCenter handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGameCenterGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGameCenterGetResult: called inline by LoginWithGameCenter handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithGameCenterAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_IOS
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithGameCenterRequest request{};
            std::string error;
            std::string param_playerId;
            if (TryGetStringParameter(parameters, "playerId", param_playerId, error))
            {
                request.playerId = param_playerId.c_str();
            }
            else
            {
                request.playerId = "test-player-id";
            }
            const HRESULT hr = PFAuthenticationReLoginWithGameCenterAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithGameCenterAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithGoogleAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            PFAuthenticationLoginWithGoogleAccountRequest request{};
            std::string error;
            std::string param_serverAuthCode;
            if (TryGetStringParameter(parameters, "serverAuthCode", param_serverAuthCode, error))
            {
                request.serverAuthCode = param_serverAuthCode.c_str();
            }
            else
            {
                request.serverAuthCode = "test-auth-code";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithGoogleAccountAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithGoogleAccountAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithGoogleAccountGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithGoogleAccountGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithGoogleAccount: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGoogleAccountGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGoogleAccountGetResultSize: called inline by LoginWithGoogleAccount handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGoogleAccountGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGoogleAccountGetResult: called inline by LoginWithGoogleAccount handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithGoogleAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithGoogleAccountRequest request{};
            std::string error;
            std::string param_serverAuthCode;
            if (TryGetStringParameter(parameters, "serverAuthCode", param_serverAuthCode, error))
            {
                request.serverAuthCode = param_serverAuthCode.c_str();
            }
            else
            {
                request.serverAuthCode = "test-auth-code";
            }
            const HRESULT hr = PFAuthenticationReLoginWithGoogleAccountAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithGoogleAccountAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithGooglePlayGamesServicesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            PFAuthenticationLoginWithGooglePlayGamesServicesRequest request{};
            std::string error;
            std::string param_serverAuthCode;
            if (TryGetStringParameter(parameters, "serverAuthCode", param_serverAuthCode, error))
            {
                request.serverAuthCode = param_serverAuthCode.c_str();
            }
            else
            {
                request.serverAuthCode = "test-auth-code";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithGooglePlayGamesServicesAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithGooglePlayGamesServicesAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithGooglePlayGamesServicesGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithGooglePlayGamesServices: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize: called inline by LoginWithGooglePlayGamesServices handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithGooglePlayGamesServicesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithGooglePlayGamesServicesGetResult: called inline by LoginWithGooglePlayGamesServices handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithGooglePlayGamesServicesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_ANDROID
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithGooglePlayGamesServicesRequest request{};
            std::string error;
            std::string param_serverAuthCode;
            if (TryGetStringParameter(parameters, "serverAuthCode", param_serverAuthCode, error))
            {
                request.serverAuthCode = param_serverAuthCode.c_str();
            }
            else
            {
                request.serverAuthCode = "test-auth-code";
            }
            const HRESULT hr = PFAuthenticationReLoginWithGooglePlayGamesServicesAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithGooglePlayGamesServicesAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithIOSDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithIOSDeviceIDAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithIOSDeviceIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithIOSDeviceIDGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithIOSDeviceIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithIOSDeviceIDGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithIOSDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithIOSDeviceIDAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithKongregateAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithKongregateAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithKongregateGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithKongregateGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithKongregateGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithKongregateGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithKongregateAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithKongregateAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoServiceAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
            PFAuthenticationLoginWithNintendoServiceAccountRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithNintendoServiceAccountAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithNintendoServiceAccountAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithNintendoServiceAccountGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithNintendoServiceAccountGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithNintendoServiceAccount: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoServiceAccountGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithNintendoServiceAccountGetResultSize: called inline by LoginWithNintendoServiceAccount handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoServiceAccountGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithNintendoServiceAccountGetResult: called inline by LoginWithNintendoServiceAccount handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithNintendoServiceAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithNintendoServiceAccountRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            const HRESULT hr = PFAuthenticationReLoginWithNintendoServiceAccountAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithNintendoServiceAccountAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithOpenIdConnectAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithOpenIdConnectRequest request{};
            std::string error;
            std::string param_connectionId;
            if (TryGetStringParameter(parameters, "connectionId", param_connectionId, error))
            {
                request.connectionId = param_connectionId.c_str();
            }
            else
            {
                request.connectionId = "test-connection-id";
            }
            std::string param_idToken;
            if (TryGetStringParameter(parameters, "idToken", param_idToken, error))
            {
                request.idToken = param_idToken.c_str();
            }
            else
            {
                request.idToken = "test-id-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithOpenIdConnectAsync(state->serviceConfigHandle, &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithOpenIdConnectGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithOpenIdConnectGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithOpenIdConnect: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithOpenIdConnectGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithOpenIdConnectGetResultSize: called inline by LoginWithOpenIdConnect handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithOpenIdConnectGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithOpenIdConnectGetResult: called inline by LoginWithOpenIdConnect handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithOpenIdConnectAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithOpenIdConnectRequest request{};
            std::string error;
            std::string param_connectionId;
            if (TryGetStringParameter(parameters, "connectionId", param_connectionId, error))
            {
                request.connectionId = param_connectionId.c_str();
            }
            else
            {
                request.connectionId = "test-connection-id";
            }
            std::string param_idToken;
            if (TryGetStringParameter(parameters, "idToken", param_idToken, error))
            {
                request.idToken = param_idToken.c_str();
            }
            else
            {
                request.idToken = "test-id-token";
            }
            const HRESULT hr = PFAuthenticationReLoginWithOpenIdConnectAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationLoginWithPlayFabAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithPlayFabAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithPlayFabGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithPlayFabGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithPlayFabGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithPlayFabGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithPlayFabAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithPlayFabAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithPSNAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
            PFAuthenticationLoginWithPSNRequest request{};
            std::string error;
            std::string param_authCode;
            if (TryGetStringParameter(parameters, "authCode", param_authCode, error))
            {
                request.authCode = param_authCode.c_str();
            }
            else
            {
                request.authCode = "test-auth-code";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithPSNAsync(state->serviceConfigHandle, &request, &async);
#else
            (void)async;
            LogToWindow("PFAuthenticationLoginWithPSNAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithPSNGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithPSNGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithPSN: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
#else
            (void)async; (void)payload;
            return S_OK;
#endif
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithPSNGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithPSNGetResultSize: called inline by LoginWithPSN handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithPSNGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithPSNGetResult: called inline by LoginWithPSN handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithPSNAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithPSNRequest request{};
            std::string error;
            std::string param_authCode;
            if (TryGetStringParameter(parameters, "authCode", param_authCode, error))
            {
                request.authCode = param_authCode.c_str();
            }
            else
            {
                request.authCode = "test-auth-code";
            }
            const HRESULT hr = PFAuthenticationReLoginWithPSNAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
#else
            (void)async;
            LogToWindow("PFAuthenticationReLoginWithPSNAsync: not available on this platform");
            return E_NOTIMPL;
#endif
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationLoginWithSteamAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithSteamRequest request{};
            std::string error;
            std::string param_steamTicket;
            if (TryGetStringParameter(parameters, "steamTicket", param_steamTicket, error))
            {
                request.steamTicket = param_steamTicket.c_str();
            }
            else
            {
                request.steamTicket = "test-steam-ticket";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            return PFAuthenticationLoginWithSteamAsync(state->serviceConfigHandle, &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationLoginWithSteamGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFEntityHandle entityHandle{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationLoginWithSteamGetResult(&async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("LoginWithSteam: entityHandle=%p, bufferSize=%zu, newlyCreated=%s",
                entityHandle, bufferSize, (loginResult && loginResult->newlyCreated) ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithSteamGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithSteamGetResultSize: called inline by LoginWithSteam handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithSteamGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithSteamGetResult: called inline by LoginWithSteam handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithSteamAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithSteamRequest request{};
            std::string error;
            std::string param_steamTicket;
            if (TryGetStringParameter(parameters, "steamTicket", param_steamTicket, error))
            {
                request.steamTicket = param_steamTicket.c_str();
            }
            else
            {
                request.steamTicket = "test-steam-ticket";
            }
            const HRESULT hr = PFAuthenticationReLoginWithSteamAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFAuthenticationLoginWithTwitchAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithTwitchAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithTwitchGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithTwitchGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithTwitchGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithTwitchGetResult: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithTwitchAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithTwitchAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithXboxAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXboxAsync: not available in this build (use LoginWithXUser instead)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithXboxGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXboxGetResultSize: called inline by LoginWithXbox handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithXboxGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXboxGetResult: called inline by LoginWithXbox handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithXboxAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithXboxAsync: not available in this build (use ReLoginWithXUser instead)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithXUserAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithXUserRequest request{};
            std::string error;
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            request.user = state->xuser;
            return PFAuthenticationLoginWithXUserAsync(state->serviceConfigHandle, &request, &async);
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
#else
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXUserAsync: API only available on GDK");
            return E_NOTIMPL;
        });
#endif
}

CommandResultPayload HandlePFAuthenticationLoginWithXUserGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXUserGetResultSize: called inline by LoginWithXUser handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationLoginWithXUserGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationLoginWithXUserGetResult: called inline by LoginWithXUser handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationReLoginWithXUserAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationLoginWithXUserRequest request{};
            std::string error;
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            request.user = state->xuser;
            const HRESULT hr = PFAuthenticationReLoginWithXUserAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
#else
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationReLoginWithXUserAsync: API only available on GDK");
            return E_NOTIMPL;
        });
#endif
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationServerLoginWithAndroidDeviceIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithAndroidDeviceIDRequest request{};
            std::string error;
            std::string param_androidDeviceId;
            if (TryGetStringParameter(parameters, "androidDeviceId", param_androidDeviceId, error))
            {
                request.androidDeviceId = param_androidDeviceId.c_str();
            }
            else
            {
                request.androidDeviceId = "test-android-device";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithAndroidDeviceIDAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithAndroidDeviceIDGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithAndroidDeviceID: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize: called inline by ServerLoginWithAndroidDeviceID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithAndroidDeviceIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithAndroidDeviceIDGetResult: called inline by ServerLoginWithAndroidDeviceID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithBattleNetAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithBattleNetRequest request{};
            std::string error;
            std::string param_identityToken;
            if (TryGetStringParameter(parameters, "identityToken", param_identityToken, error))
            {
                request.identityToken = param_identityToken.c_str();
            }
            else
            {
                request.identityToken = "test-identity-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithBattleNetAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithBattleNetGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithBattleNetGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithBattleNet: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithBattleNetGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithBattleNetGetResultSize: called inline by ServerLoginWithBattleNet handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithBattleNetGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithBattleNetGetResult: called inline by ServerLoginWithBattleNet handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithCustomIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithCustomIDRequest request{};
            std::string error;
            std::string param_customId;
            if (TryGetStringParameter(parameters, "customId", param_customId, error))
            {
                request.customId = param_customId.c_str();
            }
            else
            {
                request.customId = "test-custom-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithCustomIDAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithCustomIDGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithCustomIDGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithCustomID: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithCustomIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithCustomIDGetResultSize: called inline by ServerLoginWithCustomID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithCustomIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithCustomIDGetResult: called inline by ServerLoginWithCustomID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithIOSDeviceIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithIOSDeviceIDRequest request{};
            std::string error;
            std::string param_deviceId;
            if (TryGetStringParameter(parameters, "deviceId", param_deviceId, error))
            {
                request.deviceId = param_deviceId.c_str();
            }
            else
            {
                request.deviceId = "test-device-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithIOSDeviceIDAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithIOSDeviceIDGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithIOSDeviceID: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithIOSDeviceIDGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize: called inline by ServerLoginWithIOSDeviceID handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithIOSDeviceIDGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithIOSDeviceIDGetResult: called inline by ServerLoginWithIOSDeviceID handler");
            return S_OK;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationServerLoginWithPSNAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithPSNRequest request{};
            std::string error;
            std::string param_authCode;
            if (TryGetStringParameter(parameters, "authCode", param_authCode, error))
            {
                request.authCode = param_authCode.c_str();
            }
            else
            {
                request.authCode = "test-auth-code";
            }
            std::string param_redirectUri;
            if (TryGetStringParameter(parameters, "redirectUri", param_redirectUri, error))
            {
                request.redirectUri = param_redirectUri.c_str();
            }
            else
            {
                request.redirectUri = "test-redirect-uri";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithPSNAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithPSNGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithPSNGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithPSN: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithPSNGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithPSNGetResultSize: called inline by ServerLoginWithPSN handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithPSNGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithPSNGetResult: called inline by ServerLoginWithPSN handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithServerCustomIdAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithServerCustomIdRequest request{};
            std::string error;
            std::string param_serverCustomId;
            if (TryGetStringParameter(parameters, "serverCustomId", param_serverCustomId, error))
            {
                request.serverCustomId = param_serverCustomId.c_str();
            }
            else
            {
                request.serverCustomId = "test-server-custom-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithServerCustomIdAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithServerCustomIdGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithServerCustomIdGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithServerCustomId: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithServerCustomIdGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithServerCustomIdGetResultSize: called inline by ServerLoginWithServerCustomId handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithServerCustomIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithServerCustomIdGetResult: called inline by ServerLoginWithServerCustomId handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithSteamIdAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithSteamIdRequest request{};
            std::string error;
            std::string param_steamId;
            if (TryGetStringParameter(parameters, "steamId", param_steamId, error))
            {
                request.steamId = param_steamId.c_str();
            }
            else
            {
                request.steamId = "test-steam-id";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithSteamIdAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithSteamIdGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithSteamIdGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithSteamId: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithSteamIdGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithSteamIdGetResultSize: called inline by ServerLoginWithSteamId handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithSteamIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithSteamIdGetResult: called inline by ServerLoginWithSteamId handler");
            return S_OK;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFAuthenticationServerLoginWithTwitchAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithTwitchAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithTwitchGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithTwitchGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithTwitchGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithTwitchGetResult: API not available in current GDK");
            return S_OK;
        });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationServerLoginWithXboxAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationServerLoginWithXboxRequest request{};
            std::string error;
            std::string param_xboxToken;
            if (TryGetStringParameter(parameters, "xboxToken", param_xboxToken, error))
            {
                request.xboxToken = param_xboxToken.c_str();
            }
            else
            {
                request.xboxToken = "test-xbox-token";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithXboxAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithXbox: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithXboxGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithXboxGetResultSize: called inline by ServerLoginWithXbox handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithXboxGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithXboxGetResult: called inline by ServerLoginWithXbox handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithXboxIdAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationLoginWithXboxIdRequest request{};
            std::string error;
            std::string param_xboxId;
            if (TryGetStringParameter(parameters, "xboxId", param_xboxId, error))
            {
                request.xboxId = param_xboxId.c_str();
            }
            else
            {
                request.xboxId = "test-xbox-id";
            }
            std::string param_sandbox;
            if (TryGetStringParameter(parameters, "sandbox", param_sandbox, error))
            {
                request.sandbox = param_sandbox.c_str();
            }
            else
            {
                request.sandbox = "RETAIL";
            }
            bool createAccount = true;
            TryParseBoolParameter(parameters, "createAccount", createAccount, error);
            request.createAccount = createAccount;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationServerLoginWithXboxIdAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxIdGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationEntityTokenResponse const* tokenResponse{ nullptr };
            PFAuthenticationLoginResult const* loginResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxIdGetResult(&async, &tokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
            LogToWindowFormat("ServerLoginWithXboxId: tokenResponse=%p, bufferSize=%zu", tokenResponse, bufferSize);
            payload.result["hasTokenResponse"] = tokenResponse != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithXboxIdGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithXboxIdGetResultSize: called inline by ServerLoginWithXboxId handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationServerLoginWithXboxIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationServerLoginWithXboxIdGetResult: called inline by ServerLoginWithXboxId handler");
            return S_OK;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFAuthenticationRegisterPlayFabUserAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationRegisterPlayFabUserAsync: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationRegisterPlayFabUserGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationRegisterPlayFabUserGetResultSize: API not available in current GDK");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationRegisterPlayFabUserGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationRegisterPlayFabUserGetResult: API not available in current GDK");
            return S_OK;
        });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationAuthenticateGameServerWithCustomIdAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationAuthenticateCustomIdRequest request{};
            std::string error;
            std::string param_customId;
            if (TryGetStringParameter(parameters, "customId", param_customId, error))
            {
                request.customId = param_customId.c_str();
            }
            else
            {
                request.customId = "test-custom-id";
            }
            const HRESULT hr = PFAuthenticationAuthenticateGameServerWithCustomIdAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            bool newlyCreated{ false };
            RETURN_IF_FAILED(PFAuthenticationAuthenticateGameServerWithCustomIdGetResult(&async, &entityHandle, &newlyCreated));
            LogToWindowFormat("AuthenticateGameServerWithCustomId: entityHandle=%p, newlyCreated=%s",
                entityHandle, newlyCreated ? "true" : "false");
            payload.result["entityHandle"] = entityHandle != nullptr;
            payload.result["newlyCreated"] = newlyCreated;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationAuthenticateGameServerWithCustomIdGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationAuthenticateGameServerWithCustomIdGetResult: called inline by AuthenticateGameServerWithCustomId handler");
            return S_OK;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFAuthenticationDeleteAsync(
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
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationDeleteRequest request{};
            const HRESULT hr = PFAuthenticationDeleteAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAuthenticationGetEntityAsync(
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
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationGetEntityRequest request{};
            const HRESULT hr = PFAuthenticationGetEntityAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationGetEntityGetResult(&async, &entityHandle));
            LogToWindowFormat("GetEntity: entityHandle=%p", entityHandle);
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationGetEntityGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationGetEntityGetResult: called inline by GetEntity handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationGetEntityWithSecretKeyAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAuthenticationGetEntityRequest request{};
            std::string error;
            std::string secretKey;
            if (!TryGetStringParameter(parameters, "secretKey", secretKey, error))
            {
                secretKey = "test-secret-key";
            }
            return PFAuthenticationGetEntityWithSecretKeyAsync(state->serviceConfigHandle, secretKey.c_str(), &request, &async);
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationGetEntityWithSecretKeyGetResult(&async, &entityHandle));
            LogToWindowFormat("GetEntityWithSecretKey: entityHandle=%p", entityHandle);
            payload.result["entityHandle"] = entityHandle != nullptr;
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationGetEntityWithSecretKeyGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationGetEntityWithSecretKeyGetResult: called inline by GetEntityWithSecretKey handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationValidateEntityTokenAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(TryGetEntityHandleFromState(state, &entityHandle));
            PFAuthenticationValidateEntityTokenRequest request{};
            std::string error;
            std::string param_entityToken;
            if (TryGetStringParameter(parameters, "entityToken", param_entityToken, error))
            {
                request.entityToken = param_entityToken.c_str();
            }
            else
            {
                request.entityToken = "test-entity-token";
            }
            const HRESULT hr = PFAuthenticationValidateEntityTokenAsync(entityHandle, &request, &async);
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFAuthenticationValidateEntityTokenGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            PFAuthenticationValidateEntityTokenResponse* validateResult{ nullptr };
            RETURN_IF_FAILED(PFAuthenticationValidateEntityTokenGetResult(&async, buffer.size(), buffer.data(), &validateResult, nullptr));
            LogToWindowFormat("ValidateEntityToken: result=%p, bufferSize=%zu", validateResult, bufferSize);
            payload.result["hasResult"] = validateResult != nullptr;
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationValidateEntityTokenGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationValidateEntityTokenGetResultSize: called inline by ValidateEntityToken handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAuthenticationValidateEntityTokenGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAuthenticationValidateEntityTokenGetResult: called inline by ValidateEntityToken handler");
            return S_OK;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

// Self-registration of commands
static CommandRegistrar s_registrar({
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationAuthenticateGameServerWithCustomIdAsync", HandlePFAuthenticationAuthenticateGameServerWithCustomIdAsync },
    { "PFAuthenticationAuthenticateGameServerWithCustomIdGetResult", HandlePFAuthenticationAuthenticateGameServerWithCustomIdGetResult },
    { "PFAuthenticationDeleteAsync", HandlePFAuthenticationDeleteAsync },
    { "PFAuthenticationGetEntityAsync", HandlePFAuthenticationGetEntityAsync },
    { "PFAuthenticationGetEntityGetResult", HandlePFAuthenticationGetEntityGetResult },
    { "PFAuthenticationGetEntityWithSecretKeyAsync", HandlePFAuthenticationGetEntityWithSecretKeyAsync },
    { "PFAuthenticationGetEntityWithSecretKeyGetResult", HandlePFAuthenticationGetEntityWithSecretKeyGetResult },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationLoginWithAndroidDeviceIDAsync", HandlePFAuthenticationLoginWithAndroidDeviceIDAsync },
    { "PFAuthenticationLoginWithAndroidDeviceIDGetResult", HandlePFAuthenticationLoginWithAndroidDeviceIDGetResult },
    { "PFAuthenticationLoginWithAndroidDeviceIDGetResultSize", HandlePFAuthenticationLoginWithAndroidDeviceIDGetResultSize },
    { "PFAuthenticationLoginWithAppleAsync", HandlePFAuthenticationLoginWithAppleAsync },
    { "PFAuthenticationLoginWithAppleGetResult", HandlePFAuthenticationLoginWithAppleGetResult },
    { "PFAuthenticationLoginWithAppleGetResultSize", HandlePFAuthenticationLoginWithAppleGetResultSize },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationLoginWithBattleNetAsync", HandlePFAuthenticationLoginWithBattleNetAsync },
    { "PFAuthenticationLoginWithBattleNetGetResult", HandlePFAuthenticationLoginWithBattleNetGetResult },
    { "PFAuthenticationLoginWithBattleNetGetResultSize", HandlePFAuthenticationLoginWithBattleNetGetResultSize },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationLoginWithCustomIDAsync", HandlePFAuthenticationLoginWithCustomIDAsync },
    { "PFAuthenticationLoginWithCustomIDGetResult", HandlePFAuthenticationLoginWithCustomIDGetResult },
    { "PFAuthenticationLoginWithCustomIDGetResultSize", HandlePFAuthenticationLoginWithCustomIDGetResultSize },
    { "PFAuthenticationLoginWithEmailAddressAsync", HandlePFAuthenticationLoginWithEmailAddressAsync },
    { "PFAuthenticationLoginWithEmailAddressGetResult", HandlePFAuthenticationLoginWithEmailAddressGetResult },
    { "PFAuthenticationLoginWithEmailAddressGetResultSize", HandlePFAuthenticationLoginWithEmailAddressGetResultSize },
    { "PFAuthenticationLoginWithFacebookAsync", HandlePFAuthenticationLoginWithFacebookAsync },
    { "PFAuthenticationLoginWithFacebookGetResult", HandlePFAuthenticationLoginWithFacebookGetResult },
    { "PFAuthenticationLoginWithFacebookGetResultSize", HandlePFAuthenticationLoginWithFacebookGetResultSize },
    { "PFAuthenticationLoginWithFacebookInstantGamesIdAsync", HandlePFAuthenticationLoginWithFacebookInstantGamesIdAsync },
    { "PFAuthenticationLoginWithFacebookInstantGamesIdGetResult", HandlePFAuthenticationLoginWithFacebookInstantGamesIdGetResult },
    { "PFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize", HandlePFAuthenticationLoginWithFacebookInstantGamesIdGetResultSize },
    { "PFAuthenticationLoginWithGameCenterAsync", HandlePFAuthenticationLoginWithGameCenterAsync },
    { "PFAuthenticationLoginWithGameCenterGetResult", HandlePFAuthenticationLoginWithGameCenterGetResult },
    { "PFAuthenticationLoginWithGameCenterGetResultSize", HandlePFAuthenticationLoginWithGameCenterGetResultSize },
    { "PFAuthenticationLoginWithGoogleAccountAsync", HandlePFAuthenticationLoginWithGoogleAccountAsync },
    { "PFAuthenticationLoginWithGoogleAccountGetResult", HandlePFAuthenticationLoginWithGoogleAccountGetResult },
    { "PFAuthenticationLoginWithGoogleAccountGetResultSize", HandlePFAuthenticationLoginWithGoogleAccountGetResultSize },
    { "PFAuthenticationLoginWithGooglePlayGamesServicesAsync", HandlePFAuthenticationLoginWithGooglePlayGamesServicesAsync },
    { "PFAuthenticationLoginWithGooglePlayGamesServicesGetResult", HandlePFAuthenticationLoginWithGooglePlayGamesServicesGetResult },
    { "PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize", HandlePFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize },
    { "PFAuthenticationLoginWithIOSDeviceIDAsync", HandlePFAuthenticationLoginWithIOSDeviceIDAsync },
    { "PFAuthenticationLoginWithIOSDeviceIDGetResult", HandlePFAuthenticationLoginWithIOSDeviceIDGetResult },
    { "PFAuthenticationLoginWithIOSDeviceIDGetResultSize", HandlePFAuthenticationLoginWithIOSDeviceIDGetResultSize },
    { "PFAuthenticationLoginWithKongregateAsync", HandlePFAuthenticationLoginWithKongregateAsync },
    { "PFAuthenticationLoginWithKongregateGetResult", HandlePFAuthenticationLoginWithKongregateGetResult },
    { "PFAuthenticationLoginWithKongregateGetResultSize", HandlePFAuthenticationLoginWithKongregateGetResultSize },
    { "PFAuthenticationLoginWithNintendoServiceAccountAsync", HandlePFAuthenticationLoginWithNintendoServiceAccountAsync },
    { "PFAuthenticationLoginWithNintendoServiceAccountGetResult", HandlePFAuthenticationLoginWithNintendoServiceAccountGetResult },
    { "PFAuthenticationLoginWithNintendoServiceAccountGetResultSize", HandlePFAuthenticationLoginWithNintendoServiceAccountGetResultSize },
    { "PFAuthenticationLoginWithNintendoSwitchDeviceIdAsync", HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdAsync },
    { "PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult", HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdGetResult },
    { "PFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize", HandlePFAuthenticationLoginWithNintendoSwitchDeviceIdGetResultSize },
    { "PFAuthenticationLoginWithOpenIdConnectAsync", HandlePFAuthenticationLoginWithOpenIdConnectAsync },
    { "PFAuthenticationLoginWithOpenIdConnectGetResult", HandlePFAuthenticationLoginWithOpenIdConnectGetResult },
    { "PFAuthenticationLoginWithOpenIdConnectGetResultSize", HandlePFAuthenticationLoginWithOpenIdConnectGetResultSize },
    { "PFAuthenticationLoginWithPSNAsync", HandlePFAuthenticationLoginWithPSNAsync },
    { "PFAuthenticationLoginWithPSNGetResult", HandlePFAuthenticationLoginWithPSNGetResult },
    { "PFAuthenticationLoginWithPSNGetResultSize", HandlePFAuthenticationLoginWithPSNGetResultSize },
    { "PFAuthenticationLoginWithPlayFabAsync", HandlePFAuthenticationLoginWithPlayFabAsync },
    { "PFAuthenticationLoginWithPlayFabGetResult", HandlePFAuthenticationLoginWithPlayFabGetResult },
    { "PFAuthenticationLoginWithPlayFabGetResultSize", HandlePFAuthenticationLoginWithPlayFabGetResultSize },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationLoginWithSteamAsync", HandlePFAuthenticationLoginWithSteamAsync },
    { "PFAuthenticationLoginWithSteamGetResult", HandlePFAuthenticationLoginWithSteamGetResult },
    { "PFAuthenticationLoginWithSteamGetResultSize", HandlePFAuthenticationLoginWithSteamGetResultSize },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationLoginWithTwitchAsync", HandlePFAuthenticationLoginWithTwitchAsync },
    { "PFAuthenticationLoginWithTwitchGetResult", HandlePFAuthenticationLoginWithTwitchGetResult },
    { "PFAuthenticationLoginWithTwitchGetResultSize", HandlePFAuthenticationLoginWithTwitchGetResultSize },
    { "PFAuthenticationLoginWithXUserAsync", HandlePFAuthenticationLoginWithXUserAsync },
    { "PFAuthenticationLoginWithXUserGetResult", HandlePFAuthenticationLoginWithXUserGetResult },
    { "PFAuthenticationLoginWithXUserGetResultSize", HandlePFAuthenticationLoginWithXUserGetResultSize },
    { "PFAuthenticationLoginWithXboxAsync", HandlePFAuthenticationLoginWithXboxAsync },
    { "PFAuthenticationLoginWithXboxGetResult", HandlePFAuthenticationLoginWithXboxGetResult },
    { "PFAuthenticationLoginWithXboxGetResultSize", HandlePFAuthenticationLoginWithXboxGetResultSize },
    { "PFAuthenticationReLoginWithAndroidDeviceIDAsync", HandlePFAuthenticationReLoginWithAndroidDeviceIDAsync },
    { "PFAuthenticationReLoginWithAppleAsync", HandlePFAuthenticationReLoginWithAppleAsync },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationReLoginWithBattleNetAsync", HandlePFAuthenticationReLoginWithBattleNetAsync },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationReLoginWithCustomIDAsync", HandlePFAuthenticationReLoginWithCustomIDAsync },
    { "PFAuthenticationReLoginWithEmailAddressAsync", HandlePFAuthenticationReLoginWithEmailAddressAsync },
    { "PFAuthenticationReLoginWithFacebookAsync", HandlePFAuthenticationReLoginWithFacebookAsync },
    { "PFAuthenticationReLoginWithFacebookInstantGamesIdAsync", HandlePFAuthenticationReLoginWithFacebookInstantGamesIdAsync },
    { "PFAuthenticationReLoginWithGameCenterAsync", HandlePFAuthenticationReLoginWithGameCenterAsync },
    { "PFAuthenticationReLoginWithGoogleAccountAsync", HandlePFAuthenticationReLoginWithGoogleAccountAsync },
    { "PFAuthenticationReLoginWithGooglePlayGamesServicesAsync", HandlePFAuthenticationReLoginWithGooglePlayGamesServicesAsync },
    { "PFAuthenticationReLoginWithIOSDeviceIDAsync", HandlePFAuthenticationReLoginWithIOSDeviceIDAsync },
    { "PFAuthenticationReLoginWithKongregateAsync", HandlePFAuthenticationReLoginWithKongregateAsync },
    { "PFAuthenticationReLoginWithNintendoServiceAccountAsync", HandlePFAuthenticationReLoginWithNintendoServiceAccountAsync },
    { "PFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync", HandlePFAuthenticationReLoginWithNintendoSwitchDeviceIdAsync },
    { "PFAuthenticationReLoginWithOpenIdConnectAsync", HandlePFAuthenticationReLoginWithOpenIdConnectAsync },
    { "PFAuthenticationReLoginWithPSNAsync", HandlePFAuthenticationReLoginWithPSNAsync },
    { "PFAuthenticationReLoginWithPlayFabAsync", HandlePFAuthenticationReLoginWithPlayFabAsync },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationReLoginWithSteamAsync", HandlePFAuthenticationReLoginWithSteamAsync },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationReLoginWithTwitchAsync", HandlePFAuthenticationReLoginWithTwitchAsync },
    { "PFAuthenticationReLoginWithXUserAsync", HandlePFAuthenticationReLoginWithXUserAsync },
    { "PFAuthenticationReLoginWithXboxAsync", HandlePFAuthenticationReLoginWithXboxAsync },
    { "PFAuthenticationRegisterPlayFabUserAsync", HandlePFAuthenticationRegisterPlayFabUserAsync },
    { "PFAuthenticationRegisterPlayFabUserGetResult", HandlePFAuthenticationRegisterPlayFabUserGetResult },
    { "PFAuthenticationRegisterPlayFabUserGetResultSize", HandlePFAuthenticationRegisterPlayFabUserGetResultSize },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationServerLoginWithAndroidDeviceIDAsync", HandlePFAuthenticationServerLoginWithAndroidDeviceIDAsync },
    { "PFAuthenticationServerLoginWithAndroidDeviceIDGetResult", HandlePFAuthenticationServerLoginWithAndroidDeviceIDGetResult },
    { "PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize", HandlePFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize },
    { "PFAuthenticationServerLoginWithBattleNetAsync", HandlePFAuthenticationServerLoginWithBattleNetAsync },
    { "PFAuthenticationServerLoginWithBattleNetGetResult", HandlePFAuthenticationServerLoginWithBattleNetGetResult },
    { "PFAuthenticationServerLoginWithBattleNetGetResultSize", HandlePFAuthenticationServerLoginWithBattleNetGetResultSize },
    { "PFAuthenticationServerLoginWithCustomIDAsync", HandlePFAuthenticationServerLoginWithCustomIDAsync },
    { "PFAuthenticationServerLoginWithCustomIDGetResult", HandlePFAuthenticationServerLoginWithCustomIDGetResult },
    { "PFAuthenticationServerLoginWithCustomIDGetResultSize", HandlePFAuthenticationServerLoginWithCustomIDGetResultSize },
    { "PFAuthenticationServerLoginWithIOSDeviceIDAsync", HandlePFAuthenticationServerLoginWithIOSDeviceIDAsync },
    { "PFAuthenticationServerLoginWithIOSDeviceIDGetResult", HandlePFAuthenticationServerLoginWithIOSDeviceIDGetResult },
    { "PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize", HandlePFAuthenticationServerLoginWithIOSDeviceIDGetResultSize },
    { "PFAuthenticationServerLoginWithPSNAsync", HandlePFAuthenticationServerLoginWithPSNAsync },
    { "PFAuthenticationServerLoginWithPSNGetResult", HandlePFAuthenticationServerLoginWithPSNGetResult },
    { "PFAuthenticationServerLoginWithPSNGetResultSize", HandlePFAuthenticationServerLoginWithPSNGetResultSize },
    { "PFAuthenticationServerLoginWithServerCustomIdAsync", HandlePFAuthenticationServerLoginWithServerCustomIdAsync },
    { "PFAuthenticationServerLoginWithServerCustomIdGetResult", HandlePFAuthenticationServerLoginWithServerCustomIdGetResult },
    { "PFAuthenticationServerLoginWithServerCustomIdGetResultSize", HandlePFAuthenticationServerLoginWithServerCustomIdGetResultSize },
    { "PFAuthenticationServerLoginWithSteamIdAsync", HandlePFAuthenticationServerLoginWithSteamIdAsync },
    { "PFAuthenticationServerLoginWithSteamIdGetResult", HandlePFAuthenticationServerLoginWithSteamIdGetResult },
    { "PFAuthenticationServerLoginWithSteamIdGetResultSize", HandlePFAuthenticationServerLoginWithSteamIdGetResultSize },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationServerLoginWithTwitchAsync", HandlePFAuthenticationServerLoginWithTwitchAsync },
    { "PFAuthenticationServerLoginWithTwitchGetResult", HandlePFAuthenticationServerLoginWithTwitchGetResult },
    { "PFAuthenticationServerLoginWithTwitchGetResultSize", HandlePFAuthenticationServerLoginWithTwitchGetResultSize },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFAuthenticationServerLoginWithXboxAsync", HandlePFAuthenticationServerLoginWithXboxAsync },
    { "PFAuthenticationServerLoginWithXboxGetResult", HandlePFAuthenticationServerLoginWithXboxGetResult },
    { "PFAuthenticationServerLoginWithXboxGetResultSize", HandlePFAuthenticationServerLoginWithXboxGetResultSize },
    { "PFAuthenticationServerLoginWithXboxIdAsync", HandlePFAuthenticationServerLoginWithXboxIdAsync },
    { "PFAuthenticationServerLoginWithXboxIdGetResult", HandlePFAuthenticationServerLoginWithXboxIdGetResult },
    { "PFAuthenticationServerLoginWithXboxIdGetResultSize", HandlePFAuthenticationServerLoginWithXboxIdGetResultSize },
    { "PFAuthenticationValidateEntityTokenAsync", HandlePFAuthenticationValidateEntityTokenAsync },
    { "PFAuthenticationValidateEntityTokenGetResult", HandlePFAuthenticationValidateEntityTokenGetResult },
    { "PFAuthenticationValidateEntityTokenGetResultSize", HandlePFAuthenticationValidateEntityTokenGetResultSize }
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
});

#include "pch.h"

#include "XblPrivacyHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_privacyBuffer;

CommandResultPayload HandleXblPrivacyGetAvoidListAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            HRESULT hr = XblPrivacyGetAvoidListAsync(state->xblContext, &async);
            LogToWindowFormat("XblPrivacyGetAvoidListAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t xuidCount{};
            HRESULT hr = XblPrivacyGetAvoidListResultCount(&async, &xuidCount);
            if (SUCCEEDED(hr))
            {
                std::vector<uint64_t> xuids(xuidCount);
                hr = XblPrivacyGetAvoidListResult(&async, xuidCount, xuids.data());
                if (SUCCEEDED(hr))
                {
                    payload.result["xuidCount"] = xuidCount;
                    nlohmann::json xuidsJson = nlohmann::json::array();
                    for (size_t i = 0; i < xuidCount; ++i)
                    {
                        xuidsJson.push_back(xuids[i]);
                    }
                    payload.result["xuids"] = xuidsJson;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblPrivacyGetMuteListAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            HRESULT hr = XblPrivacyGetMuteListAsync(state->xblContext, &async);
            LogToWindowFormat("XblPrivacyGetMuteListAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t xuidCount{};
            HRESULT hr = XblPrivacyGetMuteListResultCount(&async, &xuidCount);
            if (SUCCEEDED(hr))
            {
                std::vector<uint64_t> xuids(xuidCount);
                hr = XblPrivacyGetMuteListResult(&async, xuidCount, xuids.data());
                if (SUCCEEDED(hr))
                {
                    payload.result["xuidCount"] = xuidCount;
                    nlohmann::json xuidsJson = nlohmann::json::array();
                    for (size_t i = 0; i < xuidCount; ++i)
                    {
                        xuidsJson.push_back(xuids[i]);
                    }
                    payload.result["xuids"] = xuidsJson;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            int64_t permissionToCheck{};
            int64_t targetXuid{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "permissionToCheck", permissionToCheck, error))
            {
                permissionToCheck = 5; // XblPermission::ViewTargetProfile
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "targetXuid", targetXuid, error))
            {
                targetXuid = 2743710844428572LL; // Default test XUID
            }

            HRESULT hr = XblPrivacyCheckPermissionAsync(
                state->xblContext,
                static_cast<XblPermission>(permissionToCheck),
                static_cast<uint64_t>(targetXuid),
                &async);
            LogToWindowFormat("XblPrivacyCheckPermissionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblPrivacyCheckPermissionResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_privacyBuffer.resize(resultSize);
                XblPermissionCheckResult* result{};
                hr = XblPrivacyCheckPermissionResult(&async, resultSize, s_privacyBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["isAllowed"] = result->isAllowed;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            int64_t permissionToCheck{};
            int64_t userType{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "permissionToCheck", permissionToCheck, error))
            {
                permissionToCheck = 1; // XblPermission::CommunicateUsingText
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "userType", userType, error))
            {
                userType = 1; // XblAnonymousUserType::CrossNetworkUser
            }

            HRESULT hr = XblPrivacyCheckPermissionForAnonymousUserAsync(
                state->xblContext,
                static_cast<XblPermission>(permissionToCheck),
                static_cast<XblAnonymousUserType>(userType),
                &async);
            LogToWindowFormat("XblPrivacyCheckPermissionForAnonymousUserAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblPrivacyCheckPermissionForAnonymousUserResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_privacyBuffer.resize(resultSize);
                XblPermissionCheckResult* result{};
                hr = XblPrivacyCheckPermissionForAnonymousUserResult(&async, resultSize, s_privacyBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["isAllowed"] = result->isAllowed;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblPrivacyBatchCheckPermissionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            auto permissionsToCheck = CommandHandlerShared::GetUint64Array(parameters, "permissionsToCheck");
            auto targetXuids = CommandHandlerShared::GetUint64Array(parameters, "targetXuids");
            auto targetAnonymousUserTypes = CommandHandlerShared::GetUint64Array(parameters, "targetAnonymousUserTypes");

            // API hangs with empty arrays — fail fast
            RETURN_HR_IF(E_INVALIDARG, permissionsToCheck.empty());
            RETURN_HR_IF(E_INVALIDARG, targetXuids.empty() && targetAnonymousUserTypes.empty());

            std::vector<XblPermission> permissions(permissionsToCheck.size());
            for (size_t i = 0; i < permissionsToCheck.size(); ++i)
            {
                permissions[i] = static_cast<XblPermission>(permissionsToCheck[i]);
            }

            std::vector<XblAnonymousUserType> userTypes(targetAnonymousUserTypes.size());
            for (size_t i = 0; i < targetAnonymousUserTypes.size(); ++i)
            {
                userTypes[i] = static_cast<XblAnonymousUserType>(targetAnonymousUserTypes[i]);
            }

            HRESULT hr = XblPrivacyBatchCheckPermissionAsync(
                state->xblContext,
                permissions.data(),
                permissions.size(),
                targetXuids.data(),
                targetXuids.size(),
                userTypes.data(),
                userTypes.size(),
                &async);
            LogToWindowFormat("XblPrivacyBatchCheckPermissionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblPrivacyBatchCheckPermissionResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_privacyBuffer.resize(resultSize);
                XblPermissionCheckResult* results{};
                size_t resultCount{};
                hr = XblPrivacyBatchCheckPermissionResult(&async, resultSize, s_privacyBuffer.data(), &results, &resultCount, nullptr);
                if (SUCCEEDED(hr))
                {
                    payload.result["resultCount"] = resultCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblPrivacyGetAvoidListResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyGetAvoidListResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyGetAvoidListResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyGetAvoidListResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyCheckPermissionResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyCheckPermissionResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyCheckPermissionForAnonymousUserResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyCheckPermissionForAnonymousUserResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyBatchCheckPermissionResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyBatchCheckPermissionResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyBatchCheckPermissionResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyBatchCheckPermissionResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyGetMuteListResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyGetMuteListResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPrivacyGetMuteListResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPrivacyGetMuteListResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblPrivacyBatchCheckPermissionAsync", HandleXblPrivacyBatchCheckPermissionAsync },
    { "XblPrivacyBatchCheckPermissionResult", HandleXblPrivacyBatchCheckPermissionResult },
    { "XblPrivacyBatchCheckPermissionResultSize", HandleXblPrivacyBatchCheckPermissionResultSize },
    { "XblPrivacyCheckPermissionAsync", HandleXblPrivacyCheckPermissionAsync },
    { "XblPrivacyCheckPermissionForAnonymousUserAsync", HandleXblPrivacyCheckPermissionForAnonymousUserAsync },
    { "XblPrivacyCheckPermissionForAnonymousUserResult", HandleXblPrivacyCheckPermissionForAnonymousUserResult },
    { "XblPrivacyCheckPermissionForAnonymousUserResultSize", HandleXblPrivacyCheckPermissionForAnonymousUserResultSize },
    { "XblPrivacyCheckPermissionResult", HandleXblPrivacyCheckPermissionResult },
    { "XblPrivacyCheckPermissionResultSize", HandleXblPrivacyCheckPermissionResultSize },
    { "XblPrivacyGetAvoidListAsync", HandleXblPrivacyGetAvoidListAsync },
    { "XblPrivacyGetAvoidListResult", HandleXblPrivacyGetAvoidListResult },
    { "XblPrivacyGetAvoidListResultCount", HandleXblPrivacyGetAvoidListResultCount },
    { "XblPrivacyGetMuteListAsync", HandleXblPrivacyGetMuteListAsync },
    { "XblPrivacyGetMuteListResult", HandleXblPrivacyGetMuteListResult },
    { "XblPrivacyGetMuteListResultCount", HandleXblPrivacyGetMuteListResultCount }
});

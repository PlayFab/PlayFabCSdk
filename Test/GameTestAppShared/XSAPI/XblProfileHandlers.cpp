#include "pch.h"

#include "XblProfileHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblProfileGetUserProfileAsync(
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

            int64_t xboxUserId{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }

            HRESULT hr = XblProfileGetUserProfileAsync(state->xblContext, static_cast<uint64_t>(xboxUserId), &async);
            LogToWindowFormat("XblProfileGetUserProfileAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XblUserProfile profile{};
            HRESULT hr = XblProfileGetUserProfileResult(&async, &profile);
            if (SUCCEEDED(hr))
            {
                payload.result["gamertag"] = profile.gamertag;
                payload.result["xboxUserId"] = profile.xboxUserId;
                payload.result["appDisplayPictureResizeUri"] = profile.appDisplayPictureResizeUri;
                payload.result["gameDisplayPictureResizeUri"] = profile.gameDisplayPictureResizeUri;
                payload.result["gamerscore"] = profile.gamerscore;
            }
            return hr;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesAsync(
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

            auto xboxUserIds = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            if (xboxUserIds.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserIds.push_back(userId);
            }

            HRESULT hr = XblProfileGetUserProfilesAsync(
                state->xblContext,
                xboxUserIds.data(),
                static_cast<size_t>(xboxUserIds.size()),
                &async);
            LogToWindowFormat("XblProfileGetUserProfilesAsync (count=%zu, hr=0x%08X)",
                xboxUserIds.size(), static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t profileCount{};
            HRESULT hr = XblProfileGetUserProfilesResultCount(&async, &profileCount);
            if (SUCCEEDED(hr))
            {
                std::vector<XblUserProfile> profiles(profileCount);
                hr = XblProfileGetUserProfilesResult(&async, profileCount, profiles.data());
                if (SUCCEEDED(hr))
                {
                    nlohmann::json profilesJson = nlohmann::json::array();
                    for (size_t i = 0; i < profileCount; ++i)
                    {
                        nlohmann::json profileJson;
                        profileJson["gamertag"] = profiles[i].gamertag;
                        profileJson["xboxUserId"] = profiles[i].xboxUserId;
                        profileJson["appDisplayPictureResizeUri"] = profiles[i].appDisplayPictureResizeUri;
                        profileJson["gameDisplayPictureResizeUri"] = profiles[i].gameDisplayPictureResizeUri;
                        profileJson["gamerscore"] = profiles[i].gamerscore;
                        profilesJson.push_back(profileJson);
                    }
                    payload.result["profiles"] = profilesJson;
                    payload.result["profileCount"] = profileCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesForSocialGroupAsync(
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

            std::string socialGroup, error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "socialGroup", socialGroup, error))
            {
                socialGroup = "People";
            }

            HRESULT hr = XblProfileGetUserProfilesForSocialGroupAsync(
                state->xblContext,
                socialGroup.c_str(),
                &async);
            LogToWindowFormat("XblProfileGetUserProfilesForSocialGroupAsync (socialGroup=%s, hr=0x%08X)",
                socialGroup.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t profileCount{};
            HRESULT hr = XblProfileGetUserProfilesForSocialGroupResultCount(&async, &profileCount);
            if (SUCCEEDED(hr))
            {
                std::vector<XblUserProfile> profiles(profileCount);
                hr = XblProfileGetUserProfilesForSocialGroupResult(&async, profileCount, profiles.data());
                if (SUCCEEDED(hr))
                {
                    nlohmann::json profilesJson = nlohmann::json::array();
                    for (size_t i = 0; i < profileCount; ++i)
                    {
                        nlohmann::json profileJson;
                        profileJson["gamertag"] = profiles[i].gamertag;
                        profileJson["xboxUserId"] = profiles[i].xboxUserId;
                        profileJson["appDisplayPictureResizeUri"] = profiles[i].appDisplayPictureResizeUri;
                        profileJson["gameDisplayPictureResizeUri"] = profiles[i].gameDisplayPictureResizeUri;
                        profileJson["gamerscore"] = profiles[i].gamerscore;
                        profilesJson.push_back(profileJson);
                    }
                    payload.result["profiles"] = profilesJson;
                    payload.result["profileCount"] = profileCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblProfileGetUserProfileResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblProfileGetUserProfileResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblProfileGetUserProfilesResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblProfileGetUserProfilesResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesForSocialGroupResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblProfileGetUserProfilesForSocialGroupResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblProfileGetUserProfilesForSocialGroupResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblProfileGetUserProfilesForSocialGroupResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblProfileGetUserProfileAsync", HandleXblProfileGetUserProfileAsync },
    { "XblProfileGetUserProfileResult", HandleXblProfileGetUserProfileResult },
    { "XblProfileGetUserProfilesAsync", HandleXblProfileGetUserProfilesAsync },
    { "XblProfileGetUserProfilesForSocialGroupAsync", HandleXblProfileGetUserProfilesForSocialGroupAsync },
    { "XblProfileGetUserProfilesForSocialGroupResult", HandleXblProfileGetUserProfilesForSocialGroupResult },
    { "XblProfileGetUserProfilesForSocialGroupResultCount", HandleXblProfileGetUserProfilesForSocialGroupResultCount },
    { "XblProfileGetUserProfilesResult", HandleXblProfileGetUserProfilesResult },
    { "XblProfileGetUserProfilesResultCount", HandleXblProfileGetUserProfilesResultCount }
});

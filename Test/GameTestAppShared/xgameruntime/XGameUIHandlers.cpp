#include "pch.h"

#include "XGameUIHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XGameUI.h>
#include "CommandRegistry.h"

static XGameUiTextEntryHandle s_textEntryHandle = nullptr;

CommandResultPayload HandleXGameUiShowMessageDialogAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowMessageDialogAsync(
                &async,
                "Test Title",
                "Test Content",
                "OK",
                "Retry",
                "Cancel",
                XGameUiMessageDialogButton::First,
                XGameUiMessageDialogButton::Third);
            LogToWindowFormat("XGameUiShowMessageDialogAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XGameUiMessageDialogButton resultButton{};
            HRESULT hr = XGameUiShowMessageDialogResult(&async, &resultButton);
            LogToWindowFormat("XGameUiShowMessageDialogResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["resultButton"] = static_cast<uint32_t>(resultButton);
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowMessageDialogResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowMessageDialogResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowSendGameInviteAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowSendGameInviteAsync(
                &async,
                state->xuser,
                "00000000-0000-0000-0000-000000000000",
                "default",
                "session1",
                nullptr,
                nullptr);
            LogToWindowFormat("XGameUiShowSendGameInviteAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowSendGameInviteResult(&async);
            LogToWindowFormat("XGameUiShowSendGameInviteResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowSendGameInviteResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowSendGameInviteResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowMultiplayerActivityGameInviteAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowMultiplayerActivityGameInviteAsync(
                &async,
                state->xuser);
            LogToWindowFormat("XGameUiShowMultiplayerActivityGameInviteAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowMultiplayerActivityGameInviteResult(&async);
            LogToWindowFormat("XGameUiShowMultiplayerActivityGameInviteResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowMultiplayerActivityGameInviteResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowMultiplayerActivityGameInviteResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowPlayerProfileCardAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowPlayerProfileCardAsync(
                &async,
                state->xuser,
                0);
            LogToWindowFormat("XGameUiShowPlayerProfileCardAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowPlayerProfileCardResult(&async);
            LogToWindowFormat("XGameUiShowPlayerProfileCardResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowPlayerProfileCardResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowPlayerProfileCardResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowAchievementsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowAchievementsAsync(
                &async,
                state->xuser,
                0);
            LogToWindowFormat("XGameUiShowAchievementsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowAchievementsResult(&async);
            LogToWindowFormat("XGameUiShowAchievementsResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowAchievementsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowAchievementsResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowPlayerPickerResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowPlayerPickerResultCount: requires XAsyncBlock from player picker async call");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowPlayerPickerResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowPlayerPickerResult: requires XAsyncBlock from player picker async call");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowErrorDialogAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowErrorDialogAsync(
                &async,
                E_FAIL,
                "Test error context");
            LogToWindowFormat("XGameUiShowErrorDialogAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowErrorDialogResult(&async);
            LogToWindowFormat("XGameUiShowErrorDialogResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowErrorDialogResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowErrorDialogResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiSetNotificationPositionHint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetNotificationPositionHint(XGameUiNotificationPositionHint::BottomCenter);
            LogToWindowFormat("XGameUiSetNotificationPositionHint (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowTextEntryAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowTextEntryAsync(
                &async,
                "Enter Text",
                "Please enter your text:",
                "",
                XGameUiTextEntryInputScope::Default,
                256);
            LogToWindowFormat("XGameUiShowTextEntryAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            uint32_t bufferSize = 0;
            HRESULT hr = XGameUiShowTextEntryResultSize(&async, &bufferSize);
            LogToWindowFormat("XGameUiShowTextEntryResultSize (hr=0x%08X, size=%u)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<char> buffer(bufferSize);
                uint32_t bufferUsed = 0;
                hr = XGameUiShowTextEntryResult(&async, bufferSize, buffer.data(), &bufferUsed);
                LogToWindowFormat("XGameUiShowTextEntryResult (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr))
                {
                    payload.result["resultText"] = std::string(buffer.data());
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowTextEntryResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowTextEntryResultSize: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowTextEntryResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowTextEntryResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiSetUiCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameUiUiCallbacks callbacks{};
            callbacks.context = nullptr;
            callbacks.showPlayerProfileCardCallback = nullptr;
            callbacks.showPlayerPickerCallback = nullptr;
            callbacks.showSendGameInviteCallback = nullptr;
            callbacks.showAchievementsCallback = nullptr;
            callbacks.showMultiplayerActivityGameInviteCallback = nullptr;
            callbacks.showMessageDialogCallback = nullptr;
            callbacks.showErrorDialogCallback = nullptr;
            callbacks.showTextEntryCallback = nullptr;
            HRESULT hr = XGameUiSetUiCallbacks(&callbacks, true);
            LogToWindowFormat("XGameUiSetUiCallbacks (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetMessageDialogUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetMessageDialogUiResponse(nullptr, XGameUiMessageDialogButton::First);
            LogToWindowFormat("XGameUiSetMessageDialogUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetPlayerPickerUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetPlayerPickerUiResponse(nullptr, 0, nullptr);
            LogToWindowFormat("XGameUiSetPlayerPickerUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetTextEntryUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetTextEntryUiResponse(nullptr, "");
            LogToWindowFormat("XGameUiSetTextEntryUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// These APIs are only available in the newest GDK
#if 0
CommandResultPayload HandleXGameUiSetPlayerProfileCardUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetPlayerProfileCardUiResponse(nullptr);
            LogToWindowFormat("XGameUiSetPlayerProfileCardUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetSendGameInviteUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetSendGameInviteUiResponse(nullptr);
            LogToWindowFormat("XGameUiSetSendGameInviteUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetAchievementsUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetAchievementsUiResponse(nullptr);
            LogToWindowFormat("XGameUiSetAchievementsUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetMultiplayerActivityGameInviteUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetMultiplayerActivityGameInviteUiResponse(nullptr);
            LogToWindowFormat("XGameUiSetMultiplayerActivityGameInviteUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiSetErrorDialogUiResponse(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiSetErrorDialogUiResponse(nullptr);
            LogToWindowFormat("XGameUiSetErrorDialogUiResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}
#endif

CommandResultPayload HandleXGameUiShowWebAuthenticationAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowWebAuthenticationAsync(
                &async,
                state->xuser,
                "https://example.com/auth",
                "https://example.com/callback");
            LogToWindowFormat("XGameUiShowWebAuthenticationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XGameUiShowWebAuthenticationResultSize(&async, &bufferSize);
            LogToWindowFormat("XGameUiShowWebAuthenticationResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XGameUiWebAuthenticationResultData* resultData = nullptr;
                size_t bufferUsed = 0;
                hr = XGameUiShowWebAuthenticationResult(&async, bufferSize, buffer.data(), &resultData, &bufferUsed);
                LogToWindowFormat("XGameUiShowWebAuthenticationResult (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr) && resultData != nullptr)
                {
                    payload.result["responseStatus"] = static_cast<uint32_t>(resultData->responseStatus);
                    if (resultData->responseCompletionUri != nullptr)
                    {
                        payload.result["responseCompletionUri"] = resultData->responseCompletionUri;
                    }
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowWebAuthenticationWithOptionsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowWebAuthenticationWithOptionsAsync(
                &async,
                state->xuser,
                "https://example.com/auth",
                "https://example.com/callback",
                XGameUiWebAuthenticationOptions::None);
            LogToWindowFormat("XGameUiShowWebAuthenticationWithOptionsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XGameUiShowWebAuthenticationResultSize(&async, &bufferSize);
            LogToWindowFormat("XGameUiShowWebAuthenticationResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XGameUiWebAuthenticationResultData* resultData = nullptr;
                size_t bufferUsed = 0;
                hr = XGameUiShowWebAuthenticationResult(&async, bufferSize, buffer.data(), &resultData, &bufferUsed);
                LogToWindowFormat("XGameUiShowWebAuthenticationResult (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr) && resultData != nullptr)
                {
                    payload.result["responseStatus"] = static_cast<uint32_t>(resultData->responseStatus);
                    if (resultData->responseCompletionUri != nullptr)
                    {
                        payload.result["responseCompletionUri"] = resultData->responseCompletionUri;
                    }
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowWebAuthenticationResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowWebAuthenticationResultSize: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiShowWebAuthenticationResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowWebAuthenticationResult: result is obtained via the async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiTextEntryOpen(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameUiTextEntryOptions options{};
            options.inputScope = XGameUiTextEntryInputScope::Default;
            options.positionHint = XGameUiTextEntryPositionHint::Bottom;
            options.visibilityFlags = XGameUiTextEntryVisibilityFlags::Default;
            HRESULT hr = XGameUiTextEntryOpen(&options, 256, "", 0, &s_textEntryHandle);
            LogToWindowFormat("XGameUiTextEntryOpen (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiTextEntryClose(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_textEntryHandle != nullptr)
            {
                XGameUiTextEntryClose(s_textEntryHandle);
                LogToWindow("XGameUiTextEntryClose: handle closed");
                s_textEntryHandle = nullptr;
            }
            else
            {
                LogToWindow("XGameUiTextEntryClose: no active handle");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXGameUiTextEntryGetState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (s_textEntryHandle == nullptr)
            {
                LogToWindow("XGameUiTextEntryGetState: no active handle");
                return E_UNEXPECTED;
            }
            XGameUiTextEntryChangeTypeFlags changeType{};
            uint32_t cursorIndex = 0;
            char buffer[512]{};
            HRESULT hr = XGameUiTextEntryGetState(s_textEntryHandle, &changeType, &cursorIndex, nullptr, nullptr, sizeof(buffer), buffer);
            LogToWindowFormat("XGameUiTextEntryGetState (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["changeType"] = static_cast<uint32_t>(changeType);
                payload.result["cursorIndex"] = cursorIndex;
                payload.result["text"] = std::string(buffer);
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiTextEntryGetExtents(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (s_textEntryHandle == nullptr)
            {
                LogToWindow("XGameUiTextEntryGetExtents: no active handle");
                return E_UNEXPECTED;
            }
            XGameUiTextEntryExtents extents{};
            HRESULT hr = XGameUiTextEntryGetExtents(s_textEntryHandle, &extents);
            LogToWindowFormat("XGameUiTextEntryGetExtents (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["left"] = extents.left;
                payload.result["top"] = extents.top;
                payload.result["right"] = extents.right;
                payload.result["bottom"] = extents.bottom;
            }
            return hr;
        });
}

CommandResultPayload HandleXGameUiTextEntryUpdatePositionHint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_textEntryHandle == nullptr)
            {
                LogToWindow("XGameUiTextEntryUpdatePositionHint: no active handle");
                return E_UNEXPECTED;
            }
            HRESULT hr = XGameUiTextEntryUpdatePositionHint(s_textEntryHandle, XGameUiTextEntryPositionHint::Bottom);
            LogToWindowFormat("XGameUiTextEntryUpdatePositionHint (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiTextEntryUpdateVisibility(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_textEntryHandle == nullptr)
            {
                LogToWindow("XGameUiTextEntryUpdateVisibility: no active handle");
                return E_UNEXPECTED;
            }
            HRESULT hr = XGameUiTextEntryUpdateVisibility(s_textEntryHandle, XGameUiTextEntryVisibilityFlags::Default);
            LogToWindowFormat("XGameUiTextEntryUpdateVisibility (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowStateShareAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowStateShareAsync(
                &async,
                state->xuser,
                "testLinkToken");
            LogToWindowFormat("XGameUiShowStateShareAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XGameUiShowStateShareResult(&async);
            LogToWindowFormat("XGameUiShowStateShareResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameUiShowStateShareResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameUiShowStateShareResult: result is obtained via the async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    // { "XGameUiSetAchievementsUiResponse", HandleXGameUiSetAchievementsUiResponse }, // Only available in newest GDK
    // { "XGameUiSetErrorDialogUiResponse", HandleXGameUiSetErrorDialogUiResponse }, // Only available in newest GDK
    { "XGameUiSetMessageDialogUiResponse", HandleXGameUiSetMessageDialogUiResponse },
    // { "XGameUiSetMultiplayerActivityGameInviteUiResponse", HandleXGameUiSetMultiplayerActivityGameInviteUiResponse }, // Only available in newest GDK
    { "XGameUiSetNotificationPositionHint", HandleXGameUiSetNotificationPositionHint },
    { "XGameUiSetPlayerPickerUiResponse", HandleXGameUiSetPlayerPickerUiResponse },
    // { "XGameUiSetPlayerProfileCardUiResponse", HandleXGameUiSetPlayerProfileCardUiResponse }, // Only available in newest GDK
    // { "XGameUiSetSendGameInviteUiResponse", HandleXGameUiSetSendGameInviteUiResponse }, // Only available in newest GDK
    { "XGameUiSetTextEntryUiResponse", HandleXGameUiSetTextEntryUiResponse },
    { "XGameUiSetUiCallbacks", HandleXGameUiSetUiCallbacks },
    { "XGameUiShowAchievementsAsync", HandleXGameUiShowAchievementsAsync },
    { "XGameUiShowAchievementsResult", HandleXGameUiShowAchievementsResult },
    { "XGameUiShowErrorDialogAsync", HandleXGameUiShowErrorDialogAsync },
    { "XGameUiShowErrorDialogResult", HandleXGameUiShowErrorDialogResult },
    { "XGameUiShowMessageDialogAsync", HandleXGameUiShowMessageDialogAsync },
    { "XGameUiShowMessageDialogResult", HandleXGameUiShowMessageDialogResult },
    { "XGameUiShowMultiplayerActivityGameInviteAsync", HandleXGameUiShowMultiplayerActivityGameInviteAsync },
    { "XGameUiShowMultiplayerActivityGameInviteResult", HandleXGameUiShowMultiplayerActivityGameInviteResult },
    { "XGameUiShowPlayerPickerResult", HandleXGameUiShowPlayerPickerResult },
    { "XGameUiShowPlayerPickerResultCount", HandleXGameUiShowPlayerPickerResultCount },
    { "XGameUiShowPlayerProfileCardAsync", HandleXGameUiShowPlayerProfileCardAsync },
    { "XGameUiShowPlayerProfileCardResult", HandleXGameUiShowPlayerProfileCardResult },
    { "XGameUiShowSendGameInviteAsync", HandleXGameUiShowSendGameInviteAsync },
    { "XGameUiShowSendGameInviteResult", HandleXGameUiShowSendGameInviteResult },
    { "XGameUiShowStateShareAsync", HandleXGameUiShowStateShareAsync },
    { "XGameUiShowStateShareResult", HandleXGameUiShowStateShareResult },
    { "XGameUiShowTextEntryAsync", HandleXGameUiShowTextEntryAsync },
    { "XGameUiShowTextEntryResult", HandleXGameUiShowTextEntryResult },
    { "XGameUiShowTextEntryResultSize", HandleXGameUiShowTextEntryResultSize },
    { "XGameUiShowWebAuthenticationAsync", HandleXGameUiShowWebAuthenticationAsync },
    { "XGameUiShowWebAuthenticationResult", HandleXGameUiShowWebAuthenticationResult },
    { "XGameUiShowWebAuthenticationResultSize", HandleXGameUiShowWebAuthenticationResultSize },
    { "XGameUiShowWebAuthenticationWithOptionsAsync", HandleXGameUiShowWebAuthenticationWithOptionsAsync },
    { "XGameUiTextEntryClose", HandleXGameUiTextEntryClose },
    { "XGameUiTextEntryGetExtents", HandleXGameUiTextEntryGetExtents },
    { "XGameUiTextEntryGetState", HandleXGameUiTextEntryGetState },
    { "XGameUiTextEntryOpen", HandleXGameUiTextEntryOpen },
    { "XGameUiTextEntryUpdatePositionHint", HandleXGameUiTextEntryUpdatePositionHint },
    { "XGameUiTextEntryUpdateVisibility", HandleXGameUiTextEntryUpdateVisibility }
});

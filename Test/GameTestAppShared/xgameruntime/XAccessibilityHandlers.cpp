#include "pch.h"

#include "XAccessibilityHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XAccessibility.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXClosedCaptionGetProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XClosedCaptionProperties props{};
            const HRESULT hr = XClosedCaptionGetProperties(&props);
            LogToWindowFormat("XClosedCaptionGetProperties (enabled=%s, fontScale=%.2f, hr=0x%08X)",
                props.Enabled ? "true" : "false", props.FontScale, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["enabled"] = props.Enabled;
            payload.result["fontScale"] = props.FontScale;
            return S_OK;
        });
}

CommandResultPayload HandleXClosedCaptionSetEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XClosedCaptionSetEnabled(true);
            LogToWindowFormat("XClosedCaptionSetEnabled (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXHighContrastGetMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XHighContrastMode mode{};
            const HRESULT hr = XHighContrastGetMode(&mode);
            LogToWindowFormat("XHighContrastGetMode (mode=%u, hr=0x%08X)", static_cast<uint32_t>(mode), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["mode"] = static_cast<uint32_t>(mode);
            return S_OK;
        });
}

CommandResultPayload HandleXSpeechToTextSetPositionHint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XSpeechToTextSetPositionHint(XSpeechToTextPositionHint::BottomCenter);
            LogToWindowFormat("XSpeechToTextSetPositionHint (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXSpeechToTextSendString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XSpeechToTextSendString("TestSpeaker", "Hello World", XSpeechToTextType::Text);
            LogToWindowFormat("XSpeechToTextSendString (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXSpeechToTextBeginHypothesisString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t hypothesisId = 0;
            const HRESULT hr = XSpeechToTextBeginHypothesisString("TestSpeaker", "Hello", XSpeechToTextType::Text, &hypothesisId);
            LogToWindowFormat("XSpeechToTextBeginHypothesisString (hypothesisId=%u, hr=0x%08X)", hypothesisId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["hypothesisId"] = hypothesisId;
            XSpeechToTextCancelHypothesisString(hypothesisId);
            return S_OK;
        });
}

CommandResultPayload HandleXSpeechToTextUpdateHypothesisString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint32_t hypothesisId = 0;
            HRESULT hr = XSpeechToTextBeginHypothesisString("TestSpeaker", "Hello", XSpeechToTextType::Text, &hypothesisId);
            RETURN_IF_FAILED(hr);
            hr = XSpeechToTextUpdateHypothesisString(hypothesisId, "Hello World");
            LogToWindowFormat("XSpeechToTextUpdateHypothesisString (hr=0x%08X)", static_cast<uint32_t>(hr));
            XSpeechToTextCancelHypothesisString(hypothesisId);
            return hr;
        });
}

CommandResultPayload HandleXSpeechToTextFinalizeHypothesisString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint32_t hypothesisId = 0;
            HRESULT hr = XSpeechToTextBeginHypothesisString("TestSpeaker", "Hello", XSpeechToTextType::Text, &hypothesisId);
            RETURN_IF_FAILED(hr);
            hr = XSpeechToTextFinalizeHypothesisString(hypothesisId, "Hello World");
            LogToWindowFormat("XSpeechToTextFinalizeHypothesisString (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXSpeechToTextCancelHypothesisString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint32_t hypothesisId = 0;
            HRESULT hr = XSpeechToTextBeginHypothesisString("TestSpeaker", "Hello", XSpeechToTextType::Text, &hypothesisId);
            RETURN_IF_FAILED(hr);
            hr = XSpeechToTextCancelHypothesisString(hypothesisId);
            LogToWindowFormat("XSpeechToTextCancelHypothesisString (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XClosedCaptionGetProperties", HandleXClosedCaptionGetProperties },
    { "XClosedCaptionSetEnabled", HandleXClosedCaptionSetEnabled },
    { "XHighContrastGetMode", HandleXHighContrastGetMode },
    { "XSpeechToTextBeginHypothesisString", HandleXSpeechToTextBeginHypothesisString },
    { "XSpeechToTextCancelHypothesisString", HandleXSpeechToTextCancelHypothesisString },
    { "XSpeechToTextFinalizeHypothesisString", HandleXSpeechToTextFinalizeHypothesisString },
    { "XSpeechToTextSendString", HandleXSpeechToTextSendString },
    { "XSpeechToTextSetPositionHint", HandleXSpeechToTextSetPositionHint },
    { "XSpeechToTextUpdateHypothesisString", HandleXSpeechToTextUpdateHypothesisString }
});

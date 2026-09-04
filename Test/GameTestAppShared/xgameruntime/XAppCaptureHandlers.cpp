#include "pch.h"

#include "XAppCaptureHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XAppCapture.h>
#include "CommandRegistry.h"

static XTaskQueueRegistrationToken s_broadcastChangedToken{};
static XTaskQueueRegistrationToken s_metadataPurgedToken{};
static XAppCaptureLocalStreamHandle s_localStreamHandle = nullptr;
static XAppCaptureScreenshotStreamHandle s_screenshotStreamHandle = nullptr;
static char s_userRecordLocalId[APPCAPTURE_MAX_LOCALID_LENGTH]{};

CommandResultPayload HandleXAppBroadcastShowUI(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppBroadcastShowUI(state->xuser);
            LogToWindowFormat("XAppBroadcastShowUI (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppBroadcastGetStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppBroadcastStatus status{};
            HRESULT hr = XAppBroadcastGetStatus(state->xuser, &status);
            LogToWindowFormat("XAppBroadcastGetStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["canStartBroadcast"] = status.canStartBroadcast;
                payload.result["isAnyAppBroadcasting"] = status.isAnyAppBroadcasting;
                payload.result["isCaptureResourceUnavailable"] = status.isCaptureResourceUnavailable;
                payload.result["isGameStreamInProgress"] = status.isGameStreamInProgress;
                payload.result["isGpuConstrained"] = status.isGpuConstrained;
                payload.result["isAppInactive"] = status.isAppInactive;
                payload.result["isBlockedForApp"] = status.isBlockedForApp;
                payload.result["isDisabledByUser"] = status.isDisabledByUser;
                payload.result["isDisabledBySystem"] = status.isDisabledBySystem;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppBroadcastIsAppBroadcasting(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isBroadcasting = XAppBroadcastIsAppBroadcasting();
            LogToWindowFormat("XAppBroadcastIsAppBroadcasting (result=%s)", isBroadcasting ? "true" : "false");
            payload.result["isBroadcasting"] = isBroadcasting;
            return S_OK;
        });
}

CommandResultPayload HandleXAppBroadcastRegisterIsAppBroadcastingChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XAppBroadcastRegisterIsAppBroadcastingChanged(
                state->taskQueue,
                nullptr,
                [](void*)
                {
                    LogToWindow("XAppBroadcast IsAppBroadcastingChanged callback fired");
                },
                &s_broadcastChangedToken);
            LogToWindowFormat("XAppBroadcastRegisterIsAppBroadcastingChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["token"] = static_cast<uint64_t>(s_broadcastChangedToken.token);
            }
            return hr;
        });
}

CommandResultPayload HandleXAppBroadcastUnregisterIsAppBroadcastingChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = XAppBroadcastUnregisterIsAppBroadcastingChanged(s_broadcastChangedToken, true);
            LogToWindowFormat("XAppBroadcastUnregisterIsAppBroadcastingChanged (result=%s)", result ? "true" : "false");
            s_broadcastChangedToken = {};
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXAppCaptureMetadataAddStringEvent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataAddStringEvent("TestEvent", "TestValue", XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataAddStringEvent (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataAddInt32Event(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataAddInt32Event("TestInt32Event", 42, XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataAddInt32Event (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataAddDoubleEvent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataAddDoubleEvent("TestDoubleEvent", 3.14, XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataAddDoubleEvent (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataStartStringState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataStartStringState("TestStringState", "TestValue", XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataStartStringState (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataStartInt32State(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataStartInt32State("TestInt32State", 100, XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataStartInt32State (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataStartDoubleState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataStartDoubleState("TestDoubleState", 2.718, XAppCaptureMetadataPriority::Informational);
            LogToWindowFormat("XAppCaptureMetadataStartDoubleState (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataStopState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataStopState("TestStringState");
            LogToWindowFormat("XAppCaptureMetadataStopState (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataStopAllStates(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureMetadataStopAllStates();
            LogToWindowFormat("XAppCaptureMetadataStopAllStates (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureMetadataRemainingStorageBytesAvailable(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t remainingBytes = 0;
            HRESULT hr = XAppCaptureMetadataRemainingStorageBytesAvailable(&remainingBytes);
            LogToWindowFormat("XAppCaptureMetadataRemainingStorageBytesAvailable (hr=0x%08X, bytes=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(remainingBytes));
            if (SUCCEEDED(hr))
            {
                payload.result["remainingBytes"] = remainingBytes;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureRegisterMetadataPurged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XAppCaptureRegisterMetadataPurged(
                state->taskQueue,
                nullptr,
                [](void*)
                {
                    LogToWindow("XAppCapture MetadataPurged callback fired");
                },
                &s_metadataPurgedToken);
            LogToWindowFormat("XAppCaptureRegisterMetadataPurged (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["token"] = static_cast<uint64_t>(s_metadataPurgedToken.token);
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureUnRegisterMetadataPurged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = XAppCaptureUnRegisterMetadataPurged(s_metadataPurgedToken, true);
            LogToWindowFormat("XAppCaptureUnRegisterMetadataPurged (result=%s)", result ? "true" : "false");
            s_metadataPurgedToken = {};
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXAppCaptureTakeDiagnosticScreenshot(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureDiagnosticScreenshotResult result{};
            HRESULT hr = XAppCaptureTakeDiagnosticScreenshot(true, XAppCaptureScreenshotFormatFlag::SDR, nullptr, &result);
            LogToWindowFormat("XAppCaptureTakeDiagnosticScreenshot (hr=0x%08X, fileCount=%zu)", static_cast<uint32_t>(hr), result.fileCount);
            if (SUCCEEDED(hr))
            {
                payload.result["fileCount"] = result.fileCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureRecordDiagnosticClip(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureRecordClipResult result{};
            HRESULT hr = XAppCaptureRecordDiagnosticClip(0, 5000, nullptr, &result);
            LogToWindowFormat("XAppCaptureRecordDiagnosticClip (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["path"] = std::string(result.path);
                payload.result["fileSize"] = result.fileSize;
                payload.result["durationInMs"] = result.durationInMs;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureGetVideoCaptureSettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureVideoCaptureSettings settings{};
            HRESULT hr = XAppCaptureGetVideoCaptureSettings(&settings);
            LogToWindowFormat("XAppCaptureGetVideoCaptureSettings (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["width"] = settings.width;
                payload.result["height"] = settings.height;
                payload.result["maxRecordTimespanDurationInMs"] = settings.maxRecordTimespanDurationInMs;
                payload.result["isCaptureByGamesAllowed"] = settings.isCaptureByGamesAllowed;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureRecordTimespan(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureLocalResult result{};
            HRESULT hr = XAppCaptureRecordTimespan(nullptr, 5000, &result);
            LogToWindowFormat("XAppCaptureRecordTimespan (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                s_localStreamHandle = result.clipHandle;
                payload.result["fileSizeInBytes"] = result.fileSizeInBytes;
                payload.result["durationInMilliseconds"] = result.durationInMilliseconds;
                payload.result["width"] = result.width;
                payload.result["height"] = result.height;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureReadLocalStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (s_localStreamHandle == nullptr)
            {
                LogToWindow("XAppCaptureReadLocalStream: no active stream handle");
                return E_UNEXPECTED;
            }
            uint8_t buffer[4096]{};
            uint32_t bytesWritten = 0;
            HRESULT hr = XAppCaptureReadLocalStream(s_localStreamHandle, 0, sizeof(buffer), buffer, &bytesWritten);
            LogToWindowFormat("XAppCaptureReadLocalStream (hr=0x%08X, bytesWritten=%u)", static_cast<uint32_t>(hr), bytesWritten);
            if (SUCCEEDED(hr))
            {
                payload.result["bytesRead"] = bytesWritten;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureCloseLocalStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_localStreamHandle == nullptr)
            {
                LogToWindow("XAppCaptureCloseLocalStream: no active stream handle");
                return S_OK;
            }
            HRESULT hr = XAppCaptureCloseLocalStream(s_localStreamHandle);
            LogToWindowFormat("XAppCaptureCloseLocalStream (hr=0x%08X)", static_cast<uint32_t>(hr));
            s_localStreamHandle = nullptr;
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureStartUserRecord(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            memset(s_userRecordLocalId, 0, sizeof(s_userRecordLocalId));
            HRESULT hr = XAppCaptureStartUserRecord(state->xuser, APPCAPTURE_MAX_LOCALID_LENGTH, s_userRecordLocalId);
            LogToWindowFormat("XAppCaptureStartUserRecord (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["localId"] = std::string(s_userRecordLocalId);
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureStopUserRecord(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureUserRecordingResult result{};
            HRESULT hr = XAppCaptureStopUserRecord(s_userRecordLocalId, &result);
            LogToWindowFormat("XAppCaptureStopUserRecord (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["fileSizeInBytes"] = result.fileSizeInBytes;
                payload.result["durationInMilliseconds"] = result.durationInMilliseconds;
                payload.result["width"] = result.width;
                payload.result["height"] = result.height;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureCancelUserRecord(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureCancelUserRecord(s_userRecordLocalId);
            LogToWindowFormat("XAppCaptureCancelUserRecord (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureTakeScreenshot(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAppCaptureTakeScreenshotResult result{};
            HRESULT hr = XAppCaptureTakeScreenshot(state->xuser, &result);
            LogToWindowFormat("XAppCaptureTakeScreenshot (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["localId"] = std::string(result.localId);
                payload.result["availableFormats"] = static_cast<uint32_t>(result.availableScreenshotFormats);
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureOpenScreenshotStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t totalBytes = 0;
            HRESULT hr = XAppCaptureOpenScreenshotStream("", XAppCaptureScreenshotFormatFlag::SDR, &s_screenshotStreamHandle, &totalBytes);
            LogToWindowFormat("XAppCaptureOpenScreenshotStream (hr=0x%08X, totalBytes=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(totalBytes));
            if (SUCCEEDED(hr))
            {
                payload.result["totalBytes"] = totalBytes;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureReadScreenshotStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (s_screenshotStreamHandle == nullptr)
            {
                LogToWindow("XAppCaptureReadScreenshotStream: no active stream handle");
                return E_UNEXPECTED;
            }
            uint8_t buffer[4096]{};
            uint32_t bytesWritten = 0;
            HRESULT hr = XAppCaptureReadScreenshotStream(s_screenshotStreamHandle, 0, sizeof(buffer), buffer, &bytesWritten);
            LogToWindowFormat("XAppCaptureReadScreenshotStream (hr=0x%08X, bytesWritten=%u)", static_cast<uint32_t>(hr), bytesWritten);
            if (SUCCEEDED(hr))
            {
                payload.result["bytesRead"] = bytesWritten;
            }
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureCloseScreenshotStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_screenshotStreamHandle == nullptr)
            {
                LogToWindow("XAppCaptureCloseScreenshotStream: no active stream handle");
                return S_OK;
            }
            HRESULT hr = XAppCaptureCloseScreenshotStream(s_screenshotStreamHandle);
            LogToWindowFormat("XAppCaptureCloseScreenshotStream (hr=0x%08X)", static_cast<uint32_t>(hr));
            s_screenshotStreamHandle = nullptr;
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureEnableRecord(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureEnableRecord();
            LogToWindowFormat("XAppCaptureEnableRecord (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAppCaptureDisableRecord(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XAppCaptureDisableRecord();
            LogToWindowFormat("XAppCaptureDisableRecord (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XAppBroadcastGetStatus", HandleXAppBroadcastGetStatus },
    { "XAppBroadcastIsAppBroadcasting", HandleXAppBroadcastIsAppBroadcasting },
    { "XAppBroadcastRegisterIsAppBroadcastingChanged", HandleXAppBroadcastRegisterIsAppBroadcastingChanged },
    { "XAppBroadcastShowUI", HandleXAppBroadcastShowUI },
    { "XAppBroadcastUnregisterIsAppBroadcastingChanged", HandleXAppBroadcastUnregisterIsAppBroadcastingChanged },
    { "XAppCaptureCancelUserRecord", HandleXAppCaptureCancelUserRecord },
    { "XAppCaptureCloseLocalStream", HandleXAppCaptureCloseLocalStream },
    { "XAppCaptureCloseScreenshotStream", HandleXAppCaptureCloseScreenshotStream },
    { "XAppCaptureDisableRecord", HandleXAppCaptureDisableRecord },
    { "XAppCaptureEnableRecord", HandleXAppCaptureEnableRecord },
    { "XAppCaptureGetVideoCaptureSettings", HandleXAppCaptureGetVideoCaptureSettings },
    { "XAppCaptureMetadataAddDoubleEvent", HandleXAppCaptureMetadataAddDoubleEvent },
    { "XAppCaptureMetadataAddInt32Event", HandleXAppCaptureMetadataAddInt32Event },
    { "XAppCaptureMetadataAddStringEvent", HandleXAppCaptureMetadataAddStringEvent },
    { "XAppCaptureMetadataRemainingStorageBytesAvailable", HandleXAppCaptureMetadataRemainingStorageBytesAvailable },
    { "XAppCaptureMetadataStartDoubleState", HandleXAppCaptureMetadataStartDoubleState },
    { "XAppCaptureMetadataStartInt32State", HandleXAppCaptureMetadataStartInt32State },
    { "XAppCaptureMetadataStartStringState", HandleXAppCaptureMetadataStartStringState },
    { "XAppCaptureMetadataStopAllStates", HandleXAppCaptureMetadataStopAllStates },
    { "XAppCaptureMetadataStopState", HandleXAppCaptureMetadataStopState },
    { "XAppCaptureOpenScreenshotStream", HandleXAppCaptureOpenScreenshotStream },
    { "XAppCaptureReadLocalStream", HandleXAppCaptureReadLocalStream },
    { "XAppCaptureReadScreenshotStream", HandleXAppCaptureReadScreenshotStream },
    { "XAppCaptureRecordDiagnosticClip", HandleXAppCaptureRecordDiagnosticClip },
    { "XAppCaptureRecordTimespan", HandleXAppCaptureRecordTimespan },
    { "XAppCaptureRegisterMetadataPurged", HandleXAppCaptureRegisterMetadataPurged },
    { "XAppCaptureStartUserRecord", HandleXAppCaptureStartUserRecord },
    { "XAppCaptureStopUserRecord", HandleXAppCaptureStopUserRecord },
    { "XAppCaptureTakeDiagnosticScreenshot", HandleXAppCaptureTakeDiagnosticScreenshot },
    { "XAppCaptureTakeScreenshot", HandleXAppCaptureTakeScreenshot },
    { "XAppCaptureUnRegisterMetadataPurged", HandleXAppCaptureUnRegisterMetadataPurged }
});

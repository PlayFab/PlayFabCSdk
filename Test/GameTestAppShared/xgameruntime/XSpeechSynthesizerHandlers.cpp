#include "pch.h"

#include "XSpeechSynthesizerHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XSpeechSynthesizer.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXSpeechSynthesizerEnumerateInstalledVoices(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t count = 0;
            const HRESULT hr = XSpeechSynthesizerEnumerateInstalledVoices(&count,
                [](const XSpeechSynthesizerVoiceInformation*, void* ctx) -> bool
                {
                    auto* c = static_cast<uint32_t*>(ctx);
                    ++(*c);
                    return true;
                });
            LogToWindowFormat("XSpeechSynthesizerEnumerateInstalledVoices (count=%u, hr=0x%08X)", count, static_cast<uint32_t>(hr));
            payload.result["voiceCount"] = count;
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerCreate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            const HRESULT hr = XSpeechSynthesizerCreate(&synth);
            LogToWindowFormat("XSpeechSynthesizerCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (synth) { XSpeechSynthesizerCloseHandle(synth); }
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerCloseHandle(synth);
            LogToWindowFormat("XSpeechSynthesizerCloseHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerSetDefaultVoice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            LogToWindowFormat("XSpeechSynthesizerSetDefaultVoice (hr=0x%08X)", static_cast<uint32_t>(hr));
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerSetCustomVoice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetCustomVoice(synth, "DefaultVoice");
            LogToWindowFormat("XSpeechSynthesizerSetCustomVoice (hr=0x%08X)", static_cast<uint32_t>(hr));
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerCreateStreamFromText(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            if (SUCCEEDED(hr))
            {
                XSpeechSynthesizerStreamHandle stream = nullptr;
                hr = XSpeechSynthesizerCreateStreamFromText(synth, "Hello", &stream);
                LogToWindowFormat("XSpeechSynthesizerCreateStreamFromText (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (stream) { XSpeechSynthesizerCloseStreamHandle(stream); }
            }
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerCreateStreamFromSsml(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            if (SUCCEEDED(hr))
            {
                XSpeechSynthesizerStreamHandle stream = nullptr;
                hr = XSpeechSynthesizerCreateStreamFromSsml(synth,
                    "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='en-US'>"
                    "<voice>Hello</voice></speak>", &stream);
                LogToWindowFormat("XSpeechSynthesizerCreateStreamFromSsml (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (stream) { XSpeechSynthesizerCloseStreamHandle(stream); }
            }
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerCloseStreamHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            if (SUCCEEDED(hr))
            {
                XSpeechSynthesizerStreamHandle stream = nullptr;
                hr = XSpeechSynthesizerCreateStreamFromText(synth, "Hello", &stream);
                if (SUCCEEDED(hr))
                {
                    hr = XSpeechSynthesizerCloseStreamHandle(stream);
                    LogToWindowFormat("XSpeechSynthesizerCloseStreamHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
                }
            }
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerGetStreamDataSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            if (SUCCEEDED(hr))
            {
                XSpeechSynthesizerStreamHandle stream = nullptr;
                hr = XSpeechSynthesizerCreateStreamFromText(synth, "Hello", &stream);
                if (SUCCEEDED(hr))
                {
                    size_t dataSize = 0;
                    hr = XSpeechSynthesizerGetStreamDataSize(stream, &dataSize);
                    LogToWindowFormat("XSpeechSynthesizerGetStreamDataSize (size=%zu, hr=0x%08X)", dataSize, static_cast<uint32_t>(hr));
                    payload.result["dataSize"] = dataSize;
                    XSpeechSynthesizerCloseStreamHandle(stream);
                }
            }
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

CommandResultPayload HandleXSpeechSynthesizerGetStreamData(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XSpeechSynthesizerHandle synth = nullptr;
            HRESULT hr = XSpeechSynthesizerCreate(&synth);
            RETURN_IF_FAILED(hr);
            hr = XSpeechSynthesizerSetDefaultVoice(synth);
            if (SUCCEEDED(hr))
            {
                XSpeechSynthesizerStreamHandle stream = nullptr;
                hr = XSpeechSynthesizerCreateStreamFromText(synth, "Hello", &stream);
                if (SUCCEEDED(hr))
                {
                    size_t dataSize = 0;
                    hr = XSpeechSynthesizerGetStreamDataSize(stream, &dataSize);
                    if (SUCCEEDED(hr) && dataSize > 0)
                    {
                        std::vector<uint8_t> data(dataSize);
                        size_t used = 0;
                        hr = XSpeechSynthesizerGetStreamData(stream, dataSize, data.data(), &used);
                        LogToWindowFormat("XSpeechSynthesizerGetStreamData (used=%zu, hr=0x%08X)", used, static_cast<uint32_t>(hr));
                        payload.result["bytesRead"] = used;
                    }
                    XSpeechSynthesizerCloseStreamHandle(stream);
                }
            }
            XSpeechSynthesizerCloseHandle(synth);
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XSpeechSynthesizerCloseHandle", HandleXSpeechSynthesizerCloseHandle },
    { "XSpeechSynthesizerCloseStreamHandle", HandleXSpeechSynthesizerCloseStreamHandle },
    { "XSpeechSynthesizerCreate", HandleXSpeechSynthesizerCreate },
    { "XSpeechSynthesizerCreateStreamFromSsml", HandleXSpeechSynthesizerCreateStreamFromSsml },
    { "XSpeechSynthesizerCreateStreamFromText", HandleXSpeechSynthesizerCreateStreamFromText },
    { "XSpeechSynthesizerEnumerateInstalledVoices", HandleXSpeechSynthesizerEnumerateInstalledVoices },
    { "XSpeechSynthesizerGetStreamData", HandleXSpeechSynthesizerGetStreamData },
    { "XSpeechSynthesizerGetStreamDataSize", HandleXSpeechSynthesizerGetStreamDataSize },
    { "XSpeechSynthesizerSetCustomVoice", HandleXSpeechSynthesizerSetCustomVoice },
    { "XSpeechSynthesizerSetDefaultVoice", HandleXSpeechSynthesizerSetDefaultVoice }
});

#include "pch.h"

#include "XNetworkingHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XNetworking.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXNetworkingQueryPreferredLocalUdpMultiplayerPort(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint16_t port = 0;
            const HRESULT hr = XNetworkingQueryPreferredLocalUdpMultiplayerPort(&port);
            LogToWindowFormat("XNetworkingQueryPreferredLocalUdpMultiplayerPort (port=%u, hr=0x%08X)", port, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["port"] = port;
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQueryPreferredLocalUdpMultiplayerPortAsync(
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
            const HRESULT hr = XNetworkingQueryPreferredLocalUdpMultiplayerPortAsync(&async);
            LogToWindowFormat("XNetworkingQueryPreferredLocalUdpMultiplayerPortAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            uint16_t port = 0;
            const HRESULT hr = XNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult(&async, &port);
            LogToWindowFormat("XNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult (port=%u, hr=0x%08X)", port, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { payload.result["port"] = port; }
            return hr;
        });
}

CommandResultPayload HandleXNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            const HRESULT hr = XNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged(state->taskQueue, nullptr,
                [](void*, uint16_t) {}, &token);
            LogToWindowFormat("XNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { XNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged(token, true); }
            return hr;
        });
}

CommandResultPayload HandleXNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            HRESULT hr = XNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged(state->taskQueue, nullptr,
                [](void*, uint16_t) {}, &token);
            RETURN_IF_FAILED(hr);
            const bool result = XNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged(token, true);
            LogToWindowFormat("XNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlAsync(
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
            const HRESULT hr = XNetworkingQuerySecurityInformationForUrlAsync("https://www.microsoft.com", &async);
            LogToWindowFormat("XNetworkingQuerySecurityInformationForUrlAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XNetworkingQuerySecurityInformationForUrlAsyncResultSize(&async, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XNetworkingSecurityInformation* secInfo = nullptr;
                size_t used = 0;
                hr = XNetworkingQuerySecurityInformationForUrlAsyncResult(&async, bufferSize, &used, buffer.data(), &secInfo);
                if (SUCCEEDED(hr) && secInfo) { payload.result["protocolFlags"] = secInfo->enabledHttpSecurityProtocolFlags; }
            }
            LogToWindowFormat("XNetworkingQuerySecurityInformationForUrlAsyncResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlAsyncResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XNetworkingQuerySecurityInformationForUrlAsyncResultSize: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlAsyncResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XNetworkingQuerySecurityInformationForUrlAsyncResult: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlUtf16Async(
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
            const HRESULT hr = XNetworkingQuerySecurityInformationForUrlUtf16Async(L"https://www.microsoft.com", &async);
            LogToWindowFormat("XNetworkingQuerySecurityInformationForUrlUtf16Async (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize(&async, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XNetworkingSecurityInformation* secInfo = nullptr;
                size_t used = 0;
                hr = XNetworkingQuerySecurityInformationForUrlUtf16AsyncResult(&async, bufferSize, &used, buffer.data(), &secInfo);
            }
            LogToWindowFormat("XNetworkingQuerySecurityInformationForUrlUtf16AsyncResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQuerySecurityInformationForUrlUtf16AsyncResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XNetworkingQuerySecurityInformationForUrlUtf16AsyncResult: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingVerifyServerCertificate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XNetworkingSecurityInformation secInfo{};
            const HRESULT hr = XNetworkingVerifyServerCertificate(nullptr, &secInfo);
            LogToWindowFormat("XNetworkingVerifyServerCertificate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXNetworkingGetConnectivityHint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XNetworkingConnectivityHint hint{};
            const HRESULT hr = XNetworkingGetConnectivityHint(&hint);
            LogToWindowFormat("XNetworkingGetConnectivityHint (level=%u, cost=%u, initialized=%s, hr=0x%08X)",
                static_cast<uint32_t>(hint.connectivityLevel), static_cast<uint32_t>(hint.connectivityCost),
                hint.networkInitialized ? "true" : "false", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["connectivityLevel"] = static_cast<uint32_t>(hint.connectivityLevel);
            payload.result["connectivityCost"] = static_cast<uint32_t>(hint.connectivityCost);
            payload.result["networkInitialized"] = hint.networkInitialized;
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingRegisterConnectivityHintChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            const HRESULT hr = XNetworkingRegisterConnectivityHintChanged(state->taskQueue, nullptr,
                [](void*, const XNetworkingConnectivityHint*) {}, &token);
            LogToWindowFormat("XNetworkingRegisterConnectivityHintChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { XNetworkingUnregisterConnectivityHintChanged(token, true); }
            return hr;
        });
}

CommandResultPayload HandleXNetworkingUnregisterConnectivityHintChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            HRESULT hr = XNetworkingRegisterConnectivityHintChanged(state->taskQueue, nullptr,
                [](void*, const XNetworkingConnectivityHint*) {}, &token);
            RETURN_IF_FAILED(hr);
            const bool result = XNetworkingUnregisterConnectivityHintChanged(token, true);
            LogToWindowFormat("XNetworkingUnregisterConnectivityHintChanged (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingQueryConfigurationSetting(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t value = 0;
            const HRESULT hr = XNetworkingQueryConfigurationSetting(
                XNetworkingConfigurationSetting::MaxTitleTcpQueuedReceiveBufferSize, &value);
            LogToWindowFormat("XNetworkingQueryConfigurationSetting (value=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(value), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["value"] = value;
            return S_OK;
        });
}

CommandResultPayload HandleXNetworkingSetConfigurationSetting(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint64_t currentValue = 0;
            HRESULT hr = XNetworkingQueryConfigurationSetting(
                XNetworkingConfigurationSetting::MaxTitleTcpQueuedReceiveBufferSize, &currentValue);
            if (SUCCEEDED(hr))
            {
                hr = XNetworkingSetConfigurationSetting(
                    XNetworkingConfigurationSetting::MaxTitleTcpQueuedReceiveBufferSize, currentValue);
            }
            LogToWindowFormat("XNetworkingSetConfigurationSetting (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXNetworkingQueryStatistics(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XNetworkingStatisticsBuffer stats{};
            const HRESULT hr = XNetworkingQueryStatistics(
                XNetworkingStatisticsType::TitleTcpQueuedReceivedBufferUsage, &stats);
            LogToWindowFormat("XNetworkingQueryStatistics (currentQueued=%llu, peak=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(stats.tcpQueuedReceiveBufferUsage.numBytesCurrentlyQueued),
                static_cast<unsigned long long>(stats.tcpQueuedReceiveBufferUsage.peakNumBytesEverQueued),
                static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["numBytesCurrentlyQueued"] = stats.tcpQueuedReceiveBufferUsage.numBytesCurrentlyQueued;
            payload.result["peakNumBytesEverQueued"] = stats.tcpQueuedReceiveBufferUsage.peakNumBytesEverQueued;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XNetworkingGetConnectivityHint", HandleXNetworkingGetConnectivityHint },
    { "XNetworkingQueryConfigurationSetting", HandleXNetworkingQueryConfigurationSetting },
    { "XNetworkingQueryPreferredLocalUdpMultiplayerPort", HandleXNetworkingQueryPreferredLocalUdpMultiplayerPort },
    { "XNetworkingQueryPreferredLocalUdpMultiplayerPortAsync", HandleXNetworkingQueryPreferredLocalUdpMultiplayerPortAsync },
    { "XNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult", HandleXNetworkingQueryPreferredLocalUdpMultiplayerPortAsyncResult },
    { "XNetworkingQuerySecurityInformationForUrlAsync", HandleXNetworkingQuerySecurityInformationForUrlAsync },
    { "XNetworkingQuerySecurityInformationForUrlAsyncResult", HandleXNetworkingQuerySecurityInformationForUrlAsyncResult },
    { "XNetworkingQuerySecurityInformationForUrlAsyncResultSize", HandleXNetworkingQuerySecurityInformationForUrlAsyncResultSize },
    { "XNetworkingQuerySecurityInformationForUrlUtf16Async", HandleXNetworkingQuerySecurityInformationForUrlUtf16Async },
    { "XNetworkingQuerySecurityInformationForUrlUtf16AsyncResult", HandleXNetworkingQuerySecurityInformationForUrlUtf16AsyncResult },
    { "XNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize", HandleXNetworkingQuerySecurityInformationForUrlUtf16AsyncResultSize },
    { "XNetworkingQueryStatistics", HandleXNetworkingQueryStatistics },
    { "XNetworkingRegisterConnectivityHintChanged", HandleXNetworkingRegisterConnectivityHintChanged },
    { "XNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged", HandleXNetworkingRegisterPreferredLocalUdpMultiplayerPortChanged },
    { "XNetworkingSetConfigurationSetting", HandleXNetworkingSetConfigurationSetting },
    { "XNetworkingUnregisterConnectivityHintChanged", HandleXNetworkingUnregisterConnectivityHintChanged },
    { "XNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged", HandleXNetworkingUnregisterPreferredLocalUdpMultiplayerPortChanged },
    { "XNetworkingVerifyServerCertificate", HandleXNetworkingVerifyServerCertificate }
});

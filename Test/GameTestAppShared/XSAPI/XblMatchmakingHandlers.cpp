#include "pch.h"

#include "XblMatchmakingHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_matchmakingBuffer;

CommandResultPayload HandleXblMatchmakingCreateMatchTicketAsync(
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

            std::string scid, sessionTemplateName, sessionName, error;
            std::string matchmakingServiceConfigurationId, hopperName;

            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionTemplateName", sessionTemplateName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "sessionName", sessionName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "matchmakingServiceConfigurationId", matchmakingServiceConfigurationId, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "hopperName", hopperName, error))
            {
                return E_INVALIDARG;
            }

            int64_t ticketTimeout{};
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "ticketTimeout", ticketTimeout, error))
            {
                return E_INVALIDARG;
            }

            int64_t preserveSession{};
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "preserveSession", preserveSession, error))
            {
                return E_INVALIDARG;
            }

            std::string ticketAttributesJson;
            CommandHandlerShared::TryGetStringParameter(parameters, "ticketAttributesJson", ticketAttributesJson, error);

            XblMultiplayerSessionReference sessionRef{};
            strncpy_s(sessionRef.Scid, scid.c_str(), _TRUNCATE);
            strncpy_s(sessionRef.SessionTemplateName, sessionTemplateName.c_str(), _TRUNCATE);
            strncpy_s(sessionRef.SessionName, sessionName.c_str(), _TRUNCATE);

            HRESULT hr = XblMatchmakingCreateMatchTicketAsync(
                state->xblContext,
                sessionRef,
                matchmakingServiceConfigurationId.c_str(),
                hopperName.c_str(),
                static_cast<uint64_t>(ticketTimeout),
                static_cast<XblPreserveSessionMode>(preserveSession),
                ticketAttributesJson.empty() ? nullptr : ticketAttributesJson.c_str(),
                &async);
            LogToWindowFormat("XblMatchmakingCreateMatchTicketAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XblCreateMatchTicketResponse result{};
            HRESULT hr = XblMatchmakingCreateMatchTicketResult(&async, &result);
            if (SUCCEEDED(hr))
            {
                payload.result["ticketId"] = result.matchTicketId;
                payload.result["estimatedWaitTime"] = result.estimatedWaitTime;
            }
            return hr;
        });
}

CommandResultPayload HandleXblMatchmakingDeleteMatchTicketAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string serviceConfigurationId, hopperName, ticketId, error;

            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "hopperName", hopperName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "ticketId", ticketId, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMatchmakingDeleteMatchTicketAsync(
                state->xblContext,
                serviceConfigurationId.c_str(),
                hopperName.c_str(),
                ticketId.c_str(),
                &async);
            LogToWindowFormat("XblMatchmakingDeleteMatchTicketAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMatchmakingGetMatchTicketDetailsAsync(
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

            std::string serviceConfigurationId, hopperName, ticketId, error;

            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "hopperName", hopperName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "ticketId", ticketId, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMatchmakingGetMatchTicketDetailsAsync(
                state->xblContext,
                serviceConfigurationId.c_str(),
                hopperName.c_str(),
                ticketId.c_str(),
                &async);
            LogToWindowFormat("XblMatchmakingGetMatchTicketDetailsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblMatchmakingGetMatchTicketDetailsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_matchmakingBuffer.resize(resultSize);
                XblMatchTicketDetailsResponse* result{};
                hr = XblMatchmakingGetMatchTicketDetailsResult(&async, resultSize, s_matchmakingBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["matchStatus"] = static_cast<int32_t>(result->matchStatus);
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblMatchmakingGetHopperStatisticsAsync(
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

            std::string serviceConfigurationId, hopperName, error;

            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "hopperName", hopperName, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblMatchmakingGetHopperStatisticsAsync(
                state->xblContext,
                serviceConfigurationId.c_str(),
                hopperName.c_str(),
                &async);
            LogToWindowFormat("XblMatchmakingGetHopperStatisticsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblMatchmakingGetHopperStatisticsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_matchmakingBuffer.resize(resultSize);
                XblHopperStatisticsResponse* result{};
                hr = XblMatchmakingGetHopperStatisticsResult(&async, resultSize, s_matchmakingBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["hopperName"] = result->hopperName;
                    payload.result["playersWaitingToMatch"] = result->playersWaitingToMatch;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblMatchmakingCreateMatchTicketResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMatchmakingCreateMatchTicketResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMatchmakingGetMatchTicketDetailsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMatchmakingGetMatchTicketDetailsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMatchmakingGetMatchTicketDetailsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMatchmakingGetMatchTicketDetailsResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMatchmakingGetHopperStatisticsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMatchmakingGetHopperStatisticsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMatchmakingGetHopperStatisticsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMatchmakingGetHopperStatisticsResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblMatchmakingCreateMatchTicketAsync", HandleXblMatchmakingCreateMatchTicketAsync },
    { "XblMatchmakingCreateMatchTicketResult", HandleXblMatchmakingCreateMatchTicketResult },
    { "XblMatchmakingDeleteMatchTicketAsync", HandleXblMatchmakingDeleteMatchTicketAsync },
    { "XblMatchmakingGetHopperStatisticsAsync", HandleXblMatchmakingGetHopperStatisticsAsync },
    { "XblMatchmakingGetHopperStatisticsResult", HandleXblMatchmakingGetHopperStatisticsResult },
    { "XblMatchmakingGetHopperStatisticsResultSize", HandleXblMatchmakingGetHopperStatisticsResultSize },
    { "XblMatchmakingGetMatchTicketDetailsAsync", HandleXblMatchmakingGetMatchTicketDetailsAsync },
    { "XblMatchmakingGetMatchTicketDetailsResult", HandleXblMatchmakingGetMatchTicketDetailsResult },
    { "XblMatchmakingGetMatchTicketDetailsResultSize", HandleXblMatchmakingGetMatchTicketDetailsResultSize }
});

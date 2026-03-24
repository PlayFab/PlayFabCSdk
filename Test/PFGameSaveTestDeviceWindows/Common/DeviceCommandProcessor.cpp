#include "pch.h"
#include "DeviceCommandProcessor.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "DeviceCommandHandlers.h"

#include <nlohmann/json.hpp>

namespace
{
    struct CommandEnvelopeView
    {
        std::string commandId;
        std::string command;
        std::string scenarioId;
        std::string scenarioName;
        std::string role;
        int stepIndex{ 0 };
        nlohmann::json parameters = nlohmann::json::object();
    };

    bool TryHandleAssignmentMessage(DeviceGameSaveState* state, const std::string& message)
    {
        if (state == nullptr || message.empty() || message.front() != '{')
        {
            return false;
        }

        nlohmann::json doc = nlohmann::json::parse(message, nullptr, false);
        if (doc.is_discarded() || !doc.is_object())
        {
            return false;
        }

        auto typeIt = doc.find("type");
        if (typeIt == doc.end() || !typeIt->is_string())
        {
            return false;
        }

        if (typeIt->get<std::string>() != "deviceAssignment")
        {
            return false;
        }

        auto nameIt = doc.find("deviceName");
        if (nameIt == doc.end() || !nameIt->is_string())
        {
            return false;
        }

        std::string deviceName = nameIt->get<std::string>();
        if (deviceName.empty())
        {
            return false;
        }

        state->inputDeviceId = deviceName;
        EnableFileLoggingForDevice(deviceName);
        UpdateWindowTitleWithDeviceName(deviceName);
        LogToWindow("Assigned device name: " + deviceName);
        return true;
    }

    bool TryParseCommandEnvelope(const std::string& text, CommandEnvelopeView& envelope)
    {
        using nlohmann::json;

        if (text.empty() || text.front() != '{')
        {
            return false;
        }

        json doc = json::parse(text, nullptr, false);
        if (doc.is_discarded() || !doc.is_object())
        {
            return false;
        }

        auto commandIdIt = doc.find("commandId");
        auto commandIt = doc.find("command");
        auto timeoutIt = doc.find("timeoutSeconds");
        auto contextIt = doc.find("scenarioContext");
        auto parametersIt = doc.find("parameters");

        if (commandIdIt == doc.end() || commandIt == doc.end() || timeoutIt == doc.end())
        {
            return false;
        }

        if (!commandIdIt->is_string() || !commandIt->is_string() || !timeoutIt->is_number_integer())
        {
            return false;
        }

        envelope.commandId = commandIdIt->get<std::string>();
        envelope.command = commandIt->get<std::string>();

        if (contextIt != doc.end() && contextIt->is_object())
        {
            auto idIt = contextIt->find("scenarioId");
            auto nameIt = contextIt->find("scenarioName");
            auto roleIt = contextIt->find("role");
            auto stepIndexIt = contextIt->find("stepIndex");
            if (idIt != contextIt->end() && idIt->is_string())
            {
                envelope.scenarioId = idIt->get<std::string>();
            }

            if (nameIt != contextIt->end() && nameIt->is_string())
            {
                envelope.scenarioName = nameIt->get<std::string>();
            }

            if (roleIt != contextIt->end() && roleIt->is_string())
            {
                envelope.role = roleIt->get<std::string>();
            }

            if (stepIndexIt != contextIt->end() && stepIndexIt->is_number_integer())
            {
                envelope.stepIndex = stepIndexIt->get<int>();
            }
        }

        if (parametersIt != doc.end() && parametersIt->is_object())
        {
            envelope.parameters = *parametersIt;
        }
        return true;
    }

    std::string BuildIsoTimestampUtc()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto epochMs = duration_cast<milliseconds>(now.time_since_epoch());
        const auto fractionalMs = epochMs % 1000;

        std::time_t nowTimeT = system_clock::to_time_t(now);
        std::tm tm{};
        gmtime_s(&tm, &nowTimeT);

        char timeBuffer[32] = {};
        std::strftime(timeBuffer, std::size(timeBuffer), "%Y-%m-%dT%H:%M:%S", &tm);

        char result[48] = {};
        std::snprintf(result, std::size(result), "%s.%03lldZ", timeBuffer, static_cast<long long>(fractionalMs.count()));
        return std::string(result);
    }
}

void Sample_GameSave_ConfigureWebSocketLogging(DeviceGameSaveState* state)
{
    if (state == nullptr)
    {
        return;
    }

    state->websocketClient.SetTextMessageHandler([state](const std::string& message)
    {
        if (TryHandleAssignmentMessage(state, message))
        {
            return;
        }

        CommandEnvelopeView envelope{};
        if (TryParseCommandEnvelope(message, envelope))
        {
            std::string deviceId = state->inputDeviceId;
            if (deviceId.empty())
            {
                deviceId = "TestDevice";
            }

            if (!envelope.scenarioId.empty())
            {
                bool newScenario = envelope.scenarioId != state->currentScenarioId;
                if (newScenario)
                {
                    state->currentScenarioId = envelope.scenarioId;
                    state->currentScenarioName = envelope.scenarioName;
                    const std::string friendlyName = state->currentScenarioName.empty()
                        ? state->currentScenarioId
                        : state->currentScenarioName;
                    LogToWindow("Scenario start [" + deviceId + "]: " + friendlyName);
                }
            }

            CommandResultPayload resultPayload = BuildActionResult(
                state,
                envelope.commandId,
                envelope.command,
                envelope.parameters,
                deviceId);

            resultPayload.result["elapsedMs"] = resultPayload.elapsedMs;
            resultPayload.result["timestamp"] = BuildIsoTimestampUtc();

            const std::string payload = resultPayload.result.dump();

            // Log the result status before sending
            std::string status = resultPayload.result.contains("status") ? resultPayload.result["status"].get<std::string>() : "unknown";
            uint32_t resultHr = resultPayload.result.contains("hresult") ? static_cast<uint32_t>(resultPayload.result["hresult"].get<int64_t>()) : 0;
            LogToWindowFormat("ActionResult for %s: status=%s, hresult=0x%08X", 
                envelope.command.c_str(), 
                status.c_str(), 
                resultHr);

            HRESULT sendHr = state->websocketClient.SendText(payload);
            LogToWindowFormatVerbose(true, "ActionResult send (hr=0x%08X)", static_cast<uint32_t>(sendHr));
            if (FAILED(sendHr))
            {
                LogToWindow("Failed to send action result");
            }
        }
        else
        {
            LogToWindow("WebSocket Text: " + message);
        }
    });

    state->websocketClient.SetBinaryMessageHandler([](const std::vector<uint8_t>& payload)
    {
        constexpr size_t previewCount = 8;
        std::ostringstream oss;
        oss << "WebSocket Binary: size=" << payload.size();

        if (!payload.empty())
        {
            oss << " bytes=[";
            size_t count = std::min(payload.size(), previewCount);
            std::ios::fmtflags prevFlags = oss.flags();
            char prevFill = oss.fill('0');
            oss << std::uppercase << std::hex;
            for (size_t i = 0; i < count; ++i)
            {
                if (i > 0)
                {
                    oss << ' ';
                }

                oss << std::setw(2) << static_cast<int>(payload[i]);
            }

            oss.flags(prevFlags);
            oss.fill(prevFill);
            if (payload.size() > previewCount)
            {
                oss << " ...";
            }

            oss << ']';
        }

        LogToWindow(oss.str());
    });

    state->websocketClient.SetClosedHandler([state](HCWebSocketCloseStatus closeStatus)
    {
        LogToWindowFormat("WebSocket closed (status=%u)", static_cast<uint32_t>(closeStatus));
        if (!state->websocketConnectInProgress.load())
        {
            state->websocketLastAttempt = std::chrono::steady_clock::now();
        }
    });

    LogToWindow("WebSocket handlers configured");
}

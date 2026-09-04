#include "pch.h"
#include "DeviceGameSaveState.h"
#include "DeviceWebSocketConnection.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

#include <thread>

namespace
{
    constexpr std::chrono::seconds kWebSocketReconnectInterval{ 2 };
    constexpr std::chrono::seconds kWebSocketConnectTimeout{ 15 };

    const char* GetEngineIdentifier(DeviceEngineType engineType)
    {
        switch (engineType)
        {
        case DeviceEngineType::PcGrts:
            return "pc-grts";
        case DeviceEngineType::PcInproc:
            return "pc-inproc";
        case DeviceEngineType::PcInprocGameSaves:
            return "pc-inproc-gamesaves";
        case DeviceEngineType::Xbox:
            return "xbox";
        case DeviceEngineType::Psx:
            return "psx";
        default:
            return "unknown";
        }
    }

    void BeginWebSocketConnect(DeviceGameSaveState* state)
    {
        if (state == nullptr || state->quit)
        {
            return;
        }

        if (state->websocketClient.IsConnected())
        {
            return;
        }

        bool alreadyInProgress = state->websocketConnectInProgress.exchange(true);
        if (alreadyInProgress)
        {
            return;
        }

        state->websocketLastAttempt = std::chrono::steady_clock::now();

        std::thread([state]()
        {
            std::string websocketUri = "ws://" + state->controllerIpAddress + ":15080/ws/";
            
            // Only log connection attempt on first try
            if (state->websocketFirstConnectAttempt)
            {
                LogToWindow("Connecting to controller at: " + websocketUri);
                if (state->controllerIpAddress == "localhost" || state->controllerIpAddress == "127.0.0.1")
                {
                    LogToWindow("  (To connect to a remote controller, use: /controller <ip-address> or create controllerip.txt)");
                }
            }
            
            HRESULT hr = state->websocketClient.Connect(websocketUri);
            if (SUCCEEDED(hr))
            {
                LogToWindowFormat("WebSocket connect succeeded (hr=0x%08X)", static_cast<uint32_t>(hr));
                state->websocketLastConnectError = S_OK;
                state->websocketFirstConnectAttempt = true; // Reset for next disconnect

                const char* engineIdentifier = GetEngineIdentifier(state->engineType);

                // Build capabilities JSON with registered command list
                nlohmann::json capJson;
                capJson["type"] = "capabilities";
                capJson["engine"] = engineIdentifier;

                auto& registry = CommandRegistry::Instance();
                nlohmann::json commandArray = nlohmann::json::array();
                for (const auto& [name, handler] : registry.GetAll())
                {
                    commandArray.push_back(std::string(name));
                }
                std::sort(commandArray.begin(), commandArray.end());
                capJson["commands"] = commandArray;

                std::string capabilityPayload = capJson.dump();
                HRESULT capabilityHr = state->websocketClient.SendText(capabilityPayload);
                LogToWindowFormat("Capability announce send (hr=0x%08X)", static_cast<uint32_t>(capabilityHr));
                if (FAILED(capabilityHr))
                {
                    LogToWindow("Failed to transmit capability announcement to controller");
                }
            }
            else if (hr != E_FAIL)
            {
                // Only log if error code changed or first attempt
                if (state->websocketFirstConnectAttempt || hr != state->websocketLastConnectError)
                {
                    LogToWindowFormat("WebSocket connect failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                }
                state->websocketLastConnectError = hr;
            }

            state->websocketFirstConnectAttempt = false;
            state->websocketConnectInProgress.store(false);
        }).detach();
    }
}

void PumpWebSocketAutoConnect(DeviceGameSaveState* state)
{
    if (state == nullptr || state->quit)
    {
        return;
    }

    // If an offline batch requested reconnection, force-close the current
    // websocket so the reconnect logic below kicks in. This handles the case
    // where xbstress network=broken killed the TCP stream but the HC websocket
    // never received a clean close frame, leaving m_connected stuck on true.
    if (state->forceWebsocketReconnect.exchange(false))
    {
        LogToWindow("[WSAutoConnect] Force-reconnect requested (offline batch completed). Disconnecting current websocket.");
        state->websocketClient.Disconnect();
    }

    if (state->websocketClient.IsConnected())
    {
        return;
    }

    if (state->websocketConnectInProgress.load())
    {
        // If a connect attempt has been stuck for too long, force-disconnect to unblock it.
        // xbstress network=broken can cause WinHTTP connections to hang indefinitely;
        // after xbstress stop, the hung connection may never complete on its own.
        // We do NOT clear connectInProgress here — the connect thread will do that when
        // it returns. This prevents a race where two connect threads run concurrently.
        const auto now = std::chrono::steady_clock::now();
        if (now - state->websocketLastAttempt > kWebSocketConnectTimeout)
        {
            LogToWindow("[WSAutoConnect] Connect attempt stuck for >15s, forcing disconnect to unblock.");
            state->websocketClient.Disconnect();
            // Reset the timestamp so we wait another cycle before forcing again
            state->websocketLastAttempt = now;
        }
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    if (state->websocketLastAttempt == std::chrono::steady_clock::time_point{} ||
        now - state->websocketLastAttempt >= kWebSocketReconnectInterval)
    {
        BeginWebSocketConnect(state);
    }
}

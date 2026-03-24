#include "pch.h"
#include "DeviceGameSaveState.h"
#include "DeviceWebSocketConnection.h"
#include "DeviceLogging.h"

#include <thread>

namespace
{
    constexpr std::chrono::seconds kWebSocketReconnectInterval{ 2 };

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
            std::string websocketUri = "ws://" + state->controllerIpAddress + ":5000/ws/";
            
            // Only log connection attempt on first try
            if (state->websocketFirstConnectAttempt)
            {
                LogToWindow("Connecting to controller at: " + websocketUri);
                if (state->controllerIpAddress == "localhost" || state->controllerIpAddress == "127.0.0.1")
                {
                    LogToWindow("  (To connect to a remote controller, use: /controller <ip-address>)");
                }
            }
            
            HRESULT hr = state->websocketClient.Connect(websocketUri);
            if (SUCCEEDED(hr))
            {
                LogToWindowFormat("WebSocket connect succeeded (hr=0x%08X)", static_cast<uint32_t>(hr));
                state->websocketLastConnectError = S_OK;
                state->websocketFirstConnectAttempt = true; // Reset for next disconnect

                const char* engineIdentifier = GetEngineIdentifier(state->engineType);
                std::string capabilityPayload = "{\"type\":\"capabilities\",\"engine\":\"";
                capabilityPayload += engineIdentifier;
                capabilityPayload += "\"}";
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

    if (state->websocketClient.IsConnected())
    {
        return;
    }

    if (state->websocketConnectInProgress.load())
    {
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    if (state->websocketLastAttempt == std::chrono::steady_clock::time_point{} ||
        now - state->websocketLastAttempt >= kWebSocketReconnectInterval)
    {
        BeginWebSocketConnect(state);
    }
}

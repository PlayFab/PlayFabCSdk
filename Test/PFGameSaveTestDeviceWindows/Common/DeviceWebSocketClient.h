#pragma once

#include "pch.h"

#include <functional>
#include <string>
#include <vector>

// Lightweight wrapper around libHttpClient's WebSocket APIs so the sample
// can manage connection lifecycle without plumbing the raw C callbacks.
class DeviceWebSocketClient
{
public:
    DeviceWebSocketClient() = default;
    ~DeviceWebSocketClient();

    DeviceWebSocketClient(const DeviceWebSocketClient&) = delete;
    DeviceWebSocketClient& operator=(const DeviceWebSocketClient&) = delete;

    DeviceWebSocketClient(DeviceWebSocketClient&&) = delete;
    DeviceWebSocketClient& operator=(DeviceWebSocketClient&&) = delete;

    HRESULT Initialize();
    void Cleanup();

    HRESULT Connect(const std::string& uri, const std::string& subProtocol = {});
    HRESULT SendText(const std::string& message);
    HRESULT SendBinary(const std::vector<uint8_t>& payload);
    HRESULT Disconnect();

    bool IsConnected() const noexcept { return m_connected; }

    void SetTextMessageHandler(std::function<void(const std::string&)> handler);
    void SetBinaryMessageHandler(std::function<void(const std::vector<uint8_t>&)> handler);
    void SetClosedHandler(std::function<void(HCWebSocketCloseStatus)> handler);

private:
    static void CALLBACK HandleTextMessage(_In_ HCWebsocketHandle websocket, _In_z_ const char* message, _In_ void* context) noexcept;
    static void CALLBACK HandleBinaryMessage(_In_ HCWebsocketHandle websocket, _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes, _In_ uint32_t payloadSize, _In_ void* context) noexcept;
    static void CALLBACK HandleClosed(_In_ HCWebsocketHandle websocket, _In_ HCWebSocketCloseStatus closeStatus, _In_ void* context) noexcept;

    HRESULT EnsureInitialized();
    HRESULT CreateWebSocketHandle();

    void DispatchText(const char* message) noexcept;
    void DispatchBinary(const uint8_t* payloadBytes, uint32_t payloadSize) noexcept;
    void DispatchClosed(HCWebSocketCloseStatus closeStatus) noexcept;

    bool m_initialized{ false };
    bool m_connected{ false };
    bool m_ownsHC{ false };

    XTaskQueueHandle m_queue{ nullptr };
    HCWebsocketHandle m_websocket{ nullptr };

    std::function<void(const std::string&)> m_textHandler;
    std::function<void(const std::vector<uint8_t>&)> m_binaryHandler;
    std::function<void(HCWebSocketCloseStatus)> m_closedHandler;
};

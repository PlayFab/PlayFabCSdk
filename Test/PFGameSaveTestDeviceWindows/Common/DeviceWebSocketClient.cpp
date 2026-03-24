#include "pch.h"
#include "DeviceWebSocketClient.h"

DeviceWebSocketClient::~DeviceWebSocketClient()
{
    Cleanup();
}

HRESULT DeviceWebSocketClient::Initialize()
{
    return EnsureInitialized();
}

void DeviceWebSocketClient::Cleanup()
{
    if (m_ownsHC)
    {
        XAsyncBlock async{};
        async.queue = m_queue;

        if (SUCCEEDED(HCCleanupAsync(&async)))
        {
            (void)XAsyncGetStatus(&async, true);
        }
        m_ownsHC = false;
    }

    if (m_websocket)
    {
        (void)HCWebSocketDisconnect(m_websocket);
        HCWebSocketCloseHandle(m_websocket);
        m_websocket = nullptr;
    }

    if (m_queue)
    {
        XTaskQueueTerminate(m_queue, true, nullptr, nullptr);
        XTaskQueueCloseHandle(m_queue);
        m_queue = nullptr;
    }

    m_initialized = false;
    m_connected = false;

    m_textHandler = nullptr;
    m_binaryHandler = nullptr;
    m_closedHandler = nullptr;
}

HRESULT DeviceWebSocketClient::EnsureInitialized()
{
    if (m_initialized)
    {
        return S_OK;
    }

    if (!HCIsInitialized())
    {
        HRESULT hr = HCInitialize(nullptr);
        if (FAILED(hr))
        {
            if (hr != E_HC_ALREADY_INITIALISED)
            {
                return hr;
            }
        }
        else
        {
            m_ownsHC = true;
        }
    }

    HRESULT hr = XTaskQueueCreate(XTaskQueueDispatchMode::ThreadPool, XTaskQueueDispatchMode::ThreadPool, &m_queue);
    RETURN_IF_FAILED(hr);

    m_initialized = true;
    return S_OK;
}

HRESULT DeviceWebSocketClient::CreateWebSocketHandle()
{
    RETURN_IF_FAILED(EnsureInitialized());

    if (m_websocket)
    {
        (void)HCWebSocketDisconnect(m_websocket);
        HCWebSocketCloseHandle(m_websocket);
        m_websocket = nullptr;
    }

    HRESULT hr = HCWebSocketCreate(&m_websocket, &DeviceWebSocketClient::HandleTextMessage, &DeviceWebSocketClient::HandleBinaryMessage, &DeviceWebSocketClient::HandleClosed, this);
    RETURN_IF_FAILED(hr);

    m_connected = false;
    return S_OK;
}

HRESULT DeviceWebSocketClient::Connect(const std::string& uri, const std::string& subProtocol)
{
    RETURN_IF_FAILED(CreateWebSocketHandle());

    XAsyncBlock async{};
    async.queue = m_queue;

    const char* subProto = subProtocol.empty() ? "" : subProtocol.c_str();

    HRESULT hr = HCWebSocketConnectAsync(uri.c_str(), subProto, m_websocket, &async);
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async, true);
    RETURN_IF_FAILED(hr);

    WebSocketCompletionResult result{};
    hr = HCGetWebSocketConnectResult(&async, &result);
    RETURN_IF_FAILED(hr);
    RETURN_IF_FAILED(result.errorCode);

    m_connected = true;
    return S_OK;
}

HRESULT DeviceWebSocketClient::SendText(const std::string& message)
{
    if (!m_connected)
    {
        return E_UNEXPECTED;
    }

    XAsyncBlock async{};
    async.queue = m_queue;

    HRESULT hr = HCWebSocketSendMessageAsync(m_websocket, message.c_str(), &async);
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async, true);
    RETURN_IF_FAILED(hr);

    WebSocketCompletionResult result{};
    hr = HCGetWebSocketSendMessageResult(&async, &result);
    RETURN_IF_FAILED(hr);
    RETURN_IF_FAILED(result.errorCode);

    return S_OK;
}

HRESULT DeviceWebSocketClient::SendBinary(const std::vector<uint8_t>& payload)
{
    if (!m_connected)
    {
        return E_UNEXPECTED;
    }

    XAsyncBlock async{};
    async.queue = m_queue;

    const uint8_t* data = payload.empty() ? nullptr : payload.data();
    uint32_t size = static_cast<uint32_t>(payload.size());

    HRESULT hr = HCWebSocketSendBinaryMessageAsync(m_websocket, data, size, &async);
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async, true);
    RETURN_IF_FAILED(hr);

    WebSocketCompletionResult result{};
    hr = HCGetWebSocketSendMessageResult(&async, &result);
    RETURN_IF_FAILED(hr);
    RETURN_IF_FAILED(result.errorCode);

    return S_OK;
}

HRESULT DeviceWebSocketClient::Disconnect()
{
    if (!m_websocket)
    {
        return S_OK;
    }

    HRESULT hr = HCWebSocketDisconnect(m_websocket);
    if (FAILED(hr))
    {
        return hr;
    }

    m_connected = false;
    return S_OK;
}

void DeviceWebSocketClient::SetTextMessageHandler(std::function<void(const std::string&)> handler)
{
    m_textHandler = std::move(handler);
}

void DeviceWebSocketClient::SetBinaryMessageHandler(std::function<void(const std::vector<uint8_t>&)> handler)
{
    m_binaryHandler = std::move(handler);
}

void DeviceWebSocketClient::SetClosedHandler(std::function<void(HCWebSocketCloseStatus)> handler)
{
    m_closedHandler = std::move(handler);
}

void CALLBACK DeviceWebSocketClient::HandleTextMessage(_In_ HCWebsocketHandle /*websocket*/, _In_z_ const char* message, _In_ void* context) noexcept
{
    auto* self = static_cast<DeviceWebSocketClient*>(context);
    if (self != nullptr)
    {
        self->DispatchText(message);
    }
}

void CALLBACK DeviceWebSocketClient::HandleBinaryMessage(_In_ HCWebsocketHandle /*websocket*/, _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes, _In_ uint32_t payloadSize, _In_ void* context) noexcept
{
    auto* self = static_cast<DeviceWebSocketClient*>(context);
    if (self != nullptr)
    {
        self->DispatchBinary(payloadBytes, payloadSize);
    }
}

void CALLBACK DeviceWebSocketClient::HandleClosed(_In_ HCWebsocketHandle /*websocket*/, _In_ HCWebSocketCloseStatus closeStatus, _In_ void* context) noexcept
{
    auto* self = static_cast<DeviceWebSocketClient*>(context);
    if (self != nullptr)
    {
        self->DispatchClosed(closeStatus);
    }
}

void DeviceWebSocketClient::DispatchText(const char* message) noexcept
{
    if (!m_textHandler)
    {
        return;
    }

    std::string payload = message ? message : "";
    m_textHandler(payload);
}

void DeviceWebSocketClient::DispatchBinary(const uint8_t* payloadBytes, uint32_t payloadSize) noexcept
{
    if (!m_binaryHandler)
    {
        return;
    }

    std::vector<uint8_t> payload;
    if (payloadBytes != nullptr && payloadSize > 0)
    {
        payload.assign(payloadBytes, payloadBytes + payloadSize);
    }

    m_binaryHandler(payload);
}

void DeviceWebSocketClient::DispatchClosed(HCWebSocketCloseStatus closeStatus) noexcept
{
    m_connected = false;

    if (m_closedHandler)
    {
        m_closedHandler(closeStatus);
    }
}

#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/httpClient/PFHttpClient.h>

extern "C"
{
typedef void
(CALLBACK* PFHCCallPerformFunction)(
    _In_ PFHCCallHandle call,
    _Inout_ XAsyncBlock* asyncBlock,
    _In_opt_ void* context,
    _In_ PFHCPerformEnv env
    );

PF_API PFHCSetHttpCallPerformFunction(
    _In_ PFHCCallPerformFunction performFunc,
    _In_opt_ void* performContext
    ) noexcept;

PF_API PFHCGetHttpCallPerformFunction(
    _Out_ PFHCCallPerformFunction* performFunc,
    _Out_ void** performContext
    ) noexcept;

PF_API PFHCHttpCallGetContext(
    _In_ PFHCCallHandle call,
    _In_ void** context
    ) noexcept;

PF_API PFHCHttpCallSetContext(
    _In_ PFHCCallHandle call,
    _In_opt_ void* context
    ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallRequest Get APIs
//

PF_API PFHCHttpCallRequestGetUrl(
    _In_ PFHCCallHandle call,
    _Outptr_ const char** method,
    _Outptr_ const char** url
    ) noexcept;

PF_API PFHCHttpCallRequestGetRequestBodyBytes(
    _In_ PFHCCallHandle call,
    _Outptr_result_bytebuffer_maybenull_(*requestBodySize) const uint8_t** requestBodyBytes,
    _Out_ uint32_t* requestBodySize
    ) noexcept;

PF_API PFHCHttpCallRequestGetRequestBodyString(
    _In_ PFHCCallHandle call,
    _Outptr_ const char** requestBody
    ) noexcept;

PF_API PFHCHttpCallRequestGetRequestBodyReadFunction(
    _In_ PFHCCallHandle call,
    _Out_ PFHCHttpCallRequestBodyReadFunction* readFunction,
    _Out_ size_t* bodySize,
    _Out_ void** context
    ) noexcept;

PF_API PFHCHttpCallRequestGetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
    ) noexcept;

PF_API PFHCHttpCallRequestGetNumHeaders(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* numHeaders
    ) noexcept;

PF_API PFHCHttpCallRequestGetHeaderAtIndex(
    _In_ PFHCCallHandle call,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
) noexcept;

PF_API PFHCHttpCallRequestGetRetryAllowed(
    _In_opt_ PFHCCallHandle call,
    _Out_ bool* retryAllowed
    ) noexcept;

PF_API PFHCHttpCallRequestGetRetryCacheId(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* retryAfterCacheId
    ) noexcept;

PF_API PFHCHttpCallRequestGetTimeout(
    _In_opt_ PFHCCallHandle call,
    _Out_ uint32_t* timeoutInSeconds
    ) noexcept;

PF_API PFHCHttpCallRequestGetRetryDelay(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t* retryDelayInSeconds
    ) noexcept;

PF_API PFHCHttpCallRequestGetTimeoutWindow(
    _In_opt_ PFHCCallHandle call,
    _Out_ uint32_t* timeoutWindowInSeconds
    ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Get APIs
//

PF_API PFHCHttpCallResponseGetResponseBodyWriteFunction(
    _In_ PFHCCallHandle call,
    _Out_ PFHCHttpCallResponseBodyWriteFunction* writeFunction,
    _Out_ void** context
    ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Set APIs
// 

PF_API PFHCHttpCallResponseSetResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ size_t bodySize
    ) noexcept;

PF_API PFHCHttpCallResponseAppendResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ size_t bodySize
    ) noexcept;

PF_API PFHCHttpCallResponseSetStatusCode(
    _In_ PFHCCallHandle call,
    _In_ uint32_t statusCode
    ) noexcept;

PF_API PFHCHttpCallResponseSetNetworkErrorCode(
    _In_ PFHCCallHandle call,
    _In_ HRESULT networkErrorCode,
    _In_ uint32_t platformNetworkErrorCode
    ) noexcept;

PF_API PFHCHttpCallResponseSetPlatformNetworkErrorMessage(
    _In_ PFHCCallHandle call,
    _In_z_ const char* platformNetworkErrorMessage
    ) noexcept;

PF_API PFHCHttpCallResponseSetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
    ) noexcept;

PF_API PFHCHttpCallResponseSetHeaderWithLength(
    _In_ PFHCCallHandle call,
    _In_reads_(nameSize) const char* headerName,
    _In_ size_t nameSize,
    _In_reads_(valueSize) const char* headerValue,
    _In_ size_t valueSize
    ) noexcept;

#if !HC_NOWEBSOCKETS

/////////////////////////////////////////////////////////////////////////////////////////
// WebSocket Set APIs
// 

typedef HRESULT
(CALLBACK* PFHCWebSocketConnectFunction)(
    _In_z_ const char* uri,
    _In_z_ const char* subProtocol,
    _In_ PFHCWebsocketHandle websocket,
    _Inout_ XAsyncBlock* asyncBlock,
    _In_opt_ void* context,
    _In_ PFHCPerformEnv env
    );

typedef HRESULT
(CALLBACK* PFHCWebSocketSendMessageFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* message,
    _Inout_ XAsyncBlock* asyncBlock,
    _In_opt_ void* context
    );

typedef HRESULT
(CALLBACK* PFHCWebSocketSendBinaryMessageFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes,
    _In_ uint32_t payloadSize,
    _Inout_ XAsyncBlock* asyncBlock,
    _In_opt_ void* context
    );

typedef HRESULT
(CALLBACK* PFHCWebSocketDisconnectFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_ PFHCWebSocketCloseStatus closeStatus,
    _In_opt_ void* context
    );

PF_API PFHCSetWebSocketFunctions(
    _In_ PFHCWebSocketConnectFunction websocketConnectFunc,
    _In_ PFHCWebSocketSendMessageFunction websocketSendMessageFunc,
    _In_ PFHCWebSocketSendBinaryMessageFunction websocketSendBinaryMessageFunc,
    _In_ PFHCWebSocketDisconnectFunction websocketDisconnectFunc,
    _In_opt_ void* context
    ) noexcept;

PF_API PFHCGetWebSocketFunctions(
    _Out_ PFHCWebSocketConnectFunction* websocketConnectFunc,
    _Out_ PFHCWebSocketSendMessageFunction* websocketSendMessageFunc,
    _Out_ PFHCWebSocketSendBinaryMessageFunction* websocketSendBinaryMessageFunc,
    _Out_ PFHCWebSocketDisconnectFunction* websocketDisconnectFunc,
    _Out_ void** context
    ) noexcept;

PF_API PFHCWebSocketGetProxyUri(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ const char** proxyUri
    ) noexcept;

PF_API PFHCWebSocketGetHeader(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
    ) noexcept;

PF_API PFHCWebSocketGetNumHeaders(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ uint32_t* numHeaders
    ) noexcept;


PF_API PFHCWebSocketGetHeaderAtIndex(
    _In_ PFHCWebsocketHandle websocket,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
    ) noexcept;

#endif // !HC_NOWEBSOCKETS
}

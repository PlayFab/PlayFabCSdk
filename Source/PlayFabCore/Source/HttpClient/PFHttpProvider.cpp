#include "stdafx.h"
#include <playfab/httpClient/PFHttpProvider.h>
#include <httpClient/httpProvider.h>


PF_API PFHCSetHttpCallPerformFunction(
    _In_ PFHCCallPerformFunction performFunc,
    _In_opt_ void* performContext
) noexcept
{
    return HCSetHttpCallPerformFunction(
        performFunc, 
        performContext
    );
}

PF_API PFHCGetHttpCallPerformFunction(
    _Out_ PFHCCallPerformFunction* performFunc,
    _Out_ void** performContext
) noexcept
{
    return HCGetHttpCallPerformFunction(
        performFunc, 
        performContext
    );
}

PF_API PFHCHttpCallGetContext(
    _In_ PFHCCallHandle call,
    _In_ void** context
) noexcept
{
    return HCHttpCallGetContext(
        call, 
        context
    );
}

PF_API PFHCHttpCallSetContext(
    _In_ PFHCCallHandle call,
    _In_opt_ void* context
) noexcept
{
    return HCHttpCallSetContext(
        call, 
        context
    );
}

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallRequest Get APIs
//

PF_API PFHCHttpCallRequestGetUrl(
    _In_ PFHCCallHandle call,
    _Outptr_ const char** method,
    _Outptr_ const char** url
) noexcept
{
    return HCHttpCallRequestGetUrl(
        call, 
        method, 
        url
    );
}

PF_API PFHCHttpCallRequestGetRequestBodyBytes(
    _In_ PFHCCallHandle call,
    _Outptr_result_bytebuffer_maybenull_(*requestBodySize) const uint8_t** requestBodyBytes,
    _Out_ uint32_t* requestBodySize
) noexcept
{
    return HCHttpCallRequestGetRequestBodyBytes(
        call, 
        requestBodyBytes, 
        requestBodySize
    );
}

PF_API PFHCHttpCallRequestGetRequestBodyString(
    _In_ PFHCCallHandle call,
    _Outptr_ const char** requestBody
) noexcept
{
    return HCHttpCallRequestGetRequestBodyString(
        call, 
        requestBody
    );
}

PF_API PFHCHttpCallRequestGetRequestBodyReadFunction(
    _In_ PFHCCallHandle call,
    _Out_ PFHCHttpCallRequestBodyReadFunction* readFunction,
    _Out_ size_t* bodySize,
    _Out_ void** context
) noexcept
{
    return HCHttpCallRequestGetRequestBodyReadFunction(
        call, 
        readFunction, 
        bodySize, 
        context
    );
}

PF_API PFHCHttpCallRequestGetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCHttpCallRequestGetHeader(
        call, 
        headerName, 
        headerValue
    );
}

PF_API PFHCHttpCallRequestGetNumHeaders(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* numHeaders
) noexcept
{
    return HCHttpCallRequestGetNumHeaders(
        call, 
        numHeaders
    );
}

PF_API PFHCHttpCallRequestGetHeaderAtIndex(
    _In_ PFHCCallHandle call,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCHttpCallRequestGetHeaderAtIndex(
        call, 
        headerIndex, 
        headerName, 
        headerValue
    );
}

PF_API PFHCHttpCallRequestGetRetryAllowed(
    _In_opt_ PFHCCallHandle call,
    _Out_ bool* retryAllowed
) noexcept
{
    return HCHttpCallRequestGetRetryAllowed(
        call, 
        retryAllowed
    );
}

PF_API PFHCHttpCallRequestGetRetryCacheId(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* retryAfterCacheId
) noexcept
{
    return HCHttpCallRequestGetRetryCacheId(
        call, 
        retryAfterCacheId
    );
}

PF_API PFHCHttpCallRequestGetTimeout(
    _In_opt_ PFHCCallHandle call,
    _Out_ uint32_t* timeoutInSeconds
) noexcept
{
    return HCHttpCallRequestGetTimeout(
        call, 
        timeoutInSeconds
    );
}

PF_API PFHCHttpCallRequestGetRetryDelay(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t* retryDelayInSeconds
) noexcept
{
    return HCHttpCallRequestGetRetryDelay(
        call, 
        retryDelayInSeconds
    );
}

PF_API PFHCHttpCallRequestGetTimeoutWindow(
    _In_opt_ PFHCCallHandle call,
    _Out_ uint32_t* timeoutWindowInSeconds
) noexcept
{
    return HCHttpCallRequestGetTimeoutWindow(
        call, 
        timeoutWindowInSeconds
    );
}

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Get APIs
//

PF_API PFHCHttpCallResponseGetResponseBodyWriteFunction(
    _In_ PFHCCallHandle call,
    _Out_ PFHCHttpCallResponseBodyWriteFunction* writeFunction,
    _Out_ void** context
) noexcept
{
    return HCHttpCallResponseGetResponseBodyWriteFunction(
        call, 
        writeFunction, 
        context
    );
}

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Set APIs
// 

PF_API PFHCHttpCallResponseSetResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ size_t bodySize
) noexcept
{
    return HCHttpCallResponseSetResponseBodyBytes(
        call, 
        bodyBytes, 
        bodySize
    );
}

PF_API PFHCHttpCallResponseAppendResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ size_t bodySize
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(call);
    return HCHttpCallResponseAppendResponseBodyBytes(
        call, 
        bodyBytes, 
        bodySize
    );
}

PF_API PFHCHttpCallResponseSetStatusCode(
    _In_ PFHCCallHandle call,
    _In_ uint32_t statusCode
) noexcept
{
    return HCHttpCallResponseSetStatusCode(
        call, 
        statusCode
    );
}

PF_API PFHCHttpCallResponseSetNetworkErrorCode(
    _In_ PFHCCallHandle call,
    _In_ HRESULT networkErrorCode,
    _In_ uint32_t platformNetworkErrorCode
) noexcept
{
    return HCHttpCallResponseSetNetworkErrorCode(
        call, 
        networkErrorCode, 
        platformNetworkErrorCode
    );
}

PF_API PFHCHttpCallResponseSetPlatformNetworkErrorMessage(
    _In_ PFHCCallHandle call,
    _In_z_ const char* platformNetworkErrorMessage
) noexcept
{
    return HCHttpCallResponseSetPlatformNetworkErrorMessage(
        call, 
        platformNetworkErrorMessage
    );
}

PF_API PFHCHttpCallResponseSetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
) noexcept
{
    return HCHttpCallResponseSetHeader(
        call, 
        headerName, 
        headerValue
    );
}

PF_API PFHCHttpCallResponseSetHeaderWithLength(
    _In_ PFHCCallHandle call,
    _In_reads_(nameSize) const char* headerName,
    _In_ size_t nameSize,
    _In_reads_(valueSize) const char* headerValue,
    _In_ size_t valueSize
) noexcept
{
    return HCHttpCallResponseSetHeaderWithLength(
        call, 
        headerName, 
        nameSize, 
        headerValue, 
        valueSize
    );
}

#if !HC_NOWEBSOCKETS

/////////////////////////////////////////////////////////////////////////////////////////
// WebSocket Set APIs
// 


PF_API PFHCSetWebSocketFunctions(
    _In_ PFHCWebSocketConnectFunction websocketConnectFunc,
    _In_ PFHCWebSocketSendMessageFunction websocketSendMessageFunc,
    _In_ PFHCWebSocketSendBinaryMessageFunction websocketSendBinaryMessageFunc,
    _In_ PFHCWebSocketDisconnectFunction websocketDisconnectFunc,
    _In_opt_ void* context
) noexcept
{
    return HCSetWebSocketFunctions(
        websocketConnectFunc, 
        websocketSendMessageFunc, 
        websocketSendBinaryMessageFunc, 
        (HCWebSocketDisconnectFunction)websocketDisconnectFunc, 
        context
    );
}

PF_API PFHCGetWebSocketFunctions(
    _Out_ PFHCWebSocketConnectFunction* websocketConnectFunc,
    _Out_ PFHCWebSocketSendMessageFunction* websocketSendMessageFunc,
    _Out_ PFHCWebSocketSendBinaryMessageFunction* websocketSendBinaryMessageFunc,
    _Out_ PFHCWebSocketDisconnectFunction* websocketDisconnectFunc,
    _Out_ void** context
) noexcept
{
    return HCGetWebSocketFunctions(
        websocketConnectFunc, 
        websocketSendMessageFunc, 
        websocketSendBinaryMessageFunc, 
        (HCWebSocketDisconnectFunction*)websocketDisconnectFunc, 
        context
    );
}

PF_API PFHCWebSocketGetProxyUri(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ const char** proxyUri
) noexcept
{
    return HCWebSocketGetProxyUri(
        websocket, 
        proxyUri
    );
}

PF_API PFHCWebSocketGetHeader(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCWebSocketGetHeader(
        websocket, 
        headerName, 
        headerValue
    );
}

PF_API PFHCWebSocketGetNumHeaders(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ uint32_t* numHeaders
) noexcept
{
    return HCWebSocketGetNumHeaders(
        websocket, 
        numHeaders
    );
}


PF_API PFHCWebSocketGetHeaderAtIndex(
    _In_ PFHCWebsocketHandle websocket,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCWebSocketGetHeaderAtIndex(
        websocket, 
        headerIndex, 
        headerName, 
        headerValue
    );
}

#endif // !HC_NOWEBSOCKETS
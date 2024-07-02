#include "stdafx.h"
#include <playfab/httpClient/PFHttpClient.h>
#include <httpClient/httpClient.h>

PF_API PFHCMemSetFunctions(
    _In_opt_ PFHCMemAllocFunction memAllocFunc,
    _In_opt_ PFHCMemFreeFunction memFreeFunc
) noexcept
{
    return HCMemSetFunctions(
        memAllocFunc, 
        memFreeFunc
    );
}

PF_API PFHCMemGetFunctions(
    _Out_ PFHCMemAllocFunction* memAllocFunc,
    _Out_ PFHCMemFreeFunction* memFreeFunc
) noexcept
{
    return HCMemGetFunctions(
        memAllocFunc,
        memFreeFunc
    );
}

//Global APIs

PF_API PFHCInitialize(
    _In_opt_ PFHCInitArgs* args
) noexcept
{
    UNREFERENCED_PARAMETER(args);
    PFHCIsInitialized(); // empty functions don't export with non-mangled names, so make a trivial function call
    return S_OK; // ignore this call
}

PF_API_(bool) PFHCIsInitialized() noexcept
{
    return HCIsInitialized();
}

PF_API PFHCCleanupAsync(
    _In_ XAsyncBlock* async
) noexcept
{
    UNREFERENCED_PARAMETER(async);
    PFHCIsInitialized(); // empty functions don't export with non-mangled names, so make a trivial function call
    return S_OK; // ignore this call
}

PF_API PFHCGetLibVersion(_Outptr_ const char** version) noexcept
{
    return HCGetLibVersion(version);
}

PF_API_(int32_t) PFHCAddCallRoutedHandler(
    _In_ PFHCCallRoutedHandler handler,
    _In_opt_ void* context
) noexcept
{
    return HCAddCallRoutedHandler(
        handler,
        context
    );
}

PF_API_(void) PFHCRemoveCallRoutedHandler(
    _In_ int32_t handlerId
) noexcept
{
    HCRemoveCallRoutedHandler(handlerId);
}

PF_API PFHCSetGlobalProxy(_In_ const char* proxyUri) noexcept
{
    return HCSetGlobalProxy(proxyUri);
}

//Http APIs

PF_API PFHCHttpCallCreate(
    _Out_ PFHCCallHandle* call
) noexcept
{
    return HCHttpCallCreate(call);
}

PF_API PFHCHttpCallPerformAsync(
    _In_ PFHCCallHandle call,
    _Inout_ XAsyncBlock* asyncBlock
) noexcept
{
    return HCHttpCallPerformAsync(
        call, 
        asyncBlock
    );
}

PF_API_(PFHCCallHandle) PFHCHttpCallDuplicateHandle(
    _In_ PFHCCallHandle call
) noexcept
{
    return HCHttpCallDuplicateHandle(call);
}

PF_API PFHCHttpCallCloseHandle(
    _In_ PFHCCallHandle call
) noexcept
{
    return HCHttpCallCloseHandle(call);
}

PF_API_(uint64_t) PFHCHttpCallGetId(
    _In_ PFHCCallHandle call
) noexcept
{
    return HCHttpCallGetId(call);
}

PF_API PFHCHttpCallSetTracing(
    _In_ PFHCCallHandle call,
    _In_ bool traceCall
) noexcept
{
    return HCHttpCallSetTracing(
        call, 
        traceCall
    );
}

PF_API PFHCHttpCallGetRequestUrl(
    _In_ PFHCCallHandle call,
    _Out_ const char** url
) noexcept
{
    return HCHttpCallGetRequestUrl(
        call, 
        url
    );
}

//HttpCallRequest Set APIs

PF_API PFHCHttpCallRequestSetUrl(
    _In_ PFHCCallHandle call,
    _In_z_ const char* method,
    _In_z_ const char* url
) noexcept
{
    return HCHttpCallRequestSetUrl(
        call,
        method,
        url
    );
}

PF_API PFHCHttpCallRequestSetRequestBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(requestBodySize) const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize
) noexcept
{
    return HCHttpCallRequestSetRequestBodyBytes(
        call, 
        requestBodyBytes, 
        requestBodySize
    );
}

PF_API PFHCHttpCallRequestSetRequestBodyString(
    _In_ PFHCCallHandle call,
    _In_z_ const char* requestBodyString
) noexcept
{
    return HCHttpCallRequestSetRequestBodyString(
        call,
        requestBodyString
    );
}

PF_API PFHCHttpCallRequestSetRequestBodyReadFunction(
    _In_ PFHCCallHandle call,
    _In_ PFHCHttpCallRequestBodyReadFunction readFunction,
    _In_ size_t bodySize,
    _In_opt_ void* context
) noexcept
{
    return HCHttpCallRequestSetRequestBodyReadFunction(
        call, 
        readFunction, 
        bodySize, 
        context
    );
}

PF_API PFHCHttpCallRequestSetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue,
    _In_ bool allowTracing
) noexcept
{
    return HCHttpCallRequestSetHeader(
        call, 
        headerName, 
        headerValue, 
        allowTracing
    );
}

PF_API PFHCHttpCallRequestSetRetryAllowed(
    _In_opt_ PFHCCallHandle call,
    _In_ bool retryAllowed
) noexcept
{
    return HCHttpCallRequestSetRetryAllowed(
        call, 
        retryAllowed
    );
}

PF_API PFHCHttpCallRequestSetRetryCacheId(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t retryAfterCacheId
) noexcept
{
    return HCHttpCallRequestSetRetryCacheId(
        call, 
        retryAfterCacheId
    );
}

PF_API PFHCHttpCallRequestSetTimeout(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t timeoutInSeconds
) noexcept
{
    return HCHttpCallRequestSetTimeout(
        call, 
        timeoutInSeconds
    );
}

PF_API PFHCHttpCallRequestSetRetryDelay(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t retryDelayInSeconds
) noexcept
{
    return HCHttpCallRequestSetRetryDelay(
        call, 
        retryDelayInSeconds
    );
}

PF_API PFHCHttpCallRequestSetTimeoutWindow(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t timeoutWindowInSeconds
) noexcept
{
    return HCHttpCallRequestSetTimeoutWindow(
        call, 
        timeoutWindowInSeconds
    );
}

#if HC_PLATFORM_IS_MICROSOFT && (HC_PLATFORM != HC_PLATFORM_UWP) && (HC_PLATFORM != HC_PLATFORM_XDK)
PF_API PFHCHttpCallRequestSetSSLValidation(
    _In_ PFHCCallHandle call,
    _In_ bool sslValidation
) noexcept
{
    return HCHttpCallRequestSetSSLValidation(call, sslValidation);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFHCHttpDisableAssertsForSSLValidationInDevSandboxes(
    _In_ PFHCConfigSetting setting
) noexcept
{
    return HCHttpDisableAssertsForSSLValidationInDevSandboxes((HCConfigSetting)setting);
}
#endif

//HttpCallResponse Set APIs

PF_API PFHCHttpCallResponseSetResponseBodyWriteFunction(
    _In_ PFHCCallHandle call,
    _In_ PFHCHttpCallResponseBodyWriteFunction writeFunction,
    _In_opt_ void* context
) noexcept
{
    return HCHttpCallResponseSetResponseBodyWriteFunction(
        call, 
        writeFunction, 
        context
    );
}

//HttpCallResponse Get APIs

PF_API PFHCHttpCallResponseGetResponseString(
    _In_ PFHCCallHandle call,
    _Out_ const char** responseString
) noexcept
{
    return HCHttpCallResponseGetResponseString(
        call, 
        responseString
    );
}

PF_API PFHCHttpCallResponseGetResponseBodyBytesSize(
    _In_ PFHCCallHandle call,
    _Out_ size_t* bufferSize
) noexcept
{
    return HCHttpCallResponseGetResponseBodyBytesSize(
        call, 
        bufferSize
    );
}

PF_API PFHCHttpCallResponseGetResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) uint8_t* buffer,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return HCHttpCallResponseGetResponseBodyBytes(
        call, 
        bufferSize, 
        buffer, 
        bufferUsed
    );
}

PF_API PFHCHttpCallResponseGetStatusCode(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* statusCode
) noexcept
{
    return HCHttpCallResponseGetStatusCode(
        call, 
        statusCode
    );
}

PF_API PFHCHttpCallResponseGetNetworkErrorCode(
    _In_ PFHCCallHandle call,
    _Out_ HRESULT* networkErrorCode,
    _Out_ uint32_t* platformNetworkErrorCode
) noexcept
{
    return HCHttpCallResponseGetNetworkErrorCode(
        call, 
        networkErrorCode, 
        platformNetworkErrorCode
    );
}

PF_API PFHCHttpCallResponseGetPlatformNetworkErrorMessage(
    _In_ PFHCCallHandle call,
    _Out_ const char** platformNetworkErrorMessage
) noexcept
{
    return HCHttpCallResponseGetPlatformNetworkErrorMessage(
        call, 
        platformNetworkErrorMessage
    );
}

PF_API PFHCHttpCallResponseGetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCHttpCallResponseGetHeader(
        call, 
        headerName, 
        headerValue
    );
}

PF_API PFHCHttpCallResponseGetNumHeaders(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* numHeaders
) noexcept
{
    return HCHttpCallResponseGetNumHeaders(
        call, 
        numHeaders
    );
}

PF_API PFHCHttpCallResponseGetHeaderAtIndex(
    _In_ PFHCCallHandle call,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
) noexcept
{
    return HCHttpCallResponseGetHeaderAtIndex(
        call, 
        headerIndex, 
        headerName, 
        headerValue
    );
}

PF_API PFHCHttpCallRequestEnableGzipCompression(
    _In_ PFHCCallHandle call,
    _In_ PFHCCompressionLevel level
) noexcept
{
    return HCHttpCallRequestEnableGzipCompression(
        call,
        (HCCompressionLevel)level
    );
}

PF_API PFHCHttpCallResponseSetGzipCompressed(
    _In_ PFHCCallHandle call,
    _In_ bool compress
) noexcept 
{
    return HCHttpCallResponseSetGzipCompressed(
        call,
        compress
    );
}


#if !HC_NOWEBSOCKETS

// WebSocket APIs

PF_API PFHCWebSocketCreate(
    _Out_ PFHCWebsocketHandle* websocket,
    _In_opt_ PFHCWebSocketMessageFunction messageFunc,
    _In_opt_ PFHCWebSocketBinaryMessageFunction binaryMessageFunc,
    _In_opt_ PFHCWebSocketCloseEventFunction closeFunc,
    _In_opt_ void* functionContext
) noexcept
{
    return HCWebSocketCreate(
        websocket, 
        messageFunc, 
        binaryMessageFunc, 
        (HCWebSocketCloseEventFunction)closeFunc,
        functionContext
    );
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

PF_API PFHCWebSocketSetBinaryMessageFragmentEventFunction(
    _In_ PFHCWebsocketHandle websocket,
    _In_ PFHCWebSocketBinaryMessageFragmentFunction binaryMessageFragmentFunc
) noexcept
{
    return HCWebSocketSetBinaryMessageFragmentEventFunction(
        websocket, 
        binaryMessageFragmentFunc
    );
}
#endif

PF_API PFHCWebSocketSetProxyUri(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* proxyUri
) noexcept
{
    return HCWebSocketSetProxyUri(
        websocket, 
        proxyUri
    );
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 && !HC_WINHTTP_WEBSOCKETS

PF_API PFHCWebSocketSetProxyDecryptsHttps(
    _In_ PFHCWebsocketHandle websocket,
    _In_ bool allowProxyToDecryptHttps
) noexcept
{
    return HCWebSocketSetProxyDecryptsHttps(
        websocket, 
        allowProxyToDecryptHttps
    );
}
#endif

PF_API PFHCWebSocketSetHeader(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
) noexcept
{
    return HCWebSocketSetHeader(
        websocket, 
        headerName, 
        headerValue
    );
}

PF_API PFHCWebSocketGetEventFunctions(
    _In_ PFHCWebsocketHandle websocket,
    _Out_opt_ PFHCWebSocketMessageFunction* messageFunc,
    _Out_opt_ PFHCWebSocketBinaryMessageFunction* binaryMessageFunc,
    _Out_opt_ PFHCWebSocketCloseEventFunction* closeFunc,
    _Out_ void** functionContext
) noexcept
{
    return HCWebSocketGetEventFunctions(
        websocket, 
        messageFunc, 
        binaryMessageFunc, 
        (HCWebSocketCloseEventFunction*)closeFunc,
        functionContext
    );
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

PF_API PFHCWebSocketGetBinaryMessageFragmentEventFunction(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ PFHCWebSocketBinaryMessageFragmentFunction* binaryMessageFragmentFunc,
    _Out_ void** functionContext
) noexcept
{
    return HCWebSocketGetBinaryMessageFragmentEventFunction(
        websocket, 
        binaryMessageFragmentFunc, 
        functionContext
    );
}
#endif

PF_API PFHCWebSocketConnectAsync(
    _In_z_ const char* uri,
    _In_z_ const char* subProtocol,
    _In_ PFHCWebsocketHandle websocket,
    _Inout_ XAsyncBlock* asyncBlock
) noexcept
{
    return HCWebSocketConnectAsync(
        uri, 
        subProtocol, 
        websocket, 
        asyncBlock
    );
}

PF_API PFHCGetWebSocketConnectResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ PFWebSocketCompletionResult* result
) noexcept
{
    return HCGetWebSocketConnectResult(
        asyncBlock, 
        (WebSocketCompletionResult*)result
    );
}

PF_API PFHCWebSocketSendMessageAsync(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* message,
    _Inout_ XAsyncBlock* asyncBlock
) noexcept
{
    return HCWebSocketSendMessageAsync(
        websocket, 
        message, 
        asyncBlock
    );
}

PF_API PFHCWebSocketSendBinaryMessageAsync(
    _In_ PFHCWebsocketHandle websocket,
    _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes,
    _In_ uint32_t payloadSize,
    _Inout_ XAsyncBlock* asyncBlock
) noexcept
{
    return HCWebSocketSendBinaryMessageAsync(
        websocket, 
        payloadBytes, 
        payloadSize, 
        asyncBlock
    );
}

PF_API PFHCGetWebSocketSendMessageResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ PFWebSocketCompletionResult* result
) noexcept
{
    return HCGetWebSocketSendMessageResult(
        asyncBlock, 
        (WebSocketCompletionResult*)result
    );
}

PF_API PFHCWebSocketDisconnect(
    _In_ PFHCWebsocketHandle websocket
) noexcept
{
    return HCWebSocketDisconnect(websocket);
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFHCWebSocketSetMaxReceiveBufferSize(
    _In_ PFHCWebsocketHandle websocket,
    _In_ size_t bufferSizeInBytes
) noexcept
{
    return HCWebSocketSetMaxReceiveBufferSize(
        websocket, 
        bufferSizeInBytes
    );
}
#endif

PF_API_(PFHCWebsocketHandle) PFHCWebSocketDuplicateHandle(
    _In_ PFHCWebsocketHandle websocket
) noexcept
{
    return HCWebSocketDuplicateHandle(websocket);
}

PF_API PFHCWebSocketCloseHandle(
    _In_ PFHCWebsocketHandle websocket
) noexcept
{
    return HCWebSocketCloseHandle(websocket);
}

PF_API_(int32_t) PFHCAddWebSocketRoutedHandler(
    _In_ PFHCWebSocketRoutedHandler handler,
    _In_opt_ void* context
) noexcept
{
    return HCAddWebSocketRoutedHandler(
        handler, 
        context
    );
}

PF_API_(void) PFHCRemoveWebSocketRoutedHandler(
    _In_ int32_t handlerId
) noexcept
{
    HCRemoveWebSocketRoutedHandler(handlerId);
}

#endif // !HC_NOWEBSOCKETS

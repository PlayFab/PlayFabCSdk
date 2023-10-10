#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>

#if HC_PLATFORM == HC_PLATFORM_ANDROID
#include "jni.h"
#endif

extern "C"
{
/////////////////////////////////////////////////////////////////////////////////////////
// Memory APIs
//

/// <summary>
/// A callback invoked every time a new memory buffer must be dynamically allocated by the library.
/// This callback is optionally installed by calling PFHCMemSetFunctions()
/// 
/// The callback must allocate and return a pointer to a contiguous block of memory of the 
/// specified size that will remain valid until the app's corresponding PFHCMemFreeFunction 
/// callback is invoked to release it.
/// 
/// Every non-null pointer returned by this method will be subsequently passed to the corresponding
/// PFHCMemFreeFunction callback once the memory is no longer needed.
/// </summary>
/// <returns>A pointer to an allocated block of memory of the specified size, or a null 
/// pointer if allocation failed.</returns>
/// <param name="size">The size of the allocation to be made. This value will never be zero.</param>
/// <param name="memoryType">An opaque identifier representing the internal category of 
/// memory being allocated.</param>
typedef _Ret_maybenull_ _Post_writable_byte_size_(size) void*
(STDAPIVCALLTYPE* PFHCMemAllocFunction)(
    _In_ size_t size,
    _In_ PFHCMemoryType memoryType
    );

/// <summary>
/// A callback invoked every time a previously allocated memory buffer is no longer needed by 
/// the library and can be freed. This callback is optionally installed by calling PFHCMemSetFunctions()
///
/// The callback is invoked whenever the library has finished using a memory buffer previously 
/// returned by the app's corresponding PFHCMemAllocFunction such that the application can free the
/// memory buffer.
/// </summary>
/// <param name="pointer">The pointer to the memory buffer previously allocated. This value will
/// never be a null pointer.</param>
/// <param name="memoryType">An opaque identifier representing the internal category of 
/// memory being allocated.</param>
typedef void
(STDAPIVCALLTYPE* PFHCMemFreeFunction)(
    _In_ _Post_invalid_ void* pointer,
    _In_ PFHCMemoryType memoryType
    );

/// <summary>
/// Optionally sets the memory hook functions to allow callers to control route memory 
/// allocations to their own memory manager.
/// </summary>
/// <param name="memAllocFunc">A pointer to the custom allocation callback to use, or a null 
/// pointer to restore the default.</param>
/// <param name="memFreeFunc">A pointer to the custom freeing callback to use, or a null 
/// pointer to restore the default.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_HC_ALREADY_INITIALIZED.</returns>
/// <remarks>
/// This must be called before PFHCInitialize().
/// and can not be called again until PFHCCleanup().
///
/// This method allows the application to install custom memory allocation routines in order 
/// to service all requests for new memory buffers instead of using default allocation routines.
///
/// The <paramref name="memAllocFunc" /> and <paramref name="memFreeFunc" /> parameters can be null
/// pointers to restore the default routines. Both callback pointers must be null or both must 
/// be non-null. Mixing custom and default routines is not permitted.
/// </remarks>
PF_API PFHCMemSetFunctions(
    _In_opt_ PFHCMemAllocFunction memAllocFunc,
    _In_opt_ PFHCMemFreeFunction memFreeFunc
    ) noexcept;

/// <summary>
/// Gets the memory hook functions to allow callers to control route memory allocations to their 
/// own memory manager.  
/// </summary>
/// <param name="memAllocFunc">Set to the current allocation callback.  Returns the default routine 
/// if not previously set.</param>
/// <param name="memFreeFunc">Set to the to the current memory free callback.  Returns the default 
/// routine if not previously set.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// This method allows the application get the default memory allocation routines.
/// This can be used along with PFHCMemSetFunctions() to monitor all memory allocations.
/// </remarks>
PF_API PFHCMemGetFunctions(
    _Out_ PFHCMemAllocFunction* memAllocFunc,
    _Out_ PFHCMemFreeFunction* memFreeFunc
    ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// Global APIs
// 

/// <summary>
/// Deprecated. Not needed. 
/// </summary>
typedef struct PFHCInitArgs
{
    /// <summary>
    /// Deprecated. Not needed.
    /// </summary>
    void* dummy;
} PFHCInitArgs;

/// <summary>
/// Deprecated. Not needed. Call PFInitialize instead
/// </summary>
PF_API_DEPRECATED PFHCInitialize(
    _In_opt_ PFHCInitArgs* args
    ) noexcept;

/// <summary>
/// Returns true if library has been initialized by PFHCInitialize
/// </summary>
/// <returns>Returns true if this library has been initialized, otherwise returns false.</returns>
PF_API_(bool) PFHCIsInitialized() noexcept;

/// <summary>
/// Deprecated. Not needed. Call PFUninitializeAsync instead
/// </summary>
PF_API_DEPRECATED PFHCCleanupAsync(
    _In_ XAsyncBlock* async
    ) noexcept;

/// <summary>
/// Returns the version of the library.
/// </summary>
/// <param name="version">The UTF-8 encoded version of the library in the format of release_year.release_month.date.rev</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
PF_API PFHCGetLibVersion(_Outptr_ const char** version) noexcept;

/// <summary>
/// A callback that will be synchronously invoked each time an HTTP call is performed
/// </summary>
/// <param name="call">Handle to the HTTP call.</param>
/// <param name="context">Client context pass when the handler was added.</param>
typedef void
(STDAPIVCALLTYPE* PFHCCallRoutedHandler)(
    _In_ PFHCCallHandle call,
    _In_opt_ void* context
    );

/// <summary>
/// Adds a callback to be invoked on intermediate http calls in order to debug or trace the traffic.
/// </summary>
/// <param name="handler">The handler to be called.</param>
/// <param name="context">Client context to pass to callback function.</param>
/// <returns>An unique id that can be used to remove the handler.</returns>
PF_API_(int32_t) PFHCAddCallRoutedHandler(
    _In_ PFHCCallRoutedHandler handler,
    _In_opt_ void* context
    ) noexcept;

/// <summary>
/// Removes a previously added PFHCCallRoutedHandler.
/// </summary>
/// <param name="handlerId">Id returned from the PFHCAddCallRoutedHandler call.</param>
/// <returns></returns>
PF_API_(void) PFHCRemoveCallRoutedHandler(
    _In_ int32_t handlerId
    ) noexcept;

/// <summary>
/// Manually sets an explicit proxy address.
/// </summary>
/// <param name="proxyUri">The proxy address to use in the "[ip]:[port]" format.</param> 
/// <returns>Result code for this API operation. Possible values are S_OK, E_HC_NOT_INITIALISED, or E_FAIL.</returns>
/// <remarks> If it is passed a null proxy, it will reset to default. Does not include proxying web socket traffic.</remarks>
PF_API PFHCSetGlobalProxy(_In_ const char* proxyUri) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// Http APIs
//

/// <summary>
/// Creates an HTTP call handle.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// First create a HTTP handle using PFHCHttpCallCreate().
/// Then call PFHCHttpCallRequestSet*() to prepare the PFHCCallHandle.
/// Then call PFHCHttpCallPerformAsync() to perform HTTP call using the PFHCCallHandle.
/// This call is asynchronous, so the work will be done on a background thread and will return via the callback.
///
/// The perform call is asynchronous, so the work will be done on a background thread which calls 
/// XTaskQueueDispatch( ..., XTaskQueuePort::Work ).  
///
/// The results will return to the callback on the thread that calls 
/// XTaskQueueDispatch( ..., XTaskQueuePort::Completion ), then get the result of the HTTP call by calling 
/// PFHCHttpCallResponseGet*() to get the HTTP response of the PFHCCallHandle.
/// 
/// When the PFHCCallHandle is no longer needed, call PFHCHttpCallCloseHandle() to free the 
/// memory associated with the PFHCCallHandle.
/// </remarks>
PF_API PFHCHttpCallCreate(
    _Out_ PFHCCallHandle* call
    ) noexcept;

/// <summary>
/// Perform HTTP call using the PFHCCallHandle.
/// </summary>
/// <param name="call">The handle of the HTTP call</param>
/// <param name="asyncBlock">The XAsyncBlock that defines the async operation</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>
/// First create a HTTP handle using PFHCHttpCallCreate().
/// Then call PFHCHttpCallRequestSet*() to prepare the PFHCCallHandle.
/// Then call PFHCHttpCallPerformAsync() to perform HTTP call using the PFHCCallHandle.
/// This call is asynchronous, so the work will be done on a background thread and will return via the callback.
///
/// The perform call is asynchronous, so the work will be done on a background thread which calls 
/// XTaskQueueDispatch( ..., XTaskQueuePort::Work ).  
///
/// The results will return to the callback on the thread that calls 
/// XTaskQueueDispatch( ..., XTaskQueuePort::Completion ), then get the result of the HTTP call by calling 
/// PFHCHttpCallResponseGet*() to get the HTTP response of the PFHCCallHandle.
/// 
/// When the PFHCCallHandle is no longer needed, call PFHCHttpCallCloseHandle() to free the 
/// memory associated with the PFHCCallHandle.
///
/// PFHCHttpCallPerformAsync can only be called once.  Create new PFHCCallHandle to repeat the call.
/// </remarks>
PF_API PFHCHttpCallPerformAsync(
    _In_ PFHCCallHandle call,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

/// <summary>
/// Duplicates the PFHCCallHandle object.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <returns>Returns the duplicated handle.</returns>
/// <remarks>Use PFHCHttpCallCloseHandle to close it.</remarks>
PF_API_(PFHCCallHandle) PFHCHttpCallDuplicateHandle(
    _In_ PFHCCallHandle call
    ) noexcept;

/// <summary>
/// Decrements the reference count on the call object. 
/// </summary>
/// <param name="call">The handle of the HTTP call</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>When the PFHCCallHandle ref count is 0, PFHCHttpCallCloseHandle() will free the memory associated with the PFHCCallHandle.</remarks>
PF_API PFHCHttpCallCloseHandle(
    _In_ PFHCCallHandle call
    ) noexcept;

/// <summary>
/// Returns a unique uint64_t which identifies this HTTP call object.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <returns>Returns a unique uint64_t which identifies this HTTP call object or 0 if invalid.</returns>
PF_API_(uint64_t) PFHCHttpCallGetId(
    _In_ PFHCCallHandle call
    ) noexcept;

/// <summary>
/// Enables or disables tracing for this specific HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="traceCall">Trace this call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
PF_API PFHCHttpCallSetTracing(
    _In_ PFHCCallHandle call,
    _In_ bool traceCall
    ) noexcept;

/// <summary>
/// Gets the request url for the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="url">
/// The UTF-8 encoded url body string of the HTTP call.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
PF_API PFHCHttpCallGetRequestUrl(
    _In_ PFHCCallHandle call,
    _Out_ const char** url
    ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallRequest Set APIs
//

/// <summary>
/// Sets the url and method for the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="method">UTF-8 encoded method for the HTTP call.</param>
/// <param name="url">UTF-8 encoded URL for the HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetUrl(
    _In_ PFHCCallHandle call,
    _In_z_ const char* method,
    _In_z_ const char* url
    ) noexcept;

/// <summary>
/// Set the request body bytes of the HTTP call. This API operation is mutually exclusive with
/// PFHCHttpCallRequestSetRequestBodyReadFunction and will result in any custom read callbacks that were
/// previously set on this call handle to be ignored.
/// </summary> 
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="requestBodyBytes">The request body bytes of the HTTP call.</param>
/// <param name="requestBodySize">The length in bytes of the body being set.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetRequestBodyBytes(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(requestBodySize) const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize
    ) noexcept;

/// <summary>
/// Set the request body string of the HTTP call. This API operation is mutually exclusive with
/// PFHCHttpCallRequestSetRequestBodyReadFunction and will result in any custom read callbacks that were
/// previously set on this call handle to be ignored.
/// </summary> 
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="requestBodyString">The UTF-8 encoded request body string of the HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetRequestBodyString(
    _In_ PFHCCallHandle call,
    _In_z_ const char* requestBodyString
    ) noexcept;

/// <summary>
/// The callback definition used by an HTTP call to read the request body. This callback will be invoked
/// on an unspecified background thread which is platform dependent.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="offset">The offset from the beginning of the request body.</param>
/// <param name="bytesAvailable">The maximum number of bytes that can be written to the destination.</param>
/// <param name="context">The context associated with this read function.</param>
/// <param name="destination">The destination where data may be written to.</param>
/// <param name="bytesWritten">The number of bytes that were actually written to destination.</param>
/// <returns>Result code for this callback. Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
typedef HRESULT
(CALLBACK* PFHCHttpCallRequestBodyReadFunction)(
    _In_ PFHCCallHandle call,
    _In_ size_t offset,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context,
    _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
    _Out_ size_t* bytesWritten
    );

/// <summary>
/// Sets a custom callback function that will be used to read the request body when the HTTP call is
/// performed. If a custom read callback is used, any request body data previously set by
/// PFHCHttpCallRequestSetRequestBodyBytes or PFHCHttpCallRequestSetRequestBodyString is ignored making
/// these API operations mutually exclusive.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="readFunction">The request body read function this call should use.</param>
/// <param name="bodySize">The size of the body.</param>
/// <param name="context">The context associated with this read function.</param>
/// <returns>Result code of this API operation. Possible values are S_OK or E_INVALIDARG.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetRequestBodyReadFunction(
    _In_ PFHCCallHandle call,
    _In_ PFHCHttpCallRequestBodyReadFunction readFunction,
    _In_ size_t bodySize,
    _In_opt_ void* context
    ) noexcept;

/// <summary>
/// Set a request header for the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="headerName">UTF-8 encoded request header name for the HTTP call.</param>
/// <param name="headerValue">UTF-8 encoded request header value for the HTTP call.</param>
/// <param name="allowTracing">Set to false to skip tracing this request header, for example if it contains private information.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue,
    _In_ bool allowTracing
    ) noexcept;

/// <summary>
/// Sets if retry is allowed for this HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.  Pass nullptr to set the default for future calls.</param>
/// <param name="retryAllowed">If retry is allowed for this HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_FAIL.</returns>
/// <remarks>
/// Defaults to true.
/// This must be called prior to calling PFHCHttpCallPerformAsync.
/// </remarks>
PF_API PFHCHttpCallRequestSetRetryAllowed(
    _In_opt_ PFHCCallHandle call,
    _In_ bool retryAllowed
    ) noexcept;

/// <summary>
/// ID number of this REST endpoint used to cache the Retry-After header for fast fail.
/// </summary>
/// <param name="call">The handle of the HTTP call.  Pass nullptr to set the default for future calls.</param>
/// <param name="retryAfterCacheId">ID number of this REST endpoint used to cache the Retry-After header for fast fail. Must be non-zero. 1-1000 are reserved for XSAPI.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestSetRetryCacheId(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t retryAfterCacheId
    ) noexcept;

/// <summary>
/// Sets the timeout for this HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.  Pass nullptr to set the default for future calls.</param>
/// <param name="timeoutInSeconds">The timeout for this HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_FAIL.</returns>
/// <remarks>
/// Defaults to 30 seconds.
/// This must be called prior to calling PFHCHttpCallPerformAsync.
/// </remarks>
PF_API PFHCHttpCallRequestSetTimeout(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t timeoutInSeconds
    ) noexcept;

/// <summary>
/// Sets the HTTP retry delay in seconds. The default and minimum delay is 2 seconds.
/// </summary>
/// <param name="call">The handle of the HTTP call.  Pass nullptr to set the default for future calls.</param>
/// <param name="retryDelayInSeconds">The retry delay in seconds.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_FAIL.</returns>
/// <remarks>
/// Retries are delayed using a exponential back off.  By default, it will delay 2 seconds then the 
/// next retry will delay 4 seconds, then 8 seconds, and so on up to a max of 1 min until either
/// the call succeeds or the HTTP timeout window is reached, at which point the call will fail.
/// The delay is also jittered between the current and next delay to spread out service load.
/// The default for the HTTP timeout window is 20 seconds and can be changed using PFHCSettingsSetTimeoutWindow()<br />
/// <br />
/// If the service returns an HTTP error with a "Retry-After" header and the title had previously called 
/// PFHCHttpCallRequestSetRetryCacheId(), then all future calls to that API will immediately fail with the 
/// original error without contacting the service until the "Retry-After" time has been reached.<br />
///  <br />
/// Idempotent service calls are retried when a network error occurs or the server responds with<br />
/// one of these HTTP status codes:<br />
/// 408 (Request Timeout)<br />
/// 429 (Too Many Requests)<br />
/// 500 (Internal Server Error)<br />
/// 502 (Bad Gateway)<br />
/// 503 (Service Unavailable)<br />
/// 504 (Gateway Timeout)<br />
///  <br />
/// This must be called prior to calling PFHCHttpCallPerformAsync.
/// </remarks>
PF_API PFHCHttpCallRequestSetRetryDelay(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t retryDelayInSeconds
    ) noexcept;

/// <summary>
/// Sets the HTTP timeout window in seconds.
/// </summary>
/// <param name="call">The handle of the HTTP call.  Pass nullptr to set the default for future calls</param>
/// <param name="timeoutWindowInSeconds">The timeout window in seconds</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, or E_FAIL.</returns>
/// <remarks>
/// This controls how long to spend attempting to retry idempotent service calls before failing.<br />
/// The default is 20 seconds.<br />
///<br />
/// Idempotent service calls are retried when a network error occurs or the server responds <br />
/// with one of these HTTP status codes:<br />
/// 408 (Request Timeout)<br />
/// 429 (Too Many Requests)<br />
/// 500 (Internal Server Error)<br />
/// 502 (Bad Gateway)<br />
/// 503 (Service Unavailable)<br />
/// 504 (Gateway Timeout)<br />
///<br />
/// This must be called prior to calling PFHCHttpCallPerformAsync.<br />
/// </remarks>
PF_API PFHCHttpCallRequestSetTimeoutWindow(
    _In_opt_ PFHCCallHandle call,
    _In_ uint32_t timeoutWindowInSeconds
    ) noexcept;

#if HC_PLATFORM_IS_MICROSOFT && (HC_PLATFORM != HC_PLATFORM_UWP) && (HC_PLATFORM != HC_PLATFORM_XDK)
/// <summary>
/// Enables or disables SSL server certificate validation for this specific HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="sslValidation">Boolean indicating whether to enable or disable certificate validation for the HTTP call.</param>
/// <returns>Result code for this API operation. Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// Only implemented for Win32.
/// The default is true
///
/// Optional, call prior to calling PFHCHttpCallPerformAsync.
/// </remarks>
PF_API PFHCHttpCallRequestSetSSLValidation(
    _In_ PFHCCallHandle call,
    _In_ bool sslValidation
    ) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Defines the config settings value that is passed to the below API's.
/// </summary>
enum class PFHCConfigSetting : uint32_t
{
    /// <summary>
    /// Only passed to the below API's to warn callers that this SSL validation 
    /// is enforced RETAIL sandboxes regardless of this setting
    /// </summary>
    SSLValidationEnforcedInRetailSandbox = 1
};

/// <summary>
/// Disables asserts for SSL validation in console dev sandboxes.
/// </summary>
/// <param name="setting">The config settings value to be passed down.</param>
/// <returns>HRESULT return code for this API operation.</returns>
/// <remarks>
/// On GDK console, SSL validation is enforced on RETAIL sandboxes regardless of this setting.
/// The asserts will not fire in RETAIL sandbox, and this setting has no affect in RETAIL sandboxes.
/// It is best practice to not call this API, but this can be used as a temporary way 
/// to get unblocked while in early stages of game development.
/// </remarks>
PF_API PFHCHttpDisableAssertsForSSLValidationInDevSandboxes(
    _In_ PFHCConfigSetting setting
    ) noexcept;
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Set APIs
//

/// <summary>
/// The callback definition used by an HTTP call to write the response body. This callback will be
/// invoked on an unspecified background thread which is platform dependent.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="source">The source from which bytes may be read.</param>
/// <param name="bytesAvailable">The number of bytes that can be read from the source.</param>
/// <param name="context">The context associated with this write function.</param>
/// <returns>Result code for this callback. Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
typedef HRESULT
(CALLBACK* PFHCHttpCallResponseBodyWriteFunction)(
    _In_ PFHCCallHandle call,
    _In_reads_bytes_(bytesAvailable) const uint8_t* source,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context
    );

/// <summary>
/// Sets a custom callback function that will be used to write the response body when the HTTP call
/// is performed. Using a custom write callback will cause subsequent calls to
/// PFHCHttpCallResponseGetResponseBodyBytesSize, PFHCHttpCallResponseGetResponseBodyBytes,
/// and PFHCHttpCallGetResponseBodyString to fail as these are mutually exclusive.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="writeFunction">The response body write function this call should use.</param>
/// <param name="context">The context to associate with this write function.</param>
/// <returns>Result code of this API operation. Possible values are S_OK or E_INVALIDARG.</returns>
/// <remarks>This must be called prior to calling PFHCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallResponseSetResponseBodyWriteFunction(
    _In_ PFHCCallHandle call,
    _In_ PFHCHttpCallResponseBodyWriteFunction writeFunction,
    _In_opt_ void* context
) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////
// HttpCallResponse Get APIs
//

/// <summary>
/// Get the response body string of the HTTP call. This API operation will fail if a custom write
/// callback was set on this call handle using PFHCHttpCallResponseSetResponseBodyWriteFunction.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="responseString">
/// The UTF-8 encoded response body string of the HTTP call.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetResponseString(
    _In_ PFHCCallHandle call,
    _Out_ const char** responseString
    ) noexcept;

/// <summary>
/// Get the response body buffer size of the HTTP call. This API operation will fail if a custom write
/// callback was set on this call handle using PFHCHttpCallResponseSetResponseBodyWriteFunction.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="bufferSize">The response body buffer size of the HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetResponseBodyBytesSize(
    _In_ PFHCCallHandle call,
    _Out_ size_t* bufferSize
    ) noexcept;

/// <summary>
/// Get the response body buffer of the HTTP call. This API operation will fail if a custom write
/// callback was set on this call handle using PFHCHttpCallResponseSetResponseBodyWriteFunction.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="bufferSize">The response body buffer size being passed in.</param>
/// <param name="buffer">The buffer to be written to.</param>
/// <param name="bufferUsed">The actual number of bytes written to the buffer.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetResponseBodyBytes(
    _In_ PFHCCallHandle call,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) uint8_t* buffer,
    _Out_opt_ size_t* bufferUsed
    ) noexcept;

/// <summary>
/// Get the HTTP status code of the HTTP call response.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="statusCode">the HTTP status code of the HTTP call response.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetStatusCode(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* statusCode
    ) noexcept;

/// <summary>
/// Get the network error code of the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="networkErrorCode">The network error code of the HTTP call. Possible values are S_OK, or E_FAIL.</param>
/// <param name="platformNetworkErrorCode">The platform specific network error code of the HTTP call to be used for tracing / debugging.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed. On the GDK Platform, HTTP calls that fail
/// due to the title being suspended will have platformNetworkErrorCode set to HRESULT_FROM_WIN32(PROCESS_SUSPEND_RESUME).
/// </remarks>
PF_API PFHCHttpCallResponseGetNetworkErrorCode(
    _In_ PFHCCallHandle call,
    _Out_ HRESULT* networkErrorCode,
    _Out_ uint32_t* platformNetworkErrorCode
    ) noexcept;

/// <summary>
/// Get the platform network error message of the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="platformNetworkErrorMessage">The platform specific network error message of the HTTP call to be used for tracing / debugging.</param>
/// <returns>Result code for this API operation. Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetPlatformNetworkErrorMessage(
    _In_ PFHCCallHandle call,
    _Out_ const char** platformNetworkErrorMessage
    ) noexcept;

/// <summary>
/// Get a response header for the HTTP call for a given header name.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="headerName">UTF-8 encoded response header name for the HTTP call.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <param name="headerValue">UTF-8 encoded response header value for the HTTP call.
/// Returns nullptr if the header doesn't exist.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetHeader(
    _In_ PFHCCallHandle call,
    _In_z_ const char* headerName,
    _Out_ const char** headerValue
    ) noexcept;

/// <summary>
/// Gets the number of response headers in the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="numHeaders">The number of response headers in the HTTP call.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.</remarks>
PF_API PFHCHttpCallResponseGetNumHeaders(
    _In_ PFHCCallHandle call,
    _Out_ uint32_t* numHeaders
    ) noexcept;

/// <summary>
/// Gets the response headers at specific zero based index in the HTTP call.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="headerIndex">Specific zero based index of the response header.</param>
/// <param name="headerName">UTF-8 encoded response header name for the HTTP call.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <param name="headerValue">UTF-8 encoded response header value for the HTTP call.
/// The memory for the returned string pointer remains valid for the life of the PFHCCallHandle object until PFHCHttpCallCloseHandle() is called on it.
/// </param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// Use PFHCHttpCallResponseGetNumHeaders() to know how many response headers there are in the HTTP call.
/// This can only be called after calling PFHCHttpCallPerformAsync when the HTTP task is completed.
/// </remarks>
PF_API PFHCHttpCallResponseGetHeaderAtIndex(
    _In_ PFHCCallHandle call,
    _In_ uint32_t headerIndex,
    _Out_ const char** headerName,
    _Out_ const char** headerValue
    ) noexcept;

/// <summary>
/// Defines the compression level that will be used on the compression algorithm.
/// Lower levels perform less compression but have the highest speed in the compression and
/// higher levels perform better compression but have the slowest speed in the compression.
/// </summary>
enum class PFHCCompressionLevel : uint32_t
{
    /// <summary>
    /// A value of "None" indicates that no compression will be made.
    /// </summary>
    None = 0,

    /// <summary>
    /// A value of "Low" indicates that compression level 1 will be made.
    /// </summary>
    Low = 1,

    /// <summary>
    /// A value of "Medium" indicates that compression level 6 will be made.
    /// </summary>
    Medium = 6,

    /// <summary>
    /// A value of "High" indicates that compression level 9 will be made.
    /// </summary>
    High = 9
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
/// <summary>
/// Enable GZIP compression on the provided body payload.
/// </summary>
/// <param name="call">The handle of the HTTP call.</param>
/// <param name="level">The desired compression level.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling HCHttpCallPerformAsync.</remarks>
PF_API PFHCHttpCallRequestEnableGzipCompression(
    _In_ PFHCCallHandle call,
    _In_ PFHCCompressionLevel level
) noexcept;
#endif

#if !HC_NOWEBSOCKETS
/////////////////////////////////////////////////////////////////////////////////////////
// WebSocket APIs
// 

/// <summary>
/// A callback invoked every time a WebSocket receives an incoming message
/// </summary>
/// <param name="websocket">Handle to the WebSocket that this message was sent to</param>
/// <param name="incomingBodyString">UTF-8 encoded body of the incoming message as a string value, only if the message type is UTF-8.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
typedef void
(CALLBACK* PFHCWebSocketMessageFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* incomingBodyString,
    _In_ void* functionContext
    );

/// <summary>
/// A callback invoked every time a WebSocket receives an incoming binary message
/// </summary>
/// <param name="websocket">Handle to the WebSocket that this message was sent to</param>
/// <param name="incomingBodyPayload">Binary message payload.</param>
/// <param name="incomingBodyPayloadSize">Size of the payload in bytes.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
typedef void
(CALLBACK* PFHCWebSocketBinaryMessageFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes,
    _In_ uint32_t payloadSize,
    _In_ void* functionContext
    );

/// <summary>
/// A callback invoked every time a WebSocket receives an incoming message that is larger than
/// the WebSocket receive buffer (configurable using PFHCWebSocketSetMaxReceiveBufferSize). Large messages
/// will be broken down and passed to clients in chunks.
/// </summary>
/// <param name="websocket">Handle to the WebSocket that this message was sent to</param>
/// <param name="incomingBodyPayload">Binary message payload.</param>
/// <param name="incomingBodyPayloadSize">Size of the payload in bytes.</param>
/// <param name="isLastFragment">True if this is the last fragment in a message, false otherwise.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
typedef void
(CALLBACK* PFHCWebSocketBinaryMessageFragmentFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes,
    _In_ uint32_t payloadSize,
    _In_ bool isLastFragment,
    _In_ void* functionContext
    );

/// <summary>
/// A callback invoked when a WebSocket is closed
/// </summary>
/// <param name="websocket">Handle to the WebSocket</param>
/// <param name="closeStatus">The status of why the WebSocket was closed</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
typedef void
(CALLBACK* PFHCWebSocketCloseEventFunction)(
    _In_ PFHCWebsocketHandle websocket,
    _In_ PFHCWebSocketCloseStatus closeStatus,
    _In_ void* functionContext
    );

/// <summary>
/// Creates an WebSocket handle.
/// </summary>
/// <param name="websocket">The handle of the websocket.</param>
/// <param name="messageFunc">A pointer to the message handling callback to use, or a null pointer to remove.</param>
/// <param name="binaryMessageFunc">A pointer to the binary message handling callback to use, or a null pointer to remove.</param>
/// <param name="closeFunc">A pointer to the close callback to use, or a null pointer to remove.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// WebSocket usage:<br />
/// Create a WebSocket handle using PFHCWebSocketCreate()<br />
/// Call PFHCWebSocketSetProxyUri() and PFHCWebSocketSetHeader() to prepare the PFHCWebsocketHandle<br />
/// Call PFHCWebSocketConnectAsync() to connect the WebSocket using the PFHCWebsocketHandle.<br />
/// Call PFHCWebSocketSendMessageAsync() to send a message to the WebSocket using the PFHCWebsocketHandle.<br />
/// Call PFHCWebSocketDisconnect() to disconnect the WebSocket using the PFHCWebsocketHandle.<br />
/// Call PFHCWebSocketCloseHandle() when done with the PFHCWebsocketHandle to free the associated memory<br />
/// </remarks>
PF_API PFHCWebSocketCreate(
    _Out_ PFHCWebsocketHandle* websocket,
    _In_opt_ PFHCWebSocketMessageFunction messageFunc,
    _In_opt_ PFHCWebSocketBinaryMessageFunction binaryMessageFunc,
    _In_opt_ PFHCWebSocketCloseEventFunction closeFunc,
    _In_opt_ void* functionContext
    ) noexcept;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

/// <summary>
/// Set the binary message fragment handler. The client functionContext passed to PFHCWebSocketCreate will also be passed to this handler.
/// </summary>
/// <param name="websocket">The handle of the websocket.</param>
/// <param name="binaryMessageFragmentFunc">A pointer to the binary message fragment handling callback to use, or a null pointer to remove.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// If this handler is not set, messages larger than the configured buffer size may still be broken down and passed to the
/// PFHCWebSocketBinaryMessageFunction, but there will be no indication that they are partial messages. If large WebSocket messages are expected,
/// it is recommended to either set this handler OR set a receive buffer large enough to hold the entire message.
/// </remarks>
PF_API PFHCWebSocketSetBinaryMessageFragmentEventFunction(
    _In_ PFHCWebsocketHandle websocket,
    _In_ PFHCWebSocketBinaryMessageFragmentFunction binaryMessageFragmentFunc
    ) noexcept;
#endif

/// <summary>
/// Set the proxy URI for the WebSocket.
/// </summary>
/// <param name="websocket">The handle of the WebSocket.</param>
/// <param name="proxyUri">The UTF-8 encoded proxy URI for the WebSocket.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCWebSocketConnectAsync.</remarks>
PF_API PFHCWebSocketSetProxyUri(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* proxyUri
    ) noexcept;

#if HC_PLATFORM == HC_PLATFORM_WIN32 && !HC_WINHTTP_WEBSOCKETS

/// <summary>
/// Allows proxy server to decrypt and inspect traffic; should be used only for debugging purposes
/// This must be called after calling PFHCWebSocketSetProxyUri.
/// Only applies to Win32 non-GDK builds
/// </summary>
/// <param name="websocket">The handle of the WebSocket</param>
/// <param name="allowProxyToDecryptHttps">true is proxy can decrypt, false is not allowed to decrypt</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
PF_API PFHCWebSocketSetProxyDecryptsHttps(
    _In_ PFHCWebsocketHandle websocket,
    _In_ bool allowProxyToDecryptHttps
    ) noexcept;
#endif

/// <summary>
/// Set a header for the WebSocket.
/// </summary>
/// <param name="websocket">The handle of the WebSocket.</param>
/// <param name="headerName">UTF-8 encoded header name for the WebSocket.</param>
/// <param name="headerValue">UTF-8 encoded header value for the WebSocket.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>This must be called prior to calling PFHCWebSocketConnectAsync.</remarks>
PF_API PFHCWebSocketSetHeader(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
    ) noexcept;

/// <summary>
/// Gets the WebSocket functions to allow callers to respond to incoming messages and WebSocket close events.
/// </summary>
/// <param name="websocket">The handle of the websocket.</param>
/// <param name="messageFunc">A pointer to the message handling callback to use, or a null pointer to remove.</param>
/// <param name="binaryMessageFunc">A pointer to the binary message handling callback to use, or a null pointer to remove.</param>
/// <param name="closeFunc">A pointer to the close callback to use, or a null pointer to remove.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_HC_NOT_INITIALISED, or E_FAIL.</returns>
PF_API PFHCWebSocketGetEventFunctions(
    _In_ PFHCWebsocketHandle websocket,
    _Out_opt_ PFHCWebSocketMessageFunction* messageFunc,
    _Out_opt_ PFHCWebSocketBinaryMessageFunction* binaryMessageFunc,
    _Out_opt_ PFHCWebSocketCloseEventFunction* closeFunc,
    _Out_ void** functionContext
    ) noexcept;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK

/// <summary>
/// Gets the WebSocket binary message fragment handler.
/// </summary>
/// <param name="websocket">The handle of the websocket.</param>
/// <param name="binaryMessageFragmentFunc">Returned binaryMessageFragmentFunc.</param>
/// <param name="functionContext">Client context to pass to callback function.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_HC_NOT_INITIALISED, or E_FAIL.</returns>
PF_API PFHCWebSocketGetBinaryMessageFragmentEventFunction(
    _In_ PFHCWebsocketHandle websocket,
    _Out_ PFHCWebSocketBinaryMessageFragmentFunction* binaryMessageFragmentFunc,
    _Out_ void** functionContext
    ) noexcept;
#endif

/// <summary>
/// Used by PFHCWebSocketConnectAsync() and PFHCWebSocketSendMessageAsync().
/// </summary>
typedef struct PFWebSocketCompletionResult
{
    /// <summary>The handle of the HTTP call.</summary>
    PFHCWebsocketHandle websocket;

    /// <summary>The error code of the call. Possible values are S_OK, or E_FAIL.</summary>
    HRESULT errorCode;

    /// <summary>The platform specific network error code of the call to be used for tracing / debugging.</summary>
    uint32_t platformErrorCode;
} PFWebSocketCompletionResult;

/// <summary>
/// Connects to the WebSocket.
/// </summary>
/// <param name="uri">The UTF-8 encoded URI to connect to.</param>
/// <param name="subProtocol">The UTF-8 encoded subProtocol to connect to.</param>
/// <param name="websocket">The handle of the WebSocket.</param>
/// <param name="asyncBlock">The XAsyncBlock that defines the async operation.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
/// <remarks>
/// To get the result, first call PFHCGetWebSocketConnectResult inside the AsyncBlock callback or after the AsyncBlock is complete.
/// On GDK and Win32 (Win 8+) the background work is scheduled to threads owned by WinHttp run in async mode.
/// On UWP and XDK, the connection thread is owned and controlled by Windows::Networking::Sockets::MessageWebSocket.
/// On Win32 (Win 7+), iOS, and Android, all background work (including initial connection process) will be added to the queue
/// in the provided XAsyncBlock. LibHttpClient will create a reference to that queue but it is the responsibility of the
/// caller to dispatch that queue for as long as the websocket connection is active. Note that work for 
/// PFHCWebSocketSendMessageAsync calls can be assigned to a separate queue if desired.
/// </remarks>
PF_API PFHCWebSocketConnectAsync(
    _In_z_ const char* uri,
    _In_z_ const char* subProtocol,
    _In_ PFHCWebsocketHandle websocket,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

/// <summary>
/// Gets the result for PFHCGetWebSocketConnectResult.
/// </summary>
/// <param name="asyncBlock">The XAsyncBlock that defines the async operation.</param>
/// <param name="result">Pointer to the result payload.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, E_OUTOFMEMORY, or E_FAIL.</returns>
PF_API PFHCGetWebSocketConnectResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ PFWebSocketCompletionResult* result
    ) noexcept;

/// <summary>
/// Send message the WebSocket
/// </summary>
/// <param name="websocket">Handle to the WebSocket.</param>
/// <param name="message">The UTF-8 encoded message to send.</param>
/// <param name="asyncBlock">The XAsyncBlock that defines the async operation.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// To get the result, first call PFHCGetWebSocketSendMessageResult
/// inside the AsyncBlock callback or after the AsyncBlock is complete.
/// </remarks>
PF_API PFHCWebSocketSendMessageAsync(
    _In_ PFHCWebsocketHandle websocket,
    _In_z_ const char* message,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

/// <summary>
/// Send binary message to the WebSocket.
/// </summary>
/// <param name="websocket">Handle to the WebSocket.</param>
/// <param name="payloadBytes">Binary data to send in byte buffer.</param>
/// <param name="payloadSize">Size of byte buffer.</param>
/// <param name="asyncBlock">The AsyncBlock that defines the async operation.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>
/// To get the result, first call PFHCGetWebSocketSendMessageResult
/// inside the AsyncBlock callback or after the AsyncBlock is complete.
/// </remarks>
PF_API PFHCWebSocketSendBinaryMessageAsync(
    _In_ PFHCWebsocketHandle websocket,
    _In_reads_bytes_(payloadSize) const uint8_t* payloadBytes,
    _In_ uint32_t payloadSize,
    _Inout_ XAsyncBlock* asyncBlock
    ) noexcept;

/// <summary>
/// Gets the result from PFHCWebSocketSendMessage.
/// </summary>
/// <param name="asyncBlock">The XAsyncBlock that defines the async operation.</param>
/// <param name="result">Pointer to the result payload.</param>
/// <returns>Returns the duplicated handle.</returns>
PF_API PFHCGetWebSocketSendMessageResult(
    _Inout_ XAsyncBlock* asyncBlock,
    _In_ PFWebSocketCompletionResult* result
    ) noexcept;

/// <summary>
/// Disconnects / closes the WebSocket.
/// </summary>
/// <param name="websocket">Handle to the WebSocket.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
PF_API PFHCWebSocketDisconnect(
    _In_ PFHCWebsocketHandle websocket
    ) noexcept;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Configures how large the WebSocket receive buffer is allowed to grow before passing messages to clients. If a single message
/// exceeds the maximum buffer size, the message will be broken down and passed to clients via multiple calls to the PFHCWebSocketMessageFunction.
/// The default value is 20kb.
/// </summary>
/// <param name="websocket">The handle of the WebSocket</param>
/// <param name="bufferSizeInBytes">Maximum size (in bytes) for the WebSocket receive buffer.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
PF_API PFHCWebSocketSetMaxReceiveBufferSize(
    _In_ PFHCWebsocketHandle websocket,
    _In_ size_t bufferSizeInBytes
    ) noexcept;
#endif

/// <summary>
/// Increments the reference count on the call object.
/// </summary>
/// <param name="websocket">Handle to the WebSocket.</param>
/// <returns>Returns the duplicated handle.</returns>
PF_API_(PFHCWebsocketHandle) PFHCWebSocketDuplicateHandle(
    _In_ PFHCWebsocketHandle websocket
    ) noexcept;

/// <summary>
/// Decrements the reference count on the WebSocket object. 
/// </summary>
/// <param name="websocket">Handle to the WebSocket.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_INVALIDARG, or E_FAIL.</returns>
/// <remarks>When the ref count is 0, PFHCWebSocketCloseHandle() will free the memory associated with the PFHCWebsocketHandle.</remarks>
PF_API PFHCWebSocketCloseHandle(
    _In_ PFHCWebsocketHandle websocket
    ) noexcept;

/// <summary>
/// A callback that will be synchronously invoked when websocket traffic is sent or received
/// </summary>
/// <param name="call">Handle to the HTTP call.</param>
/// <param name="receiving">True if receiving the data, false if sending the data.</param>
/// <param name="message">Optional UTF-8 encoded message.
/// Either message or payloadBytes should be non-null depending if its text or binary message</param>
/// <param name="payloadBytes">Binary data in byte buffer.
/// Either message or payloadBytes should be non-null depending if its text or binary message</param>
/// <param name="payloadSize">Size of byte buffer</param>
/// <param name="context">Client context pass when the handler was added.</param>
typedef void
(STDAPIVCALLTYPE* PFHCWebSocketRoutedHandler)(
    _In_ PFHCWebsocketHandle websocket,
    _In_ bool receiving,
    _In_opt_z_ const char* message,
    _In_opt_ const uint8_t* payloadBytes,
    _In_ size_t payloadSize,
    _In_opt_ void* context
    );

/// <summary>
/// Adds a callback to be invoked on websocket traffic in order to debug or trace the traffic.
/// </summary>
/// <param name="handler">The handler to be called.</param>
/// <param name="context">Client context to pass to callback function.</param>
/// <returns>An unique id that can be used to remove the handler.</returns>
PF_API_(int32_t) PFHCAddWebSocketRoutedHandler(
    _In_ PFHCWebSocketRoutedHandler handler,
    _In_opt_ void* context
    ) noexcept;

/// <summary>
/// Removes a previously added PFHCWebSocketRoutedHandler.
/// </summary>
/// <param name="handlerId">Id returned from the PFHCAddWebSocketRoutedHandler call.</param>
/// <returns></returns>
PF_API_(void) PFHCRemoveWebSocketRoutedHandler(
    _In_ int32_t handlerId
    ) noexcept;

#endif // !HC_NOWEBSOCKETS

}


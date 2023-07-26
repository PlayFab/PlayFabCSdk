#if !defined(__cplusplus)
    #error C++11 required
#endif

#pragma once
#include <playfab/core/PFPal.h>

extern "C"
{

/////////////////////////////////////////////////////////////////////////////////////////
// Mock APIs
// 

PF_API PFHCMockCallCreate(
    _Out_ PFHCMockCallHandle* call
    ) noexcept;

PF_API PFHCMockAddMock(
    _In_ PFHCMockCallHandle call,
    _In_opt_z_ const char* method,
    _In_opt_z_ const char* url,
    _In_reads_bytes_opt_(requestBodySize) const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize
    ) noexcept;

typedef void (CALLBACK* PFHCMockMatchedCallback)(
    _In_ PFHCMockCallHandle matchedMock,
    _In_ const char* method,
    _In_ const char* url,
    _In_ const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize,
    _In_ void* context
    );

PF_API PFHCMockSetMockMatchedCallback(
    _In_ PFHCMockCallHandle call,
    _In_ PFHCMockMatchedCallback callback,
    _In_opt_ void* context
    );

PF_API PFHCMockRemoveMock(
    _In_ PFHCMockCallHandle call
    );

PF_API PFHCMockClearMocks() noexcept;


/////////////////////////////////////////////////////////////////////////////////////////
// HCMockResponse Set APIs
// 

PF_API PFHCMockResponseSetResponseBodyBytes(
    _In_ PFHCMockCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ uint32_t bodySize
    ) noexcept;

PF_API PFHCMockResponseSetStatusCode(
    _In_ PFHCMockCallHandle call,
    _In_ uint32_t statusCode
    ) noexcept;

PF_API PFHCMockResponseSetNetworkErrorCode(
    _In_ PFHCMockCallHandle call,
    _In_ HRESULT networkErrorCode,
    _In_ uint32_t platformNetworkErrorCode
    ) noexcept;

PF_API PFHCMockResponseSetHeader(
    _In_ PFHCMockCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
    ) noexcept;

}

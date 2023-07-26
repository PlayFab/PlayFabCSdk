#include "stdafx.h"
#include <playfab/httpClient/PFMock.h>
#include <httpClient/mock.h>

/////////////////////////////////////////////////////////////////////////////////////////
// Mock APIs
// 

PF_API PFHCMockCallCreate(
    _Out_ PFHCMockCallHandle* call
) noexcept
{
    return HCMockCallCreate(call);
}

PF_API PFHCMockAddMock(
    _In_ PFHCMockCallHandle call,
    _In_opt_z_ const char* method,
    _In_opt_z_ const char* url,
    _In_reads_bytes_opt_(requestBodySize) const uint8_t* requestBodyBytes,
    _In_ uint32_t requestBodySize
) noexcept
{
    return HCMockAddMock(
        call, 
        method, 
        url, 
        requestBodyBytes, 
        requestBodySize
    );
}

PF_API PFHCMockSetMockMatchedCallback(
    _In_ PFHCMockCallHandle call,
    _In_ PFHCMockMatchedCallback callback,
    _In_opt_ void* context
)
{
    return HCMockSetMockMatchedCallback(
        call, 
        callback, 
        context
    );
}

PF_API PFHCMockRemoveMock(
    _In_ PFHCMockCallHandle call
)
{
    return HCMockRemoveMock(call);
}

PF_API PFHCMockClearMocks() noexcept
{
    return HCMockClearMocks();
}

/////////////////////////////////////////////////////////////////////////////////////////
// HCMockResponse Set APIs
// 

PF_API PFHCMockResponseSetResponseBodyBytes(
    _In_ PFHCMockCallHandle call,
    _In_reads_bytes_(bodySize) const uint8_t* bodyBytes,
    _In_ uint32_t bodySize
) noexcept
{
    return HCMockResponseSetResponseBodyBytes(
        call, 
        bodyBytes, 
        bodySize
    );
}

PF_API PFHCMockResponseSetStatusCode(
    _In_ PFHCMockCallHandle call,
    _In_ uint32_t statusCode
) noexcept
{
    return HCMockResponseSetStatusCode(
        call, 
        statusCode
    );
}

PF_API PFHCMockResponseSetNetworkErrorCode(
    _In_ PFHCMockCallHandle call,
    _In_ HRESULT networkErrorCode,
    _In_ uint32_t platformNetworkErrorCode
) noexcept
{
    return HCMockResponseSetNetworkErrorCode(
        call, 
        networkErrorCode, 
        platformNetworkErrorCode
    );
}

PF_API PFHCMockResponseSetHeader(
    _In_ PFHCMockCallHandle call,
    _In_z_ const char* headerName,
    _In_z_ const char* headerValue
) noexcept
{
    return HCMockResponseSetHeader(
        call, 
        headerName, 
        headerValue
    );
}

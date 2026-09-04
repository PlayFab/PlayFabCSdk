#include "stdafx.h"
#include "ApiHelpers.h"

namespace PlayFab
{
namespace Detail
{

HRESULT CALLBACK AsyncErrorProvider(XAsyncOp op, XAsyncProviderData const* data)
{
    assert(data);
    RETURN_HR_IF(E_UNEXPECTED, !data);

    if (op == XAsyncOp::Begin)
    {
        UniquePtr<HRESULT> resultPtr{ static_cast<HRESULT*>(data->context) };
        XAsyncComplete(data->async, *resultPtr, 0);
    }
    return S_OK;
}

HRESULT CompleteAsyncWithError(XAsyncBlock* async, const char* apiIdentity, HRESULT hr)
{
    auto resultPtr = MakeUnique<HRESULT>(hr);
    RETURN_IF_FAILED(XAsyncBegin(async, resultPtr.get(), nullptr, apiIdentity, AsyncErrorProvider));
    resultPtr.release();
    return S_OK;
}

}
}

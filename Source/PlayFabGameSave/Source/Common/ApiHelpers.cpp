#include "stdafx.h"
#include "ApiHelpers.h"

namespace PlayFab
{
namespace GameSave
{

namespace Detail
{

HRESULT CALLBACK AsyncResultProvider(XAsyncOp op, XAsyncProviderData const* data)
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

HRESULT CompleteAsyncWithResult(XAsyncBlock* async, const char* apiIdentity, HRESULT hr)
{
    auto resultPtr = MakeUnique<HRESULT>(hr);
    RETURN_IF_FAILED(XAsyncBegin(async, resultPtr.get(), nullptr, apiIdentity, AsyncResultProvider));
    resultPtr.release();
    return S_OK;
}

} // namespace Detail

} // namespace GameSave
} // namespace PlayFab
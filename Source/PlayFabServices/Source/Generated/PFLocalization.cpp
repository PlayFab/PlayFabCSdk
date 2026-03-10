#include "stdafx.h"
#include <playfab/services/PFLocalization.h>
#include "Generated/Localization.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Localization;

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFLocalizationGetLanguageListAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFLocalizationGetLanguageListRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFLocalizationGetLanguageListAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFLocalizationGetLanguageListAsync),
            std::bind(&LocalizationAPI::GetLanguageList, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFLocalizationGetLanguageListGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFLocalizationGetLanguageListGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFLocalizationGetLanguageListGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFLocalizationGetLanguageListResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFLocalizationGetLanguageListGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFLocalizationGetLanguageListResponse*>(buffer);

        return S_OK;
    });
}
#endif

}
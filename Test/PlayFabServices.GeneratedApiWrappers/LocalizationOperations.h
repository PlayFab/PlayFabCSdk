#pragma once

#if HC_PLATFORM != HC_PLATFORM_GDK
#include <playfab/services/cpp/LocalizationTypeWrappers.h>
#endif
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class GetLanguageListOperation : public XAsyncOperation<Wrappers::PFLocalizationGetLanguageListResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLocalizationGetLanguageListRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLocalizationGetLanguageListResponseWrapper<Allocator>;

    GetLanguageListOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLocalizationGetLanguageListResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

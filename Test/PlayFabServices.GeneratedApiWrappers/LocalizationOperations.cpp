#include "TestAppPch.h"
#include "LocalizationOperations.h"
#if HC_PLATFORM != HC_PLATFORM_GDK
#include <playfab/services/PFLocalization.h>
#endif

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

GetLanguageListOperation::GetLanguageListOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetLanguageListOperation::ResultType> GetLanguageListOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetLanguageListOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetLanguageListOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLocalizationGetLanguageListAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetLanguageListOperation::ResultType> GetLanguageListOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLocalizationGetLanguageListGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLocalizationGetLanguageListResponse* result;
    RETURN_IF_FAILED(PFLocalizationGetLanguageListGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

}
}

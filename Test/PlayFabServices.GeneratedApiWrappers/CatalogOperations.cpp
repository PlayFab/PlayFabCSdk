#include "TestAppPch.h"
#include "CatalogOperations.h"
#include <playfab/services/PFCatalog.h>

namespace PlayFab
{
namespace Test
{


CreateDraftItemOperation::CreateDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<CreateDraftItemOperation::ResultType> CreateDraftItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateDraftItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateDraftItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogCreateDraftItemAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<CreateDraftItemOperation::ResultType> CreateDraftItemOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogCreateDraftItemGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogCreateDraftItemResponse* result;
    RETURN_IF_FAILED(PFCatalogCreateDraftItemGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


CreateUploadUrlsOperation::CreateUploadUrlsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<CreateUploadUrlsOperation::ResultType> CreateUploadUrlsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateUploadUrlsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateUploadUrlsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogCreateUploadUrlsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<CreateUploadUrlsOperation::ResultType> CreateUploadUrlsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogCreateUploadUrlsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogCreateUploadUrlsResponse* result;
    RETURN_IF_FAILED(PFCatalogCreateUploadUrlsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

DeleteEntityItemReviewsOperation::DeleteEntityItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteEntityItemReviewsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteEntityItemReviewsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteEntityItemReviewsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogDeleteEntityItemReviewsAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


DeleteItemOperation::DeleteItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogDeleteItemAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if 0

GetCatalogConfigOperation::GetCatalogConfigOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetCatalogConfigOperation::ResultType> GetCatalogConfigOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetCatalogConfigOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetCatalogConfigOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetCatalogConfigAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetCatalogConfigOperation::ResultType> GetCatalogConfigOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetCatalogConfigGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetCatalogConfigResponse* result;
    RETURN_IF_FAILED(PFCatalogGetCatalogConfigGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


GetDraftItemOperation::GetDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetDraftItemOperation::ResultType> GetDraftItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetDraftItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetDraftItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetDraftItemAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetDraftItemOperation::ResultType> GetDraftItemOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetDraftItemGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetDraftItemResponse* result;
    RETURN_IF_FAILED(PFCatalogGetDraftItemGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetDraftItemsOperation::GetDraftItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetDraftItemsOperation::ResultType> GetDraftItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetDraftItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetDraftItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetDraftItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetDraftItemsOperation::ResultType> GetDraftItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetDraftItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetDraftItemsResponse* result;
    RETURN_IF_FAILED(PFCatalogGetDraftItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetEntityDraftItemsOperation::GetEntityDraftItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetEntityDraftItemsOperation::ResultType> GetEntityDraftItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetEntityDraftItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetEntityDraftItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetEntityDraftItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetEntityDraftItemsOperation::ResultType> GetEntityDraftItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetEntityDraftItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetEntityDraftItemsResponse* result;
    RETURN_IF_FAILED(PFCatalogGetEntityDraftItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetEntityItemReviewOperation::GetEntityItemReviewOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetEntityItemReviewOperation::ResultType> GetEntityItemReviewOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetEntityItemReviewOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetEntityItemReviewOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetEntityItemReviewAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetEntityItemReviewOperation::ResultType> GetEntityItemReviewOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetEntityItemReviewGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetEntityItemReviewResponse* result;
    RETURN_IF_FAILED(PFCatalogGetEntityItemReviewGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetItemOperation::GetItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemOperation::ResultType> GetItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemOperation::ResultType> GetItemOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetItemContainersOperation::GetItemContainersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemContainersOperation::ResultType> GetItemContainersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemContainersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemContainersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemContainersAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemContainersOperation::ResultType> GetItemContainersOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemContainersGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemContainersResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemContainersGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

GetItemModerationStateOperation::GetItemModerationStateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemModerationStateOperation::ResultType> GetItemModerationStateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemModerationStateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemModerationStateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemModerationStateAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemModerationStateOperation::ResultType> GetItemModerationStateOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemModerationStateGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemModerationStateResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemModerationStateGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


GetItemPublishStatusOperation::GetItemPublishStatusOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemPublishStatusOperation::ResultType> GetItemPublishStatusOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemPublishStatusOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemPublishStatusOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemPublishStatusAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemPublishStatusOperation::ResultType> GetItemPublishStatusOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemPublishStatusGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemPublishStatusResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemPublishStatusGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetItemReviewsOperation::GetItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemReviewsOperation::ResultType> GetItemReviewsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemReviewsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemReviewsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemReviewsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemReviewsOperation::ResultType> GetItemReviewsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemReviewsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemReviewsResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemReviewsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetItemReviewSummaryOperation::GetItemReviewSummaryOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemReviewSummaryOperation::ResultType> GetItemReviewSummaryOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemReviewSummaryOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemReviewSummaryOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemReviewSummaryAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemReviewSummaryOperation::ResultType> GetItemReviewSummaryOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemReviewSummaryGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemReviewSummaryResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemReviewSummaryGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetItemsOperation::GetItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetItemsOperation::ResultType> GetItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogGetItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetItemsOperation::ResultType> GetItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogGetItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogGetItemsResponse* result;
    RETURN_IF_FAILED(PFCatalogGetItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


PublishDraftItemOperation::PublishDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> PublishDraftItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<PublishDraftItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT PublishDraftItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogPublishDraftItemAsync(m_entity.Handle(), &m_request.Model(), async);
}



ReportItemOperation::ReportItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ReportItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ReportItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ReportItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogReportItemAsync(m_entity.Handle(), &m_request.Model(), async);
}



ReportItemReviewOperation::ReportItemReviewOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ReportItemReviewOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ReportItemReviewOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ReportItemReviewOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogReportItemReviewAsync(m_entity.Handle(), &m_request.Model(), async);
}



ReviewItemOperation::ReviewItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ReviewItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ReviewItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ReviewItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogReviewItemAsync(m_entity.Handle(), &m_request.Model(), async);
}



SearchItemsOperation::SearchItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<SearchItemsOperation::ResultType> SearchItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SearchItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SearchItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogSearchItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<SearchItemsOperation::ResultType> SearchItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogSearchItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogSearchItemsResponse* result;
    RETURN_IF_FAILED(PFCatalogSearchItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

SetItemModerationStateOperation::SetItemModerationStateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> SetItemModerationStateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SetItemModerationStateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SetItemModerationStateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogSetItemModerationStateAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


SubmitItemReviewVoteOperation::SubmitItemReviewVoteOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> SubmitItemReviewVoteOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SubmitItemReviewVoteOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SubmitItemReviewVoteOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogSubmitItemReviewVoteAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if 0

TakedownItemReviewsOperation::TakedownItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> TakedownItemReviewsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<TakedownItemReviewsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT TakedownItemReviewsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogTakedownItemReviewsAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

UpdateCatalogConfigOperation::UpdateCatalogConfigOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> UpdateCatalogConfigOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateCatalogConfigOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateCatalogConfigOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogUpdateCatalogConfigAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


UpdateDraftItemOperation::UpdateDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<UpdateDraftItemOperation::ResultType> UpdateDraftItemOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateDraftItemOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateDraftItemOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCatalogUpdateDraftItemAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<UpdateDraftItemOperation::ResultType> UpdateDraftItemOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCatalogUpdateDraftItemGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCatalogUpdateDraftItemResponse* result;
    RETURN_IF_FAILED(PFCatalogUpdateDraftItemGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

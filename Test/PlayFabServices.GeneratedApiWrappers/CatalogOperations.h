#pragma once

#include <playfab/services/cpp/CatalogTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class CreateDraftItemOperation : public XAsyncOperation<Wrappers::PFCatalogCreateDraftItemResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogCreateDraftItemRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogCreateDraftItemResponseWrapper<Allocator>;

    CreateDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogCreateDraftItemResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class CreateUploadUrlsOperation : public XAsyncOperation<Wrappers::PFCatalogCreateUploadUrlsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogCreateUploadUrlsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogCreateUploadUrlsResponseWrapper<Allocator>;

    CreateUploadUrlsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogCreateUploadUrlsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class DeleteEntityItemReviewsOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogDeleteEntityItemReviewsRequestWrapper<Allocator>;

    DeleteEntityItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class DeleteItemOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogDeleteItemRequestWrapper<Allocator>;

    DeleteItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class GetCatalogConfigOperation : public XAsyncOperation<Wrappers::PFCatalogGetCatalogConfigResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetCatalogConfigRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetCatalogConfigResponseWrapper<Allocator>;

    GetCatalogConfigOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetCatalogConfigResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class GetDraftItemOperation : public XAsyncOperation<Wrappers::PFCatalogGetDraftItemResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetDraftItemRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetDraftItemResponseWrapper<Allocator>;

    GetDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetDraftItemResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetDraftItemsOperation : public XAsyncOperation<Wrappers::PFCatalogGetDraftItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetDraftItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetDraftItemsResponseWrapper<Allocator>;

    GetDraftItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetDraftItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetEntityDraftItemsOperation : public XAsyncOperation<Wrappers::PFCatalogGetEntityDraftItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetEntityDraftItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetEntityDraftItemsResponseWrapper<Allocator>;

    GetEntityDraftItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetEntityDraftItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetEntityItemReviewOperation : public XAsyncOperation<Wrappers::PFCatalogGetEntityItemReviewResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetEntityItemReviewRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetEntityItemReviewResponseWrapper<Allocator>;

    GetEntityItemReviewOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetEntityItemReviewResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetItemOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemResponseWrapper<Allocator>;

    GetItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetItemContainersOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemContainersResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemContainersRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemContainersResponseWrapper<Allocator>;

    GetItemContainersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemContainersResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class GetItemModerationStateOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemModerationStateResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemModerationStateRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemModerationStateResponseWrapper<Allocator>;

    GetItemModerationStateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemModerationStateResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class GetItemPublishStatusOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemPublishStatusResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemPublishStatusRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemPublishStatusResponseWrapper<Allocator>;

    GetItemPublishStatusOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemPublishStatusResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetItemReviewsOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemReviewsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemReviewsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemReviewsResponseWrapper<Allocator>;

    GetItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemReviewsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetItemReviewSummaryOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemReviewSummaryResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemReviewSummaryRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemReviewSummaryResponseWrapper<Allocator>;

    GetItemReviewSummaryOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemReviewSummaryResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetItemsOperation : public XAsyncOperation<Wrappers::PFCatalogGetItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogGetItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogGetItemsResponseWrapper<Allocator>;

    GetItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogGetItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class PublishDraftItemOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogPublishDraftItemRequestWrapper<Allocator>;

    PublishDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ReportItemOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogReportItemRequestWrapper<Allocator>;

    ReportItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ReportItemReviewOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogReportItemReviewRequestWrapper<Allocator>;

    ReportItemReviewOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ReviewItemOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogReviewItemRequestWrapper<Allocator>;

    ReviewItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class SearchItemsOperation : public XAsyncOperation<Wrappers::PFCatalogSearchItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogSearchItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogSearchItemsResponseWrapper<Allocator>;

    SearchItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogSearchItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class SetItemModerationStateOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogSetItemModerationStateRequestWrapper<Allocator>;

    SetItemModerationStateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class SubmitItemReviewVoteOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogSubmitItemReviewVoteRequestWrapper<Allocator>;

    SubmitItemReviewVoteOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class TakedownItemReviewsOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogTakedownItemReviewsRequestWrapper<Allocator>;

    TakedownItemReviewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class UpdateCatalogConfigOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCatalogUpdateCatalogConfigRequestWrapper<Allocator>;

    UpdateCatalogConfigOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class UpdateDraftItemOperation : public XAsyncOperation<Wrappers::PFCatalogUpdateDraftItemResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCatalogUpdateDraftItemRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCatalogUpdateDraftItemResponseWrapper<Allocator>;

    UpdateDraftItemOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCatalogUpdateDraftItemResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

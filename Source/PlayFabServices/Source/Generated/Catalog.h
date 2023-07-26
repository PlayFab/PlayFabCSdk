#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "CatalogTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Catalog
{

class CatalogAPI
{
public:
    CatalogAPI() = delete;
    CatalogAPI(const CatalogAPI& source) = delete;
    CatalogAPI& operator=(const CatalogAPI& source) = delete;
    ~CatalogAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<CreateDraftItemResponse> CreateDraftItem(Entity const& entity, const CreateDraftItemRequest& request, RunContext rc);
    static AsyncOp<CreateUploadUrlsResponse> CreateUploadUrls(Entity const& entity, const CreateUploadUrlsRequest& request, RunContext rc);
    static AsyncOp<void> DeleteEntityItemReviews(Entity const& entity, const DeleteEntityItemReviewsRequest& request, RunContext rc);
    static AsyncOp<void> DeleteItem(Entity const& entity, const DeleteItemRequest& request, RunContext rc);
    static AsyncOp<GetCatalogConfigResponse> GetCatalogConfig(Entity const& entity, const GetCatalogConfigRequest& request, RunContext rc);
    static AsyncOp<GetDraftItemResponse> GetDraftItem(Entity const& entity, const GetDraftItemRequest& request, RunContext rc);
    static AsyncOp<GetDraftItemsResponse> GetDraftItems(Entity const& entity, const GetDraftItemsRequest& request, RunContext rc);
    static AsyncOp<GetEntityDraftItemsResponse> GetEntityDraftItems(Entity const& entity, const GetEntityDraftItemsRequest& request, RunContext rc);
    static AsyncOp<GetEntityItemReviewResponse> GetEntityItemReview(Entity const& entity, const GetEntityItemReviewRequest& request, RunContext rc);
    static AsyncOp<GetItemResponse> GetItem(Entity const& entity, const GetItemRequest& request, RunContext rc);
    static AsyncOp<GetItemContainersResponse> GetItemContainers(Entity const& entity, const GetItemContainersRequest& request, RunContext rc);
    static AsyncOp<GetItemModerationStateResponse> GetItemModerationState(Entity const& entity, const GetItemModerationStateRequest& request, RunContext rc);
    static AsyncOp<GetItemPublishStatusResponse> GetItemPublishStatus(Entity const& entity, const GetItemPublishStatusRequest& request, RunContext rc);
    static AsyncOp<GetItemReviewsResponse> GetItemReviews(Entity const& entity, const GetItemReviewsRequest& request, RunContext rc);
    static AsyncOp<GetItemReviewSummaryResponse> GetItemReviewSummary(Entity const& entity, const GetItemReviewSummaryRequest& request, RunContext rc);
    static AsyncOp<GetItemsResponse> GetItems(Entity const& entity, const GetItemsRequest& request, RunContext rc);
    static AsyncOp<void> PublishDraftItem(Entity const& entity, const PublishDraftItemRequest& request, RunContext rc);
    static AsyncOp<void> ReportItem(Entity const& entity, const ReportItemRequest& request, RunContext rc);
    static AsyncOp<void> ReportItemReview(Entity const& entity, const ReportItemReviewRequest& request, RunContext rc);
    static AsyncOp<void> ReviewItem(Entity const& entity, const ReviewItemRequest& request, RunContext rc);
    static AsyncOp<SearchItemsResponse> SearchItems(Entity const& entity, const SearchItemsRequest& request, RunContext rc);
    static AsyncOp<void> SetItemModerationState(Entity const& entity, const SetItemModerationStateRequest& request, RunContext rc);
    static AsyncOp<void> SubmitItemReviewVote(Entity const& entity, const SubmitItemReviewVoteRequest& request, RunContext rc);
    static AsyncOp<void> TakedownItemReviews(Entity const& entity, const TakedownItemReviewsRequest& request, RunContext rc);
    static AsyncOp<void> UpdateCatalogConfig(Entity const& entity, const UpdateCatalogConfigRequest& request, RunContext rc);
    static AsyncOp<UpdateDraftItemResponse> UpdateDraftItem(Entity const& entity, const UpdateDraftItemRequest& request, RunContext rc);
};

} // namespace Catalog
} // namespace PlayFab

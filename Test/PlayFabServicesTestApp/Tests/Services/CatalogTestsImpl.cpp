#include "TestAppPch.h"
#include "CatalogTests.h"
#include "CatalogOperations.h"
#include "JsonUtils.h"
#include "Platform/PlatformUtils.h"
#include <HttpRequest.h>

namespace PlayFab
{
namespace Test
{

constexpr char kPermanentItemId[]{ "PermanentItemId" };
constexpr char kPermanentBundleId[]{ "PermanentBundleId" };

namespace
{
Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator> AltId(String id = "testId")
{
    Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator> altId;
    altId.SetType("FriendlyId");
    altId.SetValue(id);

    return altId;
}
}

Wrappers::PFCatalogCatalogItemWrapper<Allocator> MakeItem(EntityKey creator, String id = "testId")
{
    Wrappers::PFCatalogCatalogItemWrapper<Allocator> item;

    ModelVector<Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>> altIds;
    altIds.push_back(AltId(id));
    item.SetAlternateIds(altIds);
    item.SetCreatorEntity(creator);
    item.SetIsHidden(false);
    item.SetStartDate(12345);
    StringDictionaryEntryVector title;
    title.insert_or_assign("NEUTRAL", "titleVal");
    item.SetTitle(title);
    item.SetType("ugc");

    return item;
}

void ValidateItem(PFCatalogCatalogItem expected, PFCatalogCatalogItem actual, TestContext& tc)
{
    tc.AssertEqual(1u, actual.alternateIdsCount, "alternateIdsCount");
    tc.AssertEqual(expected.alternateIds[0]->type, actual.alternateIds[0]->type, "alternateIds[0]->type");
    tc.AssertEqual(expected.alternateIds[0]->value, actual.alternateIds[0]->value, "alternateIds[0]->value");
    tc.AssertTrue(actual.isHidden, "isHidden");
    tc.AssertEqual(*expected.isHidden, *actual.isHidden, "isHidden");
    tc.AssertTrue(actual.startDate, "startDate");
    tc.AssertEqual(*expected.startDate, *actual.startDate, "startDate");
    tc.AssertEqual(1u, actual.titleCount, "titleCount");
    tc.AssertEqual(expected.title[0].key, actual.title[0].key, "title[0].key");
    tc.AssertEqual(expected.title[0].value, actual.title[0].value, "title[0].value");
    tc.AssertEqual(expected.type, actual.type, "type");
}

AsyncOp<String> CreateDraftItem(Wrappers::PFCatalogCatalogItemWrapper<Allocator> item, Entity entity, RunContext rc, TestContext& tc)
{
    CreateDraftItemOperation::RequestType request;
    request.SetItem(item);
    
    return CreateDraftItemOperation::Run(entity, request, rc).Then([&, item](Result<CreateDraftItemOperation::ResultType> result) -> Result<String>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ValidateItem(item.Model(), *result.Payload().Model().item, tc);

        return Result<String>(result.ExtractPayload().Model().item->id);
    });
}

AsyncOp<void> ReviewItem(Entity reviewerEntity, String itemId, RunContext rc)
{
    ReviewItemOperation::RequestType request;
    request.SetAlternateId(AltId(itemId));
    request.SetEntity(reviewerEntity.EntityKey());
    Wrappers::PFCatalogReviewWrapper<Allocator> review;
    review.SetIsInstalled(true);
    review.SetItemVersion("one");
    review.SetRating(1);
    review.SetReviewText("text");
    review.SetTitle("title");
    request.SetReview(review);

    return ReviewItemOperation::Run(reviewerEntity, request, rc);
}

AsyncOp<void> DeleteDraftItem(Result<void> result, String itemId, Entity entity, RunContext rc, TestContext& tc)
{
    tc.RecordResult(std::move(result));

    PublishDraftItemOperation::RequestType request;
    request.SetAlternateId(AltId(itemId));
    request.SetEntity(entity.EntityKey());

    return PublishDraftItemOperation::Run(entity, request, rc).Then([&, itemId, entity, rc](Result<void> result) -> AsyncOp<void>
    {
        DeleteItemOperation::RequestType request;
        request.SetEntity(entity.EntityKey());
        request.SetAlternateId(AltId(itemId));

        return DeleteItemOperation::Run(entity, request, rc);
    });
}

struct CatalogTestsState
{
    String permanentItemId;
    String permanentBundleId;
    Wrappers::PFCatalogCatalogItemWrapper<Allocator> permanentItem;
    Wrappers::PFCatalogCatalogItemWrapper<Allocator> permanentBundle;
};

AsyncOp<void> CatalogTests::Initialize()
{
    auto initResult = MakeShared<HRESULT>();

    return ServicesTestClass::Initialize().Then([&, initResult](Result<void> result) -> AsyncOp<CreateDraftItemOperation::ResultType>
    {
        *initResult = result.hr;

        RETURN_IF_FAILED(*initResult);

        m_state = MakeShared<CatalogTestsState>();
        m_state->permanentItemId = kPermanentItemId + DefaultTitlePlayerId();
        m_state->permanentBundleId = kPermanentBundleId + DefaultTitlePlayerId();
        m_state->permanentItem = MakeItem(DefaultTitlePlayer().EntityKey(), m_state->permanentItemId);

        // Create a permanent item since the catalog publishing has possibly long processing times that can and do cause intermittent test failures
        CreateDraftItemOperation::RequestType request;
        request.SetItem(m_state->permanentItem);
        request.SetPublish(true);

        return CreateDraftItemOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, initResult](Result<CreateDraftItemOperation::ResultType> result) -> Result<void>
    {
        // Waiting a few seconds before continuing to ensure that item will be ready for other operations.
        Platform::Sleep(5000);

        // We don't care if Creating/Publishing the permanent item and bundle fails since it should already exist most times
        return *initResult;
    });
}

AsyncOp<void> CatalogTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void CatalogTests::TestCreateDraftItem(TestContext& tc)
{
    // Already covered in TestGetDraftItem
    tc.EndTest(S_OK);
}


void CatalogTests::TestCreateUploadUrls(TestContext& tc)
{
    constexpr char kTestName[]{ "testName" };

    CreateUploadUrlsOperation::RequestType request;
    ModelVector<Wrappers::PFCatalogUploadInfoWrapper<Allocator>> files;
    Wrappers::PFCatalogUploadInfoWrapper<Allocator> file;
    file.SetFileName(kTestName);
    files.push_back(file);
    request.SetFiles(files);

    CreateUploadUrlsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&, kTestName](Result<CreateUploadUrlsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.uploadUrlsCount, "uploadUrlsCount");
        tc.AssertEqual(kTestName, model.uploadUrls[0]->fileName, "uploadUrls[0]->fileName");
        tc.AssertTrue(model.uploadUrls[0]->id, "uploadUrls[0]->id");
        tc.AssertTrue(model.uploadUrls[0]->url, "uploadUrls[0]->url");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestDeleteEntityItemReviews(TestContext& tc)
{
    tc.Skip();
}
#endif

void CatalogTests::TestDeleteItem(TestContext& tc)
{
    // Already covered in TestGetDraftItem
    tc.EndTest(S_OK);
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestGetCatalogConfig(TestContext& tc)
{
    tc.Skip();
}
#endif

void CatalogTests::TestGetDraftItem(TestContext& tc)
{
    GetDraftItemOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetAlternateId(AltId(m_state->permanentItemId));

    return GetDraftItemOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetDraftItemOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        ValidateItem(m_state->permanentItem.Model(), *result.Payload().Model().item, tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetDraftItems(TestContext& tc)
{
    GetDraftItemsOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    ModelVector<Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>> altIds;
    altIds.push_back(AltId(m_state->permanentItemId));
    request.SetAlternateIds(altIds);

    return GetDraftItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetDraftItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        ValidateItem(m_state->permanentItem.Model(), *model.items[0], tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetEntityDraftItems(TestContext& tc)
{
    GetEntityDraftItemsOperation::RequestType request;
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetCount(1);

    return GetEntityDraftItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetEntityDraftItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        ValidateItem(m_state->permanentItem.Model(), *model.items[0], tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetEntityItemReview(TestContext& tc)
{
    auto reviewerEntity = MakeShared<std::optional<Entity>>();
    
    GetTitlePlayer("ReviewerEntity").Then([&, reviewerEntity](Result<Entity> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        *reviewerEntity = result.ExtractPayload();
        
        return ReviewItem(reviewerEntity->value(), m_state->permanentItemId, RunContext());
    })
    .Then([&, reviewerEntity](Result<void> result) -> AsyncOp<GetEntityItemReviewOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetEntityItemReviewOperation::RequestType request;
        request.SetAlternateId(AltId(m_state->permanentItemId));
        request.SetEntity(reviewerEntity->value().EntityKey());

        return GetEntityItemReviewOperation::Run(reviewerEntity->value(), request, RunContext());
    })
    .Then([&](Result<GetEntityItemReviewOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertTrue(result.Payload().Model().review, "review");
        tc.AssertEqual(1, result.Payload().Model().review->rating, "review->rating");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetItem(TestContext& tc)
{
    GetItemOperation::RequestType request;
    request.SetAlternateId(AltId(m_state->permanentItemId));
    request.SetEntity(DefaultTitlePlayer().EntityKey());

    GetItemOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetItemOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ValidateItem(m_state->permanentItem.Model(), *result.Payload().Model().item, tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetItemContainers(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    // Skipping until service paging bug is fixed
    tc.Skip();
    /*auto permanentBundle = MakeShared<Wrappers::PFCatalogCatalogItemWrapper<Allocator>>(MakeItem(TitleEntity().EntityKey(), m_state->permanentBundleId));

    GetItemOperation::RequestType request;
    request.SetAlternateId(AltId(m_state->permanentItemId));
    request.SetEntity(TitleEntity().EntityKey());

    GetItemOperation::Run(TitleEntity(), request, RunContext()).Then([&, permanentBundle](Result<GetItemOperation::ResultType> result) -> AsyncOp<CreateDraftItemOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Create a bundle that will not be deleted
        CreateDraftItemOperation::RequestType request;
        ModelVector<Wrappers::PFCatalogCatalogItemReferenceWrapper<Allocator>> refs;
        Wrappers::PFCatalogCatalogItemReferenceWrapper<Allocator> ref;
        ref.SetId(result.Payload().Model().item->id);
        refs.push_back(ref);
        permanentBundle->SetItemReferences(refs);
        permanentBundle->SetType("bundle");
        request.SetItem(*permanentBundle);

        return CreateDraftItemOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<CreateDraftItemOperation::ResultType> result) -> AsyncOp<void>
    {
        // Doesn't matter if the previous step failed
        PublishDraftItemOperation::RequestType request;
        request.SetAlternateId(AltId(m_state->permanentBundleId));
        request.SetEntity(TitleEntity().EntityKey());

        return PublishDraftItemOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<GetItemContainersOperation::ResultType>
    {
        // Doesn't matter if the previous step failed
        GetItemContainersOperation::RequestType request;
        request.SetAlternateId(AltId(m_state->permanentItemId));
        request.SetEntity(DefaultTitlePlayer().EntityKey());

        return GetItemContainersOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, permanentBundle](Result<GetItemContainersOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.containersCount, "containersCount");
        ValidateItem(permanentBundle->Model(), *model.containers[0], tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });*/
#else
    // Test code above uses TitleEntity so skipping on GDK
    tc.Skip();
#endif
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestGetItemModerationState(TestContext& tc)
{
    tc.Skip();
}
#endif

void CatalogTests::TestGetItemPublishStatus(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    GetItemPublishStatusOperation::RequestType request;
    request.SetAlternateId(AltId(m_state->permanentItemId));
    request.SetEntity(TitleEntity().EntityKey());

    GetItemPublishStatusOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetItemPublishStatusOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(PFCatalogPublishResult::Succeeded, *result.Payload().Model().result, "result");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // Unsupported (requires Title Entity)
    tc.Skip();
#endif
}

void CatalogTests::TestGetItemReviews(TestContext& tc)
{
    // Already covered in TestReportItemReview
    tc.EndTest(S_OK);
}

void CatalogTests::TestGetItemReviewSummary(TestContext& tc)
{
    GetTitlePlayer("ReviewerEntity").Then([&](Result<Entity> result) -> AsyncOp<void>
    {
        return ReviewItem(result.Payload(), m_state->permanentItemId, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<GetItemReviewSummaryOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetItemReviewSummaryOperation::RequestType request;
        request.SetAlternateId(AltId(m_state->permanentItemId));

        return GetItemReviewSummaryOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetItemReviewSummaryOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1, result.Payload().Model().reviewsCount, "reviewsCount");
        tc.AssertEqual(1, *result.Payload().Model().rating->count1Star, "rating->count1Star");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestGetItems(TestContext& tc)
{
    GetItemsOperation::RequestType request;
    ModelVector<Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>> altIds;
    altIds.push_back(AltId(m_state->permanentItemId));
    request.SetAlternateIds(altIds);
    request.SetEntity(DefaultTitlePlayer().EntityKey());

    GetItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        ValidateItem(m_state->permanentItem.Model(), *model.items[0], tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestPublishDraftItem(TestContext& tc)
{
    // Already covered in TestGetDraftItem
    tc.EndTest(S_OK);
}

void CatalogTests::TestReportItem(TestContext& tc)
{
    ReportItemOperation::RequestType request;
    request.SetAlternateId(AltId(m_state->permanentItemId));
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetConcernCategory(PFCatalogConcernCategory::Profanity);

    ReportItemOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestReportItemReview(TestContext& tc)
{
    GetTitlePlayer("ReviewerEntity").Then([&](Result<Entity> result) -> AsyncOp<void>
    {
        return ReviewItem(result.Payload(), m_state->permanentItemId, RunContext());
    })
    .Then([&](Result<void> result) -> AsyncOp<GetItemReviewsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetItemReviewsOperation::RequestType request;
        request.SetAlternateId(AltId(m_state->permanentItemId));
        request.SetCount(1);

        return GetItemReviewsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetItemReviewsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.reviewsCount, "reviewsCount");
        tc.AssertEqual(1, model.reviews[0]->rating, "reviews[0]->rating");

        ReportItemReviewOperation::RequestType request;
        request.SetReviewId(model.reviews[0]->reviewId);
        request.SetAlternateId(AltId(m_state->permanentItemId));
        request.SetConcernCategory(PFCatalogConcernCategory::Profanity);
        request.SetEntity(DefaultTitlePlayer().EntityKey());

        return ReportItemReviewOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void CatalogTests::TestReviewItem(TestContext& tc)
{
    // Already covered in TestReportItemReview
    tc.EndTest(S_OK);
}

void CatalogTests::TestSearchItems(TestContext& tc)
{
    SearchItemsOperation::RequestType request;
    request.SetCount(10);
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    request.SetSelect("startDate");
    request.SetFilter("alternateIds/any(id: id/value eq '" + m_state->permanentItemId + "')");

    SearchItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<SearchItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        ValidateItem(m_state->permanentItem.Model(), *model.items[0], tc);

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestSetItemModerationState(TestContext& tc)
{
    tc.Skip();
}
#endif

void CatalogTests::TestSubmitItemReviewVote(TestContext& tc)
{
    // Already covered in TestGetItemReviewSummary
    tc.EndTest(S_OK);
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestTakedownItemReviews(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CatalogTests::TestUpdateCatalogConfig(TestContext& tc)
{
    tc.Skip();
}
#endif

void CatalogTests::TestUpdateDraftItem(TestContext& tc)
{
    String itemId{ "updateDraftItemId" + DefaultTitlePlayerId() };
    auto item = MakeShared<Wrappers::PFCatalogCatalogItemWrapper<Allocator>>(MakeItem(DefaultTitlePlayer().EntityKey(), itemId));
    CreateDraftItem(*item, DefaultTitlePlayer(), RunContext(), tc).Then([&, item](Result<String> result) -> AsyncOp<UpdateDraftItemOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        item->SetId(result.Payload());
        item->SetStartDate(time(nullptr));

        UpdateDraftItemOperation::RequestType request;
        request.SetItem(*item);

        return UpdateDraftItemOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, item](Result<UpdateDraftItemOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ValidateItem(item->Model(), *result.Payload().Model().item, tc);

        return S_OK;
    })
    .Then([&, itemId](Result<void> result) -> AsyncOp<void>
    {
        // Cleanup
        return DeleteDraftItem(result, itemId, DefaultTitlePlayer(), RunContext(), tc);
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}


}
}

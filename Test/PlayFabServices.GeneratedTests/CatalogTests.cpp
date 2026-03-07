#include "TestAppPch.h"
#include "CatalogTests.h"

namespace PlayFab
{
namespace Test
{

void CatalogTests::AddTests()
{
    AddTest("TestCreateDraftItem", &CatalogTests::TestCreateDraftItem);
    AddTest("TestCreateUploadUrls", &CatalogTests::TestCreateUploadUrls);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestDeleteEntityItemReviews", &CatalogTests::TestDeleteEntityItemReviews);
#endif
    AddTest("TestDeleteItem", &CatalogTests::TestDeleteItem);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestGetCatalogConfig", &CatalogTests::TestGetCatalogConfig);
#endif
    AddTest("TestGetDraftItem", &CatalogTests::TestGetDraftItem);
    AddTest("TestGetDraftItems", &CatalogTests::TestGetDraftItems);
    AddTest("TestGetEntityDraftItems", &CatalogTests::TestGetEntityDraftItems);
    AddTest("TestGetEntityItemReview", &CatalogTests::TestGetEntityItemReview, true);
    AddTest("TestGetItem", &CatalogTests::TestGetItem);
    AddTest("TestGetItemContainers", &CatalogTests::TestGetItemContainers);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestGetItemModerationState", &CatalogTests::TestGetItemModerationState);
#endif
    AddTest("TestGetItemPublishStatus", &CatalogTests::TestGetItemPublishStatus);
    AddTest("TestGetItemReviews", &CatalogTests::TestGetItemReviews);
    AddTest("TestGetItemReviewSummary", &CatalogTests::TestGetItemReviewSummary);
    AddTest("TestGetItems", &CatalogTests::TestGetItems);
    AddTest("TestPublishDraftItem", &CatalogTests::TestPublishDraftItem);
    AddTest("TestReportItem", &CatalogTests::TestReportItem);
    AddTest("TestReportItemReview", &CatalogTests::TestReportItemReview);
    AddTest("TestReviewItem", &CatalogTests::TestReviewItem);
    AddTest("TestSearchItems", &CatalogTests::TestSearchItems);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestSetItemModerationState", &CatalogTests::TestSetItemModerationState);
#endif
    AddTest("TestSubmitItemReviewVote", &CatalogTests::TestSubmitItemReviewVote);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestTakedownItemReviews", &CatalogTests::TestTakedownItemReviews);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestUpdateCatalogConfig", &CatalogTests::TestUpdateCatalogConfig);
#endif
    AddTest("TestUpdateDraftItem", &CatalogTests::TestUpdateDraftItem);

}

}
}

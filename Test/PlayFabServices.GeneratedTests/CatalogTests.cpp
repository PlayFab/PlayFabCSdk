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
#if 0
    AddTest("TestDeleteEntityItemReviews", &CatalogTests::TestDeleteEntityItemReviews);
#endif
    AddTest("TestDeleteItem", &CatalogTests::TestDeleteItem);
#if 0
    AddTest("TestGetCatalogConfig", &CatalogTests::TestGetCatalogConfig);
#endif
    AddTest("TestGetDraftItem", &CatalogTests::TestGetDraftItem);
    AddTest("TestGetDraftItems", &CatalogTests::TestGetDraftItems);
    AddTest("TestGetEntityDraftItems", &CatalogTests::TestGetEntityDraftItems);
    AddTest("TestGetEntityItemReview", &CatalogTests::TestGetEntityItemReview);
    AddTest("TestGetItem", &CatalogTests::TestGetItem);
    AddTest("TestGetItemContainers", &CatalogTests::TestGetItemContainers);
#if 0
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
#if 0
    AddTest("TestSetItemModerationState", &CatalogTests::TestSetItemModerationState);
#endif
    AddTest("TestSubmitItemReviewVote", &CatalogTests::TestSubmitItemReviewVote);
#if 0
    AddTest("TestTakedownItemReviews", &CatalogTests::TestTakedownItemReviews);
#endif
#if 0
    AddTest("TestUpdateCatalogConfig", &CatalogTests::TestUpdateCatalogConfig);
#endif
    AddTest("TestUpdateDraftItem", &CatalogTests::TestUpdateDraftItem);

}

}
}

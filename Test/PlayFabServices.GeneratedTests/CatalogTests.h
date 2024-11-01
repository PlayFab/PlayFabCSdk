#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class CatalogTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct CatalogTestsState> m_state;

private:
    // Autogenerated tests
    void TestCreateDraftItem(TestContext& testContext);
    void TestCreateUploadUrls(TestContext& testContext);
#if 0
    void TestDeleteEntityItemReviews(TestContext& testContext);
#endif
    void TestDeleteItem(TestContext& testContext);
#if 0
    void TestGetCatalogConfig(TestContext& testContext);
#endif
    void TestGetDraftItem(TestContext& testContext);
    void TestGetDraftItems(TestContext& testContext);
    void TestGetEntityDraftItems(TestContext& testContext);
    void TestGetEntityItemReview(TestContext& testContext);
    void TestGetItem(TestContext& testContext);
    void TestGetItemContainers(TestContext& testContext);
#if 0
    void TestGetItemModerationState(TestContext& testContext);
#endif
    void TestGetItemPublishStatus(TestContext& testContext);
    void TestGetItemReviews(TestContext& testContext);
    void TestGetItemReviewSummary(TestContext& testContext);
    void TestGetItems(TestContext& testContext);
    void TestPublishDraftItem(TestContext& testContext);
    void TestReportItem(TestContext& testContext);
    void TestReportItemReview(TestContext& testContext);
    void TestReviewItem(TestContext& testContext);
    void TestSearchItems(TestContext& testContext);
#if 0
    void TestSetItemModerationState(TestContext& testContext);
#endif
    void TestSubmitItemReviewVote(TestContext& testContext);
#if 0
    void TestTakedownItemReviews(TestContext& testContext);
#endif
#if 0
    void TestUpdateCatalogConfig(TestContext& testContext);
#endif
    void TestUpdateDraftItem(TestContext& testContext);

};

} // namespace Test
} // namespace PlayFab

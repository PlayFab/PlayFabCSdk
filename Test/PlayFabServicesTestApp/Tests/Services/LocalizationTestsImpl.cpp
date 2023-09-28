#include "TestAppPch.h"
#include "LocalizationTests.h"
#include "LocalizationOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> LocalizationTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> LocalizationTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
void LocalizationTests::TestGetLanguageList(TestContext& tc)
{
    GetLanguageListOperation::RequestType request;
    GetLanguageListOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<GetLanguageListOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(384u, model.languageListCount, "languageCount");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif


}
}

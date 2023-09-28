#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class LocalizationTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct LocalizationTestsState> m_state;

private:
    // Autogenerated tests
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestGetLanguageList(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

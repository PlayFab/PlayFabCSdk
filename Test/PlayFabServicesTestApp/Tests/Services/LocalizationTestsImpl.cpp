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

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void LocalizationTests::TestGetLanguageList(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

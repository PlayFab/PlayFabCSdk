#include "TestAppPch.h"
#include "LibHCTests.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/PlatformUtils.h"
#include "Platform/PlayFabPal.h"

using namespace PlayFab::Platform;

namespace PlayFab
{
    namespace Test
    {

        void LibHCTests::AddTests()
        {
            // Tracing Tests
            AddTest("TestHCTrace", &LibHCTests::TestHCTrace);
            AddTest("TestHCTraceLarge", &LibHCTests::TestHCTraceLarge);
        }

        AsyncOp<void> LibHCTests::Initialize()
        {
            return CoreTestClass::Initialize();
        }

        AsyncOp<void> LibHCTests::Uninitialize()
        {
            return CoreTestClass::Uninitialize();
        }

        void LibHCTests::TestHCTrace(TestContext& tc)
        {
            HCTraceImplArea area{ "Test", HCTraceLevel::Important };
            HCTraceImplMessage(&area, area.Verbosity, "Testing small message");

            tc.EndTest(S_OK);
        }

        void LibHCTests::TestHCTraceLarge(TestContext& tc)
        {
            HCTraceImplArea area{ "Test", HCTraceLevel::Important };
            std::string message(8192, 'w');
            HCTraceImplMessage(&area, area.Verbosity, message.c_str());

            tc.EndTest(S_OK);
        }
    }
}

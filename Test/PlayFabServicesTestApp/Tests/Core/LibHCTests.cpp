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
            PFHCTraceImplArea area{ "Test", PFHCTraceLevel::Important };
            PFHCTraceImplMessage(&area, area.Verbosity, "Testing small message");

            tc.EndTest(S_OK);
        }

        void LibHCTests::TestHCTraceLarge(TestContext& tc)
        {
            PFHCTraceImplArea area{ "Test", PFHCTraceLevel::Important };
            std::vector<char> message(8192, 'w');
            PFHCTraceImplMessage(&area, area.Verbosity, message.data());

            tc.EndTest(S_OK);
        }
    }
}

#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
    namespace Test
    {

        class LibHCTests : public CoreTestClass
        {
        public:
            using CoreTestClass::CoreTestClass;

        private:
            // TestClass overrides
            void AddTests() override;

            AsyncOp<void> Initialize() override;
            AsyncOp<void> Uninitialize() override;

            // Test Methods
            void TestHCTrace(TestContext& testContext);
            void TestHCTraceLarge(TestContext& testContext);

        };

    }
}

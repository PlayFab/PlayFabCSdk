// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pch.h"
#include "actions.h"
#include "MemoryManager.h"

class TestSuiteMisc : public testing::Test
{
protected:
    void SetUp() override
    {
        reset_all();
    }

    void TearDown() override
    {
        pfgamesave_shutdown(true);
        reset_all();
        TEST_COUT << MemoryManager::Instance().Summary();
    }
};

TEST_F(TestSuiteMisc, Trivial)
{
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}

TEST_F(TestSuiteMisc, GameSaveInitAndCleanup)
{
    int i = 0;
    while (i < 2) // test higher if doing stress testing
    {
        i++;
        AHS(pfgamesave_init());
        AHS(pfgamesave_shutdown());
    }
}

TEST_F(TestSuiteMisc, CoreInitAndCleanup)
{
    AHS(PFInitialize(nullptr));
    XAsyncBlock async{};
    AHS(PFUninitializeAsync(&async));
    AHS(XAsyncGetStatus(&async, true));
}

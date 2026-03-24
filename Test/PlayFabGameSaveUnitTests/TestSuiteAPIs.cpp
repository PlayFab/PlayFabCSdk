// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pch.h"
#include "actions.h"
#include "MemoryManager.h"

class TestSuiteAPIs : public testing::Test
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

TEST_F(TestSuiteAPIs, UserAlreadyAdded)
{
    AHS(pfgamesave_download("DeviceA"));

    XAsyncBlock async{};
    ASSERT_EQ(PFGameSaveFilesAddUserWithUiAsync(g_gameState.localUserHandle, PFGameSaveFilesAddUserOptions::None, &async), E_PF_GAMESAVE_USER_ALREADY_ADDED);

    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteAPIs, SaveAlreadyInit)
{
    AHS(pfgamesave_download("DeviceA"));

    PFGameSaveInitArgs args = {};
    HRESULT hr = PFGameSaveFilesInitialize(&args);
    ASSERT_EQ(hr, E_PF_GAMESAVE_ALREADY_INITIALIZED);

    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteAPIs, UserNotAdded)
{
    AHS(pfgamesave_init());
    AHS(pfcore_loginuser());
    PFGameSaveFilesSyncState state;
    uint64_t current = { 0 };
    uint64_t total = { 0 };
    HRESULT hr = PFGameSaveFilesUiProgressGetProgress(g_gameState.localUserHandle, &state, &current, &total);
    ASSERT_EQ(hr, E_PF_GAMESAVE_USER_NOT_ADDED);

    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteAPIs, GameSaveNotInit)
{
    AHS(pfgamesave_init(false));
    AHS(pfcore_loginuser());
    PFGameSaveFilesSyncState state;
    uint64_t current = { 0 };
    uint64_t total = { 0 };
    HRESULT hr = PFGameSaveFilesUiProgressGetProgress(g_gameState.localUserHandle, &state, &current, &total);
    ASSERT_EQ(hr, E_PF_GAMESAVE_NOT_INITIALIZED);

    AHS(pfgamesave_shutdown());
}

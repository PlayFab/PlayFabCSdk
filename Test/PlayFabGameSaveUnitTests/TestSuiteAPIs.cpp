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

// ---------------------------------------------------------------------------------------------
// Regression tests for bug 4544470
// ---------------------------------------------------------------------------------------------

namespace
{

// Recursively counts per-attempt upload staging directories (cloudsync\up-<guid>) beneath a root.
// Walking rather than assuming a layout keeps these tests independent of where the save root and
// its cloudsync folder actually sit on a given platform.
size_t CountUploadStagingFolders(const std::string& root)
{
    namespace fs = std::filesystem;

    std::error_code ec;
    if (root.empty() || !fs::exists(root, ec))
    {
        return 0;
    }

    size_t count = 0;
    for (fs::recursive_directory_iterator it(root, ec), end; it != end; it.increment(ec))
    {
        if (ec)
        {
            break;
        }

        std::error_code isDirEc;
        if (it->is_directory(isDirEc) && !isDirEc)
        {
            const std::string name = it->path().filename().string();
            if (name.rfind("up-", 0) == 0)
            {
                ++count;
            }
        }
    }

    return count;
}

// Finds the cloudsync folder beneath a root, or an empty path if there is not one yet.
std::filesystem::path FindCloudSyncFolder(const std::string& root)
{
    namespace fs = std::filesystem;

    std::error_code ec;
    if (root.empty() || !fs::exists(root, ec))
    {
        return {};
    }

    for (fs::recursive_directory_iterator it(root, ec), end; it != end; it.increment(ec))
    {
        if (ec)
        {
            break;
        }

        std::error_code isDirEc;
        if (it->is_directory(isDirEc) && !isDirEc && it->path().filename() == "cloudsync")
        {
            return it->path();
        }
    }

    return {};
}

} // anonymous namespace

// Reading the result of a FINISHED operation must not depend on GameSave still being initialized.
//
// The defect: PFGameSaveFilesUploadWithUiResult ran through GSApiImpl, which fails with
// E_PF_GAMESAVE_NOT_INITIALIZED as soon as the global state pointer is cleared - and
// PFGameSaveFilesUninitializeAsync clears it at the START of cleanup. A title that uninitializes
// on suspend therefore got NOT_INITIALIZED for an upload that had already completed with a real
// error, permanently masking why the upload failed. That is exactly what the PlayStation half of
// bug 4544470 reported, and it is why the underlying failure there is still unknown.
TEST_F(TestSuiteAPIs, UploadResultIsReadableAfterUninitialize)
{
    AHS(pfgamesave_download("DeviceA"));

    XAsyncBlock async{};
    AHS(PFGameSaveFilesUploadWithUiAsync(
        g_gameState.localUserHandle,
        PFGameSaveFilesUploadOption::KeepDeviceActive,
        &async));
    AHS(XAsyncGetStatus(&async, true));

    const HRESULT whileInitialized = PFGameSaveFilesUploadWithUiResult(&async);
    TEST_COUT << "UploadWithUiResult while initialized: 0x" << std::hex << whileInitialized;

    // Uninitialize exactly the way a title does on suspend, then read the SAME completed block.
    AHS(pfgamesave_shutdown());

    const HRESULT afterUninitialize = PFGameSaveFilesUploadWithUiResult(&async);
    TEST_COUT << "UploadWithUiResult after uninitialize: 0x" << std::hex << afterUninitialize;

    EXPECT_NE(E_PF_GAMESAVE_NOT_INITIALIZED, afterUninitialize)
        << "uninitializing must not overwrite the completed operation's result";
    EXPECT_EQ(whileInitialized, afterUninitialize)
        << "the result of a finished operation must not change because global state went away";
}

// A successful upload must not leave its staging directory behind. Staging is per attempt now, so
// nothing else will clear it: the attempt that created it owns it.
TEST_F(TestSuiteAPIs, SuccessfulUploadLeavesNoStagingFolderBehind)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(create_folder("progress"));
    AHS(write_file("progress", "save.bin", "some save data"));

    AHS(pfgamesave_upload(false /*shutdownAfter*/, false, false, false, false, true /*keepDeviceActive*/));

    EXPECT_EQ(0u, CountUploadStagingFolders(g_gameState.saveFolder))
        << "a completed upload should have removed its own staging directory";

    AHS(pfgamesave_shutdown());
}

// Staging directories orphaned by a crash, a suspend, or a provider torn down mid-transfer are
// reclaimed at the next AddUser -- the one point where no upload can be in flight for the user.
//
// This is what makes it safe for an upload attempt to STOP deleting other attempts' files. The old
// code cleared every *.zip in one shared cloudsync folder on entry to compression, which is how a
// retry (or an upload admitted after an abandoned provider reset the sync state) deleted files a
// still-live transfer was about to open.
TEST_F(TestSuiteAPIs, OrphanedUploadStagingIsSweptOnAddUser)
{
    namespace fs = std::filesystem;

    AHS(pfgamesave_download("DeviceA"));
    AHS(create_folder("progress"));
    AHS(write_file("progress", "save.bin", "some save data"));

    // A real upload is what creates the cloudsync folder in the first place.
    AHS(pfgamesave_upload(false /*shutdownAfter*/, false, false, false, false, true /*keepDeviceActive*/));

    const fs::path cloudSync = FindCloudSyncFolder(g_gameState.saveFolder);
    ASSERT_FALSE(cloudSync.empty())
        << "expected a cloudsync folder under " << g_gameState.saveFolder << " after an upload";

    // Stand in for an attempt that never got to clean up after itself.
    const fs::path orphan = cloudSync / "up-orphaned-attempt";
    std::error_code ec;
    fs::create_directories(orphan, ec);
    ASSERT_FALSE(ec);
    {
        std::ofstream stale(orphan / "stale.zip", std::ios::binary);
        stale << "stale payload";
    }
    ASSERT_TRUE(fs::exists(orphan / "stale.zip"));

    AHS(pfgamesave_shutdown());

    // Re-adding the user is the sweep point.
    AHS(pfgamesave_download("DeviceA", false /*resetCloud*/));

    EXPECT_FALSE(fs::exists(orphan, ec))
        << "AddUser should have reclaimed the orphaned staging directory";

    AHS(pfgamesave_shutdown());
}


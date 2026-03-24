// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pch.h"
#include "actions.h"
#define USE_PIX
#include "pix3.h"
#include "MemoryManager.h"

class TestSuiteFileSync : public testing::Test
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

TEST_F(TestSuiteFileSync, OfflineTestListManifests)
{
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::UseOffline;
    AHS(pfgamesave_init());
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::ListManifests));
    AHS(pfgamesave_download("DeviceA", true, false, false, false, true));
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = false;
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_upload(false, false, false, true));
    AHS(verify_cloud_connected(false));
}

TEST_F(TestSuiteFileSync, OfflineTestInitializeManifest)
{
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::UseOffline;
    AHS(pfgamesave_init());
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::InitializeManifest));
    AHS(pfgamesave_download("DeviceA", true, false, false, false, true));
    AHS(verify_cloud_connected(false));
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = false;
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_upload(false, false, false, true));
    AHS(verify_cloud_connected(false));
}

TEST_F(TestSuiteFileSync, OfflineTestInitiateUpload)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));

    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Cancel;
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::InitiateUpload));
    AHS(pfgamesave_upload(false, false, false, false, true));
    AHS(verify_cloud_connected(true)); // upload failures don't change the cloud connected state
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = false;
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::None));

    AHS(pfgamesave_upload(false, false, false, false, false));
    AHS(verify_cloud_connected(true)); // upload failures don't change the cloud connected state
    pfgamesave_shutdown(true);

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 2));
    AHS(verify_cloud_connected(true));
}

TEST_F(TestSuiteFileSync, OfflineTestFinalizeManifest)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));

    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Cancel;
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::FinalizeManifest));
    AHS(pfgamesave_upload(false, false, false, false, true));
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackExpected = false;
    g_gameState.uiSyncFailedCallbackTriggered = false;
    AHS(verify_cloud_connected(true)); // upload failures don't change the cloud connected state
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::None));
    pfgamesave_shutdown(true);

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 0));
    AHS(verify_num_files("save02", 0));
    AHS(verify_cloud_connected(true));
}

TEST_F(TestSuiteFileSync, OfflineTestUploadFile)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));

    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Cancel;
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::UploadFile));
    AHS(pfgamesave_upload(false, false, false, false, true));
    AHS(verify_cloud_connected(true)); // upload failures don't change the cloud connected state
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackExpected = false;
    g_gameState.uiSyncFailedCallbackTriggered = false;
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::None));
    pfgamesave_shutdown(true);

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 0));
    AHS(verify_num_files("save02", 0));
    AHS(verify_cloud_connected(true));
}

TEST_F(TestSuiteFileSync, OfflineTestGetManifestDownloadDetails)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));
    AHS(pfgamesave_upload());

    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::UseOffline;
    AHS(pfgamesave_init());
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::GetManifestDownloadDetails));
    AHS(pfgamesave_download("DeviceB", true, false, false, false, true));
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackExpected = false;
    g_gameState.uiSyncFailedCallbackTriggered = false;
    AHS(verify_num_files("save01", 0));
    AHS(verify_num_files("save02", 0));
    AHS(verify_cloud_connected(false));
    AHS(pfgamesave_upload(false, false, false, true));
    AHS(verify_cloud_connected(false));
}

TEST_F(TestSuiteFileSync, OfflineTestDownloadFile)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));
    AHS(pfgamesave_upload());

    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::UseOffline;
    AHS(pfgamesave_init());
    AHS(debug_force_offline(GameSaveServiceMockForcedOffline::DownloadFile));
    AHS(pfgamesave_download("DeviceB", true, false, false, false, true));
    g_gameState.uiSyncFailedCallbackExpected = false;
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
    g_gameState.uiSyncFailedCallbackTriggered = false;
    AHS(verify_num_files("save01", 0));
    AHS(verify_num_files("save02", 0));
    AHS(verify_cloud_connected(false));
    AHS(pfgamesave_upload(false, false, false, true));
    AHS(verify_cloud_connected(false));
}

TEST_F(TestSuiteFileSync, SyncBetweeenDevices)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(verify_file("save02", "save4.dat", "testdata4"));
    AHS(write_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2testdata2"));
    AHS(verify_cloud_connected(true));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2testdata2"));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(verify_file("save02", "save4.dat", "testdata4"));
    AHS(verify_cloud_connected(true));
    AHS(pfgamesave_upload());
}

//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemorySyncBetweeenDevices)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(write_file("save02", "save3.dat", "testdata3"));
//    AHS(write_file("save02", "save4.dat", "testdata4"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(verify_file("save01", "save1.dat", "testdata1"));
//    AHS(verify_file("save01", "save2.dat", "testdata2"));
//    AHS(verify_file("save02", "save3.dat", "testdata3"));
//    AHS(verify_file("save02", "save4.dat", "testdata4"));
//    AHS(write_file("save01", "save1.dat", "testdata1testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2testdata2"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(verify_file("save01", "save1.dat", "testdata1testdata1"));
//    AHS(verify_file("save01", "save2.dat", "testdata2testdata2"));
//    AHS(verify_file("save02", "save3.dat", "testdata3"));
//    AHS(verify_file("save02", "save4.dat", "testdata4"));
//    AHS(pfgamesave_upload());
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, SyncNestedFolder)
{
    AHS(pfgamesave_download("DeviceA"));
    std::string subPath1 = FilePALTestApp::AppendPath("save01", "save01sub");
    std::string subPath2 = FilePALTestApp::AppendPath(FilePALTestApp::AppendPath("save02", "save02sub"), "save02subsub");
    AHS(write_file(subPath1.c_str(), "save1.dat", "testdata1"));
    AHS(write_file(subPath2.c_str(), "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file(subPath1.c_str(), "save1.dat", "testdata1"));
    AHS(verify_file(subPath2.c_str(), "save2.dat", "testdata2"));
    AHS(write_file(subPath1.c_str(), "save1.dat", "testdata1testdata1"));
    AHS(write_file(subPath2.c_str(), "save2.dat", "testdata2testdata2"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file(subPath1.c_str(), "save1.dat", "testdata1testdata1"));
    AHS(verify_file(subPath2.c_str(), "save2.dat", "testdata2testdata2"));
    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, SyncEmptyFolderAlongWithOtherFiles)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(create_folder("save02"));
    std::string subPath = FilePALTestApp::AppendPath("save02", "save02sub");
    AHS(create_folder(subPath));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_folder_exists("save02"));
    AHS(verify_folder_exists(subPath.c_str()));
    AHS(verify_num_files("save01", 1));
    AHS(verify_num_files("save02", 0));
    AHS(verify_num_files(subPath.c_str(), 0));
    AHS(write_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, DeleteFileTest)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 2));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 2));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(verify_file("save02", "save4.dat", "testdata4"));
    AHS(delete_file("save01", "save2.dat"));
    AHS(delete_file("save02", "save4.dat"));
    AHS(pfgamesave_upload()); // Also tests what happens if upload is "file deletes" only

    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_num_files("save01", 1));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(pfgamesave_upload());
}


//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemoryDeleteFileTest)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(write_file("save02", "save3.dat", "testdata3"));
//    AHS(write_file("save02", "save4.dat", "testdata4"));
//    AHS(verify_num_files("save01", 2));
//    AHS(verify_num_files("save02", 2));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(verify_num_files("save01", 2));
//    AHS(verify_num_files("save02", 2));
//    AHS(verify_file("save01", "save1.dat", "testdata1"));
//    AHS(verify_file("save01", "save2.dat", "testdata2"));
//    AHS(verify_file("save02", "save3.dat", "testdata3"));
//    AHS(verify_file("save02", "save4.dat", "testdata4"));
//    Sleep(1000);
//    AHS(delete_file("save01", "save2.dat"));
//    AHS(delete_file("save02", "save4.dat"));
//    AHS(pfgamesave_upload()); // Also tests what happens if upload is "file deletes" only
//
//    //AHS(pfgamesave_download("DeviceA", false));
//    //AHS(verify_num_files("save01", 1));
//    //AHS(verify_file("save01", "save1.dat", "testdata1"));
//    //AHS(verify_num_files("save02", 1));
//    //AHS(verify_file("save02", "save3.dat", "testdata3"));
//    //AHS(pfgamesave_upload());
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, SyncEmptyFolderOnly)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(create_folder("save01"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_folder_exists("save01"));
    AHS(verify_num_files("save01", 0));
    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, DeleteFolderTest)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(create_folder("save01"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_folder_exists("save01"));
    AHS(verify_num_files("save01", 0));
    AHS(create_folder("save02"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_folder_exists("save01"));
    AHS(verify_num_files("save01", 0));
    AHS(write_file("save02", "save2.dat", "testdata2"));
    AHS(verify_num_files("save02", 1));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB", false));
    AHS(verify_folder_exists("save01"));
    AHS(verify_num_files("save01", 0));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save02", "save2.dat", "testdata2"));
    AHS(delete_file("save02", "save2.dat"));
    AHS(delete_folder("save02"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_folder_exists("save01"));
    AHS(verify_num_files("save01", 0));
    ASSERT_TRUE(FAILED(verify_folder_exists("save02")));
}

//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemoryDeleteFolderTest)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(create_folder("save01"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(verify_folder_exists("save01"));
//    AHS(verify_num_files("save01", 0));
//    AHS(create_folder("save02"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceA", false));
//    AHS(verify_folder_exists("save01"));
//    AHS(verify_num_files("save01", 0));
//    AHS(write_file("save02", "save2.dat", "testdata2"));
//    AHS(verify_num_files("save02", 1));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB", false));
//    AHS(verify_folder_exists("save01"));
//    AHS(verify_num_files("save01", 0));
//    AHS(verify_num_files("save02", 1));
//    AHS(verify_file("save02", "save2.dat", "testdata2"));
//    AHS(delete_file("save02", "save2.dat"));
//    AHS(delete_folder("save02"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceA", false));
//    AHS(verify_folder_exists("save01"));
//    AHS(verify_num_files("save01", 0));
//    ASSERT_TRUE(FAILED(verify_folder_exists("save02")));
//    AHS(pfgamesave_upload());
//
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, DeleteFileWithOtherChanges)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 2));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 2));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(verify_file("save02", "save4.dat", "testdata4"));
    AHS(delete_file("save01", "save2.dat"));
    AHS(delete_file("save02", "save4.dat"));
    AHS(write_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(write_file("save02", "save3.dat", "testdata3testdata3"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_num_files("save01", 1));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save01", "save1.dat", "testdata1testdata1"));
    AHS(verify_file("save02", "save3.dat", "testdata3testdata3"));
    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, ConflictBetweeenDevicesTakeLocal)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_shutdown());

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdatatestdata5"));
    AHS(write_file("save01", "save2.dat", "testdatatestdata6"));
    AHS(write_file("save02", "save3.dat", "testdatatestdata7"));
    AHS(write_file("save02", "save4.dat", "testdatatestdata8"));
    AHS(pfgamesave_upload());

    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackExpected = true;
    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeLocal;
    AHS(pfgamesave_download("DeviceA", false)); // conflict should trigger here since files need to upload and download
    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
    AHS(verify_file("save02", "save3.dat", "testdata3"));
    AHS(verify_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_upload());
}

//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemoryConflictBetweeenDevicesTakeLocal)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(write_file("save02", "save3.dat", "testdata3"));
//    AHS(write_file("save02", "save4.dat", "testdata4"));
//    AHS(pfgamesave_shutdown());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(write_file("save01", "save1.dat", "testdatatestdata5"));
//    AHS(write_file("save01", "save2.dat", "testdatatestdata6"));
//    AHS(write_file("save02", "save3.dat", "testdatatestdata7"));
//    AHS(write_file("save02", "save4.dat", "testdatatestdata8"));
//    AHS(pfgamesave_upload());
//
//    g_gameState.uiConflictCallbackTriggered = false;
//    g_gameState.uiConflictCallbackExpected = true;
//    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeLocal;
//    AHS(pfgamesave_download("DeviceA", false)); // conflict should trigger here since files need to upload and download
//    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
//    AHS(verify_file("save01", "save1.dat", "testdata1"));
//    AHS(verify_file("save01", "save2.dat", "testdata2"));
//    AHS(verify_file("save02", "save3.dat", "testdata3"));
//    AHS(verify_file("save02", "save4.dat", "testdata4"));
//    AHS(pfgamesave_upload());
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, ConflictBetweeenDevicesTakeRemote)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_shutdown());

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdatatestdata5"));
    AHS(write_file("save01", "save2.dat", "testdatatestdata6"));
    AHS(write_file("save02", "save3.dat", "testdatatestdata7"));
    AHS(write_file("save02", "save4.dat", "testdatatestdata8"));
    AHS(pfgamesave_upload());

    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackExpected = true;
    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeRemote;
    AHS(pfgamesave_download("DeviceA", false)); // conflict should trigger here since files need to upload and download
    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
    AHS(verify_file("save01", "save1.dat", "testdatatestdata5"));
    AHS(verify_file("save01", "save2.dat", "testdatatestdata6"));
    AHS(verify_file("save02", "save3.dat", "testdatatestdata7"));
    AHS(verify_file("save02", "save4.dat", "testdatatestdata8"));
    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, ConflictBetweeenDevicesCancel)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_shutdown());

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdatatestdata5"));
    AHS(write_file("save01", "save2.dat", "testdatatestdata6"));
    AHS(write_file("save02", "save3.dat", "testdatatestdata7"));
    AHS(write_file("save02", "save4.dat", "testdatatestdata8"));
    AHS(pfgamesave_upload());

    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackExpected = true;
    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::Cancel;
    HRESULT hr = pfgamesave_download("DeviceA", false);
    ASSERT_TRUE(hr == E_PF_GAMESAVE_USER_CANCELLED);
    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
    AHS(pfgamesave_shutdown());
}

//TEST_F(TestSuiteFileSync, DISABLED_BUG_TODO_ConflictBetweenDevicesAtomicUnit)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01/a", "save1.dat", "testdata1"));
//    AHS(write_file("save01/a/b", "save2.dat", "testdata2"));
//    AHS(write_file("save02/c", "save3.dat", "testdata3"));
//    AHS(write_file("save02/c/d", "save4.dat", "testdata4"));
//    AHS(pfgamesave_shutdown());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(write_file("save01/a/b", "save5.dat", "testdata5"));
//    AHS(write_file("save02/e", "save6.dat", "testdata6"));
//    AHS(pfgamesave_upload());
//
//    g_gameState.uiConflictCallbackTriggered = false;
//    g_gameState.uiConflictCallbackExpected = true;
//    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeRemote;
//    AHS(pfgamesave_download("DeviceA", false));
//    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
//    AHS(verify_file("save01/a/b", "save5.dat", "testdata5"));
//    AHS(verify_file("save02/e", "save6.dat", "testdata6"));
//}

//TEST_F(TestSuiteFileSync, DISABLED_ActiveDeviceContentionSyncLast_MISSING_FEATURE)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(pfgamesave_shutdown());
//
//    // Technically, the default setting is TakeLast because lock is always reacquired after uploading.
//    g_gameState.uiActiveDeviceContentionCallbackTriggered = false;
//    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
//    g_gameState.uiActiveDeviceContentionCallbackAutomatedAction = TestActiveDeviceContention::SyncLast;
//    AHS(pfgamesave_download("DeviceB"));
//    ASSERT_TRUE(g_gameState.uiActiveDeviceContentionCallbackTriggered);
//    AHS(write_file("save01", "save1.dat", "testdata3"));
//    AHS(write_file("save01", "save2.dat", "testdata4"));
//
//    // TODO: DeviceA gets lock change notification
//    ASSERT_HRESULT_FAILED(pfgamesave_download("DeviceA", false));
//}
//
//TEST_F(TestSuiteFileSync, DISABLED_ActiveDeviceContentionTryAgainCancel_MISSING_FEATURE)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(pfgamesave_shutdown());
//
//    g_gameState.uiActiveDeviceContentionCallbackTriggered = false;
//    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
//    g_gameState.uiActiveDeviceContentionCallbackAutomatedAction = TestActiveDeviceContention::TryAgain;
//    AHS(pfgamesave_download("DeviceB"));
//    ASSERT_TRUE(g_gameState.uiActiveDeviceContentionCallbackTriggered);
//
//    // cancel download
//    g_gameState.uiActiveDeviceContentionCallbackTriggered = false;
//    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
//    g_gameState.uiActiveDeviceContentionCallbackAutomatedAction = TestActiveDeviceContention::Cancel;
//    AHS(pfgamesave_download("DeviceB"));
//    ASSERT_TRUE(g_gameState.uiActiveDeviceContentionCallbackTriggered);
//
//    // TODO: verify no files or folders exist on device B
//    ASSERT_HRESULT_FAILED(verify_folder_exists("save01"));
//}

TEST_F(TestSuiteFileSync, ConflictDeleteReplication)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(delete_file("save01", "save1.dat"));
    AHS(verify_num_files("save01", 0));
    AHS(pfgamesave_shutdown()); // DeviceB disconnects

    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_num_files("save01", 1));
    AHS(write_file("save01", "save1.dat", "testdata2testdata2testdata2"));
    AHS(pfgamesave_upload());

    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackExpected = true;
    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeRemote;
    AHS(pfgamesave_download("DeviceB", false));
    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
    AHS(verify_file("save01", "save1.dat", "testdata2testdata2testdata2"));
}

//TEST_F(TestSuiteFileSync, DISABLED_BUG_TODO_ConflictOutOfStorage)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(write_file("save01", "save1.dat", "testdata3testdata3"));
//    AHS(write_file("save01", "save2.dat", "testdata4testdata4"));
//    AHS(write_file("save02", "save1.dat", "testdata5"));
//    AHS(write_file("save02", "save2.dat", "testdata6"));
//    AHS(pfgamesave_shutdown());
//
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata7testdata7testdata7"));
//    AHS(write_file("save01", "save2.dat", "testdata8testdata8testdata8"));
//    AHS(write_file("save01", "save3.dat", "testdata9"));
//    AHS(write_file("save01", "save4.dat", "testdata10"));
//    AHS(pfgamesave_upload());
//
//    g_gameState.uiConflictCallbackTriggered = false;
//    g_gameState.uiConflictCallbackExpected = true;
//    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeRemote;
//
//    g_gameState.uiOutOfStorageCallbackTriggered = false;
//    g_gameState.uiOutOfStorageCallbackExpected = true;
//    g_gameState.uiOutOfStorageCallbackAutomatedAction = TestOutOfStorageUIChoice::Cancel;
//    HRESULT hr = pfgamesave_download("DeviceB", false, true);
//    ASSERT_TRUE(hr == E_PF_GAMESAVE_USER_CANCELLED);
//    ASSERT_TRUE(g_gameState.uiOutOfStorageCallbackTriggered);
//    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
//}

TEST_F(TestSuiteFileSync, OutOfStorageCancel)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    g_gameState.uiOutOfStorageCallbackTriggered = false;
    g_gameState.uiOutOfStorageCallbackExpected = true;
    g_gameState.uiOutOfStorageCallbackAutomatedAction = TestOutOfStorageUIChoice::Cancel;
    HRESULT hr = pfgamesave_download("DeviceB", false, true);
    ASSERT_TRUE(hr == E_PF_GAMESAVE_USER_CANCELLED);
    ASSERT_TRUE(g_gameState.uiOutOfStorageCallbackTriggered);
}

TEST_F(TestSuiteFileSync, OutOfStorageRetry)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    g_gameState.uiOutOfStorageCallbackTriggered = false;
    g_gameState.uiOutOfStorageCallbackExpected = true;
    g_gameState.uiOutOfStorageCallbackAutomatedAction = TestOutOfStorageUIChoice::Retry;
    AHS(pfgamesave_download("DeviceB", false, true));
    ASSERT_TRUE(g_gameState.uiOutOfStorageCallbackTriggered);
}

TEST_F(TestSuiteFileSync, ManifestDownloadUploadCompare)
{
    PlayFab::JsonDocument jsonDeviceAStats;
    PlayFab::JsonDocument jsonDeviceBStats;

    const char* fileName1 = "save1.dat";
    const char* fileName2 = "save2.dat";

    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", fileName1, "testdata1"));
    AHS(write_file("save01", fileName2, "testdata2"));
    AHS(pfgamesave_upload());

    // check both files are downloaded
    AHS(pfgamesave_download("DeviceB"));
    AHS(debug_get_stats_json(jsonDeviceBStats));
    assert(jsonDeviceBStats["FilesToDownload"].size() == 2);
    assert(strcmp(jsonDeviceBStats["FilesToDownload"][0]["Name"].get<std::string>().c_str(), fileName1) == 0);
    assert(strcmp(jsonDeviceBStats["FilesToDownload"][1]["Name"].get<std::string>().c_str(), fileName2) == 0);

    // Check only save01/save1.dat is uploaded
    AHS(write_file("save01", "save1.dat", "testdata33"));
    AHS(pfgamesave_upload(false));
    AHS(debug_get_stats_json(jsonDeviceBStats));
    assert(jsonDeviceBStats["FilesToUpload"].size() == 1);
    assert(strcmp(jsonDeviceBStats["FilesToUpload"][0]["Name"].get<std::string>().c_str(), fileName1) == 0);
    AHS(pfgamesave_shutdown());

    // Check only save01/save1.dat is downloaded
    AHS(pfgamesave_download("DeviceA", false)); // don't clear folder when downloading
    AHS(debug_get_stats_json(jsonDeviceAStats));
    assert(jsonDeviceAStats["FilesToDownload"].size() == 1);
    assert(strcmp(jsonDeviceAStats["FilesToDownload"][0]["Name"].get<std::string>().c_str(), fileName1) == 0);
}

TEST_F(TestSuiteFileSync, ManifestHighestFinalizedVersion)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdata3testdata3testdata3"));
    AHS(write_file("save01", "save2.dat", "testdata4testdata4testdata4"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceC"));
    AHS(verify_file("save01", "save1.dat", "testdata3testdata3testdata3"));
    AHS(verify_file("save01", "save2.dat", "testdata4testdata4testdata4"));
    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteFileSync, ManifestHighestFinalizedVersionSleep)
{
    // This test is a copy of ManifestHighestFinalizedVersion but instead of changing file size, timestamp is changed
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    Sleep(1000); // TODO: this should be able to be set lower, but something internally prevents

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdata3"));
    AHS(write_file("save01", "save2.dat", "testdata4"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceC"));
    AHS(verify_file("save01", "save1.dat", "testdata3"));
    AHS(verify_file("save01", "save2.dat", "testdata4"));
    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteFileSync, ManifestHighestPendingVersionDifferentDevice)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdatatestdata3"));
    AHS(write_file("save01", "save2.dat", "testdatatestdata4"));
    AHS(pfgamesave_shutdown());

    g_gameState.uiActiveDeviceContentionCallbackTriggered = false;
    g_gameState.uiActiveDeviceContentionCallbackExpected = true;
    g_gameState.uiActiveDeviceContentionCallbackAutomatedAction = TestActiveDeviceContention::SyncLast;
    AHS(pfgamesave_download("DeviceC"));
    ASSERT_TRUE(g_gameState.uiActiveDeviceContentionCallbackTriggered);
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save01", "save1.dat", "testtestdata5"));
    AHS(write_file("save01", "save2.dat", "testtestdata6"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceD"));
    AHS(verify_file("save01", "save1.dat", "testtestdata5"));
    AHS(verify_file("save01", "save2.dat", "testtestdata6"));
    AHS(pfgamesave_shutdown());
}

TEST_F(TestSuiteFileSync, ManifestHighestPendingVersionSameDevice)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload()); // finalized ver 1

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdata3testdata3"));
    AHS(write_file("save01", "save2.dat", "testdata4testdata4"));
    AHS(pfgamesave_shutdown()); // pending ver 2

    AHS(pfgamesave_download("DeviceB", false));
    AHS(verify_file("save01", "save1.dat", "testdata3testdata3"));
    AHS(verify_file("save01", "save2.dat", "testdata4testdata4"));
    AHS(write_file("save01", "save1.dat", "testdata50"));
    AHS(write_file("save01", "save2.dat", "testdata60"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceC"));
    AHS(verify_file("save01", "save1.dat", "testdata50"));
    AHS(verify_file("save01", "save2.dat", "testdata60"));
    AHS(pfgamesave_upload());
}

//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemoryManifestHighestPendingVersionSameDevice)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(pfgamesave_upload()); // finalized ver 1
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(write_file("save01", "save1.dat", "testdata3testdata3"));
//    AHS(write_file("save01", "save2.dat", "testdata4testdata4"));
//    AHS(pfgamesave_shutdown()); // pending ver 2
//
//    AHS(pfgamesave_download("DeviceB", false));
//    AHS(verify_file("save01", "save1.dat", "testdata3testdata3"));
//    AHS(verify_file("save01", "save2.dat", "testdata4testdata4"));
//    AHS(write_file("save01", "save1.dat", "testdata50"));
//    AHS(write_file("save01", "save2.dat", "testdata60"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceC"));
//    AHS(verify_file("save01", "save1.dat", "testdata50"));
//    AHS(verify_file("save01", "save2.dat", "testdata60"));
//    AHS(pfgamesave_upload());
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, ManifestFileSize)
{
    AHS(pfgamesave_download("DeviceA"));
    std::string save1 = "testdata1";
    std::string save2 = "testdata2";
    AHS(write_file("save01", "save1.dat", save1));
    AHS(write_file("save01", "save2.dat", save2));
    AHS(pfgamesave_upload(false));
    PlayFab::JsonDocument jsonDeviceAStats;
    AHS(debug_get_stats_json(jsonDeviceAStats));
    AHS(pfgamesave_shutdown());

    // Add up the size of the extracted files in the extended manifest and make sure it amounts to the size value written.
    size_t fileSum = save1.size() + save2.size();
    size_t manifestSum = 0;

    for (unsigned int i = 0; i < jsonDeviceAStats["FilesToUpload"].size(); i++)
    {
        manifestSum += jsonDeviceAStats["FilesToUpload"][i]["Size"].get<int>();
    }

    assert(fileSum == manifestSum);
}

TEST_F(TestSuiteFileSync, ManifestLastWriteCreateTime)
{
    PlayFab::JsonDocument jsonDeviceAStats;
    PlayFab::JsonDocument jsonDeviceBStats;

    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(pfgamesave_upload(false));

    // Verify create time of files in extended manifest
    debug_get_stats_json(jsonDeviceAStats);
    const time_t manifestCreateTime = jsonDeviceAStats["FilesToUpload"][0]["TimeCreated"].get<time_t>();

    std::filesystem::file_time_type save1CreateTime;
    debug_get_last_write_time(save1CreateTime, "save01", "save1.dat");
    // from https://stackoverflow.com/questions/73747853/lost-about-stdfilesystems-clock-conversions-in-c17-lacking-clock-cast
    auto sys_now = std::chrono::system_clock::now();
    auto fil_now = decltype(save1CreateTime)::clock::now();
    auto sys_ftime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(save1CreateTime - fil_now + sys_now);
    auto sys_ftime_usec = std::chrono::time_point_cast<std::chrono::seconds>(sys_ftime);
    const time_t createTime = sys_ftime_usec.time_since_epoch().count();

    assert(createTime == manifestCreateTime);

    AHS(pfgamesave_shutdown());

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdata2testdata2"));
    AHS(pfgamesave_upload(false));

    // Verify last write time of files in extended manifest
    debug_get_stats_json(jsonDeviceBStats);
    const time_t manifestLastWriteTime = jsonDeviceBStats["FilesToUpload"][0]["TimeLastModified"].get<time_t>();

    std::filesystem::file_time_type save1LastWriteTime;
    debug_get_last_write_time(save1LastWriteTime, "save01", "save1.dat");
    sys_ftime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(save1LastWriteTime - fil_now + sys_now);
    sys_ftime_usec = std::chrono::time_point_cast<std::chrono::seconds>(sys_ftime);
    const time_t lastWriteTime = sys_ftime_usec.time_since_epoch().count();

    assert(lastWriteTime == manifestLastWriteTime);
}

TEST_F(TestSuiteFileSync, SyncFileLargeUpdates)
{
    AHS(pfgamesave_download("DeviceA"));
    std::string s(10485760, 'a'); // 10 x 1024 x 1024
    AHS(write_file("save01", "save1.dat", s));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save01", "save1.dat", s));
    std::string s2(10000000, 'b');
    AHS(write_file("save01", "save1.dat", s2));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file("save01", "save1.dat", s2));
    AHS(pfgamesave_upload());
}

//TEST_F(TestSuiteFileSync, DISABLED_MEMBUG_MemoryFileLargeUpdates)
//{
//    MemoryManager::Instance().SetHooks();
//    AHS(pfgamesave_download("DeviceA"));
//    std::string s(10 * 1024 * 1024, 'a');
//    AHS(write_file("save01", "save1.dat", s));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(verify_file("save01", "save1.dat", s));
//    std::string s2(9 * 1024 * 1024, 'b');
//    AHS(write_file("save01", "save1.dat", s2));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(verify_file("save01", "save1.dat", s2));
//    AHS(pfgamesave_upload());
//    std::cout << MemoryManager::Instance().Summary() << std::endl;
//    ASSERT_TRUE(MemoryManager::Instance().HasUnfreedAllocations() == false);
//}

TEST_F(TestSuiteFileSync, SyncSpecialCharacters)
{
    // limitations for special characters?
    // also try with different languages
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save~!@#$", "save%^&.dat", "testdata"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save~!@#$", "save%^&.dat", "testdata"));
}

//TEST_F(TestSuiteFileSync, DISABLED_TESTBUG_SyncFileLongPathUpdates) // On Windows max path length + filename is 256
//{
//    AHS(pfgamesave_download("DeviceA"));
//    std::string s(220, 'a'); // Values above 220 fail
//    AHS(write_file("save01/" + s, "save1.dat", "testdata1"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    std::string s2(220, 'b');
//    AHS(verify_file("save01/" + s, "save1.dat", "testdata1"));
//    AHS(delete_file("save01/" + s, "save1.dat"));
//    AHS(delete_folder("save01/" + s));
//    AHS(write_file("save01/" + s2, "save1.dat", "testdata2"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(verify_file("save01/" + s2, "save1.dat", "testdata2"));
//    AHS(pfgamesave_upload());
//}

TEST_F(TestSuiteFileSync, ShowProgress)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));

    g_gameState.uiProgressCallbackTriggered = false;
    g_gameState.uiProgressCallbackAutomatedAction = TestProgressUIChoice::ShowProgress;
    AHS(pfgamesave_upload());
    ASSERT_TRUE(g_gameState.uiProgressCallbackTriggered);

    g_gameState.uiProgressCallbackTriggered = false;
    g_gameState.uiProgressCallbackAutomatedAction = TestProgressUIChoice::ShowProgress;
    AHS(pfgamesave_download("DeviceB"));
    ASSERT_TRUE(g_gameState.uiProgressCallbackTriggered);
}

//TEST_F(TestSuiteFileSync, DISABLED_BUG_TODO_ShowProgressCancel)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//
//    g_gameState.uiProgressCallbackTriggered = false;
//    g_gameState.uiProgressCallbackAutomatedAction = TestProgressUIChoice::Cancel;
//
//    AHS(pfgamesave_upload());
//
//    ASSERT_TRUE(g_gameState.uiProgressCallbackTriggered);
//
//    // BUG: assertion failed: assert(m_pendingScheduleThreadId == "") in Utils.cpp line 277
//}

TEST_F(TestSuiteFileSync, SyncFailedDownloadRetry)
{
    AHS(pfgamesave_download("DeviceA"));
    std::string s(10 * 1024 * 1024, 'a');
    std::string s2(10 * 1024 * 1024, 'b');
    AHS(write_file("save01", "save1.dat", s));
    AHS(write_file("save01", "save2.dat", s2));
    AHS(pfgamesave_upload(true));

    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Retry;
    pfgamesave_download("DeviceB", false, false, true, true, false);
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);

    AHS(verify_file("save01", "save1.dat", s));
    AHS(verify_file("save01", "save2.dat", s2));
}

TEST_F(TestSuiteFileSync, SyncFailedUploadRetry)
{
    AHS(pfgamesave_download("DeviceA"));
    std::string s(10 * 1024 * 1024, 'a');
    std::string s2(10 * 1024 * 1024, 'b');
    AHS(write_file("save01", "save1.dat", s));
    AHS(write_file("save01", "save2.dat", s2));

    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Retry;
    AHS(pfgamesave_upload(true, true));
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);

    pfgamesave_download("DeviceB");
    AHS(verify_file("save01", "save1.dat", s));
    AHS(verify_file("save01", "save2.dat", s2));
}

TEST_F(TestSuiteFileSync, SyncFailedCancel)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    AHS(write_file("save01", "save1.dat", "testdatatestdata3"));
    AHS(write_file("save01", "save2.dat", "testdatatestdata4"));
    AHS(pfgamesave_upload());

    g_gameState.uiSyncFailedCallbackExpected = true;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::Cancel;
    HRESULT hr = pfgamesave_download("DeviceA", false, false, true);
    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);

    AHS(verify_file_no_gamestate("DeviceA/save01", "save1.dat", "testdata1"));
    AHS(verify_file_no_gamestate("DeviceA/save01", "save2.dat", "testdata2"));
}

//TEST_F(TestSuiteFileSync, DISABLED_SyncFailedUseOffline_MISSING_FEATURE)
//{
//    AHS(pfgamesave_download("DeviceA"));
//    AHS(write_file("save01", "save1.dat", "testdata1"));
//    AHS(write_file("save01", "save2.dat", "testdata2"));
//    AHS(pfgamesave_upload());
//
//    AHS(pfgamesave_download("DeviceB"));
//    AHS(write_file("save01", "save1.dat", "testdatatestdata3"));
//    AHS(write_file("save01", "save2.dat", "testdatatestdata4"));
//    AHS(pfgamesave_upload());
//
//    g_gameState.uiSyncFailedCallbackExpected = true;
//    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::UseOffline;
//    HRESULT hr = pfgamesave_download("DeviceA", false, false, true);
//    ASSERT_TRUE(g_gameState.uiSyncFailedCallbackTriggered);
//
//    // Gamestate is still device B when debugging
//    AHS(verify_file_no_gamestate("DeviceA/save01", "save1.dat", "testdata1"));
//    AHS(verify_file_no_gamestate("DeviceA/save01", "save2.dat", "testdata2"));
//
//    // Device A writes files offline
//    AHS(write_file_no_gamestate("DeviceA/save01", "save1.dat", "testdata5"));
//    AHS(write_file_no_gamestate("DeviceA/save01", "save2.dat", "testdata6"));
//
//    // Device A comes online causing a conflict
//    g_gameState.uiConflictCallbackTriggered = false;
//    g_gameState.uiConflictCallbackExpected = true;
//    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::TakeRemote;
//    AHS(pfgamesave_download("DeviceA", false, false, false)); // Fails with error code 0x89237001 at PFGameSaveFilesInitialize
//    ASSERT_TRUE(g_gameState.uiConflictCallbackTriggered);
//}

TEST_F(TestSuiteFileSync, PerformanceNetworkSpeed)
{
    AHS(reset_all());

    AHS(pfgamesave_download("DeviceA"));
    std::string s(10485760, 'a'); // 10 x 1024 x 1024
    AHS(write_file("save01", "save1.dat", s));
    AHS(write_file("save01", "save2.dat", s));

    // Verify upload happens at full network speed
    auto timeStartUpload = std::chrono::high_resolution_clock::now();
    AHS(pfgamesave_upload(true, false, false));
    auto timeEndUpload = std::chrono::high_resolution_clock::now();
    long long durationUpload = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEndUpload - timeStartUpload).count();

    // Verify download happens at full network speed
    auto timeStartDownload = std::chrono::high_resolution_clock::now();
    AHS(pfgamesave_download("DeviceB"));
    auto timeEndDownload = std::chrono::high_resolution_clock::now();
    long long durationDownload = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEndDownload - timeStartDownload).count();

    // What to do with these values?

    AHS(pfgamesave_upload());
}

TEST_F(TestSuiteFileSync, CompressAndDecompressZipReuse)
{
    PlayFab::JsonDocument jsonDeviceAStats;
    PlayFab::JsonDocument jsonDeviceBStats;

    // Create 1st zip
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    //Create 2nd zip
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    std::string s1 = "testdatatestdata1";
    std::string s2 = "testdatatestdata2";
    AHS(write_file("save01", "save1.dat", s1));
    AHS(write_file("save01", "save2.dat", s2));
    AHS(pfgamesave_upload(false));
    AHS(debug_get_stats_json(jsonDeviceBStats));
    // Compression: Check that only zip containing save01/save1.dat and save02/save2.dat is uploaded
    assert(jsonDeviceBStats["CompressedFilesToUpload"].size() == 1);
    assert(jsonDeviceBStats["CompressedFilesToUpload"][0]["Size"] == (s1.size() + s2.size()));
    AHS(pfgamesave_shutdown());

    AHS(pfgamesave_download("DeviceA", false));
    AHS(debug_get_stats_json(jsonDeviceAStats));
    // Decompression: Check that only zip containing save01/save1.dat and save02/save2.dat is downloaded
    assert(jsonDeviceAStats["CompressedFilesToDownload"].size() == 1);
    assert(jsonDeviceAStats["CompressedFilesToDownload"][0]["Size"] == (s1.size() + s2.size()));
    assert(jsonDeviceAStats["CompressedFilesToDownload"][0]["DownloadedLocally"].get<bool>());
}

TEST_F(TestSuiteFileSync, CompressZipCreation)
{
    PlayFab::JsonDocument jsonDeviceBStats;

    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(write_file("save02", "save3.dat", "testdata3"));
    AHS(write_file("save02", "save4.dat", "testdata4"));
    AHS(pfgamesave_upload());

    AHS(pfgamesave_download("DeviceB"));
    std::string s1 = "testdatatestdata1";
    std::string s2 = "testdatatestdata2";
    AHS(write_file("save01", "save1.dat", s1));
    AHS(write_file("save01", "save2.dat", s2));
    AHS(pfgamesave_upload(false));
    AHS(debug_get_stats_json(jsonDeviceBStats));
    AHS(pfgamesave_shutdown());

    assert(jsonDeviceBStats["CompressedFilesToUpload"].size() == 1);
    assert(jsonDeviceBStats["CompressedFilesToUpload"][0]["Size"] == (s1.size() + s2.size()));

    // Check original save1.dat and save2.dat is dead
    assert(jsonDeviceBStats["SkippedFiles"].size() == 2);
    assert(jsonDeviceBStats["SkippedFiles"][0]["Size"] == 9); // testdata1
    assert(jsonDeviceBStats["SkippedFiles"][0]["Name"] == "save1.dat");
    assert(jsonDeviceBStats["SkippedFiles"][1]["Size"] == 9); // testdata2
    assert(jsonDeviceBStats["SkippedFiles"][1]["Name"] == "save2.dat");
}

TEST_F(TestSuiteFileSync, CompressDecompressBigZip)
{
    AHS(pfgamesave_download("DeviceA"));
    // Make a single zip with many files
    for (int i = 1; i < 100; i++)
    {
        AHS(write_file("save01", "save" + std::to_string(i) + ".dat", "testdata" + std::to_string(i)));
    }
    AHS(pfgamesave_upload());

    PlayFab::JsonDocument jsonDeviceBStats;
    AHS(pfgamesave_download("DeviceB"));
    AHS(debug_get_stats_json(jsonDeviceBStats));
    // Checking unzip happened successfully
    uint64_t sizeSum = 0;
    for (int i = 1; i < 100; i++)
    {
        std::string s = "testdata" + std::to_string(i);
        AHS(verify_file("save01", "save" + std::to_string(i) + ".dat", s));
        sizeSum += s.size();
    }

    assert(jsonDeviceBStats["CompressedFilesToDownload"][0]["Size"] == sizeSum);
}

TEST_F(TestSuiteFileSync, ResiliencyCorruptedUpload)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));

    AHS(debug_force_pause_upload());
    std::thread thread_object([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // During upload, the files are "corrupted" by manually editing them
        AHS(write_file_no_gamestate("DeviceA/save01", "save1.dat", "corruption1"));
        AHS(write_file_no_gamestate("DeviceA/save01", "save2.dat", "corruption2"));
        AHS(debug_force_resume_upload());
    });
    AHS(pfgamesave_upload());

    thread_object.join();

    // Check that the corruption is rejected
    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
}

TEST_F(TestSuiteFileSync, ResiliencyCorruptedLocalState)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));
    AHS(pfgamesave_upload());

    AHS(write_localstate("DeviceA", "foo"));

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
}

TEST_F(TestSuiteFileSync, ResiliencyCorruptedManifest)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "save1.dat", "testdata1"));
    AHS(write_file("save01", "save2.dat", "testdata2"));

    AHS(debug_force_pause_upload());
    std::thread thread_object([]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // During upload, the files are "corrupted" by manually editing them
        AHS(write_manifest("foo"));
        AHS(debug_force_resume_upload());
    });
    AHS(pfgamesave_upload());

    thread_object.join();

    AHS(pfgamesave_download("DeviceA"));
    AHS(verify_file("save01", "save1.dat", "testdata1"));
    AHS(verify_file("save01", "save2.dat", "testdata2"));
}

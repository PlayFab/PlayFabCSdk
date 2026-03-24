// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#pragma once
#include "PFGameSaveFilesForDebug.h"

#define TEST_COUT  TestCout()
#define TEST_ERR_COUT  TestErrCout()
#define RIF(expr) { HRESULT hrTemp = expr; RIFHelper(hrTemp, #expr); RETURN_IF_FAILED(hrTemp); }
#define AHS(expr) AHSHelper(expr, #expr)

typedef void ProcessDataFn();

void AHSHelper(HRESULT hr, const char* fn);
HRESULT RIFHelper(HRESULT hr, const char* fn);
HRESULT pfcore_loginuser();
HRESULT pfgamesave_init(bool initGameSave = true);
HRESULT pfgamesave_upload(bool shutdownAfter = true, bool forceSyncFailError = false, bool forceUploadDelay = false, bool allowOfflineError = false, bool allowUserCancelError = false, bool keepDeviceActive = false);
HRESULT pfgamesave_shutdown(bool ignoreFailures = false);
HRESULT pfgamesave_reset_cloud();
HRESULT pfgamesave_get_folder(bool allowOfflineError = false);
void pfgamesave_syncdevice(const std::string& deviceName, ProcessDataFn processDataFn, bool upload = true, bool cleanup = true);
HRESULT pfgamesave_download(const std::string& deviceName, bool resetCloud = true, bool forceOutOfStorageError = false, bool forceSyncFailError = false, bool init = true, bool allowOfflineError = false);

HRESULT debug_reset_cloud(std::string folder);
HRESULT reset_all();
HRESULT debug_set_device_name(std::string deviceName);
HRESULT debug_set_root_folder(std::string rootFolder);
HRESULT debug_force_out_of_storage(bool force);
HRESULT debug_force_sync_fail(bool force);
HRESULT debug_force_pause_upload();
HRESULT debug_force_resume_upload();
HRESULT debug_set_data_folder(std::string dataFolder);
HRESULT debug_get_stats_json(_Out_ PlayFab::JsonDocument& stats);
HRESULT debug_force_offline(GameSaveServiceMockForcedOffline mode);
HRESULT create_folder(std::string folderName);
HRESULT write_file(std::string folderName, std::string fileName, std::string data);
HRESULT write_file_no_gamestate(std::string deviceFolderName, std::string fileName, std::string data);
HRESULT write_manifest(std::string data); // TODO: more detailed editors for this, localstate, and extended manifest files
HRESULT write_localstate(std::string deviceFolderName, std::string data);
HRESULT write_extended_manifest(std::string deviceFolderName, std::string fileName, std::string data);
HRESULT verify_file(std::string folderName, std::string fileName, std::string data);
HRESULT verify_file_no_gamestate(std::string deviceFolderName, std::string fileName, std::string data);
HRESULT verify_num_files(std::string folderName, int numFiles);
HRESULT verify_folder_exists(std::string folderName);
HRESULT verify_cloud_connected(bool isCloudConnected);
HRESULT delete_file(std::string folderName, std::string fileName);
HRESULT delete_folder(std::string folderName);
HRESULT debug_get_last_write_time(_Out_ std::filesystem::file_time_type& lastWriteTime, std::string folderName, std::string fileName);

void AutomatedPFGameSaveFilesUiProgressCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesSyncState syncState, _In_ void* context);
void AutomatedPFGameSaveFilesUiSyncFailedCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesSyncState syncState, _In_ HRESULT error, _In_ void* context);
void AutomatedPFGameSaveFilesUiActiveDeviceContentionCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveDescriptor* localGameSave, _In_ PFGameSaveDescriptor* remoteGameSave, _In_ void* context);
void AutomatedPFGameSaveFilesUiConflictCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveDescriptor* localGameSave, _In_ PFGameSaveDescriptor* remoteGameSave, _In_ void* context);
void AutomatedPFGameSaveFilesUiOutOfStorageCallback(_In_ PFLocalUserHandle localUserHandle, _In_ uint64_t requiredBytes, _In_ void* context);

enum class TestProgressUIChoice
{
    ShowProgress,
    Cancel
};

enum class TestSyncFailedUIChoice
{
    NoChoice,
    Retry,
    UseOffline,
    Cancel
};

enum class TestTakeUIChoice
{
    NoChoice,
    TakeLocal,
    TakeRemote,
    Cancel
};

enum class TestActiveDeviceContention
{
    NoChoice,
    TryAgain,
    SyncLast,
    Cancel
};

enum class TestOutOfStorageUIChoice
{
    NoChoice,
    Retry,
    Cancel
};

struct GameState
{
    PFServiceConfigHandle serviceConfigHandle{ nullptr };
    PFLocalUserHandle localUserHandle{ nullptr };
    std::string saveFolder;
    std::string customPlayerId;
    std::string localRoot;
    std::string debugDataFolder;
    std::shared_ptr<HttpMock> loginMock;

    bool uiProgressCallbackTriggered{ false };
    TestProgressUIChoice uiProgressCallbackAutomatedAction{ TestProgressUIChoice::ShowProgress };

    bool uiSyncFailedCallbackExpected{ false };
    bool uiSyncFailedCallbackTriggered{ false };
    TestSyncFailedUIChoice uiSyncFailedCallbackAutomatedAction{ TestSyncFailedUIChoice::NoChoice };

    bool uiConflictCallbackExpected{ false };
    bool uiConflictCallbackTriggered{ false };
    TestTakeUIChoice uiConflictCallbackAutomatedAction{ TestTakeUIChoice::NoChoice };

    bool uiActiveDeviceContentionCallbackExpected{ false };
    bool uiActiveDeviceContentionCallbackTriggered{ false };
    TestActiveDeviceContention uiActiveDeviceContentionCallbackAutomatedAction{ TestActiveDeviceContention::SyncLast };

    bool uiOutOfStorageCallbackExpected{ false };
    bool uiOutOfStorageCallbackTriggered{ false };
    TestOutOfStorageUIChoice uiOutOfStorageCallbackAutomatedAction{ TestOutOfStorageUIChoice::NoChoice };
};
extern GameState g_gameState;
extern std::mutex g_logMutex;

class TestCout : public std::stringstream
{
public:
    ~TestCout()
    {
        std::lock_guard<std::mutex> lock{ g_logMutex };
        std::cout << "\u001b[32m[          ] \u001b[33m" << str() << "\u001b[0m" << std::endl;
    }
};

class TestErrCout : public std::stringstream
{
public:
    static std::mutex s_logMutex;
    ~TestErrCout()
    {
        std::lock_guard<std::mutex> lock{ g_logMutex };
        std::cout << "\u001b[31m[          ] \u001b[31m" << str() << "\u001b[0m" << std::endl;
    }
};
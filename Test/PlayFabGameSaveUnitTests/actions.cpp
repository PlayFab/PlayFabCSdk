// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include "pch.h"
#include "actions.h"
#include "PFGameSaveFilesForDebug.h"
#define USE_MOCKS
#define USE_PIX
#include "pix3.h"

HRESULT debug_set_data_folder(std::string dataFolder);
std::mutex g_logMutex;
bool g_verboseLogs = false;

void AutomatedDebugTrace(
    _In_z_ const char* areaName,
    _In_ HCTraceLevel level,
    _In_ uint64_t threadId,
    _In_ uint64_t timestamp,
    _In_z_ const char* message
)
{
    //std::string msg = message;
    //if (msg.rfind("[GAME SAVE]", 0) == 0 ||
    //    msg.rfind("[FILEPAL]", 0) == 0 ||
    //    msg.rfind("[XAsyncProviderBase]", 0) == 0)
    {
        //if (g_verboseLogs)
        {
            // Needs to match the HCTraceLevel enum
            static char const* traceLevelNames[] =
            {
                "Off",
                "E",
                "W",
                "P",
                "I",
                "V",
            };

            // Uncomment to log trace info to screen
            TEST_COUT << "[" << traceLevelNames[static_cast<size_t>(level)] << "] " << message;

            // Uncomment to log trace info to debug output
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
        }
    }
}

HRESULT pfgamesave_init(bool initGameSave)
{
    // Init PF Core
    TEST_COUT << "PFInitialize";
    g_gameState.loginMock = nullptr;

    RIF(PFInitialize(nullptr));

    g_gameState.loginMock = std::make_shared<HttpMock>("POST", "https://E18D7.playfabapi.com/Client/LoginWithCustomID");
    g_gameState.loginMock->SetResponseHttpStatus(200);
    g_gameState.loginMock->SetResponseBody("{\"code\":200,\"status\":\"OK\",\"data\":{\"SessionTicket\":\"CA85570359462621-624BD2B0B8811A52-8E7CAC0897664A31-E18D7-8DC534A41EAC854-bCrvPAADe3lroy3K+p0D8bG6rx170fu7VgzY0RlJ+Po=\",\"PlayFabId\":\"CA85570359462621\",\"NewlyCreated\":false,\"SettingsForUser\":{\"NeedsAttribution\":false,\"GatherDeviceInfo\":true,\"GatherFocusInfo\":true},\"LastLoginTime\":\"2024-04-02T19:22:37.598Z\",\"EntityToken\":{\"EntityToken\":\"NHxtR1RIK3FPTUhKMU5yenZkRko5M01RR21YSmNMRVFLTElMZkRscVNrK0l3PXx7ImkiOiIyMDI0LTA0LTAyVDE5OjIyOjM3WiIsImlkcCI6IkN1c3RvbSIsImUiOiIyMDI0LTA0LTAzVDE5OjIyOjM3WiIsImZpIjoiMjAyNC0wNC0wMlQxOToyMjozN1oiLCJ0aWQiOiIwTUd6RS9lOS9ZdyIsImlkaSI6ImNsb3Vkc2F2ZS1wbGF5ZXItYXV0b21hdGVkMSIsImgiOiJpbnRlcm5hbCIsImVjIjoidGl0bGVfcGxheWVyX2FjY291bnQhNjI0QkQyQjBCODgxMUE1Mi9FMThENy9DQTg1NTcwMzU5NDYyNjIxLzhFN0NBQzA4OTc2NjRBMzEvIiwiZWkiOiI4RTdDQUMwODk3NjY0QTMxIiwiZXQiOiJ0aXRsZV9wbGF5ZXJfYWNjb3VudCJ9\",\"TokenExpiration\":\"2124-04-03T19:22:37Z\",\"Entity\":{\"Id\":\"8E7CAC0897664A31\",\"Type\":\"title_player_account\",\"TypeString\":\"title_player_account\"}},\"TreatmentAssignment\":{\"Variants\":[],\"Variables\":[]}}}");

    RIF(HCSettingsSetTraceLevel(HCTraceLevel::Information));
    //HCTraceSetTraceToDebugger(false);
    HCTraceSetClientCallback(AutomatedDebugTrace);
    RIF(PFServiceConfigCreateHandle(
        "https://E18D7.playfabapi.com",    // PlayFab API endpoint - obtained in the Game Manager
        "E18D7",                           // PlayFab Title id - obtained in the Game Manager
        &g_gameState.serviceConfigHandle));

    if (initGameSave)
    {
        TEST_COUT << "PFGameSaveFilesInitialize";
        PFGameSaveInitArgs args = {};
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        args.saveFolder = g_gameState.localRoot.c_str();
#endif
        RIF(PFGameSaveFilesInitialize(&args));

        if (!g_gameState.debugDataFolder.empty())
        {
            RIF(PFGameSaveFilesSetMockDataFolderForDebug(g_gameState.debugDataFolder.c_str()));
        }

        RIF(PFGameSaveFilesSetWriteManifestsToDiskForDebug(true));

        PFGameSaveUICallbacks callbacks{};
        callbacks.progressCallback = AutomatedPFGameSaveFilesUiProgressCallback;
        callbacks.syncFailedCallback = AutomatedPFGameSaveFilesUiSyncFailedCallback;
        callbacks.activeDeviceContentionCallback = AutomatedPFGameSaveFilesUiActiveDeviceContentionCallback;
        callbacks.conflictCallback = AutomatedPFGameSaveFilesUiConflictCallback;
        callbacks.outOfStorageCallback = AutomatedPFGameSaveFilesUiOutOfStorageCallback;

        RIF(PFGameSaveFilesSetUiCallbacks(&callbacks));
    }

    return S_OK;
}

HRESULT MyPFLocalUserLoginHandler(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_opt_ PFEntityHandle existingEntityHandle,
    _Inout_ XAsyncBlock* async
    )
{
    // Use PF Core to call PlayFab's LoginWithCustomID (or another PlayFab LoginWith* API)
    PFAuthenticationLoginWithCustomIDRequest request{};
    request.createAccount = true;
    request.customId = g_gameState.customPlayerId.c_str();

    return PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request, async);
}

HRESULT pfcore_loginuser()
{
    return PFLocalUserCreateHandleWithPersistedLocalId(
        g_gameState.serviceConfigHandle,
        "1",
        MyPFLocalUserLoginHandler,
        nullptr,
        &g_gameState.localUserHandle);
}

HRESULT pfgamesave_upload(bool shutdownAfter, bool forceSyncFailError, bool forceUploadDelay, bool allowOfflineError, bool allowUserCancelError, bool keepDeviceActive)
{
#ifdef USE_PIX
    PIXBeginEvent(PIX_COLOR(0, 255, 0), "Upload");
    PIXEndEvent();
#endif

    // sync game state to the cloud and display UI
    // typically this should be done in based on user action (e.g pressing a "Sync Save to Cloud" button)
    XAsyncBlock async{};
    TEST_COUT << "PFGameSaveFilesUploadWithUIAsync";

    RIF(debug_force_sync_fail(forceSyncFailError));

    auto option = keepDeviceActive ? PFGameSaveFilesUploadOption::KeepDeviceActive : PFGameSaveFilesUploadOption::ReleaseDeviceAsActive;
        
    RIF(PFGameSaveFilesUploadWithUiAsync(g_gameState.localUserHandle, option, &async));

    HRESULT hr = XAsyncGetStatus(&async, true);
    if (allowOfflineError && hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD) // Ignore this error code
    {
        hr = S_OK;
    }
    if (allowUserCancelError && hr == E_PF_GAMESAVE_USER_CANCELLED) // Ignore this error code
    {
        hr = S_OK;
    }

    TEST_COUT << "PFGameSaveFilesUploadWithUIAsync XAsyncGetStatus";
    RIF(hr);

    if (shutdownAfter)
    {
        RIF(pfgamesave_shutdown());
    }

#ifdef USE_PIX
    PIXEndEvent();
#endif

    return S_OK;
}

HRESULT pfgamesave_shutdown(bool ignoreFailures)
{
    debug_force_offline(GameSaveServiceMockForcedOffline::None);

    if (g_gameState.serviceConfigHandle != nullptr)
    {
        TEST_COUT << "PFServiceConfigCloseHandle";
        PFServiceConfigCloseHandle(g_gameState.serviceConfigHandle);
        g_gameState.serviceConfigHandle = nullptr;
    }

    if (g_gameState.localUserHandle != nullptr)
    {
        TEST_COUT << "PFEntityCloseHandle";
        PFLocalUserCloseHandle(g_gameState.localUserHandle);
        g_gameState.localUserHandle = nullptr;
    }

    TEST_COUT << "PFGameSaveFilesUninitializeAsync";
    XAsyncBlock async{};
    HRESULT hr = PFGameSaveFilesUninitializeAsync(&async);
    if (SUCCEEDED(hr))
    {
        hr = XAsyncGetStatus(&async, true);
        PFGameSaveFilesUninitializeResult(&async);
    }
    g_gameState.loginMock = nullptr;

    TEST_COUT << "PFUninitializeAsync";
    XAsyncBlock async2{};
    hr = PFUninitializeAsync(&async2);
    if (!ignoreFailures)
    {
        RIF(hr);
    }
    TEST_COUT << "PFUninitializeAsync XAsyncGetStatus";
    hr = XAsyncGetStatus(&async2, true);
    if (!ignoreFailures)
    {
        RIF(hr);
    }
    TEST_COUT << "PFUninitializeAsync done";

    return S_OK;
}

HRESULT pfgamesave_reset_cloud()
{
    TEST_COUT << "reset_cloud";
    RIF(pfgamesave_init());

    XAsyncBlock async{};
    RIF(PFGameSaveFilesResetCloudAsync(g_gameState.localUserHandle, &async));
    RIF(XAsyncGetStatus(&async, true));

    RIF(pfgamesave_shutdown());

    return S_OK;
}

HRESULT pfgamesave_get_folder(bool allowOfflineError)
{
    TEST_COUT << "PFGameSaveFilesAddUserWithUiAsync";
    XAsyncBlock async{};
    RIF(PFGameSaveFilesAddUserWithUiAsync(g_gameState.localUserHandle, PFGameSaveFilesAddUserOptions::None, &async));
    HRESULT hr = XAsyncGetStatus(&async, true); // This is doing a blocking wait for completion, but you can use the XAsyncBlock to set a callback instead for async style usage
    if (hr == E_PF_GAMESAVE_USER_CANCELLED)
    {
        // Ignore debug logging for this error code
        return hr;
    }
    TEST_COUT << "PFGameSaveFilesAddUserWithUiAsync XAsyncGetStatus";
    RIF(hr);

    bool isCloudConnected{ false };
    hr = PFGameSaveFilesIsConnectedToCloud(g_gameState.localUserHandle, &isCloudConnected);
    RIF(hr);
    TEST_COUT << "IsCloudConnected: " << (isCloudConnected ? "true" : "false");

    char saveFolder[MAX_PATH] = {};
    hr = PFGameSaveFilesGetFolder(g_gameState.localUserHandle, MAX_PATH, saveFolder, nullptr);
    if (SUCCEEDED(hr))
    {
        g_gameState.saveFolder = saveFolder;
        TEST_COUT << "SaveFolder: " << g_gameState.saveFolder;

        PFGameSaveFilesSyncState state;
        uint64_t current;
        uint64_t total;
        RIF(PFGameSaveFilesUiProgressGetProgress(g_gameState.localUserHandle, &state, &current, &total));
    }

    int64_t remainingQuota{};
    hr = PFGameSaveFilesGetRemainingQuota(g_gameState.localUserHandle, &remainingQuota);
    if (allowOfflineError)
    {
        if (hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD)
        {
            hr = S_OK;
        }
    }
    TEST_COUT << "PFGameSaveFilesGetRemainingQuota";
    RIF(hr);
    TEST_COUT << "Remaining Quota: " << remainingQuota;

    return S_OK;
}

HRESULT debug_get_stats_json(_Out_ PlayFab::JsonDocument& stats)
{
    size_t size{};
    THROW_IF_FAILED(PFGameSaveFilesGetStatsJsonSizeForDebug(g_gameState.localUserHandle, &size));

    std::vector<char> buffer(size);
    THROW_IF_FAILED(PFGameSaveFilesGetStatsJsonForDebug(g_gameState.localUserHandle, buffer.size(), buffer.data(), nullptr));

    std::string bufferString(buffer.data(), buffer.size());
    bool isEmpty = (std::all_of(bufferString.begin(), bufferString.end(), [](char c) { return c == '\0'; }));
    stats = !isEmpty ? PlayFab::JsonValue::parse(bufferString) : "";

    return S_OK;
}

HRESULT clear_folder(std::string folder)
{
    return FilePALTestApp::DeletePath(folder);
}

HRESULT debug_reset_cloud(std::string deviceName)
{
#ifdef USE_MOCKS
    RIF(clear_folder(FilePALTestApp::AppendPath(g_gameState.localRoot, deviceName)));
#else
    RIF(debug_set_device_name(deviceName));

    XAsyncBlock async{};
    RIF(PFGameSaveFilesResetCloudAsync(g_gameState.localUserHandle, &async));
    RIF(XAsyncGetStatus(&async, true));
#endif

    return S_OK;
}

HRESULT reset_all()
{
    g_gameState.uiProgressCallbackTriggered = false;
    g_gameState.uiProgressCallbackAutomatedAction = TestProgressUIChoice::ShowProgress;

    g_gameState.uiSyncFailedCallbackExpected = false;
    g_gameState.uiSyncFailedCallbackTriggered = false;
    g_gameState.uiSyncFailedCallbackAutomatedAction = TestSyncFailedUIChoice::NoChoice;

    g_gameState.uiConflictCallbackExpected = false;
    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackAutomatedAction = TestTakeUIChoice::NoChoice;

    g_gameState.uiActiveDeviceContentionCallbackExpected = false;
    g_gameState.uiActiveDeviceContentionCallbackTriggered = false;
    g_gameState.uiActiveDeviceContentionCallbackAutomatedAction = TestActiveDeviceContention::SyncLast;

    g_gameState.uiOutOfStorageCallbackExpected = false;
    g_gameState.uiOutOfStorageCallbackTriggered = false;
    g_gameState.uiOutOfStorageCallbackAutomatedAction = TestOutOfStorageUIChoice::NoChoice;

#ifndef USE_MOCKS // don't need to bother with init/shutdown here if using mocks
    RIF(pfgamesave_init());
#endif
    HCSettingsSetTraceLevel(HCTraceLevel::Information);
    RIF(debug_reset_cloud("DeviceA"));
    RIF(debug_reset_cloud("DeviceB"));
    RIF(debug_reset_cloud("DeviceC"));
    RIF(debug_reset_cloud("DeviceD"));
#ifndef USE_MOCKS
    RIF(pfgamesave_shutdown());
#endif

    RIF(clear_folder(FilePALTestApp::AppendPath(g_gameState.localRoot, "MockPFEntityFiles")));
    RIF(clear_folder(FilePALTestApp::AppendPath(g_gameState.localRoot, "MockPFGameSave")));
    return S_OK;
}

HRESULT debug_set_device_name(std::string deviceName)
{
    return PFGameSaveFilesSetMockDeviceIdForDebug(deviceName.c_str());
}

HRESULT debug_set_root_folder(std::string rootFolder)
{
    return PFGameSaveFilesSetMockRootFolderForDebug(rootFolder.c_str());
}

HRESULT debug_force_out_of_storage(bool force)
{
    return PFGameSaveFilesSetForceOutOfStorageErrorForDebug(force);
}

HRESULT debug_force_sync_fail(bool force)
{
    return PFGameSaveFilesSetForceSyncFailedErrorForDebug(force);
}

HRESULT debug_force_pause_upload()
{
    return PFGameSaveFilesPauseUploadForDebug();
}

HRESULT debug_force_offline(GameSaveServiceMockForcedOffline mode)
{
    return PFGameSaveFilesSetMockForceOfflineForDebug(mode);
}

HRESULT debug_force_resume_upload()
{
    return PFGameSaveFilesResumeUploadForDebug();
}

HRESULT debug_set_data_folder(std::string dataFolder)
{
    g_gameState.debugDataFolder = dataFolder;
    return S_OK;
}

HRESULT create_folder(std::string folderName)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    return S_OK;
}

HRESULT write_file(std::string folderName, std::string fileName, std::string data)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, fileName);
    std::vector<char> vData;
    std::copy(data.begin(), data.end(), std::back_inserter(vData));
    RIF(FilePALTestApp::WriteBinaryFile(filePath, vData));
    return S_OK;
}

HRESULT write_file_no_gamestate(std::string deviceFolderName, std::string fileName, std::string data)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.localRoot, deviceFolderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, fileName);
    std::vector<char> vData;
    std::copy(data.begin(), data.end(), std::back_inserter(vData));
    RIF(FilePALTestApp::WriteBinaryFile(filePath, vData));
    return S_OK;
}

HRESULT write_localstate(std::string deviceFolderName, std::string data)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.localRoot, deviceFolderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, "cloudsync\\localstate.json");
    std::vector<char> vData;
    std::copy(data.begin(), data.end(), std::back_inserter(vData));
    RIF(FilePALTestApp::WriteBinaryFile(filePath, vData));
    return S_OK;
}

HRESULT write_manifest(std::string data)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.localRoot, "MockPFEntityFiles");
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, "manifests.json");
    std::vector<char> vData;
    std::copy(data.begin(), data.end(), std::back_inserter(vData));
    RIF(FilePALTestApp::WriteBinaryFile(filePath, vData));
    return S_OK;
}

HRESULT write_extended_manifest(std::string deviceFolderName, std::string fileName, std::string data)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.localRoot, deviceFolderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, "cloudsync\\" + fileName);
    std::vector<char> vData;
    std::copy(data.begin(), data.end(), std::back_inserter(vData));
    RIF(FilePALTestApp::WriteBinaryFile(filePath, vData));
    return S_OK;
}

HRESULT verify_file(std::string folderName, std::string fileName, std::string data)
{
    std::string folderFullPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    std::string filePath = FilePALTestApp::AppendPath(folderFullPath, fileName);
    std::vector<char> vData = FilePALTestApp::ReadBinaryFile(filePath);
    if (vData.size() > 0)
    {
        std::string str(vData.begin(), vData.end());
        if (data == str)
        {
            return S_OK;
        }
    }
    return E_FAIL;
}

HRESULT verify_cloud_connected(bool isCloudConnected)
{
    bool isConnected = false;
    RIF(PFGameSaveFilesIsConnectedToCloud(g_gameState.localUserHandle, &isConnected));

    if (isConnected == isCloudConnected)
    {
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}

HRESULT verify_file_no_gamestate(std::string deviceFolderName, std::string fileName, std::string data)
{
    std::string folderFullPath = FilePALTestApp::AppendPath(g_gameState.localRoot, deviceFolderName);
    std::string filePath = FilePALTestApp::AppendPath(folderFullPath, fileName);
    std::vector<char> vData = FilePALTestApp::ReadBinaryFile(filePath);
    if (vData.size() > 0)
    {
        std::string str(vData.begin(), vData.end());
        if (data == str)
        {
            return S_OK;
        }
    }
    return E_FAIL;
}

HRESULT verify_num_files(std::string folderName, int numFiles)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    std::vector<std::string> files = FilePALTestApp::EnumFilesInFolder(folderPath);
    return (files.size() == numFiles) ? S_OK : E_FAIL;
}

HRESULT verify_folder_exists(std::string folderName)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    return FilePALTestApp::DoesDirectoryExist(folderPath) ? S_OK : E_FAIL;
}

HRESULT delete_file(std::string folderName, std::string fileName)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    std::string filePath = FilePALTestApp::AppendPath(folderPath, fileName);
    FilePALTestApp::DeleteLocalFile(filePath);
    return S_OK;
}

HRESULT delete_folder(std::string folderName)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    FilePALTestApp::DeletePath(folderPath);
    return S_OK;
}

HRESULT debug_get_last_write_time(_Out_ std::filesystem::file_time_type& lastWriteTime, std::string folderName, std::string fileName)
{
    std::string folderPath = FilePALTestApp::AppendPath(g_gameState.saveFolder, folderName);
    RIF(FilePALTestApp::CreatePath(folderPath));
    std::string filePath = FilePALTestApp::AppendPath(folderPath, fileName);
    std::filesystem::path path(filePath);
    lastWriteTime = std::filesystem::last_write_time(path);
    return S_OK;

}

HRESULT pfgamesave_download(
    const std::string& deviceName, 
    bool resetCloud, 
    bool forceOutOfStorageError, 
    bool forceSyncFailError, 
    bool init, 
    bool allowOfflineError)
{
#ifdef USE_PIX
    PIXBeginEvent(PIX_COLOR(0, 0, 255), "Download");
#endif

    TEST_COUT << "";
    TEST_COUT << "*** " << deviceName;
    if (init)
    {
        RIF(pfgamesave_init());
    }
    RIF(debug_set_device_name(deviceName));
    RIF(debug_set_root_folder(FilePALTestApp::AppendPath(g_gameState.localRoot, deviceName)));
    RIF(debug_force_sync_fail(forceSyncFailError));
    RIF(debug_force_out_of_storage(forceOutOfStorageError));
    RIF(pfcore_loginuser());
    if (resetCloud)
    {
        RIF(debug_reset_cloud(deviceName));
    }
    HRESULT hr = pfgamesave_get_folder(allowOfflineError);
    if (hr == E_PF_GAMESAVE_USER_CANCELLED)
    {
        // Ignore debug logging for this error code
        return hr;
    }
    TEST_COUT << "pfgamesave_get_folder";
    RIF(hr);

#ifdef USE_PIX
    PIXEndEvent();
#endif
    return S_OK;
}

void pfgamesave_syncdevice(const std::string& deviceName, ProcessDataFn processDataFn, bool upload, bool cleanup)
{
    pfgamesave_download(deviceName);
    processDataFn();
    if (upload)
    {
        AHS(pfgamesave_upload());
    }
    if (cleanup)
    {
        AHS(pfgamesave_shutdown());
    }
}

void AutomatedPFGameSaveFilesUiProgressCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesSyncState syncState, _In_ void* context)
{
    std::string status = (syncState == PFGameSaveFilesSyncState::Downloading) ? "Downloading" : "Uploading";
    TEST_COUT << "UIProgressCallback: " << status;

    g_gameState.uiProgressCallbackTriggered = true;

    switch (g_gameState.uiProgressCallbackAutomatedAction)
    {
    case TestProgressUIChoice::ShowProgress:
        // repeatedly call PFGameSaveFilesUiProgressGetProgress(entityHandle, &current, &total) and show an animated progress bar.
        break;

    case TestProgressUIChoice::Cancel:
        PFGameSaveFilesSetUiProgressResponse(localUserHandle, PFGameSaveFilesUiProgressUserAction::Cancel);
        break;
    }
}

void AutomatedPFGameSaveFilesUiSyncFailedCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesSyncState syncState, _In_ HRESULT error, _In_ void* context)
{
    TEST_COUT << "UISyncFailedCallback";

    g_gameState.uiSyncFailedCallbackTriggered = true;
    if (!g_gameState.uiSyncFailedCallbackExpected)
    {
        TEST_ERR_COUT << "Unexpected UISyncFailedCallback";
        ExitProcess(1);
    }

    switch (g_gameState.uiSyncFailedCallbackAutomatedAction)
    {
    case TestSyncFailedUIChoice::NoChoice:
        break;

    case TestSyncFailedUIChoice::Retry:
        PFGameSaveFilesSetForceSyncFailedErrorForDebug(false); // otherwise it'd loop on this error
        PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Retry);
        break;

    case TestSyncFailedUIChoice::UseOffline:
        PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::UseOffline);
        break;

    case TestSyncFailedUIChoice::Cancel:
        PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
        break;
    }
}

void AutomatedPFGameSaveFilesUiActiveDeviceContentionCallback(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveDescriptor* localGameSave, _In_ PFGameSaveDescriptor* remoteGameSave, _In_ void* context)
{
    TEST_COUT << "UIActiveDeviceContentionCallback";
    // if user chooses [Try Again], call PFGameSaveFilesUiActiveDeviceContentionRetry(entityHandle)
    // if user chooses [Sync Last Saved Data], call PFGameSaveFilesUiActiveDeviceContentionSyncLastSavedData(entityHandle)
    // if user chooses [Cancel], call PFGameSaveFilesUiActiveDeviceContentionCancel(entityHandle)

    g_gameState.uiActiveDeviceContentionCallbackTriggered = true;
    if (!g_gameState.uiActiveDeviceContentionCallbackExpected)
    {
        TEST_ERR_COUT << "Unexpected UIActiveDeviceContentionCallback";
        ExitProcess(1);
    }

    switch (g_gameState.uiActiveDeviceContentionCallbackAutomatedAction)
    {
    case TestActiveDeviceContention::NoChoice:
        break;

    case TestActiveDeviceContention::TryAgain:
        PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry);
        break;

    case TestActiveDeviceContention::SyncLast:
        PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
        break;

    case TestActiveDeviceContention::Cancel:
        PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
        break;
    }
}

void AutomatedPFGameSaveFilesUiConflictCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave, 
    _In_ PFGameSaveDescriptor* remoteGameSave, 
    _In_ void* context)
{
    TEST_COUT << "UIConflictCallback";
    g_gameState.uiConflictCallbackTriggered = true;
    if (!g_gameState.uiConflictCallbackExpected)
    {
        TEST_ERR_COUT << "Unexpected UIConflictCallback";
        ExitProcess(1);
    }

    switch (g_gameState.uiConflictCallbackAutomatedAction)
    {
    case TestTakeUIChoice::NoChoice:
        break;

    case TestTakeUIChoice::TakeLocal:
        PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeLocal);
        break;

    case TestTakeUIChoice::TakeRemote:
        PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::TakeRemote);
        break;

    case TestTakeUIChoice::Cancel:
        PFGameSaveFilesSetUiConflictResponse(localUserHandle, PFGameSaveFilesUiConflictUserAction::Cancel);
        break;
    }
}

void AutomatedPFGameSaveFilesUiOutOfStorageCallback(_In_ PFLocalUserHandle localUserHandle, _In_ uint64_t requiredBytes, _In_ void* context)
{
    TEST_COUT << "UIOutOfStorageCallback";

    g_gameState.uiOutOfStorageCallbackTriggered = true;
    if (!g_gameState.uiOutOfStorageCallbackExpected)
    {
        TEST_ERR_COUT << "Unexpected UIOutOfStorageCallback";
        ExitProcess(1);
    }

    // if user chooses [Try Again], call PFGameSaveFilesUiOutOfStorageSpaceCleared(entityHandle)
    // if user chooses [Cancel], call PFGameSaveFilesUiOutOfStorageCancel(entityHandle);

    switch (g_gameState.uiOutOfStorageCallbackAutomatedAction)
    {
    case TestOutOfStorageUIChoice::NoChoice:
        break;

    case TestOutOfStorageUIChoice::Retry:
        PFGameSaveFilesSetForceOutOfStorageErrorForDebug(false); // otherwise it'd loop on this error
        PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Retry);
        break;

    case TestOutOfStorageUIChoice::Cancel:
        PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
        break;
    }
}

HRESULT RIFHelper(HRESULT hr, const char* fn)
{
    if (FAILED(hr))
    {
        TEST_ERR_COUT << "[DEBUG FAILURE] : " << fn << std::endl;
    }
    return hr;
}

void AHSHelper(HRESULT hr, const char* fn)
{
    if (FAILED(hr))
    {
        TEST_ERR_COUT << "[DEBUG FAILURE] : " << fn << std::endl;
    }
    ASSERT_HRESULT_SUCCEEDED(hr);
}

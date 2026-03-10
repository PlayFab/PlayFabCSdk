// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include <fstream>
#include <conio.h>
#include <httpclient/httpClient.h>
#include "Sample_GameSaveUI.h"

HC_DEFINE_TRACE_AREA(Sample, HCTraceLevel::Verbose);
HC_DECLARE_TRACE_AREA(Sample);

void LaunchAndCaptureOutput(const std::string& command, std::string filterLine);

std::string GetProcessIdAsString() 
{
    DWORD pid = GetCurrentProcessId();
    std::ostringstream oss;
    oss << pid;
    return oss.str();
}

void MyDebugTrace(
    _In_z_ const char* areaName,
    _In_ HCTraceLevel level,
    _In_ uint64_t threadId,
    _In_ uint64_t timestamp,
    _In_z_ const char* message
)
{
    UNREFERENCED_PARAMETER(level);
    UNREFERENCED_PARAMETER(threadId);
    UNREFERENCED_PARAMETER(timestamp);
    LogToWindow(std::string("[") + areaName + "] " + message);
}

HRESULT Sample_GameSave_Init(Sample_GameSaveState* state)
{
    UNREFERENCED_PARAMETER(state);
    HRESULT hr = PFInitialize(nullptr);
    HC_TRACE_WARNING_HR(Sample, hr, "PFInitialize");
    RETURN_IF_FAILED(hr);

    hr = PFServicesInitialize(nullptr);
    HC_TRACE_WARNING_HR(Sample, hr, "PFServicesInitialize");
    RETURN_IF_FAILED(hr);

    HCSettingsSetTraceLevel(HCTraceLevel::Warning);
    HCTraceSetTraceToDebugger(true);
    HCTraceSetClientCallback(MyDebugTrace);

    hr = PFServiceConfigCreateHandle(
        "https://E18D7.playfabapi.com",
        "E18D7",
        &state->serviceConfigHandle);
    HC_TRACE_WARNING_HR(Sample, hr, "PFServiceConfigCreateHandle");
    RETURN_IF_FAILED(hr);

    std::string processId = GetProcessIdAsString();
    HC_TRACE_WARNING(Sample, "Process ID: %s", processId.c_str());

    PFGameSaveInitArgs args{};
    args.saveFolder = state->inputSaveFolder.c_str();
    hr = PFGameSaveFilesInitialize(&args);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesInitialize");
    RETURN_IF_FAILED(hr);

    if (state->setUICallBacks)
    {
        PFGameSaveUICallbacks callbacks{};
        callbacks.progressCallback = MyPFGameSaveFilesUiProgressCallback;
        callbacks.progressContext = state;
        callbacks.syncFailedCallback = MyPFGameSaveFilesUiSyncFailedCallback;
        callbacks.syncFailedContext = state;
        callbacks.activeDeviceContentionCallback = MyPFGameSaveFilesUiActiveDeviceContentionCallback;
        callbacks.activeDeviceContentionContext = state;
        callbacks.conflictCallback = MyPFGameSaveFilesUiConflictCallback;
        callbacks.conflictContext = state;
        callbacks.outOfStorageCallback = MyPFGameSaveFilesUiOutOfStorageCallback;
        callbacks.outOfStorageContext = state;
        hr = PFGameSaveFilesSetUiCallbacks(&callbacks);
        RETURN_IF_FAILED(hr);
    }

    hr = PFGameSaveFilesSetActiveDeviceChangedCallback(nullptr, MyPFGameSaveFilesActiveDeviceChangedCallback, state);
    RETURN_IF_FAILED(hr);


    return S_OK;
}

HRESULT MyPFLocalUserLoginHandler(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_opt_ PFEntityHandle existingEntityHandle,
    _Inout_ XAsyncBlock* async
)
{
    UNREFERENCED_PARAMETER(existingEntityHandle);
    void* customContext = nullptr;
    HRESULT hr = PFLocalUserGetCustomContext(localUserHandle, (void**)&customContext);
    RETURN_IF_FAILED(hr);
    Sample_GameSaveState* state = static_cast<Sample_GameSaveState*>(customContext);

    // Use PF Core to call PlayFab's LoginWithCustomID (or another PlayFab LoginWith* API)
    PFAuthenticationLoginWithCustomIDRequest request{};
    request.createAccount = true;
    request.customId = state->inputCustomUserId.c_str();

    return PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request, async);
}

HRESULT Sample_GameSave_SignInUser(Sample_GameSaveState* state)
{
    HRESULT hr = state->xuserResult;
    if (SUCCEEDED(hr))
    {
        uint64_t userId;
        XUserGetId(state->xuser, &userId);
        std::string xuidStr = std::to_string(userId);
        WriteXuidFile(state, xuidStr);

        if (state->idOnly)
        {
            LogToWindow("XUID: " + xuidStr);
            return S_OK;
        }

        bool signinWithXbox = true;
        if (signinWithXbox)
        {
            hr = PFLocalUserCreateHandleWithXboxUser(state->serviceConfigHandle, state->xuser, nullptr, &state->localUserHandle);
            HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserCreateHandleWithPlatformUser");
            RETURN_IF_FAILED(hr);
        }
        else
        {
            if (state->inputCustomUserId.length() == 0)
            {
                state->inputCustomUserId = "testgamesave"; // simple default 
            }

            hr = PFLocalUserCreateHandleWithPersistedLocalId(
                state->serviceConfigHandle,
                "1", // persisted local id of this user.  This simple example just uses "1"
                MyPFLocalUserLoginHandler, // this will be called to login the user as needed
                state,
                &state->localUserHandle);
            HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserCreateHandleWithPersistedLocalId");
            RETURN_IF_FAILED(hr);
        }
    }

    if (FAILED(hr))
    {
        // TODO: isn't PFLocalUserLoginAsync() needed in this path?
    }
    else
    {
        //XAsyncBlock async{};
        //RETURN_IF_FAILED(PFLocalUserLoginAsync(state->localUserHandle, true, &async));
        //RETURN_IF_FAILED(XAsyncGetStatus(&async, true));
        //size_t bufferSize{};
        //PFLocalUserLoginGetResultSize(&async, &bufferSize);
        //PFEntityHandle entityHandle{};
        //RETURN_IF_FAILED(PFLocalUserLoginGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr));

    }

    return hr;
}

HRESULT Sample_GameSave_AddUser( 
    Sample_GameSaveState* gameState,
    PFLocalUserHandle localUserHandle, 
    std::string* rootSaveFolderPath)
{
    XAsyncBlock async{};
    async.queue = gameState->taskQueue;
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(localUserHandle, PFGameSaveFilesAddUserOptions::None, &async);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesAddUserWithUiAsync");
    RETURN_IF_FAILED(hr);

    // Test PFGameSaveFilesResetCloudAsync
    //XAsyncBlock async2{};
    //hr = PFGameSaveFilesResetCloudAsync(localUserHandle, &async2);
    //HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesResetCloudAsync");
    //RETURN_IF_FAILED(hr);
    //hr = XAsyncGetStatus(&async2, true); // Simple synchronous wait, not recommended for production code
    //HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesResetCloudAsync XAsyncGetStatus");
    //RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async, true); // Simple synchronous wait, not recommended for production code

    RETURN_IF_FAILED(hr);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesAddUserWithUiResult");
    RETURN_IF_FAILED(hr);

    size_t folderSize{};
    hr = PFGameSaveFilesGetFolderSize(localUserHandle, &folderSize);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesGetFolderSize");
    RETURN_IF_FAILED(hr);
    std::vector<char> folder(folderSize);
    hr = PFGameSaveFilesGetFolder(localUserHandle, folderSize, folder.data(), nullptr);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesGetFolder");
    RETURN_IF_FAILED(hr);

    std::string folderStr(folder.data(), folderSize - 1);
    if (rootSaveFolderPath)
    {
        *rootSaveFolderPath = folderStr;
    }

    return S_OK;
}

HRESULT Sample_GameSave_Upload(PFLocalUserHandle localUserHandle, Sample_GameSaveState* state)
{
    XAsyncBlock async{};
    PFGameSaveFilesUploadOption option = PFGameSaveFilesUploadOption::ReleaseDeviceAsActive;
    if (state->keepDeviceActive)
    {
        std::cout << "KeepDeviceActive" << std::endl;
        option = PFGameSaveFilesUploadOption::KeepDeviceActive;
    }
    HRESULT hr = PFGameSaveFilesUploadWithUiAsync(localUserHandle, option, &async);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUploadWithUiAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async, true);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUploadWithUiAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    return S_OK;
}

HRESULT Sample_GameSave_Cleanup(Sample_GameSaveState* state)
{
    if (state->localUserHandle)
    {
        PFLocalUserCloseHandle(state->localUserHandle);
        state->localUserHandle = nullptr;
    }

    if (state->serviceConfigHandle)
    {
        PFServiceConfigCloseHandle(state->serviceConfigHandle);
        state->serviceConfigHandle = nullptr;
    }

    if (!state->statusFilePath.empty())
    {
        std::remove(state->statusFilePath.c_str());
    }

    XAsyncBlock async1{};
    HRESULT hr = PFGameSaveFilesUninitializeAsync(&async1);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async1, true); // Simple synchronous wait, not recommended for production code
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    hr = PFGameSaveFilesUninitializeResult(&async1);
    HC_TRACE_WARNING_HR(Sample, hr, "PFGameSaveFilesUninitializeResult");
    RETURN_IF_FAILED(hr);

    XAsyncBlock async2{};
    hr = PFServicesUninitializeAsync(&async2);
    HC_TRACE_WARNING_HR(Sample, hr, "PFServicesUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async2, true); // Simple synchronous wait, not recommended for production code
    HC_TRACE_WARNING_HR(Sample, hr, "PFServicesUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    XAsyncBlock async3{};
    hr = PFUninitializeAsync(&async3);
    HC_TRACE_WARNING_HR(Sample, hr, "PFUninitializeAsync");
    RETURN_IF_FAILED(hr);

    hr = XAsyncGetStatus(&async3, true);
    HC_TRACE_WARNING_HR(Sample, hr, "PFUninitializeAsync XAsyncGetStatus");
    RETURN_IF_FAILED(hr);

    if (state->taskQueue)
    {
        XTaskQueueCloseHandle(state->taskQueue);
        state->taskQueue = nullptr;
    }

    if (state->xuser)
    {
        XUserCloseHandle(state->xuser);
        state->xuser = nullptr;
    }

    return S_OK;
}

std::string CovertToUtf8(const wchar_t* wideStr) 
{
    if (!wideStr) return "";

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    if (sizeNeeded <= 0) return "";

    std::string utf8Str(sizeNeeded - 1, 0); // Exclude null terminator
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], sizeNeeded, nullptr, nullptr);

    return utf8Str;
}

std::vector<std::string> SplitStringBySpace(const std::string& input) 
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

bool Sample_GameSave_ParseArgs(_In_z_ _Printf_format_string_ const char* cmdLineStr, Sample_GameSaveState* gameState)
{
    std::string cmdLine = cmdLineStr;
    std::vector<std::string> argv = SplitStringBySpace(cmdLine);
    int argc = static_cast<int>(argv.size());

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i].c_str(), "/gdktestapp") == 0)
        {
            // ignore.  used by automated test app to control which app to run
        }
        else if (strcmp(argv[i].c_str(), "/v") == 0)
        {
            gameState->verboseLogs = true;
        }
        else if (strcmp(argv[i].c_str(), "/setui") == 0)
        {
            gameState->setUICallBacks = true;
        }
        else if (strcmp(argv[i].c_str(), "/p") == 0)
        {
            gameState->inputSaveFolder = argv[i + 1];
            i++;
        }        
        else if (strcmp(argv[i].c_str(), "/u") == 0)
        {
            gameState->inputCustomUserId = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/linkcustomidwithxbox") == 0)
        {
            gameState->linkCustomIdWithXbox = true;
        }
        else if (strcmp(argv[i].c_str(), "/statusfile") == 0)
        {
            gameState->statusFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/pathfile") == 0)
        {
            gameState->pathFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/xuidfile") == 0)
        {
            gameState->xuidFilePath = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/waitforackat") == 0)
        {
            gameState->waitForAckAt = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/debugbreakat") == 0)
        {
            gameState->debugBreakAt = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i].c_str(), "/idonly") == 0)
        {
            gameState->idOnly = true;
        }
        else if (strcmp(argv[i].c_str(), "/notinteractive") == 0)
        {
            gameState->interactive = false;
        }
        //else if (strcmp(argv[i].c_str(), "/downloadonly") == 0)
        //{
        //    gameState->downloadOnly = true;
        //}
        //else if (strcmp(argv[i].c_str(), "/r") == 0)
        //{
        //    gameState->resetCloud = true;
        //}
        //else if (strcmp(argv[i].c_str(), "/errorifupload") == 0)
        //{
        //    gameState->errorIfUpload = true;
        //}
        else if (strcmp(argv[i].c_str(), "/activedevicepromptsynclast") == 0)
        {
            gameState->activeDeviceContentionSyncLast = true;
        }
        else if (strcmp(argv[i].c_str(), "/keepdeviceactive") == 0)
        {
            gameState->keepDeviceActive = true;
        }
        else if (strcmp(argv[i].c_str(), "/takelocal") == 0)
        {
            gameState->takeLocal = true;
        }
        else if (strcmp(argv[i].c_str(), "/takeremote") == 0)
        {
            gameState->takeRemote = true;
        }
        //else if (strcmp(argv[i].c_str(), "/expectofflineerror") == 0)
        //{
        //    gameState->expectOfflineError = true;
        //}
        else
        {
            LogToWindow("Unknown argument: " + std::string(argv[i]));

            std::string usage =
                "Usage: game.exe [see other options below]\r\n"
                "If /v is given then show verbose logs\r\n"
                "If /p [path] is given then it will use the path as the save folder.  Only applies when using inproc client\r\n"
                "If /statusfile [file] is given then it will create the file while the device is active and delete it upon exit\r\n"
                "If /pathfile [file] is given then it will create the file containing the save game root\r\n"
                "If /xuidfile [file] is given then it will create the file containing the xuid\r\n"
                "If /waitforackat [act list] is given then it will wait for the ack at the given list\r\n"
                "If /debugbreakat [act list] is given then it will debug break at the given list\r\n"
                "If /u is given, its the custom ID to use\r\n"
                "If /linkcustomidwithxbox is given, then it will link the customId with the Xbox account\r\n";
                "If /idonly is given, then it write the xuid file and exit\r\n";
                "If /interactive is given, then it turn on interactive mode\r\n";
                "If /setui is given, then it will set the UI callbacks\r\n";
                //"If /r is given then it will reset cloud and exit\r\n"
                //"If /downloadonly is given then it will only download the data\r\n"
                "If /takeLocal is given then it will take local if there's a conflict\r\n"
                "If /takeRemote is given then it will take remote if there's a conflict\r\n"
                //"If /errorifupload is given then process will error if it upload happened\r\n"
                "If /keepdeviceactive is given then it will keep the device active when in /ioff mode\r\n";
                //"If /expectofflineerror is given then it will expect an offline error\r\n";

                LogToWindow(usage);

            return false;
        }
    }

    return true;
}

HRESULT Sample_GameSave_Update(Sample_GameSaveState* state)
{
    HRESULT hr;
    switch (state->currentState)
    {
        case SampleGameSaveState::Init:
        {
            hr = Sample_GameSave_Init(state);
            RETURN_IF_FAILED(hr);

            state->currentState = SampleGameSaveState::SignInXboxStartAsync;
            break;
        }

        case SampleGameSaveState::SignInXboxStartAsync:
        {
            Sample_Xbox_AttemptSignIn();
            state->currentState = SampleGameSaveState::SignInXboxStartAsyncWait;
            break;
        }

        case SampleGameSaveState::SignInXboxStartAsyncWait:
        {
            break;
        }

        case SampleGameSaveState::SignIn:
        {
            hr = Sample_GameSave_SignInUser(state);
            RETURN_IF_FAILED(hr);

            if (state->idOnly)
            {
                state->quit = true;
                return S_OK;
            }

            if (state->linkCustomIdWithXbox)
            {
                XAsyncBlock xasync{};
                hr = PFLocalUserLoginAsync(state->localUserHandle, true, &xasync);
                HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserLoginAsync");
                RETURN_IF_FAILED(hr);

                hr = XAsyncGetStatus(&xasync, true);
                HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserLoginAsync XAsyncGetStatus");
                RETURN_IF_FAILED(hr);

                bool forceLink = true;
                PFAccountManagementLinkCustomIDRequest request{};
                request.forceLink = &forceLink;
                request.customId = state->inputCustomUserId.c_str();

                PFEntityHandle entityHandle{};
                hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
                HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserTryGetEntityHandle");
                RETURN_IF_FAILED(hr);
                if (entityHandle != nullptr)
                {
                    XAsyncBlock async{};
                    hr = PFAccountManagementClientLinkCustomIDAsync(entityHandle, &request, &async);
                    HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserTryGetEntityHandle");
                    RETURN_IF_FAILED(hr);
                    hr = XAsyncGetStatus(&async, true);
                    HC_TRACE_WARNING_HR(Sample, hr, "PFLocalUserTryGetEntityHandle XAsyncGetStatus");
                    RETURN_IF_FAILED(hr);
                }

                state->currentState = SampleGameSaveState::Quit;
            }
            else if (state->resetCloud)
            {
                state->currentState = SampleGameSaveState::ResetCloud;
            }
            else
            {
                state->currentState = SampleGameSaveState::AddUserToGameSave;
            }
            break;
        }

        case SampleGameSaveState::AddUserToGameSave:
        {
            WriteStatusFile(state, "Downloading");

            std::string rootSaveFolderPath;
            hr = Sample_GameSave_AddUser(state, state->localUserHandle, &rootSaveFolderPath);
            RETURN_IF_FAILED(hr);

            state->saveFolder = rootSaveFolderPath;
            WritePathFile(state, rootSaveFolderPath);
            Sleep(1000);

            WriteStatusFile(state, "Downloaded");

            if (state->interactive)
            {
                state->currentState = SampleGameSaveState::UploadDisplay;
            }
            else
            {
                state->currentState = SampleGameSaveState::Upload;
            }
            break;
        }

        case SampleGameSaveState::UploadDisplay:
        {
            //LogToWindow("Status:");
            //LaunchAndCaptureOutput("C:\\git\\PlayFab.C\\Utilities\\GameSave\\gamesaveenum.exe", "E18D7");
            //LogToWindow("");
            //LogToWindow("");
            LogToWindow("Folder:");
            LogToWindow(state->saveFolder);
            LogToWindow("");
            //LogToWindow("Press 'u' to upload");
            LogToWindow("Press 'o' to open folder in explorer via ShellExecuteA");
            LogToWindow("Press 'q' to quit [which auto uploads after process exit]");
            state->currentState = SampleGameSaveState::Upload;
            break;
        }

        case SampleGameSaveState::Upload:
        {
            if (!state->interactive)
            {
                if (!state->downloadOnly)
                {
                    WriteStatusFile(state, "Preupload");
                    WriteStatusFile(state, "Uploading");

                    hr = Sample_GameSave_Upload(state->localUserHandle, state);
                    RETURN_IF_FAILED(hr);

                    WriteStatusFile(state, "Uploaded");
                }

                state->currentState = SampleGameSaveState::Quit;
            }
            break;
        }

        case SampleGameSaveState::TestLink:
        {
            // Not proper calling patterns, 
            // just to test the linking of the GDK PFGameSave APIs during compile
            hr = TestInlineClass::TestGameSaveAPIAddUser(state->xuser);
            RETURN_IF_FAILED(hr);

            hr = TestInlineClass::TestLinkGDKGameSaveAPI();
            RETURN_IF_FAILED(hr);
            break;
        }

        case SampleGameSaveState::ResetCloud:
        {
            // TODO: add when we have a reset API in XGameRuntime (or when linking to inproc)
            break;
        }

        case SampleGameSaveState::Quit:
        {
            state->quit = true;
            break;
        }
    }

    return S_OK;
}

void WritePathFile(Sample_GameSaveState* state, const std::string& path)
{
    if (state->pathFilePath.empty())
    {
        return;
    }

    std::ofstream pathFile(state->pathFilePath);
    if (pathFile.is_open())
    {
        pathFile << path << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << state->pathFilePath << std::endl;
    }
    pathFile.close();
}

void WriteXuidFile(Sample_GameSaveState* state, const std::string& xuid)
{
    if (state->xuidFilePath.empty())
    {
        return;
    }

    std::ofstream file(state->xuidFilePath);
    if (file.is_open())
    {
        file << xuid;
    }
    else
    {
        std::cerr << "Unable to open file: " << state->xuidFilePath << std::endl;
    }
    file.close();
}

void WriteStatusFile(Sample_GameSaveState* state, const std::string& content)
{
    if (state->statusFilePath.empty())
    {
        return;
    }

    HC_TRACE_WARNING(Sample, "Write Status: %s.  Waiting for ACK", content.c_str());
    std::ofstream statusFile(state->statusFilePath);
    if (statusFile.is_open())
    {
        statusFile << content;
    }
    else
    {
        std::cerr << "Unable to open file: " << state->statusFilePath << std::endl;
    }
    statusFile.close();

    if (state->debugBreakAt.find(content) != std::string::npos)
    {
        LogToWindow("Debug break at: " + content);
        RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL); // Forces the attach debugger dialog
    }

    WaitForAck(state, content);
    HC_TRACE_WARNING(Sample, "Got ACK: %s", content.c_str());
}

std::string ReadStatusFile(Sample_GameSaveState* state)
{
    if (state->statusFilePath.empty())
    {
        return "";
    }

    std::ifstream file(state->statusFilePath); 
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << state->statusFilePath << std::endl;
        return "";
    }
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return contents;
}

void WaitForAck(Sample_GameSaveState* state, const std::string& content)
{
    if (state->statusFilePath.empty())
    {
        return;
    }

    if (state->waitForAckAt.find(content) != std::string::npos)
    {
        int count = 0;
        LogToWindow("At " + content + ". Waiting for ACK");
        while (count < 100) // 10 seconds
        {
            std::string ackContent = ReadStatusFile(state);
            if (ackContent == "ACK")
            {
                break;
            }
            Sleep(100);
            count++;
            
            // Only log every 10 iterations (every second) to reduce log spam
            if (count % 10 == 0)
            {
                LogToWindow("Still waiting for ACK... (" + std::to_string(count/10) + "s)");
            }
        }
        
        if (count >= 100)
        {
            LogToWindow("Timeout waiting for ACK after 10 seconds");
        }
    }
}

void LaunchAndCaptureOutput(const std::string& command, std::string filterLine) 
{
    SECURITY_ATTRIBUTES saAttr{};
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;  // Allow inheritance for handles
    saAttr.lpSecurityDescriptor = nullptr;

    // Create a pipe for the child process's STDOUT
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) 
    {
        std::cerr << "Failed to create pipe." << std::endl;
        return;
    }

    // Ensure the read handle to the pipe is not inherited
    if (!SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0)) 
    {
        std::cerr << "Failed to set handle information." << std::endl;
        return;
    }

    // Set up STARTUPINFO to redirect stdout
    STARTUPINFOA si{};
    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;  // Redirect stderr as well
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi{};
    std::string cmdLine = "cmd.exe /C " + command;

    // Create the child process
    if (!CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr,
        nullptr,
        TRUE,  // Inherit handles
        0,
        nullptr,
        nullptr,
        &si,
        &pi)) {
        std::cerr << "Failed to create process." << std::endl;
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        return;
    }

    // Close the write end of the pipe in the parent process
    CloseHandle(hWritePipe);

    // Read output from the child process
    std::string fullOutput;
    char buffer[4096];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr))
    {
        if (bytesRead == 0) break;
        buffer[bytesRead] = '\0';
        fullOutput += buffer;
    }

    std::vector<std::string> outputLines;
    std::istringstream stream(fullOutput);
    std::string lineIter;
    while (std::getline(stream, lineIter))
    {
        outputLines.push_back(lineIter);
    }

    for (auto& line : outputLines)
    {
        if (filterLine.empty() || strstr(line.c_str(), filterLine.c_str()) != nullptr)
        {
            LogToWindow(line);
        }
    }

    // Cleanup
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
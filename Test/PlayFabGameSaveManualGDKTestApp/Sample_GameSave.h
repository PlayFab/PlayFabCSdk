// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"

enum SampleGameSaveState
{
    Init,
    SignInXboxStartAsync,
    SignInXboxStartAsyncWait,
    SignIn,
    AddUserToGameSave,
    UploadDisplay,
    Upload,
    Cleanup,
    TestLink,
    ResetCloud,
    Quit
};

struct Sample_GameSaveState
{
    PFServiceConfigHandle serviceConfigHandle{ nullptr };
    PFLocalUserHandle localUserHandle{ nullptr };
    XTaskQueueHandle taskQueue{ nullptr };
    SampleGameSaveState currentState{ Init };
    XUserHandle xuser{ nullptr };
    HRESULT xuserResult{ S_OK };
    bool quit{ false };

    std::string inputDeviceId;
    std::string inputSaveFolder;
    std::string saveFolder;
    std::string debugBreakAt;
    std::string waitForAckAt;
    std::string statusFilePath;
    std::string pathFilePath;
    std::string xuidFilePath;
    std::string inputCustomUserId;
    bool verboseLogs{ false };
    bool downloadOnly{ false };
    bool linkCustomIdWithXbox{ false };
    bool idOnly{ false };
    bool interactive{ true };
    bool setUICallBacks{ false };

    bool resetCloud{ false };
    bool errorIfUpload{ false };
    bool activeDeviceContentionSyncLast{ false };
    bool takeLocal{ false };
    bool takeRemote{ false };
    //bool expectOfflineError{ false };
    bool keepDeviceActive{ false };
    bool gotConflict{ false };
    bool gotActiveDeviceChangeNotification{ false };
    PFGameSaveDescriptor savedLocalGameSave{};
    PFGameSaveDescriptor savedRemoteGameSave{};
};

// GameSave.cpp
HRESULT Sample_GameSave_Init(Sample_GameSaveState* state);
HRESULT Sample_GameSave_SignInUser(PFServiceConfigHandle serviceConfigHandle, PFLocalUserHandle* localUserHandle);
HRESULT Sample_GameSave_AddUser(PFLocalUserHandle localUserHandle, std::string* rootSaveFolderPath);
HRESULT Sample_GameSave_Upload(PFLocalUserHandle localUserHandle);
HRESULT Sample_GameSave_Cleanup(Sample_GameSaveState* state);
bool Sample_GameSave_ParseArgs(_In_z_ _Printf_format_string_ const char* cmdLine, Sample_GameSaveState* gameSaveState);
HRESULT Sample_GameSave_Update(Sample_GameSaveState* sampleGameSaveState);

void WritePathFile(Sample_GameSaveState* state, const std::string& path);
void WriteXuidFile(Sample_GameSaveState* state, const std::string& xuid);
void WriteStatusFile(Sample_GameSaveState* state, const std::string& content);
std::string ReadStatusFile(Sample_GameSaveState* state);
void WaitForAck(Sample_GameSaveState* state, const std::string& content);

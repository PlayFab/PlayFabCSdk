// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "pch.h"
#include "DeviceWebSocketClient.h"
#include "HttpMock.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <playfab/gamesave/PFGameSaveFilesUi.h>

struct ChaosModeConfiguration
{
    bool fileCreate{ true };
    bool fileModify{ true };
    bool fileDelete{ true };
    bool folderCreate{ true };
    bool folderDelete{ true };
    bool binaryFiles{ true };
    bool textFiles{ true };
    bool unicodeFiles{ true };
    bool unicodeFolders{ true };
    bool largeFiles{ true };
    uint32_t operationsPerUpload{ 5 };
    uint32_t numUploads{ 1 };
};

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

enum class DeviceEngineType
{
    Unknown,
    PcGrts,
    PcInproc,
    PcInprocGameSaves,
    Xbox,
    Psx
};

struct DeviceGameSaveState
{
    PFServiceConfigHandle serviceConfigHandle{ nullptr };
    PFLocalUserHandle localUserHandle{ nullptr };
    XTaskQueueHandle taskQueue{ nullptr };
    bool runtimeInitialized{ false };
    bool xblInitialized{ false };
#ifdef _WIN32
    XblContextHandle xblContext{ nullptr };
    XUserPlatformOperation pendingSpopOperation{ nullptr };
    XUserHandle xuser{ nullptr };
    XTaskQueueRegistrationToken inviteEventToken{};
    XTaskQueueRegistrationToken pendingInviteEventToken{};
#endif
    std::atomic<bool> inviteEventReceived{ false };
    std::atomic<bool> pendingInviteEventReceived{ false };
    std::string lastInviteUri;
    std::string lastPendingInviteUri;
    bool pfInitialized{ false };
    bool pfServicesInitialized{ false };
    bool pfGameSaveInitialized{ false };
    bool taskQueueOwnedByCommand{ false };
    std::string serviceConfigEndpoint;
    std::string serviceConfigTitleId;
    SampleGameSaveState currentState{ Init };
    HRESULT xuserResult{ S_OK };
    bool quit{ false };
    bool waitingForUserInput{ false };
    bool userInputReceived{ false };
    int lastUserInputKey{ 0 };

    std::string inputDeviceId;
    std::string inputSaveFolder;
    std::string saveFolder;
    std::string debugBreakAt;
    std::string waitForAckAt;
    std::string statusFilePath;
    std::string pathFilePath;
    std::string xuidFilePath;
    std::string inputCustomUserId;
    std::string controllerIpAddress{ "localhost" };
    std::string persistedLocalId;
    bool verboseLogs{ false };
    bool downloadOnly{ false };
    bool linkCustomIdWithXbox{ false };
    bool idOnly{ false };
    bool interactive{ true };
    bool setUICallBacks{ false };

    bool createAccountIfMissing{ true };

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
    std::string contentionDescription;  // Description captured from contention callback for GRTS testing
    std::string currentScenarioId;
    std::string currentScenarioName;

    std::optional<PFGameSaveFilesUiSyncFailedUserAction> autoSyncFailedResponse{ PFGameSaveFilesUiSyncFailedUserAction::Cancel };
    int autoSyncFailedMaxRetries{ -1 };  // -1 means unlimited retries
    int autoSyncFailedRetryCount{ 0 };
    int autoSyncFailedDelayMs{ 0 };  // Delay in milliseconds before auto-responding (for testing UI wait bugs)
    std::optional<PFGameSaveFilesUiActiveDeviceContentionUserAction> autoActiveDeviceContentionResponse{ PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData };
    std::optional<PFGameSaveFilesUiConflictUserAction> autoConflictResponse{ PFGameSaveFilesUiConflictUserAction::Cancel };
    std::optional<PFGameSaveFilesUiOutOfStorageUserAction> autoOutOfStorageResponse{ PFGameSaveFilesUiOutOfStorageUserAction::Cancel };
    std::optional<PFGameSaveFilesUiProgressUserAction> autoProgressResponse{ };

    DeviceWebSocketClient websocketClient{};
    std::atomic<bool> websocketConnectInProgress{ false };
    std::chrono::steady_clock::time_point websocketLastAttempt{};
    HRESULT websocketLastConnectError{ S_OK };
    bool websocketFirstConnectAttempt{ true };
    DeviceEngineType engineType{ DeviceEngineType::PcGrts };
    ChaosModeConfiguration chaosConfig{};
    std::vector<std::shared_ptr<HttpMock>> httpMocks{};

    // For fire-and-forget upload mode - stores the pending async block
    std::unique_ptr<XAsyncBlock> pendingUploadAsync{ nullptr };
};

DeviceGameSaveState* GetSampleGameSaveState();
void SetSampleDeviceEngineType(DeviceEngineType engineType);
DeviceEngineType DetectSampleDeviceEngineType();


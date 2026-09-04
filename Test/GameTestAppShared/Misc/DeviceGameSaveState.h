// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "pch.h"
#include "DeviceWebSocketClient.h"
#include "HttpMock.h"

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <nlohmann/json.hpp>

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
    // Multi-user support: up to 4 users indexed by userIndex/localUserIndex parameters
    static constexpr int kMaxUsers = 4;

    // Destroying a joinable std::thread calls std::terminate. The progress sampler is normally
    // stopped by PFGameSaveFilesStopProgressSampler, but a scenario that exits before that command
    // runs would otherwise crash the device app at shutdown (this type is a file-scope static, so
    // its destructor runs at process exit).
    ~DeviceGameSaveState()
    {
        progressSamplerRunning.store(false, std::memory_order_release);
        if (progressSamplerThread.joinable())
        {
            progressSamplerThread.join();
        }
    }

    PFServiceConfigHandle serviceConfigHandle{ nullptr };
    PFLocalUserHandle localUserHandle{ nullptr };            // Primary user (index 0)
    PFLocalUserHandle localUserHandles[kMaxUsers - 1]{};     // Additional users (indices 1-3)
    PFEntityHandle entityHandle{ nullptr }; // Stored from PFAuthenticationLoginWithCustomIDAsync
    std::string entityId;   // Entity ID from login (title_player_account)
    std::string entityType; // Entity type from login
    XTaskQueueHandle taskQueue{ nullptr };
    bool runtimeInitialized{ false };
    bool xblInitialized{ false };
#ifdef _WIN32
    XblContextHandle xblContext{ nullptr };
    XUserPlatformOperation pendingSpopOperation{ nullptr };
    XUserHandle xuser{ nullptr };                            // Primary user (index 0)
    XUserHandle xusers[kMaxUsers - 1]{};                     // Additional users (indices 1-3)
    XUserHandle keepAliveUser{ nullptr };                    // Pre-flight handle kept open for app lifetime so GDK silent default-user resolution stays warm (closing all handles makes subsequent silent XUserAddAsync time out)
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
    std::string statusFilePath;
    std::string inputCustomUserId;
    std::string controllerIpAddress{ "localhost" };
    std::string persistedLocalId;
    bool verboseLogs{ false };
    bool interactive{ true };
    bool forceInproc{ false };

    bool createAccountIfMissing{ true };

    PFGameSaveDescriptor savedLocalGameSave{};
    PFGameSaveDescriptor savedRemoteGameSave{};
    bool hasContentionDescriptor{ false };  // True after contention callback captures descriptors
    bool hasConflictDescriptor{ false };    // True after conflict callback captures descriptors
    PFGameSaveDescriptor conflictLocalGameSave{};
    PFGameSaveDescriptor conflictRemoteGameSave{};
    std::string contentionDescription;  // Description captured from contention callback for GRTS testing
    std::map<std::string, int64_t> quotaRecordings;  // Named quota recordings for VerifyQuotaDelta

    // Mutex protecting progress-recording and auto-write-on-upload fields that are
    // accessed from both the UI progress callback (background thread) and the main
    // command-handler thread.  Lock order: always acquire this before any other lock.
    std::mutex progressMutex;

    std::atomic<int> activeDeviceChangedCallbackCount{ 0 };  // Number of times ActiveDeviceChangedCallback has fired
    std::atomic<bool> deviceReleased{ false };  // Set after successful upload with ReleaseDeviceAsActive; prevents auto-responders from calling SDK APIs on released context
    // Guards deviceReleased together with the SDK calls that depend on it. Checking the atomic
    // and then calling into the SDK is not atomic on its own: a release can land in between and
    // the resulting GRTS fail-fast (__fastfail) cannot be caught, so it surfaces as an
    // unattributable device-app crash. Writers that SET the flag and readers that gate an SDK
    // call on it must hold this lock for the whole check-then-call.
    std::mutex deviceReleaseMutex;
    std::string currentScenarioId;
    std::string currentScenarioName;
    std::string lastGroupId;   // Group entity ID from last CreateGroup result
    std::string lastGroupType; // Group entity type from last CreateGroup result

    std::optional<PFGameSaveFilesUiSyncFailedUserAction> autoSyncFailedResponse{ PFGameSaveFilesUiSyncFailedUserAction::Cancel };
    int autoSyncFailedMaxRetries{ -1 };  // -1 means unlimited retries
    int autoSyncFailedRetryCount{ 0 };
    int autoSyncFailedDelayMs{ 0 };  // Delay in milliseconds before auto-responding (for testing UI wait bugs)
    std::optional<PFGameSaveFilesUiSyncFailedUserAction> autoSyncFailedFallbackAction{};  // action after maxRetries exhausted (default: Cancel)
    std::optional<PFGameSaveFilesUiActiveDeviceContentionUserAction> autoActiveDeviceContentionResponse{ PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData };
    int autoContentionMaxRetries{ -1 };  // -1 means unlimited retries
    int autoContentionRetryCount{ 0 };
    std::optional<PFGameSaveFilesUiActiveDeviceContentionUserAction> autoContentionFallbackAction{};  // action after maxRetries exhausted
    std::optional<PFGameSaveFilesUiConflictUserAction> autoConflictResponse{ PFGameSaveFilesUiConflictUserAction::TakeRemote };
    std::optional<PFGameSaveFilesUiOutOfStorageUserAction> autoOutOfStorageResponse{ PFGameSaveFilesUiOutOfStorageUserAction::Cancel };
    std::optional<PFGameSaveFilesUiProgressUserAction> autoProgressResponse{ };
    std::optional<std::string> autoProgressWriteOnUploadJson{};  // serialized JSON for operations to execute when Uploading detected
    bool autoProgressWriteOnUploadFired{ false };  // only fire once per upload
    bool recordProgressStates{ false };  // enable recording of sync state transitions
    struct ProgressStateEntry { PFGameSaveFilesSyncState state; uint64_t currentBytes; uint64_t totalBytes; uint64_t elapsedMs{ 0 }; };
    std::vector<ProgressStateEntry> recordedProgressStates{};

    // Background progress sampler. PFGameSaveFilesUiProgressCallback only fires once per sync-state
    // transition, so a title driving a progress bar must poll PFGameSaveFilesUiProgressGetProgress.
    // These fields let a scenario capture that polled time series and assert the values a real
    // progress bar would render. polledProgressSamples is guarded by progressMutex.
    std::vector<ProgressStateEntry> polledProgressSamples{};
    std::thread progressSamplerThread{};
    std::atomic<bool> progressSamplerRunning{ false };
    // Captured from the UI progress callback. The first sync happens inside AddUserWithUiAsync,
    // before localUserHandle has been published, so the sampler cannot rely on that field alone.
    // The sampler thread reads ONLY this atomic: localUserHandle is a plain pointer written by
    // command handlers on the main thread, so reading it from the sampler would be a data race.
    std::atomic<PFLocalUserHandle> progressSamplerUserHandle{ nullptr };
    // Time base so samples carry an elapsed offset, letting a test plot progress against real time
    // rather than sample order (a stalled transfer must look flat, not evenly paced).
    std::chrono::steady_clock::time_point progressSamplerStart{};
    std::atomic<uint64_t> lastOutOfStorageRequiredBytes{ 0 };  // Captured from out-of-storage callback
    std::atomic<bool> outOfStorageCallbackFired{ false };

    DeviceWebSocketClient websocketClient{};
    std::atomic<bool> websocketConnectInProgress{ false };
    std::chrono::steady_clock::time_point websocketLastAttempt{};
    HRESULT websocketLastConnectError{ S_OK };
    bool websocketFirstConnectAttempt{ true };
    std::atomic<bool> forceWebsocketReconnect{ false };
    DeviceEngineType engineType{ DeviceEngineType::PcGrts };
    ChaosModeConfiguration chaosConfig{};
    std::vector<std::shared_ptr<HttpMock>> httpMocks{};

    // For fire-and-forget upload mode - stores the pending async block
    std::unique_ptr<XAsyncBlock> pendingUploadAsync{ nullptr };

    // Custom memory hook counters (set via HCMemSetFunctions at startup)
    std::atomic<uint64_t> hcMemAllocCount{ 0 };
    std::atomic<uint64_t> hcMemFreeCount{ 0 };

    // libHttpClient handle storage for command handlers
    HCCallHandle hcCall{ nullptr };
    HCWebsocketHandle hcWebSocket{ nullptr };
    HCMockCallHandle hcMockCall{ nullptr };

    // Records the WebSocket close event delivered by libHttpClient. Without this the only evidence
    // a socket was torn down is that a later send fails, which cannot distinguish "closed by
    // suspend" from "never connected". Reset by HCWebSocketCreate.
    std::atomic<bool> hcWebSocketCloseEventFired{ false };
    std::atomic<int32_t> hcWebSocketCloseStatus{ 0 };

    // In-flight HCHttpCallPerformAsync started by HCHttpCallPerformStart and awaited later by
    // HCHttpCallPerformWait. Heap-allocated so the XAsyncBlock outlives the command handler that
    // started it (the request stays in flight across subsequent commands, e.g. a PLM suspend).
    std::unique_ptr<XAsyncBlock> hcPendingPerform;

    // Shared tallies for a burst of concurrent HTTP requests. Held by shared_ptr so a completion
    // callback that outlives its handler still has somewhere valid to record its result.
    struct HttpBurstState
    {
        std::atomic<int32_t> outstanding{ 0 };
        std::atomic<int32_t> succeeded{ 0 };
        std::atomic<int32_t> failed{ 0 };
        std::atomic<uint32_t> firstFailure{ 0 };

        // Counted separately from `failed` because libHttpClient reports transport-level outcomes
        // through the network error code while the XAsync result stays S_OK. A request abandoned
        // from the pending queue during suspend completes with XAsync S_OK and a network error of
        // E_ABORT, so without this the abandon path is indistinguishable from a normal success.
        std::atomic<int32_t> aborted{ 0 };
        std::atomic<int32_t> networkErrors{ 0 };
    };

    // One request in a burst. The owner (handler or DeviceGameSaveState) keeps these alive; the
    // completion callback deliberately does NOT free its own, so a timeout path can still safely
    // inspect and cancel the XAsyncBlock of a request that never completed.
    struct HttpBurstCallContext
    {
        std::shared_ptr<HttpBurstState> burst;
        HCCallHandle call{ nullptr };
        XAsyncBlock async{};
        std::atomic<bool> completed{ false };
    };

    struct PendingHttpBurst
    {
        std::shared_ptr<HttpBurstState> burst;
        std::vector<std::unique_ptr<HttpBurstCallContext>> contexts;
    };

    // A burst started by TestHCHttpCallPerformBurstStart and awaited later by
    // TestHCHttpCallPerformBurstWait. Kept on the device state, not the handler stack, so the
    // requests stay genuinely in flight across intervening commands - which is the only way to be
    // mid-burst when a PLM suspend arrives.
    std::unique_ptr<PendingHttpBurst> hcPendingBurst;

    int32_t hcCallRoutedHandlerId{ 0 };
    int32_t hcWebSocketRoutedHandlerId{ 0 };

    // When true, the app's PLM suspend handler performs a blocking
    // XTaskQueueTerminate(taskQueue, wait=true) before letting suspend complete. This models the
    // PlayFabMultiplayer PubSub/WebRequestManager teardown that deadlocks Forza (bug 63050439):
    // a synchronous suspend-time wait on a task queue whose ports XAsync has already suspended.
    // Armed by the ArmSuspendQueueTerminate command.
    bool terminateQueueOnSuspend{ false };

    // XGameSave handle storage for command handlers
    struct XGameSaveProvider* gameSaveProvider{ nullptr };
    struct XGameSaveContainer* gameSaveContainer{ nullptr };
    struct XGameSaveUpdate* gameSaveUpdate{ nullptr };
};

DeviceGameSaveState* GetSampleGameSaveState();
void SetSampleDeviceEngineType(DeviceEngineType engineType);
DeviceEngineType DetectSampleDeviceEngineType();

// Multi-user accessors: index 0 returns the primary slot, 1+ returns from the arrays
inline PFLocalUserHandle& GetLocalUserHandle(DeviceGameSaveState* state, int index)
{
    if (index <= 0) return state->localUserHandle;
    return state->localUserHandles[index - 1];
}

#ifdef _WIN32
inline XUserHandle& GetXUserHandle(DeviceGameSaveState* state, int index)
{
    if (index <= 0) return state->xuser;
    return state->xusers[index - 1];
}
#endif

// Parse an integer index parameter from JSON (default 0)
inline int ParseIndexParam(const nlohmann::json& parameters, const char* key)
{
    if (parameters.is_object() && parameters.contains(key))
    {
        const auto& node = parameters[key];
        if (node.is_number()) return node.get<int>();
        if (node.is_string()) return std::stoi(node.get<std::string>());
    }
    return 0;
}


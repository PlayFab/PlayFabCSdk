// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "EnumTraits.h"

namespace PlayFab
{
namespace GameSave
{

enum SyncState
{
    SS_NotStarted,
    SS_Preparing,
    SS_Syncing,
    SS_Error,
    SS_Cancel,
    SS_Complete,
    SS_RollbackToLKG,
    SS_RollbackToConflict //NOTE: See below where we have to define a specialization to define the max value for this enum (needed due to Clang compiler error).  If you add more values to this enum, you must update that specialization.
};

enum ManifestState
{
    MS_Initialized = 0, // Manifest has been initialized with PlayFab
    MS_Uploading,       // Manifest data is being uploaded
    MS_Finalized,       // Manifest data has been uploaded and committed
    MS_PendingDeletion  // Manifest data is being deleted.  //NOTE: See below where we have to define a specialization to define the max value for this enum (needed due to Clang compiler error).  If you add more values to this enum, you must update that specialization.
};

enum ConflictResolution
{
    CR_NoConflictsExpected = 0, // There was no conflict to resolve
    CR_KeepLocal,               // Resolve by preferring local data over remote data
    CR_TakeRemote,              // Resolve by preferring remote data over local data
    CR_SelectVersion,           // Resolve by showing available version
    CR_NoResolutionChosen       // User canceled without resolution.  //NOTE: See below where we have to define a specialization to define the max value for this enum (needed due to Clang compiler error).  If you add more values to this enum, you must update that specialization.
};

enum SyncErrorSource
{
    SER_ListManifests,
    SER_GetManifest,
    SER_DownloadData,
    SER_RegisterUpload,
    SER_UploadData,
    SER_FinalizeUpload //NOTE: See below where we have to define a specialization to define the max value for this enum (needed due to Clang compiler error).  If you add more values to this enum, you must update that specialization.
};

enum DeleteType
{
    DT_Local,
    DT_Version,
    DT_All, //NOTE: See below where we have to define a specialization to define the max value for this enum (needed due to Clang compiler error).  If you add more values to this enum, you must update that specialization.
};

enum StateMachineLocation : uint32_t
{
    Unknown,

    // LockStep
    Login,
    ListManifests,
    SelectBaselineAndCheckContention,
    CreatePendingManifest,
    WaitForActiveDeviceContentionUI,
    WaitForFailedUI_Login,
    WaitForFailedUI_ListManifests,
    WaitForFailedUI_CreatePendingManifest,
    LockStepFailure,
    LockDone,

    // CompareStep
    GetManifestDownloadDetails,
    GetExtendedManifest,
    ReadLocalManifest,
    WaitForConflictUI,
    WaitForFailedUI_GetManifestDownloadDetails,
    WaitForFailedUI_GetExtendedManifest,
    CompareStepFailure,
    CompareDone
};

struct ContextActivationEvent
{
    ContextActivationEvent() = default;

    // required uint64 totalSizeBytes: How many bytes in the context
    uint64_t totalSizeBytes{};
    // required uint64 compressedSizeBytes: What is the storage size of the context
    uint64_t compressedSizeBytes{};
    // required ManifestState manifestState: State of the manifest, will be MS_Initialized if new or MS_Finalized if looking at an old version
    ManifestState manifestState{ MS_Finalized };
    // required SyncState syncState: Final SyncState, offline will be SS_NotStarted
    SyncState syncState{ SS_Preparing };
    // required ConflictResolution conflictResolution: Resolution if needed
    ConflictResolution conflictResolution{};
    // required bool canceled: Did user cancel the sync
    bool canceled{};
    // required wstring contextVersion: version being activated
    String contextVersion{};
    // optional wstring conflictVersion: version of the manifest with conflict
    String conflictVersion{};
    // required wstring baseVersion: current finalized version in the cloud
    String baseVersion{};
    // required uint32 hresult : HRESULT of operation
    HRESULT hresult{};

    JsonValue ToJson() const;
};

struct ContextActivationFailureEvent
{
    ContextActivationFailureEvent() = default;
    
    // required bool canceled : Did the user cancel the activation
    bool canceled{};
    // required bool aborted : Did the game abort the activation
    bool aborted{};
    // required bool retryAllowed : Can the game or user retry on this failure
    bool retryAllowed{ true };
    // required bool startedOnline : Was the system online when activation started
    bool startedOnline{ true };
    // required SyncState syncState : At what Sync point did we fail
    SyncState syncState{ SS_Preparing };
    // required uint32 callingLocation : what part of the state machine caused the failure
    uint32_t callingLocation{};
    // required uint32 httpStatus : HTTP Status of any service interactions
    uint32_t httpStatus{};
    // required uint32 hresult : HRESULT of operation
    HRESULT hresult{};
    // required wstring cv : Service CorrelationVector
    String correlationVector{};

    JsonValue ToJson() const;
};

struct ContextDeleteEvent
{
    ContextDeleteEvent() = default;

    // required uint32 elapsedMs : Elapsed milliseconds since the start of the operation
    uint32_t ElapsedMs() const { return static_cast<uint32_t>(GetMilliTicks() - startTime); }
    // required uint64 totalSizeBytes : total size of the data being discarded
    uint64_t totalSizeBytes{};
    // required uint32 hresult : HRESULT of the operation
    HRESULT hresult{};
    // required DeleteType type : type of removal
    DeleteType deleteType{};
    // optional wstring contextVersion : manifest version being deleted
    String contextVersion{};
    // optional wstring cv : Service CorrelationVector
    String correlationVector{};

    int64_t startTime{};

    JsonValue ToJson() const;
};

struct ContextSyncEvent
{
    ContextSyncEvent() = default;

    // required uint32 blockCount : Payload Blocks Synced
    uint32_t blockCount{};
    // required uint32 fileCount : Files inside Payload Blocks
    uint32_t fileCount{};
    // required uint64 syncSizeBytes : Compressed Size of Payload
    uint64_t syncSizeBytes{};
    // required uint64 originalSizeBytes : Original Size of Payload
    uint64_t originalSizeBytes{};
    // required uint64 totalSizeBytes : Total Size of All Payload
    uint64_t totalSizeBytes{};
    // required uint32 elapsedMs : Elapsed milliseconds since the start of the sync
    uint32_t ElapsedMs() const { return static_cast<uint32_t>(GetMilliTicks() - startTime); }
    // required uint32 hresult : HRESULT of the Sync
    HRESULT hresult{};
    // required uint32 httpStatus : HTTP Status of any service interactions
    uint32_t httpStatus{};
    // required uint32 retryCount : Retry count across all blocks
    uint32_t retryCount{};
    // required wstring contextVersion : manifest version synced
    String contextVersion{};
    // required bool syncDownload : direction of the sync
    bool syncDownload{};
    // required bool inGame : is the sync happening in-game
    bool inGame{ true };

    int64_t startTime{};

    JsonValue ToJson() const;
};

struct ContextSyncErrorEvent
{
    ContextSyncErrorEvent() = default;

    // required uint32 elapsedMs : Elapsed milliseconds since the start of the operation
    uint32_t ElapsedMs() const { return static_cast<uint32_t>(GetMilliTicks() - startTime); }
    // required wstring cv : Service CorrelationVector
    String correlationVector{};
    // required uint32 hresult : HRESULT of the operation
    HRESULT hresult{};
    // required uint32 retryCount : Retry count across all blocks
    uint32_t retryCount{};
    // required wstring contextVersion : manifest version
    String contextVersion{};
    // required wstring blockName : name of the failing block
    String blockName{};
    // required SyncErrorSource errorSource : operation that caused the failure
    SyncErrorSource errorSource{};

    int64_t startTime{};

    JsonValue ToJson() const;
};

class GameSaveTelemetryManager
{
public:
    GameSaveTelemetryManager() noexcept = default;
    ~GameSaveTelemetryManager() noexcept = default;

    HRESULT CreateEntityTelemetryPipeline(const LocalUser& localUser, const XTaskQueueHandle queue) noexcept;

    // ContextActivationEvent
    void SetContextActivationTotalSize(uint64_t totalSize) noexcept { m_contextActivationEvent.totalSizeBytes = totalSize; }
    void SetContextActivationCompressedSize(uint64_t compressedSize) noexcept { m_contextActivationEvent.compressedSizeBytes = compressedSize; }
    void SetContextActivationManifestState(ManifestState manifestState) noexcept { m_contextActivationEvent.manifestState = manifestState; }
    void SetContextActivationSyncState(SyncState syncState) noexcept { m_contextActivationEvent.syncState = syncState; m_contextActivationFailureEvent.syncState = syncState; }
    void SetContextActivationConflictResolution(ConflictResolution conflictResolution) noexcept { m_contextActivationEvent.conflictResolution = conflictResolution; }
    void SetContextActivationCanceled(bool canceled) noexcept { m_contextActivationEvent.canceled = canceled; m_contextActivationFailureEvent.canceled = canceled; } // Also sets error event state
    void SetContextActivationContextVersion(const String& contextVersion) noexcept { m_contextActivationEvent.contextVersion = contextVersion; }
    void SetContextActivationConflictVersion() noexcept { m_contextActivationEvent.conflictVersion = m_contextActivationEvent.baseVersion; }
    void SetContextActivationBaseVersion(const String& baseVersion) noexcept { m_contextActivationEvent.baseVersion = baseVersion; }
    void SetContextActivationHResult(HRESULT hresult) noexcept { m_contextActivationEvent.hresult = hresult; m_contextActivationFailureEvent.hresult = hresult; } // Also sets error event state

    // ContextActivationFailureEvent
    void SetContextActivationAborted(bool aborted) noexcept { m_contextActivationFailureEvent.aborted = aborted; }
    void SetContextActivationRetryAllowed(bool retryAllowed) noexcept { m_contextActivationFailureEvent.retryAllowed = retryAllowed; }
    void SetContextActivationStartedOnline(bool startedOnline) noexcept { m_contextActivationFailureEvent.startedOnline = startedOnline; }
    void SetContextActivationCallingLocation(StateMachineLocation callingLocation) noexcept { m_contextActivationFailureEvent.callingLocation = callingLocation; }
    void SetContextActivationHttpInfo(const std::optional<HttpResult>& httpResult) noexcept
    {
        if (httpResult.has_value())
        {
            m_contextActivationFailureEvent.httpStatus = httpResult.value().httpCode;
            m_contextActivationFailureEvent.correlationVector = httpResult.value().cv;
        }
    }

    // ContextDeleteEvent
    void SetContextDeleteStartTime() noexcept { m_contextDeleteEvent.startTime = GetMilliTicks(); }
    void SetContextDeleteTotalSize(uint64_t totalSize) noexcept { m_contextDeleteEvent.totalSizeBytes = totalSize; }
    void SetContextDeleteHResult(HRESULT hresult) noexcept { m_contextDeleteEvent.hresult = hresult; }
    void SetContextDeleteDeleteType(DeleteType deleteType) noexcept { m_contextDeleteEvent.deleteType = deleteType; }
    void SetContextDeleteContextVersion(const String& contextVersion) noexcept { m_contextDeleteEvent.contextVersion = contextVersion; }
    void SetContextDeleteHttpInfo(const std::optional<HttpResult>& httpResult) noexcept
    {
        if (httpResult.has_value())
        {
            m_contextDeleteEvent.correlationVector = httpResult.value().cv;
        }
    }

    // ContextSyncEvent
    void SetContextSyncStartTime() noexcept { auto startTime = GetMilliTicks(); m_contextSyncEvent.startTime = startTime; m_contextSyncErrorEvent.startTime = startTime; } // Also sets error event state
    void SetContextSyncTotalSize(uint64_t totalSize) noexcept { m_contextSyncEvent.totalSizeBytes = totalSize; }
    void SetContextSyncHResult(HRESULT hresult) noexcept { m_contextSyncEvent.hresult = hresult; m_contextSyncErrorEvent.hresult = hresult; } // Also sets error event state
    void SetContextSyncHttpInfo(const std::optional<HttpResult>& httpResult) noexcept // Also sets error event state
    {
        if (httpResult.has_value())
        {
            m_contextSyncEvent.httpStatus = httpResult.value().httpCode;
            m_contextSyncErrorEvent.correlationVector = httpResult.value().cv;
            m_contextSyncEvent.retryCount = httpResult.value().retryCount;
            m_contextSyncErrorEvent.retryCount = httpResult.value().retryCount;
        }
    }
    void SetContextSyncContextVersion(const String& contextVersion) noexcept { m_contextSyncEvent.contextVersion = contextVersion; m_contextSyncErrorEvent.contextVersion = contextVersion; } // Also sets error event state
    void SetContextSyncSyncDownload(bool syncDownload) noexcept { m_contextSyncEvent.syncDownload = syncDownload; }
    void IncrementContextSyncBlockCount() noexcept { ++m_contextSyncEvent.blockCount; }
    void AddContextSyncFileCount(uint32_t fileCount) noexcept { m_contextSyncEvent.fileCount += fileCount; }
    void AddContextSyncSyncSize(uint64_t syncSize) noexcept { m_contextSyncEvent.syncSizeBytes += syncSize; }
    void AddContextSyncOriginalSize(uint64_t originalSize) noexcept { m_contextSyncEvent.originalSizeBytes += originalSize; }

    // ContextSyncErrorEvent
    void SetContextSyncBlockName(const String& blockName) noexcept { m_contextSyncErrorEvent.blockName = blockName; }
    void SetContextSyncSyncErrorSource(SyncErrorSource errorSource) noexcept { m_contextSyncErrorEvent.errorSource = errorSource; }

    HRESULT EmitContextActivationEvent() noexcept;
    HRESULT EmitContextActivationFailureEvent() const noexcept;
    HRESULT EmitContextDeleteEvent() const noexcept;
    HRESULT EmitContextSyncEvent() noexcept;
    HRESULT EmitContextSyncErrorEvent() const noexcept;

    void ResetContextActivation() noexcept;
    void ResetContextDelete() noexcept;
    void ResetContextSync() noexcept;

    static bool useMocks;

private:
    void PopulateCommonFields(JsonValue& payload) const noexcept;
    HRESULT EmitEvent(const char* eventName, JsonValue& payload) const noexcept;

    Entity m_entity{ Entity::Wrap(nullptr) };
    EventPipeline m_telemetryPipeline{};

    // required wstring userId: Platform UserId
    String m_platformId{ "Unknown" };
    // required wstring platformType: Platform Type / DeviceClass
    PFPlatformType m_platformType{};

    bool m_contextActivationEventEmitted{ false };
    bool m_contextSyncEventEmitted{ false };

    ContextActivationEvent m_contextActivationEvent{};
    ContextActivationFailureEvent m_contextActivationFailureEvent{};
    ContextDeleteEvent m_contextDeleteEvent{};
    ContextSyncEvent m_contextSyncEvent{};
    ContextSyncErrorEvent m_contextSyncErrorEvent{};
};

} // namespace GameSave

// Specialization to define the max value for ManifestState enum (needed due to Clang compiler error)
template <>
struct EnumRange<GameSave::ManifestState>
{
    static constexpr GameSave::ManifestState maxValue = GameSave::MS_PendingDeletion;
};

// Specialization to define the max value for SyncState enum (needed due to Clang compiler error)
template <>
struct EnumRange<GameSave::SyncState>
{
    static constexpr GameSave::SyncState maxValue = GameSave::SS_RollbackToConflict;
};

// Specialization to define the max value for ConflictResolution enum (needed due to Clang compiler error)
template <>
struct EnumRange<GameSave::ConflictResolution>
{
    static constexpr GameSave::ConflictResolution maxValue = GameSave::CR_NoResolutionChosen;
};

// Specialization to define the max value for DeleteType enum (needed due to Clang compiler error)
template <>
struct EnumRange<GameSave::DeleteType>
{
    static constexpr GameSave::DeleteType maxValue = GameSave::DT_All;
};

// Specialization to define the max value for SyncErrorSource enum (needed due to Clang compiler error)
template <>
struct EnumRange<GameSave::SyncErrorSource>
{
    static constexpr GameSave::SyncErrorSource maxValue = GameSave::SER_FinalizeUpload;
};


} // namespace PlayFab
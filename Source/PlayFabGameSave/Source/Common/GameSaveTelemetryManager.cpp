// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"

namespace PlayFab
{
namespace GameSave
{

bool GameSaveTelemetryManager::useMocks{ false };

HRESULT GameSaveTelemetryManager::CreateEntityTelemetryPipeline(const LocalUser& localUser, const XTaskQueueHandle queue) noexcept
{
    ScopeTracer scopeTracer{ "GameSaveTelemetryManager::CreateEntityTelemetryPipeline" };

    if (useMocks)
    {
        TRACE_INFORMATION("[GAME SAVE] GameSaveTelemetryManager::CreateEntityTelemetryPipeline mock");
        return S_OK;
    }

    if (m_telemetryPipeline.Handle() != nullptr)
    {
        // Already initialized
        return S_OK;
    }

    PFEntityHandle entityHandle{};
    RETURN_IF_FAILED(PFLocalUserTryGetEntityHandle(localUser.Handle(), &entityHandle));
    m_entity = Entity::Wrap(entityHandle);
    
    size_t size{ 0 };
    RETURN_IF_FAILED(PFEntityGetTitleIdSize(entityHandle, &size));

    Vector<char> titleId(size, '\0');
    RETURN_IF_FAILED(PFEntityGetTitleId(entityHandle, size, &titleId[0], nullptr));

    RETURN_IF_FAILED(PFPlatformGetPlatformType(&m_platformType));

    const PFEntityKey* entityKey{};
    RETURN_IF_FAILED(PFEntityGetEntityKeySize(entityHandle, &size));
    Vector<char> entityKeyBuffer(size, '\0');
    RETURN_IF_FAILED(PFEntityGetEntityKey(entityHandle, size, &entityKeyBuffer[0], &entityKey, nullptr));

    m_platformId = localUser.LocalId();

    PFEventPipelineHandle pipelineHandle{};
    HRESULT hr = PFEventPipelineCreateTelemetryPipelineHandleWithEntity(entityHandle, queue, nullptr, nullptr, nullptr, &pipelineHandle);

    if (SUCCEEDED(hr))
    {
        TRACE_INFORMATION("[GAME SAVE] GameSaveTelemetryManager::CreateEntityTelemetryPipeline pipeline created");
        m_telemetryPipeline = EventPipeline::Wrap(pipelineHandle);
    }

    return hr;
}

void GameSaveTelemetryManager::PopulateCommonFields(JsonValue& payload) const noexcept
{
    JsonUtils::ObjectAddMember(payload, "userId", m_platformId);
    JsonUtils::ObjectAddMember(payload, "platformType", m_platformType);
}

JsonValue ContextActivationEvent::ToJson() const
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "totalSizeBytes", totalSizeBytes);
    JsonUtils::ObjectAddMember(output, "compressedSizeBytes", compressedSizeBytes);
    JsonUtils::ObjectAddMember(output, "manifestState", manifestState);
    JsonUtils::ObjectAddMember(output, "syncState", syncState);
    JsonUtils::ObjectAddMember(output, "conflictResolution", conflictResolution);
    JsonUtils::ObjectAddMember(output, "canceled", canceled);
    JsonUtils::ObjectAddMember(output, "contextVersion", contextVersion);
    if (!conflictVersion.empty())
    {
        JsonUtils::ObjectAddMember(output, "conflictVersion", conflictVersion);
    }

    return output;
}

JsonValue ContextActivationFailureEvent::ToJson() const
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "canceled", canceled);
    JsonUtils::ObjectAddMember(output, "aborted", aborted);
    JsonUtils::ObjectAddMember(output, "retryAllowed", retryAllowed);
    JsonUtils::ObjectAddMember(output, "startedOnline", startedOnline);
    JsonUtils::ObjectAddMember(output, "syncState", syncState);
    JsonUtils::ObjectAddMember(output, "callingLocation", callingLocation);
    JsonUtils::ObjectAddMember(output, "httpStatus", httpStatus);
    JsonUtils::ObjectAddMember(output, "hresult", hresult);
    JsonUtils::ObjectAddMember(output, "cv", correlationVector);
    return output;
}

JsonValue ContextDeleteEvent::ToJson() const
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "elapsedMs", ElapsedMs());
    JsonUtils::ObjectAddMember(output, "totalSizeBytes", totalSizeBytes);
    JsonUtils::ObjectAddMember(output, "hresult", hresult);
    JsonUtils::ObjectAddMember(output, "type", deleteType);
    if (!contextVersion.empty())
    {
        JsonUtils::ObjectAddMember(output, "contextVersion", contextVersion);
    }
    if (!correlationVector.empty())
    {
        JsonUtils::ObjectAddMember(output, "cv", correlationVector);
    }
    return output;
}

JsonValue ContextSyncEvent::ToJson() const
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "blockCount", blockCount);
    JsonUtils::ObjectAddMember(output, "fileCount", fileCount);
    JsonUtils::ObjectAddMember(output, "syncSizeBytes", syncSizeBytes);
    JsonUtils::ObjectAddMember(output, "originalSizeBytes", originalSizeBytes);
    JsonUtils::ObjectAddMember(output, "totalSizeBytes", totalSizeBytes);
    JsonUtils::ObjectAddMember(output, "elapsedMs", ElapsedMs());
    JsonUtils::ObjectAddMember(output, "hresult", hresult);
    JsonUtils::ObjectAddMember(output, "httpStatus", httpStatus);
    JsonUtils::ObjectAddMember(output, "retryCount", retryCount);
    JsonUtils::ObjectAddMember(output, "contextVersion", contextVersion);
    JsonUtils::ObjectAddMember(output, "syncDownload", syncDownload);
    JsonUtils::ObjectAddMember(output, "inGame", inGame);
    return output;
}

JsonValue ContextSyncErrorEvent::ToJson() const
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "elapsedMs", ElapsedMs());
    JsonUtils::ObjectAddMember(output, "cv", correlationVector);
    JsonUtils::ObjectAddMember(output, "hresult", hresult);
    JsonUtils::ObjectAddMember(output, "retryCount", retryCount);
    JsonUtils::ObjectAddMember(output, "contextVersion", contextVersion);
    JsonUtils::ObjectAddMember(output, "blockName", blockName);
    JsonUtils::ObjectAddMember(output, "errorSource", errorSource);
    return output;
}

HRESULT GameSaveTelemetryManager::EmitEvent(const char* eventName, JsonValue& payload) const noexcept
{
    if (useMocks)
    {
        TRACE_INFORMATION("[GAME SAVE] GameSaveTelemetryManager::EmitEvent %s mock", eventName);
        return S_OK;
    }

    TRACE_INFORMATION("[GAME SAVE] GameSaveTelemetryManager::EmitEvent %s", eventName);

    // Skip telemetry if pipeline was never created (e.g., login failed in offline scenario)
    if (m_telemetryPipeline.Handle() == nullptr)
    {
        TRACE_WARNING("[GAME SAVE] GameSaveTelemetryManager::EmitEvent %s skipped - pipeline not initialized (offline mode)", eventName);
        return S_OK;
    }

    PopulateCommonFields(payload);
    String payloadStr{ JsonUtils::WriteToString(payload) };

    PFEvent event{};
    event.eventNamespace = "playfab.gamesave.internal";
    event.name = eventName;
    event.payloadJson = payloadStr.c_str();

    HRESULT hr = m_telemetryPipeline.EmitEvent(&event);
    if (FAILED(hr))
    {
       TRACE_ERROR("[GAME SAVE] GameSaveTelemetryManager::EmitEvent %s failed with hr:0x%0.8x", eventName, hr);
    }

    return hr;
}

/// <summary>
/// required uint64 totalSizeBytes: How many bytes in the context
/// required uint64 compressedSizeBytes: What is the storage size of the context
/// required ManifestState manifestState: State of the manifest, will be MS_Initialized if new or MS_Finalized if looking at an old version
/// required SyncState syncState: Final SyncState, offline will be SS_NotStarted
/// required ConflictResolution conflictResolution: Resolution if needed
/// required bool canceled: Did user cancel the sync
/// required wstring contextVersion: version being activated
/// optional wstring conflictVersion: version of the manifest with conflict
/// </summary>
/// <returns></returns>
HRESULT GameSaveTelemetryManager::EmitContextActivationEvent() noexcept
{
    if (m_contextActivationEventEmitted)
    {
        return S_OK;
    }

    m_contextActivationEventEmitted = true;
    auto payload = m_contextActivationEvent.ToJson();
    return EmitEvent("context_activation", payload);
}

/// <summary>
/// required bool canceled : Did the user cancel the activation
/// required bool aborted : Did the game abort the activation
/// required bool retryAllowed : Can the game or user retry on this failure
/// required bool startedOnline : Was the system online when activation started
/// required SyncState syncState : At what Sync point did we fail
/// required uint32 callingLocation : what part of the state machine caused the failure
/// required uint32 httpStatus : HTTP Status of any service interactions
/// required uint32 hresult : HRESULT of operation
/// required wstring cv : Service CorrelationVector
/// </summary>
/// <returns></returns>
HRESULT GameSaveTelemetryManager::EmitContextActivationFailureEvent() const noexcept
{
    if (m_contextActivationEventEmitted)
    {
        return S_OK;
    }

    auto payload = m_contextActivationFailureEvent.ToJson();
    return EmitEvent("context_activation_failure", payload);
}

/// <summary>
/// required wstring platformType : Platform Type / DeviceClass
/// required wstring pfTitleId : PlayFab TitleId
/// required wstring userId : Platform UserId
/// required wstring entityId : PlayFab EntityId
/// required uint32 elapsedMs : Elapsed milliseconds since the start of the operation
/// required uint64 totalSizeBytes : total size of the data being discarded
/// required uint32 hresult : HRESULT of the operation
/// required DeleteType type : type of removal
/// optional wstring contextVersion : manifest version being deleted
/// optional wstring cv : Service CorrelationVector
/// </summary>
/// <returns></returns>
HRESULT GameSaveTelemetryManager::EmitContextDeleteEvent() const noexcept
{
    // Don't emit if startTime was never set (may occur when syncing fails early)
    // Don't emit if no data is actually being deleted for Local/All (deleting manifest, DT_Version, has no data on disk being deleted)
    if (m_contextDeleteEvent.startTime == 0 ||
        ((m_contextDeleteEvent.deleteType == DeleteType::DT_All ||
        m_contextDeleteEvent.deleteType == DeleteType::DT_Local) &&
        m_contextDeleteEvent.totalSizeBytes == 0))
    {
        return S_OK;
    }

    JsonValue payload = m_contextDeleteEvent.ToJson();
    return EmitEvent("context_delete", payload);
}

/// <summary>
/// required wstring platformType : Platform Type / DeviceClass
/// required wstring pfTitleId : PlayFab TitleId
/// required wstring userId : Platform UserId
/// required wstring entityId : PlayFab EntityId
/// required uint32 blockCount : Payload Blocks Synced
/// required uint32 fileCount : Files inside Payload Blocks
/// required uint64 syncSizeBytes : Compressed Size of Payload
/// required uint64 originalSizeBytes : Original Size of Payload
/// required uint64 totalSizeBytes : Total Size of All Payload
/// required uint32 elapsedMs : Elapsed milliseconds since the start of the sync
/// required uint32 hresult : HRESULT of the Sync
/// required uint32 retryCount : Retry count across all blocks
/// required wstring contextVersion : manifest version synced
/// required bool syncDownload : direction of the sync
/// </summary>
/// <returns></returns>
HRESULT GameSaveTelemetryManager::EmitContextSyncEvent() noexcept
{
    if (m_contextSyncEventEmitted)
    {
        return S_OK;
    }

    m_contextSyncEventEmitted = true;
    auto payload = m_contextSyncEvent.ToJson();
    return EmitEvent("context_sync", payload);
}

/// <summary>
/// required wstring platformType : Platform Type / DeviceClass
/// required wstring pfTitleId : PlayFab TitleId
/// required wstring userId : Platform UserId
/// required wstring entityId : PlayFab EntityId
/// required uint32 elapsedMs : Elapsed milliseconds since the start of the operation
/// required uint32 hresult : HRESULT of the operation
/// required wstring cv : Service CorrelationVector
/// required uint32 retryCount : Retry count across all blocks
/// required wstring contextVersion : manifest version
/// required wstring blockName : name of the failing block
/// required SyncErrorSource errorSource : operation that caused the failure
/// </summary>
/// <returns></returns>
HRESULT GameSaveTelemetryManager::EmitContextSyncErrorEvent() const noexcept
{
    if (m_contextSyncEventEmitted)
    {
        return S_OK;
    }

    auto payload = m_contextSyncErrorEvent.ToJson();
    return EmitEvent("context_sync_error", payload);
}

void GameSaveTelemetryManager::ResetContextActivation() noexcept
{
    m_contextActivationEvent = {};
    m_contextActivationFailureEvent = {};
    m_contextActivationEventEmitted = false;
}

void GameSaveTelemetryManager::ResetContextDelete() noexcept
{
    m_contextDeleteEvent = {};
}

void GameSaveTelemetryManager::ResetContextSync() noexcept
{
    m_contextSyncEvent = {};
    m_contextSyncErrorEvent = {};
    m_contextSyncEventEmitted = false;
}

} // namespace GameSave
} // namespace PlayFab
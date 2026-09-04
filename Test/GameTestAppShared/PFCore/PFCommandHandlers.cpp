#include "pch.h"

#include "PFCommandHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <XTaskQueue.h>
#include <XUser.h>

#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/core/PFEntity.h>
#include <playfab/core/PFLocalUser.h>
#include <playfab/core/PFLocalUser_Xbox.h>
#include <playfab/core/PFAuthentication.h>
#include <playfab/core/PFAuthenticationTypes.h>
#include <playfab/core/PFHttpConfig.h>
#include "PFGameSaveFilesForDebug.h"
#include "CommandRegistry.h"

using CommandHandlerShared::ComputeElapsedMs;
using CommandHandlerShared::CreateBaseResult;
using CommandHandlerShared::MarkFailure;
using CommandHandlerShared::MarkSuccess;
using CommandHandlerShared::SetHResult;
using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryGetInt64Parameter;
using CommandHandlerShared::TryParseBoolParameter;
using CommandHandlerShared::ToLowerCopy;

namespace
{
    static PFEventPipelineHandle s_eventPipelineHandle = nullptr;

    HRESULT CALLBACK PersistedLocalIdLoginHandler(
        PFLocalUserHandle localUserHandle,
        PFServiceConfigHandle serviceConfigHandle,
        PFEntityHandle existingEntityHandle,
        XAsyncBlock* async)
    {
        UNREFERENCED_PARAMETER(existingEntityHandle);

        DeviceGameSaveState* state = nullptr;
        HRESULT hr = PFLocalUserGetCustomContext(localUserHandle, reinterpret_cast<void**>(&state));
        if (FAILED(hr))
        {
            return hr;
        }

        if (!state)
        {
            return E_POINTER;
        }

        PFAuthenticationLoginWithCustomIDRequest request{};
        std::string& customId = state->inputCustomUserId;
        if (customId.empty())
        {
            customId = "pf-gamesave-automation";
        }

        request.customId = customId.c_str();
        request.createAccount = state->createAccountIfMissing;

        return PFAuthenticationLoginWithCustomIDAsync(serviceConfigHandle, &request, async);
    }
}

CommandResultPayload HandlePFInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    UNREFERENCED_PARAMETER(parameters);

    XTaskQueueHandle queueForPF = (state->taskQueueOwnedByCommand && state->taskQueue != nullptr)
        ? state->taskQueue
        : nullptr;
    auto start = std::chrono::steady_clock::now();
    if (state->pfInitialized)
    {
        LogToWindow("PFInitialize skipped (already initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    HRESULT hr = PFInitialize(queueForPF);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    // Handle E_PF_ALREADY_INITIALIZED (0x89235401) - PF is already initialized,
    // so update our state flag and treat as success. This can happen if the device
    // process wasn't properly cleaned up between test runs.
    if (hr == static_cast<HRESULT>(0x89235401))
    {
        LogToWindow("PFInitialize: already initialized (recovering state)");
        state->pfInitialized = true;
        MarkSuccess(payload.result);
        return payload;
    }

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFInitialize failed");
        return payload;
    }

    state->pfInitialized = true;

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFServicesInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    UNREFERENCED_PARAMETER(parameters);

    XTaskQueueHandle queueForPF = (state->taskQueueOwnedByCommand && state->taskQueue != nullptr)
        ? state->taskQueue
        : nullptr;
    auto start = std::chrono::steady_clock::now();
    if (state->pfServicesInitialized)
    {
        LogToWindow("PFServicesInitialize skipped (already initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    HRESULT hr = PFServicesInitialize(queueForPF);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFServicesInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    // Handle E_PF_SERVICES_ALREADY_INITIALIZED (0x89235401) - Services already initialized,
    // so update our state flag and treat as success. This can happen if the device
    // process wasn't properly cleaned up between test runs.
    if (hr == static_cast<HRESULT>(0x89235401))
    {
        LogToWindow("PFServicesInitialize: already initialized (recovering state)");
        state->pfServicesInitialized = true;
        MarkSuccess(payload.result);
        return payload;
    }

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFServicesInitialize failed");
        return payload;
    }

    state->pfServicesInitialized = true;

    // Re-apply verbose HC tracing after PFServicesInitialize. PFServicesInitialize internally
    // calls PFInitializeWithLHC which creates a TraceState that sets HCTraceSetClientCallback.
    // When that TraceState creation fails (already initialized), the error path calls
    // HCTraceSetClientCallback(nullptr), nuking any existing callback. We must re-apply ours.
    InitializeHCTraceToVerboseLog();

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFServiceConfigCreateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    std::string endpoint;
    std::string titleId;
    std::string error;

    if (!TryGetStringParameter(parameters, "endpoint", endpoint, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!TryGetStringParameter(parameters, "titleId", titleId, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        PFServiceConfigHandle newHandle{ nullptr };
        HRESULT hr = PFServiceConfigCreateHandle(endpoint.c_str(), titleId.c_str(), &newHandle);

        LogToWindowFormat("PFServiceConfigCreateHandle (endpoint=%s, titleId=%s, hr=0x%08X)",
            endpoint.c_str(), titleId.c_str(), static_cast<uint32_t>(hr));

        if (FAILED(hr))
        {
            if (newHandle)
            {
                PFServiceConfigCloseHandle(newHandle);
            }
            return hr;
        }

        if (state->serviceConfigHandle)
        {
            PFServiceConfigCloseHandle(state->serviceConfigHandle);
        }

        state->serviceConfigHandle = newHandle;
        state->serviceConfigEndpoint = endpoint;
        state->serviceConfigTitleId = titleId;
        return hr;
    });
}

#ifdef _WIN32

CommandResultPayload HandlePFLocalUserCreateHandleWithXboxUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    // Parse userIndex (which XUser to read) and localUserIndex (where to store local user)
    // If localUserIndex is not specified, it defaults to userIndex (store in matching slot)
    int userIndex = ParseIndexParam(parameters, "userIndex");
    int localUserIndex = (parameters.is_object() && parameters.contains("localUserIndex"))
        ? ParseIndexParam(parameters, "localUserIndex")
        : userIndex;

    if (!state->serviceConfigHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Service config handle not created");
        return payload;
    }

    XUserHandle& sourceXUser = GetXUserHandle(state, userIndex);
    if (!sourceXUser)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER,
            std::string("XUser handle not available at userIndex=") + std::to_string(userIndex));
        return payload;
    }

    PFLocalUserHandle& targetLocalUser = GetLocalUserHandle(state, localUserIndex);
    if (targetLocalUser)
    {
        PFLocalUserCloseHandle(targetLocalUser);
        targetLocalUser = nullptr;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFLocalUserCreateHandleWithXboxUser(state->serviceConfigHandle, sourceXUser, state, &targetLocalUser);
        LogToWindowFormat("PFLocalUserCreateHandleWithXboxUser (hr=0x%08X, userIndex=%d, localUserIndex=%d)",
            static_cast<uint32_t>(hr), userIndex, localUserIndex);
        return hr;
    });
}

#endif

CommandResultPayload HandlePFLocalUserCreateHandleWithPersistedLocalId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->serviceConfigHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Service config handle not created");
        return payload;
    }

    std::string persistedId;
    std::string error;
    if (!TryGetStringParameter(parameters, "persistedLocalId", persistedId, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->persistedLocalId = persistedId;

    auto customIdIt = parameters.find("customId");
    if (customIdIt != parameters.end())
    {
        if (!customIdIt->is_string())
        {
            MarkFailure(payload.result, E_INVALIDARG, "customId must be a string");
            return payload;
        }

        state->inputCustomUserId = customIdIt->get<std::string>();
    }

    bool createAccount = true;
    if (!TryParseBoolParameter(parameters, "createAccount", createAccount, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }
    state->createAccountIfMissing = createAccount;

    // skipLogin: If true, creates the user handle without calling PFLocalUserLoginAsync.
    // This is useful for testing scenarios where login should happen during AddUserWithUiAsync
    // instead of during user handle creation.
    bool skipLogin = false;
    if (!TryParseBoolParameter(parameters, "skipLogin", skipLogin, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (state->localUserHandle)
    {
        PFLocalUserCloseHandle(state->localUserHandle);
        state->localUserHandle = nullptr;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFLocalUserCreateHandleWithPersistedLocalId(
        state->serviceConfigHandle,
        state->persistedLocalId.c_str(),
        PersistedLocalIdLoginHandler,
        state,
        &state->localUserHandle);
    if (FAILED(hr))
    {
        payload.elapsedMs = ComputeElapsedMs(start);
        LogToWindowFormat("PFLocalUserCreateHandleWithPersistedLocalId (hr=0x%08X)", static_cast<uint32_t>(hr));
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFLocalUserCreateHandleWithPersistedLocalId failed");
        return payload;
    }

    // If skipLogin is true, skip the login step entirely. The entity will not be associated
    // with the user handle, forcing AddUserWithUiAsync to perform login during its LockStep::Login stage.
    if (skipLogin)
    {
        payload.elapsedMs = ComputeElapsedMs(start);
        LogToWindowFormat("PFLocalUserCreateHandleWithPersistedLocalId (hr=0x%08X) - skipLogin=true, skipping PFLocalUserLoginAsync", static_cast<uint32_t>(hr));
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    XAsyncBlock loginAsync{};
    HRESULT loginHr = PFLocalUserLoginAsync(state->localUserHandle, state->createAccountIfMissing, &loginAsync);
    HRESULT loginWaitHr = S_OK;
    if (SUCCEEDED(loginHr))
    {
        loginWaitHr = XAsyncGetStatus(&loginAsync, true);
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFLocalUserCreateHandleWithPersistedLocalId (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(loginHr))
    {
        SetHResult(payload.result, loginHr);
        MarkFailure(payload.result, loginHr, "PFLocalUserLoginAsync failed");
        return payload;
    }

    if (FAILED(loginWaitHr))
    {
        SetHResult(payload.result, loginWaitHr);
        MarkFailure(payload.result, loginWaitHr, "PFLocalUserLoginAsync wait failed");
        return payload;
    }

    // Must retrieve the result to complete the XAsync cleanup. Without this, the async provider
    // will not be cleaned up properly and PFUninitializeAsync will hang waiting for termination.
    PFEntityHandle entityHandle = nullptr;
    HRESULT getResultHr = PFLocalUserLoginGetResult(&loginAsync, &entityHandle, 0, nullptr, nullptr, nullptr);
    if (FAILED(getResultHr))
    {
        SetHResult(payload.result, getResultHr);
        MarkFailure(payload.result, getResultHr, "PFLocalUserLoginGetResult failed");
        return payload;
    }

    // Close the entity handle - it's automatically associated with the local user
    if (entityHandle)
    {
        PFEntityCloseHandle(entityHandle);
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFLocalUserCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    int localUserIndex = ParseIndexParam(parameters, "localUserIndex");
    PFLocalUserHandle& targetSlot = GetLocalUserHandle(state, (localUserIndex >= 0 && localUserIndex < DeviceGameSaveState::kMaxUsers) ? localUserIndex : 0);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        if (targetSlot)
        {
            PFLocalUserCloseHandle(targetSlot);
            targetSlot = nullptr;
            LogToWindowFormat("PFLocalUserCloseHandle executed (localUserIndex=%d)", localUserIndex);
        }
        else
        {
            LogToWindowFormat("PFLocalUserCloseHandle skipped (no handle at localUserIndex=%d)", localUserIndex);
        }
        return S_OK;
    });
}

CommandResultPayload HandlePFServiceConfigCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        if (state->serviceConfigHandle)
        {
            PFServiceConfigCloseHandle(state->serviceConfigHandle);
            state->serviceConfigHandle = nullptr;
            LogToWindow("PFServiceConfigCloseHandle executed");
        }
        else
        {
            LogToWindow("PFServiceConfigCloseHandle skipped (no handle)");
        }
        return S_OK;
    });
}

CommandResultPayload HandlePFServicesUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->pfServicesInitialized)
    {
        return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFServicesUninitializeAsync skipped (not initialized)");
            return S_OK;
        });
    }

    return CommandHandlerShared::AsyncCallWithResult(state->taskQueueOwnedByCommand ? state->taskQueue : nullptr, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            // Close entity handle stored from CustomID login before uninit
            if (state->entityHandle)
            {
                PFEntityCloseHandle(state->entityHandle);
                state->entityHandle = nullptr;
            }
            HRESULT hr = PFServicesUninitializeAsync(&async);
            LogToWindowFormat("PFServicesUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            state->pfServicesInitialized = false;
            return S_OK;
        });
}

CommandResultPayload HandlePFUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->pfInitialized)
    {
        return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFUninitializeAsync skipped (not initialized)");
            return S_OK;
        });
    }

    return CommandHandlerShared::AsyncCallWithResult(state->taskQueueOwnedByCommand ? state->taskQueue : nullptr, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFUninitializeAsync(&async);
            LogToWindowFormat("PFUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            state->pfInitialized = false;
            // Clear handles invalidated by PFUninitializeAsync
            state->serviceConfigHandle = nullptr;
            if (state->entityHandle) { state->entityHandle = nullptr; }
            if (state->localUserHandle) { state->localUserHandle = nullptr; }
            return S_OK;
        });
}

CommandResultPayload HandlePFEventPipelineCreateTelemetryPipelineHandleWithKey(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_eventPipelineHandle) { PFEventPipelineCloseHandle(s_eventPipelineHandle); s_eventPipelineHandle = nullptr; }
            PFEventPipelineTelemetryKeyConfig telKeyConfig{};
            telKeyConfig.serviceConfigHandle = state->serviceConfigHandle;
            telKeyConfig.telemetryKey = "testTelemetryKey";
            HRESULT hr = PFEventPipelineCreateTelemetryPipelineHandleWithKey(&telKeyConfig, state->taskQueue, nullptr, nullptr, nullptr, &s_eventPipelineHandle);
            LogToWindowFormat("PFEventPipelineCreateTelemetryPipelineHandleWithKey (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineCreateTelemetryPipelineHandleWithEntity(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_eventPipelineHandle) { PFEventPipelineCloseHandle(s_eventPipelineHandle); s_eventPipelineHandle = nullptr; }
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            if (SUCCEEDED(hr))
            {
                hr = PFEventPipelineCreateTelemetryPipelineHandleWithEntity(entityHandle, state->taskQueue, nullptr, nullptr, nullptr, &s_eventPipelineHandle);
                PFEntityCloseHandle(entityHandle);
            }
            LogToWindowFormat("PFEventPipelineCreateTelemetryPipelineHandleWithEntity (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineCreatePlayStreamPipelineHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_eventPipelineHandle) { PFEventPipelineCloseHandle(s_eventPipelineHandle); s_eventPipelineHandle = nullptr; }
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            if (SUCCEEDED(hr))
            {
                hr = PFEventPipelineCreatePlayStreamPipelineHandle(entityHandle, state->taskQueue, nullptr, nullptr, nullptr, &s_eventPipelineHandle);
                PFEntityCloseHandle(entityHandle);
            }
            LogToWindowFormat("PFEventPipelineCreatePlayStreamPipelineHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_eventPipelineHandle)
            {
                LogToWindow("PFEventPipelineDuplicateHandle: no pipeline handle available");
                return E_INVALIDARG;
            }
            PFEventPipelineHandle duplicated = nullptr;
            HRESULT hr = PFEventPipelineDuplicateHandle(s_eventPipelineHandle, &duplicated);
            LogToWindowFormat("PFEventPipelineDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && duplicated) { PFEventPipelineCloseHandle(duplicated); }
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineEmitEvent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_eventPipelineHandle)
            {
                LogToWindow("PFEventPipelineEmitEvent: no pipeline handle available");
                return E_INVALIDARG;
            }
            PFEvent event{};
            std::string eventNamespace = "custom";
            std::string eventName = "test_event";
            std::string error;
            CommandHandlerShared::TryGetStringParameter(parameters, "eventNamespace", eventNamespace, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "eventName", eventName, error);
            event.eventNamespace = eventNamespace.c_str();
            event.name = eventName.c_str();
            event.payloadJson = "{}";
            HRESULT hr = PFEventPipelineEmitEvent(s_eventPipelineHandle, &event);
            LogToWindowFormat("PFEventPipelineEmitEvent (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineAddUploadingEntity(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_eventPipelineHandle)
            {
                LogToWindow("PFEventPipelineAddUploadingEntity: no pipeline handle available");
                return E_INVALIDARG;
            }
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            if (SUCCEEDED(hr))
            {
                hr = PFEventPipelineAddUploadingEntity(s_eventPipelineHandle, entityHandle);
                PFEntityCloseHandle(entityHandle);
            }
            LogToWindowFormat("PFEventPipelineAddUploadingEntity (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineRemoveUploadingEntity(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_eventPipelineHandle)
            {
                LogToWindow("PFEventPipelineRemoveUploadingEntity: no pipeline handle available");
                return E_INVALIDARG;
            }
            HRESULT hr = PFEventPipelineRemoveUploadingEntity(s_eventPipelineHandle);
            LogToWindowFormat("PFEventPipelineRemoveUploadingEntity (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventPipelineUpdateConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!s_eventPipelineHandle)
            {
                LogToWindow("PFEventPipelineUpdateConfiguration: no pipeline handle available");
                return E_INVALIDARG;
            }
            PFEventPipelineConfig pipelineConfig{};
            HRESULT hr = PFEventPipelineUpdateConfiguration(s_eventPipelineHandle, pipelineConfig);
            LogToWindowFormat("PFEventPipelineUpdateConfiguration (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventsDeleteDataConnectionAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsDeleteDataConnectionAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsDeleteDataConnectionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsDeleteDataConnectionGetResult: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsGetDataConnectionAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsGetDataConnectionAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsGetDataConnectionGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsGetDataConnectionGetResultSize: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsGetDataConnectionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsGetDataConnectionGetResult: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsListDataConnectionsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsListDataConnectionsAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsListDataConnectionsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsListDataConnectionsGetResultSize: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsListDataConnectionsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsListDataConnectionsGetResult: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionGetResultSize: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionGetResult: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionActiveAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionActiveAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionActiveGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionActiveGetResultSize: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsSetDataConnectionActiveGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsSetDataConnectionActiveGetResult: API not available (compiled out)");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventsWriteEventsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEventsWriteEventsAsync: failed to get entity (hr=0x%08X)", static_cast<uint32_t>(hr));
                return hr;
            }
            PFEventsEventContents eventContent{};
            eventContent.eventNamespace = "custom";
            eventContent.name = "test_event";
            eventContent.payloadJSON = "{}";
            const PFEventsEventContents* events[] = { &eventContent };
            PFEventsWriteEventsRequest request{};
            request.events = events;
            request.eventsCount = 1;
            hr = PFEventsWriteEventsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFEventsWriteEventsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteEventsGetResultSize(&async, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                PFEventsWriteEventsResponse* result = nullptr;
                hr = PFEventsWriteEventsGetResult(&async, bufferSize, buffer.data(), &result, nullptr);
            }
            return hr;
        });
}

CommandResultPayload HandlePFEventsWriteEventsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteEventsGetResultSize(&localAsync, &bufferSize);
            LogToWindowFormat("PFEventsWriteEventsGetResultSize (hr=0x%08X, bufferSize=%zu)", static_cast<uint32_t>(hr), bufferSize);
            return hr;
        });
}

CommandResultPayload HandlePFEventsWriteEventsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteEventsGetResultSize(&localAsync, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                PFEventsWriteEventsResponse* result = nullptr;
                hr = PFEventsWriteEventsGetResult(&localAsync, bufferSize, buffer.data(), &result, nullptr);
            }
            LogToWindowFormat("PFEventsWriteEventsGetResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFEventsWriteTelemetryEventsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            if (FAILED(hr))
            {
                LogToWindowFormat("PFEventsWriteTelemetryEventsAsync: failed to get entity (hr=0x%08X)", static_cast<uint32_t>(hr));
                return hr;
            }
            PFEventsEventContents eventContent{};
            eventContent.eventNamespace = "custom";
            eventContent.name = "test_telemetry_event";
            eventContent.payloadJSON = "{}";
            const PFEventsEventContents* events[] = { &eventContent };
            PFEventsWriteEventsRequest request{};
            request.events = events;
            request.eventsCount = 1;
            hr = PFEventsWriteTelemetryEventsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFEventsWriteTelemetryEventsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteTelemetryEventsGetResultSize(&async, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                PFEventsWriteEventsResponse* result = nullptr;
                hr = PFEventsWriteTelemetryEventsGetResult(&async, bufferSize, buffer.data(), &result, nullptr);
            }
            return hr;
        });
}

CommandResultPayload HandlePFEventsWriteTelemetryEventsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteTelemetryEventsGetResultSize(&localAsync, &bufferSize);
            LogToWindowFormat("PFEventsWriteTelemetryEventsGetResultSize (hr=0x%08X, bufferSize=%zu)", static_cast<uint32_t>(hr), bufferSize);
            return hr;
        });
}

CommandResultPayload HandlePFEventsWriteTelemetryEventsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            size_t bufferSize = 0;
            HRESULT hr = PFEventsWriteTelemetryEventsGetResultSize(&localAsync, &bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                PFEventsWriteEventsResponse* result = nullptr;
                hr = PFEventsWriteTelemetryEventsGetResult(&localAsync, bufferSize, buffer.data(), &result, nullptr);
            }
            LogToWindowFormat("PFEventsWriteTelemetryEventsGetResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFSetHttpRetrySettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFHttpRetrySettings settings{};
            settings.allowRetry = true;
            settings.minimumRetryDelayInSeconds = 2;
            settings.timeoutWindowInSeconds = 20;
            HRESULT hr = PFSetHttpRetrySettings(&settings);
            LogToWindowFormat("PFSetHttpRetrySettings (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGetHttpRetrySettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFHttpRetrySettings settings{};
            HRESULT hr = PFGetHttpRetrySettings(&settings);
            LogToWindowFormat("PFGetHttpRetrySettings (hr=0x%08X, allowRetry=%d, minDelay=%u, timeout=%u)", static_cast<uint32_t>(hr), settings.allowRetry, settings.minimumRetryDelayInSeconds, settings.timeoutWindowInSeconds);
            return hr;
        });
}

CommandResultPayload HandlePFSetHttpSettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFHttpSettings settings{};
            settings.requestResponseCompression = false;
            HRESULT hr = PFSetHttpSettings(&settings);
            LogToWindowFormat("PFSetHttpSettings (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGetHttpSettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFHttpSettings settings{};
            HRESULT hr = PFGetHttpSettings(&settings);
            LogToWindowFormat("PFGetHttpSettings (hr=0x%08X, compression=%d)", static_cast<uint32_t>(hr), settings.requestResponseCompression);
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLocalUserHandle duplicated = nullptr;
            HRESULT hr = PFLocalUserDuplicateHandle(state->localUserHandle, &duplicated);
            LogToWindowFormat("PFLocalUserDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && duplicated) { PFLocalUserCloseHandle(duplicated); }
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserGetServiceConfigHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFServiceConfigHandle scHandle = nullptr;
            HRESULT hr = PFLocalUserGetServiceConfigHandle(state->localUserHandle, &scHandle);
            LogToWindowFormat("PFLocalUserGetServiceConfigHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && scHandle) { PFServiceConfigCloseHandle(scHandle); }
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserGetLocalIdSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t localIdSize = 0;
            HRESULT hr = PFLocalUserGetLocalIdSize(state->localUserHandle, &localIdSize);
            LogToWindowFormat("PFLocalUserGetLocalIdSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), localIdSize);
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserGetLocalId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t localIdSize = 0;
            HRESULT hr = PFLocalUserGetLocalIdSize(state->localUserHandle, &localIdSize);
            if (SUCCEEDED(hr) && localIdSize > 0)
            {
                std::vector<char> buffer(localIdSize);
                size_t used = 0;
                hr = PFLocalUserGetLocalId(state->localUserHandle, localIdSize, buffer.data(), &used);
                if (SUCCEEDED(hr)) { LogToWindowFormat("PFLocalUserGetLocalId: '%s'", buffer.data()); }
            }
            LogToWindowFormat("PFLocalUserGetLocalId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserGetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            void* customContext = nullptr;
            HRESULT hr = PFLocalUserGetCustomContext(state->localUserHandle, &customContext);
            LogToWindowFormat("PFLocalUserGetCustomContext (hr=0x%08X, context=%p)", static_cast<uint32_t>(hr), customContext);
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserTryGetEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle);
            LogToWindowFormat("PFLocalUserTryGetEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && entityHandle) { PFEntityCloseHandle(entityHandle); }
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserLoginAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLocalUserLoginAsync(state->localUserHandle, state->createAccountIfMissing, &async);
            LogToWindowFormat("PFLocalUserLoginAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserLoginGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr);
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserLoginGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            size_t bufferSize = 0;
            HRESULT hr = PFLocalUserLoginGetResultSize(&localAsync, &bufferSize);
            LogToWindowFormat("PFLocalUserLoginGetResultSize (hr=0x%08X, bufferSize=%zu)", static_cast<uint32_t>(hr), bufferSize);
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserLoginGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock localAsync{};
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserLoginGetResult(&localAsync, &entityHandle, 0, nullptr, nullptr, nullptr);
            LogToWindowFormat("PFLocalUserLoginGetResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (entityHandle) { PFEntityCloseHandle(entityHandle); }
            return hr;
        });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFLocalUserCreateHandleWithSteamUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLocalUserHandle steamUser = nullptr;
            HRESULT hr = PFLocalUserCreateHandleWithSteamUser(state->serviceConfigHandle, nullptr, &steamUser);
            LogToWindowFormat("PFLocalUserCreateHandleWithSteamUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && steamUser) { PFLocalUserCloseHandle(steamUser); }
            return hr;
        });
}

CommandResultPayload HandlePFLocalUserTryGetXUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XUserHandle xuser = nullptr;
            HRESULT hr = PFLocalUserTryGetXUser(state->localUserHandle, &xuser);
            LogToWindowFormat("PFLocalUserTryGetXUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && xuser) { XUserCloseHandle(xuser); }
            return hr;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFMemSetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFMemoryHooks hooks{};
            HRESULT hr = PFMemGetFunctions(&hooks);
            if (SUCCEEDED(hr))
            {
                hr = PFMemSetFunctions(&hooks);
            }
            LogToWindowFormat("PFMemSetFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMemGetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFMemoryHooks hooks{};
            HRESULT hr = PFMemGetFunctions(&hooks);
            LogToWindowFormat("PFMemGetFunctions (hr=0x%08X, alloc=%p, free=%p)", static_cast<uint32_t>(hr), reinterpret_cast<void*>(hooks.alloc), reinterpret_cast<void*>(hooks.free));
            return hr;
        });
}

CommandResultPayload HandlePFMemIsUsingCustomMemoryFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool isCustom = false;
            HRESULT hr = PFMemIsUsingCustomMemoryFunctions(&isCustom);
            LogToWindowFormat("PFMemIsUsingCustomMemoryFunctions (hr=0x%08X, isCustom=%d)", static_cast<uint32_t>(hr), isCustom);
            return hr;
        });
}

static HRESULT STDAPIVCALLTYPE TestLocalStorageRead(
    _In_opt_ void* /*context*/, _In_z_ const char* /*key*/, _Inout_ XAsyncBlock* async)
{
    XAsyncComplete(async, S_OK, 0);
    return S_OK;
}

static HRESULT STDAPIVCALLTYPE TestLocalStorageWrite(
    _In_opt_ void* /*context*/, _In_z_ const char* /*key*/, _In_ size_t /*dataSize*/,
    _In_reads_bytes_(dataSize) void const* /*data*/, _Inout_ XAsyncBlock* async)
{
    XAsyncComplete(async, S_OK, 0);
    return S_OK;
}

static HRESULT STDAPIVCALLTYPE TestLocalStorageClear(
    _In_opt_ void* /*context*/, _In_z_ const char* /*key*/, _Inout_ XAsyncBlock* async)
{
    XAsyncComplete(async, S_OK, 0);
    return S_OK;
}

CommandResultPayload HandlePFPlatformLocalStorageSetHandlers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLocalStorageHooks hooks{};
            hooks.read = TestLocalStorageRead;
            hooks.write = TestLocalStorageWrite;
            hooks.clear = TestLocalStorageClear;
            HRESULT hr = PFPlatformLocalStorageSetHandlers(&hooks);
            LogToWindowFormat("PFPlatformLocalStorageSetHandlers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFPlatformIsGRTSAvailable(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool isAvailable = false;
            HRESULT hr = PFPlatformIsGRTSAvailable(&isAvailable);
            LogToWindowFormat("PFPlatformIsGRTSAvailable (hr=0x%08X, isAvailable=%d)", static_cast<uint32_t>(hr), isAvailable);
            return hr;
        });
}

CommandResultPayload HandlePFPlatformGetPlatformType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFPlatformType platformType = PFPlatformType::Unknown;
            HRESULT hr = PFPlatformGetPlatformType(&platformType);
            LogToWindowFormat("PFPlatformGetPlatformType (hr=0x%08X, type=%d)", static_cast<uint32_t>(hr), static_cast<int>(platformType));
            return hr;
        });
}

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
CommandResultPayload HandlePFPlatformGetGameSaveContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            void* gameSaveContext = nullptr;
            HRESULT hr = PFPlatformGetGameSaveContext(&gameSaveContext);
            LogToWindowFormat("PFPlatformGetGameSaveContext (hr=0x%08X, context=%p)", static_cast<uint32_t>(hr), gameSaveContext);
            return hr;
        });
}
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

CommandResultPayload HandlePFServiceConfigDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFServiceConfigHandle duplicated = nullptr;
            HRESULT hr = PFServiceConfigDuplicateHandle(state->serviceConfigHandle, &duplicated);
            LogToWindowFormat("PFServiceConfigDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && duplicated) { PFServiceConfigCloseHandle(duplicated); }
            return hr;
        });
}

CommandResultPayload HandlePFServiceConfigGetAPIEndpointSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t endpointSize = 0;
            HRESULT hr = PFServiceConfigGetAPIEndpointSize(state->serviceConfigHandle, &endpointSize);
            LogToWindowFormat("PFServiceConfigGetAPIEndpointSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), endpointSize);
            return hr;
        });
}

CommandResultPayload HandlePFServiceConfigGetAPIEndpoint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t endpointSize = 0;
            HRESULT hr = PFServiceConfigGetAPIEndpointSize(state->serviceConfigHandle, &endpointSize);
            if (SUCCEEDED(hr) && endpointSize > 0)
            {
                std::vector<char> buffer(endpointSize);
                size_t used = 0;
                hr = PFServiceConfigGetAPIEndpoint(state->serviceConfigHandle, endpointSize, buffer.data(), &used);
                if (SUCCEEDED(hr)) { LogToWindowFormat("PFServiceConfigGetAPIEndpoint: '%s'", buffer.data()); }
            }
            LogToWindowFormat("PFServiceConfigGetAPIEndpoint (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFServiceConfigGetTitleIdSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t titleIdSize = 0;
            HRESULT hr = PFServiceConfigGetTitleIdSize(state->serviceConfigHandle, &titleIdSize);
            LogToWindowFormat("PFServiceConfigGetTitleIdSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), titleIdSize);
            return hr;
        });
}

CommandResultPayload HandlePFServiceConfigGetTitleId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t titleIdSize = 0;
            HRESULT hr = PFServiceConfigGetTitleIdSize(state->serviceConfigHandle, &titleIdSize);
            if (SUCCEEDED(hr) && titleIdSize > 0)
            {
                std::vector<char> buffer(titleIdSize);
                size_t used = 0;
                hr = PFServiceConfigGetTitleId(state->serviceConfigHandle, titleIdSize, buffer.data(), &used);
                if (SUCCEEDED(hr)) { LogToWindowFormat("PFServiceConfigGetTitleId: '%s'", buffer.data()); }
            }
            LogToWindowFormat("PFServiceConfigGetTitleId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFTraceEnableTraceToFile(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* traceDir = ".";
            HRESULT hr = PFTraceEnableTraceToFile(traceDir);
            LogToWindowFormat("PFTraceEnableTraceToFile (hr=0x%08X, dir='%s')", static_cast<uint32_t>(hr), traceDir);
            return hr;
        });
}

CommandResultPayload HandlePFTraceDisableTraceToFile(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTraceDisableTraceToFile: API not available");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFLocalUserHandleCompare(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int32_t compareResult = PFLocalUserHandleCompare(state->localUserHandle, state->localUserHandle);
            LogToWindowFormat("PFLocalUserHandleCompare (result=%d)", compareResult);
            payload.result["areEqual"] = (compareResult == 0);
            return S_OK;
        });
}

CommandResultPayload HandlePFLocalUserSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLocalUserSetCustomContext: API not available");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFEventPipelineCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_eventPipelineHandle)
            {
                PFEventPipelineCloseHandle(s_eventPipelineHandle);
                s_eventPipelineHandle = nullptr;
            }
            LogToWindow("PFEventPipelineCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandlePFEventsCreateDataConnectionAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFEventsCreateDataConnectionAsync: API not available (compiled out)");
            (void)async;
            return E_NOTIMPL;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFEventPipelineAddUploadingEntity", HandlePFEventPipelineAddUploadingEntity },
    { "PFEventPipelineCloseHandle", HandlePFEventPipelineCloseHandle },
    { "PFEventPipelineCreatePlayStreamPipelineHandle", HandlePFEventPipelineCreatePlayStreamPipelineHandle },
    { "PFEventPipelineCreateTelemetryPipelineHandleWithEntity", HandlePFEventPipelineCreateTelemetryPipelineHandleWithEntity },
    { "PFEventPipelineCreateTelemetryPipelineHandleWithKey", HandlePFEventPipelineCreateTelemetryPipelineHandleWithKey },
    { "PFEventPipelineDuplicateHandle", HandlePFEventPipelineDuplicateHandle },
    { "PFEventPipelineEmitEvent", HandlePFEventPipelineEmitEvent },
    { "PFEventPipelineRemoveUploadingEntity", HandlePFEventPipelineRemoveUploadingEntity },
    { "PFEventPipelineUpdateConfiguration", HandlePFEventPipelineUpdateConfiguration },
    { "PFEventsDeleteDataConnectionAsync", HandlePFEventsDeleteDataConnectionAsync },
    { "PFEventsDeleteDataConnectionGetResult", HandlePFEventsDeleteDataConnectionGetResult },
    { "PFEventsGetDataConnectionAsync", HandlePFEventsGetDataConnectionAsync },
    { "PFEventsGetDataConnectionGetResult", HandlePFEventsGetDataConnectionGetResult },
    { "PFEventsGetDataConnectionGetResultSize", HandlePFEventsGetDataConnectionGetResultSize },
    { "PFEventsListDataConnectionsAsync", HandlePFEventsListDataConnectionsAsync },
    { "PFEventsListDataConnectionsGetResult", HandlePFEventsListDataConnectionsGetResult },
    { "PFEventsListDataConnectionsGetResultSize", HandlePFEventsListDataConnectionsGetResultSize },
    { "PFEventsSetDataConnectionActiveAsync", HandlePFEventsSetDataConnectionActiveAsync },
    { "PFEventsSetDataConnectionActiveGetResult", HandlePFEventsSetDataConnectionActiveGetResult },
    { "PFEventsSetDataConnectionActiveGetResultSize", HandlePFEventsSetDataConnectionActiveGetResultSize },
    { "PFEventsSetDataConnectionAsync", HandlePFEventsSetDataConnectionAsync },
    { "PFEventsSetDataConnectionGetResult", HandlePFEventsSetDataConnectionGetResult },
    { "PFEventsSetDataConnectionGetResultSize", HandlePFEventsSetDataConnectionGetResultSize },
    { "PFEventsWriteEventsAsync", HandlePFEventsWriteEventsAsync },
    { "PFEventsWriteEventsGetResult", HandlePFEventsWriteEventsGetResult },
    { "PFEventsWriteEventsGetResultSize", HandlePFEventsWriteEventsGetResultSize },
    { "PFEventsWriteTelemetryEventsAsync", HandlePFEventsWriteTelemetryEventsAsync },
    { "PFEventsWriteTelemetryEventsGetResult", HandlePFEventsWriteTelemetryEventsGetResult },
    { "PFEventsWriteTelemetryEventsGetResultSize", HandlePFEventsWriteTelemetryEventsGetResultSize },
    { "PFEventsCreateDataConnectionAsync", HandlePFEventsCreateDataConnectionAsync },
    { "PFGetHttpRetrySettings", HandlePFGetHttpRetrySettings },
    { "PFGetHttpSettings", HandlePFGetHttpSettings },
    { "PFInitialize", HandlePFInitialize },
    { "PFLocalUserCloseHandle", HandlePFLocalUserCloseHandle },
    { "PFLocalUserCreateHandleWithPersistedLocalId", HandlePFLocalUserCreateHandleWithPersistedLocalId },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFLocalUserCreateHandleWithSteamUser", HandlePFLocalUserCreateHandleWithSteamUser },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
#ifdef _WIN32
    { "PFLocalUserCreateHandleWithXboxUser", HandlePFLocalUserCreateHandleWithXboxUser },
#endif
    { "PFLocalUserDuplicateHandle", HandlePFLocalUserDuplicateHandle },
    { "PFLocalUserGetCustomContext", HandlePFLocalUserGetCustomContext },
    { "PFLocalUserGetLocalId", HandlePFLocalUserGetLocalId },
    { "PFLocalUserGetLocalIdSize", HandlePFLocalUserGetLocalIdSize },
    { "PFLocalUserGetServiceConfigHandle", HandlePFLocalUserGetServiceConfigHandle },
    { "PFLocalUserHandleCompare", HandlePFLocalUserHandleCompare },
    { "PFLocalUserLoginAsync", HandlePFLocalUserLoginAsync },
    { "PFLocalUserLoginGetResult", HandlePFLocalUserLoginGetResult },
    { "PFLocalUserLoginGetResultSize", HandlePFLocalUserLoginGetResultSize },
    { "PFLocalUserTryGetEntityHandle", HandlePFLocalUserTryGetEntityHandle },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFLocalUserTryGetXUser", HandlePFLocalUserTryGetXUser },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFLocalUserSetCustomContext", HandlePFLocalUserSetCustomContext },
    { "PFMemGetFunctions", HandlePFMemGetFunctions },
    { "PFMemIsUsingCustomMemoryFunctions", HandlePFMemIsUsingCustomMemoryFunctions },
    { "PFMemSetFunctions", HandlePFMemSetFunctions },
#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFPlatformGetGameSaveContext", HandlePFPlatformGetGameSaveContext },
#endif // HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
    { "PFPlatformGetPlatformType", HandlePFPlatformGetPlatformType },
    { "PFPlatformIsGRTSAvailable", HandlePFPlatformIsGRTSAvailable },
    { "PFPlatformLocalStorageSetHandlers", HandlePFPlatformLocalStorageSetHandlers },
    { "PFServiceConfigCloseHandle", HandlePFServiceConfigCloseHandle },
    { "PFServiceConfigCreateHandle", HandlePFServiceConfigCreateHandle },
    { "PFServiceConfigDuplicateHandle", HandlePFServiceConfigDuplicateHandle },
    { "PFServiceConfigGetAPIEndpoint", HandlePFServiceConfigGetAPIEndpoint },
    { "PFServiceConfigGetAPIEndpointSize", HandlePFServiceConfigGetAPIEndpointSize },
    { "PFServiceConfigGetTitleId", HandlePFServiceConfigGetTitleId },
    { "PFServiceConfigGetTitleIdSize", HandlePFServiceConfigGetTitleIdSize },
    { "PFServicesInitialize", HandlePFServicesInitialize },
    { "PFServicesUninitializeAsync", HandlePFServicesUninitializeAsync },
    { "PFSetHttpRetrySettings", HandlePFSetHttpRetrySettings },
    { "PFSetHttpSettings", HandlePFSetHttpSettings },
    { "PFTraceDisableTraceToFile", HandlePFTraceDisableTraceToFile },
    { "PFTraceEnableTraceToFile", HandlePFTraceEnableTraceToFile },
    { "PFUninitializeAsync", HandlePFUninitializeAsync }
});

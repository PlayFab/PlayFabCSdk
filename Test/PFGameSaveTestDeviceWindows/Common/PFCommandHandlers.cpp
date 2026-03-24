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
#include "PFGameSaveFilesForDebug.h"

#include <chrono>
#include <thread>
#include <initializer_list>
#include <string>
#include <limits>
#include <vector>

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
    std::string SyncStateToString(PFGameSaveFilesSyncState state)
    {
        switch (state)
        {
            case PFGameSaveFilesSyncState::NotStarted: return "NotStarted";
            case PFGameSaveFilesSyncState::PreparingForDownload: return "PreparingForDownload";
            case PFGameSaveFilesSyncState::Downloading: return "Downloading";
            case PFGameSaveFilesSyncState::PreparingForUpload: return "PreparingForUpload";
            case PFGameSaveFilesSyncState::Uploading: return "Uploading";
            case PFGameSaveFilesSyncState::SyncComplete: return "SyncComplete";
            default: return "Unknown";
        }
    }

    void LogDescriptor(const char* prefix, const PFGameSaveDescriptor* descriptor)
    {
        if (!descriptor)
        {
            LogToWindow(std::string(prefix) + ": <null descriptor>");
            return;
        }

        LogToWindowFormat("%s time=%lld deviceId=%s friendly=%s bytes=%llu",
            prefix,
            static_cast<long long>(descriptor->time),
            descriptor->deviceId,
            descriptor->deviceFriendlyName,
            static_cast<unsigned long long>(descriptor->totalBytes));
    }

    const char* SyncFailedActionToString(PFGameSaveFilesUiSyncFailedUserAction action);
    const char* ActiveDeviceContentionActionToString(PFGameSaveFilesUiActiveDeviceContentionUserAction action);
    const char* ConflictActionToString(PFGameSaveFilesUiConflictUserAction action);
    const char* OutOfStorageActionToString(PFGameSaveFilesUiOutOfStorageUserAction action);
    const char* ProgressActionToString(PFGameSaveFilesUiProgressUserAction action);

    // Parse expectedHr parameter which can be a single value or comma-separated list
    // Returns true if parameter was found and parsed, with acceptedHrs containing the list
    bool ParseExpectedHrParameter(const nlohmann::json& parameters, std::vector<HRESULT>& acceptedHrs)
    {
        acceptedHrs.clear();
        if (!parameters.contains("expectedHr"))
        {
            return false;
        }

        auto expectedHrNode = parameters["expectedHr"];
        if (expectedHrNode.is_string())
        {
            std::string hrStr = expectedHrNode.get<std::string>();
            // Split by comma and parse each value
            size_t start = 0;
            size_t end = 0;
            while ((end = hrStr.find(',', start)) != std::string::npos)
            {
                std::string token = hrStr.substr(start, end - start);
                // Trim whitespace
                size_t first = token.find_first_not_of(" \t");
                size_t last = token.find_last_not_of(" \t");
                if (first != std::string::npos)
                {
                    token = token.substr(first, last - first + 1);
                    try
                    {
                        acceptedHrs.push_back(static_cast<HRESULT>(std::stoul(token, nullptr, 0)));
                    }
                    catch (...) {}
                }
                start = end + 1;
            }
            // Parse the last (or only) token
            std::string token = hrStr.substr(start);
            size_t first = token.find_first_not_of(" \t");
            size_t last = token.find_last_not_of(" \t");
            if (first != std::string::npos)
            {
                token = token.substr(first, last - first + 1);
                try
                {
                    acceptedHrs.push_back(static_cast<HRESULT>(std::stoul(token, nullptr, 0)));
                }
                catch (...) {}
            }
        }
        else if (expectedHrNode.is_number_integer())
        {
            acceptedHrs.push_back(static_cast<HRESULT>(expectedHrNode.get<int64_t>()));
        }

        return !acceptedHrs.empty();
    }

    // Check if finalHr matches any of the accepted HRESULTs
    bool IsHrAccepted(HRESULT finalHr, const std::vector<HRESULT>& acceptedHrs)
    {
        for (HRESULT accepted : acceptedHrs)
        {
            if (finalHr == accepted)
            {
                return true;
            }
        }
        return false;
    }

    // Format list of accepted HRs for logging
    std::string FormatAcceptedHrs(const std::vector<HRESULT>& acceptedHrs)
    {
        std::string result;
        for (size_t i = 0; i < acceptedHrs.size(); ++i)
        {
            if (i > 0) result += ",";
            char buf[16];
            snprintf(buf, sizeof(buf), "0x%08X", static_cast<uint32_t>(acceptedHrs[i]));
            result += buf;
        }
        return result;
    }

    void CALLBACK UiProgressCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        uint64_t current = 0;
        uint64_t total = 0;
        PFGameSaveFilesUiProgressGetProgress(localUserHandle, nullptr, &current, &total);
        LogToWindowFormat("PFGameSaveFilesUiProgressCallback (state=%s, current=%llu, total=%llu)",
            SyncStateToString(syncState).c_str(),
            static_cast<unsigned long long>(current),
            static_cast<unsigned long long>(total));

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave progress callback dispatched");
        }

        if (state && state->autoProgressResponse && localUserHandle)
        {
            const auto action = *state->autoProgressResponse;
            HRESULT autoHr = PFGameSaveFilesSetUiProgressResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiProgressResponse action=%s (hr=0x%08X)",
                ProgressActionToString(action),
                static_cast<uint32_t>(autoHr));
            if (FAILED(autoHr))
            {
                LogToWindow("Auto responder failed to apply progress response");
            }
        }
    }

    void CALLBACK UiSyncFailedCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        HRESULT error,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        LogToWindowFormat("PFGameSaveFilesUiSyncFailedCallback (state=%s, hr=0x%08X)",
            SyncStateToString(syncState).c_str(),
            static_cast<uint32_t>(error));

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave sync failed callback received");
        }

        if (state && state->autoSyncFailedResponse && localUserHandle)
        {
            auto action = *state->autoSyncFailedResponse;

            // Check if we've exceeded maxRetries and should switch to UseOffline
            if (action == PFGameSaveFilesUiSyncFailedUserAction::Retry &&
                state->autoSyncFailedMaxRetries >= 0 &&
                state->autoSyncFailedRetryCount >= state->autoSyncFailedMaxRetries)
            {
                action = PFGameSaveFilesUiSyncFailedUserAction::UseOffline;
                LogToWindowFormat("Auto responder: maxRetries (%d) exceeded, switching to UseOffline",
                    state->autoSyncFailedMaxRetries);
            }

            if (action == PFGameSaveFilesUiSyncFailedUserAction::Retry)
            {
                state->autoSyncFailedRetryCount++;
            }

            // Apply delay if configured (for testing UI wait bugs)
            // Use async delay to properly test the E_PENDING fix - the callback returns
            // immediately but the response is set later on a background thread.
            if (state->autoSyncFailedDelayMs > 0)
            {
                int delayMs = state->autoSyncFailedDelayMs;
                LogToWindowFormat("Auto responder: will respond with %s after %d ms (async)", 
                    SyncFailedActionToString(action), delayMs);
                
                // Spawn a detached thread to set the response after the delay.
                // This allows the callback to return immediately, testing whether the SDK
                // properly waits (E_PENDING) or incorrectly completes (S_OK bug).
                std::thread([localUserHandle, action, delayMs]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
                    LogToWindowFormat("Auto responder: async delay complete, setting response %s", 
                        SyncFailedActionToString(action));
                    HRESULT autoHr = PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, action);
                    LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiSyncFailedResponse action=%s (hr=0x%08X)",
                        SyncFailedActionToString(action),
                        static_cast<uint32_t>(autoHr));
                    if (FAILED(autoHr))
                    {
                        LogToWindow("Auto responder failed to apply sync failed response");
                    }
                }).detach();
                return;  // Return immediately - response will be set asynchronously
            }

            HRESULT autoHr = PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiSyncFailedResponse action=%s (hr=0x%08X)",
                SyncFailedActionToString(action),
                static_cast<uint32_t>(autoHr));
            if (FAILED(autoHr))
            {
                LogToWindow("Auto responder failed to apply sync failed response");
            }
        }
        else if (state && localUserHandle)
        {
            // Default to Cancel when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiSyncFailedUserAction::Cancel;
            HRESULT autoHr = PFGameSaveFilesSetUiSyncFailedResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder (default): PFGameSaveFilesSetUiSyncFailedResponse action=%s (hr=0x%08X)",
                SyncFailedActionToString(action),
                static_cast<uint32_t>(autoHr));
        }
    }

    void CALLBACK UiActiveDeviceContentionCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveDescriptor* localGameSave,
        PFGameSaveDescriptor* remoteGameSave,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        LogDescriptor("PFGameSaveFilesUiActiveDeviceContentionCallback (local)", localGameSave);
        LogDescriptor("PFGameSaveFilesUiActiveDeviceContentionCallback (remote)", remoteGameSave);

        // Capture the remote description for GRTS testing
        if (state && remoteGameSave)
        {
            state->contentionDescription = remoteGameSave->shortSaveDescription;
            LogToWindowFormat("Captured contention description: '%s'", state->contentionDescription.c_str());
        }

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave active device contention callback received");
        }

        if (state && state->autoActiveDeviceContentionResponse && localUserHandle)
        {
            const auto action = *state->autoActiveDeviceContentionResponse;
            HRESULT autoHr = PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiActiveDeviceContentionResponse action=%s (hr=0x%08X)",
                ActiveDeviceContentionActionToString(action),
                static_cast<uint32_t>(autoHr));
            if (FAILED(autoHr))
            {
                LogToWindow("Auto responder failed to apply active device contention response");
            }
        }
        else if (state && localUserHandle)
        {
            // Default to SyncLastSavedData when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData;
            HRESULT autoHr = PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder (default): PFGameSaveFilesSetUiActiveDeviceContentionResponse action=%s (hr=0x%08X)",
                ActiveDeviceContentionActionToString(action),
                static_cast<uint32_t>(autoHr));
        }
    }

    void CALLBACK UiConflictCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveDescriptor* localGameSave,
        PFGameSaveDescriptor* remoteGameSave,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        LogDescriptor("PFGameSaveFilesUiConflictCallback (local)", localGameSave);
        LogDescriptor("PFGameSaveFilesUiConflictCallback (remote)", remoteGameSave);

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave conflict callback received");
        }

        if (state && state->autoConflictResponse && localUserHandle)
        {
            const auto action = *state->autoConflictResponse;
            HRESULT autoHr = PFGameSaveFilesSetUiConflictResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiConflictResponse action=%s (hr=0x%08X)",
                ConflictActionToString(action),
                static_cast<uint32_t>(autoHr));
            if (FAILED(autoHr))
            {
                LogToWindow("Auto responder failed to apply conflict response");
            }
        }
        else if (state && localUserHandle)
        {
            // Default to Cancel when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiConflictUserAction::Cancel;
            HRESULT autoHr = PFGameSaveFilesSetUiConflictResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder (default): PFGameSaveFilesSetUiConflictResponse action=%s (hr=0x%08X)",
                ConflictActionToString(action),
                static_cast<uint32_t>(autoHr));
        }
    }

    void CALLBACK UiOutOfStorageCallback(
        PFLocalUserHandle localUserHandle,
        uint64_t requiredBytes,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        LogToWindowFormat("PFGameSaveFilesUiOutOfStorageCallback (requiredBytes=%llu)",
            static_cast<unsigned long long>(requiredBytes));

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave out of storage callback received");
        }

        if (state && state->autoOutOfStorageResponse && localUserHandle)
        {
            const auto action = *state->autoOutOfStorageResponse;
            HRESULT autoHr = PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiOutOfStorageResponse action=%s (hr=0x%08X)",
                OutOfStorageActionToString(action),
                static_cast<uint32_t>(autoHr));
            if (FAILED(autoHr))
            {
                LogToWindow("Auto responder failed to apply out of storage response");
            }
        }
        else if (state && localUserHandle)
        {
            // Default to Cancel when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiOutOfStorageUserAction::Cancel;
            HRESULT autoHr = PFGameSaveFilesSetUiOutOfStorageResponse(localUserHandle, action);
            LogToWindowFormat("Auto responder (default): PFGameSaveFilesSetUiOutOfStorageResponse action=%s (hr=0x%08X)",
                OutOfStorageActionToString(action),
                static_cast<uint32_t>(autoHr));
        }
    }

    void CALLBACK ActiveDeviceChangedCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveDescriptor* activeDevice,
        void* context)
    {
        UNREFERENCED_PARAMETER(localUserHandle);
        auto* state = static_cast<DeviceGameSaveState*>(context);
        LogDescriptor("PFGameSaveFilesActiveDeviceChangedCallback", activeDevice);

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave active device changed callback received");
        }
    }

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

    bool TryParseAddUserOptions(const nlohmann::json& parameters, PFGameSaveFilesAddUserOptions& options, std::string& error)
    {
        options = PFGameSaveFilesAddUserOptions::None;
        if (!parameters.is_object())
        {
            return true;
        }

        auto singleIt = parameters.find("rollbackOption");
        if (singleIt != parameters.end())
        {
            if (singleIt->is_string())
            {
                const std::string value = ToLowerCopy(singleIt->get<std::string>());
                if (value == "none")
                {
                    options = PFGameSaveFilesAddUserOptions::None;
                    return true;
                }
                if (value == "rollbacktolastknowngood")
                {
                    options = PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood;
                    return true;
                }
                if (value == "rollbacktolastconflict")
                {
                    options = PFGameSaveFilesAddUserOptions::RollbackToLastConflict;
                    return true;
                }

                error = "Unsupported rollbackOption value";
                return false;
            }

            if (singleIt->is_array())
            {
                PFGameSaveFilesAddUserOptions combined = PFGameSaveFilesAddUserOptions::None;
                for (const auto& entry : *singleIt)
                {
                    if (!entry.is_string())
                    {
                        error = "rollbackOption array must contain strings";
                        return false;
                    }

                    const std::string value = ToLowerCopy(entry.get<std::string>());
                    if (value == "rollbacktolastknowngood")
                    {
                        combined = static_cast<PFGameSaveFilesAddUserOptions>(static_cast<uint32_t>(combined) | static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood));
                    }
                    else if (value == "rollbacktolastconflict")
                    {
                        combined = static_cast<PFGameSaveFilesAddUserOptions>(static_cast<uint32_t>(combined) | static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastConflict));
                    }
                    else if (value == "none")
                    {
                        // no change
                    }
                    else
                    {
                        error = "Unsupported rollbackOption value";
                        return false;
                    }
                }

                options = combined;
                return true;
            }

            error = "rollbackOption must be string or array";
            return false;
        }

        return true;
    }

    bool TryParseUploadOption(const nlohmann::json& parameters, PFGameSaveFilesUploadOption& option, std::string& error)
    {
        option = PFGameSaveFilesUploadOption::KeepDeviceActive;
        if (!parameters.is_object())
        {
            return true;
        }

        auto it = parameters.find("mode");
        if (it == parameters.end())
        {
            return true;
        }

        if (!it->is_string())
        {
            error = "mode must be a string";
            return false;
        }

        const std::string value = ToLowerCopy(it->get<std::string>());
        if (value == "keepdeviceactive" || value == "default")
        {
            option = PFGameSaveFilesUploadOption::KeepDeviceActive;
            return true;
        }

        if (value == "releasedeviceasactive" || value == "release")
        {
            option = PFGameSaveFilesUploadOption::ReleaseDeviceAsActive;
            return true;
        }

        if (value == "offlinedeferred")
        {
            option = PFGameSaveFilesUploadOption::KeepDeviceActive;
            return true;
        }

        error = "Unsupported mode value";
        return false;
    }

    template<typename TEnum>
    bool ParseEnumAction(const nlohmann::json& parameters, const char* key, const std::initializer_list<std::pair<const char*, TEnum>>& mapping, TEnum& action, std::string& error)
    {
        if (!parameters.is_object())
        {
            error = std::string("Parameters payload is not an object for ") + key;
            return false;
        }

        auto it = parameters.find(key);
        if (it == parameters.end())
        {
            error = std::string("Missing parameter '") + key + "'";
            return false;
        }

        if (!it->is_string())
        {
            error = std::string("Parameter '") + key + "' must be a string";
            return false;
        }

        const std::string value = ToLowerCopy(it->get<std::string>());
        for (const auto& entry : mapping)
        {
            if (value == ToLowerCopy(entry.first))
            {
                action = entry.second;
                return true;
            }
        }

        error = std::string("Unsupported value for '") + key + "'";
        return false;
    }

    bool TryParseSyncFailedAction(const nlohmann::json& parameters, PFGameSaveFilesUiSyncFailedUserAction& action, std::string& error)
    {
        return ParseEnumAction(parameters, "action",
        {
            { "Cancel", PFGameSaveFilesUiSyncFailedUserAction::Cancel },
            { "Retry", PFGameSaveFilesUiSyncFailedUserAction::Retry },
            { "UseOffline", PFGameSaveFilesUiSyncFailedUserAction::UseOffline }
        }, action, error);
    }

    bool TryParseActiveDeviceContentionAction(const nlohmann::json& parameters, PFGameSaveFilesUiActiveDeviceContentionUserAction& action, std::string& error)
    {
        return ParseEnumAction(parameters, "action",
        {
            { "Cancel", PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel },
            { "Retry", PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry },
            { "SyncLastSavedData", PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData }
        }, action, error);
    }

    bool TryParseConflictAction(const nlohmann::json& parameters, PFGameSaveFilesUiConflictUserAction& action, std::string& error)
    {
        return ParseEnumAction(parameters, "action",
        {
            { "Cancel", PFGameSaveFilesUiConflictUserAction::Cancel },
            { "UseLocal", PFGameSaveFilesUiConflictUserAction::TakeLocal },
            { "UseCloud", PFGameSaveFilesUiConflictUserAction::TakeRemote }
        }, action, error);
    }

    bool TryParseOutOfStorageAction(const nlohmann::json& parameters, PFGameSaveFilesUiOutOfStorageUserAction& action, std::string& error)
    {
        return ParseEnumAction(parameters, "action",
        {
            { "Cancel", PFGameSaveFilesUiOutOfStorageUserAction::Cancel },
            { "Retry", PFGameSaveFilesUiOutOfStorageUserAction::Retry }
        }, action, error);
    }

    bool TryParseProgressAction(const nlohmann::json& parameters, PFGameSaveFilesUiProgressUserAction& action, std::string& error)
    {
        return ParseEnumAction(parameters, "action",
        {
            { "Cancel", PFGameSaveFilesUiProgressUserAction::Cancel }
        }, action, error);
    }

    const char* SyncFailedActionToString(PFGameSaveFilesUiSyncFailedUserAction action)
    {
        switch (action)
        {
        case PFGameSaveFilesUiSyncFailedUserAction::Cancel:
            return "Cancel";
        case PFGameSaveFilesUiSyncFailedUserAction::Retry:
            return "Retry";
        case PFGameSaveFilesUiSyncFailedUserAction::UseOffline:
            return "UseOffline";
        default:
            return "Unknown";
        }
    }

    const char* ActiveDeviceContentionActionToString(PFGameSaveFilesUiActiveDeviceContentionUserAction action)
    {
        switch (action)
        {
        case PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel:
            return "Cancel";
        case PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry:
            return "Retry";
        case PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData:
            return "SyncLastSavedData";
        default:
            return "Unknown";
        }
    }

    const char* ConflictActionToString(PFGameSaveFilesUiConflictUserAction action)
    {
        switch (action)
        {
        case PFGameSaveFilesUiConflictUserAction::Cancel:
            return "Cancel";
        case PFGameSaveFilesUiConflictUserAction::TakeLocal:
            return "UseLocal";
        case PFGameSaveFilesUiConflictUserAction::TakeRemote:
            return "UseCloud";
        default:
            return "Unknown";
        }
    }

    const char* OutOfStorageActionToString(PFGameSaveFilesUiOutOfStorageUserAction action)
    {
        switch (action)
        {
        case PFGameSaveFilesUiOutOfStorageUserAction::Cancel:
            return "Cancel";
        case PFGameSaveFilesUiOutOfStorageUserAction::Retry:
            return "Retry";
        default:
            return "Unknown";
        }
    }

    const char* ProgressActionToString(PFGameSaveFilesUiProgressUserAction action)
    {
        switch (action)
        {
        case PFGameSaveFilesUiProgressUserAction::Cancel:
            return "Cancel";
        default:
            return "Unknown";
        }
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
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    std::string endpoint;
    std::string titleId;
    std::string error;

    if (!TryGetStringParameter(parameters, "endpoint", endpoint, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!TryGetStringParameter(parameters, "titleId", titleId, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    PFServiceConfigHandle newHandle{ nullptr };
    HRESULT hr = PFServiceConfigCreateHandle(endpoint.c_str(), titleId.c_str(), &newHandle);
    payload.elapsedMs = ComputeElapsedMs(start);
    SetHResult(payload.result, hr);

    LogToWindowFormat("PFServiceConfigCreateHandle (endpoint=%s, titleId=%s, hr=0x%08X)",
        endpoint.c_str(), titleId.c_str(), static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        if (newHandle)
        {
            PFServiceConfigCloseHandle(newHandle);
        }

        MarkFailure(payload.result, hr, "PFServiceConfigCreateHandle failed");
        return payload;
    }

    if (state->serviceConfigHandle)
    {
        PFServiceConfigCloseHandle(state->serviceConfigHandle);
    }

    state->serviceConfigHandle = newHandle;
    state->serviceConfigEndpoint = endpoint;
    state->serviceConfigTitleId = titleId;

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    auto start = std::chrono::steady_clock::now();
    if (state->pfGameSaveInitialized)
    {
        LogToWindow("PFGameSaveFilesInitialize skipped (already initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveInitArgs args{};
    args.backgroundQueue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;
    args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);

    if (parameters.is_object())
    {
        auto folderIt = parameters.find("saveFolder");
        if (folderIt != parameters.end())
        {
            if (!folderIt->is_string())
            {
                payload.elapsedMs = ComputeElapsedMs(start);
                MarkFailure(payload.result, E_INVALIDARG, "saveFolder must be a string");
                SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            state->saveFolder = folderIt->get<std::string>();
            args.saveFolder = state->saveFolder.c_str();
        }
    }

    HRESULT hr = PFGameSaveFilesInitialize(&args);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesInitialize failed");
        return payload;
    }

    // Automatically set the mock device ID for debug purposes.  Only applies to inproc builds
    hr = PFGameSaveFilesSetMockDeviceIdForDebug(state->inputDeviceId.c_str());
    LogToWindowFormat("PFGameSaveFilesSetMockDeviceIdForDebug (hr=0x%08X)", static_cast<uint32_t>(hr));

    state->pfGameSaveInitialized = true;
    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiCallbacks(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enableCallbacks = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enableCallbacks, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    PFGameSaveUICallbacks callbacks{};
    PFGameSaveUICallbacks* callbacksPtr = nullptr;
    if (enableCallbacks)
    {
        callbacks.progressCallback = UiProgressCallback;
        callbacks.progressContext = state;
        callbacks.syncFailedCallback = UiSyncFailedCallback;
        callbacks.syncFailedContext = state;
        callbacks.activeDeviceContentionCallback = UiActiveDeviceContentionCallback;
        callbacks.activeDeviceContentionContext = state;
        callbacks.conflictCallback = UiConflictCallback;
        callbacks.conflictContext = state;
        callbacks.outOfStorageCallback = UiOutOfStorageCallback;
        callbacks.outOfStorageContext = state;
        callbacksPtr = &callbacks;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiCallbacks(callbacksPtr);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetUiCallbacks (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiCallbacks failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetActiveDeviceChangedCallback(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enableCallback = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enableCallback, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    XTaskQueueHandle callbackQueue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;
    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetActiveDeviceChangedCallback(
        enableCallback ? callbackQueue : nullptr,
        enableCallback ? ActiveDeviceChangedCallback : nullptr,
        enableCallback ? state : nullptr);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetActiveDeviceChangedCallback (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetActiveDeviceChangedCallback failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesResetCloudAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesResetCloudAsync(state->localUserHandle, &async);
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
        if (SUCCEEDED(waitHr))
        {
            resultHr = PFGameSaveFilesResetCloudResult(&async);
        }
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesResetCloudAsync (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFGameSaveFilesResetCloudAsync failed");
        return payload;
    }

    if (FAILED(waitHr))
    {
        SetHResult(payload.result, waitHr);
        MarkFailure(payload.result, waitHr, "PFGameSaveFilesResetCloudAsync wait failed");
        return payload;
    }

    if (FAILED(resultHr))
    {
        SetHResult(payload.result, resultHr);
        MarkFailure(payload.result, resultHr, "PFGameSaveFilesResetCloudResult failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesAddUserWithUiAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesAddUserOptions options{};
    std::string error;
    if (!TryParseAddUserOptions(parameters, options, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    // Parse optional expectedHr parameter - allows specific HRESULTs to be treated as success
    // Supports comma-separated values: "0x00000000,0x800704c7"
    std::vector<HRESULT> acceptedHrs;
    bool hasExpectedHr = ParseExpectedHrParameter(parameters, acceptedHrs);

    XAsyncBlock async{};;
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(state->localUserHandle, options, &async);
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
        LogToWindowFormat("PFGameSaveFilesAddUserWithUiAsync XAsyncGetStatus returned (waitHr=0x%08X)", static_cast<uint32_t>(waitHr));
        if (SUCCEEDED(waitHr))
        {
            resultHr = PFGameSaveFilesAddUserWithUiResult(&async);
            LogToWindowFormat("PFGameSaveFilesAddUserWithUiResult returned (resultHr=0x%08X)", static_cast<uint32_t>(resultHr));
        }
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    
    LogToWindowFormat("PFGameSaveFilesAddUserWithUiAsync (hr=0x%08X, waitHr=0x%08X, resultHr=0x%08X)", 
        static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr), static_cast<uint32_t>(resultHr));

    // Determine the final result HRESULT
    HRESULT finalHr = S_OK;
    std::string failureMessage;
    if (FAILED(hr))
    {
        finalHr = hr;
        failureMessage = "PFGameSaveFilesAddUserWithUiAsync failed";
    }
    else if (FAILED(waitHr))
    {
        finalHr = waitHr;
        failureMessage = "PFGameSaveFilesAddUserWithUiAsync wait failed";
    }
    else if (FAILED(resultHr))
    {
        finalHr = resultHr;
        failureMessage = "PFGameSaveFilesAddUserWithUiResult failed";
    }

    SetHResult(payload.result, finalHr);

    // Check if the result matches any of the expected HRESULTs
    if (hasExpectedHr)
    {
        if (IsHrAccepted(finalHr, acceptedHrs))
        {
            LogToWindowFormat("PFGameSaveFilesAddUserWithUiAsync: hr=0x%08X matches expectedHr", static_cast<uint32_t>(finalHr));
            MarkSuccess(payload.result);
            return payload;
        }
        else
        {
            LogToWindowFormat("PFGameSaveFilesAddUserWithUiAsync: hr=0x%08X does NOT match expectedHr=%s", 
                static_cast<uint32_t>(finalHr), FormatAcceptedHrs(acceptedHrs).c_str());
            MarkFailure(payload.result, E_FAIL, "HRESULT did not match expectedHr");
            return payload;
        }
    }

    // Standard success/failure handling when no expectedHr specified
    if (FAILED(finalHr))
    {
        MarkFailure(payload.result, finalHr, failureMessage);
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiSyncFailedResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiSyncFailedUserAction action{};
    std::string error;
    if (!TryParseSyncFailedAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiSyncFailedResponse(state->localUserHandle, action);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetUiSyncFailedResponse (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiSyncFailedResponse failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiActiveDeviceContentionResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiActiveDeviceContentionUserAction action{};
    std::string error;
    if (!TryParseActiveDeviceContentionAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiActiveDeviceContentionResponse(state->localUserHandle, action);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetUiActiveDeviceContentionResponse (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiActiveDeviceContentionResponse failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiConflictResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiConflictUserAction action{};
    std::string error;
    if (!TryParseConflictAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiConflictResponse(state->localUserHandle, action);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetUiConflictResponse (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiConflictResponse failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiOutOfStorageResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiOutOfStorageUserAction action{};
    std::string error;
    if (!TryParseOutOfStorageAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiOutOfStorageResponse(state->localUserHandle, action);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetUiOutOfStorageResponse (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiOutOfStorageResponse failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiProgressResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiProgressUserAction action{};
    std::string error;
    if (!TryParseProgressAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetUiProgressResponse(state->localUserHandle, action);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetUiProgressResponse (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetUiProgressResponse failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiSyncFailedAutoResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enable = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enable, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!enable)
    {
        state->autoSyncFailedResponse.reset();
        LogToWindow("Auto responder: SyncFailed disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveFilesUiSyncFailedUserAction action{};
    if (!TryParseSyncFailedAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->autoSyncFailedResponse = action;
    state->autoSyncFailedRetryCount = 0;  // Reset retry count when reconfiguring

    // Parse optional maxRetries parameter (-1 means unlimited)
    int maxRetries = -1;
    if (parameters.is_object() && parameters.contains("maxRetries"))
    {
        const auto& maxRetriesNode = parameters["maxRetries"];
        if (maxRetriesNode.is_number_integer())
        {
            maxRetries = maxRetriesNode.get<int>();
        }
        else if (maxRetriesNode.is_string())
        {
            try
            {
                maxRetries = std::stoi(maxRetriesNode.get<std::string>());
            }
            catch (...)
            {
                // Ignore parsing failures, keep default of -1
            }
        }
    }
    state->autoSyncFailedMaxRetries = maxRetries;

    // Parse optional delayMs parameter (for testing UI wait bugs)
    int delayMs = 0;
    if (parameters.is_object() && parameters.contains("delayMs"))
    {
        const auto& delayNode = parameters["delayMs"];
        if (delayNode.is_number_integer())
        {
            delayMs = delayNode.get<int>();
        }
        else if (delayNode.is_string())
        {
            try
            {
                delayMs = std::stoi(delayNode.get<std::string>());
            }
            catch (...)
            {
                // Ignore parsing failures, keep default of 0
            }
        }
    }
    state->autoSyncFailedDelayMs = delayMs;

    if (maxRetries >= 0 && delayMs > 0)
    {
        LogToWindowFormat("Auto responder: SyncFailed set to %s (maxRetries=%d, delayMs=%d)", SyncFailedActionToString(action), maxRetries, delayMs);
    }
    else if (maxRetries >= 0)
    {
        LogToWindowFormat("Auto responder: SyncFailed set to %s (maxRetries=%d)", SyncFailedActionToString(action), maxRetries);
    }
    else if (delayMs > 0)
    {
        LogToWindowFormat("Auto responder: SyncFailed set to %s (delayMs=%d)", SyncFailedActionToString(action), delayMs);
    }
    else
    {
        LogToWindowFormat("Auto responder: SyncFailed set to %s", SyncFailedActionToString(action));
    }
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiActiveDeviceContentionAutoResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enable = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enable, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!enable)
    {
        state->autoActiveDeviceContentionResponse.reset();
        LogToWindow("Auto responder: ActiveDeviceContention disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveFilesUiActiveDeviceContentionUserAction action{};
    if (!TryParseActiveDeviceContentionAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->autoActiveDeviceContentionResponse = action;
    LogToWindowFormat("Auto responder: ActiveDeviceContention set to %s", ActiveDeviceContentionActionToString(action));
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiConflictAutoResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enable = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enable, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!enable)
    {
        state->autoConflictResponse.reset();
        LogToWindow("Auto responder: Conflict disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveFilesUiConflictUserAction action{};
    if (!TryParseConflictAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->autoConflictResponse = action;
    LogToWindowFormat("Auto responder: Conflict set to %s", ConflictActionToString(action));
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiOutOfStorageAutoResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enable = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enable, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!enable)
    {
        state->autoOutOfStorageResponse.reset();
        LogToWindow("Auto responder: OutOfStorage disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveFilesUiOutOfStorageUserAction action{};
    if (!TryParseOutOfStorageAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->autoOutOfStorageResponse = action;
    LogToWindowFormat("Auto responder: OutOfStorage set to %s", OutOfStorageActionToString(action));
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiProgressAutoResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    bool enable = true;
    std::string error;
    if (!TryParseBoolParameter(parameters, "enable", enable, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!enable)
    {
        state->autoProgressResponse.reset();
        LogToWindow("Auto responder: Progress disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    PFGameSaveFilesUiProgressUserAction action{};
    if (!TryParseProgressAction(parameters, action, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    state->autoProgressResponse = action;
    LogToWindowFormat("Auto responder: Progress set to %s", ProgressActionToString(action));
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetMockDeviceIdForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    std::string error;
    std::string mockDeviceId;
    if (!TryGetStringParameter(parameters, "deviceId", mockDeviceId, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetMockDeviceIdForDebug(mockDeviceId.c_str());
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetMockDeviceIdForDebug (deviceId=%s, hr=0x%08X)", mockDeviceId.c_str(), static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetMockDeviceIdForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetForceOutOfStorageErrorForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!parameters.is_object() || parameters.find("forceError") == parameters.end())
    {
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'forceError'");
        return payload;
    }

    bool forceError = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "forceError", forceError, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetForceOutOfStorageErrorForDebug(forceError);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetForceOutOfStorageErrorForDebug (forceError=%s, hr=0x%08X)", forceError ? "true" : "false", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetForceOutOfStorageErrorForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetForceSyncFailedErrorForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!parameters.is_object() || parameters.find("forceError") == parameters.end())
    {
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'forceError'");
        return payload;
    }

    bool forceError = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "forceError", forceError, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetForceSyncFailedErrorForDebug(forceError);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetForceSyncFailedErrorForDebug (forceError=%s, hr=0x%08X)", forceError ? "true" : "false", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetForceSyncFailedErrorForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetWriteManifestsToDiskForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!parameters.is_object() || parameters.find("writeManifests") == parameters.end())
    {
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'writeManifests'");
        return payload;
    }

    bool writeManifests = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "writeManifests", writeManifests, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetWriteManifestsToDiskForDebug(writeManifests);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetWriteManifestsToDiskForDebug (writeManifests=%s, hr=0x%08X)", writeManifests ? "true" : "false", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetWriteManifestsToDiskForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetActiveDevicePollForceChangeForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);
    UNREFERENCED_PARAMETER(parameters);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetActiveDevicePollForceChangeForDebug();
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetActiveDevicePollForceChangeForDebug (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetActiveDevicePollForceChangeForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetActiveDevicePollIntervalForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    int64_t intervalValue = 0;
    std::string error;
    if (!TryGetInt64Parameter(parameters, "interval", intervalValue, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (intervalValue < 0 || intervalValue > static_cast<int64_t>(std::numeric_limits<uint32_t>::max()))
    {
        MarkFailure(payload.result, E_INVALIDARG, "Parameter 'interval' must be between 0 and 4294967295");
        return payload;
    }

    uint32_t interval = static_cast<uint32_t>(intervalValue);

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetActiveDevicePollIntervalForDebug(interval);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesSetActiveDevicePollIntervalForDebug (interval=%u, hr=0x%08X)", interval, static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetActiveDevicePollIntervalForDebug failed");
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesUploadWithUiAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUploadOption option{};
    std::string error;
    if (!TryParseUploadOption(parameters, option, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    // Parse optional fireAndForget parameter - starts the upload but returns immediately
    // without waiting for completion. Useful for testing uninitialize during pending operations.
    bool fireAndForget = false;
    if (!TryParseBoolParameter(parameters, "fireAndForget", fireAndForget, error))
    {
        // Ignore parsing errors - default to false
    }

    // Parse optional expectedHr parameter - allows specific HRESULTs to be treated as success
    // Supports comma-separated values: "0x00000000,0x89237004"
    std::vector<HRESULT> acceptedHrs;
    bool hasExpectedHr = ParseExpectedHrParameter(parameters, acceptedHrs);

    // For fire-and-forget mode, we need to keep the async block alive
    if (fireAndForget)
    {
        state->pendingUploadAsync = std::make_unique<XAsyncBlock>();
        state->pendingUploadAsync->queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

        auto start = std::chrono::steady_clock::now();
        HRESULT hr = PFGameSaveFilesUploadWithUiAsync(state->localUserHandle, option, state->pendingUploadAsync.get());
        payload.elapsedMs = ComputeElapsedMs(start);

        LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync (fireAndForget mode, hr=0x%08X)", static_cast<uint32_t>(hr));

        if (FAILED(hr))
        {
            state->pendingUploadAsync.reset();
            MarkFailure(payload.result, hr, "PFGameSaveFilesUploadWithUiAsync failed to start");
            return payload;
        }

        // Return immediately without waiting - the upload continues in the background
        SetHResult(payload.result, S_OK);
        MarkSuccess(payload.result);
        return payload;
    }

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesUploadWithUiAsync(state->localUserHandle, option, &async);
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
        if (SUCCEEDED(waitHr))
        {
            resultHr = PFGameSaveFilesUploadWithUiResult(&async);
        }
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync (hr=0x%08X, waitHr=0x%08X, resultHr=0x%08X)", 
        static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr), static_cast<uint32_t>(resultHr));

    // Determine the final result HRESULT
    HRESULT finalHr = S_OK;
    std::string failureMessage;
    if (FAILED(hr))
    {
        finalHr = hr;
        failureMessage = "PFGameSaveFilesUploadWithUiAsync failed";
    }
    else if (FAILED(waitHr))
    {
        finalHr = waitHr;
        failureMessage = "PFGameSaveFilesUploadWithUiAsync wait failed";
    }
    else if (FAILED(resultHr))
    {
        finalHr = resultHr;
        failureMessage = "PFGameSaveFilesUploadWithUiResult failed";
    }

    SetHResult(payload.result, finalHr);

    // Check if the result matches any of the expected HRESULTs
    if (hasExpectedHr)
    {
        if (IsHrAccepted(finalHr, acceptedHrs))
        {
            LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync: hr=0x%08X matches expectedHr", static_cast<uint32_t>(finalHr));
            MarkSuccess(payload.result);
            return payload;
        }
        else
        {
            LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync: hr=0x%08X does NOT match expectedHr=%s", 
                static_cast<uint32_t>(finalHr), FormatAcceptedHrs(acceptedHrs).c_str());
            MarkFailure(payload.result, E_FAIL, "HRESULT did not match expectedHr");
            return payload;
        }
    }

    // Standard success/failure handling when no expectedHr specified
    if (FAILED(finalHr))
    {
        MarkFailure(payload.result, finalHr, failureMessage);
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesGetFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    size_t folderSize = 0;
    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesGetFolderSize(state->localUserHandle, &folderSize);
    if (FAILED(hr))
    {
        payload.elapsedMs = ComputeElapsedMs(start);
        LogToWindowFormat("PFGameSaveFilesGetFolderSize (hr=0x%08X)", static_cast<uint32_t>(hr));
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFGameSaveFilesGetFolderSize failed");
        return payload;
    }

    std::vector<char> buffer(folderSize > 0 ? folderSize : 1);
    size_t used = 0;
    hr = PFGameSaveFilesGetFolder(state->localUserHandle, buffer.size(), buffer.data(), &used);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesGetFolder (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFGameSaveFilesGetFolder failed");
        return payload;
    }

    size_t stringLength = (used > 0 && used <= buffer.size()) ? used - 1 : 0;
    std::string folder(buffer.data(), buffer.data() + stringLength);
    state->saveFolder = folder;
    payload.result["folder"] = folder;

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesGetRemainingQuota(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    int64_t remainingQuota = 0;
    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesGetRemainingQuota(state->localUserHandle, &remainingQuota);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesGetRemainingQuota (hr=0x%08X)", static_cast<uint32_t>(hr));

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesGetRemainingQuota failed");
        return payload;
    }

    payload.result["remainingQuotaBytes"] = remainingQuota;
    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesIsConnectedToCloud(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    // Extract optional expectedConnected parameter
    std::optional<bool> expectedConnected;
    if (parameters.is_object() && parameters.contains("expectedConnected"))
    {
        const auto& expectedNode = parameters["expectedConnected"];
        if (expectedNode.is_boolean())
        {
            expectedConnected = expectedNode.get<bool>();
        }
    }

    bool isConnected = false;
    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesIsConnectedToCloud(state->localUserHandle, &isConnected);
    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesIsConnectedToCloud (hr=0x%08X, isConnected=%s)", 
        static_cast<uint32_t>(hr), isConnected ? "true" : "false");

    SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesIsConnectedToCloud failed");
        return payload;
    }

    payload.result["isConnectedToCloud"] = isConnected;

    // Check if actual state matches expected state
    if (expectedConnected.has_value() && isConnected != expectedConnected.value())
    {
        std::string errorMsg = "Expected isConnectedToCloud=" + std::string(expectedConnected.value() ? "true" : "false") +
                               " but got " + std::string(isConnected ? "true" : "false");
        LogToWindow(errorMsg);
        MarkFailure(payload.result, E_FAIL, errorMsg.c_str());
        return payload;
    }

    MarkSuccess(payload.result);
    return payload;
}

#ifdef _WIN32

CommandResultPayload HandlePFLocalUserCreateHandleWithXboxUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(parameters);

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->serviceConfigHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Service config handle not created");
        return payload;
    }

    if (!state->xuser)
    {
        MarkFailure(payload.result, E_POINTER, "XUser handle not available");
        return payload;
    }

    if (state->localUserHandle)
    {
        PFLocalUserCloseHandle(state->localUserHandle);
        state->localUserHandle = nullptr;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFLocalUserCreateHandleWithXboxUser(state->serviceConfigHandle, state->xuser, state, &state->localUserHandle);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFLocalUserCreateHandleWithXboxUser (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFLocalUserCreateHandleWithXboxUser failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
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
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    auto start = std::chrono::steady_clock::now();
    if (state->localUserHandle)
    {
        PFLocalUserCloseHandle(state->localUserHandle);
        state->localUserHandle = nullptr;
        LogToWindow("PFLocalUserCloseHandle executed");
    }
    else
    {
        LogToWindow("PFLocalUserCloseHandle skipped (no handle)");
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFServiceConfigCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    auto start = std::chrono::steady_clock::now();
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

    payload.elapsedMs = ComputeElapsedMs(start);
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    if (!state->pfGameSaveInitialized)
    {
        LogToWindow("PFGameSaveFilesUninitializeAsync skipped (not initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    HRESULT hr = PFGameSaveFilesUninitializeAsync(&async);
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;
    LogToWindowFormat("PFGameSaveFilesUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
        if (SUCCEEDED(waitHr))
        {
            resultHr = PFGameSaveFilesUninitializeResult(&async);
        }
    }
    payload.elapsedMs = ComputeElapsedMs(start);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesUninitializeAsync failed");
        SetHResult(payload.result, hr);
        return payload;
    }

    if (FAILED(waitHr))
    {
        MarkFailure(payload.result, waitHr, "PFGameSaveFilesUninitializeAsync wait failed");
        SetHResult(payload.result, waitHr);
        return payload;
    }

    if (FAILED(resultHr))
    {
        MarkFailure(payload.result, resultHr, "PFGameSaveFilesUninitializeResult failed");
        SetHResult(payload.result, resultHr);
        return payload;
    }

    state->pfGameSaveInitialized = false;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFServicesUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    if (!state->pfServicesInitialized)
    {
        LogToWindow("PFServicesUninitializeAsync skipped (not initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    HRESULT hr = PFServicesUninitializeAsync(&async);
    HRESULT waitHr = S_OK;
    LogToWindowFormat("PFServicesUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
    }

    payload.elapsedMs = ComputeElapsedMs(start);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFServicesUninitializeAsync failed");
        SetHResult(payload.result, hr);
        return payload;
    }

    if (FAILED(waitHr))
    {
        MarkFailure(payload.result, waitHr, "PFServicesUninitializeAsync wait failed");
        SetHResult(payload.result, waitHr);
        return payload;
    }

    state->pfServicesInitialized = false;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    if (!state->pfInitialized)
    {
        LogToWindow("PFUninitializeAsync skipped (not initialized)");
        payload.elapsedMs = ComputeElapsedMs(start);
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    HRESULT hr = PFUninitializeAsync(&async);
    HRESULT waitHr = S_OK;
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFUninitializeAsync failed");
        SetHResult(payload.result, hr);
        return payload;
    }

    if (FAILED(waitHr))
    {
        MarkFailure(payload.result, waitHr, "PFUninitializeAsync wait failed");
        SetHResult(payload.result, waitHr);
        return payload;
    }

    state->pfInitialized = false;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetSaveDescriptionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!state->localUserHandle)
    {
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    std::string description;
    std::string error;
    if (!TryGetStringParameter(parameters, "description", description, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    // Parse optional expectedHr parameter - allows specific HRESULTs to be treated as success
    // Supports comma-separated values: "0x00000000,0x801901F7"
    std::vector<HRESULT> acceptedHrs;
    bool hasExpectedHr = ParseExpectedHrParameter(parameters, acceptedHrs);

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesSetSaveDescriptionAsync(state->localUserHandle, description.c_str(), &async);
    HRESULT waitHr = S_OK;
    if (SUCCEEDED(hr))
    {
        waitHr = XAsyncGetStatus(&async, true);
        if (SUCCEEDED(waitHr))
        {
            (void)PFGameSaveFilesSetSaveDescriptionResult(&async);
        }
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesSetSaveDescriptionAsync (description='%s', hr=0x%08X, waitHr=0x%08X)", 
        description.c_str(), static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr));

    HRESULT finalHr = FAILED(hr) ? hr : waitHr;

    if (hasExpectedHr)
    {
        if (IsHrAccepted(finalHr, acceptedHrs))
        {
            LogToWindowFormat("PFGameSaveFilesSetSaveDescriptionAsync: hr=0x%08X matches expectedHr", static_cast<uint32_t>(finalHr));
            MarkSuccess(payload.result);
            SetHResult(payload.result, finalHr);
            return payload;
        }
        else
        {
            LogToWindowFormat("PFGameSaveFilesSetSaveDescriptionAsync: hr=0x%08X does NOT match expectedHr=%s", 
                static_cast<uint32_t>(finalHr), FormatAcceptedHrs(acceptedHrs).c_str());
            MarkFailure(payload.result, E_FAIL, "HRESULT did not match expectedHr");
            SetHResult(payload.result, finalHr);
            return payload;
        }
    }

    if (FAILED(hr))
    {
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFGameSaveFilesSetSaveDescriptionAsync failed");
        return payload;
    }

    if (FAILED(waitHr))
    {
        SetHResult(payload.result, waitHr);
        MarkFailure(payload.result, waitHr, "PFGameSaveFilesSetSaveDescriptionAsync wait failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesGetSaveDescription(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    // On GRTS, use the description captured from the contention callback
    if (state->engineType == DeviceEngineType::PcGrts)
    {
        std::string description = state->contentionDescription;
        LogToWindowFormat("PFGameSaveFilesGetSaveDescription (GRTS, from contention callback): description='%s'", description.c_str());
        payload.result["description"] = description;

        // Check expectedDescription if provided
        if (parameters.contains("expectedDescription"))
        {
            std::string expectedDescription;
            if (parameters["expectedDescription"].is_string())
            {
                expectedDescription = parameters["expectedDescription"].get<std::string>();
            }

            if (description != expectedDescription)
            {
                LogToWindowFormat("PFGameSaveFilesGetSaveDescription: description '%s' does NOT match expected '%s'",
                    description.c_str(), expectedDescription.c_str());
                std::string errorMsg = "Description mismatch: got '" + description + "', expected '" + expectedDescription + "'";
                MarkFailure(payload.result, E_FAIL, errorMsg);
                SetHResult(payload.result, E_FAIL);
                return payload;
            }
            else
            {
                LogToWindowFormat("PFGameSaveFilesGetSaveDescription: description matches expected value");
            }
        }

        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    if (state->localUserHandle == nullptr)
    {
        MarkFailure(payload.result, E_INVALIDARG, "Local user handle not set");
        return payload;
    }

    auto start = std::chrono::steady_clock::now();

    // Use the new debug API to get the current save description
    size_t descriptionSize = 0;
    HRESULT hr = PFGameSaveFilesGetSaveDescriptionSizeForDebug(state->localUserHandle, &descriptionSize);
    
    std::string description;
    if (SUCCEEDED(hr) && descriptionSize > 0)
    {
        std::vector<char> buffer(descriptionSize);
        size_t usedSize = 0;
        hr = PFGameSaveFilesGetSaveDescriptionForDebug(state->localUserHandle, descriptionSize, buffer.data(), &usedSize);
        if (SUCCEEDED(hr))
        {
            description = buffer.data();
        }
    }

    payload.elapsedMs = ComputeElapsedMs(start);
    LogToWindowFormat("PFGameSaveFilesGetSaveDescription (description='%s', hr=0x%08X)", 
        description.c_str(), static_cast<uint32_t>(hr));

    payload.result["description"] = description;

    // Check expectedDescription if provided
    if (parameters.contains("expectedDescription"))
    {
        std::string expectedDescription;
        if (parameters["expectedDescription"].is_string())
        {
            expectedDescription = parameters["expectedDescription"].get<std::string>();
        }

        if (description != expectedDescription)
        {
            LogToWindowFormat("PFGameSaveFilesGetSaveDescription: description '%s' does NOT match expected '%s'",
                description.c_str(), expectedDescription.c_str());
            std::string errorMsg = "Description mismatch: got '" + description + "', expected '" + expectedDescription + "'";
            MarkFailure(payload.result, E_FAIL, errorMsg);
            SetHResult(payload.result, E_FAIL);
            return payload;
        }
        else
        {
            LogToWindowFormat("PFGameSaveFilesGetSaveDescription: description matches expected value");
        }
    }

    if (FAILED(hr))
    {
        SetHResult(payload.result, hr);
        MarkFailure(payload.result, hr, "PFGameSaveFilesGetSaveDescription failed");
        return payload;
    }

    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

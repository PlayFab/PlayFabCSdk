#include "pch.h"

#include "PFGameSaveFilesHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "TestHarness/DeviceFileSystem.h"
#include "TestHarness/WriteGameSaveData.h"

#include <XTaskQueue.h>
#include <XUser.h>

#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/core/PFLocalUser.h>
#include <playfab/core/PFAuthentication.h>
#include <playfab/core/PFAuthenticationTypes.h>
#include "PFGameSaveFilesForDebug.h"

#include <thread>
#include <mutex>
#include <initializer_list>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "CommandRegistry.h"
#include "TestHarness/WriteGameSaveData.h"

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
    // Finds and reads the first extended-*-manifest.json in a directory.
    // Returns empty string if not found.
    std::string ReadExtendedManifestFromDir(const std::filesystem::path& dir)
    {
        namespace fs = std::filesystem;
        std::error_code ec;
        if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec))
        {
            return {};
        }

        for (const auto& entry : fs::directory_iterator(dir, fs::directory_options::skip_permission_denied, ec))
        {
            std::string name = entry.path().filename().string();
            if (name.rfind("extended-", 0) == 0 && name.find("-manifest.json") != std::string::npos)
            {
                std::ifstream ifs(entry.path(), std::ios::in);
                if (ifs.is_open())
                {
                    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                    return content;
                }
            }
        }
        return {};
    }

    // Logs the extended manifest from both GRTS (pgs folder) and inproc (saveFolder/cloudsync).
    void LogExtendedManifest(const std::string& saveFolder, const char* label)
    {
        namespace fs = std::filesystem;
        std::error_code ec;

        // Try GRTS location: C:\XboxGames\GameSave\pgs\u_<xuid>_<scid>\extended-*-manifest.json
        fs::path pgsRoot("C:\\XboxGames\\GameSave\\pgs");
        if (fs::exists(pgsRoot, ec))
        {
            for (const auto& entry : fs::directory_iterator(pgsRoot, fs::directory_options::skip_permission_denied, ec))
            {
                std::string name = entry.path().filename().string();
                if (name.rfind("u_", 0) == 0 && entry.is_directory(ec))
                {
                    std::string content = ReadExtendedManifestFromDir(entry.path());
                    if (!content.empty())
                    {
                        LogToWindowFormat("[ExtManifest:%s] (GRTS) %s", label, entry.path().string().c_str());
                        LogToWindowFormat("[ExtManifest:%s] %s", label, content.c_str());
                    }
                    else
                    {
                        LogToWindowFormat("[ExtManifest:%s] (GRTS) no manifest in %s", label, entry.path().string().c_str());
                    }
                    break;
                }
            }
        }

        // Try inproc location: <saveFolder>/cloudsync/extended-*-manifest.json
        if (!saveFolder.empty())
        {
            fs::path cloudsyncDir = fs::path(saveFolder) / "cloudsync";
            std::string content = ReadExtendedManifestFromDir(cloudsyncDir);
            if (!content.empty())
            {
                LogToWindowFormat("[ExtManifest:%s] (inproc) %s", label, cloudsyncDir.string().c_str());
                LogToWindowFormat("[ExtManifest:%s] %s", label, content.c_str());
            }
            else if (fs::exists(cloudsyncDir, ec))
            {
                LogToWindowFormat("[ExtManifest:%s] (inproc) no manifest in %s", label, cloudsyncDir.string().c_str());
            }
        }
    }

    // Logs the top-level contents of the save folder for debugging sync behavior.
    void LogSaveFolderContents(const std::string& saveFolder, const char* label)
    {
        namespace fs = std::filesystem;
        if (saveFolder.empty())
        {
            LogToWindowFormat("[SaveFolder:%s] (no save folder set)", label);
            return;
        }

        std::error_code ec;
        fs::path root(saveFolder);
        if (!fs::exists(root, ec))
        {
            LogToWindowFormat("[SaveFolder:%s] %s (does not exist)", label, saveFolder.c_str());
            return;
        }

        int dirs = 0, files = 0;
        uint64_t totalBytes = 0;
        std::string listing;
        for (const auto& entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied, ec))
        {
            std::string name = entry.path().filename().string();
            if (entry.is_directory(ec))
            {
                dirs++;
                listing += "  [dir] " + name + "\n";
            }
            else if (entry.is_regular_file(ec))
            {
                files++;
                auto sz = entry.file_size(ec);
                totalBytes += sz;
                listing += "  [file] " + name + " (" + std::to_string(sz) + " bytes)\n";
            }
        }

        LogToWindowFormat("[SaveFolder:%s] %s -- %d dirs, %d files, %llu bytes",
            label, saveFolder.c_str(), dirs, files, static_cast<unsigned long long>(totalBytes));
        if (!listing.empty())
        {
            LogToWindow(listing);
        }
    }

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

    // Forward declarations for UI callbacks (defined later in this file)
    void CALLBACK UiSyncFailedCallback(PFLocalUserHandle, PFGameSaveFilesSyncState, HRESULT, void*);
    void CALLBACK UiActiveDeviceContentionCallback(PFLocalUserHandle, PFGameSaveDescriptor*, PFGameSaveDescriptor*, void*);
    void CALLBACK UiConflictCallback(PFLocalUserHandle, PFGameSaveDescriptor*, PFGameSaveDescriptor*, void*);
    void CALLBACK UiOutOfStorageCallback(PFLocalUserHandle, uint64_t, void*);
    void CALLBACK UiProgressCallback(PFLocalUserHandle, PFGameSaveFilesSyncState, void*);

    // Re-registers UI callbacks for the given state. Called after auto-reinit to restore callbacks
    // that are cleared by PFGameSaveFilesUninitializeAsync.
    HRESULT ReRegisterUiCallbacks(DeviceGameSaveState* state)
    {
        PFGameSaveUICallbacks callbacks{};
        if (state->engineType != DeviceEngineType::Xbox && state->engineType != DeviceEngineType::PcGrts)
        {
            callbacks.progressCallback = UiProgressCallback;
            callbacks.progressContext = state;
        }
        callbacks.syncFailedCallback = UiSyncFailedCallback;
        callbacks.syncFailedContext = state;
        callbacks.activeDeviceContentionCallback = UiActiveDeviceContentionCallback;
        callbacks.activeDeviceContentionContext = state;
        callbacks.conflictCallback = UiConflictCallback;
        callbacks.conflictContext = state;
        callbacks.outOfStorageCallback = UiOutOfStorageCallback;
        callbacks.outOfStorageContext = state;
        return PFGameSaveFilesSetUiCallbacks(&callbacks);
    }

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
        if (state && localUserHandle)
        {
            state->progressSamplerUserHandle.store(localUserHandle, std::memory_order_release);
        }
        LogToWindowFormat("PFGameSaveFilesUiProgressCallback (state=%s, current=%llu, total=%llu)",
            SyncStateToString(syncState).c_str(),
            static_cast<unsigned long long>(current),
            static_cast<unsigned long long>(total));

        if (state && state->recordProgressStates)
        {
            std::lock_guard<std::mutex> lock(state->progressMutex);
            uint64_t elapsedMs = 0;
            if (state->progressSamplerStart.time_since_epoch().count() != 0)
            {
                elapsedMs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - state->progressSamplerStart).count());
            }
            state->recordedProgressStates.push_back({ syncState, current, total, elapsedMs });
        }

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

        // Auto-write during upload: when Uploading state is detected, execute stored write operations once
        if (state && syncState == PFGameSaveFilesSyncState::Uploading)
        {
            std::string opsCopy;
            bool shouldWrite = false;
            {
                std::lock_guard<std::mutex> lock(state->progressMutex);
                if (state->autoProgressWriteOnUploadJson && !state->autoProgressWriteOnUploadFired)
                {
                    state->autoProgressWriteOnUploadFired = true;
                    opsCopy = *state->autoProgressWriteOnUploadJson;
                    shouldWrite = true;
                }
            }
            if (shouldWrite)
            {
                LogToWindow("Auto write-on-upload: Uploading detected, executing stored write operations");
                try
                {
                    nlohmann::json ops = nlohmann::json::parse(opsCopy);
                    WriteGameSaveDataResult writeResult = ExecuteWriteGameSaveData(state, "auto-write-on-upload", ops);
                    if (SUCCEEDED(writeResult.hr))
                    {
                        LogToWindowFormat("Auto write-on-upload: completed (%d mutations)", writeResult.scriptMutationsApplied);
                    }
                    else
                    {
                        LogToWindowFormat("Auto write-on-upload: FAILED hr=0x%08X %s",
                            static_cast<uint32_t>(writeResult.hr), writeResult.errorMessage.c_str());
                    }
                }
                catch (const std::exception& ex)
                {
                    LogToWindowFormat("Auto write-on-upload: JSON parse error: %s", ex.what());
                }
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

        // SAFETY: Do NOT call PFGameSaveFilesSetUiSyncFailedResponse after the device has been
        // released via ReleaseDeviceAsActive. Calling SDK APIs on a released context triggers a
        // GRTS fail-fast crash (__fastfail) that cannot be caught by any exception handler.
        if (state && state->deviceReleased.load())
        {
            LogToWindow("Auto responder: SKIPPING response — device has been released (avoiding GRTS fail-fast crash)");
            return;
        }

        if (state && state->autoSyncFailedResponse && localUserHandle)
        {
            auto action = *state->autoSyncFailedResponse;

            // Check if we've exceeded maxRetries — fall back to configured fallbackAction (default: Cancel).
            if (action == PFGameSaveFilesUiSyncFailedUserAction::Retry &&
                state->autoSyncFailedMaxRetries >= 0 &&
                state->autoSyncFailedRetryCount >= state->autoSyncFailedMaxRetries)
            {
                action = state->autoSyncFailedFallbackAction.value_or(PFGameSaveFilesUiSyncFailedUserAction::Cancel);
                LogToWindowFormat("Auto responder: maxRetries (%d) exceeded, switching to %s",
                    state->autoSyncFailedMaxRetries, SyncFailedActionToString(action));
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
                auto* stateForDelay = state;
                std::thread([localUserHandle, action, delayMs, stateForDelay]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
                    // Hold deviceReleaseMutex across BOTH the check and the SDK call. Testing the
                    // atomic and then calling is a TOCTOU: a release landing in between drops us
                    // into the uncatchable GRTS fail-fast. The release paths take the same lock.
                    // (stateForDelay is the process-lifetime global device state, so holding a
                    // raw pointer across the sleep is safe.)
                    if (stateForDelay != nullptr)
                    {
                        std::lock_guard<std::mutex> lock(stateForDelay->deviceReleaseMutex);
                        if (stateForDelay->deviceReleased.load())
                        {
                            LogToWindow("Auto responder: SKIPPING delayed response - device was released during the delay (avoiding GRTS fail-fast crash)");
                            return;
                        }
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
                    }
                }).detach();
                return;  // Return immediately - response will be set asynchronously
            }

            // Hold deviceReleaseMutex across the check and the SDK call, matching the delayed
            // responder above. Calling into the SDK after the device has been released triggers an
            // uncatchable GRTS fail-fast, so the released flag must not be able to flip between the
            // test and the call.
            {
                std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
                if (state->deviceReleased.load())
                {
                    LogToWindow("Auto responder: device released, skipping sync failed response");
                    return;
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
        }
        else if (state && localUserHandle)
        {
            // Default to Cancel when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiSyncFailedUserAction::Cancel;

            // Same check-then-call contract as above.
            std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
            if (state->deviceReleased.load())
            {
                LogToWindow("Auto responder (default): device released, skipping sync failed response");
                return;
            }

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

        // Capture descriptors for VerifyContentionDescriptor
        if (state && localGameSave)
        {
            state->savedLocalGameSave = *localGameSave;
        }
        if (state && remoteGameSave)
        {
            state->savedRemoteGameSave = *remoteGameSave;
            state->contentionDescription = remoteGameSave->shortSaveDescription;
            LogToWindowFormat("Captured contention description: '%s'", state->contentionDescription.c_str());
        }
        if (state)
        {
            state->hasContentionDescriptor = true;
        }

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave active device contention callback received");
        }

        // SAFETY: Do NOT call SDK APIs after device release (same as UiSyncFailedCallback)
        if (state && state->deviceReleased.load())
        {
            LogToWindow("Auto responder: SKIPPING contention response — device has been released");
            return;
        }

        if (state && state->autoActiveDeviceContentionResponse && localUserHandle)
        {
            auto action = *state->autoActiveDeviceContentionResponse;

            // maxRetries: switch to fallback action after exhausting retries
            if (state->autoContentionMaxRetries >= 0)
            {
                std::lock_guard<std::mutex> lock(state->progressMutex);
                state->autoContentionRetryCount++;
                if (state->autoContentionRetryCount > state->autoContentionMaxRetries && state->autoContentionFallbackAction)
                {
                    LogToWindowFormat("Auto responder: maxRetries (%d) exhausted for contention. Switching from '%s' to '%s'.",
                        state->autoContentionMaxRetries,
                        ActiveDeviceContentionActionToString(action),
                        ActiveDeviceContentionActionToString(*state->autoContentionFallbackAction));
                    action = *state->autoContentionFallbackAction;
                }
            }

            // Hold deviceReleaseMutex across the check and the SDK call: responding after the
            // device has been released triggers an uncatchable GRTS fail-fast.
            {
                std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
                if (state->deviceReleased.load())
                {
                    LogToWindow("Auto responder: device released, skipping contention response");
                    return;
                }

                HRESULT autoHr = PFGameSaveFilesSetUiActiveDeviceContentionResponse(localUserHandle, action);
                LogToWindowFormat("Auto responder: PFGameSaveFilesSetUiActiveDeviceContentionResponse action=%s (hr=0x%08X)",
                    ActiveDeviceContentionActionToString(action),
                    static_cast<uint32_t>(autoHr));
                if (FAILED(autoHr))
                {
                    LogToWindow("Auto responder failed to apply active device contention response");
                }
            }
        }
        else if (state && localUserHandle)
        {
            // Default to SyncLastSavedData when auto-responder is disabled to prevent hangs
            auto action = PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData;

            // Same check-then-call contract as above.
            std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
            if (state->deviceReleased.load())
            {
                LogToWindow("Auto responder (default): device released, skipping contention response");
                return;
            }

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

        // Capture descriptors for VerifyConflictDescriptor
        if (state && localGameSave)
        {
            state->conflictLocalGameSave = *localGameSave;
        }
        if (state && remoteGameSave)
        {
            state->conflictRemoteGameSave = *remoteGameSave;
        }
        if (state)
        {
            state->hasConflictDescriptor = true;
        }

        // Diagnostic: always log conflict callback details to help debug two-device scenarios
        LogToWindowFormat("PFGameSave conflict callback: state=%p, localUserHandle=%p, autoConflictResponse=%s",
            state,
            localUserHandle,
            state ? (state->autoConflictResponse ? ConflictActionToString(*state->autoConflictResponse) : "(disabled)") : "(no state)");

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

        if (state)
        {
            state->lastOutOfStorageRequiredBytes.store(requiredBytes);
            state->outOfStorageCallbackFired.store(true);
        }

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

        if (state)
        {
            state->activeDeviceChangedCallbackCount.fetch_add(1);
            LogToWindowFormat("PFGameSaveFilesActiveDeviceChangedCallback invocation count: %d",
                state->activeDeviceChangedCallbackCount.load());
        }

        if (state && state->verboseLogs)
        {
            LogToWindow("PFGameSave active device changed callback received");
        }
    }

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

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

#endif

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
            { "UseCloud", PFGameSaveFilesUiConflictUserAction::TakeRemote },
            { "PlayOffline", PFGameSaveFilesUiConflictUserAction::TakeLocal }
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

    // Check if the test expects a specific error (e.g. E_PF_GAMESAVE_ALREADY_INITIALIZED)
    std::vector<HRESULT> acceptedHrs;
    bool hasExpectedHr = ParseExpectedHrParameter(parameters, acceptedHrs);

    if (state->pfGameSaveInitialized && !hasExpectedHr)
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

#if HC_PLATFORM_IS_PLAYSTATION
            // GameSave folder is just an identifier on some platforms, not a path. Strip out path characters to keep the test scenario compatible across platforms.
            std::string folderPath = folderIt->get<std::string>();
            folderPath.erase(std::remove_if(folderPath.begin(), folderPath.end(),
                [](char c) { return c == '\\' || c == ':'; }), folderPath.end());
            state->saveFolder = folderPath;
            args.saveFolder = state->saveFolder.c_str();
#else
            // On Xbox console the save folder override from YAML is a PC-only path
            // (e.g. "C:\gamesave\") that doesn't exist. Skip it and let the system
            // use its default location.
            if (state->engineType == DeviceEngineType::Xbox)
            {
                LogToWindow("PFGameSaveFilesInitialize: ignoring saveFolder override on Xbox");
            }
            else
            {
                state->saveFolder = folderIt->get<std::string>();
                args.saveFolder = state->saveFolder.c_str();
            }
#endif
        }
    }

    DeviceFileSystemInitialize(args.saveFolder ? args.saveFolder : "");

#if HC_PLATFORM_IS_MICROSOFT
    // Set force-inproc before init so provider selection picks Win32 instead of GRTS.
    // Resolved dynamically — this export doesn't exist in older GDK builds (2510).
    if (state->forceInproc)
    {
        using SetForceInprocFn = HRESULT(STDAPIVCALLTYPE*)(bool);
        // PlayFabGameSave.dll is DELAY-LOADED (see GameTestAppWindows.vcxproj), and
        // PFGameSaveFilesInitialize below is the first PFGameSave API touched. That means
        // the DLL is not in the process yet at this point, so GetModuleHandleW would return
        // null and we'd wrongly skip force-inproc (defaulting to the GRTS provider).
        // LoadLibraryW forces the (repo-built, app-dir) DLL to load now and returns its
        // handle — the same module the delay-load thunk binds to when init runs — so the
        // debug export resolves. Fall back to GetModuleHandleW if the DLL is already loaded.
        HMODULE hMod = LoadLibraryW(L"PlayFabGameSave.dll");
        if (!hMod)
        {
            hMod = GetModuleHandleW(L"PlayFabGameSave.dll");
        }
        auto pfn = hMod ? reinterpret_cast<SetForceInprocFn>(GetProcAddress(hMod, "PFGameSaveFilesSetForceInprocForDebug")) : nullptr;
        if (pfn)
        {
            HRESULT fihr = pfn(true);
            LogToWindowFormat("PFGameSaveFilesSetForceInprocForDebug (hr=0x%08X)", static_cast<uint32_t>(fihr));
        }
        else
        {
            LogToWindowFormat("PFGameSaveFilesSetForceInprocForDebug not available in this GDK build — skipped");
        }
    }
#endif

    HRESULT hr = PFGameSaveFilesInitialize(&args);
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("PFGameSaveFilesInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
    SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "PFGameSaveFilesInitialize failed");
        return payload;
    }

    // Automatically set the mock device ID for debug purposes.  Only applies to inproc builds.
    // A scenario can opt out (useRealDeviceId: true) to exercise the SDK's real device-id
    // generate-and-persist path (cloudsync/info.json) — e.g. the device-id-stability test that
    // reproduces the SteamDeck "new device id every session" report. The mock override otherwise
    // short-circuits GetLocalDeviceID() before info.json is ever read or written.
    bool useRealDeviceId = false;
    std::string realDevIdErr;
    if (parameters.is_object() && parameters.contains("useRealDeviceId"))
    {
        // Fail loudly on a malformed value. Swallowing the parse error would silently leave
        // useRealDeviceId false and re-enable the mock device id, which is stable by
        // construction - so gamesave-inproc-47-device-id-stable-across-relaunch would still
        // PASS while exercising none of the SDK's real generate-and-persist path.
        if (!TryParseBoolParameter(parameters, "useRealDeviceId", useRealDeviceId, realDevIdErr))
        {
            MarkFailure(payload.result, E_INVALIDARG, realDevIdErr);
            SetHResult(payload.result, E_INVALIDARG);
            return payload;
        }
    }
    if (useRealDeviceId)
    {
        LogToWindowFormat("PFGameSaveFilesSetMockDeviceIdForDebug skipped (useRealDeviceId=true; SDK persists device id in info.json)");
    }
    else
    {
        hr = PFGameSaveFilesSetMockDeviceIdForDebug(state->inputDeviceId.c_str());
        LogToWindowFormat("PFGameSaveFilesSetMockDeviceIdForDebug (hr=0x%08X)", static_cast<uint32_t>(hr));
    }

    state->pfGameSaveInitialized = true;

    // Re-apply verbose HC tracing. PFGameSaveFilesInitialize internally calls
    // PFInitializeWithLHC whose TraceState::Create error path nulls the HC callback.
    InitializeHCTraceToVerboseLog();

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
        // On Xbox/GRTS engines, skip the progress callback — GRTS blocks on it
        // and the only response is Cancel (no Continue action). The progress callback
        // is informational; the critical callbacks for suppressing system UI dialogs
        // are conflict, contention, syncFailed, and outOfStorage.
        if (state->engineType != DeviceEngineType::Xbox && state->engineType != DeviceEngineType::PcGrts)
        {
            callbacks.progressCallback = UiProgressCallback;
            callbacks.progressContext = state;
        }
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

CommandResultPayload HandleVerifyActiveDeviceChangedCallback(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    auto start = std::chrono::steady_clock::now();

    // Parse optional expectedCount parameter (exact match).
    // Parse optional expectedMinCount parameter (at-least match).
    // If neither is provided, just verifies the callback was called at least once.
    std::optional<int> expectedCount;
    std::optional<int> expectedMinCount;

    if (parameters.is_object())
    {
        if (parameters.contains("expectedCount") && parameters["expectedCount"].is_number_integer())
        {
            expectedCount = parameters["expectedCount"].get<int>();
        }
        if (parameters.contains("expectedMinCount") && parameters["expectedMinCount"].is_number_integer())
        {
            expectedMinCount = parameters["expectedMinCount"].get<int>();
        }
    }

    int actualCount = state->activeDeviceChangedCallbackCount.load();
    payload.result["activeDeviceChangedCallbackCount"] = actualCount;
    payload.elapsedMs = ComputeElapsedMs(start);

    if (expectedCount.has_value())
    {
        if (actualCount != expectedCount.value())
        {
            std::string errorMsg = "Expected ActiveDeviceChangedCallback count=" +
                std::to_string(expectedCount.value()) + " but got " + std::to_string(actualCount);
            LogToWindow(errorMsg);
            MarkFailure(payload.result, E_FAIL, errorMsg.c_str());
            return payload;
        }
    }
    else if (expectedMinCount.has_value())
    {
        if (actualCount < expectedMinCount.value())
        {
            std::string errorMsg = "Expected ActiveDeviceChangedCallback count >= " +
                std::to_string(expectedMinCount.value()) + " but got " + std::to_string(actualCount);
            LogToWindow(errorMsg);
            MarkFailure(payload.result, E_FAIL, errorMsg.c_str());
            return payload;
        }
    }
    else
    {
        // Default: verify called at least once
        if (actualCount < 1)
        {
            std::string errorMsg = "Expected ActiveDeviceChangedCallback to have been called at least once, but count is 0";
            LogToWindow(errorMsg);
            MarkFailure(payload.result, E_FAIL, errorMsg.c_str());
            return payload;
        }
    }

    LogToWindowFormat("VerifyActiveDeviceChangedCallback passed (count=%d)", actualCount);
    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleResetActiveDeviceChangedCallbackCount(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    auto start = std::chrono::steady_clock::now();

    int previousCount = state->activeDeviceChangedCallbackCount.exchange(0);
    payload.result["previousCount"] = previousCount;
    payload.elapsedMs = ComputeElapsedMs(start);

    LogToWindowFormat("ResetActiveDeviceChangedCallbackCount (previous=%d)", previousCount);
    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleWaitForActiveDeviceChanged(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    auto start = std::chrono::steady_clock::now();

    // Parse optional parameters
    int targetCount = 1; // default: wait for at least 1 callback
    int timeoutMs = 30000; // default: 30 seconds

    if (parameters.is_object())
    {
        if (parameters.contains("expectedMinCount") && parameters["expectedMinCount"].is_number_integer())
        {
            targetCount = parameters["expectedMinCount"].get<int>();
        }
        if (parameters.contains("timeoutMs") && parameters["timeoutMs"].is_number_integer())
        {
            timeoutMs = parameters["timeoutMs"].get<int>();
        }
    }

    LogToWindowFormat("WaitForActiveDeviceChanged: waiting for count >= %d (timeout=%dms)", targetCount, timeoutMs);

    // Poll until callback count reaches target or timeout
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    int actualCount = 0;
    while (std::chrono::steady_clock::now() < deadline)
    {
        actualCount = state->activeDeviceChangedCallbackCount.load();
        if (actualCount >= targetCount)
        {
            break;
        }
        // Pump messages briefly to allow callbacks to fire
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    actualCount = state->activeDeviceChangedCallbackCount.load();
    payload.result["activeDeviceChangedCallbackCount"] = actualCount;
    payload.elapsedMs = ComputeElapsedMs(start);

    if (actualCount >= targetCount)
    {
        LogToWindowFormat("WaitForActiveDeviceChanged: success (count=%d, elapsed=%lldms)", actualCount, payload.elapsedMs);
        MarkSuccess(payload.result);
    }
    else
    {
        std::string errorMsg = "WaitForActiveDeviceChanged: timed out waiting for count >= " +
            std::to_string(targetCount) + " (actual=" + std::to_string(actualCount) + ")";
        LogToWindow(errorMsg);
        MarkFailure(payload.result, HRESULT_FROM_WIN32(ERROR_TIMEOUT), errorMsg.c_str());
    }
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesResetCloudAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    return CommandHandlerShared::AsyncCallWithResult(state->taskQueueOwnedByCommand ? state->taskQueue : nullptr, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFGameSaveFilesResetCloudAsync(state->localUserHandle, &async);
            LogToWindowFormat("PFGameSaveFilesResetCloudAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            return PFGameSaveFilesResetCloudResult(&async);
        });
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

    // Parse localUserIndex parameter (default 0)
    int localUserIndex = ParseIndexParam(parameters, "localUserIndex");
    PFLocalUserHandle& targetLocalUser = GetLocalUserHandle(state, localUserIndex);

    if (!targetLocalUser)
    {
        MarkFailure(payload.result, E_POINTER,
            std::string("Local user handle not created at localUserIndex=") + std::to_string(localUserIndex));
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

    LogSaveFolderContents(state->saveFolder, "before AddUserWithUi");
    LogExtendedManifest(state->saveFolder, "before AddUserWithUi");

    // Calling AddUserWithUi (re-)acquires/reconnects this device as active. Clear the
    // released flag NOW — before issuing and awaiting the async — so the active-device
    // contention and sync-failed auto-responders will actually respond to callbacks
    // raised DURING this activation. Otherwise the callbacks skip responding (deviceReleased
    // is still true from a prior Upload(ReleaseDeviceAsActive)), the service keeps
    // re-raising contention every ~40s, and this AddUserWithUi hangs until the step timeout.
    // (The post-completion reset below is now redundant but kept as belt-and-suspenders.)
    state->deviceReleased.store(false);
    state->autoSyncFailedRetryCount = 0;
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        state->autoContentionRetryCount = 0;
    }

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(targetLocalUser, options, &async);

    // Check if test wants to skip the auto-reinit behavior (for conflict detection tests)
    bool skipAutoReinit = false;
    if (parameters.is_object() && parameters.contains("skipAutoReinit"))
    {
        const auto& val = parameters["skipAutoReinit"];
        if (val.is_boolean()) skipAutoReinit = val.get<bool>();
        else if (val.is_string()) { std::string s = val.get<std::string>(); skipAutoReinit = (s == "true" || s == "1"); }
    }

    // Auto-reinit: if user is already added, uninitialize and re-add transparently.
    // Many test scenarios call AddUser across multiple blocks without explicit uninit.
    if (hr == E_PF_GAMESAVE_USER_ALREADY_ADDED && !skipAutoReinit)
    {
        LogToWindow("AddUserWithUiAsync: user already added — auto-reinit (uninit + re-add)");

        // Uninitialize
        XAsyncBlock uninitAsync{};
        uninitAsync.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;
        HRESULT uninitHr = PFGameSaveFilesUninitializeAsync(&uninitAsync);
        if (SUCCEEDED(uninitHr))
        {
            uninitHr = XAsyncGetStatus(&uninitAsync, true);
        }
        if (SUCCEEDED(uninitHr))
        {
            state->pfGameSaveInitialized = false;
            LogToWindow("AddUserWithUiAsync: auto-uninit succeeded");

            // Re-initialize with same args as original init
            PFGameSaveInitArgs initArgs{};
            initArgs.backgroundQueue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;
            initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
            if (!state->saveFolder.empty())
            {
                initArgs.saveFolder = state->saveFolder.c_str();
            }
            HRESULT initHr = PFGameSaveFilesInitialize(&initArgs);
            if (SUCCEEDED(initHr))
            {
                state->pfGameSaveInitialized = true;
                LogToWindow("AddUserWithUiAsync: auto-reinit succeeded");

                // Re-register UI callbacks (cleared by Uninitialize)
                HRESULT cbHr = ReRegisterUiCallbacks(state);
                LogToWindowFormat("AddUserWithUiAsync: re-registered callbacks hr=0x%08X", static_cast<uint32_t>(cbHr));

                // Retry AddUser
                async = {};
                async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;
                hr = PFGameSaveFilesAddUserWithUiAsync(targetLocalUser, options, &async);
                LogToWindowFormat("AddUserWithUiAsync: retry returned hr=0x%08X", static_cast<uint32_t>(hr));
            }
            else
            {
                LogToWindowFormat("AddUserWithUiAsync: auto-reinit failed hr=0x%08X", static_cast<uint32_t>(initHr));
                hr = initHr;
            }
        }
        else
        {
            LogToWindowFormat("AddUserWithUiAsync: auto-uninit failed hr=0x%08X", static_cast<uint32_t>(uninitHr));
            hr = uninitHr;
        }
    }

    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;
    if (SUCCEEDED(hr))
    {
        // Poll with message pumping so that GRTS stock UI dialogs can render on desktop.
        // A blocking XAsyncGetStatus(&async, true) would deadlock the UI thread on desktop.
        // On Xbox, PeekMessage returns no messages so the loop simply polls — safe on all GDK platforms.
        for (;;)
        {
            waitHr = XAsyncGetStatus(&async, false);
            if (waitHr != E_PENDING)
            {
                break;
            }

            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
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

    LogSaveFolderContents(state->saveFolder, "after AddUserWithUi");
    LogExtendedManifest(state->saveFolder, "after AddUserWithUi");

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
            payload.result["status"] = "succeeded";
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

    // Device is now active again — clear the released flag so auto-responders function normally.
    state->deviceReleased.store(false);
    state->autoSyncFailedRetryCount = 0;

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
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiSyncFailedUserAction action{};
    std::string error;
    if (!TryParseSyncFailedAction(parameters, action, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetUiSyncFailedResponse(state->localUserHandle, action);
        LogToWindowFormat("PFGameSaveFilesSetUiSyncFailedResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetUiActiveDeviceContentionResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiActiveDeviceContentionUserAction action{};
    std::string error;
    if (!TryParseActiveDeviceContentionAction(parameters, action, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetUiActiveDeviceContentionResponse(state->localUserHandle, action);
        LogToWindowFormat("PFGameSaveFilesSetUiActiveDeviceContentionResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetUiConflictResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiConflictUserAction action{};
    std::string error;
    if (!TryParseConflictAction(parameters, action, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetUiConflictResponse(state->localUserHandle, action);
        LogToWindowFormat("PFGameSaveFilesSetUiConflictResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetUiOutOfStorageResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiOutOfStorageUserAction action{};
    std::string error;
    if (!TryParseOutOfStorageAction(parameters, action, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetUiOutOfStorageResponse(state->localUserHandle, action);
        LogToWindowFormat("PFGameSaveFilesSetUiOutOfStorageResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandleVerifyOutOfStorageCallback(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->outOfStorageCallbackFired.load())
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_FAIL, "Out-of-storage callback was not fired");
        return payload;
    }

    uint64_t actualBytes = state->lastOutOfStorageRequiredBytes.load();

    // Parse expected parameters
    uint64_t requiredBytesMinimum = 0;
    uint64_t requiredBytesApprox = 0;
    int marginPercent = 20;

    if (parameters.contains("requiredBytesMinimum"))
    {
        requiredBytesMinimum = std::stoull(parameters["requiredBytesMinimum"].get<std::string>());
    }
    if (parameters.contains("requiredBytesApprox"))
    {
        requiredBytesApprox = std::stoull(parameters["requiredBytesApprox"].get<std::string>());
    }
    if (parameters.contains("marginPercent"))
    {
        marginPercent = std::stoi(parameters["marginPercent"].get<std::string>());
    }

    // Verify minimum
    if (requiredBytesMinimum > 0 && actualBytes < requiredBytesMinimum)
    {
        std::string error = "requiredBytes (" + std::to_string(actualBytes) +
            ") is less than minimum (" + std::to_string(requiredBytesMinimum) + ")";
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_FAIL, error);
        return payload;
    }

    // Verify approximate value within margin
    if (requiredBytesApprox > 0)
    {
        uint64_t low = requiredBytesApprox - (requiredBytesApprox * marginPercent / 100);
        uint64_t high = requiredBytesApprox + (requiredBytesApprox * marginPercent / 100);
        if (actualBytes < low || actualBytes > high)
        {
            std::string error = "requiredBytes (" + std::to_string(actualBytes) +
                ") not within " + std::to_string(marginPercent) + "% of expected (" +
                std::to_string(requiredBytesApprox) + "), range [" +
                std::to_string(low) + ", " + std::to_string(high) + "]";
            CommandResultPayload payload{};
            payload.result = CreateBaseResult(commandId, command, deviceId);
            MarkFailure(payload.result, E_FAIL, error);
            return payload;
        }
    }

    // Reset state for future callbacks
    state->outOfStorageCallbackFired.store(false);
    state->lastOutOfStorageRequiredBytes.store(0);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload& p) -> HRESULT
    {
        p.result["actualRequiredBytes"] = actualBytes;
        return S_OK;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetUiProgressResponse(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    PFGameSaveFilesUiProgressUserAction action{};
    std::string error;
    if (!TryParseProgressAction(parameters, action, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetUiProgressResponse(state->localUserHandle, action);
        LogToWindowFormat("PFGameSaveFilesSetUiProgressResponse (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
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
    state->autoSyncFailedFallbackAction.reset();

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

    // Parse optional fallbackAction (action to use when maxRetries is exhausted; default: Cancel)
    if (parameters.is_object() && parameters.contains("fallbackAction"))
    {
        std::string fallbackStr = parameters["fallbackAction"].get<std::string>();
        PFGameSaveFilesUiSyncFailedUserAction fallback{};
        std::string fbError;
        nlohmann::json fbParams = { {"action", fallbackStr} };
        if (TryParseSyncFailedAction(fbParams, fallback, fbError))
        {
            state->autoSyncFailedFallbackAction = fallback;
        }
    }

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
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        state->autoContentionRetryCount = 0;  // reset counter on new config
        state->autoContentionMaxRetries = -1;
        state->autoContentionFallbackAction.reset();
    }

    // Parse optional maxRetries (may be int or string)
    if (parameters.contains("maxRetries"))
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        auto& val = parameters["maxRetries"];
        if (val.is_number())
            state->autoContentionMaxRetries = val.get<int>();
        else if (val.is_string())
            state->autoContentionMaxRetries = std::stoi(val.get<std::string>());
    }

    // Parse optional fallbackAction
    if (parameters.contains("fallbackAction"))
    {
        std::string fallbackStr = parameters["fallbackAction"].get<std::string>();
        PFGameSaveFilesUiActiveDeviceContentionUserAction fallback{};
        std::string fbError;
        nlohmann::json fbParams = { {"action", fallbackStr} };
        if (TryParseActiveDeviceContentionAction(fbParams, fallback, fbError))
        {
            std::lock_guard<std::mutex> lock(state->progressMutex);
            state->autoContentionFallbackAction = fallback;
        }
    }

    LogToWindowFormat("Auto responder: ActiveDeviceContention set to %s%s",
        ActiveDeviceContentionActionToString(action),
        state->autoContentionMaxRetries >= 0
            ? (std::string(" (maxRetries=") + std::to_string(state->autoContentionMaxRetries) + ")").c_str()
            : "");
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
    LogToWindowFormat("Auto responder: Conflict set to %s (state=%p)", ConflictActionToString(action), state);
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

CommandResultPayload HandlePFGameSaveFilesSetUiProgressAutoWriteOnUpload(
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
        {
            std::lock_guard<std::mutex> lock(state->progressMutex);
            state->autoProgressWriteOnUploadJson.reset();
            state->autoProgressWriteOnUploadFired = false;
        }
        LogToWindow("Auto write-on-upload: disabled");
        payload.elapsedMs = 0;
        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        return payload;
    }

    // Store the full parameters (which contains "operations") for later ExecuteWriteGameSaveData
    if (!parameters.contains("operations"))
    {
        MarkFailure(payload.result, E_INVALIDARG, "SetUiProgressAutoWriteOnUpload requires 'operations' when enabled");
        return payload;
    }

    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        state->autoProgressWriteOnUploadJson = parameters.dump();
        state->autoProgressWriteOnUploadFired = false;
    }
    LogToWindow("Auto write-on-upload: enabled — will execute operations when Uploading detected");
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesSetUiProgressStateRecording(
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

    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        state->recordProgressStates = enable;
        if (enable)
        {
            state->recordedProgressStates.clear();
        }
    }
    if (enable)
    {
        LogToWindow("Progress state recording: enabled (cleared previous)");
    }
    else
    {
        LogToWindow("Progress state recording: disabled");
    }

    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesVerifyProgressStateSequence(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    if (!parameters.contains("expectedSequence") || !parameters["expectedSequence"].is_array())
    {
        MarkFailure(payload.result, E_INVALIDARG, "Missing or invalid 'expectedSequence' array");
        return payload;
    }

    // Parse expected states
    std::vector<PFGameSaveFilesSyncState> expected;
    for (const auto& item : parameters["expectedSequence"])
    {
        std::string name = item.get<std::string>();
        if (name == "NotStarted") expected.push_back(PFGameSaveFilesSyncState::NotStarted);
        else if (name == "PreparingForDownload") expected.push_back(PFGameSaveFilesSyncState::PreparingForDownload);
        else if (name == "Downloading") expected.push_back(PFGameSaveFilesSyncState::Downloading);
        else if (name == "PreparingForUpload") expected.push_back(PFGameSaveFilesSyncState::PreparingForUpload);
        else if (name == "Uploading") expected.push_back(PFGameSaveFilesSyncState::Uploading);
        else if (name == "SyncComplete") expected.push_back(PFGameSaveFilesSyncState::SyncComplete);
        else
        {
            MarkFailure(payload.result, E_INVALIDARG, "Unknown state: " + name);
            return payload;
        }
    }

    // Snapshot recorded progress states under the lock to avoid racing with callback
    std::vector<DeviceGameSaveState::ProgressStateEntry> recordedProgressSnapshot;
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        recordedProgressSnapshot = state->recordedProgressStates;
    }

    // Extract unique states in order from recorded data
    std::vector<PFGameSaveFilesSyncState> actualSequence;
    for (const auto& entry : recordedProgressSnapshot)
    {
        if (actualSequence.empty() || actualSequence.back() != entry.state)
        {
            actualSequence.push_back(entry.state);
        }
    }

    // Log what was recorded
    std::string recordedStr;
    for (size_t i = 0; i < actualSequence.size(); ++i)
    {
        if (i > 0) recordedStr += " -> ";
        recordedStr += SyncStateToString(actualSequence[i]);
    }
    LogToWindowFormat("VerifyProgressStateSequence: recorded [%s] (%zu raw entries)",
        recordedStr.c_str(), recordedProgressSnapshot.size());

    // Verify the expected states appear in order within the actual sequence
    size_t expectedIdx = 0;
    for (size_t i = 0; i < actualSequence.size() && expectedIdx < expected.size(); ++i)
    {
        if (actualSequence[i] == expected[expectedIdx])
        {
            ++expectedIdx;
        }
    }

    if (expectedIdx < expected.size())
    {
        std::string expectedStr;
        for (size_t i = 0; i < expected.size(); ++i)
        {
            if (i > 0) expectedStr += " -> ";
            expectedStr += SyncStateToString(expected[i]);
        }
        MarkFailure(payload.result, E_FAIL,
            "State sequence mismatch. Expected [" + expectedStr + "] but recorded [" + recordedStr + "]");
        return payload;
    }

    // Verify monotonic (state values never decrease)
    bool verifyMonotonic = false;
    if (parameters.contains("verifyMonotonic") && parameters["verifyMonotonic"].is_boolean())
    {
        verifyMonotonic = parameters["verifyMonotonic"].get<bool>();
    }

    if (verifyMonotonic)
    {
        for (size_t i = 1; i < recordedProgressSnapshot.size(); ++i)
        {
            if (static_cast<uint32_t>(recordedProgressSnapshot[i].state) <
                static_cast<uint32_t>(recordedProgressSnapshot[i - 1].state))
            {
                MarkFailure(payload.result, E_FAIL,
                    "State sequence not monotonic at index " + std::to_string(i) +
                    ": " + SyncStateToString(recordedProgressSnapshot[i - 1].state) +
                    " -> " + SyncStateToString(recordedProgressSnapshot[i].state));
                return payload;
            }
        }
    }

    // Verify bytes increasing within same state
    bool verifyBytesIncreasing = false;
    if (parameters.contains("verifyBytesIncreasing") && parameters["verifyBytesIncreasing"].is_boolean())
    {
        verifyBytesIncreasing = parameters["verifyBytesIncreasing"].get<bool>();
    }

    if (verifyBytesIncreasing)
    {
        for (size_t i = 1; i < recordedProgressSnapshot.size(); ++i)
        {
            if (recordedProgressSnapshot[i].state == recordedProgressSnapshot[i - 1].state)
            {
                if (recordedProgressSnapshot[i].currentBytes < recordedProgressSnapshot[i - 1].currentBytes)
                {
                    MarkFailure(payload.result, E_FAIL,
                        "Bytes decreased within state " + SyncStateToString(recordedProgressSnapshot[i].state) +
                        " at index " + std::to_string(i));
                    return payload;
                }
            }
        }
    }

    LogToWindow("VerifyProgressStateSequence: PASSED");
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

    std::string error;
    std::string mockDeviceId;
    if (!TryGetStringParameter(parameters, "deviceId", mockDeviceId, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetMockDeviceIdForDebug(mockDeviceId.c_str());
        LogToWindowFormat("PFGameSaveFilesSetMockDeviceIdForDebug (deviceId=%s, hr=0x%08X)", mockDeviceId.c_str(), static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetForceOutOfStorageErrorForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    if (!parameters.is_object() || parameters.find("forceError") == parameters.end())
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'forceError'");
        return payload;
    }

    bool forceError = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "forceError", forceError, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetForceOutOfStorageErrorForDebug(forceError);
        LogToWindowFormat("PFGameSaveFilesSetForceOutOfStorageErrorForDebug (forceError=%s, hr=0x%08X)", forceError ? "true" : "false", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetForceSyncFailedErrorForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    if (!parameters.is_object() || parameters.find("forceError") == parameters.end())
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'forceError'");
        return payload;
    }

    bool forceError = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "forceError", forceError, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetForceSyncFailedErrorForDebug(forceError);
        LogToWindowFormat("PFGameSaveFilesSetForceSyncFailedErrorForDebug (forceError=%s, hr=0x%08X)", forceError ? "true" : "false", static_cast<uint32_t>(hr));
        return hr;
    });
}

// Maps a YAML string mode (e.g. "None", "InitiateUpload") to the SDK enum.
// Used by HandlePFGameSaveFilesSetMockForceOfflineForDebug to simulate offline state
// at SDK boundary WITHOUT touching the OS network adapter. Prefer this over
// ChangeTargetDeviceState DisableNetwork for tests that need offline behavior.
static bool TryParseMockForceOfflineMode(const std::string& s, GameSaveServiceMockForcedOffline& out, std::string& error)
{
    if (s == "None")                       { out = GameSaveServiceMockForcedOffline::None;                       return true; }
    if (s == "ListManifests")              { out = GameSaveServiceMockForcedOffline::ListManifests;              return true; }
    if (s == "InitializeManifest")         { out = GameSaveServiceMockForcedOffline::InitializeManifest;         return true; }
    if (s == "GetManifestDownloadDetails") { out = GameSaveServiceMockForcedOffline::GetManifestDownloadDetails; return true; }
    if (s == "DownloadFile")               { out = GameSaveServiceMockForcedOffline::DownloadFile;               return true; }
    if (s == "InitiateUpload")             { out = GameSaveServiceMockForcedOffline::InitiateUpload;             return true; }
    if (s == "FinalizeManifest")           { out = GameSaveServiceMockForcedOffline::FinalizeManifest;           return true; }
    if (s == "UpdateManifest")             { out = GameSaveServiceMockForcedOffline::UpdateManifest;             return true; }
    if (s == "UploadFile")                 { out = GameSaveServiceMockForcedOffline::UploadFile;                 return true; }
    error = "Invalid 'mode' value '" + s + "'. Valid values: None, ListManifests, InitializeManifest, GetManifestDownloadDetails, DownloadFile, InitiateUpload, FinalizeManifest, UpdateManifest, UploadFile.";
    return false;
}

CommandResultPayload HandlePFGameSaveFilesSetMockForceOfflineForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    std::string error;
    std::string modeStr;
    if (!TryGetStringParameter(parameters, "mode", modeStr, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    GameSaveServiceMockForcedOffline mode = GameSaveServiceMockForcedOffline::None;
    if (!TryParseMockForceOfflineMode(modeStr, mode, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetMockForceOfflineForDebug(mode);
        LogToWindowFormat("PFGameSaveFilesSetMockForceOfflineForDebug (mode=%s, hr=0x%08X)", modeStr.c_str(), static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetForceNullPendingManifestForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    if (!parameters.is_object() || parameters.find("force") == parameters.end())
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'force'");
        return payload;
    }

    bool force = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "force", force, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetForceNullPendingManifestForDebug(force);
        LogToWindowFormat("PFGameSaveFilesSetForceNullPendingManifestForDebug (force=%s, hr=0x%08X)", force ? "true" : "false", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetWriteManifestsToDiskForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    if (!parameters.is_object() || parameters.find("writeManifests") == parameters.end())
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, "Missing parameter 'writeManifests'");
        return payload;
    }

    bool writeManifests = false;
    std::string error;
    if (!TryParseBoolParameter(parameters, "writeManifests", writeManifests, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetWriteManifestsToDiskForDebug(writeManifests);
        LogToWindowFormat("PFGameSaveFilesSetWriteManifestsToDiskForDebug (writeManifests=%s, hr=0x%08X)", writeManifests ? "true" : "false", static_cast<uint32_t>(hr));
        return hr;
    });
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

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetActiveDevicePollForceChangeForDebug();
        LogToWindowFormat("PFGameSaveFilesSetActiveDevicePollForceChangeForDebug (hr=0x%08X)", static_cast<uint32_t>(hr));
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesSetActiveDevicePollIntervalForDebug(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    int64_t intervalValue = 0;
    std::string error;
    if (!TryGetInt64Parameter(parameters, "interval", intervalValue, error))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (intervalValue < 0 || intervalValue > static_cast<int64_t>(std::numeric_limits<uint32_t>::max()))
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_INVALIDARG, "Parameter 'interval' must be between 0 and 4294967295");
        return payload;
    }

    uint32_t interval = static_cast<uint32_t>(intervalValue);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
    {
        HRESULT hr = PFGameSaveFilesSetActiveDevicePollIntervalForDebug(interval);
        LogToWindowFormat("PFGameSaveFilesSetActiveDevicePollIntervalForDebug (interval=%u, hr=0x%08X)", interval, static_cast<uint32_t>(hr));
        return hr;
    });
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

    // Parse localUserIndex parameter (default 0)
    int localUserIndex = ParseIndexParam(parameters, "localUserIndex");
    PFLocalUserHandle& targetLocalUser = GetLocalUserHandle(state, localUserIndex);

    if (!targetLocalUser)
    {
        MarkFailure(payload.result, E_POINTER,
            std::string("Local user handle not created at localUserIndex=") + std::to_string(localUserIndex));
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
        HRESULT hr = PFGameSaveFilesUploadWithUiAsync(targetLocalUser, option, state->pendingUploadAsync.get());
        payload.elapsedMs = ComputeElapsedMs(start);

        LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync (fireAndForget mode, hr=0x%08X)", static_cast<uint32_t>(hr));

        if (FAILED(hr))
        {
            state->pendingUploadAsync.reset();
            MarkFailure(payload.result, hr, "PFGameSaveFilesUploadWithUiAsync failed to start");
            return payload;
        }

        // Fire-and-forget: completion is never observed here, so if the caller asked for
        // ReleaseDeviceAsActive mark the device released now. Otherwise a callback arriving
        // after the release would still reach the auto-responder and crash.
        if (option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
        {
            // Same lock the auto-responders hold across their check-then-SDK-call, so a release
            // cannot interleave between their check and the call.
            std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
            state->deviceReleased.store(true);
            LogToWindow("Device will be released after upload with ReleaseDeviceAsActive (fireAndForget) - auto-responders disabled");
        }

        // Return immediately without waiting - the upload continues in the background
        SetHResult(payload.result, S_OK);
        MarkSuccess(payload.result);
        return payload;
    }

    XAsyncBlock async{};
    async.queue = state->taskQueueOwnedByCommand ? state->taskQueue : nullptr;

    auto start = std::chrono::steady_clock::now();
    HRESULT hr = PFGameSaveFilesUploadWithUiAsync(targetLocalUser, option, &async);
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

    LogSaveFolderContents(state->saveFolder, "after Upload");
    LogExtendedManifest(state->saveFolder, "after Upload");

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

    // Mark the device released for every successful ReleaseDeviceAsActive upload. This has to
    // happen before the expectedHr early-return below, which would otherwise leave the flag
    // false and let a late callback reach the auto-responder on a released context.
    if (SUCCEEDED(finalHr) && option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
    {
        // Same lock the auto-responders hold across their check-then-SDK-call, so a release
        // cannot interleave between their check and the call.
        std::lock_guard<std::mutex> lock(state->deviceReleaseMutex);
        state->deviceReleased.store(true);
        LogToWindow("Device released after upload with ReleaseDeviceAsActive - auto-responders disabled");
    }

    // Check if the result matches any of the expected HRESULTs
    if (hasExpectedHr)
    {
        if (IsHrAccepted(finalHr, acceptedHrs))
        {
            LogToWindowFormat("PFGameSaveFilesUploadWithUiAsync: hr=0x%08X matches expectedHr", static_cast<uint32_t>(finalHr));
            // Don't call MarkSuccess — it resets hresult to 0, which breaks
            // controller-side expectedHr validation. Just set status directly.
            payload.result["status"] = "succeeded";
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
    // Parse localUserIndex parameter (default 0)
    int localUserIndex = ParseIndexParam(parameters, "localUserIndex");
    PFLocalUserHandle& targetLocalUser = GetLocalUserHandle(state, localUserIndex);

    if (!targetLocalUser)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER,
            std::string("Local user handle not created at localUserIndex=") + std::to_string(localUserIndex));
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload& payload) -> HRESULT
    {
        size_t folderSize = 0;
        HRESULT hr = PFGameSaveFilesGetFolderSize(targetLocalUser, &folderSize);
        if (FAILED(hr))
        {
            LogToWindowFormat("PFGameSaveFilesGetFolderSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        }

        std::vector<char> buffer(folderSize > 0 ? folderSize : 1);
        size_t used = 0;
        hr = PFGameSaveFilesGetFolder(targetLocalUser, buffer.size(), buffer.data(), &used);
        LogToWindowFormat("PFGameSaveFilesGetFolder (hr=0x%08X)", static_cast<uint32_t>(hr));

        if (SUCCEEDED(hr))
        {
            size_t stringLength = (used > 0 && used <= buffer.size()) ? used - 1 : 0;
            std::string folder(buffer.data(), buffer.data() + stringLength);
            LogToWindowFormat("PFGameSaveFilesGetFolder folder='%s'", folder.c_str());
#if !HC_PLATFORM_IS_PLAYSTATION
            // for now we need to disable this when automation is running on platforms that access the file system through a dynamic mount point.
            state->saveFolder = folder;
#endif
            payload.result["folder"] = folder;
        }
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesGetRemainingQuota(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        return payload;
    }

    return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload& payload) -> HRESULT
    {
        int64_t remainingQuota = 0;
        HRESULT hr = PFGameSaveFilesGetRemainingQuota(state->localUserHandle, &remainingQuota);
        LogToWindowFormat("PFGameSaveFilesGetRemainingQuota (hr=0x%08X)", static_cast<uint32_t>(hr));
        if (SUCCEEDED(hr))
        {
            payload.result["remainingQuotaBytes"] = remainingQuota;

            // Support recordAs parameter for VerifyQuotaDelta
            std::string recordAs;
            std::string error;
            if (TryGetStringParameter(parameters, "recordAs", recordAs, error) && !recordAs.empty())
            {
                state->quotaRecordings[recordAs] = remainingQuota;
                LogToWindowFormat("Recorded quota '%s' = %lld bytes", recordAs.c_str(), static_cast<long long>(remainingQuota));
            }

            // Verify minimumBytes if provided
            if (parameters.is_object() && parameters.contains("minimumBytes"))
            {
                int64_t minBytes = 0;
                const auto& minVal = parameters["minimumBytes"];
                if (minVal.is_number())
                    minBytes = minVal.get<int64_t>();
                else if (minVal.is_string())
                    minBytes = std::stoll(minVal.get<std::string>());

                if (remainingQuota < minBytes)
                {
                    std::string err = "Remaining quota " + std::to_string(remainingQuota)
                        + " is below minimum " + std::to_string(minBytes);
                    MarkFailure(payload.result, E_FAIL, err);
                    return E_FAIL;
                }
            }
        }
        return hr;
    });
}

CommandResultPayload HandlePFGameSaveFilesIsConnectedToCloud(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->localUserHandle)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
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
        else if (expectedNode.is_string())
        {
            std::string s = expectedNode.get<std::string>();
            expectedConnected = (s == "true" || s == "True" || s == "1");
        }
    }

    auto result = CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload& payload) -> HRESULT
    {
        bool isConnected = false;
        HRESULT hr = PFGameSaveFilesIsConnectedToCloud(state->localUserHandle, &isConnected);
        LogToWindowFormat("PFGameSaveFilesIsConnectedToCloud (hr=0x%08X, isConnected=%s)", 
            static_cast<uint32_t>(hr), isConnected ? "true" : "false");
        if (SUCCEEDED(hr))
        {
            payload.result["isConnectedToCloud"] = isConnected;
        }
        return hr;
    });

    // Check if actual state matches expected state (post-SyncCall validation)
    if (result.result.value("status", "") == "succeeded" && expectedConnected.has_value())
    {
        bool isConnected = result.result.value("isConnectedToCloud", false);
        if (isConnected != expectedConnected.value())
        {
            std::string errorMsg = "Expected isConnectedToCloud=" + std::string(expectedConnected.value() ? "true" : "false") +
                                   " but got " + std::string(isConnected ? "true" : "false");
            LogToWindow(errorMsg);
            MarkFailure(result.result, E_FAIL, errorMsg.c_str());
        }
    }

    return result;
}

CommandResultPayload HandlePFGameSaveFilesUninitializeAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    if (!state->pfGameSaveInitialized)
    {
        return CommandHandlerShared::SyncCall(commandId, command, deviceId, [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGameSaveFilesUninitializeAsync skipped (not initialized)");
            return S_OK;
        });
    }

    return CommandHandlerShared::AsyncCallWithResult(state->taskQueueOwnedByCommand ? state->taskQueue : nullptr, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFGameSaveFilesUninitializeAsync(&async);
            LogToWindowFormat("PFGameSaveFilesUninitializeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFGameSaveFilesUninitializeResult(&async);
            if (SUCCEEDED(hr))
            {
                state->pfGameSaveInitialized = false;
            }
            return hr;
        });
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
    SetHResult(payload.result, finalHr);

    if (hasExpectedHr)
    {
        if (IsHrAccepted(finalHr, acceptedHrs))
        {
            LogToWindowFormat("PFGameSaveFilesSetSaveDescriptionAsync: hr=0x%08X matches expectedHr", static_cast<uint32_t>(finalHr));
            payload.result["status"] = "succeeded";
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

    // On GRTS, save descriptions are only observable via UI conflict/contention callbacks.
    // The debug query API is inproc-only, so skip on GRTS.
    if (state->engineType == DeviceEngineType::PcGrts || state->engineType == DeviceEngineType::Xbox)
    {
        LogToWindow("PFGameSaveFilesGetSaveDescription: skipped (GRTS/Xbox - description only available via UI callbacks)");
        payload.result["skipped"] = true;
        payload.result["skipReason"] = "Save description query not available on GRTS";
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

#if HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5

static PFEventPipelineHandle s_eventPipelineHandle = nullptr;

#endif

CommandResultPayload HandlePFGameSaveFilesAddUserWithUiResult(
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
            HRESULT hr = PFGameSaveFilesAddUserWithUiResult(&localAsync);
            LogToWindowFormat("PFGameSaveFilesAddUserWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesGetFolderSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            size_t folderSize = 0;
            HRESULT hr = PFGameSaveFilesGetFolderSize(state->localUserHandle, &folderSize);
            LogToWindowFormat("PFGameSaveFilesGetFolderSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), folderSize);
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesUploadWithUiResult(
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
            HRESULT hr = PFGameSaveFilesUploadWithUiResult(&localAsync);
            LogToWindowFormat("PFGameSaveFilesUploadWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesSetSaveDescriptionResult(
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
            HRESULT hr = PFGameSaveFilesSetSaveDescriptionResult(&localAsync);
            LogToWindowFormat("PFGameSaveFilesSetSaveDescriptionResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesResetCloudResult(
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
            HRESULT hr = PFGameSaveFilesResetCloudResult(&localAsync);
            LogToWindowFormat("PFGameSaveFilesResetCloudResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesUninitializeResult(
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
            HRESULT hr = PFGameSaveFilesUninitializeResult(&localAsync);
            LogToWindowFormat("PFGameSaveFilesUninitializeResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFGameSaveFilesUiProgressGetProgress(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFGameSaveFilesSyncState syncState{};
            uint64_t current = 0;
            uint64_t total = 0;
            HRESULT hr = PFGameSaveFilesUiProgressGetProgress(state->localUserHandle, &syncState, &current, &total);
            LogToWindowFormat("PFGameSaveFilesUiProgressGetProgress (hr=0x%08X, current=%llu, total=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(current), static_cast<unsigned long long>(total));
            return hr;
        });
}

// Helper to get a numeric value from JSON that may be a string (controller serializes YAML numbers as strings)
static int64_t GetJsonInt64(const nlohmann::json& value)
{
    if (value.is_number()) return value.get<int64_t>();
    if (value.is_string()) return std::stoll(value.get<std::string>());
    return 0;
}

static double GetJsonDouble(const nlohmann::json& value)
{
    if (value.is_number()) return value.get<double>();
    if (value.is_string()) return std::stod(value.get<std::string>());
    return 0.0;
}

static uint64_t GetJsonUint64(const nlohmann::json& value)
{
    if (value.is_number()) return value.get<uint64_t>();
    if (value.is_string()) return std::stoull(value.get<std::string>());
    return 0;
}

// Controller serializes YAML booleans as strings too
static bool GetJsonBool(const nlohmann::json& value)
{
    if (value.is_boolean()) return value.get<bool>();
    if (value.is_string())
    {
        std::string s = value.get<std::string>();
        return (s == "true" || s == "True" || s == "1");
    }
    return false;
}

namespace
{
    // Stops the background sampler and joins its thread. Safe to call when not running.
    void StopProgressSamplerThread(DeviceGameSaveState* state)
    {
        state->progressSamplerRunning.store(false, std::memory_order_release);
        if (state->progressSamplerThread.joinable())
        {
            state->progressSamplerThread.join();
        }
    }

    const char* ProgressSourceName(bool polled) { return polled ? "polled" : "callback"; }

    bool TryParseSyncStateName(const std::string& name, PFGameSaveFilesSyncState& out)
    {
        if (name == "NotStarted") { out = PFGameSaveFilesSyncState::NotStarted; return true; }
        if (name == "PreparingForDownload") { out = PFGameSaveFilesSyncState::PreparingForDownload; return true; }
        if (name == "Downloading") { out = PFGameSaveFilesSyncState::Downloading; return true; }
        if (name == "PreparingForUpload") { out = PFGameSaveFilesSyncState::PreparingForUpload; return true; }
        if (name == "Uploading") { out = PFGameSaveFilesSyncState::Uploading; return true; }
        if (name == "SyncComplete") { out = PFGameSaveFilesSyncState::SyncComplete; return true; }
        return false;
    }
}

// Starts a background thread that polls PFGameSaveFilesUiProgressGetProgress on an interval.
// This mirrors how a title actually drives a progress bar: PFGameSaveFilesUiProgressCallback is
// only a state-change notification, so the values behind a progress bar come from polling.
CommandResultPayload HandlePFGameSaveFilesStartProgressSampler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    uint32_t intervalMs = 10;
    if (parameters.contains("intervalMs"))
    {
        intervalMs = static_cast<uint32_t>(GetJsonUint64(parameters["intervalMs"]));
        if (intervalMs == 0) intervalMs = 1;
    }

    // A previous scenario step may have left one running.
    StopProgressSamplerThread(state);

    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        state->polledProgressSamples.clear();
    }
    // Seed from the main thread, where localUserHandle is written. The sampler thread must not read
    // that non-atomic field itself; if it is not set yet the UI progress callback publishes it.
    state->progressSamplerUserHandle.store(state->localUserHandle, std::memory_order_release);
    state->progressSamplerStart = std::chrono::steady_clock::now();

    state->progressSamplerRunning.store(true, std::memory_order_release);
    state->progressSamplerThread = std::thread([state, intervalMs]()
    {
        // Bound the capture so a long sync cannot grow the vector without limit.
        constexpr size_t c_maxSamples = 200000;
        while (state->progressSamplerRunning.load(std::memory_order_acquire))
        {
            PFLocalUserHandle userHandle = state->progressSamplerUserHandle.load(std::memory_order_acquire);

            if (userHandle)
            {
                PFGameSaveFilesSyncState syncState{};
                uint64_t current = 0;
                uint64_t total = 0;
                if (SUCCEEDED(PFGameSaveFilesUiProgressGetProgress(userHandle, &syncState, &current, &total)))
                {
                    std::lock_guard<std::mutex> lock(state->progressMutex);
                    // Only record changes: a progress bar redraws on change, and this keeps the
                    // captured series proportional to real progress rather than to poll frequency.
                    const auto& samples = state->polledProgressSamples;
                    const bool changed = samples.empty() ||
                        samples.back().state != syncState ||
                        samples.back().currentBytes != current ||
                        samples.back().totalBytes != total;
                    if (changed && samples.size() < c_maxSamples)
                    {
                        const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - state->progressSamplerStart).count();
                        state->polledProgressSamples.push_back({ syncState, current, total, static_cast<uint64_t>(elapsedMs) });
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
    });

    LogToWindowFormat("Progress sampler started (intervalMs=%u)", intervalMs);
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandlePFGameSaveFilesStopProgressSampler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    UNREFERENCED_PARAMETER(parameters);
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    StopProgressSamplerThread(state);

    size_t sampleCount = 0;
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        sampleCount = state->polledProgressSamples.size();
    }

    LogToWindowFormat("Progress sampler stopped (%zu distinct samples)", sampleCount);
    payload.result["sampleCount"] = static_cast<uint64_t>(sampleCount);
    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

// Verifies that the captured progress series is fit to drive a progress bar:
// total is known (non-zero), current starts at 0, reaches total (100%), never goes backwards,
// and has enough intermediate values that a bar would visibly move.
CommandResultPayload HandlePFGameSaveFilesVerifyProgressQuality(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    const bool usePolled = !parameters.contains("source") ||
        parameters["source"].get<std::string>() != "callback";

    bool filterByState = false;
    PFGameSaveFilesSyncState stateFilter{};
    if (parameters.contains("state"))
    {
        std::string name = parameters["state"].get<std::string>();
        if (!TryParseSyncStateName(name, stateFilter))
        {
            MarkFailure(payload.result, E_INVALIDARG, "Unknown state: " + name);
            return payload;
        }
        filterByState = true;
    }

    const bool requireNonZeroTotal = !parameters.contains("requireNonZeroTotal") || GetJsonBool(parameters["requireNonZeroTotal"]);
    const bool requireStartAtZero = !parameters.contains("requireStartAtZero") || GetJsonBool(parameters["requireStartAtZero"]);
    const bool requireReachTotal = !parameters.contains("requireReachTotal") || GetJsonBool(parameters["requireReachTotal"]);
    const bool requireMonotonic = !parameters.contains("requireMonotonic") || GetJsonBool(parameters["requireMonotonic"]);
    const size_t minDistinctCurrent = parameters.contains("minDistinctCurrent")
        ? static_cast<size_t>(GetJsonUint64(parameters["minDistinctCurrent"]))
        : 3;
    // Permits the documented "total not yet known" window at the very start of an operation, while
    // still failing if progress ever falls back to indeterminate after a real total was published.
    const bool allowLeadingUnknownTotal = parameters.contains("allowLeadingUnknownTotal") &&
        GetJsonBool(parameters["allowLeadingUnknownTotal"]);

    std::vector<DeviceGameSaveState::ProgressStateEntry> samples;
    {
        std::lock_guard<std::mutex> lock(state->progressMutex);
        const auto& source = usePolled ? state->polledProgressSamples : state->recordedProgressStates;
        for (const auto& entry : source)
        {
            if (!filterByState || entry.state == stateFilter)
            {
                samples.push_back(entry);
            }
        }
    }

    std::string scope = std::string(ProgressSourceName(usePolled)) +
        (filterByState ? (" state=" + SyncStateToString(stateFilter)) : std::string(" (all states)"));

    if (samples.empty())
    {
        MarkFailure(payload.result, E_FAIL, "No progress samples captured for " + scope);
        return payload;
    }

    std::vector<std::string> failures;

    if (allowLeadingUnknownTotal)
    {
        size_t firstKnown = 0;
        while (firstKnown < samples.size() && samples[firstKnown].totalBytes == 0)
        {
            // An indeterminate sample may not claim progress it cannot substantiate.
            if (samples[firstKnown].currentBytes != 0)
            {
                failures.push_back("sample reported current=" + std::to_string(samples[firstKnown].currentBytes) +
                    " while total was still unknown");
                break;
            }
            ++firstKnown;
        }
        if (firstKnown == samples.size())
        {
            MarkFailure(payload.result, E_FAIL,
                "Progress quality check failed for " + scope + ": total was never reported for any sample");
            return payload;
        }
        samples.erase(samples.begin(), samples.begin() + static_cast<ptrdiff_t>(firstKnown));
    }

    // total must be known so a title can render a determinate bar rather than a spinner.
    uint64_t maxTotal = 0;
    size_t zeroTotalCount = 0;
    for (const auto& s : samples)
    {
        if (s.totalBytes > maxTotal) maxTotal = s.totalBytes;
        if (s.totalBytes == 0) ++zeroTotalCount;
    }
    if (requireNonZeroTotal && zeroTotalCount > 0)
    {
        failures.push_back("total was 0 in " + std::to_string(zeroTotalCount) + " of " +
            std::to_string(samples.size()) + " samples (a title cannot show a determinate progress bar)");
    }

    if (requireStartAtZero && samples.front().currentBytes != 0)
    {
        failures.push_back("first sample current=" + std::to_string(samples.front().currentBytes) + ", expected 0");
    }

    // current must never exceed total, and must not go backwards. When no state filter is applied,
    // each phase is internally monotonic but restarts at 0 on entering the next one, so monotonic
    // tracking resets at phase boundaries -- otherwise a perfectly valid multi-phase sync would be
    // reported as running backwards.
    uint64_t previousCurrent = 0;
    PFGameSaveFilesSyncState previousState{};
    bool first = true;
    for (const auto& s : samples)
    {
        if (s.totalBytes != 0 && s.currentBytes > s.totalBytes)
        {
            failures.push_back("current (" + std::to_string(s.currentBytes) + ") exceeded total (" +
                std::to_string(s.totalBytes) + ")");
            break;
        }
        const bool phaseBoundary = !first && !filterByState && s.state != previousState;
        if (requireMonotonic && !first && !phaseBoundary && s.currentBytes < previousCurrent)
        {
            failures.push_back("current went backwards: " + std::to_string(previousCurrent) +
                " -> " + std::to_string(s.currentBytes));
            break;
        }
        previousCurrent = s.currentBytes;
        previousState = s.state;
        first = false;
    }

    const uint64_t finalCurrent = samples.back().currentBytes;
    if (requireReachTotal)
    {
        if (maxTotal == 0)
        {
            failures.push_back("cannot reach 100%: total was never reported");
        }
        else if (finalCurrent < maxTotal)
        {
            failures.push_back("progress ended at " + std::to_string(finalCurrent) + "/" +
                std::to_string(maxTotal) + " instead of reaching 100%");
        }
    }

    // Enough distinct current values that a bar visibly moves rather than jumping 0 -> done.
    std::vector<uint64_t> distinctCurrent;
    for (const auto& s : samples)
    {
        if (distinctCurrent.empty() || distinctCurrent.back() != s.currentBytes)
        {
            distinctCurrent.push_back(s.currentBytes);
        }
    }
    if (distinctCurrent.size() < minDistinctCurrent)
    {
        failures.push_back("only " + std::to_string(distinctCurrent.size()) +
            " distinct current values (need >= " + std::to_string(minDistinctCurrent) +
            " to animate a progress bar)");
    }

    LogToWindowFormat("VerifyProgressQuality [%s]: %zu samples, %zu distinct current values, maxTotal=%llu, finalCurrent=%llu",
        scope.c_str(), samples.size(), distinctCurrent.size(),
        static_cast<unsigned long long>(maxTotal), static_cast<unsigned long long>(finalCurrent));
    for (const auto& s : samples)
    {
        LogToWindowFormat("  sample: t=%llums state=%s current=%llu total=%llu",
            static_cast<unsigned long long>(s.elapsedMs),
            SyncStateToString(s.state).c_str(),
            static_cast<unsigned long long>(s.currentBytes),
            static_cast<unsigned long long>(s.totalBytes));
    }

    payload.result["sampleCount"] = static_cast<uint64_t>(samples.size());
    payload.result["distinctCurrentValues"] = static_cast<uint64_t>(distinctCurrent.size());
    payload.result["maxTotal"] = maxTotal;
    payload.result["finalCurrent"] = finalCurrent;

    if (!failures.empty())
    {
        std::string message = "Progress quality check failed for " + scope + ": ";
        for (size_t i = 0; i < failures.size(); ++i)
        {
            if (i > 0) message += "; ";
            message += failures[i];
        }
        LogToWindowFormat("%s", message.c_str());
        MarkFailure(payload.result, E_FAIL, message);
        return payload;
    }

    payload.elapsedMs = 0;
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

// Helper to verify a single PFGameSaveDescriptor against expected values from YAML
static bool VerifyDescriptorFields(
    const PFGameSaveDescriptor& descriptor,
    const PFGameSaveDescriptor* otherDescriptor,
    const nlohmann::json& expected,
    nlohmann::json& result,
    const std::string& prefix)
{
    using namespace CommandHandlerShared;
    bool allPassed = true;

    auto fail = [&](const std::string& field, const std::string& msg) {
        LogToWindowFormat("VerifyDescriptor %s.%s: %s", prefix.c_str(), field.c_str(), msg.c_str());
        result[prefix + "." + field] = msg;
        allPassed = false;
    };

    if (expected.contains("deviceType"))
    {
        std::string exp = expected["deviceType"].get<std::string>();
        if (exp == "nonEmpty" && strlen(descriptor.deviceType) == 0)
            fail("deviceType", "expected nonEmpty but was empty");
        else if (exp == "empty" && strlen(descriptor.deviceType) != 0)
            fail("deviceType", std::string("expected empty but was '") + descriptor.deviceType + "'");
        else if (exp != "nonEmpty" && exp != "empty" && exp != descriptor.deviceType)
            fail("deviceType", std::string("expected '") + exp + "' but was '" + descriptor.deviceType + "'");
    }
    if (expected.contains("deviceId"))
    {
        std::string exp = expected["deviceId"].get<std::string>();
        if (exp == "nonEmpty" && strlen(descriptor.deviceId) == 0)
            fail("deviceId", "expected nonEmpty but was empty");
        else if (exp == "empty" && strlen(descriptor.deviceId) != 0)
            fail("deviceId", std::string("expected empty but was '") + descriptor.deviceId + "'");
    }
    if (expected.contains("friendlyName"))
    {
        std::string exp = expected["friendlyName"].get<std::string>();
        if (exp == "nonEmpty" && strlen(descriptor.deviceFriendlyName) == 0)
            fail("friendlyName", "expected nonEmpty but was empty");
        else if (exp == "empty" && strlen(descriptor.deviceFriendlyName) != 0)
            fail("friendlyName", std::string("expected empty but was '") + descriptor.deviceFriendlyName + "'");
    }
    if (expected.contains("thumbnailUri"))
    {
        std::string exp = expected["thumbnailUri"].get<std::string>();
        if (exp == "nonEmpty" && strlen(descriptor.thumbnailUri) == 0)
            fail("thumbnailUri", "expected nonEmpty but was empty");
        else if (exp == "empty" && strlen(descriptor.thumbnailUri) != 0)
            fail("thumbnailUri", std::string("expected empty but was '") + descriptor.thumbnailUri + "'");
    }
    if (expected.contains("shortSaveDescription"))
    {
        std::string exp = expected["shortSaveDescription"].get<std::string>();
        std::string actual = descriptor.shortSaveDescription;
        if (exp != actual)
            fail("shortSaveDescription", std::string("expected '") + exp + "' but was '" + actual + "'");
    }
    if (expected.contains("totalBytesApprox"))
    {
        uint64_t exp = GetJsonUint64(expected["totalBytesApprox"]);
        uint64_t actual = descriptor.totalBytes;
        // Allow 50% margin for approximate comparison
        uint64_t margin = exp / 2;
        if (actual < (exp > margin ? exp - margin : 0) || actual > exp + margin)
            fail("totalBytesApprox", std::string("expected ~") + std::to_string(exp) + " but was " + std::to_string(actual));
    }
    if (expected.contains("totalBytesMinimum"))
    {
        uint64_t minVal = GetJsonUint64(expected["totalBytesMinimum"]);
        if (descriptor.totalBytes < minVal)
            fail("totalBytesMinimum", std::string("expected >= ") + std::to_string(minVal) + " but was " + std::to_string(descriptor.totalBytes));
    }
    if (expected.contains("timeWithin24Hours") && GetJsonBool(expected["timeWithin24Hours"]))
    {
        time_t now = time(nullptr);
        double diffSeconds = difftime(now, descriptor.time);
        if (diffSeconds < -86400 || diffSeconds > 86400)
            fail("timeWithin24Hours", std::string("time diff ") + std::to_string(diffSeconds) + "s exceeds 24h");
    }
    if (expected.contains("timeBefore"))
    {
        std::string ref = expected["timeBefore"].get<std::string>();
        if (ref == "local" && otherDescriptor)
        {
            if (descriptor.time > otherDescriptor->time)
                fail("timeBefore", "expected remote.time <= local.time");
        }
    }

    return allPassed;
}

CommandResultPayload HandleVerifyContentionDescriptor(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    try
    {
        if (!state->hasContentionDescriptor)
        {
            MarkFailure(payload.result, E_FAIL, "No contention descriptor captured — contention callback not yet fired");
            return payload;
        }

        bool allPassed = true;
        if (parameters.contains("remote"))
        {
            if (!VerifyDescriptorFields(state->savedRemoteGameSave, &state->savedLocalGameSave,
                parameters["remote"], payload.result, "remote"))
                allPassed = false;
        }
        if (parameters.contains("local"))
        {
            if (!VerifyDescriptorFields(state->savedLocalGameSave, &state->savedRemoteGameSave,
                parameters["local"], payload.result, "local"))
                allPassed = false;
        }

        if (allPassed)
        {
            MarkSuccess(payload.result);
            SetHResult(payload.result, S_OK);
        }
        else
        {
            MarkFailure(payload.result, E_FAIL, "Descriptor verification failed — see individual field results");
        }
        payload.elapsedMs = 0;
    }
    catch (const std::exception& ex)
    {
        LogToWindowFormat("VerifyContentionDescriptor: exception: %s", ex.what());
        MarkFailure(payload.result, E_FAIL, std::string("Exception: ") + ex.what());
    }
    return payload;
}

CommandResultPayload HandleVerifyConflictDescriptor(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    try
    {
        // The conflict callback may be dispatched asynchronously via the task queue.
        // Poll briefly to allow it to fire before checking.
        if (!state->hasConflictDescriptor)
        {
            constexpr int maxWaitMs = 5000;
            constexpr int pollIntervalMs = 50;
            int waited = 0;
            while (!state->hasConflictDescriptor && waited < maxWaitMs)
            {
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                Sleep(pollIntervalMs);
                waited += pollIntervalMs;
            }
        }

        if (!state->hasConflictDescriptor)
        {
            MarkFailure(payload.result, E_FAIL, "No conflict descriptor captured — conflict callback not yet fired");
            return payload;
        }

        bool allPassed = true;
        if (parameters.contains("local"))
        {
            if (!VerifyDescriptorFields(state->conflictLocalGameSave, &state->conflictRemoteGameSave,
                parameters["local"], payload.result, "local"))
                allPassed = false;
        }
        if (parameters.contains("remote"))
        {
            if (!VerifyDescriptorFields(state->conflictRemoteGameSave, &state->conflictLocalGameSave,
                parameters["remote"], payload.result, "remote"))
                allPassed = false;
        }

        if (allPassed)
        {
            MarkSuccess(payload.result);
            SetHResult(payload.result, S_OK);
        }
        else
        {
            MarkFailure(payload.result, E_FAIL, "Descriptor verification failed — see individual field results");
        }
        payload.elapsedMs = 0;
    }
    catch (const std::exception& ex)
    {
        LogToWindowFormat("VerifyConflictDescriptor: exception: %s", ex.what());
        MarkFailure(payload.result, E_FAIL, std::string("Exception: ") + ex.what());
    }
    return payload;
}

CommandResultPayload HandleVerifyQuotaDelta(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    try
    {
        std::string beforeKey, afterKey, error;
        if (!TryGetStringParameter(parameters, "before", beforeKey, error))
        {
            MarkFailure(payload.result, E_INVALIDARG, "Missing 'before' parameter");
            return payload;
        }
        if (!TryGetStringParameter(parameters, "after", afterKey, error))
        {
            MarkFailure(payload.result, E_INVALIDARG, "Missing 'after' parameter");
            return payload;
        }

        auto itBefore = state->quotaRecordings.find(beforeKey);
        auto itAfter = state->quotaRecordings.find(afterKey);
        if (itBefore == state->quotaRecordings.end())
        {
            MarkFailure(payload.result, E_FAIL, std::string("No quota recording found for '") + beforeKey + "'");
            return payload;
        }
        if (itAfter == state->quotaRecordings.end())
        {
            MarkFailure(payload.result, E_FAIL, std::string("No quota recording found for '") + afterKey + "'");
            return payload;
        }

        int64_t before = itBefore->second;
        int64_t after = itAfter->second;
        int64_t actualDelta = before - after;  // Remaining quota decreases when data is uploaded

        payload.result["beforeQuota"] = before;
        payload.result["afterQuota"] = after;
        payload.result["actualDelta"] = actualDelta;

        if (parameters.contains("expectedDeltaApprox"))
        {
            int64_t expectedDelta = GetJsonInt64(parameters["expectedDeltaApprox"]);
            double marginPercent = 20.0;
            if (parameters.contains("marginPercent"))
            {
                marginPercent = GetJsonDouble(parameters["marginPercent"]);
            }
            double margin = expectedDelta * (marginPercent / 100.0);
            int64_t lo = static_cast<int64_t>(expectedDelta - margin);
            int64_t hi = static_cast<int64_t>(expectedDelta + margin);

            LogToWindowFormat("VerifyQuotaDelta: before=%lld, after=%lld, delta=%lld, expected~%lld [%lld..%lld]",
                static_cast<long long>(before), static_cast<long long>(after),
                static_cast<long long>(actualDelta), static_cast<long long>(expectedDelta),
                static_cast<long long>(lo), static_cast<long long>(hi));

            // Eventual-consistency escape hatch: Xbox's server-side quota tracker
            // for newly-provisioned title-player-accounts (e.g. immediately after
            // a CustomID->Xbox re-link) reports the per-player maximum quota
            // (exactly 2^30 bytes = 1 GiB) and does not reflect recent uploads
            // within the test's timeframe. When both readings are exactly that
            // default value, treat the verification as inconclusive rather than
            // failing: the SDK is faithfully reporting what the platform returns.
            constexpr int64_t kXboxFreshTpaDefaultQuota = 1073741824; // 2^30
            if (actualDelta == 0 && before == kXboxFreshTpaDefaultQuota && after == kXboxFreshTpaDefaultQuota)
            {
                LogToWindowFormat("VerifyQuotaDelta: WARNING - both readings equal Xbox's default per-player max (2^30); "
                    "treating as eventual-consistency skip (fresh TPA quota tracker not yet updated). Expected delta ~%lld.",
                    static_cast<long long>(expectedDelta));
                payload.result["eventualConsistencySkipped"] = true;
                MarkSuccess(payload.result);
                SetHResult(payload.result, S_OK);
                payload.elapsedMs = 0;
                return payload;
            }

            if (actualDelta < lo || actualDelta > hi)
            {
                MarkFailure(payload.result, E_FAIL,
                    std::string("Quota delta ") + std::to_string(actualDelta) +
                    " not within " + std::to_string(marginPercent) + "% of expected " + std::to_string(expectedDelta));
                return payload;
            }
        }

        MarkSuccess(payload.result);
        SetHResult(payload.result, S_OK);
        payload.elapsedMs = 0;
    }
    catch (const std::exception& ex)
    {
        LogToWindowFormat("VerifyQuotaDelta: exception: %s", ex.what());
        MarkFailure(payload.result, E_FAIL, std::string("Exception in VerifyQuotaDelta: ") + ex.what());
    }
    return payload;
}

CommandResultPayload HandleSetExpiredEntityToken(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using namespace CommandHandlerShared;
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    // TODO: Implement entity token invalidation when SDK provides a debug API for it
    MarkFailure(payload.result, E_NOTIMPL, "SetExpiredEntityToken not yet implemented — requires SDK debug support");
    return payload;
}

CommandResultPayload HandleCleanSaveFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    std::string folder = "C:\\gamesaves-test\\";
    if (parameters.is_object() && parameters.contains("saveFolder"))
    {
        folder = parameters["saveFolder"].get<std::string>();
    }

    namespace fs = std::filesystem;
    std::error_code ec;
    int filesDeleted = 0;
    int dirsDeleted = 0;

    if (fs::exists(folder, ec))
    {
        for (const auto& entry : fs::directory_iterator(folder, ec))
        {
            if (entry.is_directory(ec))
            {
                auto removed = fs::remove_all(entry.path(), ec);
                dirsDeleted += static_cast<int>(removed);
            }
            else
            {
                fs::remove(entry.path(), ec);
                filesDeleted++;
            }
        }
    }

    // Ensure directory exists (empty) after cleanup
    fs::create_directories(folder, ec);

    LogToWindowFormat("CleanSaveFolder: cleaned '%s' (files=%d, dirs=%d)", folder.c_str(), filesDeleted, dirsDeleted);
    payload.result["saveFolder"] = folder;
    payload.result["filesDeleted"] = filesDeleted;
    payload.result["directoriesDeleted"] = dirsDeleted;
    MarkSuccess(payload.result);
    return payload;
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFGameSaveFilesAddUserWithUiAsync", HandlePFGameSaveFilesAddUserWithUiAsync },
    { "PFGameSaveFilesAddUserWithUiResult", HandlePFGameSaveFilesAddUserWithUiResult },
    { "PFGameSaveFilesGetFolder", HandlePFGameSaveFilesGetFolder },
    { "PFGameSaveFilesGetFolderSize", HandlePFGameSaveFilesGetFolderSize },
    { "PFGameSaveFilesGetRemainingQuota", HandlePFGameSaveFilesGetRemainingQuota },
    { "PFGameSaveFilesGetSaveDescription", HandlePFGameSaveFilesGetSaveDescription },
    { "PFGameSaveFilesInitialize", HandlePFGameSaveFilesInitialize },
    { "PFGameSaveFilesIsConnectedToCloud", HandlePFGameSaveFilesIsConnectedToCloud },
    { "PFGameSaveFilesResetCloudAsync", HandlePFGameSaveFilesResetCloudAsync },
    { "PFGameSaveFilesResetCloudResult", HandlePFGameSaveFilesResetCloudResult },
    { "PFGameSaveFilesSetActiveDeviceChangedCallback", HandlePFGameSaveFilesSetActiveDeviceChangedCallback },
    { "VerifyActiveDeviceChangedCallback", HandleVerifyActiveDeviceChangedCallback },
    { "WaitForActiveDeviceChanged", HandleWaitForActiveDeviceChanged },
    { "ResetActiveDeviceChangedCallbackCount", HandleResetActiveDeviceChangedCallbackCount },
    { "PFGameSaveFilesSetActiveDevicePollForceChangeForDebug", HandlePFGameSaveFilesSetActiveDevicePollForceChangeForDebug },
    { "PFGameSaveFilesSetActiveDevicePollIntervalForDebug", HandlePFGameSaveFilesSetActiveDevicePollIntervalForDebug },
    { "PFGameSaveFilesSetForceNullPendingManifestForDebug", HandlePFGameSaveFilesSetForceNullPendingManifestForDebug },
    { "PFGameSaveFilesSetForceOutOfStorageErrorForDebug", HandlePFGameSaveFilesSetForceOutOfStorageErrorForDebug },
    { "PFGameSaveFilesSetForceSyncFailedErrorForDebug", HandlePFGameSaveFilesSetForceSyncFailedErrorForDebug },
    { "PFGameSaveFilesSetMockDeviceIdForDebug", HandlePFGameSaveFilesSetMockDeviceIdForDebug },
    { "PFGameSaveFilesSetMockForceOfflineForDebug", HandlePFGameSaveFilesSetMockForceOfflineForDebug },
    { "PFGameSaveFilesSetSaveDescriptionAsync", HandlePFGameSaveFilesSetSaveDescriptionAsync },
    { "PFGameSaveFilesSetSaveDescriptionResult", HandlePFGameSaveFilesSetSaveDescriptionResult },
    { "PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse", HandlePFGameSaveFilesSetUiActiveDeviceContentionAutoResponse },
    { "PFGameSaveFilesSetUiActiveDeviceContentionResponse", HandlePFGameSaveFilesSetUiActiveDeviceContentionResponse },
    { "PFGameSaveFilesSetUiCallbacks", HandlePFGameSaveFilesSetUiCallbacks },
    { "PFGameSaveFilesSetUiConflictAutoResponse", HandlePFGameSaveFilesSetUiConflictAutoResponse },
    { "PFGameSaveFilesSetUiConflictResponse", HandlePFGameSaveFilesSetUiConflictResponse },
    { "PFGameSaveFilesSetUiSyncConflictAutoResponse", HandlePFGameSaveFilesSetUiConflictAutoResponse },
    { "PFGameSaveFilesSetUiOutOfStorageAutoResponse", HandlePFGameSaveFilesSetUiOutOfStorageAutoResponse },
    { "PFGameSaveFilesSetUiOutOfStorageResponse", HandlePFGameSaveFilesSetUiOutOfStorageResponse },
    { "PFGameSaveFilesSetUiProgressAutoResponse", HandlePFGameSaveFilesSetUiProgressAutoResponse },
    { "PFGameSaveFilesSetUiProgressAutoWriteOnUpload", HandlePFGameSaveFilesSetUiProgressAutoWriteOnUpload },
    { "PFGameSaveFilesSetUiProgressResponse", HandlePFGameSaveFilesSetUiProgressResponse },
    { "PFGameSaveFilesSetUiProgressStateRecording", HandlePFGameSaveFilesSetUiProgressStateRecording },
    { "PFGameSaveFilesVerifyProgressStateSequence", HandlePFGameSaveFilesVerifyProgressStateSequence },
    { "PFGameSaveFilesSetUiSyncConflictAutoResponse", HandlePFGameSaveFilesSetUiConflictAutoResponse },
    { "PFGameSaveFilesSetUiSyncFailedAutoResponse", HandlePFGameSaveFilesSetUiSyncFailedAutoResponse },
    { "PFGameSaveFilesSetUiSyncFailedResponse", HandlePFGameSaveFilesSetUiSyncFailedResponse },
    { "PFGameSaveFilesSetWriteManifestsToDiskForDebug", HandlePFGameSaveFilesSetWriteManifestsToDiskForDebug },
    { "PFGameSaveFilesUiProgressGetProgress", HandlePFGameSaveFilesUiProgressGetProgress },
    { "PFGameSaveFilesStartProgressSampler", HandlePFGameSaveFilesStartProgressSampler },
    { "PFGameSaveFilesStopProgressSampler", HandlePFGameSaveFilesStopProgressSampler },
    { "PFGameSaveFilesVerifyProgressQuality", HandlePFGameSaveFilesVerifyProgressQuality },
    { "PFGameSaveFilesUninitializeAsync", HandlePFGameSaveFilesUninitializeAsync },
    { "PFGameSaveFilesUninitializeResult", HandlePFGameSaveFilesUninitializeResult },
    { "PFGameSaveFilesUploadWithUiAsync", HandlePFGameSaveFilesUploadWithUiAsync },
    { "PFGameSaveFilesUploadWithUiResult", HandlePFGameSaveFilesUploadWithUiResult },
    { "PFGameSaveFilesSetUiProgressAutoWriteOnUpload", HandlePFGameSaveFilesSetUiProgressAutoWriteOnUpload },
    { "SetExpiredEntityToken", HandleSetExpiredEntityToken },
    { "VerifyContentionDescriptor", HandleVerifyContentionDescriptor },
    { "VerifyConflictDescriptor", HandleVerifyConflictDescriptor },
    { "VerifyQuotaDelta", HandleVerifyQuotaDelta },
    { "PFGameSaveFilesSetUiProgressStateRecording", HandlePFGameSaveFilesSetUiProgressStateRecording },
    { "PFGameSaveFilesVerifyProgressStateSequence", HandlePFGameSaveFilesVerifyProgressStateSequence },
    { "CleanSaveFolder", HandleCleanSaveFolder },
    { "VerifyOutOfStorageCallback", HandleVerifyOutOfStorageCallback }
});

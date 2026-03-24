#include "pch.h"

#include "ConsumeDiskSpace.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandHandlerShared.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <mutex>
#include <random>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include <playfab/gamesave/PFGameSaveFiles.h>

namespace
{
    namespace fs = std::filesystem;

    struct ReservationRecord
    {
        std::string scope;
        fs::path basePath;
        fs::path reservationPath;
        std::vector<fs::path> directories;
        std::vector<fs::path> files;
        int64_t bytesAllocated{ 0 };
    };

    std::mutex g_reservationsMutex;
    std::unordered_map<std::string, ReservationRecord> g_reservations;
    std::atomic<uint64_t> g_reservationCounter{ 1 };

    std::string NormalizeScope(std::string scope)
    {
        std::transform(scope.begin(), scope.end(), scope.begin(), [](unsigned char ch)
        {
            return static_cast<char>(std::tolower(ch));
        });

        if (scope.empty())
        {
            return "user";
        }

        return scope;
    }

    HRESULT FetchSaveFolder(DeviceGameSaveState* state, fs::path& folderOut, std::string& error)
    {
        if (!state || !state->localUserHandle)
        {
            error = "Local user handle not created";
            return E_POINTER;
        }

        if (!state->saveFolder.empty())
        {
            folderOut = fs::path(state->saveFolder);
            return S_OK;
        }

        size_t folderSize = 0;
        HRESULT hr = PFGameSaveFilesGetFolderSize(state->localUserHandle, &folderSize);
        if (FAILED(hr))
        {
            error = "PFGameSaveFilesGetFolderSize failed";
            return hr;
        }

        std::vector<char> buffer(folderSize > 0 ? folderSize : 1);
        size_t used = 0;
        hr = PFGameSaveFilesGetFolder(state->localUserHandle, buffer.size(), buffer.data(), &used);
        if (FAILED(hr))
        {
            error = "PFGameSaveFilesGetFolder failed";
            return hr;
        }

        size_t stringLength = (used > 0 && used <= buffer.size()) ? used - 1 : 0;
        state->saveFolder.assign(buffer.data(), buffer.data() + stringLength);
        folderOut = fs::path(state->saveFolder);
        return S_OK;
    }

    HRESULT ResolveScopeRoot(
        DeviceGameSaveState* state,
        const std::string& scope,
        const std::string& customPath,
        fs::path& rootOut,
        std::string& error)
    {
        if (scope == "user" || scope.empty())
        {
            HRESULT hr = FetchSaveFolder(state, rootOut, error);
            if (FAILED(hr))
            {
                return hr;
            }

            if (rootOut.empty())
            {
                error = "Save folder path is empty";
                return E_FAIL;
            }

            return S_OK;
        }

        if (scope == "system")
        {
            std::error_code ec;
            rootOut = fs::temp_directory_path(ec);
            if (ec)
            {
                error = std::string("Failed to resolve temp directory: ") + ec.message();
                return HRESULT_FROM_WIN32(static_cast<uint32_t>(ec.value()));
            }

            return S_OK;
        }

        if (scope == "custom")
        {
            if (customPath.empty())
            {
                error = "Custom scope requires 'customPath' parameter";
                return E_INVALIDARG;
            }

            rootOut = fs::path(customPath);
            return S_OK;
        }

        error = "Unsupported scope value";
        return E_INVALIDARG;
    }

    std::string GenerateToken()
    {
        const uint64_t value = g_reservationCounter.fetch_add(1);
        std::ostringstream oss;
        oss << "pfres-" << value;
        return oss.str();
    }
}

ConsumeDiskSpaceResult ExecuteConsumeDiskSpace(
    DeviceGameSaveState* state,
    int64_t bytes,
    std::string scope,
    std::string customPath,
    std::string token)
{
    ConsumeDiskSpaceResult result{};
    result.bytesRequested = bytes;

    if (bytes <= 0)
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = "Parameter 'bytes' must be greater than zero";
        return result;
    }

    scope = NormalizeScope(scope);
    result.scope = scope.empty() ? "user" : scope;

    fs::path root;
    std::string resolveError;
    HRESULT hr = ResolveScopeRoot(state, result.scope, customPath, root, resolveError);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = resolveError;
        return result;
    }

    std::error_code ec;
    if (!fs::exists(root, ec))
    {
        ec.clear();
        fs::create_directories(root, ec);
        if (ec)
        {
            result.hr = HRESULT_FROM_WIN32(static_cast<uint32_t>(ec.value()));
            std::ostringstream oss;
            oss << "Failed to ensure scope root exists: " << ec.message();
            result.errorMessage = oss.str();
            return result;
        }
    }

    if (token.empty())
    {
        token = GenerateToken();
    }

    result.token = token;

    result.resolvedPath = root / "pf_gs_filler" / token;

    // TODO: Replace with PFGameSaves disk reservation mock when available in the PFGameSaves DLL.
    result.hr = HRESULT_FROM_WIN32(ERROR_CALL_NOT_IMPLEMENTED);
    result.errorMessage = "ConsumeDiskSpace mock not implemented";
    return result;
}

ReleaseDiskSpaceResult ExecuteReleaseDiskSpace(
    DeviceGameSaveState* state,
    std::string scope,
    std::string token)
{
    UNREFERENCED_PARAMETER(state);

    ReleaseDiskSpaceResult result{};
    std::string normalizedScope = NormalizeScope(scope);
    if (normalizedScope.empty())
    {
        normalizedScope = "user";
    }
    result.scope = normalizedScope;
    result.token = token;

    // TODO: Replace with PFGameSaves disk reservation mock when available in the PFGameSaves DLL.
    result.hr = HRESULT_FROM_WIN32(ERROR_CALL_NOT_IMPLEMENTED);
    result.errorMessage = "ReleaseDiskSpace mock not implemented";
    return result;
}

CommandResultPayload HandleConsumeDiskSpace(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state || !state->localUserHandle)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        CommandHandlerShared::SetHResult(payload.result, E_POINTER);
        return payload;
    }

    int64_t bytes = 0;
    std::string parseError;
    if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bytes", bytes, parseError))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, parseError);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        return payload;
    }

    std::string scope = "user";
    if (parameters.is_object())
    {
        auto scopeIt = parameters.find("scope");
        if (scopeIt != parameters.end())
        {
            if (!scopeIt->is_string())
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Parameter 'scope' must be a string");
                CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            scope = scopeIt->get<std::string>();
        }
    }

    std::string customPath;
    if (parameters.is_object())
    {
        auto customPathIt = parameters.find("customPath");
        if (customPathIt != parameters.end())
        {
            if (!customPathIt->is_string())
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Parameter 'customPath' must be a string");
                CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            customPath = customPathIt->get<std::string>();
        }
    }

    std::string token;
    if (parameters.is_object())
    {
        auto tokenIt = parameters.find("token");
        if (tokenIt != parameters.end())
        {
            if (!tokenIt->is_string())
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Parameter 'token' must be a string");
                CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            token = tokenIt->get<std::string>();
        }
    }

    const auto start = std::chrono::steady_clock::now();
    const ConsumeDiskSpaceResult result = ExecuteConsumeDiskSpace(state, bytes, scope, customPath, token);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["scope"] = result.scope;
    payload.result["token"] = result.token;
    payload.result["bytesRequested"] = result.bytesRequested;
    payload.result["bytesAllocated"] = result.bytesAllocated;
    payload.result["filesCreated"] = result.filesCreated;
    payload.result["directoriesCreated"] = result.directoriesCreated;
    payload.result["resolvedPath"] = result.resolvedPath.u8string();

    if (!result.notes.empty())
    {
        nlohmann::json notesArray = nlohmann::json::array();
        for (const auto& note : result.notes)
        {
            notesArray.push_back(note);
        }
        payload.result["notes"] = std::move(notesArray);
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandleReleaseDiskSpace(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state || !state->localUserHandle)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        CommandHandlerShared::SetHResult(payload.result, E_POINTER);
        return payload;
    }

    std::string scope = "user";
    if (parameters.is_object())
    {
        auto scopeIt = parameters.find("scope");
        if (scopeIt != parameters.end())
        {
            if (!scopeIt->is_string())
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Parameter 'scope' must be a string");
                CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            scope = scopeIt->get<std::string>();
        }
    }

    std::string token;
    if (parameters.is_object())
    {
        auto tokenIt = parameters.find("token");
        if (tokenIt != parameters.end())
        {
            if (!tokenIt->is_string())
            {
                CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Parameter 'token' must be a string");
                CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
                return payload;
            }

            token = tokenIt->get<std::string>();
        }
    }

    const auto start = std::chrono::steady_clock::now();
    const ReleaseDiskSpaceResult result = ExecuteReleaseDiskSpace(state, scope, token);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["scope"] = result.scope;
    if (!result.token.empty())
    {
        payload.result["token"] = result.token;
    }
    payload.result["filesRemoved"] = result.filesRemoved;
    payload.result["directoriesRemoved"] = result.directoriesRemoved;
    payload.result["bytesFreed"] = result.bytesFreed;

    if (!result.residualFiles.empty())
    {
        nlohmann::json residual = nlohmann::json::array();
        for (const auto& entry : result.residualFiles)
        {
            residual.push_back(entry);
        }
        payload.result["residualFiles"] = std::move(residual);
    }

    if (!result.notes.empty())
    {
        nlohmann::json notesArray = nlohmann::json::array();
        for (const auto& note : result.notes)
        {
            notesArray.push_back(note);
        }
        payload.result["notes"] = std::move(notesArray);
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

#include "pch.h"

#include "DeleteSaveRoot.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "DeviceFileSystem.h"
#include "CommandHandlerShared.h"
#include "ResolveSaveRoot.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include <playfab/gamesave/PFGameSaveFiles.h>
#include "CommandRegistry.h"

namespace
{
    namespace fs = std::filesystem;

    std::string ToLowerCopy(std::string text)
    {
        std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch)
        {
            return static_cast<char>(std::tolower(ch));
        });
        return text;
    }

    HRESULT ConvertFilesystemError(const std::error_code& ec)
    {
        if (!ec)
        {
            return S_OK;
        }

        if (ec.category() == std::system_category())
        {
            return HRESULT_FROM_WIN32(static_cast<uint32_t>(ec.value()));
        }

        return E_FAIL;
    }

    bool IsRootLevelCloudsync(const fs::path& relativePath)
    {
        // Only preserve cloudsync folders that sit directly under the save root.
        // Nested ones (e.g., DeviceB/cloudsync) are stale leftovers and should be deleted.
        auto it = relativePath.begin();
        if (it == relativePath.end())
        {
            return false;
        }
        std::string first = ToLowerCopy(GetStringFromU8String(it->generic_u8string()));
        return first == "cloudsync";
    }

    struct DeleteContext
    {
        fs::path root;
        DeleteSaveRootResult* result{ nullptr };
    };

    bool DeleteDirectoryContents(
        DeleteContext& context,
        const fs::path& directory,
        bool isRoot,
        bool& directoryEmptyOut,
        HRESULT& failureHr,
        std::string& error)
    {
        LogToWindowFormat("DeleteDirectoryContents: attempting to enumerate directory='%s' (isRoot=%s)",
            GetStringFromU8String(directory.u8string()).c_str(), isRoot ? "true" : "false");

        // Pre-flight: check if directory exists and is accessible
        {
            std::error_code preEc;
            bool exists = fs::exists(directory, preEc);
            if (preEc)
            {
                LogToWindowFormat("DeleteDirectoryContents: fs::exists pre-check failed (error: %s, code=%d, category=%s)",
                    preEc.message().c_str(), preEc.value(), preEc.category().name());
            }
            else
            {
                LogToWindowFormat("DeleteDirectoryContents: fs::exists='%s'", exists ? "true" : "false");
                if (!exists)
                {
                    // Directory doesn't exist — nothing to delete, treat as success
                    LogToWindowFormat("DeleteDirectoryContents: directory does not exist, nothing to delete");
                    directoryEmptyOut = true;
                    return true;
                }
            }

            bool isDir = fs::is_directory(directory, preEc);
            if (preEc)
            {
                LogToWindowFormat("DeleteDirectoryContents: fs::is_directory pre-check failed (error: %s, code=%d, category=%s)",
                    preEc.message().c_str(), preEc.value(), preEc.category().name());
            }
            else
            {
                LogToWindowFormat("DeleteDirectoryContents: fs::is_directory='%s'", isDir ? "true" : "false");
            }
        }

        std::error_code ec;
        fs::directory_iterator end;
        fs::directory_iterator it(directory, fs::directory_options::skip_permission_denied, ec);
        if (ec)
        {
            failureHr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to enumerate directory '" << directory << "': " << ec.message();
            error = oss.str();
            LogToWindowFormat("DeleteDirectoryContents: directory_iterator FAILED (error: %s, code=%d, category=%s, hr=0x%08X)",
                ec.message().c_str(), ec.value(), ec.category().name(), failureHr);
            return false;
        }

        directoryEmptyOut = true;
        for (; it != end; it.increment(ec))
        {
            if (ec)
            {
                failureHr = ConvertFilesystemError(ec);
                std::ostringstream oss;
                oss << "Failed to iterate directory '" << directory << "': " << ec.message();
                error = oss.str();
                return false;
            }

            const fs::path entryPath = it->path();
            fs::path relative;
            relative = fs::relative(entryPath, context.root, ec);
            if (ec)
            {
                ec.clear();
                relative = entryPath.filename();
            }

            std::error_code typeEc;
            const bool isDirectory = it->is_directory(typeEc);
            if (typeEc)
            {
                failureHr = ConvertFilesystemError(typeEc);
                std::ostringstream oss;
                oss << "Failed to query entry type for '" << entryPath << "': " << typeEc.message();
                error = oss.str();
                return false;
            }

            typeEc.clear();
            const bool isSymlink = it->is_symlink(typeEc);
            if (typeEc)
            {
                failureHr = ConvertFilesystemError(typeEc);
                std::ostringstream oss;
                oss << "Failed to query entry symlink state for '" << entryPath << "': " << typeEc.message();
                error = oss.str();
                return false;
            }

            if (isDirectory && !isSymlink)
            {
                if (IsRootLevelCloudsync(relative))
                {
                    directoryEmptyOut = false;
                    context.result->preservedEntries.push_back(GetStringFromU8String(relative.generic_u8string()));
                    continue;
                }

                bool childEmpty = true;
                if (!DeleteDirectoryContents(context, entryPath, false, childEmpty, failureHr, error))
                {
                    return false;
                }

                if (childEmpty)
                {
                    std::error_code removeEc;
                    fs::remove(entryPath, removeEc);
                    if (removeEc)
                    {
                        failureHr = ConvertFilesystemError(removeEc);
                        std::ostringstream oss;
                        oss << "Failed to remove directory '" << entryPath << "': " << removeEc.message();
                        error = oss.str();
                        return false;
                    }

                    context.result->directoriesDeleted++;
                }
                else
                {
                    directoryEmptyOut = false;
                }
                continue;
            }

            if (IsRootLevelCloudsync(relative))
            {
                directoryEmptyOut = false;
                context.result->preservedEntries.push_back(GetStringFromU8String(relative.generic_u8string()));
                continue;
            }

            std::error_code removeEc;
            fs::remove(entryPath, removeEc);
            if (removeEc)
            {
                failureHr = ConvertFilesystemError(removeEc);
                std::ostringstream oss;
                oss << "Failed to remove file '" << entryPath << "': " << removeEc.message();
                error = oss.str();
                return false;
            }

            context.result->filesDeleted++;
        }

        if (isRoot)
        {
            // Root directory remains; nothing else to do.
            return true;
        }

        // bool indicates whether directory is empty so callers can decide if removal is needed.
        return true;
    }
}

DeleteSaveRootResult ExecuteDeleteSaveRoot(
    DeviceGameSaveState* state,
    bool preserveManifest)
{
    DeleteSaveRootResult result{};

    if (!state || !state->localUserHandle)
    {
        result.hr = E_POINTER;
        result.errorMessage = "Local user handle not created";
        return result;
    }

    LogToWindowFormat("ExecuteDeleteSaveRoot: state->saveFolder='%s' (length=%zu), preserveManifest=%s",
        state->saveFolder.c_str(), state->saveFolder.size(), preserveManifest ? "true" : "false");

    fs::path rootPath;
    std::string resolveError;
    HRESULT hr = ResolveSaveRoot(state, rootPath, resolveError);
    if (FAILED(hr))
    {
        // If the save root can't be resolved (e.g., no user added yet on Xbox/GRTS),
        // treat as success — there's nothing to delete if we can't find the path.
        LogToWindowFormat("ExecuteDeleteSaveRoot: ResolveSaveRoot returned hr=0x%08X (%s); treating as no-op success",
            hr, resolveError.c_str());
        result.hr = S_OK;
        return result;
    }

    result.saveFolder = GetStringFromU8String(rootPath.u8string());
    LogToWindowFormat("ExecuteDeleteSaveRoot: resolved rootPath='%s'", result.saveFolder.c_str());

    auto mountGuard = DeviceFileSystemMount(state->saveFolder);

    DeleteContext context;
    context.root = rootPath;
    context.result = &result;

    HRESULT deleteHr = S_OK;
    std::string deleteError;
    bool rootEmpty = true;
    if (!DeleteDirectoryContents(context, rootPath, true, rootEmpty, deleteHr, deleteError))
    {
        result.hr = deleteHr;
        result.errorMessage = deleteError;
        return result;
    }

    if (!preserveManifest)
    {
        const fs::path cloudsyncPath = rootPath / "cloudsync";
        std::error_code cloudsyncExistsEc;
        const bool cloudsyncExists = fs::exists(cloudsyncPath, cloudsyncExistsEc);
        if (cloudsyncExistsEc)
        {
            LogToWindowFormat(
                "DeleteSaveRoot: failed to query cloudsync folder '%s' (error: %s)",
                cloudsyncPath.u8string().c_str(),
                cloudsyncExistsEc.message().c_str());
        }
        else if (cloudsyncExists)
        {
            std::error_code deleteEc;
            fs::remove_all(cloudsyncPath, deleteEc);
            if (deleteEc)
            {
                LogToWindowFormat(
                    "DeleteSaveRoot: failed to remove cloudsync folder '%s' (error: %s)",
                    cloudsyncPath.u8string().c_str(),
                    deleteEc.message().c_str());
            }
            else
            {
                LogToWindowFormat(
                    "DeleteSaveRoot removed cloudsync folder '%s'",
                    cloudsyncPath.u8string().c_str());
            }
        }
    }

    result.manifestPreserved = !result.preservedEntries.empty();
    result.hr = S_OK;

    // Write a sentinel file so the save root is never truly empty.
    // Console-flash's data-loss protection fires incorrectly when local is empty
    // but cloud has data (see investigations/07-01-empty-xvd-dataloss).
    {
        const fs::path sentinelPath = rootPath / "reset-marker.txt";
        std::ofstream sentinel(sentinelPath, std::ios::trunc);
        if (sentinel.is_open())
        {
            sentinel << "Reset at this location. This file prevents empty-folder data loss protection.\n";
            sentinel.close();
            LogToWindowFormat("DeleteSaveRoot: wrote sentinel '%s'", sentinelPath.u8string().c_str());
        }
        else
        {
            LogToWindowFormat("DeleteSaveRoot: WARNING — failed to write sentinel '%s'", sentinelPath.u8string().c_str());
        }
    }

    LogToWindowFormat(
        "DeleteSaveRoot removed %d files, %d directories%s",
        result.filesDeleted,
        result.directoriesDeleted,
        result.manifestPreserved ? " (manifest preserved)" : "");

    return result;
}

CommandResultPayload HandleDeleteSaveRoot(
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
        return payload;
    }

    bool preserveManifest = true;
    std::string parseError;
    if (!CommandHandlerShared::TryParseBoolParameter(parameters, "preserveManifest", preserveManifest, parseError))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, parseError);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        return payload;
    }

    const auto start = std::chrono::steady_clock::now();
    const DeleteSaveRootResult result = ExecuteDeleteSaveRoot(state, preserveManifest);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["saveFolder"] = result.saveFolder;
    payload.result["filesDeleted"] = result.filesDeleted;
    payload.result["directoriesDeleted"] = result.directoriesDeleted;
    payload.result["preserveManifest"] = preserveManifest;
    payload.result["manifestPreserved"] = result.manifestPreserved;

    if (!result.preservedEntries.empty())
    {
        nlohmann::json preserved = nlohmann::json::array();
        for (const auto& entry : result.preservedEntries)
        {
            preserved.push_back(entry);
        }

        payload.result["preservedEntries"] = std::move(preserved);
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "DeleteSaveRoot", HandleDeleteSaveRoot }
});

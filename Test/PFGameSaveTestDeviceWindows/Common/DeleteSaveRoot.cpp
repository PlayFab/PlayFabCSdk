#include "pch.h"

#include "DeleteSaveRoot.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandHandlerShared.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <filesystem>
#include <sstream>
#include <utility>

#include <playfab/gamesave/PFGameSaveFiles.h>

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

    HRESULT FetchSaveFolder(DeviceGameSaveState* state, std::string& folder)
    {
        if (!state || !state->localUserHandle)
        {
            return E_POINTER;
        }

        if (!state->saveFolder.empty())
        {
            folder = state->saveFolder;
            return S_OK;
        }

        size_t folderSize = 0;
        HRESULT hr = PFGameSaveFilesGetFolderSize(state->localUserHandle, &folderSize);
        if (FAILED(hr))
        {
            return hr;
        }

        std::vector<char> buffer(folderSize > 0 ? folderSize : 1);
        size_t used = 0;
        hr = PFGameSaveFilesGetFolder(state->localUserHandle, buffer.size(), buffer.data(), &used);
        if (FAILED(hr))
        {
            return hr;
        }

        size_t stringLength = (used > 0 && used <= buffer.size()) ? used - 1 : 0;
        folder.assign(buffer.data(), buffer.data() + stringLength);
        state->saveFolder = folder;
        return S_OK;
    }

    HRESULT ResolveSaveRoot(DeviceGameSaveState* state, fs::path& rootPath, std::string& error)
    {
        std::string folder;
        HRESULT hr = FetchSaveFolder(state, folder);
        if (FAILED(hr))
        {
            error = "Failed to resolve save root via PFGameSaveFilesGetFolder";
            return hr;
        }

        if (folder.empty())
        {
            error = "PFGameSaveFilesGetFolder returned an empty path";
            return E_FAIL;
        }

        rootPath = fs::path(folder);
        std::error_code ec;
        fs::create_directories(rootPath, ec);
        if (ec)
        {
            error = std::string("Failed to ensure save root directory exists: ") + ec.message();
            return ConvertFilesystemError(ec);
        }

        return S_OK;
    }

    bool HasReservedCloudsyncComponent(const fs::path& relativePath)
    {
        for (const auto& part : relativePath)
        {
            std::string lower = ToLowerCopy(GetStringFromU8String(part.generic_u8string()));
            if (lower == "cloudsync")
            {
                return true;
            }
        }

        return false;
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
        std::error_code ec;
        fs::directory_iterator end;
        fs::directory_iterator it(directory, fs::directory_options::skip_permission_denied, ec);
        if (ec)
        {
            failureHr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to enumerate directory '" << directory << "': " << ec.message();
            error = oss.str();
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
                if (HasReservedCloudsyncComponent(relative))
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

            if (HasReservedCloudsyncComponent(relative))
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

    fs::path rootPath;
    std::string resolveError;
    HRESULT hr = ResolveSaveRoot(state, rootPath, resolveError);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = resolveError;
        return result;
    }

    result.saveFolder = GetStringFromU8String(rootPath.u8string());

    std::error_code existsEc;
    const bool rootExists = fs::exists(rootPath, existsEc);
    if (existsEc)
    {
        result.hr = ConvertFilesystemError(existsEc);
        std::ostringstream oss;
        oss << "Failed to query save folder existence: " << existsEc.message();
        result.errorMessage = oss.str();
        return result;
    }

    if (!rootExists)
    {
        result.hr = S_OK;
        LogToWindowFormat("DeleteSaveRoot: save folder does not exist (%s)", result.saveFolder.c_str());
        return result;
    }

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

    if (!state->localUserHandle)
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

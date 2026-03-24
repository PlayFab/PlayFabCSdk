#include "pch.h"

#include "CopyTargetFolderToSaveFolder.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <sstream>

namespace
{
    namespace fs = std::filesystem;

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

    void CopyDirectoryRecursive(
        const fs::path& source,
        const fs::path& destination,
        std::uintmax_t& filesCopied,
        std::uintmax_t& directoriesCreated,
        std::uintmax_t& totalBytesCopied,
        std::error_code& ec)
    {
        // Create destination directory if it doesn't exist
        if (!fs::exists(destination, ec))
        {
            if (ec)
            {
                return;
            }
            
            fs::create_directories(destination, ec);
            if (ec)
            {
                return;
            }
            directoriesCreated++;
        }

        // Iterate through source directory
        for (const auto& entry : fs::directory_iterator(source, ec))
        {
            if (ec)
            {
                return;
            }

            const fs::path currentSource = entry.path();
            const fs::path currentDestination = destination / currentSource.filename();

            if (entry.is_directory(ec))
            {
                if (ec)
                {
                    return;
                }

                // Recursively copy subdirectory
                CopyDirectoryRecursive(
                    currentSource,
                    currentDestination,
                    filesCopied,
                    directoriesCreated,
                    totalBytesCopied,
                    ec);

                if (ec)
                {
                    return;
                }
            }
            else if (entry.is_regular_file(ec))
            {
                if (ec)
                {
                    return;
                }

                // Copy file
                fs::copy_file(
                    currentSource,
                    currentDestination,
                    fs::copy_options::overwrite_existing,
                    ec);

                if (ec)
                {
                    return;
                }

                filesCopied++;

                // Track bytes copied
                const auto fileSize = entry.file_size(ec);
                if (!ec)
                {
                    totalBytesCopied += fileSize;
                }
            }
        }
    }
}

CopyTargetFolderToSaveFolderResult ExecuteCopyTargetFolderToSaveFolder(
    DeviceGameSaveState* state,
    std::string sourceFolder)
{
    CopyTargetFolderToSaveFolderResult result{};
    result.sourceFolder = std::move(sourceFolder);

    if (!state)
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = "Device state cannot be null";
        return result;
    }

    if (result.sourceFolder.empty())
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = "Parameter 'sourceFolder' cannot be empty";
        return result;
    }

    // Get the save root folder from the device state
    if (state->saveFolder.empty())
    {
        result.hr = E_FAIL;
        result.errorMessage = "Save folder not initialized. Call PFGameSaveFilesGetFolder first.";
        return result;
    }

    result.destinationFolder = state->saveFolder;

    fs::path sourcePath(result.sourceFolder);
    fs::path destinationPath(result.destinationFolder);

    // Check if source exists
    std::error_code existsEc;
    const bool sourceExists = fs::exists(sourcePath, existsEc);
    if (existsEc)
    {
        result.hr = ConvertFilesystemError(existsEc);
        std::ostringstream oss;
        oss << "CopyTargetFolderToSaveFolder: failed to query source path existence for '"
            << result.sourceFolder << "' (error: " << existsEc.message() << ")";
        result.errorMessage = oss.str();
        return result;
    }

    if (!sourceExists)
    {
        result.hr = HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND);
        std::ostringstream oss;
        oss << "CopyTargetFolderToSaveFolder: source folder does not exist: '" << result.sourceFolder << "'";
        result.errorMessage = oss.str();
        LogToWindow(result.errorMessage);
        return result;
    }

    // Check if source is a directory
    if (!fs::is_directory(sourcePath, existsEc))
    {
        result.hr = existsEc ? ConvertFilesystemError(existsEc) : E_INVALIDARG;
        std::ostringstream oss;
        oss << "CopyTargetFolderToSaveFolder: source path is not a directory: '" << result.sourceFolder << "'";
        result.errorMessage = oss.str();
        LogToWindow(result.errorMessage);
        return result;
    }

    LogToWindowFormat(
        "CopyTargetFolderToSaveFolder: copying from '%s' to '%s'",
        result.sourceFolder.c_str(),
        result.destinationFolder.c_str());

    // Copy recursively
    std::error_code copyEc;
    CopyDirectoryRecursive(
        sourcePath,
        destinationPath,
        result.filesCopied,
        result.directoriesCreated,
        result.totalBytesCopied,
        copyEc);

    if (copyEc)
    {
        result.hr = ConvertFilesystemError(copyEc);
        std::ostringstream oss;
        oss << "CopyTargetFolderToSaveFolder: failed to copy from '" << result.sourceFolder
            << "' to '" << result.destinationFolder << "' (error: " << copyEc.message() << ")";
        result.errorMessage = oss.str();
        LogToWindow(result.errorMessage);
        return result;
    }

    result.hr = S_OK;

    LogToWindowFormat(
        "CopyTargetFolderToSaveFolder copied %llu files (%llu directories, %llu bytes) from '%s' to '%s'",
        static_cast<unsigned long long>(result.filesCopied),
        static_cast<unsigned long long>(result.directoriesCreated),
        static_cast<unsigned long long>(result.totalBytesCopied),
        result.sourceFolder.c_str(),
        result.destinationFolder.c_str());

    return result;
}

CommandResultPayload HandleCopyTargetFolderToSaveFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    using CommandHandlerShared::CreateBaseResult;
    using CommandHandlerShared::MarkSuccess;
    using CommandHandlerShared::MarkFailure;

    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    auto startTime = std::chrono::high_resolution_clock::now();

    std::string sourceFolder;
    if (parameters.contains("sourceFolder") && parameters["sourceFolder"].is_string())
    {
        sourceFolder = parameters["sourceFolder"].get<std::string>();
    }

    auto result = ExecuteCopyTargetFolderToSaveFolder(state, std::move(sourceFolder));

    if (SUCCEEDED(result.hr))
    {
        nlohmann::json details;
        details["sourceFolder"] = result.sourceFolder;
        details["destinationFolder"] = result.destinationFolder;
        details["filesCopied"] = result.filesCopied;
        details["directoriesCreated"] = result.directoriesCreated;
        details["totalBytesCopied"] = result.totalBytesCopied;

        payload.result["details"] = details;
        MarkSuccess(payload.result);
    }
    else
    {
        MarkFailure(payload.result, result.hr, result.errorMessage);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    payload.elapsedMs = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

    return payload;
}

#include "pch.h"

#include "DeleteLocalFolder.h"
#include "DeviceFileSystem.h"
#include "CommandHandlerShared.h"
#include "DeviceLogging.h"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <sstream>
#include "CommandRegistry.h"

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
}

uintmax_t clear_directory(const fs::path& dir, std::error_code& ec) {
    uintmax_t removed = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        std::error_code entryEc;
        removed += fs::remove_all(entry.path(), entryEc);
        if (entryEc)
        {
            ec = entryEc;
        }
    }
    return removed;
}

DeleteLocalFolderResult ExecuteDeleteLocalFolder(
    std::string folderPath)
{
    DeleteLocalFolderResult result{};
    result.folderPath = std::move(folderPath);

    if (result.folderPath.empty())
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = "Parameter 'folderPath' cannot be empty";
        return result;
    }

    auto mountGuard = DeviceFileSystemMount(result.folderPath);

    std::string mountedPath = DeviceFileSystemGetMountedPath();

    fs::path target(mountedPath);

    std::error_code existsEc;
    const bool exists = fs::exists(target, existsEc);
    if (existsEc)
    {
        result.hr = ConvertFilesystemError(existsEc);
        std::ostringstream oss;
        oss << "DeleteLocalFolder: failed to query path existence for '" << mountedPath
            << "' (error: " << existsEc.message() << ")";
        result.errorMessage = oss.str();
        return result;
    }

    if (!exists)
    {
        LogToWindowFormat(
            "DeleteLocalFolder: folder does not exist ('%s')",
            mountedPath.c_str());
        result.hr = S_OK;
        return result;
    }

    result.folderExisted = true;

    std::error_code removeEc;
    const std::uintmax_t removed = clear_directory(target, removeEc);
    if (removeEc)
    {
        result.hr = ConvertFilesystemError(removeEc);
        std::ostringstream oss;
        oss << "DeleteLocalFolder: failed to remove '" << mountedPath
            << "' (error: " << removeEc.message() << ")";
        result.errorMessage = oss.str();
        return result;
    }

    result.entriesRemoved = removed;
    result.folderRemoved = true;
    result.hr = S_OK;

    LogToWindowFormat(
        "DeleteLocalFolder removed %llu entries from '%s'",
        static_cast<unsigned long long>(removed),
        mountedPath.c_str());

    return result;
}

CommandResultPayload HandleDeleteLocalFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(state);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string folderPath;
            std::string parseError;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "folderPath", folderPath, parseError))
            {
                return E_INVALIDARG;
            }

            DeleteLocalFolderResult result = ExecuteDeleteLocalFolder(std::move(folderPath));
            RETURN_IF_FAILED(result.hr);

            int64_t entriesRemoved = 0;
            if (result.entriesRemoved > static_cast<std::uintmax_t>(std::numeric_limits<int64_t>::max()))
            {
                entriesRemoved = std::numeric_limits<int64_t>::max();
            }
            else
            {
                entriesRemoved = static_cast<int64_t>(result.entriesRemoved);
            }

            payload.result["folderPath"] = result.folderPath;
            payload.result["folderExisted"] = result.folderExisted;
            payload.result["folderRemoved"] = result.folderRemoved;
            payload.result["entriesRemoved"] = entriesRemoved;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "DeleteLocalFolder", HandleDeleteLocalFolder }
});

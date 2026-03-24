#include "pch.h"

#include "DeleteLocalFolder.h"

#include "CommandHandlerShared.h"
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

    fs::path target(result.folderPath);

    std::error_code existsEc;
    const bool exists = fs::exists(target, existsEc);
    if (existsEc)
    {
        result.hr = ConvertFilesystemError(existsEc);
        std::ostringstream oss;
        oss << "DeleteLocalFolder: failed to query path existence for '" << result.folderPath
            << "' (error: " << existsEc.message() << ")";
        result.errorMessage = oss.str();
        return result;
    }

    if (!exists)
    {
        LogToWindowFormat(
            "DeleteLocalFolder: folder does not exist ('%s')",
            result.folderPath.c_str());
        result.hr = S_OK;
        return result;
    }

    result.folderExisted = true;

    std::error_code removeEc;
    const std::uintmax_t removed = fs::remove_all(target, removeEc);
    if (removeEc)
    {
        result.hr = ConvertFilesystemError(removeEc);
        std::ostringstream oss;
        oss << "DeleteLocalFolder: failed to remove '" << result.folderPath
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
        result.folderPath.c_str());

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

    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    std::string folderPath;
    std::string parseError;
    if (!CommandHandlerShared::TryGetStringParameter(parameters, "folderPath", folderPath, parseError))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, parseError);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        return payload;
    }

    const auto start = std::chrono::steady_clock::now();
    DeleteLocalFolderResult result = ExecuteDeleteLocalFolder(std::move(folderPath));
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

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

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

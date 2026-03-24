#include "pch.h"

#include "GatherLogs.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace
{
    namespace fs = std::filesystem;

    constexpr std::size_t kMaxLogBytes = 512 * 1024; // Limit payload size

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

    std::string ResolveDeviceName(DeviceGameSaveState* state)
    {
        if (state != nullptr && !state->inputDeviceId.empty())
        {
            return state->inputDeviceId;
        }

        return std::string("Device");
    }

    struct LogFileReadResult
    {
        HRESULT hr{ S_OK };
        std::string errorMessage;
        std::uint64_t fileSize{ 0 };
        std::uint64_t bytesReturned{ 0 };
        bool truncated{ false };
        std::string content;
    };

    LogFileReadResult ReadLogFile(const fs::path& logPath)
    {
        LogFileReadResult result{};

        std::error_code existsEc;
        const bool exists = fs::exists(logPath, existsEc);
        if (existsEc)
        {
            result.hr = ConvertFilesystemError(existsEc);
            std::ostringstream oss;
            oss << "Failed to query log file existence: " << existsEc.message();
            result.errorMessage = oss.str();
            return result;
        }

        if (!exists)
        {
            result.hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            std::ostringstream oss;
            oss << "Log file does not exist: " << GetStringFromU8String(logPath.u8string());
            result.errorMessage = oss.str();
            return result;
        }

        std::error_code sizeEc;
        const std::uintmax_t fileSize = fs::file_size(logPath, sizeEc);
        if (sizeEc)
        {
            result.hr = ConvertFilesystemError(sizeEc);
            std::ostringstream oss;
            oss << "Failed to query log file size: " << sizeEc.message();
            result.errorMessage = oss.str();
            return result;
        }

        result.fileSize = static_cast<std::uint64_t>(fileSize);

        const std::size_t bytesToRead = static_cast<std::size_t>(std::min<std::uintmax_t>(fileSize, kMaxLogBytes));
        if (fileSize > kMaxLogBytes)
        {
            result.truncated = true;
        }

        std::string content;
        content.reserve(bytesToRead);

        if (bytesToRead > 0)
        {
            std::ifstream file(logPath, std::ios::in | std::ios::binary);
            if (!file)
            {
                result.hr = E_FAIL;
                std::ostringstream oss;
                oss << "Failed to open log file: " << GetStringFromU8String(logPath.u8string());
                result.errorMessage = oss.str();
                return result;
            }

            if (fileSize > kMaxLogBytes)
            {
                const std::streamoff seekOffset = static_cast<std::streamoff>(fileSize - kMaxLogBytes);
                file.seekg(seekOffset, std::ios::beg);
            }

            content.resize(bytesToRead);
            file.read(content.data(), static_cast<std::streamsize>(bytesToRead));
            std::streamsize readCount = file.gcount();
            if (!file && !file.eof())
            {
                result.hr = E_FAIL;
                std::ostringstream oss;
                oss << "Failed to read log file: " << GetStringFromU8String(logPath.u8string());
                result.errorMessage = oss.str();
                return result;
            }
            if (readCount < 0)
            {
                readCount = 0;
            }

            content.resize(static_cast<std::size_t>(readCount));
            result.bytesReturned = static_cast<std::uint64_t>(readCount);
        }
        else
        {
            result.bytesReturned = 0;
            content.clear();
        }

        result.content = std::move(content);
        result.hr = S_OK;
        return result;
    }
}

GatherLogsResult ExecuteGatherLogs(DeviceGameSaveState* state)
{
    GatherLogsResult result{};
    result.deviceName = ResolveDeviceName(state);

    FlushDeviceLogFile();

    std::string activeLogPath;
    if (!TryGetCurrentLogFilePath(activeLogPath) || activeLogPath.empty())
    {
        activeLogPath = ResolveLogFilePathForDevice(result.deviceName);
    }

    fs::path logPath = fs::path(activeLogPath);
    result.logPath = GetStringFromU8String(logPath.u8string());
    result.logFileName = GetStringFromU8String(logPath.filename().u8string());

    if (logPath.empty())
    {
        result.hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        result.errorMessage = "Log file path was empty";
        return result;
    }

    // Read main log file
    LogFileReadResult mainLogResult = ReadLogFile(logPath);
    if (FAILED(mainLogResult.hr))
    {
        result.hr = mainLogResult.hr;
        result.errorMessage = mainLogResult.errorMessage;
        return result;
    }

    result.fileSize = mainLogResult.fileSize;
    result.bytesReturned = mainLogResult.bytesReturned;
    result.truncated = mainLogResult.truncated;
    result.logContent = std::move(mainLogResult.content);

    // Read summary log file (replace -log.txt with -summary.txt)
    std::string summaryLogPathStr = result.logPath;
    size_t logPos = summaryLogPathStr.rfind("-log.txt");
    if (logPos != std::string::npos)
    {
        summaryLogPathStr.replace(logPos, 8, "-summary.txt");
        fs::path summaryLogPath = fs::path(summaryLogPathStr);
        result.summaryLogPath = GetStringFromU8String(summaryLogPath.u8string());
        result.summaryLogFileName = GetStringFromU8String(summaryLogPath.filename().u8string());

        LogFileReadResult summaryLogResult = ReadLogFile(summaryLogPath);
        if (SUCCEEDED(summaryLogResult.hr))
        {
            result.summaryFileSize = summaryLogResult.fileSize;
            result.summaryBytesReturned = summaryLogResult.bytesReturned;
            result.summaryTruncated = summaryLogResult.truncated;
            result.summaryLogContent = std::move(summaryLogResult.content);
        }
        // If summary log doesn't exist, that's okay - just leave it empty
    }

    result.hr = S_OK;

    LogToWindowFormat(
        "GatherLogs returning main: %llu/%llu bytes from '%s'%s, summary: %llu/%llu bytes from '%s'%s",
        static_cast<unsigned long long>(result.bytesReturned),
        static_cast<unsigned long long>(result.fileSize),
        result.logPath.c_str(),
        result.truncated ? " (truncated)" : "",
        static_cast<unsigned long long>(result.summaryBytesReturned),
        static_cast<unsigned long long>(result.summaryFileSize),
        result.summaryLogPath.c_str(),
        result.summaryTruncated ? " (truncated)" : "");

    return result;
}

CommandResultPayload HandleGatherLogs(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    const GatherLogsResult result = ExecuteGatherLogs(state);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["deviceName"] = result.deviceName;
    payload.result["logPath"] = result.logPath;
    payload.result["logFileName"] = result.logFileName;
    payload.result["fileSize"] = result.fileSize;
    payload.result["bytesRead"] = result.bytesReturned;
    payload.result["truncated"] = result.truncated;
    payload.result["logContent"] = result.logContent;
    payload.result["summaryLogPath"] = result.summaryLogPath;
    payload.result["summaryLogFileName"] = result.summaryLogFileName;
    payload.result["summaryFileSize"] = result.summaryFileSize;
    payload.result["summaryBytesRead"] = result.summaryBytesReturned;
    payload.result["summaryTruncated"] = result.summaryTruncated;
    payload.result["summaryLogContent"] = result.summaryLogContent;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

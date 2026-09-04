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
#include "CommandRegistry.h"

namespace
{
    namespace fs = std::filesystem;

    constexpr std::size_t kMaxLogBytes = 10 * 1024 * 1024; // Limit payload size (not an issue now that we send the log in chunks)

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

    struct LogFileMetadata
    {
        HRESULT hr{ S_OK };
        std::string errorMessage;
        std::uint64_t fileSize{ 0 };
        std::uint64_t bytesToTransfer{ 0 };
        std::uint64_t fileStartOffset{ 0 };
        bool truncated{ false };
    };

    LogFileMetadata ComputeLogFileMetadata(const fs::path& logPath)
    {
        LogFileMetadata result{};

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

        if (fileSize > kMaxLogBytes)
        {
            result.truncated = true;
            result.bytesToTransfer = kMaxLogBytes;
            result.fileStartOffset = static_cast<std::uint64_t>(fileSize - kMaxLogBytes);
        }
        else
        {
            result.truncated = false;
            result.bytesToTransfer = result.fileSize;
            result.fileStartOffset = 0;
        }

        result.hr = S_OK;
        return result;
    }

    constexpr std::size_t kChunkSize = 64 * 1024; // 64 KB max chunk

    struct LogFileChunkResult
    {
        HRESULT hr{ S_OK };
        std::string errorMessage;
        std::string content;
        std::uint64_t bytesRead{ 0 };
    };

    LogFileChunkResult ReadLogFileChunk(const fs::path& logPath, std::uint64_t fileOffset, std::size_t requestedSize)
    {
        LogFileChunkResult result{};

        const std::size_t clampedSize = std::min(requestedSize, kChunkSize);

        std::ifstream file(logPath, std::ios::in | std::ios::binary);
        if (!file)
        {
            result.hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open log file: " << GetStringFromU8String(logPath.u8string());
            result.errorMessage = oss.str();
            return result;
        }

        file.seekg(static_cast<std::streamoff>(fileOffset), std::ios::beg);
        if (!file)
        {
            result.hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to seek in log file to offset " << fileOffset;
            result.errorMessage = oss.str();
            return result;
        }

        std::string content;
        content.resize(clampedSize);
        file.read(content.data(), static_cast<std::streamsize>(clampedSize));
        std::streamsize readCount = file.gcount();
        if (!file && !file.eof())
        {
            result.hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to read log file chunk at offset " << fileOffset;
            result.errorMessage = oss.str();
            return result;
        }
        if (readCount < 0)
        {
            readCount = 0;
        }

        content.resize(static_cast<std::size_t>(readCount));
        result.content = std::move(content);
        result.bytesRead = static_cast<std::uint64_t>(readCount);
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

    // Compute main log file metadata
    LogFileMetadata mainMeta = ComputeLogFileMetadata(logPath);
    if (FAILED(mainMeta.hr))
    {
        result.hr = mainMeta.hr;
        result.errorMessage = mainMeta.errorMessage;
        return result;
    }

    result.fileSize = mainMeta.fileSize;
    result.bytesToTransfer = mainMeta.bytesToTransfer;
    result.fileStartOffset = mainMeta.fileStartOffset;
    result.truncated = mainMeta.truncated;

    // Compute summary log file metadata (replace -log.txt with -summary.txt)
    std::string summaryLogPathStr = result.logPath;
    size_t logPos = summaryLogPathStr.rfind("-log.txt");
    if (logPos != std::string::npos)
    {
        summaryLogPathStr.replace(logPos, 8, "-summary.txt");
        fs::path summaryLogPath = fs::path(summaryLogPathStr);
        result.summaryLogPath = GetStringFromU8String(summaryLogPath.u8string());
        result.summaryLogFileName = GetStringFromU8String(summaryLogPath.filename().u8string());

        LogFileMetadata summaryMeta = ComputeLogFileMetadata(summaryLogPath);
        if (SUCCEEDED(summaryMeta.hr))
        {
            result.summaryFileSize = summaryMeta.fileSize;
            result.summaryBytesToTransfer = summaryMeta.bytesToTransfer;
            result.summaryFileStartOffset = summaryMeta.fileStartOffset;
            result.summaryTruncated = summaryMeta.truncated;
        }
        // If summary log doesn't exist, that's okay - just leave it empty
    }

    result.hr = S_OK;

    LogToWindowFormat(
        "GatherLogs metadata: main %llu bytes (transfer %llu from offset %llu)%s, summary %llu bytes (transfer %llu from offset %llu)%s",
        static_cast<unsigned long long>(result.fileSize),
        static_cast<unsigned long long>(result.bytesToTransfer),
        static_cast<unsigned long long>(result.fileStartOffset),
        result.truncated ? " (truncated)" : "",
        static_cast<unsigned long long>(result.summaryFileSize),
        static_cast<unsigned long long>(result.summaryBytesToTransfer),
        static_cast<unsigned long long>(result.summaryFileStartOffset),
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

    // Fallback log gathering is supported - if the WebSocket-based GatherLogs fails,
    // the controller will attempt to copy logs directly from the device.
    // PC: copies from Out\logs directory
    // Xbox: uses xbcp to copy from D:\ drive

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
    payload.result["bytesToTransfer"] = result.bytesToTransfer;
    payload.result["fileStartOffset"] = result.fileStartOffset;
    payload.result["truncated"] = result.truncated;
    payload.result["summaryLogPath"] = result.summaryLogPath;
    payload.result["summaryLogFileName"] = result.summaryLogFileName;
    payload.result["summaryFileSize"] = result.summaryFileSize;
    payload.result["summaryBytesToTransfer"] = result.summaryBytesToTransfer;
    payload.result["summaryFileStartOffset"] = result.summaryFileStartOffset;
    payload.result["summaryTruncated"] = result.summaryTruncated;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandleGatherLogsChunk(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();

    std::string logPath;
    std::string error;
    if (!CommandHandlerShared::TryGetStringParameter(parameters, "logPath", logPath, error))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, error);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
        return payload;
    }

    int64_t fileOffset = 0;
    if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "fileOffset", fileOffset, error))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, error);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
        return payload;
    }

    int64_t size = 0;
    if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "size", size, error))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, error);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
        return payload;
    }

    namespace fs = std::filesystem;
    LogFileChunkResult chunkResult = ReadLogFileChunk(
        fs::path(logPath),
        static_cast<std::uint64_t>(fileOffset),
        static_cast<std::size_t>(size));

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(chunkResult.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, chunkResult.hr, chunkResult.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, chunkResult.hr);
        return payload;
    }

    payload.result["content"] = std::move(chunkResult.content);
    payload.result["offset"] = static_cast<std::uint64_t>(fileOffset);
    payload.result["bytesRead"] = chunkResult.bytesRead;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "GatherLogs", HandleGatherLogs },
    { "GatherLogsChunk", HandleGatherLogsChunk }
});

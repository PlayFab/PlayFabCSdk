#include "pch.h"

#include "CaptureSaveContainerSnapshot.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "HarnessHash.h"
#include "CommandHandlerShared.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <vector>

#include <playfab/gamesave/PFGameSaveFiles.h>

namespace
{
    namespace fs = std::filesystem;

    constexpr uint64_t WindowsTicksPerMillisecond = 10000ULL;
    constexpr int64_t WindowsEpochToUnixEpochMilliseconds = 11644473600000LL;

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
            const std::string lower = ToLowerCopy(part.generic_string());
            if (lower == "cloudsync")
            {
                return true;
            }
        }

        return false;
    }

    int64_t FileTimeToUnixEpochMilliseconds(const FILETIME& ft)
    {
        ULARGE_INTEGER value{};
        value.LowPart = ft.dwLowDateTime;
        value.HighPart = ft.dwHighDateTime;

        const uint64_t ticks = value.QuadPart;
        const uint64_t milliseconds = ticks / WindowsTicksPerMillisecond;
        const int64_t epochMilliseconds = static_cast<int64_t>(milliseconds) - WindowsEpochToUnixEpochMilliseconds;
        return epochMilliseconds;
    }

    std::chrono::system_clock::time_point EpochMillisecondsToTimePoint(int64_t epochMs)
    {
        using namespace std::chrono;
        return system_clock::time_point(milliseconds(epochMs));
    }

    std::string FormatIsoTimestampUtc(const std::chrono::system_clock::time_point& tp)
    {
        using namespace std::chrono;

        auto duration = tp.time_since_epoch();
        auto secondsPart = floor<seconds>(duration);
        auto fractional = duration_cast<milliseconds>(duration - secondsPart);
        if (fractional.count() < 0)
        {
            fractional += milliseconds(1000);
            secondsPart -= seconds(1);
        }

        auto secondsTimePoint = system_clock::time_point(secondsPart);
        auto timeT = system_clock::to_time_t(secondsTimePoint);
        std::tm tm{};
        gmtime_s(&tm, &timeT);

        char timeBuffer[32] = {};
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%dT%H:%M:%S", &tm);

        char result[48] = {};
        std::snprintf(result, sizeof(result), "%s.%03lldZ", timeBuffer, static_cast<long long>(fractional.count()));
        return std::string(result);
    }

    std::string FormatIsoTimestampUtcFromMilliseconds(int64_t epochMs)
    {
        return FormatIsoTimestampUtc(EpochMillisecondsToTimePoint(epochMs));
    }

    struct SnapshotEntry
    {
        std::string path;
        std::string type;
        uint64_t size{ 0 };
        int64_t lastWriteTimeMs{ 0 };
        std::string lastWriteTimeIso;
        uint32_t attributes{ 0 };
        bool hasAttributes{ false };
        std::string sha256;
    };

    std::string PathToGenericString(const fs::path& path)
    {
        return path.generic_string();
    }
}

CaptureSaveContainerSnapshotResult ExecuteCaptureSaveContainerSnapshot(
    DeviceGameSaveState* state,
    bool includeHashes)
{
    CaptureSaveContainerSnapshotResult result{};
    result.includeHashes = includeHashes;

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

    result.saveFolder = PathToGenericString(rootPath);

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

    std::vector<SnapshotEntry> entries;

    if (rootExists)
    {
        std::error_code iterEc;
        fs::recursive_directory_iterator it(rootPath, fs::directory_options::skip_permission_denied, iterEc);
        if (iterEc)
        {
            result.hr = ConvertFilesystemError(iterEc);
            std::ostringstream oss;
            oss << "Failed to enumerate save folder: " << iterEc.message();
            result.errorMessage = oss.str();
            return result;
        }

        fs::recursive_directory_iterator end;
        for (; it != end; it.increment(iterEc))
        {
            if (iterEc)
            {
                result.hr = ConvertFilesystemError(iterEc);
                std::ostringstream oss;
                oss << "Failed to iterate save folder: " << iterEc.message();
                result.errorMessage = oss.str();
                return result;
            }

            const fs::path entryPath = it->path();
            std::error_code relativeEc;
            fs::path relative = fs::relative(entryPath, rootPath, relativeEc);
            if (relativeEc)
            {
                relativeEc.clear();
                relative = entryPath.filename();
            }

            if (HasReservedCloudsyncComponent(relative))
            {
                if (it->is_directory(relativeEc) && !relativeEc)
                {
                    it.disable_recursion_pending();
                }

                result.cloudsyncEntriesSkipped++;
                result.skippedEntries.push_back(PathToGenericString(relative));
                continue;
            }

            std::error_code statusEc;
            const fs::file_status status = it->symlink_status(statusEc);
            if (statusEc)
            {
                result.hr = ConvertFilesystemError(statusEc);
                std::ostringstream oss;
                oss << "Failed to query entry status for '" << entryPath.string() << "': " << statusEc.message();
                result.errorMessage = oss.str();
                return result;
            }

            SnapshotEntry entry;
            entry.path = PathToGenericString(relative);

            switch (status.type())
            {
                case fs::file_type::directory:
                    entry.type = "directory";
                    break;
                case fs::file_type::regular:
                    entry.type = "file";
                    break;
                case fs::file_type::symlink:
                    entry.type = "symlink";
                    break;
                case fs::file_type::block:
                    entry.type = "block";
                    break;
                case fs::file_type::character:
                    entry.type = "character";
                    break;
                case fs::file_type::fifo:
                    entry.type = "fifo";
                    break;
                case fs::file_type::socket:
                    entry.type = "socket";
                    break;
                case fs::file_type::unknown:
                    entry.type = "unknown";
                    break;
                default:
                    entry.type = "none";
                    break;
            }

            WIN32_FILE_ATTRIBUTE_DATA attributes{};
            if (!GetFileAttributesExW(entryPath.c_str(), GetFileExInfoStandard, &attributes))
            {
                const DWORD attributeError = GetLastError();
                result.hr = HRESULT_FROM_WIN32(attributeError);
                std::ostringstream oss;
                oss << "GetFileAttributesExW failed for '" << entryPath.string() << "' (error=" << attributeError << ")";
                result.errorMessage = oss.str();
                return result;
            }

            entry.hasAttributes = true;
            entry.attributes = attributes.dwFileAttributes;
            entry.lastWriteTimeMs = FileTimeToUnixEpochMilliseconds(attributes.ftLastWriteTime);
            entry.lastWriteTimeIso = FormatIsoTimestampUtcFromMilliseconds(entry.lastWriteTimeMs);

            ULARGE_INTEGER sizeValue{};
            sizeValue.HighPart = attributes.nFileSizeHigh;
            sizeValue.LowPart = attributes.nFileSizeLow;
            entry.size = sizeValue.QuadPart;

            if (entry.type == "file")
            {
                result.filesCaptured++;
                result.totalBytes += entry.size;

                if (includeHashes)
                {
                    std::string hash;
                    std::string hashError;
                    hr = HarnessHash::ComputeSha256(entryPath, hash, hashError);
                    if (FAILED(hr))
                    {
                        result.hr = hr;
                        result.errorMessage = hashError;
                        return result;
                    }

                    entry.sha256 = std::move(hash);
                }
            }
            else if (entry.type == "directory")
            {
                result.directoriesCaptured++;
            }

            entries.push_back(std::move(entry));
        }
    }

    std::sort(entries.begin(), entries.end(), [](const SnapshotEntry& a, const SnapshotEntry& b)
    {
        return a.path < b.path;
    });

    nlohmann::json entriesJson = nlohmann::json::array();
    for (const auto& entry : entries)
    {
        nlohmann::json entryJson = nlohmann::json::object();
        entryJson["path"] = entry.path;
        entryJson["type"] = entry.type;
        entryJson["lastWriteTimeEpochMs"] = entry.lastWriteTimeMs;
        entryJson["lastWriteTime"] = entry.lastWriteTimeIso;
        if (entry.hasAttributes)
        {
            entryJson["attributes"] = entry.attributes;
        }

        if (entry.type == "file" || entry.type == "symlink")
        {
            entryJson["size"] = entry.size;
        }

        if (!entry.sha256.empty())
        {
            entryJson["sha256"] = entry.sha256;
        }

        entriesJson.push_back(std::move(entryJson));
    }

    nlohmann::json totals = nlohmann::json::object();
    totals["files"] = result.filesCaptured;
    totals["directories"] = result.directoriesCaptured;
    totals["bytes"] = result.totalBytes;

    const std::string deviceId = (state && !state->inputDeviceId.empty()) ? state->inputDeviceId : "TestDevice";
    const auto capturedAt = std::chrono::system_clock::now();

    result.snapshot = nlohmann::json::object();
    result.snapshot["schemaVersion"] = 1;
    result.snapshot["deviceId"] = deviceId;
    result.snapshot["scenarioId"] = state ? state->currentScenarioId : "";
    result.snapshot["scenarioName"] = state ? state->currentScenarioName : "";
    result.snapshot["saveFolder"] = result.saveFolder;
    result.snapshot["hashesIncluded"] = includeHashes;
    result.snapshot["capturedAtUtc"] = FormatIsoTimestampUtc(capturedAt);
    result.snapshot["entries"] = std::move(entriesJson);
    result.snapshot["totals"] = std::move(totals);
    result.snapshot["cloudsyncEntriesSkipped"] = result.cloudsyncEntriesSkipped;

    if (!result.skippedEntries.empty())
    {
        std::sort(result.skippedEntries.begin(), result.skippedEntries.end());

        nlohmann::json skipped = nlohmann::json::array();
        for (const auto& item : result.skippedEntries)
        {
            skipped.push_back(item);
        }
        result.snapshot["skippedEntries"] = std::move(skipped);
    }

    LogToWindowFormat(
        "CaptureSaveContainerSnapshot captured %d files, %d directories, %llu bytes",
        result.filesCaptured,
        result.directoriesCaptured,
        static_cast<unsigned long long>(result.totalBytes));

    const std::string snapshotJson = result.snapshot.dump(2);
    LogToWindowFormatVerbose(true, "CaptureSaveContainerSnapshot manifest:%s", snapshotJson.c_str());

    result.hr = S_OK;
    return result;
}

CommandResultPayload HandleCaptureSaveContainerSnapshot(
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

    std::string parseError;
    bool includeHashes = true;
    if (!CommandHandlerShared::TryParseBoolParameter(parameters, "includeHashes", includeHashes, parseError))
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, parseError);
        CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
        return payload;
    }

    auto start = std::chrono::steady_clock::now();
    CaptureSaveContainerSnapshotResult result = ExecuteCaptureSaveContainerSnapshot(state, includeHashes);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["includeHashes"] = result.includeHashes;
    payload.result["saveFolder"] = result.saveFolder;
    payload.result["filesCaptured"] = result.filesCaptured;
    payload.result["directoriesCaptured"] = result.directoriesCaptured;
    payload.result["cloudsyncEntriesSkipped"] = result.cloudsyncEntriesSkipped;
    payload.result["totalBytes"] = result.totalBytes;
    payload.result["entriesCaptured"] = static_cast<int>(result.snapshot["entries"].size());

    if (!result.skippedEntries.empty())
    {
        nlohmann::json skipped = nlohmann::json::array();
        for (const auto& entry : result.skippedEntries)
        {
            skipped.push_back(entry);
        }

        payload.result["skippedEntries"] = std::move(skipped);
    }

    payload.result["snapshot"] = result.snapshot;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

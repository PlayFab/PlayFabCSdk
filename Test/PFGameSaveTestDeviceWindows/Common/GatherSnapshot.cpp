#include "pch.h"

#include "GatherSnapshot.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <archive.h>
#include <archive_entry.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include <playfab/gamesave/PFGameSaveFiles.h>

namespace
{
    namespace fs = std::filesystem;

    constexpr std::size_t kMaxArchiveBytes = 16 * 1024 * 1024; // 16 MiB cap for transfer

    // Archive write context for file I/O callbacks
    struct ArchiveWriteContext
    {
        std::ofstream file;
        std::string error;
    };

    int ArchiveOpenCallback(archive*, void* clientData)
    {
        auto* ctx = static_cast<ArchiveWriteContext*>(clientData);
        if (!ctx || !ctx->file.is_open())
        {
            return ARCHIVE_FATAL;
        }
        return ARCHIVE_OK;
    }

    la_ssize_t ArchiveWriteCallback(archive*, void* clientData, const void* buffer, size_t length)
    {
        auto* ctx = static_cast<ArchiveWriteContext*>(clientData);
        if (!ctx || !ctx->file.is_open())
        {
            return -1;
        }

        ctx->file.write(static_cast<const char*>(buffer), static_cast<std::streamsize>(length));
        if (!ctx->file)
        {
            ctx->error = "Failed to write to archive file";
            return -1;
        }

        return static_cast<la_ssize_t>(length);
    }

    int ArchiveCloseCallback(archive*, void* clientData)
    {
        auto* ctx = static_cast<ArchiveWriteContext*>(clientData);
        if (ctx && ctx->file.is_open())
        {
            ctx->file.close();
        }
        return ARCHIVE_OK;
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

    std::string ResolveDeviceName(DeviceGameSaveState* state)
    {
        if (state != nullptr && !state->inputDeviceId.empty())
        {
            return state->inputDeviceId;
        }

        return std::string("Device");
    }

    HRESULT FetchSaveFolder(DeviceGameSaveState* state, std::string& folder)
    {
        if (!state)
        {
            return E_POINTER;
        }

        if (!state->saveFolder.empty())
        {
            folder = state->saveFolder;
            return S_OK;
        }

        if (!state->inputSaveFolder.empty())
        {
            folder = state->inputSaveFolder;
            state->saveFolder = folder;
            return S_OK;
        }

        if (!state->localUserHandle)
        {
            return HRESULT_FROM_WIN32(ERROR_INVALID_STATE);
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
            if (hr == HRESULT_FROM_WIN32(ERROR_INVALID_STATE))
            {
                error = "Save folder is unavailable; no cached path and PFGameSave is not initialized.";
            }
            else
            {
                error = "Failed to resolve save root via PFGameSaveFilesGetFolder";
            }
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

    std::string NormalizeEntryPath(const std::string& path)
    {
        std::string normalized = path;
        std::replace(normalized.begin(), normalized.end(), '\\', '/');
        while (!normalized.empty() && normalized.front() == '/')
        {
            normalized.erase(normalized.begin());
        }
        return normalized;
    }

    time_t ToTimeT(const fs::file_time_type& fileTime)
    {
        using FileClock = fs::file_time_type::clock;
        auto systemTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - FileClock::now() + std::chrono::system_clock::now());
        return std::chrono::system_clock::to_time_t(systemTime);
    }

    HRESULT ConvertArchiveResult(int result, archive* ar, std::string& error, const char* message)
    {
        if (result >= ARCHIVE_OK)
        {
            return S_OK;
        }

        const char* archiveError = ar != nullptr ? archive_error_string(ar) : "unknown";
        std::ostringstream oss;
        oss << message << ": " << archiveError;
        error = oss.str();
        return E_FAIL;
    }

    HRESULT WriteDirectoryEntry(archive* ar, const std::string& entryPath, const fs::file_time_type& mtime, std::string& error)
    {
        archive_entry* entry = archive_entry_new();
        if (!entry)
        {
            error = "Failed to allocate archive entry";
            return E_OUTOFMEMORY;
        }

        std::string normalized = NormalizeEntryPath(entryPath);
        if (!normalized.empty() && normalized.back() != '/')
        {
            normalized.push_back('/');
        }

        archive_entry_set_pathname(entry, normalized.c_str());
        archive_entry_set_filetype(entry, AE_IFDIR);
        archive_entry_set_perm(entry, 0755);
        archive_entry_set_size(entry, 0);
        archive_entry_set_mtime(entry, ToTimeT(mtime), 0);

        int arResult = archive_write_header(ar, entry);
        archive_entry_free(entry);
        return ConvertArchiveResult(arResult, ar, error, "Failed to write directory entry header");
    }

    HRESULT WriteFileEntry(archive* ar, const fs::path& fullPath, const std::string& entryPath, std::string& error)
    {
        std::error_code ec;
        const auto fileSize = fs::file_size(fullPath, ec);
        if (ec)
        {
            error = std::string("Failed to query file size for '") + GetStringFromU8String(fullPath.u8string()) + "': " + ec.message();
            return ConvertFilesystemError(ec);
        }

        archive_entry* entry = archive_entry_new();
        if (!entry)
        {
            error = "Failed to allocate archive entry";
            return E_OUTOFMEMORY;
        }

        std::string normalized = NormalizeEntryPath(entryPath);
        archive_entry_set_pathname(entry, normalized.c_str());
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);
        archive_entry_set_size(entry, static_cast<la_int64_t>(fileSize));

        std::error_code timeEc;
        const auto lastWrite = fs::last_write_time(fullPath, timeEc);
        if (!timeEc)
        {
            archive_entry_set_mtime(entry, ToTimeT(lastWrite), 0);
        }

        int arResult = archive_write_header(ar, entry);
        if (arResult == ARCHIVE_OK && fileSize > 0)
        {
            std::ifstream file(fullPath, std::ios::binary);
            if (!file)
            {
                archive_entry_free(entry);
                error = std::string("Failed to open file '") + GetStringFromU8String(fullPath.u8string()) + "' for reading";
                return E_FAIL;
            }

            std::vector<char> buffer(128 * 1024);
            while (file)
            {
                file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
                std::streamsize readCount = file.gcount();
                if (readCount <= 0)
                {
                    break;
                }

                la_ssize_t writeResult = archive_write_data(ar, buffer.data(), static_cast<size_t>(readCount));
                if (writeResult < 0)
                {
                    archive_entry_free(entry);
                    error = std::string("Failed to write data for '") + GetStringFromU8String(fullPath.u8string()) + "': " + archive_error_string(ar);
                    return E_FAIL;
                }
            }

            if (!file.eof())
            {
                archive_entry_free(entry);
                error = std::string("Failed to read file '") + GetStringFromU8String(fullPath.u8string()) + "' fully";
                return E_FAIL;
            }
        }
        else if (arResult < ARCHIVE_OK)
        {
            archive_entry_free(entry);
            return ConvertArchiveResult(arResult, ar, error, "Failed to write file entry header");
        }

        archive_entry_free(entry);
        return S_OK;
    }

    HRESULT CreateArchive(const fs::path& rootPath, const fs::path& archivePath, std::string& error)
    {
        archive* ar = archive_write_new();
        if (!ar)
        {
            error = "Failed to create archive context";
            return E_OUTOFMEMORY;
        }

        HRESULT hr = S_OK;
        int arResult = archive_write_set_format_zip(ar);
        if (SUCCEEDED(hr))
        {
            hr = ConvertArchiveResult(arResult, ar, error, "Failed to set archive format");
        }

        if (SUCCEEDED(hr))
        {
            arResult = archive_write_set_options(ar, "compression=deflate,!zip64");
            hr = ConvertArchiveResult(arResult, ar, error, "Failed to configure archive compression");
        }

        // Set up file I/O context and open using callbacks
        ArchiveWriteContext writeContext;
        if (SUCCEEDED(hr))
        {
            writeContext.file.open(archivePath, std::ios::binary | std::ios::trunc);
            if (!writeContext.file.is_open())
            {
                error = std::string("Failed to open archive file for writing: ") + GetStringFromU8String(archivePath.u8string());
                hr = E_FAIL;
            }
        }

        if (SUCCEEDED(hr))
        {
            arResult = archive_write_open2(ar, &writeContext, ArchiveOpenCallback, ArchiveWriteCallback, ArchiveCloseCallback, nullptr);
            hr = ConvertArchiveResult(arResult, ar, error, "Failed to open archive for writing");
        }

        if (FAILED(hr))
        {
            if (writeContext.file.is_open())
            {
                writeContext.file.close();
            }
            archive_write_close(ar);
            archive_write_free(ar);
            return hr;
        }

        std::string rootName = GetStringFromU8String(rootPath.filename().u8string());
        if (rootName.empty())
        {
            rootName = "Snapshot";
        }

        fs::file_time_type rootTime{};
        std::error_code rootTimeEc;
        rootTime = fs::last_write_time(rootPath, rootTimeEc);
        if (rootTimeEc)
        {
            rootTime = fs::file_time_type::clock::now();
        }

        hr = WriteDirectoryEntry(ar, rootName, rootTime, error);
        if (FAILED(hr))
        {
            archive_write_close(ar);
            archive_write_free(ar);
            return hr;
        }

        std::error_code iterEc;
        fs::recursive_directory_iterator it(rootPath, fs::directory_options::skip_permission_denied, iterEc);
        if (iterEc)
        {
            archive_write_close(ar);
            archive_write_free(ar);
            error = std::string("Failed to enumerate save folder: ") + iterEc.message();
            return ConvertFilesystemError(iterEc);
        }

        fs::recursive_directory_iterator end;
        for (; it != end; it.increment(iterEc))
        {
            if (iterEc)
            {
                archive_write_close(ar);
                archive_write_free(ar);
                error = std::string("Failed to iterate directory: ") + iterEc.message();
                return ConvertFilesystemError(iterEc);
            }

            const fs::path fullPath = it->path();
            std::error_code relativeEc;
            fs::path relative = fs::relative(fullPath, rootPath, relativeEc);
            if (relativeEc)
            {
                relative = fullPath.filename();
            }

            std::string entryPath = rootName;
            std::string relativeStr = GetStringFromU8String(relative.generic_u8string());
            if (!relativeStr.empty())
            {
                if (!entryPath.empty())
                {
                    entryPath.append("/");
                }
                entryPath.append(relativeStr);
            }

            std::error_code statusEc;
            const bool isDirectory = it->is_directory(statusEc);
            if (statusEc)
            {
                archive_write_close(ar);
                archive_write_free(ar);
                error = std::string("Failed to query entry type for '") + GetStringFromU8String(fullPath.u8string()) + "': " + statusEc.message();
                return ConvertFilesystemError(statusEc);
            }

            if (isDirectory)
            {
                fs::file_time_type dirTime{};
                std::error_code timeEc;
                dirTime = fs::last_write_time(fullPath, timeEc);
                if (timeEc)
                {
                    dirTime = fs::file_time_type::clock::now();
                }

                hr = WriteDirectoryEntry(ar, entryPath, dirTime, error);
                if (FAILED(hr))
                {
                    archive_write_close(ar);
                    archive_write_free(ar);
                    return hr;
                }

                continue;
            }

            if (it->is_regular_file(statusEc) && !statusEc)
            {
                hr = WriteFileEntry(ar, fullPath, entryPath, error);
                if (FAILED(hr))
                {
                    archive_write_close(ar);
                    archive_write_free(ar);
                    return hr;
                }
            }
        }

        archive_write_close(ar);
        archive_write_free(ar);

        return S_OK;
    }

    std::string Base64Encode(const uint8_t* data, size_t length)
    {
        static constexpr char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string output;
        output.reserve(((length + 2) / 3) * 4);

        size_t i = 0;
        while (i + 2 < length)
        {
            uint32_t value = (static_cast<uint32_t>(data[i]) << 16)
                | (static_cast<uint32_t>(data[i + 1]) << 8)
                | static_cast<uint32_t>(data[i + 2]);
            output.push_back(kAlphabet[(value >> 18) & 0x3F]);
            output.push_back(kAlphabet[(value >> 12) & 0x3F]);
            output.push_back(kAlphabet[(value >> 6) & 0x3F]);
            output.push_back(kAlphabet[value & 0x3F]);
            i += 3;
        }

        if (i < length)
        {
            uint32_t value = static_cast<uint32_t>(data[i]) << 16;
            output.push_back(kAlphabet[(value >> 18) & 0x3F]);
            if (i + 1 < length)
            {
                value |= static_cast<uint32_t>(data[i + 1]) << 8;
                output.push_back(kAlphabet[(value >> 12) & 0x3F]);
                output.push_back(kAlphabet[(value >> 6) & 0x3F]);
                output.push_back('=');
            }
            else
            {
                output.push_back(kAlphabet[(value >> 12) & 0x3F]);
                output.push_back('=');
                output.push_back('=');
            }
        }

        return output;
    }
}

GatherSnapshotResult ExecuteGatherSnapshot(DeviceGameSaveState* state)
{
    GatherSnapshotResult result{};
    result.deviceName = ResolveDeviceName(state);

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
        result.hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        result.errorMessage = "Save folder does not exist";
        return result;
    }

    fs::path tempArchive = fs::temp_directory_path() / fs::path("pf_snapshot_" + std::to_string(GetTickCount64()) + ".zip");

    std::string archiveError;
    hr = CreateArchive(rootPath, tempArchive, archiveError);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = archiveError;
        std::error_code cleanupEc;
        fs::remove(tempArchive, cleanupEc);
        return result;
    }

    std::error_code sizeEc;
    const std::uintmax_t archiveSize = fs::file_size(tempArchive, sizeEc);
    if (sizeEc)
    {
        result.hr = ConvertFilesystemError(sizeEc);
        result.errorMessage = std::string("Failed to query archive size: ") + sizeEc.message();
        std::error_code cleanupEc;
        fs::remove(tempArchive, cleanupEc);
        return result;
    }

    result.archivePath = GetStringFromU8String(tempArchive.u8string());
    result.archiveFileName = GetStringFromU8String(tempArchive.filename().u8string());
    result.archiveSize = static_cast<std::uint64_t>(archiveSize);

    const std::size_t bytesToRead = static_cast<std::size_t>(std::min<std::uintmax_t>(archiveSize, kMaxArchiveBytes));
    if (archiveSize > kMaxArchiveBytes)
    {
        result.truncated = true;
    }

    std::vector<uint8_t> buffer(bytesToRead);
    std::size_t bytesRead = 0;

    if (bytesToRead > 0)
    {
        std::ifstream file(tempArchive, std::ios::binary);
        if (!file)
        {
            result.hr = E_FAIL;
            result.errorMessage = std::string("Failed to open archive '") + GetStringFromU8String(tempArchive.u8string()) + "' for reading";
            std::error_code cleanupEc;
            fs::remove(tempArchive, cleanupEc);
            return result;
        }

        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(bytesToRead));
        std::streamsize readCount = file.gcount();
        if (!file && !file.eof())
        {
            result.hr = E_FAIL;
            result.errorMessage = std::string("Failed to read archive '") + GetStringFromU8String(tempArchive.u8string()) + "'";
            std::error_code cleanupEc;
            fs::remove(tempArchive, cleanupEc);
            return result;
        }

        if (readCount < 0)
        {
            readCount = 0;
        }

        bytesRead = static_cast<std::size_t>(readCount);
        buffer.resize(bytesRead);
    }

    result.bytesReturned = static_cast<std::uint64_t>(bytesRead);
    result.base64Content = bytesRead > 0 ? Base64Encode(buffer.data(), bytesRead) : std::string();
    result.hr = S_OK;

    std::error_code removeEc;
    fs::remove(tempArchive, removeEc);
    if (removeEc)
    {
        LogToWindowFormat("GatherSnapshot: failed to delete temp archive '%s' (error: %s)", result.archivePath.c_str(), removeEc.message().c_str());
    }

    LogToWindowFormat(
        "GatherSnapshot returning %llu/%llu bytes from '%s'%s",
        static_cast<unsigned long long>(result.bytesReturned),
        static_cast<unsigned long long>(result.archiveSize),
        result.saveFolder.c_str(),
        result.truncated ? " (truncated)" : "");

    return result;
}

CommandResultPayload HandleGatherSnapshot(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(parameters);

    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    const GatherSnapshotResult result = ExecuteGatherSnapshot(state);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["deviceName"] = result.deviceName;
    payload.result["saveFolder"] = result.saveFolder;
    payload.result["archiveFileName"] = result.archiveFileName;
    payload.result["archiveSize"] = result.archiveSize;
    payload.result["bytesRead"] = result.bytesReturned;
    payload.result["truncated"] = result.truncated;
    payload.result["base64Content"] = result.base64Content;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

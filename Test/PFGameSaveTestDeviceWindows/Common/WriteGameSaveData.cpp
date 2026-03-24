#include "pch.h"

#include "WriteGameSaveData.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandHandlerShared.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <optional>
#include <random>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <playfab/gamesave/PFGameSaveFiles.h>

namespace
{
    using json = nlohmann::json;
    namespace fs = std::filesystem;

    constexpr const char* kReservedMetadataFolder = "cloudsync";

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

    bool ContainsReservedComponent(const fs::path& rel)
    {
        for (const auto& part : rel)
        {
            const std::string component = ToLowerCopy(GetStringFromU8String(part.generic_u8string()));
            if (component.empty() || component == "." || component == "..")
            {
                continue;
            }

            if (component == kReservedMetadataFolder)
            {
                return true;
            }
        }

        return false;
    }

    bool TryNormalizeRelativePath(const std::string& raw, fs::path& normalized, std::string& error)
    {
        fs::path rel(raw);
        if (rel.empty())
        {
            error = "relativePath must not be empty";
            return false;
        }

        if (rel.is_absolute())
        {
            error = "Path must be relative to the save root";
            return false;
        }

        for (const auto& part : rel)
        {
            const std::string component = GetStringFromU8String(part.generic_u8string());
            if (component == "..")
            {
                error = "Path may not navigate above the save root";
                return false;
            }
        }

        if (ContainsReservedComponent(rel))
        {
            error = "Path targets reserved cloudsync/ metadata";
            return false;
        }

        normalized = rel.lexically_normal();
        return true;
    }

    fs::path MakeAbsolutePath(const fs::path& root, const fs::path& relative)
    {
        return (root / relative).lexically_normal();
    }

    bool EnsureParentDirectories(const fs::path& target, std::string& error, HRESULT& hr)
    {
        const fs::path parent = target.parent_path();
        if (parent.empty())
        {
            return true;
        }

        std::error_code ec;
        fs::create_directories(parent, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to create parent directories for " << target << ": " << ec.message();
            error = oss.str();
            return false;
        }

        return true;
    }

    bool EnsureDirectoryExists(const fs::path& directory, std::string& error, HRESULT& hr)
    {
        std::error_code ec;
        fs::create_directories(directory, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to create directory " << directory << ": " << ec.message();
            error = oss.str();
            return false;
        }

        return true;
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

    std::string PathToString(const fs::path& path)
    {
        return GetStringFromU8String(path.generic_u8string());
    }

    std::vector<uint8_t> ParsePatternBytes(const json& patternNode, std::string& error)
    {
        std::vector<uint8_t> pattern;
        if (!patternNode.is_array() || patternNode.empty())
        {
            error = "pattern must be a non-empty array";
            return pattern;
        }

        for (const auto& entry : patternNode)
        {
            int64_t value = 0;
            if (entry.is_number_integer())
            {
                value = entry.get<int64_t>();
            }
            else if (entry.is_string())
            {
                const std::string text = entry.get<std::string>();
                try
                {
                    size_t consumed = 0;
                    value = std::stoll(text, &consumed, 0);
                    if (consumed != text.size())
                    {
                        error = "pattern entries must parse as bytes";
                        pattern.clear();
                        return pattern;
                    }
                }
                catch (...)
                {
                    error = "pattern entries must parse as bytes";
                    pattern.clear();
                    return pattern;
                }
            }
            else
            {
                error = "pattern entries must be numbers or numeric strings";
                pattern.clear();
                return pattern;
            }

            if (value < 0 || value > 255)
            {
                error = "pattern bytes must be in range 0-255";
                pattern.clear();
                return pattern;
            }

            pattern.push_back(static_cast<uint8_t>(value));
            if (pattern.size() > 16)
            {
                error = "pattern may not contain more than 16 entries";
                pattern.clear();
                return pattern;
            }
        }

        return pattern;
    }

    bool WriteBinaryFileWithPattern(const fs::path& absolutePath, size_t bytes, const std::vector<uint8_t>& pattern, std::string& error, HRESULT& hr)
    {
        if (pattern.empty())
        {
            hr = E_INVALIDARG;
            error = "pattern must include at least one byte";
            return false;
        }

        if (!EnsureParentDirectories(absolutePath, error, hr))
        {
            return false;
        }

        std::ofstream stream(absolutePath, std::ios::binary | std::ios::trunc);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for writing: " << absolutePath;
            error = oss.str();
            return false;
        }

        if (bytes > 0)
        {
            const size_t chunkSize = 4096;
            std::vector<uint8_t> buffer;
            buffer.reserve(chunkSize);

            size_t remaining = bytes;
            while (remaining > 0)
            {
                buffer.clear();
                size_t toWrite = std::min(chunkSize, remaining);
                while (buffer.size() < toWrite)
                {
                    const size_t copyCount = std::min(pattern.size(), toWrite - buffer.size());
                    buffer.insert(buffer.end(), pattern.begin(), pattern.begin() + copyCount);
                }

                stream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(toWrite));
                if (!stream)
                {
                    hr = E_FAIL;
                    error = "Failed while writing binary pattern";
                    return false;
                }

                remaining -= toWrite;
            }
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush binary file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool WriteRandomBinaryFile(const fs::path& absolutePath, size_t bytes, std::mt19937& rng, std::string& error, HRESULT& hr)
    {
        if (!EnsureParentDirectories(absolutePath, error, hr))
        {
            return false;
        }

        std::ofstream stream(absolutePath, std::ios::binary | std::ios::trunc);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for writing: " << absolutePath;
            error = oss.str();
            return false;
        }

        std::uniform_int_distribution<int> byteDist(0, 255);
        const size_t chunkSize = 4096;
        std::vector<uint8_t> buffer(chunkSize);

        size_t remaining = bytes;
        while (remaining > 0)
        {
            size_t toWrite = std::min(chunkSize, remaining);
            for (size_t i = 0; i < toWrite; ++i)
            {
                buffer[i] = static_cast<uint8_t>(byteDist(rng));
            }

            stream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(toWrite));
            if (!stream)
            {
                hr = E_FAIL;
                error = "Failed while writing random binary data";
                return false;
            }

            remaining -= toWrite;
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush random binary file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool WriteRandomTextFile(const fs::path& absolutePath, size_t bytes, std::mt19937& rng, std::string& error, HRESULT& hr)
    {
        if (!EnsureParentDirectories(absolutePath, error, hr))
        {
            return false;
        }

        std::ofstream stream(absolutePath, std::ios::binary | std::ios::trunc);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for writing: " << absolutePath;
            error = oss.str();
            return false;
        }

        static constexpr char kAlphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ._-";
        constexpr size_t alphabetLength = sizeof(kAlphabet) - 1;
        std::uniform_int_distribution<size_t> charDist(0, alphabetLength - 1);

        const size_t chunkSize = 4096;
        std::vector<char> buffer(chunkSize);

        size_t remaining = bytes;
        while (remaining > 0)
        {
            size_t toWrite = std::min(chunkSize, remaining);
            for (size_t i = 0; i < toWrite; ++i)
            {
                buffer[i] = kAlphabet[charDist(rng)];
            }

            stream.write(buffer.data(), static_cast<std::streamsize>(toWrite));
            if (!stream)
            {
                hr = E_FAIL;
                error = "Failed while writing random text data";
                return false;
            }

            remaining -= toWrite;
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush random text file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool WriteCompressibleTextFile(const fs::path& absolutePath, size_t bytes, std::string& error, HRESULT& hr)
    {
        if (!EnsureParentDirectories(absolutePath, error, hr))
        {
            return false;
        }

        std::ofstream stream(absolutePath, std::ios::binary | std::ios::trunc);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for writing: " << absolutePath;
            error = oss.str();
            return false;
        }

        // Fill with a single repeating character for maximum compression
        static constexpr char kRepeatChar = 'A';
        const size_t chunkSize = 4096;
        std::vector<char> buffer(chunkSize, kRepeatChar);

        size_t remaining = bytes;
        while (remaining > 0)
        {
            size_t toWrite = std::min(chunkSize, remaining);
            stream.write(buffer.data(), static_cast<std::streamsize>(toWrite));
            if (!stream)
            {
                hr = E_FAIL;
                error = "Failed while writing compressible text data";
                return false;
            }

            remaining -= toWrite;
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush compressible text file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool WriteTextFileWithContent(const fs::path& absolutePath, const std::string& content, std::string& error, HRESULT& hr)
    {
        if (!EnsureParentDirectories(absolutePath, error, hr))
        {
            return false;
        }

        std::ofstream stream(absolutePath, std::ios::binary | std::ios::trunc);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for writing: " << absolutePath;
            error = oss.str();
            return false;
        }

        stream.write(content.data(), static_cast<std::streamsize>(content.size()));
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed while writing text content";
            return false;
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush text file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool ModifyFileWithPattern(const fs::path& absolutePath, size_t bytesToModify, const std::vector<uint8_t>& pattern, std::string& error, HRESULT& hr)
    {
        if (pattern.empty())
        {
            hr = E_INVALIDARG;
            error = "pattern must include at least one byte";
            return false;
        }

        std::error_code ec;
        if (!fs::exists(absolutePath, ec) || !fs::is_regular_file(absolutePath, ec))
        {
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            std::ostringstream oss;
            oss << "File not found for modification: " << absolutePath;
            error = oss.str();
            return false;
        }

        // Open file in read-write mode
        std::fstream stream(absolutePath, std::ios::in | std::ios::out | std::ios::binary);
        if (!stream)
        {
            hr = E_FAIL;
            std::ostringstream oss;
            oss << "Failed to open file for modification: " << absolutePath;
            error = oss.str();
            return false;
        }

        // Seek to beginning
        stream.seekp(0, std::ios::beg);
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to seek to beginning of file";
            return false;
        }

        // Write pattern bytes
        size_t remaining = bytesToModify;
        const size_t chunkSize = 4096;
        std::vector<uint8_t> buffer;
        buffer.reserve(chunkSize);

        while (remaining > 0)
        {
            buffer.clear();
            size_t toWrite = std::min(chunkSize, remaining);
            while (buffer.size() < toWrite)
            {
                const size_t copyCount = std::min(pattern.size(), toWrite - buffer.size());
                buffer.insert(buffer.end(), pattern.begin(), pattern.begin() + copyCount);
            }

            stream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<std::streamsize>(toWrite));
            if (!stream)
            {
                hr = E_FAIL;
                error = "Failed while modifying file";
                return false;
            }

            remaining -= toWrite;
        }

        stream.flush();
        if (!stream)
        {
            hr = E_FAIL;
            error = "Failed to flush modified file";
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool CopyFileRelative(const fs::path& root, const fs::path& sourceRelative, const fs::path& destinationRelative, std::string& error, HRESULT& hr)
    {
        const fs::path source = MakeAbsolutePath(root, sourceRelative);
        const fs::path destination = MakeAbsolutePath(root, destinationRelative);

        std::error_code ec;
        if (!fs::exists(source, ec) || !fs::is_regular_file(source, ec))
        {
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            std::ostringstream oss;
            oss << "Source file not found: " << sourceRelative;
            error = oss.str();
            return false;
        }

        if (!EnsureParentDirectories(destination, error, hr))
        {
            return false;
        }

        fs::copy_file(source, destination, fs::copy_options::overwrite_existing, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to copy file from " << sourceRelative << " to " << destinationRelative << ": " << ec.message();
            error = oss.str();
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool RenameRelativePath(const fs::path& root, const fs::path& sourceRelative, const fs::path& destinationRelative, std::string& error, HRESULT& hr)
    {
        const fs::path source = MakeAbsolutePath(root, sourceRelative);
        const fs::path destination = MakeAbsolutePath(root, destinationRelative);

        std::error_code ec;
        if (!fs::exists(source, ec))
        {
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            std::ostringstream oss;
            oss << "Source path not found: " << sourceRelative;
            error = oss.str();
            return false;
        }

        if (!EnsureParentDirectories(destination, error, hr))
        {
            return false;
        }

        if (fs::exists(destination, ec))
        {
            std::error_code removeEc;
            if (fs::is_directory(destination, removeEc))
            {
                fs::remove_all(destination, removeEc);
            }
            else
            {
                fs::remove(destination, removeEc);
            }

            if (removeEc)
            {
                hr = ConvertFilesystemError(removeEc);
                std::ostringstream oss;
                oss << "Failed to remove existing destination before rename: " << destinationRelative << ": " << removeEc.message();
                error = oss.str();
                return false;
            }
        }

        fs::rename(source, destination, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to rename " << sourceRelative << " to " << destinationRelative << ": " << ec.message();
            error = oss.str();
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool DeleteFileRelative(const fs::path& root, const fs::path& relative, std::string& error, HRESULT& hr)
    {
        const fs::path absolute = MakeAbsolutePath(root, relative);
        std::error_code ec;
        if (!fs::exists(absolute, ec))
        {
            hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            std::ostringstream oss;
            oss << "File not found: " << relative;
            error = oss.str();
            return false;
        }

        if (!fs::is_regular_file(absolute, ec))
        {
            hr = E_INVALIDARG;
            std::ostringstream oss;
            oss << "Path is not a file: " << relative;
            error = oss.str();
            return false;
        }

        fs::remove(absolute, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to delete file " << relative << ": " << ec.message();
            error = oss.str();
            return false;
        }

        hr = S_OK;
        return true;
    }

    bool DeleteFolderRelative(const fs::path& root, const fs::path& relative, std::string& error, HRESULT& hr)
    {
        const fs::path absolute = MakeAbsolutePath(root, relative);
        std::error_code ec;
        if (!fs::exists(absolute, ec))
        {
            hr = HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND);
            std::ostringstream oss;
            oss << "Folder not found: " << relative;
            error = oss.str();
            return false;
        }

        if (!fs::is_directory(absolute, ec))
        {
            hr = E_INVALIDARG;
            std::ostringstream oss;
            oss << "Path is not a directory: " << relative;
            error = oss.str();
            return false;
        }

        fs::remove_all(absolute, ec);
        if (ec)
        {
            hr = ConvertFilesystemError(ec);
            std::ostringstream oss;
            oss << "Failed to delete folder " << relative << ": " << ec.message();
            error = oss.str();
            return false;
        }

        hr = S_OK;
        return true;
    }

    std::vector<fs::path> EnumerateDirectories(const fs::path& root, bool includeRoot)
    {
        std::vector<fs::path> directories;
        if (includeRoot)
        {
            directories.emplace_back();
        }

        std::error_code ec;
        if (!fs::exists(root, ec))
        {
            return directories;
        }

        for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec))
        {
            if (ec)
            {
                break;
            }

            if (!it->is_directory())
            {
                continue;
            }

            fs::path relative = fs::relative(it->path(), root, ec);
            if (ec)
            {
                ec.clear();
                continue;
            }

            if (ContainsReservedComponent(relative))
            {
                continue;
            }

            directories.push_back(relative.lexically_normal());
        }

        std::sort(directories.begin(), directories.end(), [](const fs::path& a, const fs::path& b)
        {
            return a.generic_u8string() < b.generic_u8string();
        });

        directories.erase(std::unique(directories.begin(), directories.end(), [](const fs::path& a, const fs::path& b)
        {
            return a.generic_u8string() == b.generic_u8string();
        }), directories.end());

        return directories;
    }

    std::vector<fs::path> EnumerateFiles(const fs::path& root)
    {
        std::vector<fs::path> files;
        std::error_code ec;
        if (!fs::exists(root, ec))
        {
            return files;
        }

        for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end; it != end; it.increment(ec))
        {
            if (ec)
            {
                break;
            }

            if (!it->is_regular_file())
            {
                continue;
            }

            fs::path relative = fs::relative(it->path(), root, ec);
            if (ec)
            {
                ec.clear();
                continue;
            }

            if (ContainsReservedComponent(relative))
            {
                continue;
            }

            files.push_back(relative.lexically_normal());
        }

        std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b)
        {
            return a.generic_u8string() < b.generic_u8string();
        });

        files.erase(std::unique(files.begin(), files.end(), [](const fs::path& a, const fs::path& b)
        {
            return a.generic_u8string() == b.generic_u8string();
        }), files.end());

        return files;
    }

    std::string GenerateRandomName(std::mt19937& rng, size_t minLength = 6, size_t maxLength = 12)
    {
        static constexpr char kNameAlphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        constexpr size_t nameAlphabetLength = sizeof(kNameAlphabet) - 1;
        std::uniform_int_distribution<size_t> lengthDist(minLength, maxLength);
        std::uniform_int_distribution<size_t> charDist(0, nameAlphabetLength - 1);

        const size_t length = lengthDist(rng);
        std::string name;
        name.reserve(length);
        for (size_t i = 0; i < length; ++i)
        {
            name.push_back(kNameAlphabet[charDist(rng)]);
        }

        if (ToLowerCopy(name) == kReservedMetadataFolder)
        {
            name.push_back('x');
        }

        return name;
    }



    struct ChaosConfig
    {
        int operationCount{ 0 };
        uint32_t seed{ 0 };
        bool seedProvided{ false };
        int minBytes{ 256 };
        int maxBytes{ 32768 };
        std::vector<std::string> verbs;
    };

    bool TryGetStringField(const json& node, const char* key, bool required, std::string& value, std::string& error)
    {
        auto it = node.find(key);
        if (it == node.end())
        {
            if (required)
            {
                error = std::string("Missing field '") + key + "'";
                return false;
            }

            return true;
        }

        if (!it->is_string())
        {
            error = std::string("Field '") + key + "' must be a string";
            return false;
        }

        value = it->get<std::string>();
        return true;
    }

    bool TryGetInt64Field(const json& node, const char* key, bool required, int64_t& value, std::string& error)
    {
        auto it = node.find(key);
        if (it == node.end())
        {
            if (required)
            {
                error = std::string("Missing field '") + key + "'";
                return false;
            }

            return true;
        }

        if (it->is_number_integer())
        {
            value = it->get<int64_t>();
            return true;
        }

        if (it->is_number_unsigned())
        {
            value = static_cast<int64_t>(it->get<uint64_t>());
            return true;
        }

        if (it->is_string())
        {
            try
            {
                size_t consumed = 0;
                const std::string text = it->get<std::string>();
                value = std::stoll(text, &consumed, 0);
                if (consumed == text.size())
                {
                    return true;
                }
            }
            catch (...)
            {
                // fall through to error
            }
        }

        error = std::string("Field '") + key + "' must be an integer";
        return false;
    }

    const std::vector<std::string>& SupportedWriteVerbs()
    {
        static const std::vector<std::string> verbs{
            "CreateBinaryFile",
            "CreateTextFile",
            "CreateRandomBinaryFile",
            "CreateRandomText",
            "CreateMultipleBinaryFiles",
            "CreateMultipleTextFiles",
            "ModifyFile",
            "CreateFolder",
            "CopyFile",
            "RenameFile",
            "RenameFolder",
            "DeleteFolder",
            "DeleteFile"
        };

        return verbs;
    }

    const std::vector<std::string>& DefaultChaosVerbs()
    {
        static const std::vector<std::string> verbs{
            "CreateRandomBinaryFile",
            "CreateRandomText",
            "CreateFolder",
            "RenameFile",
            "RenameFolder",
            "DeleteFile",
            "DeleteFolder"
        };

        return verbs;
    }

    bool TryParseChaosConfig(const json& parameters, const std::string& commandId, bool& hasChaos, ChaosConfig& config, std::string& error)
    {
        (void)commandId;
        hasChaos = false;
        auto chaosIt = parameters.find("chaos");
        if (chaosIt == parameters.end())
        {
            return true;
        }

        if (!chaosIt->is_object())
        {
            error = "chaos must be an object";
            return false;
        }

        hasChaos = true;
        const auto& chaosObj = *chaosIt;

        int64_t count = 0;
        if (!TryGetInt64Field(chaosObj, "operationCount", true, count, error))
        {
            return false;
        }

        if (count <= 0)
        {
            error = "chaos.operationCount must be greater than zero";
            return false;
        }

        config.operationCount = static_cast<int>((count > static_cast<int64_t>(std::numeric_limits<int>::max())) ? std::numeric_limits<int>::max() : count);

        int64_t seedValue = 0;
        if (auto seedIt = chaosObj.find("seed"); seedIt != chaosObj.end())
        {
            if (!TryGetInt64Field(chaosObj, "seed", true, seedValue, error))
            {
                return false;
            }

            if (seedValue < 0)
            {
                error = "chaos.seed must be non-negative";
                return false;
            }

            config.seed = static_cast<uint32_t>(seedValue & 0xFFFFFFFFu);
            config.seedProvided = true;
        }

        if (!config.seedProvided)
        {
            config.seed = static_cast<uint32_t>(std::rand());
        }

        int64_t minBytes = config.minBytes;
        int64_t maxBytes = config.maxBytes;
        if (auto minIt = chaosObj.find("minFileBytes"); minIt != chaosObj.end())
        {
            if (!TryGetInt64Field(chaosObj, "minFileBytes", true, minBytes, error))
            {
                return false;
            }
        }

        if (auto maxIt = chaosObj.find("maxFileBytes"); maxIt != chaosObj.end())
        {
            if (!TryGetInt64Field(chaosObj, "maxFileBytes", true, maxBytes, error))
            {
                return false;
            }
        }

        if (minBytes < 0 || maxBytes < 0)
        {
            error = "chaos min/max bytes must be non-negative";
            return false;
        }

        if (minBytes > maxBytes)
        {
            std::swap(minBytes, maxBytes);
        }

        config.minBytes = static_cast<int>((minBytes > static_cast<int64_t>(std::numeric_limits<int>::max())) ? std::numeric_limits<int>::max() : minBytes);
        config.maxBytes = static_cast<int>((maxBytes > static_cast<int64_t>(std::numeric_limits<int>::max())) ? std::numeric_limits<int>::max() : maxBytes);

        auto verbsIt = chaosObj.find("verbs");
        if (verbsIt != chaosObj.end())
        {
            if (!verbsIt->is_array() || verbsIt->empty())
            {
                error = "chaos.verbs must be a non-empty array";
                return false;
            }

            std::vector<std::string> parsedVerbs;
            for (const auto& verbNode : *verbsIt)
            {
                if (!verbNode.is_string())
                {
                    error = "chaos.verbs entries must be strings";
                    return false;
                }

                const std::string verb = verbNode.get<std::string>();
                if (std::find(SupportedWriteVerbs().begin(), SupportedWriteVerbs().end(), verb) == SupportedWriteVerbs().end())
                {
                    error = std::string("Unsupported chaos verb '") + verb + "'";
                    return false;
                }

                parsedVerbs.push_back(verb);
            }

            config.verbs = std::move(parsedVerbs);
        }
        else
        {
            config.verbs = DefaultChaosVerbs();
        }

        return true;
    }

    enum class MutationOutcome
    {
        Applied,
        NotFeasible,
        Error
    };

    void AppendMutationRecord(
        std::vector<json>& records,
        const std::string& origin,
        const std::string& verb,
        const json& details)
    {
        json record = json::object();
        record["origin"] = origin;
        record["verb"] = verb;
        if (!details.is_null())
        {
            record["details"] = details;
        }

        records.push_back(std::move(record));
    }

    void LogMutationEvent(const std::string& origin, const std::string& verb, const std::string& description)
    {
        LogToWindowFormat("WriteGameSaveData[%s] %s - %s", origin.c_str(), verb.c_str(), description.c_str());
    }

    bool TryGenerateUniqueRelativePath(
        std::mt19937& rng,
        const fs::path& base,
        const std::string& suffix,
        fs::path& relativeOut,
        std::string& error)
    {
        error.clear();
        for (int attempt = 0; attempt < 16; ++attempt)
        {
            std::string name = GenerateRandomName(rng);
            fs::path candidate = base / (name + suffix);
            fs::path normalized;
            if (TryNormalizeRelativePath(GetStringFromU8String(candidate.generic_u8string()), normalized, error))
            {
                relativeOut = normalized;
                error.clear();
                return true;
            }
        }

        error = "Failed to generate a unique relative path";
        return false;
    }

    MutationOutcome ExecuteChaosVerb(
        const fs::path& root,
        const ChaosConfig& config,
        std::mt19937& rng,
        const std::string& verb,
        std::vector<json>& records,
        HRESULT& hr,
        std::string& error)
    {
        error.clear();
        hr = S_OK;

        auto directoriesIncludingRoot = EnumerateDirectories(root, true);
        auto directoriesWithoutRoot = EnumerateDirectories(root, false);
        auto files = EnumerateFiles(root);

        auto chooseDirectory = [&](bool includeRoot) -> std::optional<fs::path>
        {
            const auto& dirs = includeRoot ? directoriesIncludingRoot : directoriesWithoutRoot;
            if (dirs.empty())
            {
                return std::nullopt;
            }

            std::uniform_int_distribution<size_t> dist(0, dirs.size() - 1);
            return dirs[dist(rng)];
        };

        auto chooseFile = [&]() -> std::optional<fs::path>
        {
            if (files.empty())
            {
                return std::nullopt;
            }

            std::uniform_int_distribution<size_t> dist(0, files.size() - 1);
            return files[dist(rng)];
        };

        auto chooseByteCount = [&]() -> size_t
        {
            if (config.maxBytes <= config.minBytes)
            {
                return static_cast<size_t>(std::max(config.minBytes, 0));
            }

            std::uniform_int_distribution<int> dist(config.minBytes, config.maxBytes);
            return static_cast<size_t>(dist(rng));
        };

        auto recordAndLog = [&](const std::string& description, const json& details)
        {
            AppendMutationRecord(records, "chaos", verb, details);
            LogMutationEvent("chaos", verb, description);
        };

        if (verb == "CreateRandomBinaryFile")
        {
            auto baseOpt = chooseDirectory(true);
            if (!baseOpt)
            {
                baseOpt = fs::path();
            }

            fs::path relative;
            if (!TryGenerateUniqueRelativePath(rng, *baseOpt, ".bin", relative, error))
            {
                hr = E_FAIL;
                return MutationOutcome::Error;
            }

            const size_t bytes = chooseByteCount();
            std::string opError;
            if (!WriteRandomBinaryFile(MakeAbsolutePath(root, relative), bytes, rng, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = static_cast<int64_t>(bytes);
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "CreateRandomText")
        {
            auto baseOpt = chooseDirectory(true);
            if (!baseOpt)
            {
                baseOpt = fs::path();
            }

            fs::path relative;
            if (!TryGenerateUniqueRelativePath(rng, *baseOpt, ".txt", relative, error))
            {
                hr = E_FAIL;
                return MutationOutcome::Error;
            }

            const size_t bytes = chooseByteCount();
            std::string opError;
            if (!WriteRandomTextFile(MakeAbsolutePath(root, relative), bytes, rng, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = static_cast<int64_t>(bytes);
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "CreateFolder")
        {
            auto baseOpt = chooseDirectory(true);
            if (!baseOpt)
            {
                baseOpt = fs::path();
            }

            fs::path relative;
            if (!TryGenerateUniqueRelativePath(rng, *baseOpt, "", relative, error))
            {
                hr = E_FAIL;
                return MutationOutcome::Error;
            }

            std::string opError;
            if (!EnsureDirectoryExists(MakeAbsolutePath(root, relative), opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "RenameFile")
        {
            auto sourceOpt = chooseFile();
            if (!sourceOpt)
            {
                return MutationOutcome::NotFeasible;
            }

            const fs::path sourceRelative = sourceOpt->lexically_normal();
            const fs::path parent = sourceRelative.parent_path();

            fs::path destinationRelative;
            std::string validationError;
            for (int attempt = 0; attempt < 16; ++attempt)
            {
                std::string stem = GenerateRandomName(rng);
                fs::path candidate = parent / (stem + GetStringFromU8String(sourceRelative.extension().generic_u8string()));
                if (TryNormalizeRelativePath(GetStringFromU8String(candidate.generic_u8string()), destinationRelative, validationError) && destinationRelative != sourceRelative)
                {
                    break;
                }
            }

            if (destinationRelative.empty())
            {
                hr = E_FAIL;
                error = validationError.empty() ? "Failed to build destination path for RenameFile" : validationError;
                return MutationOutcome::Error;
            }

            std::string opError;
            if (!RenameRelativePath(root, sourceRelative, destinationRelative, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["sourcePath"] = PathToString(sourceRelative);
            details["destinationPath"] = PathToString(destinationRelative);
            recordAndLog(PathToString(sourceRelative) + " -> " + PathToString(destinationRelative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "RenameFolder")
        {
            auto sourceOpt = chooseDirectory(false);
            if (!sourceOpt)
            {
                return MutationOutcome::NotFeasible;
            }

            const fs::path sourceRelative = sourceOpt->lexically_normal();
            const fs::path parent = sourceRelative.parent_path();

            fs::path destinationRelative;
            std::string validationError;
            for (int attempt = 0; attempt < 16; ++attempt)
            {
                std::string name = GenerateRandomName(rng);
                fs::path candidate = parent / name;
                if (TryNormalizeRelativePath(GetStringFromU8String(candidate.generic_u8string()), destinationRelative, validationError) && destinationRelative != sourceRelative)
                {
                    break;
                }
            }

            if (destinationRelative.empty())
            {
                hr = E_FAIL;
                error = validationError.empty() ? "Failed to build destination path for RenameFolder" : validationError;
                return MutationOutcome::Error;
            }

            std::string opError;
            if (!RenameRelativePath(root, sourceRelative, destinationRelative, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["sourcePath"] = PathToString(sourceRelative);
            details["destinationPath"] = PathToString(destinationRelative);
            recordAndLog(PathToString(sourceRelative) + " -> " + PathToString(destinationRelative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "DeleteFile")
        {
            auto targetOpt = chooseFile();
            if (!targetOpt)
            {
                return MutationOutcome::NotFeasible;
            }

            const fs::path relative = targetOpt->lexically_normal();
            std::string opError;
            if (!DeleteFileRelative(root, relative, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "DeleteFolder")
        {
            auto targetOpt = chooseDirectory(false);
            if (!targetOpt)
            {
                return MutationOutcome::NotFeasible;
            }

            const fs::path relative = targetOpt->lexically_normal();
            std::string opError;
            if (!DeleteFolderRelative(root, relative, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "CopyFile")
        {
            auto sourceOpt = chooseFile();
            if (!sourceOpt)
            {
                return MutationOutcome::NotFeasible;
            }

            const fs::path sourceRelative = sourceOpt->lexically_normal();
            auto baseOpt = chooseDirectory(true);
            if (!baseOpt)
            {
                baseOpt = fs::path();
            }

            fs::path destinationRelative;
            std::string validationError;
            for (int attempt = 0; attempt < 16; ++attempt)
            {
                std::string stem = GenerateRandomName(rng);
                fs::path candidate = *baseOpt / (stem + GetStringFromU8String(sourceRelative.extension().generic_u8string()));
                if (TryNormalizeRelativePath(GetStringFromU8String(candidate.generic_u8string()), destinationRelative, validationError) && destinationRelative != sourceRelative)
                {
                    break;
                }
            }

            if (destinationRelative.empty())
            {
                hr = E_FAIL;
                error = validationError.empty() ? "Failed to build destination path for CopyFile" : validationError;
                return MutationOutcome::Error;
            }

            std::string opError;
            if (!CopyFileRelative(root, sourceRelative, destinationRelative, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["sourcePath"] = PathToString(sourceRelative);
            details["destinationPath"] = PathToString(destinationRelative);
            recordAndLog(PathToString(sourceRelative) + " -> " + PathToString(destinationRelative), details);
            return MutationOutcome::Applied;
        }

        if (verb == "CreateBinaryFile")
        {
            auto baseOpt = chooseDirectory(true);
            if (!baseOpt)
            {
                baseOpt = fs::path();
            }

            fs::path relative;
            if (!TryGenerateUniqueRelativePath(rng, *baseOpt, ".bin", relative, error))
            {
                hr = E_FAIL;
                return MutationOutcome::Error;
            }

            const size_t bytes = chooseByteCount();
            std::vector<uint8_t> pattern{ 0xAA, 0x55 };
            std::string opError;
            if (!WriteBinaryFileWithPattern(MakeAbsolutePath(root, relative), bytes, pattern, opError, hr))
            {
                error = opError;
                return MutationOutcome::Error;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = static_cast<int64_t>(bytes);
            details["pattern"] = json::array({ 0xAA, 0x55 });
            recordAndLog(PathToString(relative), details);
            return MutationOutcome::Applied;
        }

        hr = E_INVALIDARG;
        error = std::string("Unsupported chaos verb '") + verb + "'";
        return MutationOutcome::Error;
    }

    // Bias chaos verb selection toward additive mutations so destructive verbs are less frequent.
    uint32_t ChaosVerbWeight(std::string_view verb)
    {
        if (verb == "DeleteFile" || verb == "DeleteFolder")
        {
            return 1;
        }

        if (verb == "RenameFile" || verb == "RenameFolder" || verb == "CopyFile")
        {
            return 3;
        }

        return 5;
    }

    std::vector<uint32_t> BuildChaosVerbWeights(const std::vector<std::string>& verbs)
    {
        std::vector<uint32_t> weights;
        weights.reserve(verbs.size());
        for (const auto& verb : verbs)
        {
            uint32_t weight = ChaosVerbWeight(verb);
            if (weight == 0)
            {
                weight = 1;
            }
            weights.push_back(weight);
        }
        return weights;
    }

    bool ExecuteChaosMutations(
        const fs::path& root,
        const ChaosConfig& config,
        std::vector<json>& records,
        HRESULT& failureHr,
        std::string& error)
    {
        std::mt19937 rng(config.seed);
        failureHr = S_OK;
        error.clear();

        if (config.verbs.empty())
        {
            return true;
        }

        const auto verbWeights = BuildChaosVerbWeights(config.verbs);
        uint32_t totalWeight = 0;
        for (uint32_t weight : verbWeights)
        {
            totalWeight += weight;
        }

        if (totalWeight == 0)
        {
            totalWeight = static_cast<uint32_t>(config.verbs.size());
        }

        for (int i = 0; i < config.operationCount; ++i)
        {
            // Use the global rand() so chaos runs follow the seed set by the handler.
            uint32_t pick = static_cast<uint32_t>(std::rand());
            pick %= totalWeight;
            size_t startIndex = 0;
            uint32_t cumulative = 0;
            for (size_t idx = 0; idx < verbWeights.size(); ++idx)
            {
                cumulative += verbWeights[idx];
                if (pick < cumulative)
                {
                    startIndex = idx;
                    break;
                }
            }
            bool applied = false;

            for (size_t attempt = 0; attempt < config.verbs.size(); ++attempt)
            {
                const std::string& verb = config.verbs[(startIndex + attempt) % config.verbs.size()];
                HRESULT hr = S_OK;
                std::string opError;
                MutationOutcome outcome = ExecuteChaosVerb(root, config, rng, verb, records, hr, opError);
                if (outcome == MutationOutcome::Applied)
                {
                    applied = true;
                    break;
                }

                if (outcome == MutationOutcome::Error)
                {
                    failureHr = hr;
                    error = opError;
                    return false;
                }
            }

            if (!applied)
            {
                failureHr = E_FAIL;
                error = "No feasible chaos verbs available for current save root state";
                return false;
            }
        }

        return true;
    }

    bool ExecuteScriptedOperation(
        const fs::path& root,
        const std::string& commandId,
        size_t operationIndex,
        const json& operationNode,
        std::vector<json>& records,
        HRESULT& hr,
        std::string& error)
    {
        (void)commandId;
        (void)operationIndex;
        error.clear();
        hr = S_OK;

        if (!operationNode.is_object())
        {
            error = "operations entries must be objects";
            hr = E_INVALIDARG;
            return false;
        }

        std::string verb;
        if (!TryGetStringField(operationNode, "verb", true, verb, error))
        {
            hr = E_INVALIDARG;
            return false;
        }

        if (std::find(SupportedWriteVerbs().begin(), SupportedWriteVerbs().end(), verb) == SupportedWriteVerbs().end())
        {
            error = std::string("Unsupported verb '") + verb + "'";
            hr = E_INVALIDARG;
            return false;
        }

        auto recordAndLog = [&](const std::string& description, const json& details)
        {
            AppendMutationRecord(records, "script", verb, details);
            LogMutationEvent("script", verb, description);
        };

        auto normalize = [&](const std::string& raw, fs::path& normalized) -> bool
        {
            if (!TryNormalizeRelativePath(raw, normalized, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            return true;
        };

        if (verb == "CreateBinaryFile")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            int64_t bytesValue = 0;
            if (!TryGetInt64Field(operationNode, "bytes", true, bytesValue, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (bytesValue < 0)
            {
                error = "bytes must be non-negative";
                hr = E_INVALIDARG;
                return false;
            }

            auto patternIt = operationNode.find("pattern");
            if (patternIt == operationNode.end())
            {
                error = "CreateBinaryFile requires 'pattern'";
                hr = E_INVALIDARG;
                return false;
            }

            std::string patternError;
            std::vector<uint8_t> pattern = ParsePatternBytes(*patternIt, patternError);
            if (!patternError.empty())
            {
                error = patternError;
                hr = E_INVALIDARG;
                return false;
            }

            std::string opError;
            if (!WriteBinaryFileWithPattern(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesValue), pattern, opError, hr))
            {
                error = opError;
                return false;
            }

            json patternJson = json::array();
            for (uint8_t value : pattern)
            {
                patternJson.push_back(static_cast<int>(value));
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = bytesValue;
            details["pattern"] = patternJson;
            recordAndLog(PathToString(relative), details);
            return true;
        }

        if (verb == "CreateTextFile")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            std::string content;
            if (!TryGetStringField(operationNode, "content", true, content, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            std::string opError;
            if (!WriteTextFileWithContent(MakeAbsolutePath(root, relative), content, opError, hr))
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = static_cast<int64_t>(content.size());
            recordAndLog(PathToString(relative), details);
            return true;
        }

        if (verb == "CreateMultipleBinaryFiles")
        {
            std::string baseDirectoryText;
            if (!TryGetStringField(operationNode, "baseDirectory", true, baseDirectoryText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path baseDirectory;
            if (!normalize(baseDirectoryText, baseDirectory))
            {
                return false;
            }

            int64_t fileCount = 0;
            if (!TryGetInt64Field(operationNode, "fileCount", true, fileCount, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (fileCount <= 0)
            {
                error = "fileCount must be positive";
                hr = E_INVALIDARG;
                return false;
            }

            int64_t bytesPerFile = 0;
            if (!TryGetInt64Field(operationNode, "bytesPerFile", true, bytesPerFile, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (bytesPerFile < 0)
            {
                error = "bytesPerFile must be non-negative";
                hr = E_INVALIDARG;
                return false;
            }

            auto patternIt = operationNode.find("pattern");
            if (patternIt == operationNode.end())
            {
                error = "CreateMultipleBinaryFiles requires 'pattern'";
                hr = E_INVALIDARG;
                return false;
            }

            std::string patternError;
            std::vector<uint8_t> pattern = ParsePatternBytes(*patternIt, patternError);
            if (!patternError.empty())
            {
                error = patternError;
                hr = E_INVALIDARG;
                return false;
            }

            // Check for optional uncompressible parameter
            bool uncompressible = false;
            if (auto uncompressibleIt = operationNode.find("uncompressible"); uncompressibleIt != operationNode.end())
            {
                if (uncompressibleIt->is_boolean())
                {
                    uncompressible = uncompressibleIt->get<bool>();
                }
                else if (uncompressibleIt->is_string())
                {
                    std::string strValue = uncompressibleIt->get<std::string>();
                    uncompressible = (strValue == "true" || strValue == "True" || strValue == "TRUE");
                }
            }

            // Create all files
            if (uncompressible)
            {
                // Use random binary files for uncompressible data
                std::mt19937 rng(static_cast<uint32_t>(std::rand()));
                for (int64_t i = 0; i < fileCount; ++i)
                {
                    // Generate filename: file0000.bin, file0001.bin, etc.
                    std::ostringstream filenameStream;
                    filenameStream << "file" << std::setfill('0') << std::setw(4) << i << ".bin";
                    fs::path relative = baseDirectory / filenameStream.str();

                    std::string opError;
                    if (!WriteRandomBinaryFile(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesPerFile), rng, opError, hr))
                    {
                        error = opError;
                        return false;
                    }
                }
            }
            else
            {
                // Use pattern-based binary files
                for (int64_t i = 0; i < fileCount; ++i)
                {
                    // Generate filename: file0000.bin, file0001.bin, etc.
                    std::ostringstream filenameStream;
                    filenameStream << "file" << std::setfill('0') << std::setw(4) << i << ".bin";
                    fs::path relative = baseDirectory / filenameStream.str();

                    std::string opError;
                    if (!WriteBinaryFileWithPattern(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesPerFile), pattern, opError, hr))
                    {
                        error = opError;
                        return false;
                    }
                }
            }

            json details = json::object();
            details["baseDirectory"] = PathToString(baseDirectory);
            details["fileCount"] = fileCount;
            details["bytesPerFile"] = bytesPerFile;
            details["totalBytes"] = fileCount * bytesPerFile;
            recordAndLog(PathToString(baseDirectory) + " (" + std::to_string(fileCount) + " files)", details);
            return true;
        }

        if (verb == "CreateMultipleTextFiles")
        {
            std::string baseDirectoryText;
            if (!TryGetStringField(operationNode, "baseDirectory", true, baseDirectoryText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path baseDirectory;
            if (!normalize(baseDirectoryText, baseDirectory))
            {
                return false;
            }

            int64_t fileCount = 0;
            if (!TryGetInt64Field(operationNode, "fileCount", true, fileCount, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (fileCount <= 0)
            {
                error = "fileCount must be positive";
                hr = E_INVALIDARG;
                return false;
            }

            int64_t bytesPerFile = 0;
            if (!TryGetInt64Field(operationNode, "bytesPerFile", true, bytesPerFile, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (bytesPerFile < 0)
            {
                error = "bytesPerFile must be non-negative";
                hr = E_INVALIDARG;
                return false;
            }

            // Check for optional compressible parameter
            bool compressible = false;
            if (auto compressibleIt = operationNode.find("compressible"); compressibleIt != operationNode.end())
            {
                if (compressibleIt->is_boolean())
                {
                    compressible = compressibleIt->get<bool>();
                }
                else if (compressibleIt->is_string())
                {
                    std::string strValue = compressibleIt->get<std::string>();
                    compressible = (strValue == "true" || strValue == "True" || strValue == "TRUE");
                }
            }

            // Create all files
            if (compressible)
            {
                // Use compressible text files (single repeating character)
                for (int64_t i = 0; i < fileCount; ++i)
                {
                    std::ostringstream filenameStream;
                    filenameStream << "file" << std::setfill('0') << std::setw(4) << i << ".txt";
                    fs::path relative = baseDirectory / filenameStream.str();

                    std::string opError;
                    if (!WriteCompressibleTextFile(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesPerFile), opError, hr))
                    {
                        error = opError;
                        return false;
                    }
                }
            }
            else
            {
                // Use random text files
                std::mt19937 rng(static_cast<uint32_t>(std::rand()));
                for (int64_t i = 0; i < fileCount; ++i)
                {
                    std::ostringstream filenameStream;
                    filenameStream << "file" << std::setfill('0') << std::setw(4) << i << ".txt";
                    fs::path relative = baseDirectory / filenameStream.str();

                    std::string opError;
                    if (!WriteRandomTextFile(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesPerFile), rng, opError, hr))
                    {
                        error = opError;
                        return false;
                    }
                }
            }

            json details = json::object();
            details["baseDirectory"] = PathToString(baseDirectory);
            details["fileCount"] = fileCount;
            details["bytesPerFile"] = bytesPerFile;
            details["totalBytes"] = fileCount * bytesPerFile;
            recordAndLog(PathToString(baseDirectory) + " (" + std::to_string(fileCount) + " files)", details);
            return true;
        }

        if (verb == "ModifyFile")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            int64_t bytesValue = 0;
            if (!TryGetInt64Field(operationNode, "bytes", true, bytesValue, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (bytesValue < 0)
            {
                error = "bytes must be non-negative";
                hr = E_INVALIDARG;
                return false;
            }

            auto patternIt = operationNode.find("pattern");
            if (patternIt == operationNode.end())
            {
                error = "ModifyFile requires 'pattern'";
                hr = E_INVALIDARG;
                return false;
            }

            std::string patternError;
            std::vector<uint8_t> pattern = ParsePatternBytes(*patternIt, patternError);
            if (!patternError.empty())
            {
                error = patternError;
                hr = E_INVALIDARG;
                return false;
            }

            std::string opError;
            if (!ModifyFileWithPattern(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesValue), pattern, opError, hr))
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = bytesValue;
            recordAndLog(PathToString(relative), details);
            return true;
        }

        if (verb == "CreateRandomBinaryFile" || verb == "CreateRandomText")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            int64_t bytesValue = 0;
            if (!TryGetInt64Field(operationNode, "bytes", true, bytesValue, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            if (bytesValue < 0)
            {
                error = "bytes must be non-negative";
                hr = E_INVALIDARG;
                return false;
            }

            std::mt19937 rng(static_cast<uint32_t>(std::rand()));
            std::string opError;
            bool success = false;
            if (verb == "CreateRandomBinaryFile")
            {
                success = WriteRandomBinaryFile(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesValue), rng, opError, hr);
            }
            else
            {
                success = WriteRandomTextFile(MakeAbsolutePath(root, relative), static_cast<size_t>(bytesValue), rng, opError, hr);
            }

            if (!success)
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            details["bytes"] = bytesValue;
            recordAndLog(PathToString(relative), details);
            return true;
        }

        if (verb == "CreateFolder")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            std::string opError;
            if (!EnsureDirectoryExists(MakeAbsolutePath(root, relative), opError, hr))
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            recordAndLog(PathToString(relative), details);
            return true;
        }

        if (verb == "CopyFile")
        {
            std::string sourceText;
            if (!TryGetStringField(operationNode, "sourceRelativePath", true, sourceText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path source;
            if (!normalize(sourceText, source))
            {
                return false;
            }

            std::string destinationText;
            if (!TryGetStringField(operationNode, "destinationRelativePath", true, destinationText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path destination;
            if (!normalize(destinationText, destination))
            {
                return false;
            }

            std::string opError;
            if (!CopyFileRelative(root, source, destination, opError, hr))
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["sourcePath"] = PathToString(source);
            details["destinationPath"] = PathToString(destination);
            recordAndLog(PathToString(source) + " -> " + PathToString(destination), details);
            return true;
        }

        if (verb == "RenameFile" || verb == "RenameFolder")
        {
            std::string sourceText;
            if (!TryGetStringField(operationNode, "sourceRelativePath", true, sourceText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path source;
            if (!normalize(sourceText, source))
            {
                return false;
            }

            std::string destinationText;
            if (!TryGetStringField(operationNode, "destinationRelativePath", true, destinationText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path destination;
            if (!normalize(destinationText, destination))
            {
                return false;
            }

            std::string opError;
            if (!RenameRelativePath(root, source, destination, opError, hr))
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["sourcePath"] = PathToString(source);
            details["destinationPath"] = PathToString(destination);
            recordAndLog(PathToString(source) + " -> " + PathToString(destination), details);
            return true;
        }

        if (verb == "DeleteFolder" || verb == "DeleteFile")
        {
            std::string relativeText;
            if (!TryGetStringField(operationNode, "relativePath", true, relativeText, error))
            {
                hr = E_INVALIDARG;
                return false;
            }

            fs::path relative;
            if (!normalize(relativeText, relative))
            {
                return false;
            }

            std::string opError;
            bool success = false;
            if (verb == "DeleteFile")
            {
                success = DeleteFileRelative(root, relative, opError, hr);
            }
            else
            {
                success = DeleteFolderRelative(root, relative, opError, hr);
            }

            if (!success)
            {
                error = opError;
                return false;
            }

            json details = json::object();
            details["relativePath"] = PathToString(relative);
            recordAndLog(PathToString(relative), details);
            return true;
        }

        error = std::string("Unhandled verb '") + verb + "'";
        hr = E_INVALIDARG;
        return false;
    }
}

CommandResultPayload HandleWriteGameSaveData(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    const WriteGameSaveDataResult result = ExecuteWriteGameSaveData(state, commandId, parameters);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    nlohmann::json mutationsJson = nlohmann::json::array();
    for (const auto& entry : result.mutations)
    {
        mutationsJson.push_back(entry);
    }

    payload.result["mutations"] = std::move(mutationsJson);
    payload.result["mutationsApplied"] = static_cast<int>(result.mutations.size());
    payload.result["chaosMutationsApplied"] = result.chaosMutationsApplied;
    payload.result["scriptMutationsApplied"] = result.scriptMutationsApplied;
    payload.result["saveFolder"] = result.saveFolder;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

WriteGameSaveDataResult ExecuteWriteGameSaveData(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const nlohmann::json& parameters)
{
    WriteGameSaveDataResult result{};

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

    bool hasChaos = false;
    ChaosConfig chaosConfig;
    std::string parseError;
    if (!TryParseChaosConfig(parameters, commandId, hasChaos, chaosConfig, parseError))
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = parseError;
        return result;
    }

    if (hasChaos)
    {
        HRESULT chaosHr = S_OK;
        std::string chaosError;
        const size_t beforeSize = result.mutations.size();
        if (!ExecuteChaosMutations(rootPath, chaosConfig, result.mutations, chaosHr, chaosError))
        {
            result.hr = chaosHr;
            result.errorMessage = chaosError;
            return result;
        }

        result.chaosMutationsApplied = static_cast<int>(result.mutations.size() - beforeSize);
    }

    size_t operationIndex = 0;
    bool hasOperations = false;
    if (parameters.is_object())
    {
        auto operationsIt = parameters.find("operations");
        if (operationsIt != parameters.end())
        {
            hasOperations = true;
            if (!operationsIt->is_array())
            {
                result.hr = E_INVALIDARG;
                result.errorMessage = "operations must be an array";
                return result;
            }

            HRESULT opHr = S_OK;
            std::string opError;
            const size_t beforeScript = result.mutations.size();
            for (const auto& opNode : *operationsIt)
            {
                ++operationIndex;
                if (!ExecuteScriptedOperation(rootPath, commandId, operationIndex, opNode, result.mutations, opHr, opError))
                {
                    result.hr = opHr;
                    result.errorMessage = opError;
                    return result;
                }
            }

            result.scriptMutationsApplied = static_cast<int>(result.mutations.size() - beforeScript);
        }
    }

    if (!hasChaos && !hasOperations)
    {
        result.hr = E_INVALIDARG;
        result.errorMessage = "WriteGameSaveData requires 'operations' or a 'chaos' block";
        return result;
    }

    result.saveFolder = GetStringFromU8String(rootPath.u8string());
    result.hr = S_OK;

    LogToWindowFormat(
        "WriteGameSaveData completed (chaos=%d, script=%d)",
        result.chaosMutationsApplied,
        result.scriptMutationsApplied);

    return result;
}

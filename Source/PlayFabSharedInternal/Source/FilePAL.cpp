#include "pch.h"
#include "FilePAL_Generic.h"
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>
#if !HC_PLATFORM_IS_MICROSOFT
#include <utime.h>
#endif
#include <locale>
#include <codecvt>

namespace PlayFab
{

const String FilePAL::GetPathInvalidChars() noexcept
{
#if HC_PLATFORM_IS_MICROSOFT
    return "<>\"|?*";
#else
    return "";
#endif
}

char FilePAL::GetPathSeparatorChar() noexcept
{
#ifdef HC_PLATFORM_IS_MICROSOFT
    return '\\';
#else
    return '/';
#endif
}

std::filesystem::path ConvertStringToPath(const String& pathUtf8);

#if !HC_PLATFORM_IS_MICROSOFT
std::filesystem::path ConvertStringToPath(const String& pathUtf8)
{
    auto path = std::filesystem::path(pathUtf8.c_str());
    return path;
}
#endif

Result<FileHandle> FilePAL::OpenFile(const String& filePath, FileOpenMode openMode) noexcept
{
    std::ios::openmode mode = std::ios::binary;

    if (openMode == FileOpenMode::Read)
    {
        mode |= std::ios::in;
    }
    if (openMode == FileOpenMode::Write)
    {
        mode |= std::ios::out | std::ios::trunc;
    }

    auto fileHandle = MakeUnique<FileStreamContainer>();
    auto path = ConvertStringToPath(filePath);
    fileHandle->file.open(path.c_str(), mode);
    RETURN_HR_IF_FALSE(E_INVALIDARG, fileHandle->file);

    return UniquePtr<FileContainer>(static_cast<FileContainer*>(fileHandle.release()));;
}

HRESULT FilePAL::ReadFileBytes(FileHandle& fileHandle, size_t bytesAvailable, char* destination, size_t* bytesWritten) noexcept
{
    assert(fileHandle);
    assert(bytesAvailable > 0);
    assert(destination);
    assert(bytesWritten);

    FileStreamContainer* pfFile = static_cast<FileStreamContainer*>(fileHandle.get());

    // min(bytesAvailable, max_platform_chunk_size) to avoid reading too much at once because libcurl/xcurl provide some "nitems" buffer size that may be too large
    size_t constrainedBytesToRead = std::min(bytesAvailable, size_t(64 * 1024)); // 64 KB default to match current winhttp_connection bytesAvailable

    pfFile->file.read(destination, constrainedBytesToRead);
    *bytesWritten = pfFile->file.gcount();

    RETURN_HR_IF(E_FAIL, pfFile->file.bad());

    return S_OK;
}

HRESULT FilePAL::WriteFileBytes(FileHandle& fileHandle, const char* source, size_t bytesAvailable) noexcept
{
    assert(fileHandle);
    assert(source);
    assert(bytesAvailable > 0);

    FileStreamContainer* pfFile = static_cast<FileStreamContainer*>(fileHandle.get());

    pfFile->file.write(source, bytesAvailable);
    pfFile->file.flush();
    RETURN_HR_IF(E_FAIL, pfFile->file.bad());

    return S_OK;
}

void FilePAL::CloseFile(FileHandle& fileHandle) noexcept
{
    FileStreamContainer* pfFile = static_cast<FileStreamContainer*>(fileHandle.get());
    pfFile->file.close();
}

bool FilePAL::DoesDirectoryExist(const String& directoryPath) noexcept
{
    try
    {
        auto path = ConvertStringToPath(directoryPath);
        return std::filesystem::is_directory(path.c_str());
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return false;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool FilePAL::DoesFileExist(const String& filePath) noexcept
{
    auto path = ConvertStringToPath(filePath);
    std::ifstream file(path);
    return file.good();
}

HRESULT FilePAL::DeleteLocalFile(const String& filePath) noexcept
{
    TRACE_INFORMATION("[FILEPAL] DeleteLocalFile %s", filePath.c_str());
    RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(filePath));

    try
    {
        auto path = ConvertStringToPath(filePath);
        bool result = std::filesystem::remove(path.c_str());
        TRACE_INFORMATION("[FILEPAL] std::filesystem::remove %d", result);
        bool exists = DoesFileExist(filePath);
        TRACE_INFORMATION("[FILEPAL] DoesFileExist %d", exists);
        return result ? S_OK : E_FAIL;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

HRESULT FilePAL::CreatePath(const String& directoryPath) noexcept
{
    RETURN_HR_IF_FALSE(E_INVALIDARG, IsValidPath(directoryPath));

    RETURN_HR_IF(S_OK, DoesDirectoryExist(directoryPath));

    try
    {
        std::error_code error;
        auto path = ConvertStringToPath(directoryPath);
        std::filesystem::create_directories(path, error);
        RETURN_HR_IF(E_FAIL, error);

        return S_OK;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

HRESULT FilePAL::DeletePath(const String& directoryPath) noexcept
{
    TRACE_INFORMATION("[FILEPAL] DeletePath %s", directoryPath.c_str());
    RETURN_HR_IF_FALSE(E_INVALIDARG, IsValidPath(directoryPath));
    RETURN_HR_IF_FALSE(S_OK, DoesDirectoryExist(directoryPath));

    try
    {
        std::error_code ec;
        auto path = ConvertStringToPath(directoryPath);
        std::filesystem::remove_all(path, ec);
        if (!ec)
        {
            if (DoesDirectoryExist(directoryPath))
            {
                TRACE_INFORMATION("[FILEPAL] std::filesystem::remove_all failure");
                return E_FAIL;
            }
        }

        TRACE_INFORMATION("[FILEPAL] std::filesystem::remove_all success");
        return S_OK;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

Result<Vector<String>> FilePAL::EnumDirectories(const String& directoryPath) noexcept
{
    if (!DoesDirectoryExist(directoryPath))
    {
        RETURN_IF_FAILED(CreatePath(directoryPath));
    }

    try
    {
        Vector<String> folders;
        auto dirPath = ConvertStringToPath(directoryPath);
        for (const auto& entry : std::filesystem::directory_iterator(dirPath.c_str()))
        {
            if (std::filesystem::is_directory(entry.path()))
            {
                auto folderNameUtf = entry.path().filename().u8string();
                String folder(folderNameUtf.begin(), folderNameUtf.end());
                folders.push_back(std::move(folder));
            }
        }

        return folders;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

Result<Vector<String>> FilePAL::EnumFiles(const String& directoryPath) noexcept
{
    if (!DoesDirectoryExist(directoryPath))
    {
        RETURN_IF_FAILED(CreatePath(directoryPath));
    }

    try
    {
        Vector<String> fileNames;
        auto dirPath = ConvertStringToPath(directoryPath);
        for (const auto& entry : std::filesystem::directory_iterator(dirPath.c_str()))
        {
            if (std::filesystem::is_regular_file(entry.path()))
            {
                auto fileNameUtf = entry.path().filename().u8string();
                String fileName(fileNameUtf.begin(), fileNameUtf.end());
                fileNames.push_back(std::move(fileName));
            }
        }

        return fileNames;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

Result<uint64_t> FilePAL::GetAvailableStorageSize(const String& filePath) noexcept
{
    RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(filePath) || DoesDirectoryExist(filePath));

    try
    {
        auto dirPath = ConvertStringToPath(filePath);
        uint64_t space = static_cast<uint64_t>(std::filesystem::space(dirPath).available);
        TRACE_INFORMATION("[FILEPAL] GetAvailableStorageSize %llu", space);
        return space;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

Result<uint64_t> FilePAL::GetFileSize(const String& filePath) noexcept
{
    RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(filePath));

    try
    {
        auto path = ConvertStringToPath(filePath);
        uint64_t fileSize = static_cast<uint64_t>( std::filesystem::file_size(path));
        return fileSize;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

HRESULT FilePAL::GetFileTimes(const String& filePath, time_t& timeCreated, time_t& timeModified) noexcept
{
    if (!DoesFileExist(filePath))
    {
        timeCreated = 0;
        timeModified = 0;
        return E_FAIL;
    }

#if HC_PLATFORM_IS_MICROSOFT
    auto path = ConvertStringToPath(filePath);
    struct _stat64 fileInfo64;
    if (_wstat64(path.c_str(), &fileInfo64) != 0)
    {
        timeCreated = 0;
        timeModified = 0;
        return E_FAIL;
    }

    timeCreated = fileInfo64.st_ctime;
    timeModified = fileInfo64.st_mtime;
    return S_OK;
#else
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0)
    {
        timeCreated = 0;
        timeModified = 0;
        return E_FAIL;
    }

    timeCreated = fileInfo.st_ctime;
    timeModified = fileInfo.st_mtime;
    return S_OK;
#endif
}

#if !HC_PLATFORM_IS_MICROSOFT
HRESULT FilePAL::SetFileLastModifiedTime(const String& filePath, time_t timeCreated, time_t timeModified) noexcept
{
    struct utimbuf newTimes;
    newTimes.actime = timeModified;
    newTimes.modtime = timeModified;
    if (utime(filePath.c_str(), &newTimes) != 0)
    {
        return E_FAIL;
    }

    return S_OK;
}
#endif

HRESULT FilePAL::MoveLocalFile(const String& srcPath, const String& destPath) noexcept
{
    RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(srcPath) && IsValidPath(destPath));

    try
    {
        auto srcPath2 = ConvertStringToPath(srcPath);
        auto destPath2 = ConvertStringToPath(destPath);
        std::filesystem::rename(srcPath2, destPath2);
        return S_OK;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return E_FAIL;
    }
    catch (const std::exception&)
    {
        return E_FAIL;
    }
}

} // namespace FileIO

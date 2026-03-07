#pragma once

#include "Result.h"

namespace PlayFab
{

#define FILEPAL_MAX_BYTES_AVAILABLE 64 * 1024

enum class FileOpenMode
{
    Read,
    Write
};

class FileContainer
{
};

typedef UniquePtr<FileContainer> FileHandle;

class FilePAL
{
public:
    static Result<FileHandle> OpenFile(const String& filePath, FileOpenMode openMode) noexcept;
    static HRESULT ReadFileBytes(FileHandle& fileHandle, size_t bytesAvailable, char* destination, size_t* bytesWritten) noexcept;
    static HRESULT WriteFileBytes(FileHandle& fileHandle, const char* source, size_t bytesAvailable) noexcept;
    static void CloseFile(FileHandle& fileHandle) noexcept;

    static bool DoesFileExist(const String& filePath) noexcept;
    static bool DoesDirectoryExist(const String& directoryPath) noexcept;

    static Result<Vector<String>> EnumFiles(const String& directoryPath) noexcept;
    static Result<Vector<String>> EnumDirectories(const String& directoryPath) noexcept;

    static HRESULT CreatePath(const String& directoryPath) noexcept;
    static HRESULT DeletePath(const String& directoryPath) noexcept;
    static Result<String> JoinPath(const String& pathHead, const String& pathTail) noexcept;
    static Result<String> GetParentPath(const String& path) noexcept;
    static char GetPathSeparatorChar() noexcept;

    static Result<uint64_t> GetAvailableStorageSize(const String& path) noexcept;
    static Result<uint64_t> GetFileSize(const String& filePath) noexcept;
    static HRESULT GetFileTimes(const String& filePath, time_t& fileCreation, time_t& fileModified) noexcept;
    static HRESULT SetFileLastModifiedTime(const String& filePath, time_t newCreationTime, time_t newModifiedTime) noexcept;

    static HRESULT DeleteLocalFile(const String& filePath) noexcept;
    static HRESULT MoveLocalFile(const String& srcPath, const String& destPath) noexcept;

private:
    static bool IsValidPath(const String& filePath) noexcept;
    static const String GetPathInvalidChars() noexcept;
};

} // namespace FileIO

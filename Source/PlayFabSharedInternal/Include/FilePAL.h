#pragma once

#include "Result.h"

// Native PAL builds without disk-space simulation set this to 0 for the GameSave API sources.
#ifndef PF_FILEPAL_HAS_DISK_SPACE_SIMULATION
#define PF_FILEPAL_HAS_DISK_SPACE_SIMULATION 1
#endif

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

    // Platform overlays implement their own FilePAL instead of FilePAL.cpp. FilePAL_Shared.cpp
    // supplies compatibility members for storage without links. An overlay defining
    // PF_FILEPAL_HAS_NATIVE_EMPTY_DIRECTORY_PRUNE=1 supplies both DeletePathIfEmpty overloads;
    // its containingRoot overload must validate the target and intervening components before
    // native directory-only pruning. DoesEntryExist and EnumDirectories fallbacks remain shared.

    // True when anything is at path (a file, a directory, or a link, even a dangling one). A link
    // isn't followed.
    static bool DoesEntryExist(const String& path) noexcept;

    // Neither enumerator follows a link (a symbolic link, or an NTFS junction): link entries are
    // left out. The second EnumDirectories also reports the names of the links it left out
    // (whatever they point at) in skippedLinks when it isn't null.
    static Result<Vector<String>> EnumFiles(const String& directoryPath) noexcept;
    static Result<Vector<String>> EnumDirectories(const String& directoryPath) noexcept;
    static Result<Vector<String>> EnumDirectories(const String& directoryPath, Vector<String>* skippedLinks) noexcept;

    static HRESULT CreatePath(const String& directoryPath) noexcept;
    static HRESULT DeletePath(const String& directoryPath) noexcept;
    // Deletes directoryPath only if no files survive beneath it. Empty subdirectories are
    // removed bottom-up; any directory that still holds a file is kept, along with its
    // ancestors. Sets fullyDeleted to true when the entire tree was removed. A link is never
    // followed: directoryPath itself being a link fails with E_INVALIDARG, and with the
    // containingRoot overload so does any component between containingRoot (when not empty;
    // directoryPath must then be beneath it) and directoryPath, so a link inside a save tree can't
    // lead the prune outside it.
    static HRESULT DeletePathIfEmpty(const String& directoryPath, bool& fullyDeleted) noexcept;
    static HRESULT DeletePathIfEmpty(const String& directoryPath, bool& fullyDeleted, const String& containingRoot) noexcept;
    static Result<String> JoinPath(const String& pathHead, const String& pathTail) noexcept;
    static Result<String> GetParentPath(const String& path) noexcept;
    static char GetPathSeparatorChar() noexcept;

    static Result<uint64_t> GetAvailableStorageSize(const String& path) noexcept;
    static Result<uint64_t> GetFileSize(const String& filePath) noexcept;
    static HRESULT GetFileTimes(const String& filePath, time_t& fileCreation, time_t& fileModified) noexcept;
    static HRESULT SetFileLastModifiedTime(const String& filePath, time_t newCreationTime, time_t newModifiedTime) noexcept;

    static HRESULT DeleteLocalFile(const String& filePath) noexcept;
    static HRESULT MoveLocalFile(const String& srcPath, const String& destPath) noexcept;

    // Test-only fault injection simulating another process exhausting the volume mid-operation.
    // Once bytesBeforeFull more bytes have been written through WriteFileBytes, writes fail the way
    // the OS fails them on a full disk (errno ENOSPC / ERROR_DISK_FULL), and from the first refused
    // write GetAvailableStorageSize reports 0. Until then, both behave normally, so a budget of 0
    // fails the first write after a passing space check. enable=false turns the simulation off.
    static void SetSimulatedDiskFullForDebug(bool enable, uint64_t bytesBeforeFull) noexcept;
    // Test-only: the volume is already nearly full. GetAvailableStorageSize reports freeBytes (less
    // whatever has since been written) and writes beyond it fail as on a full disk.
    static void SetSimulatedDiskFreeSpaceForDebug(bool enable, uint64_t freeBytes) noexcept;

private:
    static bool IsValidPath(const String& filePath) noexcept;
    static const String GetPathInvalidChars() noexcept;
};

} // namespace FileIO

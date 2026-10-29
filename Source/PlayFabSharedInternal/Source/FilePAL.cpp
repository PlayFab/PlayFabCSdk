#include "pch.h"
#include "FilePAL_Generic.h"
#include <filesystem>
#include <algorithm>
#include <cerrno>
#include <system_error>
#include <atomic>
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

namespace
{

// Win32 system error codes used by the mappings below. Spelled out numerically rather than taken
// from platform headers so the same HRESULTs are produced on platforms whose PAL does not define
// the ERROR_* constants.
//
// Note the mappings use __HRESULT_FROM_WIN32 (two leading underscores), NOT HRESULT_FROM_WIN32.
// This file is compiled for Linux, Android, iOS and macOS as well as Windows, and only the
// double-underscore form is portable: libHttpClient's pal.h defines it for non-Windows platforms,
// while the single-underscore form exists solely in the Windows SDK's winerror.h. The rest of the
// SDK already follows this convention (see Generated/Error.cpp).
constexpr uint32_t kWin32FileNotFound = 2;        // ERROR_FILE_NOT_FOUND
constexpr uint32_t kWin32PathNotFound = 3;        // ERROR_PATH_NOT_FOUND
constexpr uint32_t kWin32TooManyOpenFiles = 4;    // ERROR_TOO_MANY_OPEN_FILES
constexpr uint32_t kWin32AccessDenied = 5;        // ERROR_ACCESS_DENIED
constexpr uint32_t kWin32WriteProtect = 19;       // ERROR_WRITE_PROTECT
constexpr uint32_t kWin32SharingViolation = 32;   // ERROR_SHARING_VIOLATION
constexpr uint32_t kWin32LockViolation = 33;      // ERROR_LOCK_VIOLATION
constexpr uint32_t kWin32HandleDiskFull = 39;     // ERROR_HANDLE_DISK_FULL
constexpr uint32_t kWin32DiskFull = 112;          // ERROR_DISK_FULL
constexpr uint32_t kWin32InvalidName = 123;       // ERROR_INVALID_NAME
constexpr uint32_t kWin32FilenameExcedRange = 206;// ERROR_FILENAME_EXCED_RANGE

// A local file operation failing is never an argument error. Reporting E_INVALIDARG for it is
// actively harmful: the caller cannot tell "the path you passed is malformed" (permanent, the
// caller's fault) from "the file was locked or gone for an instant" (transient, retryable), and
// GameSave surfaces these codes verbatim out of the public upload API. Map to the underlying
// platform error so both the SDK and the title can classify the failure.
//
// The converse still holds: an argument that cannot name a file at all IS an argument error, and
// must keep reporting E_INVALIDARG rather than falling through to E_FAIL.
HRESULT HResultFromErrno(int err) noexcept
{
    switch (err)
    {
    case ENOENT:        return __HRESULT_FROM_WIN32(kWin32FileNotFound);
    case ENOTDIR:       return __HRESULT_FROM_WIN32(kWin32PathNotFound);
    case EACCES:        return __HRESULT_FROM_WIN32(kWin32AccessDenied);
    case EPERM:         return __HRESULT_FROM_WIN32(kWin32AccessDenied);
    case EISDIR:        return __HRESULT_FROM_WIN32(kWin32AccessDenied);
    case EBUSY:         return __HRESULT_FROM_WIN32(kWin32SharingViolation);
    case EMFILE:        return __HRESULT_FROM_WIN32(kWin32TooManyOpenFiles);
    case ENFILE:        return __HRESULT_FROM_WIN32(kWin32TooManyOpenFiles);
    case ENOSPC:        return __HRESULT_FROM_WIN32(kWin32DiskFull);
    case EROFS:         return __HRESULT_FROM_WIN32(kWin32WriteProtect);
    case ENAMETOOLONG:  return __HRESULT_FROM_WIN32(kWin32FilenameExcedRange);
    // An empty path reaches the CRT as EINVAL with no Win32 last error set. Without this it fell
    // through to E_FAIL, which is the one case where OpenFile silently stopped honoring the
    // "malformed input stays E_INVALIDARG" half of this contract.
    case EINVAL:        return E_INVALIDARG;
    default:            return E_FAIL;
    }
}

// std::filesystem reports through std::error_code. On Microsoft platforms the values come from
// system_category() and are already Win32 codes; elsewhere they are errno values.
HRESULT HResultFromFileErrorCode(const std::error_code& ec) noexcept
{
    if (!ec)
    {
        return E_FAIL;
    }

#if HC_PLATFORM_IS_MICROSOFT
    if (ec.category() == std::system_category())
    {
        return __HRESULT_FROM_WIN32(static_cast<uint32_t>(ec.value()));
    }
#endif

    return HResultFromErrno(ec.value());
}

// Maps a just-failed std::fstream open or write. The CRT reports the coarse reason through errno,
// which cannot distinguish a sharing violation from a permission denial - both arrive as EACCES. On
// Microsoft platforms the underlying Win32 call also sets the last error, which does make that
// distinction, so prefer it when it names a failure we recognize. Callers clear the last error
// immediately before the operation so this cannot pick up a stale value.
HRESULT HResultFromStreamFailure(int err) noexcept
{
#if HC_PLATFORM_IS_MICROSOFT
    const uint32_t lastError = static_cast<uint32_t>(GetLastError());

    // A name the filesystem cannot represent is a caller error, not an I/O condition, so it keeps
    // reporting E_INVALIDARG. In practice the CRT usually reports such a name as EINVAL and it is
    // classified by HResultFromErrno below; this case covers the paths where the OS reports
    // ERROR_INVALID_NAME directly instead.
    if (lastError == kWin32InvalidName)
    {
        return E_INVALIDARG;
    }

    switch (lastError)
    {
    case kWin32FileNotFound:
    case kWin32PathNotFound:
    case kWin32AccessDenied:
    case kWin32SharingViolation:
    case kWin32LockViolation:
    case kWin32TooManyOpenFiles:
    case kWin32HandleDiskFull:
    case kWin32DiskFull:
    case kWin32WriteProtect:
    case kWin32FilenameExcedRange:
        return __HRESULT_FROM_WIN32(lastError);
    default:
        break;
    }
#endif

    return HResultFromErrno(err);
}

// Test-only disk-full simulation state; see FilePAL::SetSimulatedDiskFullForDebug.
std::atomic<bool> g_simulatedDiskFullEnabled{ false };
std::atomic<uint64_t> g_simulatedDiskBytesRemaining{ 0 };
std::atomic<bool> g_simulatedDiskFullHit{ false };
// Set by SetSimulatedDiskFreeSpaceForDebug: the volume is already nearly full, so free space reads as
// the remaining budget from the start, not only once a write has been refused.
std::atomic<bool> g_simulatedDiskReportsBudget{ false };

// Free space reads as 0 only once a write has actually been refused. That models another process
// filling the volume after the SDK's pre-flight space check, which is the reported scenario, and
// lets a budget of 0 fail the very first byte of a transfer instead of tripping the pre-flight.
bool IsSimulatedDiskFull() noexcept
{
    return g_simulatedDiskFullEnabled.load() && g_simulatedDiskFullHit.load();
}

// Consumes up to bytesRequested of the simulated budget and returns how many bytes may actually be
// written. A return smaller than bytesRequested means the write must fail as a full disk would,
// after writing the returned prefix -- a real full volume accepts a partial write before failing.
size_t ConsumeSimulatedDiskBytes(size_t bytesRequested) noexcept
{
    if (!g_simulatedDiskFullEnabled.load())
    {
        return bytesRequested;
    }

    uint64_t remaining = g_simulatedDiskBytesRemaining.load();
    uint64_t granted{};
    do
    {
        granted = std::min<uint64_t>(remaining, bytesRequested);
    } while (!g_simulatedDiskBytesRemaining.compare_exchange_weak(remaining, remaining - granted));

    return static_cast<size_t>(granted);
}

} // anonymous namespace

void FilePAL::SetSimulatedDiskFullForDebug(bool enable, uint64_t bytesBeforeFull) noexcept
{
    TRACE_INFORMATION("[FILEPAL] SetSimulatedDiskFullForDebug enable=%d bytesBeforeFull=%llu", enable ? 1 : 0, bytesBeforeFull);
    g_simulatedDiskBytesRemaining.store(bytesBeforeFull);
    g_simulatedDiskFullHit.store(false);
    g_simulatedDiskReportsBudget.store(false);
    g_simulatedDiskFullEnabled.store(enable);
}

void FilePAL::SetSimulatedDiskFreeSpaceForDebug(bool enable, uint64_t freeBytes) noexcept
{
    TRACE_INFORMATION("[FILEPAL] SetSimulatedDiskFreeSpaceForDebug enable=%d freeBytes=%llu", enable ? 1 : 0, freeBytes);
    g_simulatedDiskBytesRemaining.store(freeBytes);
    g_simulatedDiskFullHit.store(false);
    g_simulatedDiskReportsBudget.store(enable);
    g_simulatedDiskFullEnabled.store(enable);
}

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

    errno = 0;
#if HC_PLATFORM_IS_MICROSOFT
    SetLastError(ERROR_SUCCESS);
#endif
    fileHandle->file.open(path.c_str(), mode);
    if (!fileHandle->file)
    {
        const int openError = errno;
        HRESULT hr = HResultFromStreamFailure(openError);
        TRACE_ERROR(
            "[FILEPAL] OpenFile failed path='%s' mode=%u errno=%d streamState=0x%X hr=0x%08X",
            filePath.c_str(),
            static_cast<unsigned>(openMode),
            openError,
            static_cast<unsigned>(fileHandle->file.rdstate()),
            static_cast<unsigned>(hr));
        return Result<FileHandle>{ hr };
    }

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
    RETURN_HR_IF(E_INVALIDARG, !fileHandle || (!source && bytesAvailable > 0));
    RETURN_HR_IF(S_OK, bytesAvailable == 0);

    assert(fileHandle);
    assert(source);
    assert(bytesAvailable > 0);

    FileStreamContainer* pfFile = static_cast<FileStreamContainer*>(fileHandle.get());

    errno = 0;
#if HC_PLATFORM_IS_MICROSOFT
    SetLastError(ERROR_SUCCESS);
#endif
    // A stream that has already failed does no I/O at all, so a real full disk would not report
    // again here; only simulate the failure for a healthy stream to keep the simulation faithful.
    const bool streamAlreadyBad = pfFile->file.bad();
    if (streamAlreadyBad && FAILED(pfFile->lastFailure))
    {
        TRACE_ERROR("[FILEPAL] WriteFileBytes on a stream that already failed hr=0x%08X", pfFile->lastFailure);
        return pfFile->lastFailure;
    }
    const size_t bytesAllowed = streamAlreadyBad ? bytesAvailable : ConsumeSimulatedDiskBytes(bytesAvailable);
    if (bytesAllowed > 0)
    {
        pfFile->file.write(source, bytesAllowed);
    }
    pfFile->file.flush();
    if (bytesAllowed < bytesAvailable)
    {
        // Simulated full volume: fail exactly as the CRT does when WriteFile reports ERROR_DISK_FULL.
        g_simulatedDiskFullHit.store(true);
        errno = ENOSPC;
#if HC_PLATFORM_IS_MICROSOFT
        SetLastError(kWin32DiskFull);
#endif
        pfFile->file.setstate(std::ios::badbit);
    }

    if (pfFile->file.bad())
    {
        // Report the real reason. A full disk in particular must stay recognizable all the way up:
        // GameSave turns it into the out-of-storage UI, which a generic E_FAIL can never reach.
        HRESULT hr = HResultFromStreamFailure(errno);
        pfFile->lastFailure = hr;
        TRACE_ERROR("[FILEPAL] WriteFileBytes failed hr=0x%08X", hr);
        return hr;
    }

    return S_OK;
}

void FilePAL::CloseFile(FileHandle& fileHandle) noexcept
{
    FileStreamContainer* pfFile = static_cast<FileStreamContainer*>(fileHandle.get());
    pfFile->file.close();
    fileHandle.reset();
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
    // Deliberately does NOT open the file. Opening conflates "not there" with "there but not
    // openable right now": a save file momentarily held by the game's own writer (or by AV, or by
    // a cloud-sync agent) fails to open with a sharing violation, and reporting that as "does not
    // exist" made callers such as GetFileSize reject a perfectly healthy file.
    try
    {
        auto path = ConvertStringToPath(filePath);
        std::error_code ec;
        const std::filesystem::file_status status = std::filesystem::status(path, ec);
        if (ec)
        {
            return false;
        }

        // Directories are not files. The previous ifstream-based implementation also rejected
        // them (open fails with EACCES), so keep that behavior.
        return std::filesystem::is_regular_file(status);
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

bool FilePAL::DoesEntryExist(const String& path) noexcept
{
    try
    {
        std::error_code ec;
        const std::filesystem::file_status status = std::filesystem::symlink_status(ConvertStringToPath(path), ec);
        // A missing path (or parent) reads as not_found; any other error can't prove the path is free.
        return status.type() != std::filesystem::file_type::not_found;
    }
    catch (const std::exception&)
    {
        return true;
    }
}

HRESULT FilePAL::DeleteLocalFile(const String& filePath) noexcept
{
    TRACE_INFORMATION("[FILEPAL] DeleteLocalFile %s", filePath.c_str());

    try
    {
        auto path = ConvertStringToPath(filePath);
        std::error_code ec;
        std::filesystem::remove(path.c_str(), ec);
        if (ec)
        {
            TRACE_INFORMATION("[FILEPAL] std::filesystem::remove failed: %s", ec.message().c_str());
            return E_FAIL;
        }
        TRACE_INFORMATION("[FILEPAL] std::filesystem::remove success");
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

    if (!DoesDirectoryExist(directoryPath))
    {
        TRACE_INFORMATION("[FILEPAL] DeletePath: directory does not exist, returning S_OK");
        return S_OK;
    }

    try
    {
        std::error_code ec;
        auto path = ConvertStringToPath(directoryPath);
        std::filesystem::remove_all(path, ec);
        if (ec)
        {
            TRACE_INFORMATION("[FILEPAL] std::filesystem::remove_all failed: %s", ec.message().c_str());
            return E_FAIL;
        }

        if (DoesDirectoryExist(directoryPath))
        {
            TRACE_INFORMATION("[FILEPAL] std::filesystem::remove_all failure: directory still exists");
            return E_FAIL;
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

namespace
{

// True for a file status that is a link: a symbolic link, or (Windows) an NTFS junction, which
// std::filesystem reports as its own file type rather than as a symlink. A save tree never follows
// a link: its target can be anywhere on the device.
bool IsLinkStatus(const std::filesystem::file_status& status) noexcept
{
    if (std::filesystem::is_symlink(status))
    {
        return true;
    }
#if defined(_MSC_VER)
    if (status.type() == std::filesystem::file_type::junction)
    {
        return true;
    }
#endif
    return false;
}

// See IsLinkStatus. An entry whose status can't be read isn't reported as a link.
bool IsLinkEntry(const std::filesystem::directory_entry& entry) noexcept
{
    std::error_code ec;
    const std::filesystem::file_status status = entry.symlink_status(ec);
    if (ec)
    {
        return false;
    }
    return IsLinkStatus(status);
}

// Removes dir only if neither it nor any descendant contains a file. Empty subdirectories
// are removed bottom-up. Returns true when dir itself was removed.
//
// Entries are collected before recursing so the directory_iterator is never invalidated by
// removals performed during traversal. Descendant links (symlinks, junctions) are treated as
// survivors and are never followed. Note this says nothing about `dir` itself: directory_iterator follows a
// symlinked root and would enumerate its target, so callers must reject a symlinked root
// before calling in (DeletePathIfEmpty does). Any enumeration or removal error is treated as
// "keep", because this runs on the game-save delete path where preserving unexpected content
// is always safer than destroying it.
bool RemoveDirectoryIfEmptyRecursive(const std::filesystem::path& dir)
{
    Vector<std::filesystem::path> subDirectories;
    bool survivorFound = false;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(dir, ec), end; it != end; it.increment(ec))
    {
        if (ec)
        {
            return false;
        }

        std::error_code entryEc;
        if (IsLinkEntry(*it) || it->is_symlink(entryEc) || entryEc)
        {
            survivorFound = true;
            continue;
        }

        if (it->is_directory(entryEc) && !entryEc)
        {
            subDirectories.push_back(it->path());
        }
        else
        {
            survivorFound = true;
        }
    }

    if (ec)
    {
        return false;
    }

    for (const std::filesystem::path& subDirectory : subDirectories)
    {
        if (!RemoveDirectoryIfEmptyRecursive(subDirectory))
        {
            survivorFound = true;
        }
    }

    if (survivorFound)
    {
        return false;
    }

    std::error_code removeEc;
    std::filesystem::remove(dir, removeEc);
    return !removeEc;
}

} // anonymous namespace

HRESULT FilePAL::DeletePathIfEmpty(const String& directoryPath, bool& fullyDeleted) noexcept
{
    return DeletePathIfEmpty(directoryPath, fullyDeleted, String{});
}

HRESULT FilePAL::DeletePathIfEmpty(const String& directoryPath, bool& fullyDeleted, const String& containingRoot) noexcept
{
    fullyDeleted = false;

    TRACE_INFORMATION("[FILEPAL] DeletePathIfEmpty %s", directoryPath.c_str());
    RETURN_HR_IF_FALSE(E_INVALIDARG, IsValidPath(directoryPath));

    if (!DoesDirectoryExist(directoryPath))
    {
        TRACE_INFORMATION("[FILEPAL] DeletePathIfEmpty: directory does not exist, returning S_OK");
        fullyDeleted = true;
        return S_OK;
    }

    try
    {
        auto path = ConvertStringToPath(directoryPath);

        // DoesDirectoryExist above follows symlinks, so directoryPath itself may be a link. Walking it
        // would enumerate the link's TARGET and prune empty directories outside the tree being cleaned.
        // RemoveDirectoryIfEmptyRecursive cannot prevent that - it only inspects entries it enumerates,
        // never the root it was handed. symlink_status does not follow the link, so check it here.
        std::error_code statusEc;
        const std::filesystem::file_status linkStatus = std::filesystem::symlink_status(path, statusEc);
        if (statusEc)
        {
            TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: symlink_status failed for %s: %s", directoryPath.c_str(), statusEc.message().c_str());
            return E_FAIL;
        }

        if (IsLinkStatus(linkStatus))
        {
            TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: refusing to prune %s because it is a link", directoryPath.c_str());
            return E_INVALIDARG;
        }

        if (!containingRoot.empty())
        {
            // A link in any component between the root and the target leads the prune into the
            // link's target as surely as a link at the target itself: refuse those too. The root
            // itself isn't checked (a platform may mount the save folder through one).
            String rootString = containingRoot;
            while (rootString.size() > 1 && (rootString.back() == '/' || rootString.back() == '\\'))
            {
                rootString.pop_back();
            }
            const std::filesystem::path root = ConvertStringToPath(rootString).lexically_normal();
            const std::filesystem::path relative = path.lexically_normal().lexically_relative(root);
            if (relative.empty() || *relative.begin() == std::filesystem::path(".."))
            {
                TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: refusing to prune %s because it isn't beneath %s", directoryPath.c_str(), containingRoot.c_str());
                return E_INVALIDARG;
            }
            std::filesystem::path current = root;
            for (const std::filesystem::path& component : relative)
            {
                if (component == std::filesystem::path("."))
                {
                    continue;
                }
                current /= component;
                std::error_code componentEc;
                const std::filesystem::file_status componentStatus = std::filesystem::symlink_status(current, componentEc);
                if (componentEc)
                {
                    TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: symlink_status failed for a component of %s: %s", directoryPath.c_str(), componentEc.message().c_str());
                    return E_FAIL;
                }
                if (IsLinkStatus(componentStatus))
                {
                    TRACE_WARNING("[FILEPAL] DeletePathIfEmpty: refusing to prune %s because a folder on its path is a link", directoryPath.c_str());
                    return E_INVALIDARG;
                }
            }
        }

        fullyDeleted = RemoveDirectoryIfEmptyRecursive(path);
        TRACE_INFORMATION("[FILEPAL] DeletePathIfEmpty: fullyDeleted:%d", fullyDeleted);
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
    return EnumDirectories(directoryPath, nullptr);
}

Result<Vector<String>> FilePAL::EnumDirectories(const String& directoryPath, Vector<String>* skippedLinks) noexcept
{
    if (skippedLinks != nullptr)
    {
        skippedLinks->clear();
    }
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
            // Links (symlinks, junctions) aren't part of the tree: following one would read, and
            // on a later delete prune, content outside the folder being enumerated.
            if (IsLinkEntry(entry))
            {
                if (skippedLinks != nullptr)
                {
                    auto linkNameUtf = entry.path().filename().u8string();
                    skippedLinks->push_back(String(linkNameUtf.begin(), linkNameUtf.end()));
                }
                continue;
            }
            // The member form uses the attributes the iterator already fetched (no extra stat
            // per entry on Windows); it still follows symlinks and throws on error like the free
            // function did.
            if (entry.is_directory())
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
            // See EnumDirectories: links aren't followed.
            if (IsLinkEntry(entry))
            {
                continue;
            }
            // See EnumDirectories: uses the iterator's cached attributes instead of a stat per file.
            if (entry.is_regular_file())
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
    try
    {
        auto dirPath = ConvertStringToPath(filePath);
        std::error_code ec;
        const std::uintmax_t available = std::filesystem::space(dirPath, ec).available;
        if (ec)
        {
            HRESULT hr = HResultFromFileErrorCode(ec);
            TRACE_ERROR("[FILEPAL] GetAvailableStorageSize failed %s hr=0x%08X", filePath.c_str(), hr);
            return Result<uint64_t>{ hr };
        }

        uint64_t space = static_cast<uint64_t>(available);
        if (IsSimulatedDiskFull())
        {
            TRACE_WARNING("[FILEPAL] GetAvailableStorageSize: simulated disk full, reporting 0 (actual %llu)", space);
            space = 0;
        }
        else if (g_simulatedDiskFullEnabled.load() && g_simulatedDiskReportsBudget.load())
        {
            const uint64_t budget = g_simulatedDiskBytesRemaining.load();
            TRACE_WARNING("[FILEPAL] GetAvailableStorageSize: simulated nearly-full disk, reporting %llu (actual %llu)", std::min(space, budget), space);
            space = std::min(space, budget);
        }
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
    try
    {
        auto path = ConvertStringToPath(filePath);
        std::error_code ec;
        const std::uintmax_t fileSize = std::filesystem::file_size(path, ec);
        if (ec)
        {
            // Reports the real reason (missing, bad path, denied) rather than E_INVALIDARG. Note
            // that a file merely held open by another writer still succeeds here - its metadata is
            // readable - so a transient lock now fails later at OpenFile with a sharing violation
            // instead of being misreported as a bad argument.
            HRESULT hr = HResultFromFileErrorCode(ec);
            TRACE_ERROR("[FILEPAL] GetFileSize failed %s hr=0x%08X", filePath.c_str(), hr);
            return Result<uint64_t>{ hr };
        }

        uint64_t fileSizeBytes = static_cast<uint64_t>(fileSize);
        return fileSizeBytes;
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
    // One stat instead of a DoesFileExist pre-check plus a stat. A missing path fails the stat;
    // a directory is rejected by the S_IFREG check, as DoesFileExist did before.
#if HC_PLATFORM_IS_MICROSOFT
    auto path = ConvertStringToPath(filePath);
    struct _stat64 fileInfo64;
    if (_wstat64(path.c_str(), &fileInfo64) != 0 || (fileInfo64.st_mode & _S_IFREG) == 0)
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
    if (stat(filePath.c_str(), &fileInfo) != 0 || !S_ISREG(fileInfo.st_mode))
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
    // A malformed destination is a real argument error; a missing source is an I/O condition.
    RETURN_HR_IF_FALSE(E_INVALIDARG, IsValidPath(destPath));

    try
    {
        auto srcPath2 = ConvertStringToPath(srcPath);
        auto destPath2 = ConvertStringToPath(destPath);
        std::error_code ec;
        std::filesystem::rename(srcPath2, destPath2, ec);
        if (ec)
        {
            HRESULT hr = HResultFromFileErrorCode(ec);
            TRACE_ERROR("[FILEPAL] MoveLocalFile failed %s -> %s hr=0x%08X", srcPath.c_str(), destPath.c_str(), hr);
            return hr;
        }
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

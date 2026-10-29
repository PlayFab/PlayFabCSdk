#pragma once

#include "Result.h"
#include "FilePAL.h"
#include <atomic>

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

namespace PlayFab
{

enum class ArchiveOpenMode
{
    Decompress,
    Compress
};

enum class ArchiveSource
{
    File,
    Memory
};

// An extracted entry is written to an SDK-owned temporary file, "~pfgs" + 8 hex digits + ".tmp"
// (IsArchiveExtractTempFileName), in the extraction root (the folder the selection's relative paths
// are relative to, so the temporary path is no longer than the entries' own) or, when that can't be
// derived, next to the destination. Its name is random and never already taken (a file, folder or
// link found there is left alone and another name is drawn), and it moves over the destination in
// one rename only once the entry is proven complete (size and the archive's own checks), so a
// damaged or interrupted extraction never touches the file it would replace. A save-tree scan skips
// an unrecorded file with this exact name pattern, and AddUser deletes one left in the save root by
// a crash.
constexpr const char* kArchiveExtractTempPrefix = "~pfgs";
constexpr const char* kArchiveExtractTempExtension = ".tmp";
constexpr size_t kArchiveExtractTempHexDigits = 8;

inline bool IsArchiveExtractTempFileName(const String& fileName) noexcept
{
    const size_t prefixLength = 5; // "~pfgs"
    const size_t extensionLength = 4; // ".tmp"
    if (fileName.size() != prefixLength + kArchiveExtractTempHexDigits + extensionLength ||
        fileName.compare(0, prefixLength, kArchiveExtractTempPrefix) != 0 ||
        fileName.compare(prefixLength + kArchiveExtractTempHexDigits, extensionLength, kArchiveExtractTempExtension) != 0)
    {
        return false;
    }
    for (size_t i = prefixLength; i < prefixLength + kArchiveExtractTempHexDigits; i++)
    {
        const char c = fileName[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')))
        {
            return false;
        }
    }
    return true;
}

class ArchiveFileDetail
{
public:
    String fullPath{};
    uint64_t uncompressedSize{};
    time_t timeLastModified{};
    time_t timeCreated{};
};

class LibArchiveContext
{
public:
    bool dataAvailable{};
    ConditionVariable dataAvailableCondition{};
    ConditionVariable dataRequestedCondition{};
    Mutex dataMutex{};
    
    bool reachedZipEof{};
    size_t dataBufferLength{};
    Vector<char> dataBuffer{};
    FileHandle zipFileHandle{};
    HRESULT callbackResult{ S_OK };
};

class ArchiveContext
{
public:
    ~ArchiveContext();
    HRESULT Initialize(ArchiveOpenMode mode, ArchiveSource source = ArchiveSource::Memory, String zipFilePath = "") noexcept;
    HRESULT AddFile(String& relativePath, ArchiveFileDetail&& file) noexcept;
    /// <summary>
    /// Drops every file previously added and resets the running uncompressed-size total. Callers
    /// that rebuild their file selection (for example after conflict resolution re-runs the
    /// comparison) must call this first, otherwise re-adding the same files double-counts their
    /// size against the archive size limits.
    /// </summary>
    void ClearFiles() noexcept;
    UnorderedMap<String, ArchiveFileDetail>& GetFiles() noexcept;
    uint64_t GetTotalUncompressedSize() const noexcept;
    uint64_t GetTotalCompressedSize() const noexcept;
    uint64_t GetUncompressedBytesProcessed() const noexcept;
    void AddCompressedBytesSize(uint64_t size) noexcept;
    HRESULT CompressBytes() noexcept;
    HRESULT CompressBytesGetData(size_t bytesAvailable, char* destination, size_t* bytesWritten, size_t* uncompressedBytesWritten, bool* finishedCompressing) noexcept;
    HRESULT CompressBytesGetAnyData(String& destination, size_t* uncompressedBytesWritten, bool* finishedCompressing) noexcept;
    HRESULT DecompressBytes() noexcept;
    /// <summary>
    /// Asynchronously provide data to the archive's decompression operation. When the archive requires new data, it will block until this function is called and provides the data.
    /// </summary>
    /// <param name="source">Buffer holding the data to decompress</param>
    /// <param name="bytesAvailable">Number of bytes in the source buffer</param>
    /// <param name="uncompressedBytesWritten">Returns current number of uncompressed bytes written from the archive</param>
    /// <returns>Latest archive operation result</returns>
    HRESULT DecompressBytesProvideData(const char* source, size_t bytesAvailable, size_t* uncompressedBytesWritten) noexcept;
    bool IsArchiveOperationDone() noexcept;
    void Close() noexcept;

    LibArchiveContext libArchiveContext{};

private:
    HRESULT ConvertLibarchiveResult(int ar, const char* message) noexcept;
    HRESULT CheckNotifyResult(HRESULT hr, const char* message = nullptr) noexcept;
    // Decompress mode: closes the entry being extracted and deletes its temporary file (the
    // destination is untouched).
    void AbandonExtractedEntry() noexcept;

    struct pfarchive* m_archive{};
    struct pfarchive_entry* m_archiveEntry{};
    HRESULT m_lastArchiveResult{};

    ArchiveOpenMode m_mode{};
    ArchiveSource m_source{};
    std::atomic<uint64_t> m_uncompressedBytesWritten{};
    Vector<char> m_remainingBuffer{};
    uint64_t m_totalUncompressedSize{};
    std::atomic<uint64_t> m_totalCompressedSize{};
    UnorderedMap<String, ArchiveFileDetail> m_fileMap{};
    int32_t m_nextFileIndex{};
    // Cursor for m_nextFileIndex. std::next(begin, index) on an unordered map is O(index), which
    // made compressing N files O(N^2). Valid because m_fileMap is not modified while compressing.
    UnorderedMap<String, ArchiveFileDetail>::iterator m_nextFileIt{};
    // One read/write chunk buffer reused across calls instead of allocating per 64 KB chunk.
    Vector<char> m_ioBuffer{};
    // Bytes of the current entry already read into m_ioBuffer (the sample used to choose the
    // entry's compression method) and not yet written to the archive.
    size_t m_pendingEntryBytes{};
    // Parent directory of the last extracted entry; consecutive entries usually share it, so
    // CreatePath (a directory stat) only runs when it changes.
    String m_lastCreatedDirectory{};
    FileHandle m_entryFile{};
    String m_entryFilePath;
    String m_entryTempPath; // decompress mode: where the current entry is written until it is proven (empty: in place)
    time_t m_entryFileTimeLastModified{};
    time_t m_entryFileTimeCreated{};
    bool m_skipEntry{};
    std::atomic<uint64_t> m_fileRemainingSize{};
    bool m_open{};
};

} // namespace FileIO

#endif

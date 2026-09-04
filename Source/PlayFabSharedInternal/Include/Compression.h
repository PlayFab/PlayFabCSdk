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
    std::condition_variable dataAvailableCondition{};
    std::condition_variable dataRequestedCondition{};
    std::mutex dataMutex{};
    
    bool reachedZipEof{};
    size_t dataBufferLength{};
    Vector<char> dataBuffer{};
    FileHandle zipFileHandle{};
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
    FileHandle m_entryFile{};
    String m_entryFilePath;
    time_t m_entryFileTimeLastModified{};
    time_t m_entryFileTimeCreated{};
    bool m_skipEntry{};
    std::atomic<uint64_t> m_fileRemainingSize{};
    bool m_open{};
};

} // namespace FileIO

#endif

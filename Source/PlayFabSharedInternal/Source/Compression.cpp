#include "pch.h"

#include "Compression.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <random>

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

#include <sys/stat.h>
#include "archive.h"
#include "archive_entry.h"

namespace PlayFab
{

namespace
{

#if HC_PLATFORM_IS_MICROSOFT
// Paths at or past this many UTF-16 units can't be opened through the Win32 file APIs unless the
// process opted into long paths (or the path carries the \\?\ prefix).
constexpr size_t kLegacyMaxPathUnits = 260;

size_t Utf16Length(const String& utf8) noexcept
{
    size_t units = 0;
    for (const char c : utf8)
    {
        const unsigned char byte = static_cast<unsigned char>(c);
        if ((byte & 0xC0) != 0x80)
        {
            units += (byte >= 0xF0) ? 2 : 1; // a 4-byte sequence is a surrogate pair
        }
    }
    return units;
}

// True when the temporary path can't be opened but the destination itself can (no long-path
// support): the entry is then extracted in place, as before temporary files.
bool TempPathTooLong(const String& tempPath, const String& destination) noexcept
{
    if (destination.rfind("\\\\?\\", 0) == 0)
    {
        return false;
    }
    return Utf16Length(tempPath) >= kLegacyMaxPathUnits && Utf16Length(destination) < kLegacyMaxPathUnits;
}
#endif

uint32_t RandomTempNameBits() noexcept
{
    static std::atomic<uint32_t> s_counter{ 0 };
    uint32_t bits = s_counter.fetch_add(0x9E3779B9u) ^ static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    try
    {
        std::random_device device;
        bits ^= device();
    }
    catch (...)
    {
        // The counter and the clock still differ between calls; DoesEntryExist below rules out a reuse.
    }
    return bits;
}

// Chooses the SDK-owned temporary file an entry is extracted to (see kArchiveExtractTempPrefix):
// in the extraction root when destination ends with "/<key>" (either separator), else next to the
// destination. Never a path anything already occupies, a link included.
HRESULT ChooseExtractTempPath(const String& destination, const String& key, String& tempPath) noexcept
{
    tempPath.clear();
    String folder;
    if (!key.empty() && destination.size() > key.size())
    {
        const size_t split = destination.size() - key.size();
        bool matches = destination[split - 1] == '/' || destination[split - 1] == '\\';
        for (size_t i = 0; matches && i < key.size(); i++)
        {
            const char a = destination[split + i] == '\\' ? '/' : destination[split + i];
            const char b = key[i] == '\\' ? '/' : key[i];
            matches = a == b;
        }
        if (matches)
        {
            folder = destination.substr(0, split - 1);
        }
    }
    if (folder.empty())
    {
        Result<String> parent = FilePAL::GetParentPath(destination);
        RETURN_IF_FAILED(parent.hr);
        folder = parent.ExtractPayload();
    }

    for (int attempt = 0; attempt < 16; attempt++)
    {
        char name[32]{};
        snprintf(name, sizeof(name), "%s%08x%s", kArchiveExtractTempPrefix, RandomTempNameBits(), kArchiveExtractTempExtension);
        Result<String> candidate = FilePAL::JoinPath(folder, String{ name });
        RETURN_IF_FAILED(candidate.hr);
        if (!FilePAL::DoesEntryExist(candidate.Payload()))
        {
            tempPath = candidate.ExtractPayload();
            return S_OK;
        }
        TRACE_WARNING("[GAME SAVE] ArchiveContext: %s is taken; choosing another temporary name", candidate.Payload().c_str());
    }
    return __HRESULT_FROM_WIN32(80); // ERROR_FILE_EXISTS
}

} // anonymous namespace

// Zip bomb protection limits (must fit in non-Zip64 ZIP, which uses 32-bit size/offset fields)
static constexpr uint64_t kMaxIndividualFileSize = 3ULL * 1024 * 1024 * 1024;   // 3 GB (below Zip64 threshold)
static constexpr uint64_t kMaxTotalUncompressedSize = 3ULL * 1024 * 1024 * 1024; // 3 GB total (below Zip64 threshold)

// Below this an entry is always deflated: deflating it is cheap and a smaller sample is too noisy
// to judge.
static constexpr size_t kMinCompressionSampleBytes = 4 * 1024;

// True when a sample of an entry looks incompressible (already-compressed, encrypted or random
// data). Deflate runs at only ~40 MB/s on such data and saves nothing - the zip comes out slightly
// larger - so those entries are stored instead. Both tests must agree: near-maximal byte entropy,
// and almost no repeated 4-byte sequences (which deflate's LZ77 stage would still exploit in data
// whose byte distribution looks random, such as a repeated block of random bytes).
static bool LooksIncompressible(const char* data, size_t size) noexcept
{
    if (size < kMinCompressionSampleBytes)
    {
        return false;
    }

    uint32_t counts[256]{};
    for (size_t i = 0; i < size; ++i)
    {
        ++counts[static_cast<uint8_t>(data[i])];
    }
    double entropyBits = 0.0;
    for (uint32_t count : counts)
    {
        if (count != 0)
        {
            const double p = static_cast<double>(count) / static_cast<double>(size);
            entropyBits -= p * std::log2(p);
        }
    }
    // Random data scores ~7.96 on a 4 KB sample and ~8.0 on 64 KB; text and structured data score
    // well below 7.
    if (entropyBits < 7.8)
    {
        return false;
    }

    constexpr uint32_t kHashBits = 14;
    Vector<uint32_t> lastPositionPlusOne(1u << kHashBits, 0);
    size_t repeats = 0;
    for (size_t i = 0; i + 4 <= size; ++i)
    {
        uint32_t value;
        memcpy(&value, data + i, sizeof(value));
        const uint32_t slot = (value * 2654435761u) >> (32 - kHashBits);
        const uint32_t previous = lastPositionPlusOne[slot];
        if (previous != 0 && memcmp(data + previous - 1, data + i, sizeof(value)) == 0)
        {
            ++repeats;
        }
        lastPositionPlusOne[slot] = static_cast<uint32_t>(i + 1);
    }
    return repeats * 100 < size;
}

static int ArchiveEmptyCallback(archive*, void*)
{
    return ARCHIVE_OK;
}

static la_ssize_t ArchiveReadFromFileCallback(archive* archive, void* clientData, const void** buffer) noexcept
{
    UNREFERENCED_PARAMETER(archive);
    auto context = static_cast<ArchiveContext*>(clientData);

    size_t bytesRead{};
    auto hr = FilePAL::ReadFileBytes(context->libArchiveContext.zipFileHandle, FILEPAL_MAX_BYTES_AVAILABLE, context->libArchiveContext.dataBuffer.data(), &bytesRead);
    if (FAILED(hr))
    {
        *buffer = nullptr;
        return -1;
    }

    *buffer = context->libArchiveContext.dataBuffer.data();
    context->libArchiveContext.dataBufferLength = bytesRead;
    return bytesRead;
}

static la_ssize_t ArchiveReadCallback(archive* archive, void* clientData, const void** buffer) noexcept
{
    auto context = static_cast<ArchiveContext*>(clientData);

    // Signal provider to fill buffer
    context->libArchiveContext.dataRequestedCondition.notify_one();

    // Wait for data to be provided. Timeout in case of HTTP issues
    std::unique_lock<std::mutex> lock(context->libArchiveContext.dataMutex);
    auto conditionMet = context->libArchiveContext.dataAvailableCondition.wait_for(lock, std::chrono::seconds(5), [context] { return context->libArchiveContext.dataAvailable; });
    if (!conditionMet)
    {
        archive_set_error(archive, ARCHIVE_FAILED, "Timed out waiting for data to be provided");
        *buffer = nullptr;
        return ARCHIVE_FAILED;
    }

    // Set all data (copyless)
    context->libArchiveContext.dataAvailable = false;
    *buffer = context->libArchiveContext.dataBuffer.data();
    return context->libArchiveContext.dataBufferLength;
}

static la_ssize_t ArchiveWriteToFileCallback(struct archive* archive, void* clientData, const void* buffer, size_t length) noexcept
{
    auto context = static_cast<ArchiveContext*>(clientData);
    if (!context)
    {
        if (archive)
        {
            archive_set_error(archive, ARCHIVE_FATAL, "Archive write context is null");
        }
        return ARCHIVE_FATAL;
    }

    auto hr = FilePAL::WriteFileBytes(context->libArchiveContext.zipFileHandle, static_cast<const char*>(buffer), length);
    if (FAILED(hr))
    {
        if (SUCCEEDED(context->libArchiveContext.callbackResult))
        {
            context->libArchiveContext.callbackResult = hr;
        }
        if (archive)
        {
            archive_set_error(archive, ARCHIVE_FATAL, "Archive destination write failed (0x%08X)", static_cast<unsigned>(hr));
        }
        return ARCHIVE_FATAL;
    }

    context->AddCompressedBytesSize(length);
    return length;
}

static la_ssize_t ArchiveWriteCallback(struct archive* archive, void* clientData, const void* buffer, size_t length) noexcept
{
    UNREFERENCED_PARAMETER(archive);

    auto context = static_cast<ArchiveContext*>(clientData);
    context->AddCompressedBytesSize(length);

    {
        // Lock to provide data
        std::unique_lock<std::mutex> lock(context->libArchiveContext.dataMutex);
        context->libArchiveContext.dataBuffer.resize(length);
        memcpy(context->libArchiveContext.dataBuffer.data(), buffer, length);
        context->libArchiveContext.dataBufferLength = length;
        context->libArchiveContext.dataAvailable = true;
    }

    // Notify the read callback that data is available
    context->libArchiveContext.dataAvailableCondition.notify_one();

    return length;
}

ArchiveContext::~ArchiveContext()
{
    TRACE_INFORMATION("ArchiveContext::~ArchiveContext");
    Close();
}

HRESULT ArchiveContext::ConvertLibarchiveResult(int ar, const char* message) noexcept
{
    HRESULT hr = S_OK;

    if (ar < ARCHIVE_OK)
    {
        auto internalArchive = reinterpret_cast<archive*>(m_archive);
        // m_archive is null while Initialize is still setting the archive up, and
        // archive_error_string can itself return null - neither is safe to hand to "%s".
        const char* errorString = internalArchive ? archive_error_string(internalArchive) : nullptr;
        if (errorString == nullptr)
        {
            errorString = "<no error string>";
        }

        if (ar < ARCHIVE_WARN)
        {
            TRACE_ERROR("Error - %s: (%d) %s", message, ar, errorString);
            hr = FAILED(libArchiveContext.callbackResult) ? libArchiveContext.callbackResult : E_FAIL;
        }
        else
        {
            TRACE_WARNING("Warning - %s: (%d) %s", message, ar, errorString);
        }
    }

    return CheckNotifyResult(hr);
}

HRESULT ArchiveContext::CheckNotifyResult(HRESULT hr, const char* message) noexcept
{
    {
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
        if (SUCCEEDED(m_lastArchiveResult))
        {
            m_lastArchiveResult = hr;
        }
    }

    if (FAILED(hr))
    {
        if (message)
        {
            TRACE_ERROR_HR(hr, message);
        }

        {
            // Fail fast if we're in a bad state
            std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
            libArchiveContext.reachedZipEof = true;
            if (m_mode == ArchiveOpenMode::Decompress)
            {
                libArchiveContext.dataAvailable = false;
            }
            else
            {
                libArchiveContext.dataAvailable = true;
            }
        }

        if (m_mode == ArchiveOpenMode::Decompress)
        {
            libArchiveContext.dataRequestedCondition.notify_one();
        }
        else
        {
            libArchiveContext.dataAvailableCondition.notify_one();
        }
    }

    return m_lastArchiveResult;
}

HRESULT ArchiveContext::Initialize(ArchiveOpenMode mode, ArchiveSource source, String zipFilePath) noexcept
{
    int ar{};
    archive* internalArchive{};

    // A context outlives a single archive operation: a download that fails part-way through
    // extraction is retried with the same CompressedFile, and therefore the same context. A failure
    // latches reachedZipEof (see CheckNotifyResult) and leaves the entry cursor mid-file, so without
    // this reset the retry reported "done" before reading a byte - AddUser then succeeded over a
    // truncated or missing save. The file list (m_fileMap) is deliberately kept; it is the
    // operation's input, not its progress.
    {
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
        libArchiveContext.reachedZipEof = false;
        libArchiveContext.dataAvailable = false;
        libArchiveContext.dataBufferLength = 0;
        libArchiveContext.callbackResult = S_OK;
        m_lastArchiveResult = S_OK;
    }
    if (m_entryFile)
    {
        FilePAL::CloseFile(m_entryFile);
        m_entryFile.reset();
    }
    m_entryFilePath.clear();
    m_remainingBuffer.clear();
    m_uncompressedBytesWritten = 0;
    m_nextFileIndex = 0;
    m_nextFileIt = m_fileMap.end();
    m_pendingEntryBytes = 0;
    m_lastCreatedDirectory.clear();
    m_skipEntry = false;
    m_fileRemainingSize = 0;
    m_open = false;

    // Publish the allocation (and the mode it was allocated with) before any fallible setup call,
    // so error logging has a real archive to read its message from and every failure path below
    // can free it instead of leaking it.
    m_mode = mode;
    m_source = source;

    auto freeArchiveOnFailure = [&]()
    {
        if (internalArchive != nullptr)
        {
            if (mode == ArchiveOpenMode::Decompress)
            {
                archive_read_free(internalArchive);
            }
            else
            {
                archive_write_free(internalArchive);
            }
            internalArchive = nullptr;
            m_archive = nullptr;
        }
    };

    if (mode == ArchiveOpenMode::Decompress)
    {
        internalArchive = archive_read_new();
        if (internalArchive == nullptr)
        {
            return E_OUTOFMEMORY;
        }
        m_archive = reinterpret_cast<pfarchive*>(internalArchive);

        ar = archive_read_support_format_zip(internalArchive);
        if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive format")))
        {
            freeArchiveOnFailure();
            return E_FAIL;
        }
        ar = archive_read_support_filter_gzip(internalArchive);
        if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive compression")))
        {
            freeArchiveOnFailure();
            return E_FAIL;
        }

        if (source == ArchiveSource::File)
        {
            auto fileResult = FilePAL::OpenFile(zipFilePath, FileOpenMode::Read);
            if (FAILED(fileResult.hr))
            {
                freeArchiveOnFailure();
                return fileResult.hr;
            }
            libArchiveContext.zipFileHandle = fileResult.ExtractPayload();
            libArchiveContext.dataBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
        }
    }
    else
    {
        TRACE_INFORMATION("ArchiveContext::Initialize Compress");
        internalArchive = archive_write_new();
        if (internalArchive == nullptr)
        {
            return E_OUTOFMEMORY;
        }
        m_archive = reinterpret_cast<pfarchive*>(internalArchive);

        ar = archive_write_set_format_zip(internalArchive);
        if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive format")))
        {
            freeArchiveOnFailure();
            return E_FAIL;
        }
        ar = archive_write_set_options(internalArchive, "compression=deflate,!zip64");
        if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive compression")))
        {
            freeArchiveOnFailure();
            return E_FAIL;
        }

        TRACE_INFORMATION("ArchiveContext::Initialize Compress Source:%d", source);
        if (source == ArchiveSource::File)
        {
            auto fileResult = FilePAL::OpenFile(zipFilePath, FileOpenMode::Write);
            if (FAILED(fileResult.hr))
            {
                freeArchiveOnFailure();
                return fileResult.hr;
            }
            libArchiveContext.zipFileHandle = fileResult.ExtractPayload();
        }
        else
        {
            // Set the block size to 1MB (up from 10KB default) when compressing to memory to reduce upload requests
            // libarchive will trigger ArchiveWriteCallback only when the compressed data reaches this size
            ar = archive_write_set_bytes_per_block(internalArchive, 1024 * 1024);
            if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive block size")))
            {
                freeArchiveOnFailure();
                return E_FAIL;
            }
            ar = archive_write_set_bytes_in_last_block(internalArchive, 1);
            if (FAILED(ConvertLibarchiveResult(ar, "Failed to set archive last block size")))
            {
                freeArchiveOnFailure();
                return E_FAIL;
            }
        }
    }

    return S_OK;
}

HRESULT ArchiveContext::AddFile(String& relativePath, ArchiveFileDetail&& file) noexcept
{
    std::replace(relativePath.begin(), relativePath.end(), '\\', '/');
    size_t trimPos = relativePath.find_first_not_of('/');
    if (trimPos != String::npos)
    {
        relativePath.erase(0, trimPos);
    }

    // Path traversal protection: reject ".." only as a path segment, not as a substring (e.g., "foo..bar" is valid).
    // Each segment is trimmed of trailing spaces/dots first, because Win32 path normalization
    // strips those - ".. " and "..." both resolve to the parent directory.
    size_t segmentStart = 0;
    while (segmentStart <= relativePath.length())
    {
        size_t segmentEnd = relativePath.find('/', segmentStart);
        if (segmentEnd == String::npos)
        {
            segmentEnd = relativePath.length();
        }

        String segment = relativePath.substr(segmentStart, segmentEnd - segmentStart);
        if (!segment.empty())
        {
            size_t lastValid = segment.find_last_not_of(" .");
            if (lastValid == String::npos || segment.substr(0, lastValid + 1) == "..")
            {
                // Segment is all dots/spaces, or trims down to "..".
                return E_INVALIDARG;
            }
        }

        segmentStart = segmentEnd + 1;
    }

    // Individual file size limit
    if (file.uncompressedSize > kMaxIndividualFileSize)
    {
        return E_INVALIDARG;
    }

    // Replacing an existing entry must not double-count its size against the archive limits.
    auto existing = m_fileMap.find(relativePath);
    uint64_t supersededSize = (existing != m_fileMap.end()) ? existing->second.uncompressedSize : 0;
    uint64_t currentTotal = m_totalUncompressedSize - supersededSize;

    // Integer overflow check
    if (currentTotal > UINT64_MAX - file.uncompressedSize)
    {
        return E_INVALIDARG;
    }

    // Total archive size limit (check before updating running total)
    if (currentTotal + file.uncompressedSize > kMaxTotalUncompressedSize)
    {
        return E_INVALIDARG;
    }

    m_totalUncompressedSize = currentTotal + file.uncompressedSize;

    m_fileMap.insert_or_assign(relativePath, file);

    return S_OK;
}

void ArchiveContext::ClearFiles() noexcept
{
    m_fileMap.clear();
    m_totalUncompressedSize = 0;
}

UnorderedMap<String, ArchiveFileDetail>& ArchiveContext::GetFiles() noexcept
{
    return m_fileMap;
}

uint64_t ArchiveContext::GetTotalUncompressedSize() const noexcept
{
    return m_totalUncompressedSize;
}

uint64_t ArchiveContext::GetTotalCompressedSize() const noexcept
{
    return m_totalCompressedSize;
}

uint64_t ArchiveContext::GetUncompressedBytesProcessed() const noexcept
{
    return m_uncompressedBytesWritten;
}

void ArchiveContext::AddCompressedBytesSize(uint64_t size) noexcept
{
    m_totalCompressedSize += size;
}

bool ArchiveContext::IsArchiveOperationDone() noexcept
{
    std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
    return libArchiveContext.reachedZipEof;
}

HRESULT ArchiveContext::CompressBytesGetAnyData(String& destination, size_t* uncompressedBytesWritten, bool* finishedCompressing) noexcept
{
    RETURN_HR_IF(E_INVALIDARG, !finishedCompressing || !uncompressedBytesWritten);
    HRESULT hr = S_OK;

    {
        // Wait for data to be provided
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);

        auto conditionMet = libArchiveContext.dataAvailableCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return libArchiveContext.dataAvailable || libArchiveContext.reachedZipEof; });
        if (!conditionMet)
        {
            // Compression operation has not finished providing the next compressed bytes
            return E_PENDING;
        }

        *finishedCompressing = libArchiveContext.reachedZipEof;
        if (FAILED(m_lastArchiveResult))
        {
            *uncompressedBytesWritten = 0;
            return m_lastArchiveResult;
        }

        if (libArchiveContext.reachedZipEof && libArchiveContext.dataBufferLength == 0)
        {
            destination.clear();
        }
        else
        {
            // Copy all data
            destination.resize(libArchiveContext.dataBufferLength);
            memcpy(destination.data(), libArchiveContext.dataBuffer.data(), libArchiveContext.dataBufferLength);
            Vector<char>().swap(libArchiveContext.dataBuffer);
            libArchiveContext.dataBufferLength = 0;

            libArchiveContext.dataAvailable = false;
        }

        *uncompressedBytesWritten = m_uncompressedBytesWritten;
        hr = m_lastArchiveResult;
        m_uncompressedBytesWritten = 0;
    }

    // Signal write callback to fill buffer
    libArchiveContext.dataRequestedCondition.notify_one();

    return hr;
}

HRESULT ArchiveContext::CompressBytesGetData(size_t bytesAvailable, char* destination, size_t* bytesWritten, size_t* uncompressedBytesWritten, bool* finishedCompressing) noexcept
{
    RETURN_HR_IF(E_INVALIDARG, !destination || !bytesWritten || !uncompressedBytesWritten || !finishedCompressing || bytesAvailable == 0);

    *finishedCompressing = false;

    // Leftover data from previous buffer
    if (!m_remainingBuffer.empty())
    {
        auto bytesToCopy = std::min(bytesAvailable, m_remainingBuffer.size());
        memcpy(destination, m_remainingBuffer.data(), bytesToCopy);
        m_remainingBuffer.erase(m_remainingBuffer.begin(), m_remainingBuffer.begin() + bytesToCopy);
        *bytesWritten = bytesToCopy;

        {
            std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
            *finishedCompressing = libArchiveContext.reachedZipEof;
            *uncompressedBytesWritten = m_uncompressedBytesWritten;
            m_uncompressedBytesWritten = 0;

            if (m_remainingBuffer.empty())
            {
                Vector<char>().swap(m_remainingBuffer);
                libArchiveContext.dataAvailable = false;
            }
        }

        return S_OK;
    }

    // Signal write callback to fill buffer
    libArchiveContext.dataRequestedCondition.notify_one();

    // Wait for data to be provided
    std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
    auto conditionMet = libArchiveContext.dataAvailableCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return libArchiveContext.dataAvailable || libArchiveContext.reachedZipEof; });
    if (!conditionMet)
    {
        // Compression operation has not finished providing the next compressed bytes
        return E_PENDING;
    }
    
    *finishedCompressing = libArchiveContext.reachedZipEof;
    if (FAILED(m_lastArchiveResult))
    {
        *bytesWritten = 0;
        *uncompressedBytesWritten = 0;
        return m_lastArchiveResult;
    }

    if (libArchiveContext.dataAvailable)
    {
        // If we have extra data, save it for next time, and keep dataAvailable true
        if (libArchiveContext.dataBufferLength > bytesAvailable)
        {
            m_remainingBuffer.insert(m_remainingBuffer.end(), libArchiveContext.dataBuffer.data() + bytesAvailable, libArchiveContext.dataBuffer.data() + libArchiveContext.dataBufferLength);
        }
        else
        {
            libArchiveContext.dataAvailable = false;
        }

        // Copy all data
        auto bytesToCopy = std::min(bytesAvailable, libArchiveContext.dataBufferLength);
        memcpy(destination, libArchiveContext.dataBuffer.data(), bytesToCopy);
        *bytesWritten = bytesToCopy;
        Vector<char>().swap(libArchiveContext.dataBuffer);
        *uncompressedBytesWritten = m_uncompressedBytesWritten;
        m_uncompressedBytesWritten = 0;
        return m_lastArchiveResult;
    }

    // Else we've reached the end of the zip file
    *bytesWritten = 0;
    *uncompressedBytesWritten = 0;
    return m_lastArchiveResult;
}

HRESULT ArchiveContext::CompressBytes() noexcept
{
    {
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
        auto conditionMet = libArchiveContext.dataRequestedCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return !libArchiveContext.dataAvailable; });
        if (!conditionMet)
        {
            // Write/Upload operation has not finished writing the previous compressed bytes
            return S_OK;
        }
    }

    int ar{};
    auto internalArchive = reinterpret_cast<archive*>(m_archive);
    auto internalEntry = reinterpret_cast<archive_entry*>(m_archiveEntry);

    TRACE_VERBOSE("ArchiveContext::CompressBytes");
    if (libArchiveContext.reachedZipEof)
    {
        return S_OK;
    }

    if (!m_open)
    {
        m_open = true;

        TRACE_INFORMATION("ArchiveContext::CompressBytes open");
        auto writeCallback = m_source == ArchiveSource::File ? ArchiveWriteToFileCallback : ArchiveWriteCallback;
        ar = archive_write_open2(internalArchive, this, ArchiveEmptyCallback, writeCallback, ArchiveEmptyCallback, ArchiveEmptyCallback);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to open memory for archive write"));
    }

    if (!m_entryFile)
    {
        if (m_nextFileIndex >= m_fileMap.size())
        {
            // We have residual metadata that will be written here even if there's nothing left to read
            ar = archive_write_close(internalArchive);
            RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to close archive write"));

            {
                std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
                libArchiveContext.reachedZipEof = true;
            }

            libArchiveContext.dataAvailableCondition.notify_one();
            return S_OK;
        }

        auto it = (m_nextFileIndex == 0) ? m_fileMap.begin() : m_nextFileIt;
        m_nextFileIt = std::next(it);
        auto& relativePath = it->first;
        auto& file = it->second;

        m_fileRemainingSize = file.uncompressedSize;
        TRACE_VERBOSE("ArchiveContext::CompressBytes FileRemainingSize:%llu", m_fileRemainingSize.load());

        if (!internalEntry)
        {
            internalEntry = archive_entry_new();
            m_archiveEntry = reinterpret_cast<pfarchive_entry*>(internalEntry);
        }
        else
        {
            archive_entry_clear(internalEntry);
        }

        archive_entry_set_pathname(internalEntry, relativePath.c_str());
        archive_entry_set_mode(internalEntry, S_IFREG);
        archive_entry_set_size(internalEntry, file.uncompressedSize);
        
        // Convert local time to UTC for archive mtime.
        // GRTS extracts ZIP files treating mtime as UTC, so we must offset by timezone.
        struct tm utcTm{};
        time_t adjustedTime{};
#if HC_PLATFORM_IS_PLAYSTATION
        if (gmtime_s(&file.timeLastModified, &utcTm) == 0)
        {
            time_t localTime = mktime(&utcTm);
            struct tm localTm{};
            gmtime_s(&localTime, &localTm);
            time_t utcOffset = mktime(&localTm) - localTime;
            adjustedTime = localTime - utcOffset;
        }
        else
        {
            TRACE_WARNING("Failed to convert time to UTC using gmtime_s");
        }
#elif defined(_WIN32)
        if (gmtime_s(&utcTm, &file.timeLastModified) == 0)
        {
            adjustedTime = _mkgmtime(&utcTm);
        }
        else
        {
            TRACE_WARNING("Failed to convert time to UTC using gmtime_s");
        }
#else
        if (gmtime_r(&file.timeLastModified, &utcTm) != nullptr)
        {
            time_t localTime = mktime(&utcTm);
            struct tm localTm{};
            gmtime_r(&localTime, &localTm);
            time_t utcOffset = mktime(&localTm) - localTime;
            adjustedTime = localTime - utcOffset;
        }
        else
        {
            TRACE_WARNING("Failed to convert time to UTC using gmtime_r");
        }
#endif
        if (adjustedTime != static_cast<time_t>(-1))
        {
            archive_entry_set_mtime(internalEntry, adjustedTime, 0);
        }
        else
        {
            TRACE_WARNING("Failed to convert UTC time for archive mtime, using original time");
            archive_entry_set_mtime(internalEntry, file.timeLastModified, 0);
        }

        TRACE_INFORMATION("[GAME SAVE] ArchiveContext: Compressing %s", file.fullPath.c_str());
        auto fileResult = FilePAL::OpenFile(file.fullPath, FileOpenMode::Read);
        RETURN_IF_FAILED(CheckNotifyResult(fileResult.hr, "Unable to open archive entry file"));
        m_entryFile = fileResult.ExtractPayload();
        ++m_nextFileIndex;

        // Read the entry's first chunk now, before the header is written, so it can pick the
        // entry's compression method. The chunk is written out by the data step below.
        m_pendingEntryBytes = 0;
        bool storeEntry = false;
        if (m_fileRemainingSize > 0)
        {
            m_ioBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
            size_t sampleBytesRead{};
            auto hr = FilePAL::ReadFileBytes(m_entryFile, FILEPAL_MAX_BYTES_AVAILABLE, m_ioBuffer.data(), &sampleBytesRead);
            RETURN_IF_FAILED(CheckNotifyResult(hr, "Unable to read from archive entry file"));
            m_uncompressedBytesWritten += sampleBytesRead;
            m_pendingEntryBytes = sampleBytesRead;
            storeEntry = LooksIncompressible(m_ioBuffer.data(), sampleBytesRead);
        }

        // Format options can only be set before the archive is opened; these setters are allowed
        // between entries, and the zip writer applies them when the next header is written.
        ar = storeEntry ? archive_write_zip_set_compression_store(internalArchive) : archive_write_zip_set_compression_deflate(internalArchive);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set entry compression"));
        if (storeEntry)
        {
            TRACE_VERBOSE("[GAME SAVE] ArchiveContext: storing %s uncompressed (sample looks incompressible)", file.fullPath.c_str());
        }

        ar = archive_write_header(internalArchive, internalEntry);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to write header to archive"));
    }

    if (m_fileRemainingSize > 0)
    {
        size_t fileBytesRead{};
        if (m_pendingEntryBytes > 0)
        {
            // The sample read when the entry was started; already counted as processed.
            fileBytesRead = m_pendingEntryBytes;
            m_pendingEntryBytes = 0;
        }
        else
        {
            TRACE_VERBOSE("ArchiveContext::CompressBytes ReadFileBytes");
            m_ioBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
            auto hr = FilePAL::ReadFileBytes(m_entryFile, FILEPAL_MAX_BYTES_AVAILABLE, m_ioBuffer.data(), &fileBytesRead);
            RETURN_IF_FAILED(CheckNotifyResult(hr, "Unable to read from archive entry file"));

            m_uncompressedBytesWritten += fileBytesRead;
        }

        if (fileBytesRead == 0)
        {
            // The file on disk is shorter than the size recorded when it was scanned (it was
            // truncated or rewritten between the metadata scan and this compression pass).
            // ReadFileBytes reports EOF as S_OK with zero bytes, so without this guard nothing is
            // subtracted from m_fileRemainingSize, the entry file is never closed and
            // reachedZipEof is never set - leaving the caller's
            // "while (SUCCEEDED(hr) && !IsArchiveOperationDone())" loop spinning forever with the
            // sync mutex held. Fail the upload instead; this mirrors the truncated-entry handling
            // on the decompress side.
            TRACE_ERROR("[GAME SAVE] ArchiveContext: archive entry source file ended with %llu bytes still expected",
                m_fileRemainingSize.load());
            FilePAL::CloseFile(m_entryFile);
            m_entryFile.reset();
            m_fileRemainingSize = 0;
            return CheckNotifyResult(E_UNEXPECTED, "Truncated archive entry source file");
        }

        if (static_cast<uint64_t>(fileBytesRead) > m_fileRemainingSize.load())
        {
            return E_UNEXPECTED;
        }
        m_fileRemainingSize -= static_cast<uint64_t>(fileBytesRead);

        auto lastBytesWritten = archive_write_data(internalArchive, m_ioBuffer.data(), fileBytesRead);
        RETURN_IF_FAILED(ConvertLibarchiveResult(static_cast<int>(lastBytesWritten), "Failed to write data to archive"));
    }

    if (m_fileRemainingSize == 0)
    {
        TRACE_VERBOSE("ArchiveContext::CompressBytes CloseFile");
        FilePAL::CloseFile(m_entryFile);
        m_entryFile.reset();
    }

    return S_OK;
}

HRESULT ArchiveContext::DecompressBytesProvideData(const char* source, size_t bytesAvailable, size_t* uncompressedBytesWritten) noexcept
{
    RETURN_HR_IF(E_INVALIDARG, !source || !uncompressedBytesWritten || bytesAvailable == 0);

    {
        // Lock to provide data
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
        if (libArchiveContext.reachedZipEof)
        {
            *uncompressedBytesWritten = 0;
            return m_lastArchiveResult;
        }

        // Provide data
        libArchiveContext.dataBuffer.resize(bytesAvailable);
        memcpy(libArchiveContext.dataBuffer.data(), source, bytesAvailable);
        libArchiveContext.dataBufferLength = bytesAvailable;
        libArchiveContext.dataAvailable = true;
    }

    // Notify the read callback that data is available
    libArchiveContext.dataAvailableCondition.notify_one();

    // Wait to return until done decompressing current buffer. Time out in case of a hang (probably due to an XAsync scheduling issue)
    std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
    auto conditionMet = libArchiveContext.dataRequestedCondition.wait_for(lock, std::chrono::seconds(5), [this] { return !libArchiveContext.dataAvailable || libArchiveContext.reachedZipEof; });
    if (!conditionMet)
    {
        return E_FAIL;
    }
        
    *uncompressedBytesWritten = m_uncompressedBytesWritten;
    m_uncompressedBytesWritten = 0;
    return m_lastArchiveResult;
}

HRESULT ArchiveContext::DecompressBytes() noexcept
{
    int ar{};
    auto internalArchive = reinterpret_cast<archive*>(m_archive);
    auto internalEntry = reinterpret_cast<archive_entry*>(m_archiveEntry);

    if (libArchiveContext.reachedZipEof)
    {
        return S_OK;
    }

    if (!m_open)
    {
        m_open = true;
        auto readCallback = m_source == ArchiveSource::File ? ArchiveReadFromFileCallback : ArchiveReadCallback;
        ar = archive_read_open2(internalArchive, this, ArchiveEmptyCallback, readCallback, nullptr, ArchiveEmptyCallback);
        // Opening is fallible (bad source, allocation failure). Continuing against an unopened
        // archive just produces confusing downstream errors.
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to open archive for read"));
    }

    if (m_skipEntry)
    {
        ar = archive_read_data_skip(internalArchive);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to skip entry in archive"));

        if (ar >= ARCHIVE_OK)
        {
            if (ar == ARCHIVE_EOF)
            {
                std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
                libArchiveContext.reachedZipEof = true;
            }

            m_skipEntry = false;
        }
        
        return S_OK;
    }

    // Start next entry if current entry is closed
    if (!m_entryFile)
    {
        ar = archive_read_next_header(internalArchive, &internalEntry);
        m_archiveEntry = reinterpret_cast<pfarchive_entry*>(internalEntry);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to read next header in archive"));

        // Finding next header didn't fail but warned, meaning we're on a header boundary, or it returned EOF,
        // meaning we're at the end of the archive and any remaining data is just the central directory
        if (ar < ARCHIVE_OK)
        {
            return S_OK;
        }
        else if (ar == ARCHIVE_EOF)
        {
            std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
            libArchiveContext.reachedZipEof = true;
            return S_OK;
        }

        auto relativePath = archive_entry_pathname(internalEntry);
        if (relativePath == nullptr || m_fileMap.count(relativePath) == 0)
        {
            m_skipEntry = true;
            return S_OK;
        }

        auto file = m_fileMap[relativePath];
        auto filePath = file.fullPath;
        auto parentPath = FilePAL::GetParentPath(filePath);
        RETURN_IF_FAILED(parentPath.hr);
        if (parentPath.Payload() != m_lastCreatedDirectory)
        {
            RETURN_IF_FAILED(FilePAL::CreatePath(parentPath.Payload()));
            m_lastCreatedDirectory = parentPath.ExtractPayload();
        }

        TRACE_INFORMATION("[GAME SAVE] ArchiveContext: Decompressing %s", filePath.c_str());
        // Extract to an SDK-owned temporary file (see kArchiveExtractTempPrefix): the destination
        // (often the title's synced save file) is only replaced once this entry is proven complete
        // below. The selection key (the entry's relative path) locates the extraction root.
        String tempPath;
        RETURN_IF_FAILED(CheckNotifyResult(ChooseExtractTempPath(filePath, String{ relativePath }, tempPath), "Unable to choose a temporary file for the archive entry"));
#if HC_PLATFORM_IS_MICROSOFT
        if (TempPathTooLong(tempPath, filePath))
        {
            // Rare: a destination whose path is shorter than the temporary file's (a short name in
            // the extraction root) and which already sits at the platform's path limit. Extracting
            // in place keeps the file downloadable; it loses only the protection of the temporary
            // file, as extraction was before it.
            TRACE_WARNING("[GAME SAVE] ArchiveContext: the temporary path for %s is too long; extracting in place", filePath.c_str());
            tempPath.clear();
        }
#endif
        auto fileResult = FilePAL::OpenFile(tempPath.empty() ? filePath : tempPath, FileOpenMode::Write);
        RETURN_IF_FAILED(CheckNotifyResult(fileResult.hr, "Unable to open archive entry file"));

        m_entryFile = fileResult.ExtractPayload();
        m_entryFilePath = filePath;
        m_entryTempPath = std::move(tempPath);
        m_entryFileTimeLastModified = file.timeLastModified;
        m_entryFileTimeCreated = file.timeCreated;
        m_fileRemainingSize = file.uncompressedSize;
    }

    // Read data from the current entry
    m_ioBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
    la_ssize_t bytesRead{};
    if (m_fileRemainingSize > 0 && (bytesRead = archive_read_data(internalArchive, m_ioBuffer.data(), FILEPAL_MAX_BYTES_AVAILABLE)) > 0)
    {
        if (static_cast<uint64_t>(bytesRead) > m_fileRemainingSize.load())
        {
            // The entry holds more than the (untrusted) manifest selected: a zip-bomb shape, or a
            // bundle that doesn't match its manifest. Stop before writing past the selected size.
            TRACE_ERROR("[GAME SAVE] ArchiveContext: archive entry '%s' is longer than the %llu bytes still expected",
                m_entryFilePath.c_str(), m_fileRemainingSize.load());
            AbandonExtractedEntry();
            m_fileRemainingSize = 0;
            return CheckNotifyResult(E_UNEXPECTED, "Archive entry longer than selected");
        }

        auto hr = FilePAL::WriteFileBytes(m_entryFile, m_ioBuffer.data(), static_cast<size_t>(bytesRead));
        if (FAILED(hr))
        {
            AbandonExtractedEntry();
        }
        RETURN_IF_FAILED(CheckNotifyResult(hr, "Unable to write to archive entry file"));
        m_uncompressedBytesWritten += bytesRead;
        m_fileRemainingSize -= static_cast<uint64_t>(bytesRead);
    }
    else if (m_fileRemainingSize > 0 && bytesRead == 0)
    {
        // The entry ended before the size the (untrusted) manifest advertised. Neither completion
        // condition below would fire, so the caller's decompress loop would spin forever. Fail the
        // extraction instead. The partial bytes are only in the temporary file, which goes; the
        // destination (the title's real save file) was never touched.
        TRACE_ERROR("[GAME SAVE] ArchiveContext: archive entry '%s' ended with %llu bytes still expected",
            m_entryFilePath.c_str(), m_fileRemainingSize.load());
        AbandonExtractedEntry();
        m_fileRemainingSize = 0;
        return CheckNotifyResult(E_UNEXPECTED, "Truncated archive entry");
    }

    // We've reached the end of the entry
    if (m_fileRemainingSize == 0 || bytesRead < 0)
    {
        // Exactly the selected size was written. The entry must also end here: read once more.
        // libarchive verifies an entry's CRC and sizes only on the read that reaches its end, which
        // the read above never makes when the selected size fills its buffer exactly (an entry whose
        // size is a multiple of the buffer, or one longer than the manifest says). More data, or a
        // failed check, means the entry isn't the one the manifest selected.
        la_ssize_t endRead{};
        if (bytesRead >= 0)
        {
            char probe{};
            endRead = archive_read_data(internalArchive, &probe, 1);
        }

        // Close the file and recurse to start the next entry
        FilePAL::CloseFile(m_entryFile);
        m_entryFile.reset();

        if (bytesRead < 0 || endRead != 0)
        {
            // A read error, more data than selected, or a failed end-of-entry check: not proven.
            // The destination keeps its previous content.
            AbandonExtractedEntry();
            if (bytesRead < 0)
            {
                // Any failed read fails the extraction. ARCHIVE_WARN counts: libarchive reports some
                // failed end-of-entry checks (an authentication-code mismatch; a bad CRC in some
                // versions) that way, and ConvertLibarchiveResult treats a warning as success.
                const HRESULT readHr = ConvertLibarchiveResult(static_cast<int>(bytesRead), "Failed to read data from archive");
                m_fileRemainingSize = 0;
                return CheckNotifyResult(FAILED(readHr) ? readHr : E_UNEXPECTED, "Unable to read the archive entry");
            }
            TRACE_ERROR("[GAME SAVE] ArchiveContext: archive entry '%s' %s after its selected size",
                m_entryFilePath.c_str(), endRead > 0 ? "has more data" : "failed its end-of-entry checks");
            return CheckNotifyResult(E_UNEXPECTED, "Archive entry doesn't end at its selected size");
        }
        else
        {
            // Proven: exactly the selected size was written (m_fileRemainingSize only reaches 0 by
            // counting it down) and the entry ended there. Apply the times, then replace the
            // destination in one rename (nothing to move when the entry was extracted in place).
            const String& writtenPath = m_entryTempPath.empty() ? m_entryFilePath : m_entryTempPath;
            HRESULT timesHr = FilePAL::SetFileLastModifiedTime(writtenPath, m_entryFileTimeCreated, m_entryFileTimeLastModified);
            HRESULT moveHr = timesHr;
            if (SUCCEEDED(timesHr) && !m_entryTempPath.empty())
            {
                moveHr = FilePAL::MoveLocalFile(m_entryTempPath, m_entryFilePath);
            }
            if (FAILED(moveHr))
            {
                TRACE_ERROR("[GAME SAVE] ArchiveContext: couldn't put archive entry '%s' in place hr=0x%08X", m_entryFilePath.c_str(), moveHr);
                AbandonExtractedEntry();
                return CheckNotifyResult(moveHr, "Unable to replace the archive entry's file");
            }
            m_entryTempPath.clear();
        }
    }

    return S_OK;
}

void ArchiveContext::Close() noexcept
{
    TRACE_INFORMATION("ArchiveContext::Close");

    // NOTE: the destination file handle is deliberately NOT closed here. archive_write_free below
    // finishes the archive, and finishing flushes any buffered data through the write callback --
    // which, in ArchiveSource::File mode, writes to that very handle. Closing first meant every
    // abandoned compression (an entry whose source file could not be opened, a cancel, an error
    // mid-archive) flushed into a released handle, which asserts inside FilePAL on debug builds and
    // stalls the upload with no further progress. The handle is closed after the free instead.

    // m_open only becomes true on the first Compress/DecompressBytes call, but Initialize()
    // allocates the archive (and, in compress mode, the entry) before that. Skipping the whole
    // block here leaked both whenever a context was initialized and then abandoned - so only the
    // close calls are conditional on m_open; the frees always run.
    const bool wasOpen = m_open;
    m_open = false;

    auto internalArchive = reinterpret_cast<archive*>(m_archive);

    if (internalArchive)
    {
        // m_archive has to be cleared BEFORE the result is logged: ConvertLibarchiveResult
        // dereferences m_archive to call archive_error_string(), so logging a failing free result
        // while m_archive still points at the just-released block is a read of freed memory.
        // Log the numeric code directly instead - libarchive's error string dies with the object.
        if (m_mode == ArchiveOpenMode::Decompress)
        {
            if (wasOpen)
            {
                auto closeResult = archive_read_close(internalArchive);
                ConvertLibarchiveResult(closeResult, "Failed to close archive read");
            }
            auto ar = archive_read_free(internalArchive);
            m_archive = nullptr;
            if (ar < ARCHIVE_WARN)
            {
                TRACE_ERROR("Error - Failed to free archive read: (%d)", ar);
                CheckNotifyResult(E_FAIL);
            }
            else if (ar < ARCHIVE_OK)
            {
                TRACE_WARNING("Warning - Failed to free archive read: (%d)", ar);
            }
        }
        else
        {
            auto ar = archive_write_free(internalArchive);
            m_archive = nullptr;
            if (ar < ARCHIVE_WARN)
            {
                TRACE_ERROR("Error - Failed to free archive write: (%d)", ar);
                CheckNotifyResult(FAILED(libArchiveContext.callbackResult) ? libArchiveContext.callbackResult : E_FAIL);
            }
            else if (ar < ARCHIVE_OK)
            {
                TRACE_WARNING("Warning - Failed to free archive write: (%d)", ar);
            }
        }
    }

    // Only free entries we own (created via archive_entry_new in compress mode).
    // In decompress mode, m_archiveEntry is owned by libarchive and must not be freed.
    auto internalEntry = reinterpret_cast<archive_entry*>(m_archiveEntry);
    if (internalEntry && m_mode == ArchiveOpenMode::Compress)
    {
        archive_entry_free(internalEntry);
        m_archiveEntry = nullptr;
    }

    // Last: libarchive has finished with the destination by now, so any flush it needed to do has
    // already gone through the write callback while this handle was still valid.
    if (libArchiveContext.zipFileHandle)
    {
        FilePAL::CloseFile(libArchiveContext.zipFileHandle);
        libArchiveContext.zipFileHandle.reset();
    }

    // The source file for a partially-written entry (compress mode) is also released here rather
    // than being left open until the context is destroyed. In decompress mode an entry still open
    // here never completed: its temporary file goes, the destination keeps its content.
    if (m_mode == ArchiveOpenMode::Decompress && !m_entryTempPath.empty())
    {
        AbandonExtractedEntry();
    }
    if (m_entryFile)
    {
        FilePAL::CloseFile(m_entryFile);
        m_entryFile.reset();
    }
}

void ArchiveContext::AbandonExtractedEntry() noexcept
{
    if (m_entryFile)
    {
        FilePAL::CloseFile(m_entryFile);
        m_entryFile.reset();
    }
    if (!m_entryTempPath.empty())
    {
        HRESULT hr = FilePAL::DeleteLocalFile(m_entryTempPath);
        if (FAILED(hr))
        {
            TRACE_WARNING("[GAME SAVE] ArchiveContext: couldn't delete the partial extraction %s hr=0x%08X", m_entryTempPath.c_str(), hr);
        }
        m_entryTempPath.clear();
    }
}

} // namespace PlayFab

#endif
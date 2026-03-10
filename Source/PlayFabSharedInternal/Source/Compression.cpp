#include "pch.h"

#include "Compression.h"
#include <algorithm>

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

#include <sys/stat.h>
#include "archive.h"
#include "archive_entry.h"

namespace PlayFab
{

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
    UNREFERENCED_PARAMETER(archive);
    auto context = static_cast<ArchiveContext*>(clientData);

    auto hr = FilePAL::WriteFileBytes(context->libArchiveContext.zipFileHandle, static_cast<const char*>(buffer), length);
    if (FAILED(hr))
    {
        return -1;
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

        if (ar < ARCHIVE_WARN)
        {
            TRACE_ERROR("Error - %s: (%d) %s", message, ar, archive_error_string(internalArchive));
            hr = E_FAIL;
        }
        else
        {
            TRACE_WARNING("Warning - %s: (%d) %s", message, ar, archive_error_string(internalArchive));
        }
    }

    return CheckNotifyResult(hr);
}

HRESULT ArchiveContext::CheckNotifyResult(HRESULT hr, const char* message) noexcept
{
    {
        std::unique_lock<std::mutex> lock(libArchiveContext.dataMutex);
        m_lastArchiveResult = hr;
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

    if (mode == ArchiveOpenMode::Decompress)
    {
        internalArchive = archive_read_new();
        ar = archive_read_support_format_zip(internalArchive);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive format"));
        ar = archive_read_support_filter_gzip(internalArchive);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive compression"));

        if (source == ArchiveSource::File)
        {
            auto fileResult = FilePAL::OpenFile(zipFilePath, FileOpenMode::Read);
            RETURN_IF_FAILED(fileResult.hr);
            libArchiveContext.zipFileHandle = fileResult.ExtractPayload();
            libArchiveContext.dataBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
        }
    }
    else
    {
        TRACE_INFORMATION("ArchiveContext::Initialize Compress");
        internalArchive = archive_write_new();
        ar = archive_write_set_format_zip(internalArchive);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive format"));
        ar = archive_write_set_options(internalArchive, "compression=deflate,!zip64");
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive compression"));

        TRACE_INFORMATION("ArchiveContext::Initialize Compress Source:%d", source);
        if (source == ArchiveSource::File)
        {
            auto fileResult = FilePAL::OpenFile(zipFilePath, FileOpenMode::Write);
            RETURN_IF_FAILED(fileResult.hr);
            libArchiveContext.zipFileHandle = fileResult.ExtractPayload();
        }
        else
        {
            // Set the block size to 1MB (up from 10KB default) when compressing to memory to reduce upload requests
            // libarchive will trigger ArchiveWriteCallback only when the compressed data reaches this size
            ar = archive_write_set_bytes_per_block(internalArchive, 1024 * 1024);
            RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive block size"));
            ar = archive_write_set_bytes_in_last_block(internalArchive, 1);
            RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to set archive last block size"));
        }
    }

    m_mode = mode;
    m_source = source;
    m_archive = reinterpret_cast<pfarchive*>(internalArchive);

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

    m_totalUncompressedSize += file.uncompressedSize;
    m_fileMap.insert_or_assign(relativePath, file);

    return S_OK;
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
    RETURN_HR_IF(E_INVALIDARG, !destination || !bytesWritten || !uncompressedBytesWritten || bytesAvailable == 0);

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
        *bytesWritten = libArchiveContext.dataBufferLength;
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

    TRACE_INFORMATION("ArchiveContext::CompressBytes");
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

        auto it = std::next(m_fileMap.begin(), m_nextFileIndex);
        auto& relativePath = it->first;
        auto& file = it->second;

        m_fileRemainingSize = file.uncompressedSize;
        TRACE_INFORMATION("ArchiveContext::CompressBytes FileRemainingSize:%llu", m_fileRemainingSize);

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
#ifdef _WIN32
        if (gmtime_s(&utcTm, &file.timeLastModified) == 0)
        {
            adjustedTime = _mkgmtime(&utcTm);
        }
        else
        {
            TRACE_WARNING("Failed to convert time to UTC using gmtime_s");
        }
#else
        // Non-GNU platforms use gmtime_s with POSIX signature: struct tm* gmtime_s(const time_t*, struct tm*)
        if (gmtime_s(&file.timeLastModified, &utcTm) != nullptr)
        {
            // timegm is a GNU extension not available on PlayStation; use portable equivalent
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

        //auto before = archive_filter_bytes(internalArchive, 0);
        ar = archive_write_header(internalArchive, internalEntry);
        RETURN_IF_FAILED(ConvertLibarchiveResult(ar, "Failed to write header to archive"));
        //auto after = archive_filter_bytes(internalArchive, 0);

        //*bytesWritten += after - before;

        TRACE_INFORMATION("[GAME SAVE] ArchiveContext: Compressing %s", file.fullPath.c_str());
        auto fileResult = FilePAL::OpenFile(file.fullPath, FileOpenMode::Read);
        RETURN_IF_FAILED(CheckNotifyResult(fileResult.hr, "Unable to open archive entry file"));
        m_entryFile = fileResult.ExtractPayload();
        ++m_nextFileIndex;
    }

    if (m_fileRemainingSize > 0)
    {
        TRACE_INFORMATION("ArchiveContext::CompressBytes ReadFileBytes");
        size_t fileBytesRead{};
        Vector<char> fileBuffer(FILEPAL_MAX_BYTES_AVAILABLE);
        auto hr = FilePAL::ReadFileBytes(m_entryFile, FILEPAL_MAX_BYTES_AVAILABLE, fileBuffer.data(), &fileBytesRead);
        RETURN_IF_FAILED(CheckNotifyResult(hr, "Unable to read from archive entry file"));

        m_uncompressedBytesWritten += fileBytesRead;
        m_fileRemainingSize -= fileBytesRead;

        auto lastBytesWritten = archive_write_data(internalArchive, fileBuffer.data(), fileBytesRead);
        RETURN_IF_FAILED(ConvertLibarchiveResult(static_cast<int>(lastBytesWritten), "Failed to write data to archive"));
    }

    if (m_fileRemainingSize == 0)
    {
        TRACE_INFORMATION("ArchiveContext::CompressBytes CloseFile");
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
        RETURN_IF_FAILED(FilePAL::CreatePath(parentPath.Payload()));

        TRACE_INFORMATION("[GAME SAVE] ArchiveContext: Decompressing %s", filePath.c_str());
        auto fileResult = FilePAL::OpenFile(filePath, FileOpenMode::Write);
        RETURN_IF_FAILED(CheckNotifyResult(fileResult.hr, "Unable to open archive entry file"));

        m_entryFile = fileResult.ExtractPayload();
        m_entryFilePath = filePath;
        m_entryFileTimeLastModified = file.timeLastModified;
        m_entryFileTimeCreated = file.timeCreated;
        m_fileRemainingSize = file.uncompressedSize;
    }

    // Read data from the current entry
    Vector<char> outBuffer{};
    outBuffer.resize(FILEPAL_MAX_BYTES_AVAILABLE);
    la_ssize_t bytesRead{};
    if (m_fileRemainingSize > 0 && (bytesRead = archive_read_data(internalArchive, outBuffer.data(), FILEPAL_MAX_BYTES_AVAILABLE)) > 0)
    {
        auto hr = FilePAL::WriteFileBytes(m_entryFile, outBuffer.data(), static_cast<size_t>(bytesRead));
        RETURN_IF_FAILED(CheckNotifyResult(hr, "Unable to write to archive entry file"));
        m_uncompressedBytesWritten += bytesRead;
        m_fileRemainingSize -= bytesRead;
    }

    // We've reached the end of the entry
    if (m_fileRemainingSize == 0 || bytesRead < 0)
    {
        // Close the file and recurse to start the next entry
        FilePAL::CloseFile(m_entryFile);
        RETURN_IF_FAILED(FilePAL::SetFileLastModifiedTime(m_entryFilePath, m_entryFileTimeCreated, m_entryFileTimeLastModified));
        m_entryFile.reset();
    }
    
    if (bytesRead < 0)
    {
        // bytesRead < 0 indicates an error
        RETURN_IF_FAILED(ConvertLibarchiveResult(static_cast<int>(bytesRead), "Failed to read data from archive"));
    }

    return S_OK;
}

void ArchiveContext::Close() noexcept
{
    TRACE_INFORMATION("ArchiveContext::Close");

    if (libArchiveContext.zipFileHandle)
    {
        FilePAL::CloseFile(libArchiveContext.zipFileHandle);
        libArchiveContext.zipFileHandle.reset();
    }

    if (!m_open)
    {
        return;
    }

    m_open = false;

    auto internalArchive = reinterpret_cast<archive*>(m_archive);

    if (internalArchive)
    {
        if (m_mode == ArchiveOpenMode::Decompress)
        {
            auto ar = archive_read_close(internalArchive);
            ConvertLibarchiveResult(ar, "Failed to close archive read");
            ar = archive_read_free(internalArchive);
            ConvertLibarchiveResult(ar, "Failed to free archive read");
        }
        else
        {
            auto ar = archive_write_free(internalArchive);
            ConvertLibarchiveResult(ar, "Failed to free archive write");
        }

        m_archive = nullptr;
    }
}

} // namespace PlayFab

#endif
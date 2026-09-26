#include "stdafx.h"
#include "ApiHelpers.h"
#include "InfoManifest.h"
#include "GameSaveGlobalState.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"

namespace PlayFab
{
namespace GameSave
{

// Linear Congruential Generator (LCG) constants from Numerical Recipes
// These are the traditional parameters for the MINSTD generator variant
constexpr uint64_t LCG_MULTIPLIER = 1103515245;
constexpr uint64_t LCG_INCREMENT = 12345;
constexpr uint32_t LCG_MODULUS_DIVISOR = 65536;
constexpr uint32_t LCG_MODULUS_RANGE = 32768;

static uint64_t internal_seed = 0;
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr));
    }
    internal_seed = internal_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    return (uint32_t)(internal_seed / LCG_MODULUS_DIVISOR) % LCG_MODULUS_RANGE;
}

char RandomHexDigit()
{
    // Generate a random number between 0 and 15 (inclusive)
    int randomNum = internal_rand() % 16;

    // Convert the random number to a hexadecimal digit
    char hexDigit;
    if (randomNum < 10) {
        hexDigit = static_cast<char>('0' + randomNum);
    }
    else {
        hexDigit = static_cast<char>('A' + (randomNum - 10));
    }

    return hexDigit;
}

// Function to generate a GUID and return it as a string
String CreateGUID()
{
    std::ostringstream guidStream;

    for (int i = 0; i < 32; ++i)
    {
        // Insert dashes at the appropriate positions
        if (i == 8 || i == 12 || i == 16 || i == 20)
        {
            guidStream << '-';
        }
        // Generate a random hexadecimal digit
        guidStream << RandomHexDigit();
    }

    return String(guidStream.str().c_str());
}

String RemoveRootPath(String folder, const String& rootFolder)
{
    size_t index = folder.find(rootFolder, 0);
    if (index != std::string::npos)
    {
        folder.replace(index, rootFolder.length(), "");
    }

    return folder;
}

HRESULT JoinPathHelper(_In_ const String& pathA, _In_ const String& pathB, _Out_ String& pathResult)
{
    Result<String> result = FilePAL::JoinPath(pathA, pathB);
    if (FAILED(result.hr))
    {
        pathResult = String();
        return result.hr;
    }
    pathResult = result.ExtractPayload();
    return S_OK;
}

HRESULT ReadEntireFile(_In_ const String& filePath, _Out_ Vector<char>& fileBuffer)
{
    Result<uint64_t> fileSizeResult = FilePAL::GetFileSize(filePath);
    if (FAILED(fileSizeResult.hr))
    {
        fileBuffer.resize(0);
        return fileSizeResult.hr;
    }
    
    uint64_t fileSize = fileSizeResult.Payload();
    if (fileSize == 0)
    {
        fileBuffer.resize(0);
        return S_OK;
    }
    
    Result<FileHandle> fileResult = FilePAL::OpenFile(filePath, FileOpenMode::Read);
    if (FAILED(fileResult.hr))
    {
        TRACE_ERROR("[GAME SAVE] ReadEntireFile: OpenFile FAILED hr=0x%08X path=%s", fileResult.hr, filePath.c_str());
        return fileResult.hr;
    }
    
    FileHandle file = fileResult.ExtractPayload();
    
    // Guard against files too large for memory (e.g., >256 MB which exceeds game save quota)
    constexpr uint64_t maxAllowedFileSize = 256ULL * 1024 * 1024;
    if (fileSize > maxAllowedFileSize)
    {
        TRACE_ERROR("[GAME SAVE] ReadEntireFile: File too large (%llu bytes), max=%llu, path=%s", fileSize, maxAllowedFileSize, filePath.c_str());
        FilePAL::CloseFile(file);
        return E_OUTOFMEMORY;
    }

    try
    {
        fileBuffer.resize(static_cast<size_t>(fileSize));
    }
    catch (const std::bad_alloc&)
    {
        TRACE_ERROR("[GAME SAVE] ReadEntireFile: Failed to allocate %llu bytes for file %s", fileSize, filePath.c_str());
        FilePAL::CloseFile(file);
        return E_OUTOFMEMORY;
    }

    uint64_t bytesWrittenTotal{ 0 };
    while (bytesWrittenTotal < fileSize)
    {
        uint64_t bytesWritten{ 0 };
        uint64_t bytesRemaining = fileSize - bytesWrittenTotal;
        HRESULT hr = FilePAL::ReadFileBytes(file, bytesRemaining, &fileBuffer[bytesWrittenTotal], &bytesWritten);
        if (FAILED(hr))
        {
            TRACE_ERROR("[GAME SAVE] ReadEntireFile: ReadFileBytes FAILED hr=0x%08X after reading %llu bytes", hr, bytesWrittenTotal);
            FilePAL::CloseFile(file);
            return hr;
        }
        
        if (bytesWritten == 0)
        {
            TRACE_ERROR("[GAME SAVE] ReadEntireFile: ReadFileBytes returned 0 bytes unexpectedly. bytesWrittenTotal=%llu fileSize=%llu", 
                bytesWrittenTotal, fileSize);
            FilePAL::CloseFile(file);
            return E_FAIL; // Return an error code indicating a partial read failure
        }
        
        bytesWrittenTotal += bytesWritten;
    }
    FilePAL::CloseFile(file);

    return S_OK;
}

HRESULT WriteEntireFile(_In_ const String& filePath, _In_ const Vector<char>& fileBuffer)
{
    Result<FileHandle> fileResult = FilePAL::OpenFile(filePath, FileOpenMode::Write);
    if (FAILED(fileResult.hr))
    {
        TRACE_ERROR("[GAME SAVE] WriteEntireFile: OpenFile FAILED hr=0x%08X path=%s", fileResult.hr, filePath.c_str());
        return fileResult.hr;
    }

    FileHandle file = fileResult.ExtractPayload();
    uint64_t fileSize = fileBuffer.size();
    
    HRESULT hr = FilePAL::WriteFileBytes(file, fileBuffer.data(), fileSize);
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] WriteEntireFile: WriteFileBytes FAILED hr=0x%08X path=%s", hr, filePath.c_str());
    }
    FilePAL::CloseFile(file);

    return hr;
}

// Determine cloudsync folder path - use temp storage on platforms that support it
HRESULT GetCloudSyncFolder(_In_ const String& saveFolder, _Out_ String& cloudSyncFolder)
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        String tempPath = globalState->ApiProvider().GetTempCloudSyncPath();
        if (!tempPath.empty())
        {
            cloudSyncFolder = tempPath;
            return S_OK;
        }
    }
    return JoinPathHelper(saveFolder, "cloudsync", cloudSyncFolder);
}

void CleanupTempCloudSyncFiles()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        globalState->ApiProvider().CleanupTempCloudSyncFiles();
    }
}

HRESULT CreateUploadStagingFolder(_In_ const String& saveFolder, _Out_ String& stagingFolder)
{
    stagingFolder.clear();

    String cloudSyncFolder;
    RETURN_IF_FAILED(GetCloudSyncFolder(saveFolder, cloudSyncFolder));
    RETURN_IF_FAILED(FilePAL::CreatePath(cloudSyncFolder));

    // A fresh directory per attempt. Staging used to be a single flat folder that every upload
    // attempt cleared on entry, so a retry (or an upload admitted after a previous provider was
    // abandoned mid-transfer) deleted the zips an still-live transfer was about to open, which
    // surfaced to the title as a bogus argument error from the upload API.
    String folderName = FormatString("%s%s", kUploadStagingFolderPrefix, CreateGUID().c_str());
    RETURN_IF_FAILED(JoinPathHelper(cloudSyncFolder, folderName, stagingFolder));
    RETURN_IF_FAILED(FilePAL::CreatePath(stagingFolder));

    TRACE_INFORMATION("[GAME SAVE] CreateUploadStagingFolder: %s", stagingFolder.c_str());
    return S_OK;
}

void DeleteUploadStagingFolder(_In_ const String& stagingFolder)
{
    if (stagingFolder.empty())
    {
        return;
    }

    TRACE_INFORMATION("[GAME SAVE] DeleteUploadStagingFolder: %s", stagingFolder.c_str());
    HRESULT hr = FilePAL::DeletePath(stagingFolder);
    if (FAILED(hr))
    {
        // Non-fatal. Anything left behind is reclaimed by SweepUploadStagingFolders on the next
        // AddUser, and a staging folder never contains data that is not also either uploaded or
        // reproducible from the local save.
        TRACE_WARNING("[GAME SAVE] DeleteUploadStagingFolder: failed to delete %s, HR:0x%0.8x", stagingFolder.c_str(), hr);
    }
}

void SweepUploadStagingFolders(_In_ const String& saveFolder)
{
    // Reclaims staging folders orphaned by a crash, a suspend, or an upload whose provider was
    // torn down mid-transfer. Only safe to call when no upload can be in flight for this user --
    // AddUser is such a point, because TryReserveDownload rejects while a user is added.
    String cloudSyncFolder;
    if (FAILED(GetCloudSyncFolder(saveFolder, cloudSyncFolder)))
    {
        return;
    }

    if (!FilePAL::DoesDirectoryExist(cloudSyncFolder))
    {
        return;
    }

    Result<Vector<String>> foldersResult = FilePAL::EnumDirectories(cloudSyncFolder);
    if (FAILED(foldersResult.hr))
    {
        return;
    }

    const size_t prefixLength = strlen(kUploadStagingFolderPrefix);
    for (const String& folderName : foldersResult.Payload())
    {
        if (folderName.size() <= prefixLength ||
            folderName.compare(0, prefixLength, kUploadStagingFolderPrefix) != 0)
        {
            continue;
        }

        String fullPath;
        if (FAILED(JoinPathHelper(cloudSyncFolder, folderName, fullPath)))
        {
            continue;
        }

        TRACE_INFORMATION("[GAME SAVE] SweepUploadStagingFolders: removing orphaned %s", fullPath.c_str());
        DeleteUploadStagingFolder(fullPath);
    }
}

// Classifies a failure that came out of local file access during a sync and, when it is one,
// re-reports it as E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE.
//
// The save folder is enumerated once at the start of an upload, but compression and transfer read
// those files later -- potentially much later if the title is suspended in between. A file can
// legitimately disappear or become momentarily unopenable in that window (the title rewriting a
// save or its thumbnail, anti-virus, a platform cloud-sync agent). That is a transient, retryable
// condition, but it used to surface to the title as whatever raw code the file layer produced,
// which said nothing actionable. Anything that is not a local-file condition is passed through
// untouched so genuine service and network errors keep their own codes.
//
// CALLER CONTRACT: only pass HRESULTs that originated in the local file layer (FilePAL, libarchive
// writing through it, or a step whose only failure mode is local I/O). This function cannot verify
// provenance, and one of the codes below aliases a common general-purpose HRESULT:
//
//     __HRESULT_FROM_WIN32(5) == E_ACCESSDENIED == 0x80070005
//
// so an E_ACCESSDENIED arriving from a platform layer (GRTS/XUser/GDK all return it freely) would
// be reported to the title as "transient local file problem, nothing was committed, retry" when it
// may be none of those things. No such path exists today - there is no E_ACCESSDENIED literal in
// Source, and HTTP 403 maps to HTTP_E_STATUS_FORBIDDEN rather than E_ACCESSDENIED - but the
// aliasing is invisible at the call site, so keep the call sites narrow rather than mapping
// whatever a broad continuation happens to hand back.
HRESULT MapLocalFileFailure(HRESULT hr)
{
    if (SUCCEEDED(hr))
    {
        return hr;
    }

    switch (hr)
    {
    // Win32 codes spelled out numerically to match FilePAL's mapping and to stay valid on
    // platforms whose PAL does not define the ERROR_* constants.
    case __HRESULT_FROM_WIN32(2):   // ERROR_FILE_NOT_FOUND
    case __HRESULT_FROM_WIN32(3):   // ERROR_PATH_NOT_FOUND
    case __HRESULT_FROM_WIN32(4):   // ERROR_TOO_MANY_OPEN_FILES
    case __HRESULT_FROM_WIN32(5):   // ERROR_ACCESS_DENIED - NB: identical to E_ACCESSDENIED, see above
    case __HRESULT_FROM_WIN32(32):  // ERROR_SHARING_VIOLATION
    case __HRESULT_FROM_WIN32(33):  // ERROR_LOCK_VIOLATION
        TRACE_WARNING("[GAME SAVE] Local save file unavailable during sync (HR:0x%0.8x), reporting E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE", hr);
        return E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE;
    default:
        return hr;
    }
}

HRESULT EnsureGameStorageMarker(_In_ const String& saveFolder)
{
    // On platforms with separate metadata storage, write a sentinel marker into
    // game storage so we can detect if the game container is deleted externally.
    SharedPtr<GameSaveGlobalState> globalState;
    if (FAILED(GameSaveGlobalState::Get(globalState)))
    {
        TRACE_ERROR("[GAME SAVE] EnsureGameStorageMarker: Failed to get global state");
        return E_FAIL;
    }
        
    if(!globalState->ApiProvider().HasSeparateMetadataStorage())
    {
        return S_OK; // Not applicable on this platform
    }

    String markerFolder, markerPath;
    if (FAILED(JoinPathHelper(saveFolder, PFGS_GAME_STORAGE_MARKER_FOLDER, markerFolder)) ||
        FAILED(JoinPathHelper(markerFolder, PFGS_GAME_STORAGE_MARKER_FILENAME, markerPath)))
    {
        TRACE_ERROR("[GAME SAVE] EnsureGameStorageMarker: Failed to construct marker file path");
        return E_FAIL;
    }

    RETURN_IF_FAILED(FilePAL::CreatePath(markerFolder));
    if (!FilePAL::DoesFileExist(markerPath))
    {
        Vector<char> markerData = { '1' };
        HRESULT markerHr = WriteEntireFile(markerPath, markerData);
        if (FAILED(markerHr))
        {
            TRACE_ERROR("[GAME SAVE] EnsureGameStorageMarker: Failed to write marker file hr=0x%08X", markerHr);
            return markerHr;
        }
        TRACE_INFORMATION("[GAME SAVE] EnsureGameStorageMarker: Created marker file at %s", markerPath.c_str());
    }

    return S_OK;
}

bool GetForceOutOfStorageError()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        return globalState->GetForceOutOfStorageError();
    }
    return false;
}

int64_t GetDebugManifestOffset()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        return globalState->GetDebugManifestOffset();
    }
    return 0;
}

bool GetForceSyncFailedError()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        return globalState->GetForceSyncFailedError();
    }
    return false;
}

bool GetForceNullPendingManifest()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        return globalState->GetForceNullPendingManifest();
    }
    return false;
}

void ClearForceNullPendingManifest()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        globalState->SetForceNullPendingManifest(false);
    }
}

bool GetWriteManifestsToDisk()
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        return globalState->GetWriteManifestsToDisk();
    }

    return false;
}

String GetLocalDeviceID(const String& saveFolder)
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        String deviceIdOverride = globalState->GetDebugDeviceIdOverride();
        if (!deviceIdOverride.empty())
        {
            return deviceIdOverride;
        }

        // Check if there's a mem cache of the device ID yet
        String deviceId = globalState->GetLocalDeviceID();
        if (!deviceId.empty())
        {
            return deviceId;
        }

        // If no mem cache, try to read info.json from disk
        String folderPath, filePath;
        if (SUCCEEDED(JoinPathHelper(saveFolder, "cloudsync", folderPath)) &&
            SUCCEEDED(JoinPathHelper(folderPath, "info.json", filePath)))
        {
            InfoManifestData data;
            if (SUCCEEDED(InfoManifestData::ReadInfoManifest(filePath, data)))
            {
                // Store in mem cache to avoid needing to read the file again
                globalState->SetLocalDeviceID(data.deviceId);
                return data.deviceId;
            }

            // Reading info.json failed - try to create directory and write
            if (SUCCEEDED(FilePAL::CreatePath(folderPath)))
            {
                data.deviceId = CreateGUID();
                HRESULT writeHr = InfoManifestData::WriteInfoManifest(filePath, data);
                if (FAILED(writeHr))
                {
                    TRACE_WARNING("[GAME SAVE] GetLocalDeviceID: WriteInfoManifest failed hr=0x%08X", writeHr);
                }

                globalState->SetLocalDeviceID(data.deviceId);
                return data.deviceId;
            }
        }

        // If we couldn't read or write info.json (e.g., game storage not yet prepared
        // for writes on this platform), generate a device ID and cache it in memory.
        // It will be persisted to info.json when storage becomes writable.
        String fallbackId = CreateGUID();
        TRACE_WARNING("[GAME SAVE] GetLocalDeviceID: Unable to read/write info.json, caching device ID in memory");
        globalState->SetLocalDeviceID(fallbackId);
        return fallbackId;
    }

    assert(false);
    return CreateGUID();
}

void EnsureDeviceIdPersisted(const String& saveFolder)
{
    SharedPtr<GameSaveGlobalState> globalState;
    if (FAILED(GameSaveGlobalState::Get(globalState)))
    {
        return;
    }

    String deviceId = globalState->GetLocalDeviceID();
    if (deviceId.empty())
    {
        return;
    }

    String folderPath, filePath;

    if (SUCCEEDED(JoinPathHelper(saveFolder, "cloudsync", folderPath)) &&
        SUCCEEDED(FilePAL::CreatePath(folderPath)) &&
        SUCCEEDED(JoinPathHelper(folderPath, "info.json", filePath)))
    {
        InfoManifestData data;
        if (FAILED(InfoManifestData::ReadInfoManifest(filePath, data)))
        {
            // info.json doesn't exist yet - persist the cached device ID
            data.deviceId = deviceId;
            HRESULT writeHr = InfoManifestData::WriteInfoManifest(filePath, data);
            if (FAILED(writeHr))
            {
                TRACE_WARNING("[GAME SAVE] EnsureDeviceIdPersisted: WriteInfoManifest failed hr=0x%08X", writeHr);
            }
        }
    }
}

PlayFab::GameSaveWrapper::ManifestStatus ConvertToManifestStatusEnum(String str)
{
    if (str == "Initialized") return PlayFab::GameSaveWrapper::ManifestStatus::Initialized;
    else if (str == "Uploading") return PlayFab::GameSaveWrapper::ManifestStatus::Uploading;
    else if (str == "Finalized") return PlayFab::GameSaveWrapper::ManifestStatus::Finalized;
    else if (str == "Quarantined") return PlayFab::GameSaveWrapper::ManifestStatus::Quarantined;
    else return PlayFab::GameSaveWrapper::ManifestStatus::PendingDeletion; // return if "PendingDeletion" or unknown
}

String ConvertToManifestStatusString(PlayFab::GameSaveWrapper::ManifestStatus n)
{
    switch (n)
    {
        case PlayFab::GameSaveWrapper::ManifestStatus::Initialized: return "Initialized";
        case PlayFab::GameSaveWrapper::ManifestStatus::Uploading: return "Uploading";
        case PlayFab::GameSaveWrapper::ManifestStatus::Finalized: return "Finalized";
        case PlayFab::GameSaveWrapper::ManifestStatus::Quarantined: return "Quarantined";
        default:
            [[fallthrough]];
        case PlayFab::GameSaveWrapper::ManifestStatus::PendingDeletion: return "PendingDeletion";
    }
}

int64_t StringToInt64(String str)
{
    try 
    {
        if (str.length() == 0)
        {
            return 0;
        }
        int64_t num = std::stoll(str.c_str());
        return num;
    } 
    catch (...) 
    {
        return 0;
    }
}

uint64_t StringToUint64(String str)
{
    try 
    {
        if (str.length() == 0)
        {
            return 0;
        }
        uint64_t num = std::stoull(str.c_str());
        return num;
    } 
    catch (...) 
    {
        return 0;
    }
}

PlayFab::String Uint64ToString(uint64_t n)
{
    return FormatString("%llu", n);
}

ScopeTracer::ScopeTracer(const String& traceMessage) :
    m_traceMessage(traceMessage)
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[GAME SAVE] [ThreadID %s] %s enter", threadIdStream.str().c_str(), m_traceMessage.c_str());
}

ScopeTracer::~ScopeTracer()
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[GAME SAVE] [ThreadID %s] %s exit", threadIdStream.str().c_str(), m_traceMessage.c_str());
}

#if defined(_DEBUG)
void SingleThreadProviderValidation::Set()
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    m_activeThreadId = threadIdStream.str();
    m_pendingScheduleThreadId = "";
}

void SingleThreadProviderValidation::Clear()
{
    m_activeThreadId = "";
}

void SingleThreadProviderValidation::AssertUponSchedule()
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    assert(m_pendingScheduleThreadId == "");
    m_pendingScheduleThreadId = threadIdStream.str();
    assert(m_activeThreadId == m_pendingScheduleThreadId || m_activeThreadId == "");
}

SingleThreadProviderValidationScope::SingleThreadProviderValidationScope(SingleThreadProviderValidation& singleThreadProvider) :
    m_singleThreadProvider(singleThreadProvider)
{
    m_singleThreadProvider.Set();
}

SingleThreadProviderValidationScope::~SingleThreadProviderValidationScope()
{
    m_singleThreadProvider.Clear();
}
#endif

} // namespace GameSave
} // namespace PlayFab
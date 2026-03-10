#include "stdafx.h"
#include "ApiHelpers.h"
#include "InfoManifest.h"

namespace PlayFab
{
namespace GameSave
{

static uint64_t internal_seed = 0;
uint32_t internal_rand(void)
{
    if (internal_seed == 0)
    {
        internal_seed = static_cast<uint64_t>(std::time(nullptr));
    }
    internal_seed = internal_seed * 1103515245 + 12345;
    return (uint32_t)(internal_seed / 65536) % 32768;
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
        // Ignore missing/empty files
        fileBuffer.resize(0);
        return S_OK;
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
    fileBuffer.resize(fileSize);

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
        const String& deviceId = globalState->GetLocalDeviceID();
        if (!deviceId.empty())
        {
            return deviceId;
        }

        // If no mem cache, then read info.json
        String folderPath, filePath;
        if (SUCCEEDED(JoinPathHelper(saveFolder, "cloudsync", folderPath)))
        {
            if (SUCCEEDED(FilePAL::CreatePath(folderPath)))
            {
                if (SUCCEEDED(JoinPathHelper(folderPath, "info.json", filePath)))
                {
                    InfoManifestData data;
                    if (SUCCEEDED(InfoManifestData::ReadInfoManifest(filePath, data)))
                    {
                        // Store in mem cache to avoid needing to reading the file again
                        globalState->SetLocalDeviceID(data.deviceId);
                        return data.deviceId;
                    }
                    else
                    {
                        // If reading info.json failed, create info.json
                        data.deviceId = CreateGUID();
                        InfoManifestData::WriteInfoManifest(filePath, data);

                        globalState->SetLocalDeviceID(data.deviceId);
                        return data.deviceId;
                    }
                }
            }
        }
    }

    assert(false);
    return CreateGUID();
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
    return FormatString("%lld", n);
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

#if _DEBUG
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
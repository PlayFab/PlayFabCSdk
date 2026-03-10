#include "stdafx.h"
#include "GameSaveServiceMock.h"
#include "ApiHelpers.h"
#include "ZipUtils.h"
#include "Platform/Platform.h"
#include "ArchiveOperations.h"
#include "Generated/GameSaveTypes.h"
#include "Generated/GameSave.h"

using namespace PlayFab::GameSave;

namespace PlayFab
{
namespace GameSaveWrapper
{

class ArchiveUploadMockProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    ArchiveUploadMockProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<ArchiveContext> archiveContext, const String filePath) :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_filePath{ std::move(filePath) },
        m_archiveContext{ archiveContext }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        TRACE_INFORMATION("ArchiveUploadMockProvider Begin");
        auto fileResult = FilePAL::OpenFile(m_filePath, FileOpenMode::Write);
        RETURN_IF_FAILED(fileResult.hr);

        m_fileHandle = fileResult.ExtractPayload();
        return Schedule(0);
    }

    HRESULT DoWork(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        TRACE_INFORMATION("ArchiveUploadMockProvider DoWork");

        String blockContent{};
        size_t uncompressedBytesWritten{};
        bool finishedCompressing{};
        auto hr = m_archiveContext->CompressBytesGetAnyData(blockContent, &uncompressedBytesWritten, &finishedCompressing);

        // If we're waiting for more data, schedule the provider to run again
        if (hr == E_PENDING)
        {
            RETURN_IF_FAILED(Schedule(0));
            return hr;
        }

        if (FAILED(hr))
        {
            TRACE_INFORMATION("ArchiveUploadMockProvider DoWork HR:0x%0.8x", hr);
            Fail(hr);
            return hr;
        }

        // Done uploading blocks
        if (finishedCompressing && blockContent.empty())
        {
            TRACE_INFORMATION("ArchiveUploadMockProvider done uploading blocks");
            FilePAL::CloseFile(m_fileHandle);
            Complete(0);
            return E_PENDING;
        }

        if (blockContent.empty())
        {
            TRACE_INFORMATION("ArchiveUploadMockProvider empty");
            Fail(E_FAIL);
            return E_FAIL;
        }

        TRACE_INFORMATION("ArchiveUploadMockProvider WriteFileBytes");
        RETURN_IF_FAILED(FilePAL::WriteFileBytes(m_fileHandle, blockContent.c_str(), blockContent.size()));
        RETURN_IF_FAILED(Schedule(0));

        return E_PENDING;
    }

private:
    const String m_filePath;
    FileHandle m_fileHandle;
    SharedPtr<ArchiveContext> m_archiveContext;
};

class HttpArchiveUploadMockOp : public XAsyncOperation<void>
{
public:
    HttpArchiveUploadMockOp(
        SharedPtr<ArchiveContext> archiveContext,
        const String filePath,
        PlayFab::RunContext runContext
    ) noexcept :
        XAsyncOperation<void>{ std::move(runContext) },
        m_filePath{ filePath },
        m_archiveContext{ archiveContext }
    {
    }

    HttpArchiveUploadMockOp(HttpArchiveUploadMockOp const&) = delete;
    HttpArchiveUploadMockOp& operator=(HttpArchiveUploadMockOp) = delete;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        auto provider = MakeUnique<ArchiveUploadMockProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(HttpArchiveUploadMockOp), m_archiveContext, std::move(m_filePath));
        return XAsyncProviderBase::Run(std::move(provider));
    }

    const String m_filePath;
    SharedPtr<ArchiveContext> m_archiveContext;
};

AsyncOp<FinalizeManifestResponse> GameSaveServiceMock::FinalizeManifest(
    Entity const& entity,
    const FinalizeManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::FinalizeManifest)
    {
        return AsyncOp<FinalizeManifestResponse>(E_FAIL);
    }

    // Mock with a file
    // Real solution will be just a service call, no file.
    String manifestsFile = "manifests.json";
    String manifestsPath = GetMockGlobalPath(entity, manifestsFile);

    ListManifestsResponse response = ReadManifestsFile(manifestsPath);
    Vector<ManifestWrap> manifests;
    for (size_t iManifest = 0; iManifest < response.GetManifests().size(); iManifest++)
    {
        // update manifest manually
        const ManifestWrap& manifestOld = response.GetManifests()[iManifest];
        if (manifestOld.GetVersion() == request.GetVersion())
        {
            PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator>, Allocator> filesToRequestList;
            filesToRequestList = request.GetFilesToFinalize();
            ManifestWrap manifestNew;
            FileMetadataWrapVector fileMetadataList;
            for (size_t iFile = 0; iFile < filesToRequestList.size(); iFile++)
            {
                PlayFab::Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator> fileToRequest = filesToRequestList[iFile];
                String fileStr = fileToRequest.GetFileName();
                FileMetadataWrap fileWrap;
                fileWrap.SetFileName(std::move(fileStr));
                fileMetadataList.push_back(std::move(fileWrap));
            }
            manifestNew.SetFiles(std::move(fileMetadataList));
            manifestNew.SetStatus(ConvertToManifestStatusString(ManifestStatus::Finalized));
            manifestNew.SetVersion(manifestOld.GetVersion());
            manifestNew.SetCreationTimestamp(manifestOld.GetCreationTimestamp());
            manifestNew.SetFinalizationTimestamp(manifestOld.GetFinalizationTimestamp());
            manifestNew.SetLastUpdateTimestamp(manifestOld.GetLastUpdateTimestamp());
            manifestNew.SetMetadata(manifestOld.GetMetadata());
            manifestNew.SetUploadProgress(manifestOld.GetUploadProgress());

            manifests.push_back(std::move(manifestNew));
        }
        else
        {
            manifests.push_back(manifestOld);
        }
    }

    WriteManifestsFile(manifestsPath, manifests);
    return GameSaveServiceMock::UploadFileToCloud(rc, manifestsFile, manifestsPath)
    .Then([request, manifests](Result<void> result) -> AsyncOp<FinalizeManifestResponse>
    {
        UNREFERENCED_PARAMETER(result);

        FinalizeManifestResponse r{};
        ManifestWrap manifestNew{};
        for (auto& manifest : manifests)
        {
            if (manifest.GetVersion() == request.GetVersion())
            {
                manifestNew = manifest;
                r.SetManifest(std::move(manifestNew));
            }
        }
        return AsyncOp<FinalizeManifestResponse>(std::move(r));
    });
}

AsyncOp<GetQuotaForPlayerResponse> GameSaveServiceMock::GetQuotaForPlayer(
    Entity const& entity,
    const GetQuotaForPlayerRequest& request,
    RunContext rc)
{
    UNREFERENCED_PARAMETER(rc);
    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(request);

    GetQuotaForPlayerResponse r{};
    r.SetAvailableBytes("1000000000");
    r.SetTotalBytes("1000000000");
    return AsyncOp<GetQuotaForPlayerResponse>(std::move(r));
}

AsyncOp<GetManifestDownloadDetailsResponse> GameSaveServiceMock::GetManifestDownloadDetails(
    Entity const& entity,
    const GetManifestDownloadDetailsRequest& request,
    RunContext rc)
{
    UNREFERENCED_PARAMETER(rc);
    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(request);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::GetManifestDownloadDetails)
    {
        return AsyncOp<GetManifestDownloadDetailsResponse>(E_FAIL);
    }

    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    assert(!rootFolder.empty());
    if (rootFolder.empty())
    {
        return AsyncOp<GetManifestDownloadDetailsResponse>(E_FAIL);
    }
    FilePAL::CreatePath(rootFolder);

    Result<Vector<String>> filesInFolderResult = FilePAL::EnumFiles(rootFolder);
    RETURN_IF_FAILED(filesInFolderResult.hr);
    Vector<String> filesInFolder = filesInFolderResult.ExtractPayload();
    Vector<String> fullPaths;

    GetManifestDownloadDetailsResponse r{};
    DownloadDetailsWrapVector files;
    for (const String& file : filesInFolder)
    {
        String fullPath;
        JoinPathHelper(rootFolder, file, fullPath);
        fullPaths.push_back(std::move(fullPath));
        String downloadUrl = fullPaths[fullPaths.size() - 1].c_str();

        DownloadDetailsWrap fileDownloadDetail;
        fileDownloadDetail.SetFileName(file);
        fileDownloadDetail.SetDownloadUrl(downloadUrl);
        files.push_back(std::move(fileDownloadDetail));
    }

    r.SetFiles(std::move(files));
    return AsyncOp<GetManifestDownloadDetailsResponse>(std::move(r));
}

AsyncOp<InitializeManifestResponse> GameSaveServiceMock::InitializeManifest(
    Entity const& entity,
    const InitializeManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::InitializeManifest)
    {
        return AsyncOp<InitializeManifestResponse>(E_FAIL);
    }

    // Mock with a file
    // Real solution will be just a service call, no file.
    String manifestsFile = "manifests.json";
    String manifestsPath = GetMockGlobalPath(entity, manifestsFile);

    ListManifestsResponse response = ReadManifestsFile(manifestsPath);
    ManifestWrap manifestToReturn{};
    ManifestWrap manifest{};
    manifest.SetVersion(request.GetVersion());
    manifest.SetStatus(ConvertToManifestStatusString(ManifestStatus::Initialized));

    PlayFab::Wrappers::PFGameSaveManifestMetadataWrapper<Allocator> metadata;
    metadata.SetDeviceId(request.GetMetadata().GetDeviceId());
    metadata.SetDeviceName(request.GetMetadata().GetDeviceName());
    metadata.SetDeviceType(request.GetMetadata().GetDeviceType());
    manifest.SetMetadata(std::move(metadata));

    PlayFab::Wrappers::PFGameSaveUploadProgressWrapper<Allocator> uploadProgress;
    uploadProgress.SetTotalBytes("0");
    uploadProgress.SetUploadedBytes("0");
    manifest.SetUploadProgress(std::move(uploadProgress));

    manifest.SetCreationTimestamp(GetTimeTNow());
    manifest.SetFinalizationTimestamp(0);

    manifestToReturn = manifest;

    Vector<ManifestWrap> manifests;
    for (size_t i = 0; i < response.GetManifests().size(); i++)
    {
        const ManifestWrap& manifestOld = response.GetManifests()[i];
        manifests.push_back(manifestOld);
    }
    manifests.push_back(manifest);

    WriteManifestsFile(manifestsPath, manifests);

    return GameSaveServiceMock::UploadFileToCloud(rc, manifestsFile, manifestsPath)
    .Then([manifestToReturn, manifests, request](Result<void> result) -> AsyncOp<InitializeManifestResponse>
    {
        UNREFERENCED_PARAMETER(result);

        InitializeManifestResponse r{};
        ManifestWrap manifestNew{};
        for (auto& manifest : manifests)
        {
            if (manifest.GetVersion() == request.GetVersion())
            {
                manifestNew = manifest;
                r.SetManifest(std::move(manifestNew));
            }
        }
        return AsyncOp<InitializeManifestResponse>(std::move(r));
    });
}

AsyncOp<InitiateUploadResponse> GameSaveServiceMock::InitiateUpload(
    Entity const& entity,
    const InitiateUploadRequest& request,
    RunContext rc,
    const Vector<UploadFileDetail>& mockFilesRequest
)
{
    UNREFERENCED_PARAMETER(rc);
    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(request);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::InitiateUpload)
    {
        return AsyncOp<InitiateUploadResponse>(E_FAIL);
    }

    Vector<UploadFileDetail> files;
    for (const UploadFileDetail& mockFile : mockFilesRequest)
    {
        UploadFileDetail ud{};
        //ud.fileName = FormatString("%s%s", manifestVerString.c_str(), mockFile.fileName.c_str());
        ud.fileName = mockFile.fileName;
        ud.fullFilePath = mockFile.fullFilePath;
        files.push_back(std::move(ud));
    }

    InitiateUploadResponse r{};

    AllocatedFileWrapVector allocfiles;
    for (const UploadFileDetail& mockFile : files)
    {
        AllocatedFileWrap file;
        file.SetFileName(mockFile.fileName);
        file.SetUploadUrl(mockFile.fullFilePath);
        allocfiles.push_back(std::move(file));
    }
    r.SetFiles(std::move(allocfiles));

    return AsyncOp<InitiateUploadResponse>(std::move(r));
}

AsyncOp<ListManifestsResponse> GameSaveServiceMock::ListManifests(
    Entity const& entity,
    const ListManifestsRequest& request,
    RunContext rc)
{
    UNREFERENCED_PARAMETER(request);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::ListManifests)
    {
        return AsyncOp<ListManifestsResponse>(E_FAIL);
    }

    // Mock with a file
    // Real solution will be just a service call, no file.
    String manifestsFile = "manifests.json";
    String manifestsPath = GetMockGlobalPath(entity, manifestsFile);

    // Mock the file list with a file called files.json

    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    Result<Vector<String>> filesInFolderResult = FilePAL::EnumFiles(rootFolder);
    if (FAILED(filesInFolderResult.hr))
    {
        return AsyncOp<ListManifestsResponse>(filesInFolderResult.hr);
    }
    Vector<String> filesInFolder = filesInFolderResult.ExtractPayload();

    GetManifestDownloadDetailsResponse r{};
    for (const String& file : filesInFolder)
    {
        if (file == manifestsFile)
        {
            String fullPath;
            JoinPathHelper(rootFolder, file, fullPath);
            return GameSaveServiceMock::DownloadFileFromCloud(rc, manifestsPath, fullPath, nullptr)
            .Then([manifestsPath](Result<void> result)
            {
                if (FAILED(result.hr))
                {
                    return AsyncOp<ListManifestsResponse>(result.hr);
                }
                ListManifestsResponse response = ReadManifestsFile(manifestsPath);
                return AsyncOp<ListManifestsResponse>(std::move(response));
            });
        }
    }

    ListManifestsResponse result{};
    return AsyncOp<ListManifestsResponse>(std::move(result));
}

AsyncOp<UpdateManifestResponse> GameSaveServiceMock::UpdateManifest(
    Entity const& entity,
    const UpdateManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::UpdateManifest)
    {
        return AsyncOp<UpdateManifestResponse>(E_FAIL);
    }

    // Mock with a file
    // Real solution will be just a service call, no file.
    String manifestsFile = "manifests.json";
    String manifestsPath = GetMockGlobalPath(entity, manifestsFile);

    ListManifestsResponse response = ReadManifestsFile(manifestsPath);

    Vector<ManifestWrap> manifests;
    for (size_t i = 0; i < response.GetManifests().size(); i++)
    {
        const ManifestWrap& manifest = response.GetManifests()[i];
        if (manifest.GetVersion() == request.GetVersion())
        {
            ManifestWrap manifestNew;
            FileMetadataWrapVector fileMetadataList;
            const FileMetadataWrapVector& fileList = manifest.GetFiles();
            for (size_t iFile = 0; iFile < fileList.size(); iFile++)
            {
                FileMetadataWrap fileWrap;
                const FileMetadataWrap& file = fileList[iFile];
                fileWrap.SetFileName(file.GetFileName());
                fileMetadataList.push_back(std::move(fileWrap));
            }
            manifestNew.SetFiles(std::move(fileMetadataList));
            manifestNew.SetStatus(ConvertToManifestStatusString(ManifestStatus::Finalized));
            manifestNew.SetVersion(request.GetVersion());
            manifestNew.SetCreationTimestamp(0);
            manifestNew.SetFinalizationTimestamp(0);
            manifestNew.SetLastUpdateTimestamp(0);

            PlayFab::Wrappers::PFGameSaveUploadProgressWrapper<Allocator> uploadProgress;
            if (request.GetUploadProgress().has_value())
            {
                uploadProgress.SetUploadedBytes(request.GetUploadProgress().value().GetUploadedBytes());
                uploadProgress.SetTotalBytes(request.GetUploadProgress().value().GetTotalBytes());
            }
            else
            {
                uploadProgress.SetUploadedBytes("0");
                uploadProgress.SetTotalBytes("0");
            }
            manifestNew.SetUploadProgress(std::move(uploadProgress));

            manifests.push_back(manifest);
        }
        else
        {
            manifests.push_back(manifest);
        }
    }

    WriteManifestsFile(manifestsPath, manifests);
    return GameSaveServiceMock::UploadFileToCloud(rc, manifestsFile, manifestsPath)
    .Then([request, manifests](Result<void> result) -> AsyncOp<UpdateManifestResponse>
    {
        UNREFERENCED_PARAMETER(result);

        UpdateManifestResponse r{};
        ManifestWrap manifestNew{};
        for (auto& manifest : manifests)
        {
            if (manifest.GetVersion() == request.GetVersion())
            {
                manifestNew = manifest;
                r.SetManifest(std::move(manifestNew));
            }
        }
        return AsyncOp<UpdateManifestResponse>(std::move(r));
    });
}

String GameSaveServiceMock::GetMockGlobalPath(const Entity& entity, const String& file)
{
    SharedPtr<GameSaveGlobalState> state;
    HRESULT hr = GameSaveGlobalState::Get(state);
    if (SUCCEEDED(hr))
    {
        String root = state->GetDebugRootFolderOverride();
        String pathA, pathB, pathC;
        if (SUCCEEDED(JoinPathHelper(root, "MockPFGameSave", pathA)))
        {
            if (SUCCEEDED(JoinPathHelper(pathA, entity.EntityKey().Model().id, pathB)))
            {
                FilePAL::CreatePath(pathB);
                if (SUCCEEDED(JoinPathHelper(pathB, file, pathC)))
                {
                    return pathC;
                }
            }
        }
    }

    return String();
}

ListManifestsResponse GameSaveServiceMock::ReadManifestsFile(const String& manifestsPath)
{
    ListManifestsResponse response{};

    Vector<char> fileData;
    ReadEntireFile(manifestsPath, fileData);
    fileData.push_back(0);
    JsonValue responseJson;
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        String fileString(fileData.data(), fileData.size());
        bool isEmpty = (std::all_of(fileString.begin(), fileString.end(), [](char c) { return c == '\0'; }));
        responseJson = !isEmpty ? JsonValue::parse(fileString) : "";
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
    }

    if (!parseError && responseJson.contains("Manifests"))
    {
        auto& manifestsJson = responseJson["Manifests"];
        if (manifestsJson.is_array() && manifestsJson.size() > 0)
        {
            ManifestWrapVector manifestList;

            for (auto& manifestJson : manifestsJson.get<Vector<JsonValue>>())
            {
                ManifestWrap n;
                String dateStr;

                String str;
                JsonUtils::ObjectGetMember(manifestJson, "Version", str);
                n.SetVersion(std::move(str));

                PlayFab::Wrappers::PFGameSaveManifestMetadataWrapper<Allocator> metadata;
                JsonUtils::ObjectGetMember(manifestJson, "DeviceId", str);
                metadata.SetDeviceId(std::move(str));

                JsonUtils::ObjectGetMember(manifestJson, "DeviceName", str);
                metadata.SetDeviceName(std::move(str));

                JsonUtils::ObjectGetMember(manifestJson, "DeviceType", str);
                metadata.SetDeviceType(std::move(str));
                n.SetMetadata(std::move(metadata));

                JsonUtils::ObjectGetMember(manifestJson, "Status", str);
                n.SetStatus(std::move(str));

                PlayFab::Wrappers::PFGameSaveUploadProgressWrapper<Allocator> uploadProgress;
                JsonUtils::ObjectGetMember(manifestJson, "UploadProgressBytesUploaded", str);
                uploadProgress.SetUploadedBytes(std::move(str));

                JsonUtils::ObjectGetMember(manifestJson, "UploadProgressTotalBytes", str);
                uploadProgress.SetTotalBytes(std::move(str));
                n.SetUploadProgress(std::move(uploadProgress));

                JsonUtils::ObjectGetMember(manifestJson, "CreationTimestamp", str);
                n.SetCreationTimestamp(Iso8601StringToTimeT(str));

                JsonUtils::ObjectGetMember(manifestJson, "FinalizationTimestamp", str);
                n.SetFinalizationTimestamp(Iso8601StringToTimeT(str));

                if (manifestJson.contains("Files"))
                {
                    auto& manifestsFilesJson = manifestJson["Files"];
                    if (manifestsFilesJson.is_array() && manifestsFilesJson.size() > 0)
                    {
                        FileMetadataWrapVector files;
                        for (auto& manifestsFileJson : manifestsFilesJson.get<Vector<JsonValue>>())
                        {
                            FileMetadataWrap file;
                            JsonUtils::ObjectGetMember(manifestsFileJson, "FileName", str);
                            file.SetFileName(std::move(str));

                            files.push_back(std::move(file));
                        }
                        n.SetFiles(std::move(files));
                    }
                }

                manifestList.push_back(std::move(n));
            }

            response.SetManifests(std::move(manifestList));
        }
    }

    return response;
}

void GameSaveServiceMock::WriteManifestsFile(const String& manifestsPath, const Vector<ManifestWrap>& manifests)
{
    JsonValue fileJsonArray = JsonValue::array();
    for (const ManifestWrap& manifest : manifests)
    {
        JsonValue jsonObj = JsonValue::object();
        JsonUtils::ObjectAddMember(jsonObj, "Version", manifest.GetVersion());
        JsonUtils::ObjectAddMember(jsonObj, "DeviceId", (manifest.GetMetadata().has_value()) ? manifest.GetMetadata().value().GetDeviceId() : "Unknown");
        JsonUtils::ObjectAddMember(jsonObj, "DeviceName", (manifest.GetMetadata().has_value()) ? manifest.GetMetadata().value().GetDeviceName() : "Unknown");
        JsonUtils::ObjectAddMember(jsonObj, "DeviceType", (manifest.GetMetadata().has_value()) ? manifest.GetMetadata().value().GetDeviceType() : "Unknown");
        JsonUtils::ObjectAddMember(jsonObj, "Status", manifest.GetStatus());
        JsonUtils::ObjectAddMember(jsonObj, "UploadProgressBytesUploaded", (manifest.GetUploadProgress().has_value()) ? manifest.GetUploadProgress().value().GetUploadedBytes() : "0");
        JsonUtils::ObjectAddMember(jsonObj, "UploadProgressTotalBytes", (manifest.GetUploadProgress().has_value()) ? manifest.GetUploadProgress().value().GetTotalBytes() : "0");
        JsonUtils::ObjectAddMember(jsonObj, "CreationTimestamp", TimeTToIso8601String(manifest.GetCreationTimestamp()));
        JsonUtils::ObjectAddMember(jsonObj, "FinalizationTimestamp", TimeTToIso8601String((manifest.GetFinalizationTimestamp().has_value()) ? manifest.GetFinalizationTimestamp().value() : 0));
        JsonValue fileListJsonArray = JsonValue::array();
        for (size_t iFile = 0; iFile < manifest.GetFiles().size(); iFile++)
        {
            const FileMetadataWrap& file = manifest.GetFiles()[iFile];
            JsonValue jsonFileObj = JsonValue::object();
            JsonUtils::ObjectAddMember(jsonFileObj, "FileName", file.GetFileName());
            fileListJsonArray.push_back(jsonFileObj);
        }
        JsonUtils::ObjectAddMember(jsonObj, "Files", std::move(fileListJsonArray));

        fileJsonArray.push_back(jsonObj);
    }
    JsonValue lsJson = JsonValue::object();
    JsonUtils::ObjectAddMember(lsJson, "Manifests", std::move(fileJsonArray));

    String str = JsonUtils::WriteToString(lsJson);
    Vector<char> vData;
    std::copy(str.begin(), str.end(), std::back_inserter(vData));
    WriteEntireFile(manifestsPath, vData);
}

String GameSaveServiceMock::GetMockDataFolder()
{
    SharedPtr<GameSaveGlobalState> state;
    if (SUCCEEDED(GameSaveGlobalState::Get(state)))
    {
        return state->GetDebugMockDataOverride();
    }
    else
    {
        return "";
    }
}

AsyncOp<void> GameSaveServiceMock::DownloadFileFromCloud(RunContext runContext, const String& filePath, const String& downloadUrl, SharedPtr<ArchiveContext> archiveContext)
{
    UNREFERENCED_PARAMETER(runContext);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::DownloadFile)
    {
        return AsyncOp<void>(E_FAIL);
    }

    if (archiveContext)
    {
        RETURN_IF_FAILED(ZipUtils::UnzipFilesFromSingleZip(archiveContext, downloadUrl));
    }
    else
    {
        Vector<char> fileData;
        RETURN_IF_FAILED(ReadEntireFile(downloadUrl, fileData));
        WriteEntireFile(filePath, fileData);
    }

    return AsyncOp<void>(S_OK);
}

AsyncOp<void> GameSaveServiceMock::UploadFileFromStringToCloud(RunContext runContext, const String& fileName, const String& fileContent)
{
    UNREFERENCED_PARAMETER(runContext);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::UploadFile)
    {
        return AsyncOp<void>(E_FAIL);
    }

    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    String fullPath;
    if (SUCCEEDED(JoinPathHelper(rootFolder, fileName, fullPath)))
    {
        // Check if upload is paused
        {
            std::unique_lock<std::mutex> lock(m_pauseMutex);
            while (m_paused)
            {
                TRACE_INFORMATION("[GAME SAVE] UploadFileToCloud: Waiting for lock");
                m_pauseCondition.wait(lock);
            }
        }

        Vector<char> fileData(fileContent.begin(), fileContent.end());
        WriteEntireFile(fullPath, fileData);
    }

    return AsyncOp<void>(S_OK);
}

AsyncOp<void> GameSaveServiceMock::CompressAndUploadFileToCloud(RunContext runContext, const UploadFileDetail& fileDetail)
{
    auto subRequestOp = MakeUnique<HttpArchiveUploadMockOp>(
        fileDetail.archiveContext,
        fileDetail.fullFilePath,
        runContext.Derive()
    );

    auto compressionOp = MakeUnique<ArchiveStreamCompressionOp>(
        fileDetail.archiveContext,
        true,
        runContext.Derive()
    );

    Vector<AsyncOp<void>> operations;
    operations.push_back(RunOperation(std::move(subRequestOp)));
    operations.push_back(RunOperation(std::move(compressionOp)));

    // Combine the operations to run them in parallel and continue after they both complete
    return CombinedOp::MakeCombinedWithHttpResult(std::move(operations), runContext).Then([fileName = fileDetail.fileName, filePath = fileDetail.fullFilePath, runContext](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED(result.hr);

        return GameSaveServiceMock::UploadFileToCloud(runContext, fileName, filePath);
    });
}

AsyncOp<void> GameSaveServiceMock::UploadFileToCloud(RunContext runContext, const String& fileName, const String& filePath)
{
    UNREFERENCED_PARAMETER(runContext);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::UploadFile)
    {
        return AsyncOp<void>(E_FAIL);
    }

    Vector<char> fileData;
    RETURN_IF_FAILED(ReadEntireFile(filePath, fileData));
    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    String fullPath;
    if (SUCCEEDED(JoinPathHelper(rootFolder, fileName, fullPath)))
    {
        // Check if upload is paused
        {
            std::unique_lock<std::mutex> lock(m_pauseMutex);
            while (m_paused)
            {
                TRACE_INFORMATION("[GAME SAVE] UploadFileToCloud: Waiting for lock");
                m_pauseCondition.wait(lock);
            }
        }

        WriteEntireFile(fullPath, fileData);
    }

    return AsyncOp<void>(S_OK);
}

void GameSaveServiceMock::PauseUpload()
{
    std::unique_lock<std::mutex> lock(m_pauseMutex);
    m_paused = true;
}

void GameSaveServiceMock::ResumeUpload()
{
    {
        std::unique_lock<std::mutex> lock(m_pauseMutex);
        m_paused = false;
    }
    m_pauseCondition.notify_all();
}

void GameSaveServiceMock::SetForcedOffline(GameSaveServiceMockForcedOffline forcedOffline)
{
    m_forcedOffline = forcedOffline;
}

} // namespace GameSave
} // namespace PlayFab

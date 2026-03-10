#include "stdafx.h"
#include "GameSaveServiceMock.h"
#include "ApiHelpers.h"
#include "ArchiveOperations.h"
#include "HttpFileUploadRequest.h"
#include "HttpFileDownloadRequest.h"
#include "HttpArchiveUploadRequest.h"
#include "HttpArchiveDownloadRequest.h"
#include "Generated/GameSaveTypes.h"
#include "Generated/GameSave.h"

using namespace PlayFab::GameSave;

namespace PlayFab
{
namespace GameSaveWrapper
{

bool GameSaveServiceSelector::useMocks{ false };

AsyncOp<FinalizeManifestResponse> GameSaveServiceSelector::FinalizeManifest(
    Entity const& entity,
    const FinalizeManifestRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::FinalizeManifest(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::FinalizeManifest(entity, request, rc);
    }
}

AsyncOp<GetManifestDownloadDetailsResponse> GameSaveServiceSelector::GetManifestDownloadDetails(
    Entity const& entity,
    const GetManifestDownloadDetailsRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::GetManifestDownloadDetails(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::GetManifestDownloadDetails(entity, request, rc);
    }
}

AsyncOp<GetQuotaForPlayerResponse> GameSaveServiceSelector::GetQuotaForPlayer(
    Entity const& entity,
    const GetQuotaForPlayerRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::GetQuotaForPlayer(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::GetQuotaForPlayer(entity, request, rc);
    }
}

AsyncOp<InitializeManifestResponse> GameSaveServiceSelector::InitializeManifest(
    Entity const& entity,
    const InitializeManifestRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::InitializeManifest(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::InitializeManifest(entity, request, rc);
    }
}

AsyncOp<InitiateUploadResponse> GameSaveServiceSelector::InitiateUpload(
    Entity const& entity,
    const InitiateUploadRequest& request,
    RunContext rc,
    const Vector<UploadFileDetail>& mockFilesRequest
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::InitiateUpload(entity, request, rc, mockFilesRequest);
    }
    else
    {
        // Call auto-gen service wrapper
        UNREFERENCED_PARAMETER(mockFilesRequest);
        return GameSaveAPI::InitiateUpload(entity, request, rc);
    }
}

AsyncOp<ListManifestsResponse> GameSaveServiceSelector::ListManifests(
    Entity const& entity,
    const ListManifestsRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::ListManifests(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::ListManifests(entity, request, rc);
    }
}

AsyncOp<UpdateManifestResponse> GameSaveServiceSelector::UpdateManifest(
    Entity const& entity,
    const UpdateManifestRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::UpdateManifest(entity, request, rc);
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::UpdateManifest(entity, request, rc);
    }
}

AsyncOp<Vector<char>> GameSaveServiceSelector::DownloadFileFromCloudToBytes(
    RunContext runContext,
    const String& downloadUrl,
    const String& mockFilePath
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::DownloadFileFromCloud(runContext, mockFilePath, downloadUrl, nullptr)
        .Then([mockFilePath](Result<void> result) -> Result<Vector<char>>
        {
            RETURN_IF_FAILED(result.hr);

            Vector<char> fileData;
            RETURN_IF_FAILED(ReadEntireFile(mockFilePath, fileData));
            return Result<Vector<char>>(std::move(fileData));
        });
    }
    else
    {
        assert(!downloadUrl.empty());

        UnorderedMap<String, String> headers;
        headers["x-ms-blob-type"] = "BlockBlob";

        auto requestOp = MakeUnique<HCHttpCall>(
            "GET",
            downloadUrl,
            headers,
            "",                             // no request body
            runContext.Derive()
        );
        requestOp->SetIsPlayfabCall(false);

        return RunOperation(std::move(requestOp))
        .Then([](Result<ServiceResponse> result) -> Result<Vector<char>>
        {
            RETURN_IF_FAILED(result.hr);

            Vector<char> bytes = result.Payload().ResponseBody;
            return Result<Vector<char>>(std::move(bytes));
        });
    }
}


AsyncOp<void> GameSaveServiceSelector::DownloadFileFromCloud(
    RunContext runContext,
    const DownloadFileDetail& fileDetail,
    const String& downloadUrl,
    HCHttpCallProgressReportFunction progressCallback,
    void* progressCallbackContext,
    const uint64_t customTotalSizeBytes,
    const uint64_t customCurrentSizeBytes
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::DownloadFileFromCloud(runContext, fileDetail.fullFilePath, downloadUrl, fileDetail.archiveContext);
    }
    else
    {
        assert(!fileDetail.fullFilePath.empty());
        assert(!downloadUrl.empty());

        UnorderedMap<String, String> headers;
        headers["x-ms-blob-type"] = "BlockBlob";

        // If the file is an archive, kick off operations to download and decompress the archive
        if (fileDetail.archiveContext)
        {
            auto subRequestOp = MakeUnique<HCHttpArchiveDownloadCall>(
                "GET",
                downloadUrl,
                headers,
                "",                             // no request body
                fileDetail.archiveContext,
                runContext
            );

            if (progressCallback)
            {
                subRequestOp->SetProgressReportCallback(false, progressCallbackContext, progressCallback);
            }

            if (customTotalSizeBytes > 0)
            {
                subRequestOp->SetDynamicSize(customTotalSizeBytes, customCurrentSizeBytes);
            }

            auto compressionOp = MakeUnique<ArchiveStreamCompressionOp>(
                fileDetail.archiveContext,
                false,
                runContext
            );

            Vector<AsyncOp<void>> operations;
            auto requestOp = RunOperation(std::move(subRequestOp))
            .Then([archiveContext = fileDetail.archiveContext](Result<ServiceResponse> result) -> AsyncOp<void>
            {
                return Result<void>{ result.hr, std::move(result.httpResult) };
            });
            operations.push_back(std::move(requestOp));
            operations.push_back(RunOperation(std::move(compressionOp)));

            // Combine the operations to run them in parallel and continue after they both complete
            return CombinedOp::MakeCombinedWithHttpResult(std::move(operations), runContext).Then([archiveContext = fileDetail.archiveContext](Result<void> result) -> AsyncOp<void>
            {
                archiveContext->Close();
                return Result<void>{ result.hr, std::move(result.httpResult) };
            });
        }

        auto requestOp = MakeUnique<HCHttpFileDownloadCall>(
            "GET",
            downloadUrl,
            headers,
            "",                             // no request body
            fileDetail.fullFilePath,
            runContext.Derive()
        );

        if (progressCallback)
        {
            requestOp->SetProgressReportCallback(false, progressCallbackContext, progressCallback);
        }

        if (customTotalSizeBytes > 0)
        {
            requestOp->SetDynamicSize(customTotalSizeBytes, customCurrentSizeBytes);
        }

        return RunOperation(std::move(requestOp))
        .Then([](Result<ServiceResponse> result) -> Result<void>
        {
            return Result<void>{ result.hr, std::move(result.httpResult) };
        });
    }
}

PlayFab::AsyncOp<void> GameSaveServiceSelector::UploadFileFromStringToCloud(
    RunContext runContext,
    const UploadFileDetail& fileDetail,
    const String& fileContent,
    const PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>, Allocator>& initiateResult
)
{
    if (useMocks)
    {
        return GameSaveServiceMock::UploadFileFromStringToCloud(runContext, fileDetail.fileName, fileContent);
    }

    String url;
    for (uint32_t i = 0; i < initiateResult.size(); i++)
    {
        auto& uploadDetail = initiateResult[i];
        if (uploadDetail.GetFileName() == fileDetail.fileName)
        {
            url = uploadDetail.GetUploadUrl();
            break;
        }
    }
    if (url.empty())
    {
        return E_UNEXPECTED;
    }

    UnorderedMap<String, String> headers;
    headers["x-ms-blob-type"] = "BlockBlob";

    auto requestOp = MakeUnique<HCHttpCall>(
        "PUT",
        url,
        headers,
        fileContent,
        runContext
    );
    requestOp->SetIsPlayfabCall(false);

    return RunOperation(std::move(requestOp))
    .Then([](Result<ServiceResponse> result) -> AsyncOp<void>
    {
        return result.hr;
    });
}

PlayFab::AsyncOp<void> GameSaveServiceSelector::UploadSingleFileToCloud(
    RunContext runContext,
    const UploadFileDetail& fileDetail,
    const PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>, Allocator>& initiateResult,
    HCHttpCallProgressReportFunction progressCallback,
    void* progressCallbackContext,
    const uint64_t customTotalSizeBytes,
    const uint64_t customCurrentSizeBytes
)
{
    if (useMocks)
    {
        TRACE_INFORMATION("[GAME SAVE] UploadStep: UploadSingleFileToCloud using mocks");
        return GameSaveServiceMock::UploadFileToCloud(runContext, fileDetail.fileName, fileDetail.fullFilePath);
    }

    String url;
    for (uint32_t i = 0; i < initiateResult.size(); i++)
    {
        auto& uploadDetail = initiateResult[i];
        if (uploadDetail.GetFileName() == fileDetail.fileName)
        {
            url = uploadDetail.GetUploadUrl();
            break;
        }
    }
    if (url.empty())
    {
        return E_UNEXPECTED;
    }

    TRACE_INFORMATION("[GAME SAVE] UploadStep: url %s", url.c_str());
    UnorderedMap<String, String> headers;
    headers["x-ms-blob-type"] = "BlockBlob";

    auto requestOp = MakeUnique<HCHttpFileUploadCall>(
        "PUT",
        url,
        headers,
        fileDetail.fullFilePath,
        runContext
    );

    if (progressCallback)
    {
        requestOp->SetProgressReportCallback(true, progressCallbackContext, progressCallback);
    }

    if (customTotalSizeBytes > 0)
    {
        requestOp->SetDynamicSize(customTotalSizeBytes, customCurrentSizeBytes);
    }

    TRACE_TASK("ServiceResponse");
    return RunOperation(std::move(requestOp))
    .Then([](Result<ServiceResponse> result) -> AsyncOp<void>
    {
        TRACE_TASK(FormatString("ServiceResponse HR:0x%0.8x", result.hr));
        return result.hr;
    });
}

AsyncOp<void> GameSaveServiceSelector::DeleteManifest(
    Entity const& entity,
    const DeleteManifestRequest& request,
    RunContext rc
)
{
    if (useMocks)
    {
        // TOOD: needed?
        UNREFERENCED_PARAMETER(rc);
        UNREFERENCED_PARAMETER(entity);
        UNREFERENCED_PARAMETER(request);
        return Result<void>{ S_OK };
    }
    else
    {
        // Call auto-gen service wrapper
        return GameSaveAPI::DeleteManifest(entity, request, rc);
    }
}


} // namespace GameSave
} // namespace PlayFab

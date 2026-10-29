#include "stdafx.h"
#include "GameSaveServiceMock.h"
#include "ApiHelpers.h"
#include "ZipUtils.h"
#include "Platform/Platform.h"
#include "ArchiveOperations.h"
#include "Generated/GameSaveTypes.h"
#include "Generated/GameSave.h"
#include "GameSaveGlobalState.h"
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

// Debug: a mock service call held in flight like a slow HTTP request.
// Completes S_OK once its fault point is no longer paused (the call then runs normally), or E_ABORT
// once its cancellation token is cancelled (XAsyncCancel of the SDK call, uninitialize), which is how
// a cancelled libHttpClient request completes.
class HeldServiceCallMockProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    HeldServiceCallMockProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], const char* point, bool once) :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_point{ point },
        m_once{ once }
    {
    }

protected:
    HRESULT DoWork(RunContext runContext) override
    {
        if (runContext.CancellationToken().IsCancelled())
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: held call '%s' cancelled", m_point.c_str());
            Fail(E_ABORT);
            return E_ABORT;
        }
        HRESULT hr = S_OK;
        // PauseOnce holds this call alone (later calls pass the point); Pause holds every call.
        const bool held = m_once ? IsFaultPointHoldingForDebug(m_point.c_str()) : CheckFaultPointForDebug(m_point.c_str(), hr) == PFGameSaveFaultAction::Pause;
        if (held)
        {
            RETURN_IF_FAILED(Schedule(kHeldCallPollMs));
            return E_PENDING;
        }
        TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: held call '%s' released", m_point.c_str());
        Complete(0);
        return E_PENDING;
    }

private:
    static constexpr uint32_t kHeldCallPollMs = 20;
    const String m_point;
    const bool m_once;
};

class HeldServiceCallMockOp : public XAsyncOperation<void>
{
public:
    HeldServiceCallMockOp(const char* point, bool once, PlayFab::RunContext runContext) noexcept :
        XAsyncOperation<void>{ std::move(runContext) },
        m_point{ point },
        m_once{ once }
    {
    }

    HeldServiceCallMockOp(HeldServiceCallMockOp const&) = delete;
    HeldServiceCallMockOp& operator=(HeldServiceCallMockOp) = delete;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        auto provider = MakeUnique<HeldServiceCallMockProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(HeldServiceCallMockOp), m_point, m_once);
        return XAsyncProviderBase::Run(std::move(provider));
    }

    const char* m_point;
    const bool m_once;
};

// Debug fault point at the start of a mock service call ("Service.<Name>", see
// PFGameSaveFilesSetFaultInjectionForDebug). S_OK with `held` empty: run the call now. A failure:
// the call fails with it (Fail action, once). S_OK with `held` set: Pause - the call is in flight
// until `held` completes (HeldServiceCallMockProvider).
static HRESULT CheckServiceCallFault(_In_z_ const char* point, RunContext& rc, std::optional<AsyncOp<void>>& held)
{
    HRESULT hr = S_OK;
    switch (CheckFaultPointForDebug(point, hr))
    {
        case PFGameSaveFaultAction::Fail:
        case PFGameSaveFaultAction::Abandon:
            return FAILED(hr) ? hr : E_FAIL;
        case PFGameSaveFaultAction::Pause:
            // PauseOnce reports Pause for the one call it holds (IsFaultPointHoldingForDebug).
            held.emplace(RunOperation(MakeUnique<HeldServiceCallMockOp>(point, IsFaultPointHoldingForDebug(point), rc.Derive())));
            return S_OK;
        default:
            return S_OK;
    }
}

// Runs `call` behind its "Service.<Name>" fault point (CheckServiceCallFault): a Fail fails the call,
// a Pause holds it in flight and runs it once resumed (E_ABORT when cancelled first).
template<typename TResponse, typename TCall>
static AsyncOp<TResponse> RunBehindServiceCallFault(_In_z_ const char* point, RunContext& rc, TCall&& call)
{
    std::optional<AsyncOp<void>> held;
    HRESULT faultHr = CheckServiceCallFault(point, rc, held);
    if (FAILED(faultHr))
    {
        return AsyncOp<TResponse>(faultHr);
    }
    if (held.has_value())
    {
        return held->Then([call = std::forward<TCall>(call)](Result<void> result) mutable -> AsyncOp<TResponse>
        {
            if (FAILED(result.hr))
            {
                return AsyncOp<TResponse>(result.hr);
            }
            return call();
        });
    }
    return call();
}

// ---- H-2 (core unit test plan): the mock service's manifest store -------------------------------
//
// The service state is one JSON document, manifests.json in the mock cloud folder:
//   {"Manifests":[record...], "TitleConfiguration":{...}}
// A record keeps the original fields (Version, DeviceId, DeviceName, DeviceType, Status,
// UploadProgressBytesUploaded, UploadProgressTotalBytes, CreationTimestamp, FinalizationTimestamp,
// Files [{FileName}]) and adds optional ones the service also keeps: DeviceVersion, BaseVersion,
// LastUpdateTimestamp, ManifestDescription (as sent, Base64), IsKnownGood (bool), Conflict
// {IsWinner (bool), ConflictingVersion}, InitiatedFiles [names] (the files InitiateUpload allocated,
// for the per-manifest file limit) and FileSizeBytes on a finalized file. TitleConfiguration (written
// by a test: PerPlayerQuotaBytes as the raw string the service sends, DisableClientRollbackToLastKnownGood,
// DisableClientRollbackToLastConflictLoser) is returned by ListManifests as is.
// Every endpoint reads and writes the cloud document directly (not a device's cached copy), under one
// lock, so a call never acts on a stale list and concurrent calls (an upload and the active-device
// poll) never tear it. The endpoint rules follow specs/playfab-gamesave/design/gamesave-service-endpoint-rules.md.
namespace
{

constexpr size_t kMockServiceMaxManifestFiles = 100;    // InitiateUpload: files across all uploads of one manifest
constexpr size_t kMockServiceMaxDescriptionLength = 1024; // ManifestDescription

RecursiveMutex& CloudDocumentMutex()
{
    static RecursiveMutex mutex;
    return mutex;
}

String CloudManifestsPath()
{
    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    String path;
    if (rootFolder.empty() || FAILED(JoinPathHelper(rootFolder, "manifests.json", path)))
    {
        return String{};
    }
    return path;
}

// The cloud document; an absent or unreadable file is an empty cloud (as the service would answer).
JsonValue LoadCloudDocument()
{
    JsonValue doc = JsonValue::object();
    String path = CloudManifestsPath();
    Vector<char> data;
    if (!path.empty() && FilePAL::DoesFileExist(path) && SUCCEEDED(ReadEntireFile(path, data)) && !data.empty())
    {
        try
        {
            String text(data.data(), data.size());
            JsonValue parsed = JsonValue::parse(text);
            if (parsed.is_object())
            {
                doc = std::move(parsed);
            }
        }
        catch (...)
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: manifests.json isn't valid JSON - read as an empty cloud");
        }
    }
    if (!doc.contains("Manifests") || !doc["Manifests"].is_array())
    {
        doc["Manifests"] = JsonValue::array();
    }
    return doc;
}

HRESULT SaveCloudDocument(const JsonValue& doc)
{
    String rootFolder = GameSaveServiceMock::GetMockDataFolder();
    String path = CloudManifestsPath();
    RETURN_HR_IF(E_FAIL, path.empty());
    FilePAL::CreatePath(rootFolder);
    String text = JsonUtils::WriteToString(doc);
    Vector<char> data(text.begin(), text.end());
    HRESULT hr = WriteEntireFile(path, data);
    if (FAILED(hr))
    {
        TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: writing manifests.json failed hr=0x%08X", hr);
    }
    return hr;
}

String RecordString(const JsonValue& record, const char* name)
{
    if (record.is_object() && record.contains(name) && record[name].is_string())
    {
        return record[name].get<String>();
    }
    return String{};
}

bool RecordBool(const JsonValue& record, const char* name, bool defaultValue = false)
{
    if (record.is_object() && record.contains(name) && record[name].is_boolean())
    {
        return record[name].get<bool>();
    }
    return defaultValue;
}

time_t RecordTime(const JsonValue& record, const char* name)
{
    String text = RecordString(record, name);
    return text.empty() ? 0 : Iso8601StringToTimeT(text);
}

ManifestStatus RecordStatus(const JsonValue& record)
{
    return ConvertToManifestStatusEnum(RecordString(record, "Status"));
}

uint64_t RecordVersion(const JsonValue& record)
{
    return StringToUint64(RecordString(record, "Version"));
}

JsonValue* FindRecord(JsonValue& doc, const String& version)
{
    for (auto& record : doc["Manifests"])
    {
        if (RecordString(record, "Version") == version)
        {
            return &record;
        }
    }
    return nullptr;
}

void Touch(JsonValue& record)
{
    record["LastUpdateTimestamp"] = TimeTToIso8601String(GetTimeTNow());
}

// The service's NextAvailableVersion: max(all versions, deleted ones too) + 1, or "0" with none.
String NextAvailableVersion(const JsonValue& doc)
{
    uint64_t maxVersion = 0;
    bool any = false;
    for (const auto& record : doc["Manifests"])
    {
        any = true;
        maxVersion = (std::max)(maxVersion, RecordVersion(record));
    }
    return any ? Uint64ToString(maxVersion + 1) : String{ "0" };
}

// The quota FinalizeManifest enforces: TitleConfiguration.PerPlayerQuotaBytes when it is a positive
// number ("0" is the 256 MB default); anything else (absent, not a number) isn't enforced.
std::optional<uint64_t> EnforcedQuotaBytes(const JsonValue& doc)
{
    if (!doc.contains("TitleConfiguration") || !doc["TitleConfiguration"].is_object())
    {
        return std::nullopt;
    }
    String raw = RecordString(doc["TitleConfiguration"], "PerPlayerQuotaBytes");
    if (raw.empty() || std::find_if(raw.begin(), raw.end(), [](char c) { return c < '0' || c > '9'; }) != raw.end())
    {
        return std::nullopt;
    }
    try
    {
        uint64_t quota = std::stoull(std::string{ raw.c_str() });
        return quota == 0 ? 256ull * 1024ull * 1024ull : quota;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

ManifestWrap ToManifestWrap(const JsonValue& record)
{
    ManifestWrap manifest;
    manifest.SetVersion(RecordString(record, "Version"));
    manifest.SetStatus(RecordString(record, "Status"));

    PlayFab::Wrappers::PFGameSaveManifestMetadataWrapper<Allocator> metadata;
    metadata.SetDeviceId(RecordString(record, "DeviceId"));
    metadata.SetDeviceName(RecordString(record, "DeviceName"));
    metadata.SetDeviceType(RecordString(record, "DeviceType"));
    if (record.contains("DeviceVersion"))
    {
        metadata.SetDeviceVersion(RecordString(record, "DeviceVersion"));
    }
    manifest.SetMetadata(std::move(metadata));

    PlayFab::Wrappers::PFGameSaveUploadProgressWrapper<Allocator> uploadProgress;
    uploadProgress.SetUploadedBytes(RecordString(record, "UploadProgressBytesUploaded"));
    uploadProgress.SetTotalBytes(RecordString(record, "UploadProgressTotalBytes"));
    manifest.SetUploadProgress(std::move(uploadProgress));

    manifest.SetCreationTimestamp(RecordTime(record, "CreationTimestamp"));
    manifest.SetFinalizationTimestamp(RecordTime(record, "FinalizationTimestamp"));
    manifest.SetLastUpdateTimestamp(RecordTime(record, "LastUpdateTimestamp"));

    String baseVersion = RecordString(record, "BaseVersion");
    if (!baseVersion.empty())
    {
        manifest.SetBaseVersion(std::move(baseVersion));
    }
    String description = RecordString(record, "ManifestDescription");
    if (!description.empty())
    {
        manifest.SetManifestDescription(std::move(description));
    }
    if (record.contains("IsKnownGood") && record["IsKnownGood"].is_boolean())
    {
        manifest.SetIsKnownGood(record["IsKnownGood"].get<bool>());
    }
    if (record.contains("Conflict") && record["Conflict"].is_object())
    {
        PlayFab::Wrappers::PFGameSaveManifestConflictWrapper<Allocator> conflict;
        conflict.SetIsWinner(RecordBool(record["Conflict"], "IsWinner"));
        conflict.SetConflictingVersion(RecordString(record["Conflict"], "ConflictingVersion"));
        manifest.SetConflict(std::move(conflict));
    }

    if (record.contains("Files") && record["Files"].is_array())
    {
        FileMetadataWrapVector files;
        for (const auto& fileJson : record["Files"])
        {
            FileMetadataWrap file;
            file.SetFileName(RecordString(fileJson, "FileName"));
            files.push_back(std::move(file));
        }
        manifest.SetFiles(std::move(files));
    }
    return manifest;
}

} // anonymous namespace

AsyncOp<FinalizeManifestResponse> GameSaveServiceMock::FinalizeManifest(
    Entity const& entity,
    const FinalizeManifestRequest& request,
    RunContext rc
)
{
    // H-3 (core unit test plan): the "Service.FinalizeManifest" fault point.
    return RunBehindServiceCallFault<FinalizeManifestResponse>("Service.FinalizeManifest", rc,
        [entity = Entity{ entity }, request, rc]() mutable
        {
            return FinalizeManifestUnfaulted(entity, request, rc);
        });
}

AsyncOp<FinalizeManifestResponse> GameSaveServiceMock::FinalizeManifestUnfaulted(
    Entity const& entity,
    const FinalizeManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::FinalizeManifest)
    {
        return AsyncOp<FinalizeManifestResponse>(E_FAIL);
    }

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::FinalizeManifestInvalidParamsOnce)
    {
        m_forcedOffline = GameSaveServiceMockForcedOffline::None;
        return AsyncOp<FinalizeManifestResponse>(E_PF_INVALID_PARAMS);
    }

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::FinalizeManifestNewerManifestExistsOnce)
    {
        m_forcedOffline = GameSaveServiceMockForcedOffline::None;
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS);
    }

    // The service's file cap: a version may list at most 100 files, like the real FinalizeManifest
    // ("FilesToFinalize ... maximum length of '100'").
    {
        const size_t maxFiles = kMockServiceMaxManifestFiles;
        if (request.GetFilesToFinalize().size() > maxFiles)
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: FinalizeManifest lists %zu files, over the cap of %zu - InvalidParams",
                static_cast<size_t>(request.GetFilesToFinalize().size()), maxFiles);
            return AsyncOp<FinalizeManifestResponse>(E_PF_INVALID_PARAMS);
        }
    }

    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(rc);
    const String& version = request.GetVersion();
    std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
    JsonValue doc = LoadCloudDocument();
    JsonValue* record = FindRecord(doc, version);
    if (record == nullptr)
    {
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_MANIFEST_NOT_FOUND);
    }

    const auto& conflict = request.GetConflict();
    if (conflict.has_value() && conflict->GetConflictingVersion() == version)
    {
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_MANIFEST_NOT_ELIGIBLE_AS_CONFLICTING_VERSION);
    }
    const bool markBaseAsKnownGood = request.GetMarkBaseAsKnownGood().value_or(false);
    const String baseVersion = RecordString(*record, "BaseVersion");
    if (markBaseAsKnownGood && (baseVersion.empty() || baseVersion == version))
    {
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD);
    }

    const ManifestStatus status = RecordStatus(*record);
    if (status == ManifestStatus::Finalized)
    {
        // Idempotent: finalizing a finalized version again answers with it.
        FinalizeManifestResponse r{};
        r.SetManifest(ToManifestWrap(*record));
        return AsyncOp<FinalizeManifestResponse>(std::move(r));
    }
    if (status != ManifestStatus::Uploading)
    {
        TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: FinalizeManifest v%s in status %s - ManifestUpdatesNotAllowed", version.c_str(), RecordString(*record, "Status").c_str());
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED);
    }

    // Latest-version rule: unless forced or a conflict loser, no newer version (other than one
    // pending deletion) may exist.
    const bool isConflictLoser = conflict.has_value() && !conflict->GetIsWinner();
    if (!request.GetForce() && !isConflictLoser)
    {
        const uint64_t thisVersion = StringToUint64(version);
        for (const auto& other : doc["Manifests"])
        {
            if (RecordStatus(other) != ManifestStatus::PendingDeletion && RecordVersion(other) > thisVersion)
            {
                TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: FinalizeManifest v%s but v%s exists - NewerManifestExists", version.c_str(), RecordString(other, "Version").c_str());
                return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS);
            }
        }
    }

    // Quota (TitleConfiguration.PerPlayerQuotaBytes, when the test configured one).
    uint64_t totalBytes = 0;
    for (const auto& file : request.GetFilesToFinalize())
    {
        totalBytes += StringToUint64(file.GetFileSizeBytes());
    }
    std::optional<uint64_t> quota = EnforcedQuotaBytes(doc);
    if (quota.has_value() && totalBytes > quota.value())
    {
        TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: FinalizeManifest v%s holds %llu bytes, over the quota of %llu - DataStorageQuotaExceeded",
            version.c_str(), static_cast<unsigned long long>(totalBytes), static_cast<unsigned long long>(quota.value()));
        return AsyncOp<FinalizeManifestResponse>(E_PF_GAME_SAVE_DATA_STORAGE_QUOTA_EXCEEDED);
    }

    const String& description = request.GetManifestDescription();
    if (description.size() > kMockServiceMaxDescriptionLength)
    {
        return AsyncOp<FinalizeManifestResponse>(E_PF_INVALID_PARAMS);
    }

    JsonValue files = JsonValue::array();
    for (const auto& file : request.GetFilesToFinalize())
    {
        JsonValue fileJson = JsonValue::object();
        fileJson["FileName"] = file.GetFileName();
        fileJson["FileSizeBytes"] = file.GetFileSizeBytes();
        files.push_back(std::move(fileJson));
    }
    (*record)["Files"] = std::move(files);
    (*record)["Status"] = ConvertToManifestStatusString(ManifestStatus::Finalized);
    (*record)["FinalizationTimestamp"] = TimeTToIso8601String(GetTimeTNow());
    Touch(*record);
    if (!description.empty())
    {
        (*record)["ManifestDescription"] = description;
    }
    else
    {
        record->erase("ManifestDescription");
    }

    if (conflict.has_value())
    {
        JsonValue conflictJson = JsonValue::object();
        conflictJson["IsWinner"] = conflict->GetIsWinner();
        conflictJson["ConflictingVersion"] = conflict->GetConflictingVersion();
        (*record)["Conflict"] = std::move(conflictJson);
        // The conflicting version gets the inverse; a missing one doesn't block the finalize.
        JsonValue* other = FindRecord(doc, conflict->GetConflictingVersion());
        if (other != nullptr)
        {
            JsonValue inverse = JsonValue::object();
            inverse["IsWinner"] = !conflict->GetIsWinner();
            inverse["ConflictingVersion"] = version;
            (*other)["Conflict"] = std::move(inverse);
            Touch(*other);
        }
    }

    if (markBaseAsKnownGood)
    {
        JsonValue* base = FindRecord(doc, baseVersion);
        if (base != nullptr && !RecordBool(*base, "IsKnownGood"))
        {
            (*base)["IsKnownGood"] = true;
            if (RecordStatus(*base) != ManifestStatus::Quarantined)
            {
                (*base)["Status"] = ConvertToManifestStatusString(ManifestStatus::Finalized);
            }
            Touch(*base);
        }
        // A missing base isn't an error.
        record = FindRecord(doc, version);
    }

    ManifestWrap finalized = ToManifestWrap(*record);
    RETURN_IF_FAILED(SaveCloudDocument(doc));
    FinalizeManifestResponse r{};
    r.SetManifest(std::move(finalized));
    return AsyncOp<FinalizeManifestResponse>(std::move(r));
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

    std::optional<AsyncOp<void>> held;
    HRESULT faultHr = CheckServiceCallFault("Service.GetManifestDownloadDetails", rc, held);
    if (FAILED(faultHr))
    {
        return AsyncOp<GetManifestDownloadDetailsResponse>(faultHr);
    }
    if (held.has_value())
    {
        return held->Then([entity = Entity{ entity }, request, rc](Result<void> result) -> AsyncOp<GetManifestDownloadDetailsResponse>
        {
            if (FAILED(result.hr))
            {
                return AsyncOp<GetManifestDownloadDetailsResponse>(result.hr);
            }
            return GameSaveServiceMock::GetManifestDownloadDetails(entity, request, rc);
        });
    }

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
    // H-3 (core unit test plan): the "Service.InitializeManifest" fault point.
    return RunBehindServiceCallFault<InitializeManifestResponse>("Service.InitializeManifest", rc,
        [entity = Entity{ entity }, request, rc]() mutable
        {
            return InitializeManifestUnfaulted(entity, request, rc);
        });
}

AsyncOp<InitializeManifestResponse> GameSaveServiceMock::InitializeManifestUnfaulted(
    Entity const& entity,
    const InitializeManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::InitializeManifest)
    {
        return AsyncOp<InitializeManifestResponse>(E_FAIL);
    }

    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(rc);
    const String& version = request.GetVersion();
    std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
    JsonValue doc = LoadCloudDocument();

    const auto& requestMetadata = request.GetMetadata();
    JsonValue* existing = FindRecord(doc, version);
    if (existing != nullptr)
    {
        // Idempotent: the same Initialized manifest requested again by the same device.
        if (RecordStatus(*existing) == ManifestStatus::Initialized &&
            RecordString(*existing, "DeviceId") == requestMetadata.GetDeviceId() &&
            RecordString(*existing, "DeviceName") == requestMetadata.GetDeviceName() &&
            RecordString(*existing, "DeviceType") == requestMetadata.GetDeviceType())
        {
            InitializeManifestResponse r{};
            r.SetManifest(ToManifestWrap(*existing));
            return AsyncOp<InitializeManifestResponse>(std::move(r));
        }
        TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: InitializeManifest v%s already exists - ManifestVersionAlreadyExists", version.c_str());
        return AsyncOp<InitializeManifestResponse>(E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS);
    }

    // BaseVersion rules: equal to Version or 0 means no base; otherwise it must be lower and finalized.
    const uint64_t newVersion = StringToUint64(version);
    const uint64_t baseVersion = StringToUint64(request.GetBaseVersion());
    if (baseVersion != newVersion && baseVersion != 0)
    {
        JsonValue* base = FindRecord(doc, request.GetBaseVersion());
        if (baseVersion > newVersion || base == nullptr || RecordStatus(*base) != ManifestStatus::Finalized)
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: InitializeManifest v%s base v%s unavailable - BaseVersionNotAvailable", version.c_str(), request.GetBaseVersion().c_str());
            return AsyncOp<InitializeManifestResponse>(E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE);
        }
    }

    const String now = TimeTToIso8601String(GetTimeTNow());
    JsonValue record = JsonValue::object();
    record["Version"] = version;
    record["BaseVersion"] = request.GetBaseVersion();
    record["DeviceId"] = requestMetadata.GetDeviceId();
    record["DeviceName"] = requestMetadata.GetDeviceName();
    record["DeviceType"] = requestMetadata.GetDeviceType();
    record["DeviceVersion"] = requestMetadata.GetDeviceVersion();
    record["Status"] = ConvertToManifestStatusString(ManifestStatus::Initialized);
    record["UploadProgressBytesUploaded"] = "0";
    record["UploadProgressTotalBytes"] = "0";
    record["CreationTimestamp"] = now;
    record["FinalizationTimestamp"] = TimeTToIso8601String(0);
    record["LastUpdateTimestamp"] = now;
    record["Files"] = JsonValue::array();
    ManifestWrap created = ToManifestWrap(record);
    doc["Manifests"].push_back(std::move(record));
    RETURN_IF_FAILED(SaveCloudDocument(doc));

    InitializeManifestResponse r{};
    r.SetManifest(std::move(created));
    return AsyncOp<InitializeManifestResponse>(std::move(r));
}

AsyncOp<InitiateUploadResponse> GameSaveServiceMock::InitiateUpload(
    Entity const& entity,
    const InitiateUploadRequest& request,
    RunContext rc,
    const Vector<UploadFileDetail>& mockFilesRequest
)
{
    // H-3 (core unit test plan): the "Service.InitiateUpload" fault point.
    return RunBehindServiceCallFault<InitiateUploadResponse>("Service.InitiateUpload", rc,
        [entity = Entity{ entity }, request, rc, mockFilesRequest]() mutable
        {
            return InitiateUploadUnfaulted(entity, request, rc, mockFilesRequest);
        });
}

AsyncOp<InitiateUploadResponse> GameSaveServiceMock::InitiateUploadUnfaulted(
    Entity const& entity,
    const InitiateUploadRequest& request,
    RunContext rc,
    const Vector<UploadFileDetail>& mockFilesRequest
)
{
    UNREFERENCED_PARAMETER(rc);
    UNREFERENCED_PARAMETER(entity);

    if (m_forcedOffline == GameSaveServiceMockForcedOffline::InitiateUpload)
    {
        return AsyncOp<InitiateUploadResponse>(E_FAIL);
    }

    {
        // Initialized or Uploading only; the version becomes Uploading; at most 100 files across all
        // uploads of one manifest (a re-upload of the same name counts once).
        std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
        JsonValue doc = LoadCloudDocument();
        JsonValue* record = FindRecord(doc, request.GetVersion());
        if (record == nullptr)
        {
            return AsyncOp<InitiateUploadResponse>(E_PF_GAME_SAVE_MANIFEST_NOT_FOUND);
        }
        const ManifestStatus status = RecordStatus(*record);
        if (status != ManifestStatus::Initialized && status != ManifestStatus::Uploading)
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: InitiateUpload v%s in status %s - ManifestUpdatesNotAllowed", request.GetVersion().c_str(), RecordString(*record, "Status").c_str());
            return AsyncOp<InitiateUploadResponse>(E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED);
        }
        std::set<String> initiated;
        if (record->contains("InitiatedFiles") && (*record)["InitiatedFiles"].is_array())
        {
            for (const auto& name : (*record)["InitiatedFiles"])
            {
                if (name.is_string())
                {
                    initiated.insert(name.get<String>());
                }
            }
        }
        for (const auto& file : request.GetFiles())
        {
            initiated.insert(file.GetFileName());
        }
        if (initiated.size() > kMockServiceMaxManifestFiles)
        {
            TRACE_WARNING("[GAME SAVE] GameSaveServiceMock: InitiateUpload v%s brings the manifest to %zu files - ManifestFilesLimitExceeded", request.GetVersion().c_str(), initiated.size());
            return AsyncOp<InitiateUploadResponse>(E_PF_GAME_SAVE_MANIFEST_FILES_LIMIT_EXCEEDED);
        }
        JsonValue initiatedJson = JsonValue::array();
        for (const String& name : initiated)
        {
            initiatedJson.push_back(name);
        }
        (*record)["InitiatedFiles"] = std::move(initiatedJson);
        (*record)["Status"] = ConvertToManifestStatusString(ManifestStatus::Uploading);
        Touch(*record);
        RETURN_IF_FAILED(SaveCloudDocument(doc));
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
    // H-3 (core unit test plan): the "Service.ListManifests" fault point.
    return RunBehindServiceCallFault<ListManifestsResponse>("Service.ListManifests", rc,
        [entity = Entity{ entity }, request, rc]() mutable
        {
            return ListManifestsUnfaulted(entity, request, rc);
        });
}

AsyncOp<ListManifestsResponse> GameSaveServiceMock::ListManifestsUnfaulted(
    Entity const& entity,
    const ListManifestsRequest& request,
    RunContext rc)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::ListManifests)
    {
        return AsyncOp<ListManifestsResponse>(E_FAIL);
    }

    // The device's copy of manifests.json (a "Service.DownloadFile" call, as before H-2), then the
    // answer from the cloud document itself.
    String manifestsFile = "manifests.json";
    String manifestsPath = GetMockGlobalPath(entity, manifestsFile);
    String cloudPath = CloudManifestsPath();
    const bool includeUnavailable = request.GetIncludeUnavailable();

    auto answer = [includeUnavailable]() -> AsyncOp<ListManifestsResponse>
    {
        // The SDK is gone (uninitialized while the call was in flight): fail like a cancelled HTTP
        // request instead of answering with an empty cloud.
        if (GameSaveServiceMock::GetMockDataFolder().empty())
        {
            return AsyncOp<ListManifestsResponse>(E_ABORT);
        }
        std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
        JsonValue doc = LoadCloudDocument();
        ListManifestsResponse response{};
        ManifestWrapVector manifests;
        for (const auto& record : doc["Manifests"])
        {
            ManifestStatus status = RecordStatus(record);
            if (!includeUnavailable && (status == ManifestStatus::PendingDeletion || status == ManifestStatus::Quarantined))
            {
                continue;
            }
            manifests.push_back(ToManifestWrap(record));
        }
        response.SetManifests(std::move(manifests));
        response.SetNextAvailableVersion(NextAvailableVersion(doc));
        if (doc.contains("TitleConfiguration") && doc["TitleConfiguration"].is_object())
        {
            const JsonValue& configJson = doc["TitleConfiguration"];
            PlayFab::Wrappers::PFGameSaveTitleConfigurationWrapper<Allocator> config;
            config.SetPerPlayerQuotaBytes(RecordString(configJson, "PerPlayerQuotaBytes"));
            config.SetDisableClientRollbackToLastKnownGood(RecordBool(configJson, "DisableClientRollbackToLastKnownGood"));
            config.SetDisableClientRollbackToLastConflictLoser(RecordBool(configJson, "DisableClientRollbackToLastConflictLoser"));
            response.SetTitleConfiguration(std::move(config));
        }
        return AsyncOp<ListManifestsResponse>(std::move(response));
    };

    if (cloudPath.empty() || !FilePAL::DoesFileExist(cloudPath))
    {
        return answer();
    }
    AsyncOp<void> copy = [&]()
    {
        std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
        return GameSaveServiceMock::DownloadFileFromCloud(rc, manifestsPath, cloudPath, nullptr);
    }();
    return copy.Then([answer](Result<void> result) -> AsyncOp<ListManifestsResponse>
    {
        if (FAILED(result.hr))
        {
            return AsyncOp<ListManifestsResponse>(result.hr);
        }
        return answer();
    });
}

AsyncOp<UpdateManifestResponse> GameSaveServiceMock::UpdateManifest(
    Entity const& entity,
    const UpdateManifestRequest& request,
    RunContext rc
)
{
    // H-3 (core unit test plan): the "Service.UpdateManifest" fault point.
    return RunBehindServiceCallFault<UpdateManifestResponse>("Service.UpdateManifest", rc,
        [entity = Entity{ entity }, request, rc]() mutable
        {
            return UpdateManifestUnfaulted(entity, request, rc);
        });
}

AsyncOp<UpdateManifestResponse> GameSaveServiceMock::UpdateManifestUnfaulted(
    Entity const& entity,
    const UpdateManifestRequest& request,
    RunContext rc
)
{
    if (m_forcedOffline == GameSaveServiceMockForcedOffline::UpdateManifest)
    {
        return AsyncOp<UpdateManifestResponse>(E_FAIL);
    }
    UNREFERENCED_PARAMETER(entity);
    UNREFERENCED_PARAMETER(rc);

    std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
    JsonValue doc = LoadCloudDocument();
    JsonValue* record = FindRecord(doc, request.GetVersion());
    if (record == nullptr)
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_MANIFEST_NOT_FOUND);
    }

    // A field is requested when the request carries it (an empty description isn't sent).
    const bool updateProgress = request.GetUploadProgress().has_value();
    const bool updateDescription = !request.GetManifestDescription().empty();
    const std::optional<bool>& markAsKnownGood = request.GetMarkAsKnownGood();
    if (!updateProgress && !updateDescription && !markAsKnownGood.has_value())
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_NO_UPDATES_REQUESTED);
    }

    const ManifestStatus status = RecordStatus(*record);
    const bool pending = status == ManifestStatus::Initialized || status == ManifestStatus::Uploading;
    if (status == ManifestStatus::Quarantined)
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED);
    }
    if (updateProgress && !pending)
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_MANIFEST_UPLOAD_PROGRESS_UPDATE_NOT_ALLOWED);
    }
    if (updateDescription && !pending)
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_MANIFEST_DESCRIPTION_UPDATE_NOT_ALLOWED);
    }
    if (updateDescription && request.GetManifestDescription().size() > kMockServiceMaxDescriptionLength)
    {
        return AsyncOp<UpdateManifestResponse>(E_PF_INVALID_PARAMS);
    }
    if (markAsKnownGood.has_value())
    {
        // Finalized, or PendingDeletion that was finalized; never a pending manifest.
        const bool wasFinalized = status == ManifestStatus::Finalized ||
            (status == ManifestStatus::PendingDeletion && RecordTime(*record, "FinalizationTimestamp") != 0);
        if (!wasFinalized)
        {
            return AsyncOp<UpdateManifestResponse>(E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD);
        }
    }

    if (updateProgress)
    {
        (*record)["UploadProgressBytesUploaded"] = request.GetUploadProgress()->GetUploadedBytes();
        (*record)["UploadProgressTotalBytes"] = request.GetUploadProgress()->GetTotalBytes();
    }
    if (updateDescription)
    {
        (*record)["ManifestDescription"] = request.GetManifestDescription();
    }
    if (markAsKnownGood.has_value())
    {
        (*record)["IsKnownGood"] = markAsKnownGood.value();
        if (markAsKnownGood.value())
        {
            (*record)["Status"] = ConvertToManifestStatusString(ManifestStatus::Finalized);
        }
    }
    Touch(*record);
    ManifestWrap updated = ToManifestWrap(*record);
    RETURN_IF_FAILED(SaveCloudDocument(doc));

    UpdateManifestResponse r{};
    r.SetManifest(std::move(updated));
    return AsyncOp<UpdateManifestResponse>(std::move(r));
}

String GameSaveServiceMock::GetMockGlobalPath(const Entity& entity, const String& file)
{
    SharedPtr<GameSaveGlobalState> state;
    HRESULT hr = GameSaveGlobalState::Get(state);
    if (SUCCEEDED(hr))
    {
        String root = state->GetDebugRootFolderOverride();
        String pathA, pathB, pathC;
        if (SUCCEEDED(JoinPathHelper(root, kMockSaveFolderName, pathA)))
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
    std::optional<AsyncOp<void>> held;
    HRESULT faultHr = CheckServiceCallFault("Service.DownloadFile", runContext, held);
    if (FAILED(faultHr))
    {
        return AsyncOp<void>(faultHr);
    }
    if (held.has_value())
    {
        return held->Then([runContext, filePath, downloadUrl, archiveContext](Result<void> result) -> AsyncOp<void>
        {
            if (FAILED(result.hr))
            {
                return AsyncOp<void>(result.hr);
            }
            return GameSaveServiceMock::DownloadFileFromCloud(runContext, filePath, downloadUrl, archiveContext);
        });
    }

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
        RETURN_IF_FAILED(WriteEntireFile(filePath, fileData));
    }

    return AsyncOp<void>(S_OK);
}

AsyncOp<void> GameSaveServiceMock::UploadFileFromStringToCloud(RunContext runContext, const String& fileName, const String& fileContent)
{
    // H-3 (core unit test plan): a blob upload (the extended manifest) is a "Service.UploadFile" call.
    return RunBehindServiceCallFault<void>("Service.UploadFile", runContext,
        [runContext, fileName, fileContent]() mutable
        {
            return UploadFileFromStringToCloudUnfaulted(runContext, fileName, fileContent);
        });
}

AsyncOp<void> GameSaveServiceMock::UploadBlobToCloud(RunContext runContext, const String& fileName, const String& filePath)
{
    // H-3 (core unit test plan): a blob upload (bundle, thumbnail) is a "Service.UploadFile" call. The
    // mock's own manifests.json writes use UploadFileToCloud directly and don't count.
    return RunBehindServiceCallFault<void>("Service.UploadFile", runContext,
        [runContext, fileName, filePath]() mutable
        {
            return UploadFileToCloud(runContext, fileName, filePath);
        });
}

AsyncOp<void> GameSaveServiceMock::DeleteManifest(Entity const& entity, const DeleteManifestRequest& request, RunContext rc)
{
    UNREFERENCED_PARAMETER(entity);
    // H-3 (core unit test plan): the "Service.DeleteManifest" fault point. H-2: a soft delete - the
    // version becomes PendingDeletion (filtered from ListManifests, still counted by
    // NextAvailableVersion); deleting it again succeeds.
    return RunBehindServiceCallFault<void>("Service.DeleteManifest", rc,
        [version = request.GetVersion()]() -> AsyncOp<void>
        {
            std::lock_guard<std::recursive_mutex> lock(CloudDocumentMutex());
            JsonValue doc = LoadCloudDocument();
            JsonValue* record = FindRecord(doc, version);
            if (record == nullptr)
            {
                return AsyncOp<void>(E_PF_GAME_SAVE_MANIFEST_NOT_FOUND);
            }
            if (RecordStatus(*record) != ManifestStatus::PendingDeletion)
            {
                (*record)["Status"] = ConvertToManifestStatusString(ManifestStatus::PendingDeletion);
                Touch(*record);
                RETURN_IF_FAILED(SaveCloudDocument(doc));
            }
            return AsyncOp<void>(S_OK);
        });
}

AsyncOp<void> GameSaveServiceMock::UploadFileFromStringToCloudUnfaulted(RunContext runContext, const String& fileName, const String& fileContent)
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
        RETURN_IF_FAILED(WriteEntireFile(fullPath, fileData));
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

        RETURN_IF_FAILED(WriteEntireFile(fullPath, fileData));
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

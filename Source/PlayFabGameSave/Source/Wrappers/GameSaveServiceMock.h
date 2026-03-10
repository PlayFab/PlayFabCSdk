#pragma once

#include "Generated/GameSaveTypeWrappers.h"
#include "Generated/GameSaveTypes.h"
#include "Wrappers/Types.h"
#include "PFGameSaveFilesForDebug.h"
#include <mutex>
#include <condition_variable>

namespace PlayFab
{
namespace GameSaveWrapper
{

class GameSaveServiceMock
{
public:
    GameSaveServiceMock() = delete;
    GameSaveServiceMock(const GameSaveServiceMock& source) = delete;
    GameSaveServiceMock& operator=(const GameSaveServiceMock& source) = delete;
    ~GameSaveServiceMock() = default;

    static AsyncOp<void> UploadFileFromStringToCloud(RunContext runContext, const String& fileName, const String& fileContent);
    static AsyncOp<void> CompressAndUploadFileToCloud(RunContext runContext, const UploadFileDetail& fileDetail);
    static AsyncOp<void> UploadFileToCloud(RunContext runContext, const String& fileName, const String& filePath);
    static AsyncOp<void> DownloadFileFromCloud(RunContext runContext, const String& filePath, const String& downloadUrl, SharedPtr<ArchiveContext> archiveContext);
    static String GetMockDataFolder();

    // These should match generated API calls
    static AsyncOp<ListManifestsResponse> ListManifests(Entity const& entity, const ListManifestsRequest& request, RunContext rc);
    static AsyncOp<GetManifestDownloadDetailsResponse> GetManifestDownloadDetails(Entity const& entity, const GetManifestDownloadDetailsRequest& request, RunContext rc);
    static AsyncOp<GetQuotaForPlayerResponse> GetQuotaForPlayer(Entity const& entity, const GetQuotaForPlayerRequest& request, RunContext rc);
    static AsyncOp<InitializeManifestResponse> InitializeManifest(Entity const& entity, const InitializeManifestRequest& request, RunContext rc);
    static AsyncOp<InitiateUploadResponse> InitiateUpload(Entity const& entity, const InitiateUploadRequest& request, RunContext rc, const Vector<UploadFileDetail>& mockFilesRequest);
    static AsyncOp<FinalizeManifestResponse> FinalizeManifest(Entity const& entity, const FinalizeManifestRequest& request, RunContext rc);
    static AsyncOp<UpdateManifestResponse> UpdateManifest(Entity const& entity, const UpdateManifestRequest& request, RunContext rc);

    static void PauseUpload();
    static void ResumeUpload();
    static void SetForcedOffline(GameSaveServiceMockForcedOffline forcedOffline);

private:
    static String GetMockGlobalPath(const Entity& entity, const String& file);
    static ListManifestsResponse ReadManifestsFile(const String& manifestsPath);
    static void WriteManifestsFile(const String& manifestsPath, const Vector<ManifestWrap>& manifests);

    static inline std::mutex m_pauseMutex;
    static inline std::condition_variable m_pauseCondition;
    static inline std::atomic<bool> m_paused{ false };
    static inline GameSaveServiceMockForcedOffline m_forcedOffline{ GameSaveServiceMockForcedOffline::None };
};


} // namespace GameSave
} // namespace PlayFab

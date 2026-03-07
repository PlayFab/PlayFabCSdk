#pragma once

#include "GameSaveTypes.h"
#include "GameSaveGlobalState.h"
#include "Generated/GameSaveTypes.h"

namespace PlayFab
{
namespace GameSaveWrapper
{

class GameSaveAPI
{
public:
    GameSaveAPI() = delete;
    GameSaveAPI(const GameSaveAPI& source) = delete;
    GameSaveAPI& operator=(const GameSaveAPI& source) = delete;
    ~GameSaveAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> DeleteManifest(Entity const& entity, const DeleteManifestRequest& request, RunContext rc);
    static AsyncOp<FinalizeManifestResponse> FinalizeManifest(Entity const& entity, const FinalizeManifestRequest& request, RunContext rc);
    static AsyncOp<GetConfigForTitleResponse> GetConfigForTitle(Entity const& entity, const GetConfigForTitleRequest& request, RunContext rc);
    static AsyncOp<GetManifestDownloadDetailsResponse> GetManifestDownloadDetails(Entity const& entity, const GetManifestDownloadDetailsRequest& request, RunContext rc);
    static AsyncOp<GetQuotaForPlayerResponse> GetQuotaForPlayer(Entity const& entity, const GetQuotaForPlayerRequest& request, RunContext rc);
    static AsyncOp<InitializeManifestResponse> InitializeManifest(Entity const& entity, const InitializeManifestRequest& request, RunContext rc);
    static AsyncOp<InitiateUploadResponse> InitiateUpload(Entity const& entity, const InitiateUploadRequest& request, RunContext rc);
    static AsyncOp<ListManifestsResponse> ListManifests(Entity const& entity, const ListManifestsRequest& request, RunContext rc);
    static AsyncOp<void> RequestOnboarding(Entity const& entity, const RequestOnboardingRequest& request, RunContext rc);
    static AsyncOp<RollbackToManifestResponse> RollbackToManifest(Entity const& entity, const RollbackToManifestRequest& request, RunContext rc);
    static AsyncOp<UpdateConfigForTitleResponse> UpdateConfigForTitle(Entity const& entity, const UpdateConfigForTitleRequest& request, RunContext rc);
    static AsyncOp<UpdateManifestResponse> UpdateManifest(Entity const& entity, const UpdateManifestRequest& request, RunContext rc);
};

} // namespace GameSave
} // namespace PlayFab

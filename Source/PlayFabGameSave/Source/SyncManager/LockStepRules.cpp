// Copyright (C) Microsoft Corporation. All rights reserved.
//
// LockStep's pure rules: the baseline and pending-manifest selection, the rollback baseline and the
// InitializeManifest request. Moved unchanged from LockStep.cpp into their own translation unit so the
// unit test executable can compile them (core unit test plan H-1, EngineWhiteBoxSeam). Same class,
// same behavior; SelectBaselineManifest's rule takes the manifest list and AddUser options as
// arguments (the member passes its own).
#include "stdafx.h"
#include "LockStep.h"
#include "Metadata.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

HRESULT LockStep::TryGetLatestFinalizedManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest)
{
    // Pass 1: Find highest finalized manifest that is either non-conflict or the winner of a conflict.
    uint64_t latestEligibleVersion = 0;
    for (uint64_t i = 0; i < manifests.size(); ++i)
    {
        const ManifestWrap& manifest = manifests[i];
        if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        {
            continue;
        }

        bool isLoser = false;
        if (auto const& conflictOpt = manifest.GetConflict(); conflictOpt.has_value())
        {
            if (!conflictOpt->GetIsWinner())
            {
                isLoser = true;
            }
        }
        if (isLoser)
        {
            continue;
        }

        uint64_t ver = StringToUint64(manifest.GetVersion());
        if (ver > latestEligibleVersion)
        {
            latestEligibleVersion = ver;
            latestFinalizedManifest = manifest;
        }
    }

    if (latestEligibleVersion != 0)
    {
        TRACE_INFORMATION("[GAME SAVE] LockStep: Latest Finalized Manifest v:%llu", latestEligibleVersion);
        return S_OK;
    }

    // Pass 2: Pathological fallback - no eligible winner/non-conflict found. Use highest loser.
    uint64_t highestLoserVersion = 0;
    ManifestWrap highestLoserManifest;
    for (uint64_t i = 0; i < manifests.size(); ++i)
    {
        const ManifestWrap& manifest = manifests[i];
        if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        {
            continue;
        }
        bool loser = false;
        if (auto const& conflictOpt = manifest.GetConflict(); conflictOpt.has_value() && !conflictOpt->GetIsWinner())
        {
            loser = true;
        }
        if (loser)
        {
            uint64_t ver = StringToUint64(manifest.GetVersion());
            if (ver > highestLoserVersion)
            {
                highestLoserVersion = ver;
                highestLoserManifest = manifest;
            }
        }
    }

    if (highestLoserVersion != 0)
    {
        latestFinalizedManifest = highestLoserManifest;
        TRACE_WARNING("[GAME SAVE] Pathological manifest state: only conflict losers present. Using highest loser as latest finalized fallback v:%llu", highestLoserVersion);
    }
    else
    {
        // No finalized manifests at all.
        TRACE_INFORMATION("[GAME SAVE] No finalized manifests present when computing Latest Finalized (L)");
        latestFinalizedManifest = ManifestWrap(); // Ensure the output manifest is reset to a default state
    }
    return S_OK;
}

const ManifestWrap* LockStep::TryGetLatestPendingManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest)
{
    const ManifestWrap* latestPendingManifest = nullptr;
    uint64_t latestPendingVersion = 0;
    for (uint64_t i = 0; i < manifests.size(); i++)
    {
        const ManifestWrap& manifest = manifests[i];
        ManifestStatus manifestStatus = ConvertToManifestStatusEnum(manifest.GetStatus());
        if (manifestStatus == ManifestStatus::Initialized)
        {
            uint64_t manifestVer = StringToUint64(manifest.GetVersion());
            uint64_t latestFinalizedManifestVer = StringToUint64(latestFinalizedManifest.GetVersion());
            if (manifestVer <= latestFinalizedManifestVer)
            {
                continue; // ignore any old pending manifests
            }

            if (manifestVer > latestPendingVersion)
            {
                latestPendingVersion = manifestVer;
                latestPendingManifest = &manifest;
            }
        }
    }

    return latestPendingManifest;
}

// Given the latest finalized manifest (L), apply any surviving rollback intent
// (LastKnownGood or LastConflict) to choose an alternate baseline. Returns the chosen
// baseline manifest. Sets baselineFromRollback=true if a rollback-specific manifest
// was selected instead of L.
// Selects the baseline manifest given L (latest finalized) and current rollback intent.
// Returns either L or an alternate rollback baseline; sets rollbackBaselineSelected=true if alternate chosen.
ManifestWrap LockStep::SelectBaselineManifest(
    _In_ const ManifestWrapVector& manifests,
    _In_ PFGameSaveFilesAddUserOptions addUserOptions,
    _In_ const ManifestWrap& latestFinalizedL,
    _Out_ bool& rollbackBaselineSelected)
{
    rollbackBaselineSelected = false;
    ManifestWrap selectedBaseline = latestFinalizedL; // default L

    // LastKnownGood intent
    if ((addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) == PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood)
    {
        uint64_t bestKGVersion = 0;
        ManifestWrap bestKGManifest;
        for (size_t i = 0; i < manifests.size(); ++i)
        {
            const auto& manifest = manifests[i];
            if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
            {
                continue;
            }
            auto isKnownGood = manifest.GetIsKnownGood();
            if (!isKnownGood.has_value() || !isKnownGood.value())
            {
                continue;
            }
            uint64_t ver = StringToUint64(manifest.GetVersion());
            if (ver > bestKGVersion)
            {
                bestKGVersion = ver;
                bestKGManifest = manifest;
            }
        }
        if (bestKGVersion != 0)
        {
            rollbackBaselineSelected = true;
            TRACE_INFORMATION("[GAME SAVE] Selected rollback baseline (LastKnownGood) v:%llu", bestKGVersion);
            return bestKGManifest;
        }
        TRACE_INFORMATION("[GAME SAVE] RollbackToLastKnownGood requested but no IsKnownGood manifest present. Falling back to latest v:%s", latestFinalizedL.GetVersion().c_str());
        return selectedBaseline; // fallback
    }

    // LastConflict intent
    if ((addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastConflict) == PFGameSaveFilesAddUserOptions::RollbackToLastConflict)
    {
        uint64_t bestLoserVersion = 0; // highest own version among conflict losers
        ManifestWrap bestLoser;
        bool foundLoser = false;
        for (size_t i = 0; i < manifests.size(); ++i)
        {
            const auto& manifest = manifests[i];
            if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
            {
                continue;
            }

            // Conflict metadata lives in the optional Conflict wrapper, not directly on the manifest.
            String conflictingVersion; // empty means no conflict metadata
            bool isWinner = false;
            auto const& conflictOpt = manifest.GetConflict();
            if (conflictOpt.has_value())
            {
                conflictingVersion = conflictOpt->GetConflictingVersion();
                isWinner = conflictOpt->GetIsWinner();
            }

            if (conflictingVersion.empty() || isWinner)
            {
                continue; // need a loser side with conflicting version
            }

            uint64_t thisVer = StringToUint64(manifest.GetVersion());
            // Service defines "last conflict loser" as the loser with the highest own version.
            // Use the loser's own version as the ranking key (not max of pair versions).
            if (thisVer > bestLoserVersion)
            {
                bestLoserVersion = thisVer;
                bestLoser = manifest;
                foundLoser = true;
            }
        }
        if (foundLoser)
        {
            rollbackBaselineSelected = true;
            uint64_t chosenVer = StringToUint64(bestLoser.GetVersion());
            TRACE_INFORMATION("[GAME SAVE] Selected rollback baseline (LastConflict loser) v:%llu", chosenVer);
            return bestLoser;
        }
        TRACE_INFORMATION("[GAME SAVE] RollbackToLastConflict requested but no conflict loser manifest present. Falling back to latest v:%s", latestFinalizedL.GetVersion().c_str());
        return selectedBaseline; // fallback
    }

    // No rollback intent
    return selectedBaseline;
}

void LockStep::CreateInitManifestRequest(
    _In_ const Entity& entity, 
    _In_ InitializeManifestRequest& request, 
    _In_ uint64_t baseManifestVersion,
    _In_ uint64_t newManifestVersion,
    _In_ uint64_t manifestVersionOffset,
    _In_ const String& saveFolder)
{
    UNREFERENCED_PARAMETER(entity);
    //request.SetEntity(entity.EntityKey());
    request.SetVersion(Uint64ToString(newManifestVersion + manifestVersionOffset));
    if (newManifestVersion == baseManifestVersion)
    {
        // keep base version in sync with the manifest version as we increment it
        request.SetBaseVersion(Uint64ToString(baseManifestVersion + manifestVersionOffset));
    }
    else
    {
        request.SetBaseVersion(Uint64ToString(baseManifestVersion));
    }

    PlayFab::Wrappers::PFGameSaveManifestMetadataWrapper<Allocator> metadata;
    metadata.SetDeviceId(GetLocalDeviceID(saveFolder));
    metadata.SetDeviceName(GetDeviceFriendlyName());
    metadata.SetDeviceType(GetDeviceType());
    metadata.SetDeviceVersion(GetDeviceVersion());
    request.SetMetadata(std::move(metadata));
}

} // namespace GameSave
} // namespace PlayFab

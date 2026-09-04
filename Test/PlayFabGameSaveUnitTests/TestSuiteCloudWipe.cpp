// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// REGRESSION TESTS for administrative cloud wipe handling.
//
// Scenario:
//   Support deletes all of a player's cloud game save state - the Game Manager "Delete all"
//   button, or POST /GameSave/DeleteAllManifests, which runs CleanupPlayerExecutor server-side
//   and removes every blob, every manifest (including Quarantined and PendingDeletion), and the
//   file map. This is done to reset a player whose cloud chain is broken beyond rollback.
//
//   The player's console is untouched. Their save files are still on disk, and so is
//   cloudsync\localstate.json, which still describes the pre-wipe cloud.
//
// The defect:
//   On the next activation the client deletes every local save file, silently.
//
//   CompareStep.cpp:53      - no remote manifest, so !HasVersion() skips ahead to
//                             ReadLocalManifest. There is no "cloud is empty, treat as a new
//                             save" branch; an empty cloud is indistinguishable from a cloud
//                             with nothing to download.
//   CompareStep.cpp:251-267 - localstate.json survived, so every lastSyncFileSize is non-zero,
//                             hasValidLastSyncData is true, and ReconcileMetadataLoss is never
//                             called. (It would return immediately anyway - :414 bails when the
//                             remote set is empty.)
//   CompareStep.cpp:891     - MarkFilesToDeleteUponDownload matches on
//                             "lastSyncFileSize != 0 && remoteFile == nullptr". After a wipe
//                             that is true for EVERY local file. The comment reads "if there's
//                             last sync data but no remote file, then remote file was deleted"
//                             - which is exactly the wrong conclusion here.
//   CompareStep.cpp:819     - ScanForConflicts needs remote *changes* to intersect local
//                             uploads. An empty cloud produces none, so conflictFound stays
//                             false and no conflict UI is raised to warn the player.
//   FolderSyncManager.cpp:332 - DeleteFiles runs on the download path, before any upload can
//                             occur. MarkFilesToTransferUponUpload does queue the files, but
//                             that queue is only drained while uploading.
//
//   Net effect: the operation intended to give the player a clean slate instead destroys the
//   offline progress that was the only surviving copy.
//
//   Note the mirror case IS handled - CompareStep.cpp:240 detects local storage wiped
//   externally while cloud metadata survived, and calls localFileFolderSet->Clear(). There is
//   no equivalent for cloud-wiped-while-local-survives.
//
// The safe sequence:
//   Remove cloudsync\localstate.json along with the cloud state, keeping the save files. That
//   zeroes the lastSync baseline, which suppresses the deletion, marks every file as changed
//   for upload, and still raises no conflict. WipeCloudAndLocalStatePreservesLocalSave covers
//   this and should pass both before and after any client fix.
//
// Tracked by Bug 63589697. The two tests that fail against current code are checked in with
// gtest's DISABLED_ prefix, so the suite is green for everyone else. Remove the prefix to
// verify a fix.
//
// Expected results:
//   DISABLED_WipeCloudOnlyPreservesLocalSave         - FAILS today (documents the bug)
//   DISABLED_WipeCloudOnlyThenUploadRepopulatesCloud - FAILS today (data is gone by upload time)
//   WipeCloudOnlyDoesNotPromptForConflict            - PASSES now; guards the silence
//   WipeCloudAndLocalStatePreservesLocalSave         - PASSES now; proves the workaround
//   WipeCloudOnlyWithNoLocalFilesIsClean             - PASSES now; no-local-data control
//   RemoteFileDeletionStillReplicates                - PASSES now; guards against overcorrection

#include "pch.h"
#include "actions.h"
#define USE_PIX
#include "pix3.h"
#include "MemoryManager.h"

class TestSuiteCloudWipe : public testing::Test
{
protected:
    void SetUp() override
    {
        reset_all();
    }

    void TearDown() override
    {
        pfgamesave_shutdown(true);
        reset_all();
        TEST_COUT << MemoryManager::Instance().Summary();
    }
};

namespace
{

// Establishes a player who has synced successfully and therefore has both cloud state and a
// populated local lastSync baseline. Leaves the device shut down, as it would be between
// sessions.
void SeedSyncedPlayer()
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(write_file("save01", "career.dat", "career-progress-v1"));
    AHS(write_file("save01", "garage.dat", "garage-contents-v1"));
    AHS(write_file("save02", "settings.dat", "settings-v1"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 1));
    AHS(pfgamesave_upload());

    // The upload wrote localstate.json with real lastSync data. That baseline is the precondition
    // for the defect, so assert it rather than assuming it.
    AHS(verify_localstate_has_sync_baseline("DeviceA", true));
}

} // anonymous namespace

// The core regression. A cloud wipe must not delete the player's local save.
//
// Against current code this fails at the first verify_file: MarkFilesToDeleteUponDownload
// queued all three files and DownloadStep::DeleteFiles removed them during activation.
//
// DISABLED_ pending the fix for Bug 63589697.
TEST_F(TestSuiteCloudWipe, DISABLED_WipeCloudOnlyPreservesLocalSave)
{
    SeedSyncedPlayer();

    // Support wipes the cloud. The console is not touched.
    AHS(debug_wipe_cloud_only());
    AHS(verify_localstate_has_sync_baseline("DeviceA", true));

    // Player launches the game again. resetCloud=false so the device folder survives, which is
    // what makes this a wipe rather than a fresh install.
    AHS(pfgamesave_download("DeviceA", false));

    // The save must still be here. The cloud being empty is not evidence that the player
    // deleted anything.
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save01", "career.dat", "career-progress-v1"));
    AHS(verify_file("save01", "garage.dat", "garage-contents-v1"));
    AHS(verify_file("save02", "settings.dat", "settings-v1"));
}

// The deletion is silent - no conflict UI is raised. This test passes against current code and
// exists to pin that behavior down: if a future change starts prompting here, that is a
// meaningful behavioral shift and should be a deliberate decision, not a surprise.
//
// It also documents why the bug is so damaging. A prompt would at least give the player a
// chance to choose "keep local".
TEST_F(TestSuiteCloudWipe, WipeCloudOnlyDoesNotPromptForConflict)
{
    SeedSyncedPlayer();

    AHS(debug_wipe_cloud_only());

    g_gameState.uiConflictCallbackTriggered = false;
    g_gameState.uiConflictCallbackExpected = false;

    AHS(pfgamesave_download("DeviceA", false));

    // ScanForConflicts cannot fire: an empty cloud contributes no changed remote folders, so
    // there is nothing for the local upload set to collide with.
    ASSERT_FALSE(g_gameState.uiConflictCallbackTriggered);
}

// The documented workaround. Removing the lastSync baseline along with the cloud state makes
// the wipe safe, because MarkFilesToDeleteUponDownload's "lastSyncFileSize != 0" guard no
// longer matches.
//
// This passes against current code. It is the operational recipe, and it should keep passing
// after any fix.
TEST_F(TestSuiteCloudWipe, WipeCloudAndLocalStatePreservesLocalSave)
{
    SeedSyncedPlayer();

    AHS(debug_wipe_cloud_only());
    AHS(delete_localstate("DeviceA"));
    AHS(verify_localstate_has_sync_baseline("DeviceA", false));

    AHS(pfgamesave_download("DeviceA", false));

    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save01", "career.dat", "career-progress-v1"));
    AHS(verify_file("save01", "garage.dat", "garage-contents-v1"));
    AHS(verify_file("save02", "settings.dat", "settings-v1"));
}

// End-to-end intent: after a wipe the player's local progress should make it back to the cloud,
// so a second device sees it. This is what support actually wants from "reset their save".
//
// Against current code this fails - the files are deleted at activation, so the upload has
// nothing to push and DeviceB downloads an empty save.
//
// DISABLED_ pending the fix for Bug 63589697.
TEST_F(TestSuiteCloudWipe, DISABLED_WipeCloudOnlyThenUploadRepopulatesCloud)
{
    SeedSyncedPlayer();

    AHS(debug_wipe_cloud_only());

    // Player launches, plays, and the game syncs.
    AHS(pfgamesave_download("DeviceA", false));
    AHS(pfgamesave_upload());

    // A different device should now see the recovered save.
    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 2));
    AHS(verify_num_files("save02", 1));
    AHS(verify_file("save01", "career.dat", "career-progress-v1"));
    AHS(verify_file("save01", "garage.dat", "garage-contents-v1"));
    AHS(verify_file("save02", "settings.dat", "settings-v1"));
}

// Control test. A wipe against a player with no local files must stay clean and must not error.
// This is the population where an administrative wipe is genuinely safe, and it passes both
// before and after a fix - useful for confirming a fix doesn't overcorrect into resurrecting
// files that really were deleted remotely.
TEST_F(TestSuiteCloudWipe, WipeCloudOnlyWithNoLocalFilesIsClean)
{
    AHS(pfgamesave_download("DeviceA"));
    AHS(pfgamesave_upload());

    AHS(debug_wipe_cloud_only());

    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_num_files("save01", 0));
}

// Guards the distinction the fix must preserve: a genuine remote deletion still has to
// replicate. If a fix suppresses MarkFilesToDeleteUponDownload too broadly - for example by
// ignoring the "remoteFile == nullptr" case whenever anything is missing rather than only when
// the manifest is absent entirely - this test catches it.
//
// DeviceB deletes a file and uploads; DeviceA must pick that deletion up. Passes today.
TEST_F(TestSuiteCloudWipe, RemoteFileDeletionStillReplicates)
{
    SeedSyncedPlayer();

    AHS(pfgamesave_download("DeviceB"));
    AHS(verify_num_files("save01", 2));
    AHS(delete_file("save01", "garage.dat"));
    AHS(pfgamesave_upload());

    // DeviceA must honor the deletion - the cloud manifest still exists here, it simply no
    // longer lists the file.
    AHS(pfgamesave_download("DeviceA", false));
    AHS(verify_num_files("save01", 1));
    AHS(verify_file("save01", "career.dat", "career-progress-v1"));
}

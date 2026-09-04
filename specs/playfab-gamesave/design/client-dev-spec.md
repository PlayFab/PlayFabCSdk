
# PFGameSave client dev spec

# Contents

[PFGameSave client dev spec
](#pfgamesave-client-dev-spec)

[Design vision ](#design-vision)

[Client SDK architecture
](#client-sdk-architecture)

[Title Adoption Requirements
](#title-adoption-requirements)

[Ship Vehicle ](#ship-vehicle)

[High Level Logic - Download
](#high-level-logic---download)

[High Level Logic - Upload
](#high-level-logic---upload)

[Public API ](#public-api)

[PFGameSave Service Manifest Object
](#pfgamesave-service-manifest-object)

[PFGameSave Service Endpoints
](#pfgamesave-service-endpoints)

[The Download Dance ](#the-download-dance)

[The Upload Dance ](#the-upload-dance)

[Conflicts ](#conflicts)

[Atomic Unit ](#atomic-unit)

[Delete Replication ](#delete-replication)

[Compression ](#compression)

[Releasing Active Device
](#releasing-active-device)

[Resuming Uploads ](#resuming-uploads)

[Rollback ](#rollback)

[Offline Support ](#offline-support)

[Game Save Limits ](#game-save-limits)

[Extended Manifest ](#extended-manifest)

[Local State JSON (not final)
](#local-state-json)

[Open Questions ](#open-questions)

[A) GDK Interaction ](#a-gdk-interaction)

[GRTS / Xbox specifics ](#grts-xbox-specifics)

Author: jasonsa@microsoft.com

## Design vision 

Imagine PlayFab version of XGameSaveFiles but also:

- Adds support for cross platform and cross network game saves

- Adds support for compression

  - Avoids uploading 100s of files to Azure one at a time

- Adds support for rollback

  - Rollback UX and API will be simple

- Adds support for completing upload even during active device
  contention conditions

- Similar UI and similar public API to XGameSaveFiles

## Client SDK architecture

![diagram](media/image1.png)

Another high-level stack view of the components:  
  
![diagram](/media/image4.png)

Threat model for inproc:

![diagram](/media/image5.png)

Threat model for out of proc (xgameruntime):

![diagram](/media/image6.png)

## Title Adoption Requirements

Games that are Windows based device (Xbox / PC) only:

- No additional requirements.

- GRTS/OS flash (green box above) handles out of process upload and UI
  just as it does today for XGameSaves

To enable cross network game saves:

- Title must use cross plat identity provider (EA / Uplay / etc.) that
  supports OpenID to get same Playfab user ID between platforms

  - For 1pp, they can use Xbox Live as its cross platform.

  - TBD: Any other options? MSA? Link code?

- On non-Steam 3PP platforms, UI provided by game dev and show upon
  callback

  - 3PP platform UI might be provided in future release

- On non-Steam 3PP platforms, upload is in-process so upload must
  complete before user exits game

  - Therefore, game dev must prompt user for upload and show progress UX

  - OR optimally design game save to be small enough to upload to cloud
    seamlessly during save

- Game dev must plan on various storefronts shipping title updates out
  of lockstep but support shared cloud data moving between these
  different versions of game.

  - Like the need of a cross network multiplayer game

## Ship Vehicle 

Follows design discussed in [SDK Merge
document.](https://microsoft.sharepoint.com/:w:/t/GamingDeveloperExperiencesGDX/EbJSszwohLVBmZlPSApE_cwBokErLPWnJUmuYvCVUYLjpw?e=aWrcgJ&CID=d32ef568-3be5-66b8-03de-7d6bcd99ea63)
Namely:

- GDK: ship as Extension SDK in GDK

- NDA Platforms: ship in an access protected ADO artifact stream

- Other Platforms: ship in PlayFab "merged" SDK package on GitHub

## High Level Logic - Download 

Download with PFGameSaveFilesAddUserWithUiAsync

- Always call regardless if offline or not

Internally this API, does these steps:

1.  Active device acquisition w/ active device contention UX if needed

2.  Comparison and merger of metadata w/ conflict UX if needed

3.  Download w/ progress UX if needed

Any 5xx HTTP status or network failures at any part of this process will
pop a sync error UX with [Retry] button and [Use Offline] button. It
will internally handle 401 to refresh expired tokens

PFGameSaveFilesGetFolder returns save game path:

- All files & folders written by game inside this folder will be roamed

## High Level Logic - Upload 

Upload with PFGameSaveFilesUploadWithUIAsync

Active device already obtained from download step

Internally this API, does these steps:

1.  Comparison and merger of local files and cloud metadata

2.  Upload w/ progress UX

Active device optionally released after upload since game can upload
again as desired or quit via main menu.

Any HTTP status or network failures at any part of this process will pop
a sync error UX with [Retry] button and [Cancel] button.

## Public API 

See [PFGameSave API
Specification.docx](https://microsoft.sharepoint.com/:w:/t/Endpoints/ETazqe_vN6dCsyPFVtcIV4ABqSnD_DOXDoxoa_U42czAHQ?e=KJlmNJ)

### Implemented Public APIs

The following public APIs are implemented in `Source/PlayFabGameSave/Include/playfab/gamesave/`:

**Lifecycle:**
- `PFGameSaveFilesInitialize(PFGameSaveInitArgs* args)` -- Initialize the library. `PFGameSaveInitArgs` includes an optional `saveFolder` for non-Xbox platforms and `options` flags.
- `PFGameSaveFilesUninitializeAsync` / `PFGameSaveFilesUninitializeResult` -- Cleanup.

**User & Sync:**
- `PFGameSaveFilesAddUserWithUiAsync` / `PFGameSaveFilesAddUserWithUiResult` -- Add user and download. Accepts `PFGameSaveFilesAddUserOptions` (None, RollbackToLastKnownGood, RollbackToLastConflict).
- `PFGameSaveFilesUploadWithUiAsync` / `PFGameSaveFilesUploadWithUiResult` -- Upload. Accepts `PFGameSaveFilesUploadOption` (KeepDeviceActive, ReleaseDeviceAsActive).

**Query:**
- `PFGameSaveFilesGetFolderSize` / `PFGameSaveFilesGetFolder` -- Get save root folder path.
- `PFGameSaveFilesGetRemainingQuota` -- Returns remaining cloud storage quota in bytes (negative = over quota). Returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` when offline.
- `PFGameSaveFilesIsConnectedToCloud` -- Returns whether user is in ConnectedToCloud mode.

**Metadata:**
- `PFGameSaveFilesSetSaveDescriptionAsync` / `PFGameSaveFilesSetSaveDescriptionResult` -- Set a short save description shown in conflict/contention UI. Works offline (stored locally with dirty flag, committed on next upload).
- `PFGameSaveFilesSetActiveDeviceChangedCallback` -- Register callback for when another device becomes active (title should return to main menu).

**Dev/Test:**
- `PFGameSaveFilesResetCloudAsync` / `PFGameSaveFilesResetCloudResult` -- Reset cloud state for a user. Does not alter local saves.

### PFGameSaveDescriptor

The `PFGameSaveDescriptor` struct is passed to UI callbacks (conflict, active device contention) and the active device changed callback:

```c
typedef struct PFGameSaveDescriptor
{
    time_t time;                          // Relevant time of the descriptor
    uint64_t totalBytes;                  // Total bytes of the save
    uint64_t uploadedBytes;               // Size of pending upload, if any
    char deviceType[256];
    char deviceId[256];
    char deviceFriendlyName[256];
    char thumbnailUri[2048];              // URI to pfthumbnail.png if it exists at root
    char shortSaveDescription[4096];      // Set via PFGameSaveFilesSetSaveDescriptionAsync
} PFGameSaveDescriptor;
```

### Thumbnail Support

If a file named `pfthumbnail.png` exists at the game save root folder, the SDK
treats it specially: it is uploaded first (outside the normal zip batches) and
its cloud URI is exposed via `PFGameSaveDescriptor.thumbnailUri` in UI callbacks.

### Sync State Enum

Progress callbacks receive a `PFGameSaveFilesSyncState` value:

```c
enum class PFGameSaveFilesSyncState : uint32_t
{
    NotStarted = 0,
    PreparingForDownload,
    Downloading,
    PreparingForUpload,
    Uploading,
    SyncComplete
};
```

### Error Codes

Game-save-specific HRESULT error codes (defined in `PFGameSaveFilesTypes.h`):

| Error | Value | Description |
|-------|-------|-------------|
| `E_PF_GAMESAVE_USER_CANCELLED` | 0x800704c7 | User cancelled a UI dialog |
| `E_PF_GAMESAVE_NOT_INITIALIZED` | 0x89237000 | `PFGameSaveFilesInitialize()` not called |
| `E_PF_GAMESAVE_ALREADY_INITIALIZED` | 0x89237001 | Already initialized without uninit |
| `E_PF_GAMESAVE_USER_ALREADY_ADDED` | 0x89237002 | `AddUserWithUiAsync` called again for same user |
| `E_PF_GAMESAVE_USER_NOT_ADDED` | 0x89237003 | `AddUserWithUiAsync` not called or not completed |
| `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` | 0x89237004 | User is in offline/disconnected mode |
| `E_PF_GAMESAVE_NETWORK_FAILURE` | 0x89237005 | Network failure during upload while connected |
| `E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS` | 0x89237006 | Upload called before download completed |
| `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE` | 0x89237007 | Local device lost active status |
| `E_PF_GAMESAVE_DISK_FULL` | 0x89237008 | Not enough local disk space for download |

### UI Callbacks

Set via `PFGameSaveFilesSetUiCallbacks(PFGameSaveUICallbacks*)`. On Xbox/Windows,
stock platform UI is provided; on other platforms these callbacks are required for
game-rendered UI. Each callback has a corresponding `SetUi*Response()` API that
must be called (inside or outside the callback) for the state machine to progress.

| Callback | Triggered During | User Actions |
|----------|------------------|--------------|
| `PFGameSaveFilesUiProgressCallback` | Download & Upload | Cancel |
| `PFGameSaveFilesUiSyncFailedCallback` | Download & Upload | Cancel, Retry, UseOffline (download only) |
| `PFGameSaveFilesUiActiveDeviceContentionCallback` | Download | Cancel, Retry, SyncLastSavedData |
| `PFGameSaveFilesUiConflictCallback` | Download | Cancel, TakeLocal, TakeRemote |
| `PFGameSaveFilesUiOutOfStorageCallback` | Download | Cancel, Retry |

## PFGameSave Service Manifest Object

From Rakesh's PF Game Save service spec: [PlayFab Game
Saves.docx](https://microsoft-my.sharepoint.com/:w:/r/personal/ravarna_microsoft_com/Documents/PlayFab%20Game%20Saves.docx?d=w5b87c291fb5b425d98ad892c2bf907a3&csf=1&web=1&e=tOnSWo)

This is a central object to the design of the game save system.

Manifest object

- Version \#

- Device ID

- Status

1.  Initialized

2.  Uploading

3.  Finalized

4.  PendingDeletion

- Create/finalize timestamps

- UploadProgress

  - Current bytes uploaded

  - Total bytes to upload

- Flat list of files (which might be compressed or uncompressed). Each
  file:

  - Name

  - Size compressed

  - Size uncompressed

## PFGameSave Service Endpoints

From Rakesh's PF Game Save service spec: [PlayFab Game
Saves.docx](https://microsoft-my.sharepoint.com/:w:/r/personal/ravarna_microsoft_com/Documents/PlayFab%20Game%20Saves.docx?d=w5b87c291fb5b425d98ad892c2bf907a3&csf=1&web=1&e=tOnSWo)

List of service endpoints to support this feature:

1.  ListManifests() -> Manifest[]

2.  GetManifestDownloadDetails( Version ) -> Download URLs

3.  InitializeManifest( OldVersion, DeviceId ) -> New manifest in
    "Initialized" State

4.  InitiateUpload( Version, \# Upload URLs ) -> Manifest in
    "Uploading" State, Upload URLs

5.  FinalizeManifest( Version, Delta FileDetails ) -> Manifest in
    "Finalized" State

6.  UpdateManifest( Version, UploadProgress ) -> updated manifest

7.  DeleteManifest( Version ) -> Manifest in "PendingDeletion" State

## The Download Dance

This all happens inside PFGameSaveFilesAddUserWithUiAsync

1.  ListManifests()

    1.  Find highest finalized version in Finalized state.

    2.  Find highest pending version in [Initialized|Uploading] state
        that's higher Finalized version.

        1.  If found, is DeviceId matching?

            1.  If yes: (aka reconnect flow). Skip creating new
                Initialized via InitializeManifest (reuse this manifest)

            2.  If no: show active device contention UI,

                1.  If user breaks and becomes active device,
                    InitializeManifest as normal

                2.  If user cancels, return failure

2.  InitializeManifest( Highest finalized version + 1, DeviceId ) which
    returns a new pending version manifest in "Initialized" State

    1.  This is the "active device" or in XGF called the "lock"

3.  GetManifestDownloadDetails( Highest finalized version) which returns
    download URLs

4.  Download "extended-<version>-manifest.json" first. Well known
    file name

    1.  This details files & folders inside each compressed file

5.  Compare extended-<version>-manifest.json against local
    files/folders/state to determine which other files (likely
    compressed zips) to download.

    1.  Show conflict UX if needed (pending upload but also needs to
        download)

6.  Download each compressed file from Azure w/ progress UX

7.  Uncompress files into appropriate folders, and delete each zip it is
    extracted

## The Upload Dance

This all happens inside PFGameSaveFilesUploadWithUIAsync

1.  Gets exclusive read access on files and folders where possible
    (released when async callback finishes)

2.  InitiateUpload( pending version, \# Upload URLs ) -> Manifest in
    "Uploading" State, \# of Upload URLs

3.  Compare local folders to Highest finalized version (previous
    downloaded this) remote state
    ("extended-<version>-manifest.json") to figure which
    uncompressed files need to be uploaded

    1.  Compress the uncompressed files

    2.  Reuse existing cloud compressed files where possible.

    3.  Extended manifest should marking old uncompressed original files
        as dead

    4.  pending version extended manifest should be created, gzip'd and
        added to upload in step 5 below

4.  Upload compress files w/ progress UX occasionally calling:

    1.  UpdateManifest( pending version, UploadProgress ) -> updated
        manifest

5.  When done, call FinalizeManifest( pending version, FileDetails )
    -> Manifest in "Finalized" State

    1.  At same time get create a new Initialized manifest with version
        +1 higher than the call to finalize which to retain active
        device & update local state

    2.  See "Releasing Active Device" section for detail how/when to
        cleanup this pending manifest to avoid active device contention
        when naturally changing devices

## Conflicts

After uploading, the local state file is updated with each file's last
uploaded file size and date modified. This data in the local state file
is used to detect if the file has changed since the last upload.

How could the file change after an upload? It could be the game saved
again but didn't start an upload (eg. power lost, network lost, crash,
etc), or the local file was changed between game sessions on PC.

During download, if the file(s) didn't change then it can safely pull
down the latest changes from the cloud. However, if the file(s) did
change (aka the file timestamp changed since last upload) AND there are
cloud changes to download to the same file(s) then there's a sync
conflict and a conflict dialog is shown (via UX callback) to ask the
user if they wish to keep the local or cloud data. The choice is only
made once and is applied to all files.

It's important to note that this means when a conflict happens, the user
will either end up with the local state unchanged if they selected "Take
Local' or they will end up with an exact mirror of the cloud state if
they selected "Take Remote". There's no complex merging when there's a
conflict.

If there's not a conflict found, then each root level subfolder will
take any changes from the cloud that has not been downloaded yet. In
this no conflict scenario, some root level subfolders may have local
changes that have not yet been sent to the cloud but it also means the
files/folders in that root level subfolder were not changed on the
cloud.

The best practice is for game studio to group related game save data
into separate root level subfolders as that will help reduce conflict
pops and ensure that the game supports the merging behavior as described
in this section.

Note that files that still need to be uploaded when the download API is
called are not uploaded until the upload API is called regardless of if
there's a conflict or not. This matches existing Xbox XGS behavior

Simple chart summary:

| Local Changes | Remote Changes | Operation |
|---|---|---|
| N | N | No op |
| N | Y | Download |
| Y | N | Upload needed |
| Y | Y | Conflict |

Conflicts detection is described next using atomic units.

## Atomic Unit

There are various choices of atomic unit possible. Some options
considered:

| A | Resolve at individual file level |
|---|---|
| B | Resolve at every folder level (ie container, any folder that has files) |
| C | Resolve at root subfolder(s) |
| D | Resolve at root level (aka when any change occurs anywhere) |

For each of these atomic units, consider this chart:

![diagram](media/image2.png)

The existing XGameSavesFiles uses option B, and we believe option D
would avoid incorrect mergers but lead to customers being prompted for
merge conflict far more often, and option A isn't realistic for game
saves due highly likely of intercoupling of dependances between files in
the same folder.

Therefore, we will implement C by default.

If desired later, we will will add support B via a new flag to
PFGameSaveInitOptions:

> enum class PFGameSaveInitOptions : uint32_t
>
> {
>
> None = 0x00, // options might be added in future release
>
> };
>
> PF_API PFGameSaveFilesInitialize(_In_ PFGameSaveInitArgs\* args)
> noexcept;

## Delete Replication

Deleted files are also replicated. During download, if a file was
deleted on the cloud and the local file hasn't changed according to
local state (see Conflicts section), then the local file is also
deleted.

In a similar fashion a deleted file can also cause a conflict in the
same manner as described previously in the Conflicts section. If during
download, the file changed (it existed during last upload but now its
deleted), and the cloud has a different version of the file then there's
a conflict with need to show a conflict dialog to ask the user to
resolve.

## Compression

Given all the files that need to be uploaded, it splits the list into
sets of 64MB max.

Each set is zipped and uploaded as a single zip and added to the service
manifest.

Files inside zips that were previously uploaded will be reused without
uploading and added to manifest. If file(s) inside a previous zips need
to be uploaded, then those file(s) are marked in the external manifest
as "skipped". If all the files of a previous zip are marked as skipped,
that zip is no longer needed and not added manifest when it is
finalized.

Example 1: Game writing/overwrite a single save file (eg Elden Ring)

- Game writes all save data to a single file: slot1\gamesave.dat

- Upon upload, a zip is created with slot1\gamesave.dat, uploaded and
  added to manifest.

- Each save overwrites slot1\gamesave.dat, so next upload will mark
  previous file as skipped and previous zip is no longer in manifest as
  all files are skipped.

- Manifest will contain the new single zip.

Example 2: Game writing to new folder/files upon each save (eg
Starfield)

- Game writes to new folder/files upon each new save folder taking
  around 10MB of data.

- Upon upload, a zip is created with the files in the new folder(s), and
  it's uploaded and added to manifest.

- Previous zips are included as those are still present in manifest
  (until eg they are deleted by user).

In example 2, the number of zips in the manifest might grow up until
quota limits are hit.

In a pathological version of case 2, each save could be very small, and
the user might make frequent cloud uploads which in turn leads to a lot
of zips in the manifest over the lifetime of the game.

To minimize the number of zips in the manifest, we could compact by
re-uploading all files in 1 or more previously uploaded zips to remove
them from the manifest.

There's a cost to uploading data that's previously been uploaded (user
time, user bandwidth, cloud storage costs), but there's a cost savings
on the service when we remove a zip from the manifest.

We need to know what the right tradeoff is in the client to know what
when to compact.

If re-uploading 1KB of data can remove a zip from the manifest, is that
worth the cost? 1MB? 10MB? 64MB?

## Releasing Active Device

As discussed, the active device is taken by creating a new manifest in
the "Initialized" state during the download logic. When uploading this
manifest is used and finalized, and a new manifest in the "Initialized"
state is created so the current device retains the active device
allowing the user to continue uploading as needed.

The question then is when is the active device ever released? Optimally
it'd be released after the gamer has uploaded and is done playing on
that device (aka no more progress will be made in the current session).
However, the SDK component does not know if/has the user will make more
progress in the current session, and it's not reliable to try to release
the active device when the title suspends or exits as that's a very fast
operation with no guarantee of an HTTP call succeeding during the
suspend/terminate event.

If the user switches to a device on the same platform, then likely the
platform's cloud save will have sync'd the game state between the
devices resulting no Playfab Cloud Game Save sync needed. But switching
between platforms will result in a user experience that that will pop a
active device contention dialog which looks something like this:

> Your other device is taking a long time to sync to the cloud. Do you
> want to sync the last saved data we have in the cloud? This will
> cancel your other sync.
>
> To have us check if the other device has finished syncing, choose:
> [Try Again] or [Sync Last Saved Data]

The current design is that we will expose a flag during upload that lets
the game dev signal to us when it knows the user is going back to main
menu and we should in turn release the device as device.

## Resuming Uploads

If an upload is aborted (game crashes, system powers off, game
exits/suspends, etc), then in the in-proc client the upload will not be
automatically resumed. If the game wishes to continue uploading, then
the game dev can call the Upload API again when it's ready to do so.

Upon calling the Upload API, resuming a partial upload is desired and
will be explored to see if it's reasonably possible in the first version
of the product.

## Rollback

When calling PFGameSaveFilesAddUserWithUiAsync, the game dev can pass
a rollback option via `PFGameSaveFilesAddUserOptions` to sync to a
non-latest finalized manifest. Two rollback modes are available:

- `PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood` (0x01) -- Syncs
  using the most recently verified prior cloud save state (a previously
  loaded save that was later replaced by a newer upload). Use when you
  suspect the latest upload is bad (e.g. load failure, failed integrity /
  version check, crash during or immediately after save, player reports
  corrupted / regressed state) and you want to automatically recover to
  the last known good manifest. If no earlier verified state exists this
  behaves like `None` and the current latest is kept.

- `PFGameSaveFilesAddUserOptions::RollbackToLastConflict` (0x02) -- Syncs
  using the save state that was kept aside (the "losing" choice) from the
  most recent conflict resolution. Use when you determine (or the player
  reports) that they chose the wrong side during the last conflict
  resolution dialog and you want to restore the alternate snapshot. If no
  preserved conflict snapshot remains this quietly falls back to the
  current latest.

If either flag is passed the same Download dance is used except instead of
using the latest finalized manifest, it syncs to the appropriate previous
finalized manifest selected by the rollback mode. These are advanced
options typically exposed by the game in a menu if the functionality is
desired.

## Offline Support

When the game has a connected network and
PFGameSaveFilesAddUserWithUiAsync() completes successfully then the
system is in "ConnectedToCloud" mode (discussed below).

When network is unavailable and PFGameSaveFilesAddUserWithUiAsync() is
called, it triggers a PFGameSaveFilesUiSyncFailedCallback callback. The
user can respond with one of the following by calling
PFGameSaveFilesSetUiSyncFailedResponse():

a\) PFGameSaveFilesUiSyncFailedUserAction::Retry. This tries the network
call again and loops back to another UI failure callback if still
offline

b\) PFGameSaveFilesUiSyncFailedUserAction::Cancel.
PFGameSaveFilesAddUserWithUiResult() returns
E_PF_GAMESAVE_USER_CANCELLED the system acts like
PFGameSaveFilesAddUserWithUiAsync was never called (eg various APIs
return E_PF_GAMESAVE_USER_NOT_ADDED)

c\) PFGameSaveFilesUiSyncFailedUserAction::UseOffline.
PFGameSaveFilesAddUserWithUiResult() returns S_OK but it will move the
system into "not ConnectedToCloud" mode.

There's a new PFGameSaveFilesIsConnectedToCloud API which returns if
user is in "ConnectedToCloud" mode or not.

While in the "not ConnectedToCloud" mode, the APIs behave as follows:

- PFGameSaveFilesGetFolder() works as expected

- PFGameSaveFilesSetSaveDescriptionAsync() works as expected.The description
  is stored locally in localstate.json and will be committed to cloud on next
  successful upload when the device reconnects.

- PFGameSaveFilesUploadWithUiAsync() returns S_OK immediately, and
  async worker does nothing and async completion returns
  E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD so game can show status to
  user as desired. This error will be returned regardless if the network
  is restored as the system is still considered in "not
  ConnectedToCloud" mode.

- PFGameSaveFilesGetRemainingQuota() returns
  E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD because we won't know data

- PFGameSaveFilesSetActiveDeviceChangedCallback() can be called but will
  never fire or internally poll (because user is offline)

- PFGameSaveFilesAddUserWithUiAsync() can be called again if you want to
  bring the user online. It will show the failure UI again if the
  network is still offline. No need to re-init gamesave fully but you
  can if desired.

While in the "ConnectedToCloud" mode, the APIs behave as follows:

- PFGameSaveFilesAddUserWithUiAsync() cannot be called again until you
  cleanup and re-init gamesave. Doing so will return
  E_PF_GAMESAVE_USER_ALREADY_ADDED

- PFGameSaveFilesGetFolder() works as expected

- PFGameSaveFilesGetRemainingQuota() works as expected

- PFGameSaveFilesSetActiveDeviceChangedCallback() works as expected.If
  there is network fails during polling, it is silently ignored, and
  service will be polled again next timer heartbeat.

- PFGameSaveFilesUploadWithUiAsync() trys to make network calls to
  upload as normal. If this fails with network failure, the failure UI
  callback will be triggered allowing the user to respond with either
  retry or cancel. If the user cancels, the async completion will return
  E_PF_GAMESAVE_USER_CANCELLED, so the game can show the status to
  user as desired. The game can be called
  PFGameSaveFilesUploadWithUiAsync() again as desired without issue and
  if network was restored it'll work as expected.\`

## Game Save Limits

There are various limits that need to be defined (or marked as
unlimited). Some of these could have throttle/usage limits as defined in
service spec.

Cloud quota per title: TBD

File & folder name character limitations: TBD

File & folder name length limitation: TBD

Max nested path length limitation: TBD

Single file max size limitation: TBD

Max size of single zip file: TBD

Max number of zips per manifest: TBD

Max number of (uncompressed) files in game save folder: Unlimited?

Max mem allocated during large file upload: TBD

How often endpoints can be called: TBD

Note that current XGameSaveFiles allows 64MB max per file.

It allows 256MB by default per user/per title cloud quota but that can
be bumped up. Some titles are bumped to 2GB or 4GB.

## Extended Manifest

Design and code shared with GRTS via common source code or shared static
lib for reading/writing

{

"v1": // for forward compat, future SDKs all sdks
must always write "v1" section as designed (until some far-future
GameSave system requires a new min bar). For this design, any new
formatted data needed post ship goes into "v1.1", etc

{

"Files": // these files are same file list as seen in
ListManifests & GetManifestDownloadDetails from service. no other files
should appear in this array (or its a bug)

[

{

"Name": "C60375FB-C783-9C8D-2370-3CC85F203980", //
GUIDs don't have {} and are uppper case

"LastModified": "2024-06-26T17:26:38.000Z", //
ISO8601 UTC timestamp

"FileId": "guid2",

"CompressSize": 20000, // same data as seen in
ListManifests. this is truth

"Size": 40000, // should match sum ExtractedFiles's
array UncompressedSizeBytes

"Compression": "zip", // options: zip, gzip, none.
all lowercase

"Extract": // files inside payload aren't tracked by
service

[

{

"Name": "test.txt", // need to define limits (allowed
chars, length)

"FileId": "guid3",

"FolderId": "folderguid1", // GUID index to which
folder this lives in, see "Folders" array below

"Size": 164834,  
"SkipFile": false, // Used to mark old extracted
files as "dead"

"LastModified": "2024-06-26T17:26:38.000Z" // ISO8601
UTC timestamp

"Created": "2024-06-26T17:26:38.000Z" // Set the
original file create time where possible

},

{

"Name": "test2.txt",

"FileId": "guid4",

"FolderId": "folderguid2", // notice this is same zip
as previous file, but goes into a different folder

"Size": 16483,

"SkipFile": false,

"LastModified": "2024-06-26T17:26:38.000Z", //
ISO8601 UTC timestamp

"Created": "2024-06-26T17:26:38.000Z" // ISO8601 UTC
timestamp

}

]

},

{

"Name": "guid8",

"FileId": "guid5",

"CompressSize": 10000,

"Size": 10000,

"LastModified": "133546813003653325",

"Compression": "none",

"Extract": // if CompressionType == none, it should
have single entry in ExtractedFiles

[

{

"Name": "map1.dat", // real name of uncompressed
file

"FileId": "guid6",

"FolderId": "guid7", // folder of where to put
uncompressed file

"Size": 10000,

"SkipFile": false,

"LastModified": "2024-06-26T17:26:38.000Z", //
ISO8601 UTC timestamp

"Created": "2024-06-26T17:26:38.000Z" // ISO8601 UTC
timestamp

}

]

}

],

"Folders": // note there's a single object in
"Folders". Root folder is not listed

[

{

"Name": "save1", // need to define limits (allowed
chars, length)

"Id": "folderguid1",

"Folders":

[

{

"Name": "subfolder1",

"Id": "folderguid2",

"Folders": []

},

{

"Name": "subfolder2",

"Id": "folderguid3", // notice no file refers to this
folder, so meaning SDK should just a create blank folder during
download.

"Folders": []

}

]

}

]

}

## Local State JSON

The design of this file is similar to old XGameSave containers where it
stores a flat array of folders, and each folder contains a list of
files. This is used internally to track metadata about the files and
folders - namely the last time each file was last sync'd. This lets the
client know if the file locally changed since last sync and thus needs
to be sync'd again.

Design is not sync'd with how GRTS stores its local state, as local
state isn't shared between the clients or sync'd across devices. This
design applies to in-proc only:

{

"Folders":

[

{

"relFolderPath": null,

"folderName": null,

"folderId": "93E41C6E-2091-1B9B-36BC-7CE94EDC677E",

"Files": []

},

{

"relFolderPath": "\\save01",

"folderName": "save01",

"folderId": "32D83BB6-F3AD-985F-D4BC-655B3D9ACBE2",

"Files":

[

{

"relPath": "\\DeviceA\\save01\\save1.dat",

"fileName": "save1.dat",

"fileId": "621D1BFA-0A92-92D9-C0E5-95902A9C3F76",

"lastSyncFileSize": 10000

"lastSyncTimeLastModified": "2024-05-09T11:48:44.000Z",

},

{

"relPath": "\\save01\\save2.dat",

"fileName": "save2.dat",

"fileId": "C29D7042-ECB2-518C-32FA-18A51958EC96",

"lastSyncFileSize": 18,

"lastSyncTimeLastModified": "2024-05-09T11:48:44.000Z",

}

]

}

]

},

"Metadata":

{

"shortSaveDescription": "Level 5 - Boss Fight",

"descriptionDirty": false // true if description was set offline and not yet uploaded to cloud

}

}

## Open Questions

### A) GDK Interaction

Playfab C API in the GDK supports 3 login methods:

PFAuthenticationLoginWithXUserAsync

PFAuthenticationLoginWithCustomIDAsync

PFAuthenticationLoginWithOpenIdConnectAsync

If the title uses LoginWithXUser, then XGameRuntime (GRTS/Console flash)
won't have trouble logging into Playfab on behalf of the title on demand
when the game isn't running.

If the title uses LoginWithCustomId, then XGameRuntime would need the
same customID as the game used so that will need to be passed along if
this is a supported path.

However, we need to consider what happens if the title logins into
Playfab with LoginWithOpenId. If the title isn't running, how will
XGameRuntime get an OpenID token the game would have used for example?
Perhaps we force the game link to XUser in this case, otherwise its not
supported?

## GRTS / Xbox specifics

This is the 'Out Of Proc' specific implementations for our existing
platforms. This implementation will live alongside the existing
ConnectedStorage / XGameSave and will leverage as much of the existing
UX as possible. Game developers can leverage platform tooling to
interact with the storage and delegate the upload to the service
allowing the game to exit quickly.

There are several layers of changes to enable the PlayFab C API. We will
need to provide a set of header/lib updates to xgameruntime.dll so the
game can call the new API. This new layer will invoke the NT service via
a new RPC / COM endpoints. We will tap into the existing storage
management system and partition it to provide PlayFab specific storage,
on Xbox this will include new XVD management for handoff to the game.
The implied portion of being out of proc is that we have the ability to
act on behalf of the user. Via our existing Xbox authentication methods
and LoginWithXbox we can continue to upload even after the game has
closed. The game closing is actually our signal to kick off the upload
process on both xbox and PC, with the Xbox control over Application
Lifetime we can use other state changes to kick off an upload upon
suspend will abandon if the game resumes assuming they are going to
create new data that will invalidate the upload anyway. On PC we will
abandon this upload as well but we only action on terminate or init due
to our lack of control.

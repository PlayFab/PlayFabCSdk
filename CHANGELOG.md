# PlayFab.C Changelog

## Changes from March 21, 2025 (f35a478) to November 7, 2025

### November 2025

#### GitHub Release and Build Improvements (Nov 6, 2025)
- **Removed Android download** from release pipeline
- **Removed Win32 download** from release pipeline
- **Removed Android and tests** to enable GitHub release
- **Updated build order** - SharedInternal now builds first
- **Removed Win32 release** and updated NDK version inconsistencies
- **Updated Android Gradle Plugin** to latest version
- **Fixed GitHub release pipeline** for 2510 PF.C GitHub release

### October 2025

#### Bug Fixes
- **Bug 59468890**: Fixed ShamWow crash on SteamDeck when selecting Prepare after SetUser (Oct 17)
- **Bug 59765924**: Fixed PFGameSave failure in PAL layer when not using XUser on PC (Oct 13)
- **Bug 59671549**: Fixed PFGameSave PG reports failure with E_PF_GAMESAVE_ALREADY_INITIALIZED on resume (Oct 13)
- **Bug 59704068**: Fixed PFGameSave inproc crashes during download (Oct 8)
- **Removed pathcch.lib dependency** which caused build issues in PF.Alls (Oct 8)

#### Features & Improvements
- **Bug 59650019**: Added missing SteamDeck indicator in telemetry (Oct 8)
- **Bug 59467685**: Hooked up GRTS API for ActiveDeviceChanged (Oct 6)
- **Bug 59088914**: Prevented KnownFolders from being set as root folder (Oct 4)
- **Bug 59024384**: Fixed bug when uploading and setting description simultaneously (Oct 3)
- **Added tests for GDK expanded API 2510** (Oct 3)
- **Added PFGameSaves state machine spec** (no code changes) (Oct 3)
- **Fixed missing std::placeholders reference** in TypeWrapperHelpers.h (Oct 2)
- **Bug 59382941**: Added in-game upload differentiator for telemetry (in proc) (Oct 1)

### September 2025

#### Major Features
- **Updated SdkVersion.h for GA** and updated XCurl.lib paths (Sep 30)
- **Updated ADO to use latest LHC** (Sep 30)
- **Deliverable 59176316**: Added 3PP auth support into PFGameSave and hooked up to GRTS API (Sep 26)
- **Disabled unused libarchive compression formats** (Sep 25)
- **Deliverable 52846872**: Added PFGameSave Rollback API additions - GRTS side (Sep 25)
- **Task 59246554**: Removed quota endpoint call (quota now returned by List endpoint) (Sep 24)
- **Deliverable 58976311**: Created cross-platform multidevice test automation suite (Sep 24)
- **Disabled incremental linking** when /PROFILE is enabled to avoid LNK4075 warning (Sep 24)
- **Removed S_OVERRIDE from function definitions** (Sep 20)

#### Platform Updates
- **Removed HC_platform_win32 and win32 projects** (Sep 20)
- **Removed Win32 from pipelines** (Sep 4)
- **Updated SdkVersion.h** (Sep 9)
- **Added missing secretkey APIs for GDK** and typedef guards (Sep 4)

#### Samples & Documentation
- **Added GDK console sample for PFGameSaves** (Sep 2)
- **Fixed missing Steam API** (Sep 2)
- **Added PFGameSaveFilesSetSaveDescriptionAsync** (Sep 1)

### August 2025

#### API Expansion
- **Expanded GDK API Surface for Unified SDK** (Aug 22)
  - New APIs integration
  - Fine-tuned Inventory and Catalog customizations
  - Updated "GDK" references to "Windows" in remarks
- **Adding more tracing around platform init** (Aug 22)
- **Adding debug log tracing to PFGameSave sample** (Aug 21)

#### Build System Improvements
- **Centralized file lists via shared props** and fixed GDK build/link (Aug 20)
- **Fixed Android TestApp to build on release with Gradle** (Aug 18)
- **Added logging around PFGameSaves provider selection** (Aug 12)
- **Fixed undefined HC_PLATFORM in Unreal Plugin** (Aug 9)
- **Updated GameSave telemetry events** to use playfab.gamesave.internal namespace (Aug 8)

#### Testing & Quality
- **Set up legacy C# SDK nuget publish pipeline** (Aug 5)
- **Bug 58635317**: Fixed PFCore not automatically including Xbox auth in PFAuthentication (Aug 4)
- **Enabled NuGet Central Package Management** (Aug 4)
- **Added AI review and unit tests** (Aug 4)
- **Updated GDK artifact zip** during PRs to sit on top of existing GDK install (Aug 4)

### July 2025

#### Major Deliverables
- **Deliverable 58322635**: Support for in-proc game saves upload on Windows to enable 3PP (Jul 31)
- **Task 58362960**: Created SDK sample to verify Steamdeck / Windows paths (Jul 30)
- **Task 58551113**: Renamed Android LocalUser away from Platform (Jul 29)
- **Bug 58536053**: Cleaned up tests and dead GDK gamesave files (Jul 29)

#### Bug Fixes
- **Task 58363828**: Only extract zips after all are downloaded to avoid network errors (Jul 28)
- **Bug 58535428**: Fixed string copy crash in PFGameSaves.dll on out of proc path (Jul 28)
- **Bug 58363091**: Fixed PFGameSaveFilesUploadWithUiAsync crash with nullptr queue (Jul 28)
- **Bug 58472486**: Added console handling to GRTS detection (Jul 24)

#### Pipeline & Documentation
- **Restored PRs back into main** after 2510 preview (Jul 14)
- **Updated generate reference docs script** (Jul 10)
- **Bug 58219891**: PFGameSaves now takes in saveFolder for SteamDeck support in init args (Jul 2)
- **Bug 58180984**: PFGameSaves now reads ForceUseLocalServices regkey to use inproc (Jul 1)

### June 2025

#### Core Features
- **Removed JKS file** to fix PR OB pipeline (Jun 25)
- **MacOS image update** (Jun 20)
- **Task 58004023**: Deduped Core PFErrors generation (Jun 17)
- **Bug 57963808**: Fixed Gamesave init API regression (Jun 16)

#### Exception Handling & Steam API
- **Added exception handling helpers to all public APIs**, including auto-generated ones (Jun 12)
- **Task 57845165**: Cleaned up ctor/dtor of Steam API (Jun 12)
- **Added exception handling helpers to GameSave public APIs** (Jun 11)
- **Task 57845165**: Bug fixes for Steam API (Jun 11)
- **Task 57706665**: Added Steam API to PF Core GDK binary (Jun 10)
- **Removed dead files** (Jun 10)

#### LocalUser Updates
- **Bug 57819072**: Added missing replacement for GetPlatformContext for LocalUser_Xbox (Jun 5)
- **Removed conditional for SDK release prep** (Jun 3)
- **Task 57706703**: LocalUser changes to enable side-by-side similar to auth APIs (Jun 2)
- **Task 57706665**: Created Steam extension for PF Core part 1 (May 30)

### May 2025

#### GameSave Merge (Major Update)
- **Gamesave Merge Pt3: Pipelines** (May 20)
  - Updated official and PR pipelines
  - Added PlayFab.C.GameSave.GDK.yml
  - Updated all build scripts
- **Gamesave Merge Pt2: GameSave** (May 17)
  - Added complete GameSave implementation
  - Added GDK and Win32 samples
  - Added Steam Win32 sample
  - Added automated and manual test suites
  - Added sync manager, providers, and platform-specific code
- **Gamesave Merge Pt1: Core** (May 15)
  - Added PFLocalUser API
  - Added platform-specific implementations (Android, Steam, GDK)
  - Added file I/O and archive operations
  - Added compression support with libarchive
  - Updated shared internal libraries

#### Build & Infrastructure
- **Added GameSave resource compile files** (May 30)
- **Bug 57620601**: Fixed errors in Gamesave automated tests (May 29)
- **Fixes for Gamesave GDK Build** in Unified SDK Pipelines (May 29)
- **Task 57724105**: Updated GameSave to latest service data JSON (May 29)
- **Task 57565375**: Added more test flushout for metadata support (May 23)
- **SDKs.All Android Fix** (May 22)
- **Unreferenced params fix for Windows Builds** (May 21)
- **Adding version stamping for LHC** in Win32 builds and refactored version setting (May 20)
- **Removed deprecated PkgESSetupBuild task** (May 19)

### April 2025

#### API & Documentation
- **Bug 57049820**: Fixed PFCore incorrect ...GetResultSize ref doc for AuthenticateGameServer (May 12)
- **Exposed LoginWithXbox on GDK** for Unreal OSS (Apr 24)
- **Bug 56897740**: Fixed missing reference docs for PFHttpConfig (Apr 24)
- **Set isNativeCode in Official build** for CyberEO for Windows Undocked (Apr 11)
- **Task 53006110**: Fixed minor tracing issues (Apr 3)
- **Task 56949160**: Deduplicated PF make.js to support maintenance and extensibility (Apr 2)

---

## Summary of Major Changes

### New Features
- **PFGameSave System**: Complete game save implementation with multi-device sync, GRTS support, Steam integration
- **Steam Extension**: New Steam platform support for LocalUser and authentication
- **3PP Authentication**: Third-party authentication support in PFGameSave
- **Expanded GDK API**: Additional APIs for Unified SDK integration
- **Exception Handling**: Comprehensive exception handling across all public APIs

### Platform Support
- **Removed Win32 projects** in favor of GDK
- **Enhanced Android support** with updated Gradle and build system
- **SteamDeck support** with proper path and telemetry handling
- **macOS updates** with image and build improvements

### Infrastructure
- **NuGet Central Package Management** enabled
- **Version stamping** for all binaries
- **Centralized file lists** via shared props
- **Updated libHttpClient** to latest version
- **GitHub release pipeline** improvements

### Quality & Testing
- **Cross-platform test automation suite**
- **AI review integration**
- **Comprehensive unit tests** for GameSave
- **Stress testing tools**
- **Folder monitoring tools** for GameSave validation

### Bug Fixes
- 50+ bug fixes across authentication, game saves, telemetry, and platform support
- Memory management improvements
- Crash fixes for various edge cases
- Build and linking issue resolutions

### Breaking Changes
- Removal of Win32-specific projects (replaced with GDK)
- LocalUser API restructuring for platform-specific implementations
- Removal of deprecated PkgESSetupBuild task

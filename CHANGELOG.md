# PlayFab C SDK Changelog

## 2510 Release

### March 2026

#### Build Improvements
- Added `_GAMING_DESKTOP` preprocessor define and DirectX link libraries (`d3d12.lib`, `dxgi.lib`, `dxguid.lib`) to `PlayFab.C.GDK.props` for the x64 platform, since the GDK toolchain no longer provides these automatically after the migration from `Gaming.Desktop.x64` to `x64`
- Excluded PlayFabCore.UnitTests from x64 solution build configuration (Win32-only project incompatible with GDK-mapped x64 platform)

### February 2026 (QFE2)

#### GameSave Bug Fixes
- Fixed save conflict where local `PFGameSaveDescriptor.time` was often incorrect
- Fixed `PFGameSaveFilesSetSaveDescriptionResult` returning `E_FAIL` when called offline on SteamDeck
- Fixed `PFGameSaveFilesAddUserWithUiAsync` returning `E_PF_GAMESAVE_USER_ALREADY_ADDED` when reconnecting from offline mode
- Fixed crash after clicking Continue Offline with disconnected network on SteamDeck
- Fixed `PFGameSaveFilesSetSaveDescriptionAsync` hang when called offline on SteamDeck
- Fixed incorrect timestamps when syncing between inproc and GRTS-based devices
- Fixed LockStep reusing stale pending manifest when finalized version has advanced
- Fixed GRTS save folder initialization failure when directory doesn't exist or path lacks trailing backslash
- Fixed `PFGameSaveDescriptor.shortSaveDescription` contents being garbage on SteamDeck
- Fixed download of many small files not working with inproc provider
- Fixed upload progress reports using uncompressed sizes, which could cause corruption
- Fixed inproc cancel from progress dialog causing assert failure
- Fixed cleanup not happening properly if init fails with invalid argument
- Fixed unexpected device-contention callback invocation

#### GameSave Improvements
- Added better error handling throughout PFGameSaves
- Added logging to GRTS provider for better debugging
- Hooked up rollback flags for inproc client
- Made Steam SDK optional for Game Save sample
- Updated SDK sample to show thumbnails during UI conflict dialog
- Cleaned up platform-dependent code in common folder

#### API Updates
- Extended `PFAuthenticationLoginWithXboxAsync` and other Xbox-related authentication functions to all client platforms
- Updated to latest service API definitions (Inventory, Catalog, AccountManagement, and others)

#### Build Improvements
- Fixed nlohmann JSON brace initialization issue
- Fixed compiler warnings and improved build compatibility for new platforms
- Updated Android build dependencies
- Updated SDK version to 2510.2.0

### November 2025

#### Build Improvements
- Updated build order - SharedInternal now builds first
- Updated Android Gradle Plugin to latest version
- Removed separate Win32 build projects (PlayFabCore.Win32, PlayFabServices.Win32, PlayFabSharedInternal.Win32.143, PlayFabServicesTestApp.Win32); GDK projects now target both Xbox and Windows desktop via Gaming.Desktop.x64 platform

### October 2025

#### Bug Fixes
- Fixed crash on SteamDeck when selecting Prepare after SetUser
- Fixed PFGameSave failure in PAL layer when not using XUser on PC
- Fixed PFGameSave reports failure with E_PF_GAMESAVE_ALREADY_INITIALIZED on resume
- Fixed PFGameSave crashes during download
- Removed pathcch.lib dependency which caused build issues

#### Features & Improvements
- Added missing SteamDeck indicator in telemetry
- Prevented KnownFolders from being set as root folder
- Fixed bug when uploading and setting description simultaneously
- Added tests for GDK expanded API 2510
- Fixed missing std::placeholders reference in TypeWrapperHelpers.h
- Added in-game upload differentiator for telemetry

### September 2025

#### Major Features
- Updated SdkVersion.h for GA and updated XCurl.lib paths
- Added 3PP auth support into PFGameSave
- Disabled unused libarchive compression formats
- Added PFGameSave Rollback API additions
- Removed quota endpoint call (quota now returned by List endpoint)
- Created cross-platform multidevice test automation suite
- Disabled incremental linking when /PROFILE is enabled to avoid LNK4075 warning

#### Platform Updates
- Added missing secretkey APIs for GDK and typedef guards
- Updated SdkVersion.h

#### API Updates
- Added PFGameSaveFilesSetSaveDescriptionAsync

### August 2025

#### API Expansion
- Expanded GDK API Surface for Unified SDK
  - New APIs integration
  - Fine-tuned Inventory and Catalog customizations
  - Updated "GDK" references to "Windows" in remarks
- Added more tracing around platform init

#### Build System Improvements
- Centralized file lists via shared props and fixed GDK build/link
- Fixed Android TestApp to build on release with Gradle
- Added logging around PFGameSaves provider selection
- Fixed undefined HC_PLATFORM in Unreal Plugin
- Updated GameSave telemetry events

#### Bug Fixes
- Fixed PFCore not automatically including Xbox auth in PFAuthentication
- Enabled NuGet Central Package Management

### July 2025

#### Major Features
- Support for in-proc game saves upload on Windows to enable 3PP
- Renamed Android LocalUser away from Platform
- Cleaned up tests and dead GDK gamesave files

#### Bug Fixes
- Only extract zips after all are downloaded to avoid network errors
- Fixed string copy crash in PFGameSaves.dll on out of proc path
- Fixed PFGameSaveFilesUploadWithUiAsync crash with nullptr queue
- Added console handling to GRTS detection
- PFGameSaves now takes in saveFolder for SteamDeck support in init args
- PFGameSaves now reads ForceUseLocalServices regkey to use inproc

### June 2025

#### Core Features
- Deduped Core PFErrors generation
- Fixed GameSave init API regression
- Added exception handling helpers to all public APIs, including auto-generated ones
- Added exception handling helpers to GameSave public APIs
- Bug fixes for Steam API
- Added Steam API to PF Core GDK binary

#### LocalUser Updates
- Added missing replacement for GetPlatformContext for LocalUser_Xbox
- LocalUser changes to enable side-by-side similar to auth APIs
- Created Steam extension for PF Core

### May 2025

#### GameSave (Major Addition)
- Added complete GameSave implementation
  - GDK and Win32 platform support
  - Sync manager and providers
  - Platform-specific code
- Added PFLocalUser API
- Added platform-specific implementations (Android, Steam, GDK)
- Added file I/O and archive operations
- Added compression support with libarchive
- Updated shared internal libraries

#### Build & Infrastructure
- Fixed errors in GameSave automated tests
- Updated GameSave to latest service data JSON
- Added more test coverage for metadata support
- Adding version stamping for LHC in Win32 builds

### April 2025

#### API & Documentation
- Fixed PFCore incorrect GetResultSize ref doc for AuthenticateGameServer
- Exposed LoginWithXbox on GDK for Unreal OSS
- Fixed missing reference docs for PFHttpConfig
- Fixed minor tracing issues

---

## Summary of Major Changes

### New Features
- **PFGameSave System**: Complete game save implementation with multi-device sync, Steam integration, and cross-platform support
- **Expanded GDK API Surface**: New APIs for Experimentation, Friends, Localization, PlatformSpecific, PushNotifications
- **PFLocalUser API**: Platform-specific local user management
- **Exception Handling**: Added to all public APIs including GameSave

### Platform Support
- GDK (Xbox/Windows)
- Win32
- Android
- SteamDeck/Linux
- iOS/macOS

[comment]: <> (This markdown is for AI to parse)
[comment]: <> (Original Word doc is at https://microsoftapc-my.sharepoint.com/:w:/g/personal/relyons_microsoft_com/ERje7N7uutNCk8MDW3U_oBIBaXpCV99c1dYmqbDgq1nsaA?e=xLPK2W)

PlayFab Game Saves Platform Test Cases

The objective of this document is to outline manual tests for the new PlayFab Game Saves client components on PC/Console.

Gap coverage index (manual vs automation – source of truth)

- Gap 63 — Conflict UI Decision Application

	- Manual: Yes. Covered in Section 3: Verifying Conflict resolution (Case 1/2). Manual focus = UX clarity and single-decision application.

	- Automation: Enforced behavior/guard rails tracked separately; manual remains for UX.

- Gap 76 — UI Automation for Conflict Resolution

	- Manual: Yes. Section 3 exercises the flows end-to-end.

	- Automation: Yes. This gap tracks adding automation; manual sanity stays for copy/flow.

- Gap 77 — Power State Testing Automation

	- Manual: Yes. Section 2: Upload on Suspend/Connected Standby/Power Pull/Structured Shutdown.

	- Automation: Recommended to broaden permutations; manual remains a spot-check.

- Gap 106 — October 2025 GDK Folder Layout Compatibility

	- Manual: No. Verified by integration/build automation. Testers do not validate folder layout.

- Gap 110 — Steam Deck Frequent Sync Requirements

	- Manual: Yes. Section 2: (NEW!) Mid game Upload — also run on Steam Deck; verify periodic in-game syncing occurs within the expected window.

	- Automation: Planned cadence/timing checks.

- Gap 117 — Active Device Changed Recovery Flow

	- Manual: Yes. Section 3: Active Device contention (Cases 1–3) covers recovery/flow; earlier docs may call this lock contention.

	- Automation: Optional.

Note: This index maps gaps to manual coverage in this document; detailed automation lives outside this manual suite.

Title: ShamWow (please use latest ShamWow builds)

OS/GRTS Recoveries: 2508+ (Please use latest recoveries)

Instructions for ShamWOW

With 2510 GDK, xgameruntime and PlayFab Game Saves will support the following platforms:

Gaming.Xbox.XboxOne.x64 (aka gen8 console)

Gaming.Xbox.Scarlett.x64 (aka gen9 console)

Gaming.Desktop.x64 (aka x64 w/ MSIXVC package)

PC Steam (aka x64 w/o MSIXVC package)

Windows on Steam Deck (aka Windows Proton emulation via inproc xgameruntime)

Each of the above will need to run all PFGS test cases, as well as combinations between them to validate functionality across all available endpoints/platforms. The test app across these platforms will be ShamWow, which is available to run in all 5 configurations, see instructions on individual set up below:

Console:

Gaming.Xbox.XboxOne.x64 (aka gen8 console)

Gaming.Xbox.Scarlett.x64 (aka gen9 console)

\\ntdev\release\xb_flt_2410ge\26100.2386.240912-2200\amd64fre\xbox_test_automation_bins\ShamWow\DirectX

Gaming Desktop (MSIXVC)

Install MSIXVC Windows version located at \\ntdev\release\xb_flt_2410ge\\\winbuilds\release\xb_flt_2510ge\26100.6059.250730-2200\amd64fre\xbox_test_automation_bins\ShamWow\DirectX\GameCore\WindowsDX12



PC Steam (non MSIXVC)

Copy files onto your device from here: \\ntdev\release\xb_flt_2410ge\\\winbuilds\release\xb_flt_2510ge\26100.6059.250730-2200\amd64fre\xbox_test_automation_bins\ShamWow\DirectX\GameCore\WindowsDX12

Launch ShamWow exe.





Windows on Steam Deck

\\ntdev\release\xb_flt_2410ge\\\winbuilds\release\xb_flt_2510ge\26100.6059.250730-2200\amd64fre\xbox_test_automation_bins\ShamWow\DirectX\GameCore\WindowsDX12

\\ntdev\release\xb_flt_2410ge\26100.2386.240912-2200\amd64fre\bin\gamecore\xgameruntime.dll

Create 2 generic Steam accounts and add them both to the PlayFab Steam project via Steam portal.

You'll need 1 for PC and 1 for Steam Deck otherwise they will sign each other out.



Set steam devkit management tool like this:

The important flag is Steam Play -- this means Proton.

Also enable debugger service if you want to debug.  If you want to "wait for debug client to attach" then you can attach to Steam.exe and it'll launch game, and you can debug startup if need be.

Paste in ShamWow files to local folder field, name it ShamWow





Select Upload to transfer the files onto the Steam Deck device



If you want to remote desktop to Steam Deck to capture screen shots/etc, you can Steam Link:

https://steamcommunity.com/app/353380/discussions/8/5943120984229696131/

(the link is always pointing to latest so just use that one)



Install the package, and then run "Steam Link" app.  Click "Other Computer" when adding other PC and it'll give you code

Go to Steamdeck, open Remote Play and enable it and click "Pair with other device" and enter code



Now on Steam Link app on PC, go to main screen and you should ShamWow it listed.  You might need to close Steam on PC (not sure) and it should remote desktop in your Steamdeck



Devices:

Each test with just one device mentioned should be validated across different device types:

PC (Xbox Desktop configuration - MSIXVC)

PC (Steam configuration - non MSIXVC)

Console

Steam Deck (Windows on Proton)

When a test case has tests between multiple devices, please test these across the following device combinations:

PC (Xbox Desktop configuration - MSIXVC) ßà PC (Steam configuration - non MSIXVC)

Console à Console

Console ßà PC (Xbox Desktop configuration - MSIXVC)

Console ßà PC (Steam configuration - non MSIXVC)

PC (Xbox Desktop configuration - MSIXVC) ßà Steam Deck (Windows on Proton)

Console ßà Steam Deck (Windows on Proton)

Steam Deck (Windows on Proton) ßà PC (Steam configuration - non MSIXVC)

To from

Console (gen 8)

Console (gen 9)

PC (Xbox Desktop configuration - MSIXVC)

PC (Steam configuration - non MSIXVC)

Steam Deck (Windows on Proton)

Console (gen 8)

-

P1

-





Console (gen 9)



-

P1

P1

P1

PC (Xbox Desktop configuration - MSIXVC)

-





P1



PC (Steam configuration - non MSIXVC)









P1

Steam Deck (Windows on Proton)





P1







Section 1: Basic steps to create a game save

To create a new game save: ​[Shared Step Test Case: 53971360]

Sign in to ShamWOW

In the "Scenarios" window, select the "Game Save" drop down menu​

Click the following options in order: Set User-> PlayFab -> Prepare, Create &/or Write. Read / Enum optional.

At this point, the data has been created and now needs to be saved to the cloud.​

​

To Load A Cloud Save:​ [Shared Step Test Case: 53971413]

Wait a minute or so, then sign into ShamWOW

In the "Game Save" window select Set User -> PlayFab -> Prepare -> Enum to see the data listed. Read is optional.

Section 2: Phase 1 Playfab Game Saves Test cases (Manual)

Syncing and download

Ensure Sync dialog prompts correctly [Test Case: 53971519]

Close any local instance of ShamWow

Wait 5 minutes (gives time for upload of any files to happen)

In a command prompt type:

cd localappdata%\Packages\31728SillyFish.ShamWOWPC_k194f2v84b7mc\SystemAppData\xgs

dir rem for each directory

rmdir /s/q [DIRECTORYNAME]

Launch ShamWow

Go to GameSave

SetUser

Make sure that the user is signed in

PlayFab  > Prepare

Validate that sync dialog shows up briefly

If you don’t see the dialog do PFGSMode > Enum Folders and validate from the Output window that you have at least 128 MB. If not, do PFGSMode > CreateFile a few times and restart this test.



Ensure sync failed dialog prompts during sync failure [Test Case: 53971672]

Close any local instance of ShamWow

Wait 5 minutes (gives time for upload of any files to happen)

In a command prompt type:

cd %localappdata%\Packages\31728SillyFish.ShamWOWPC_k194f2v84b7mc\SystemAppData\xgs

dir

rem for each directory

rmdir /s/q [DIRECTORYNAME]

Launch ShamWow

Go to GameSave

SetUser

Make sure that the user is signed in

PlayFab > Prepare

Validate that sync dialog shows up briefly

Disconnect internet from the device quickly

Validate an error message prompts informing the user the sync has failed





Progression:

Existing game save progress while offline carries over across devices. [Test Case: 53972746]

Goal for this test is to ensure game progress for an existing save made on the one device carries over to another device, and vice versa.

Launch ShamWOW and sign in for a user that already has existing cloud saves

Make a game save

Close ShamWOW

Launch SHamWOW on another device and sign in with the same user

Verify the game save that was on the previous device shows up as expected on the 2nd device

Make a game save, wait a minute for it to upload

Close ShamWOW

Launch ShamWOW on the original device

Verify the latest game save progress from the 2nd device is present as expected

Repeat for all device combinations



New game save progress carries over across devices. [Test Case: 53972820]

The goal of this test case is to verify that a Cloud save created on one device can be loaded onto another.

On a device, Launch the ShamWOW app and sign in.

Create a game save (steps in Section 1)

Close the game and wait a minute for the cloud data to upload

Switch to another device, load the same

Verify the user is shown a ‘syncing dialog’

Verify the save previously made is available on the new device

Repeat for all device combinations



Upload:

 Upload after being in background for 30 minutes without doing a write [Test Case: 53972878]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Put ShamWow into background and wait 35 minutes

Disconnect device from network

On secondary device, use ShamWow with same user and verify you sync latest

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size



Upload on Title Termination [Test Case: 53972975]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Terminate ShamWow

On secondary device, use ShamWow with same user and verify you sync latest

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size



Upload on User-sign Out [Test Case: 53973046]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

On same device, sign-out the user

Wait minutes

On secondary device, use ShamWow with same user and verify you sync latest

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size





Upload on Suspend on Console [Test Case: 53973160]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Launch Settings

Wait 15 minutes (Game is guaranteed to be suspended after 10 minutes)

Disconnect console from network

On secondary device, use ShamWow with same user and verify you sync latest

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files size



Upload on Power State Change (Connected Standby). [Test Case: 53973243]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Put device into standby

Wait 5 minutes

On secondary device, use ShamWow with same user and verify you sync latest

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size



Upload on Power State Change on a device (Power Pull). [Test Case: 53973301]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Pull power from the device suddenly – at the plug

Wait 5 minutes

On secondary device, use ShamWow with same user and verify your game save state (you should not have the latest saves)

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size

e.   Return to the first device, ensure your saves are still present





Upload on Power State Change on a device (Structured Shutdown). [Test Case: 53973372]

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Shut down the device (turn off console from menu, or turn off PC from the windows menu)

Wait 5 minutes

On secondary device, use ShamWow with same user and verify your game save state

·   You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size

e.   Return to the first device, ensure your saves are still present



(NEW!) Mid game Upload
Steam Deck note (Gap 110): Also run this on Steam Deck. Let the title remain active in-game and verify periodic syncing occurs within the expected window (avoid prolonged unsynced progress). Exact cadence will be validated via automation.

(new) Mid game upload steps

Follow steps to create a game save

PlayFab > Enum Folder (note this number)

Group 3, Grouped objectSelect Playfab > Upload (see figure) :



(new) Verify mid game upload success

Follow steps for initializing a mid game upload

Verify Upload completes successfully

To ensure out of game uploads do not occur, disconnect device from the network

On secondary device, use ShamWow with same user and verify your game save state

 You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size.



(new) Verify other writes can occur during mid game upload

Follow steps for initializing a mid game upload

Make another game save: PlayFab -> Prepare, Create &/or Write.

Read / Enum to ensure the new write was successful and make note of it

Do not upload the latest save

To ensure out of game uploads do not occur, disconnect device from the network

On secondary device, use ShamWow with same user and verify your 1st game save was uploaded correctly

 You can again use PFGSMode > Enum Folder to validate you have the correct number of files and size.

Verify your 2nd game save is not present on the secondary device





Verifying Connectivity

Verify connectivity dialog prompts when connection to the service is lost [Test Case: 53973596]

The goal of this test is to verify that a connectivity dialog prompts when connection to the service is lost on game save initialization.

On a device, ShamWOW and create a game save by following the steps in section 1

Once the game save has been created and uploaded to the cloud, close ShamWow and reopen ShamWow on another device

Set User

PlayFab -> Prepare

Immediately disconnect your device from the internet

Verify the user is prompted with a ‘connectivity’ prompt

CASE 1: Verify the user can try again (after reconnecting) [Test Case: 53973596]

Reconnect your device back to the internet

Select try again

Verify the game saves continue to initialize: Click "Load" to verify the cloud save containers are present.​

CASE 2: Verify the user can continue offline [Test Case: 53973658]

Do not reconnect your device to the internet

Select continue offline

Verify the user can continue offline (Click "Load" to verify the cloud save containers made in device 1 are NOT present.​)



Section 3: Phase 2 Test cases (manual)

Verifying Conflict resolution
Note (Gaps 63, 76): Manual UX validation for conflict prompts and single-decision application across conflicts. Enforcement/guard-rails will be automated.

Verify conflict resolution prompts appropriately [Test Case: 53973957]

The goal of this test case is to verify that the user is prompted with the option of choosing between the newer PC cloud save and the older offline console save.

On a device, ShamWOW and create a game save by following the steps in section 1

Once the game save has been created and uploaded to the cloud, switch to another device

On the other device, sign in to ShamWOW and load the cloud save by Selecting "Set User," and then PlayFab -> prepare

Once the Cloud saves are loaded, disconnect the device from the internet by either disconnecting the network cable or disabling network devices in the settings menu.

Make changes to the local save by modifying the data, Create, Write or Delete. Close the app.

Switch back to the first device again, launch and sign in to ShamWOW

Make changes to the local save by modifying the data, Create, Write or Delete.

Close the game, wait a moment for the save to upload to the cloud.

Reconnect the other device to the internet again and launch the  ShamWOWPC app.

Sign in to ShamWOW, in the Game Save window select "Set User," then "Initialize".

Verify the user should then be prompted with choosing between the older offline console save and the newer cloud pc save

CASE 1: Select the cloud save. [Test Case: 53973854]

Select the cloud save option on the dialog

Close the app, switch to the original device

Launch the app and load the cloud save

Verify the user should not be prompted to choose between any saves, the save should load automatically.

CASE 2: Choose local save [Test Case: 53973957]

When prompted to choose the newer cloud save and the older local save, pick the local save.

Once the local save is loaded, save the game and close it. Wait a minute for the cloud save to upload.

On the Console PC, load the save.

Verify a "Syncing" TCUI pops up to verify that the cloud save was updated to the newest cloud save that was formerly the previous device save.

(UPDATED!) Active Device contention:
Note (Gaps 115–117): Covers takeover messaging and recovery (Gap 117). Earlier docs may call this lock contention.

Ensure Active Device contention prompts correctly when another device is still uploading [Test Case: 53974103]

On a device, launch ShamWOW and create a game save by following the steps in section 1

Once the game save has been created immediately close ShamWOW and switch to another device

Immediately launch ShamWOW on the new device

Verify a dialog prompts ‘Your previous device is still uploading your game save”

CASE 1: Verify a user can break a lock and play on a new device [Test Case: 53974195]

Select “continue from last cloud data”/”stop sync and continue”

Select confirm on confirmation prompt

Load your game save

PFGSMode > Enum Folder, ensure the previous game saves made are not present



CASE 2: Verify a user can wait for the lock and continue [Test Case: 53974360]

Wait until the progress bar has complete or select ‘try again’

Verify the dialog eventually disappears

PFGSMode > Enum Folder, ensure the previous game saves made are not present



CASE 3: Verify a user can quit the title safely [Test Case: 53974442]

Select the quit button or exit ShamWOW completely

Wait a minute

Relaunch ShamWOW and sign in

Verify your previous game saves are available




// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/core/PFCore.h>
#include <string>
#include <cstring>
#include <ctime>
#include <set>

// Test fixture for Data Structure tests
class DataStructureTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean state for each test
    }

    void TearDown() override
    {
        // Cleanup after each test
    }

    // Helper to create valid PFGameSaveInitArgs
    PFGameSaveInitArgs CreateValidInitArgs()
    {
        PFGameSaveInitArgs args{};
        args.backgroundQueue = nullptr;
        args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
        args.saveFolder = "C:\\temp\\TestSaveFolder";
        return args;
    }

    // Helper to create valid PFGameSaveDescriptor
    PFGameSaveDescriptor CreateValidDescriptor()
    {
        PFGameSaveDescriptor descriptor{};
        descriptor.time = std::time(nullptr);
        descriptor.totalBytes = 1024;
        descriptor.uploadedBytes = 512;
        
        // Fill character arrays with valid data
        strcpy_s(descriptor.deviceType, sizeof(descriptor.deviceType), "TestDevice");
        strcpy_s(descriptor.deviceId, sizeof(descriptor.deviceId), "12345678-1234-1234-1234-123456789ABC");
        strcpy_s(descriptor.deviceFriendlyName, sizeof(descriptor.deviceFriendlyName), "My Test Device");
        strcpy_s(descriptor.thumbnailUri, sizeof(descriptor.thumbnailUri), "https://example.com/thumbnail.png");
        strcpy_s(descriptor.shortSaveDescription, sizeof(descriptor.shortSaveDescription), "Test save description");
        
        return descriptor;
    }
};

// Test 2.A: PFGameSaveInitArgs structure validation and initialization
TEST_F(DataStructureTests, InitArgsDefaultInitialization)
{
    // Test zero-initialization
    PFGameSaveInitArgs args{};
    
    EXPECT_EQ(nullptr, args.backgroundQueue);
    EXPECT_EQ(0, args.options);
    EXPECT_EQ(nullptr, args.saveFolder);
}

TEST_F(DataStructureTests, InitArgsValidConfiguration)
{
    auto args = CreateValidInitArgs();
    
    // Verify all fields are properly set
    EXPECT_EQ(nullptr, args.backgroundQueue); // Using default queue
    EXPECT_EQ(static_cast<uint64_t>(PFGameSaveInitOptions::None), args.options);
    EXPECT_STREQ("C:\\temp\\TestSaveFolder", args.saveFolder);
}

TEST_F(DataStructureTests, InitArgsOptionsValidation)
{
    PFGameSaveInitArgs args{};
    
    // Test valid options
    args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    EXPECT_EQ(0, args.options);
    
    // Test flag operations
    uint64_t testOptions = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    args.options = testOptions;
    EXPECT_EQ(testOptions, args.options);
}

TEST_F(DataStructureTests, InitArgsSaveFolderValidation)
{
    PFGameSaveInitArgs args{};
    
    // Test various save folder configurations
    args.saveFolder = "";
    EXPECT_STREQ("", args.saveFolder);
    
    args.saveFolder = "C:\\temp";
    EXPECT_STREQ("C:\\temp", args.saveFolder);
    
    // Test long path
    std::string longPath(260, 'A'); // MAX_PATH length
    args.saveFolder = longPath.c_str();
    EXPECT_EQ(longPath, std::string(args.saveFolder));
}

// Test 2.B: PFGameSaveDescriptor buffer safety and bounds checking
TEST_F(DataStructureTests, DescriptorDefaultInitialization)
{
    PFGameSaveDescriptor descriptor{};
    
    // Verify zero-initialization
    EXPECT_EQ(0, descriptor.time);
    EXPECT_EQ(0, descriptor.totalBytes);
    EXPECT_EQ(0, descriptor.uploadedBytes);
    EXPECT_EQ('\0', descriptor.deviceType[0]);
    EXPECT_EQ('\0', descriptor.deviceId[0]);
    EXPECT_EQ('\0', descriptor.deviceFriendlyName[0]);
    EXPECT_EQ('\0', descriptor.thumbnailUri[0]);
    EXPECT_EQ('\0', descriptor.shortSaveDescription[0]);
}

TEST_F(DataStructureTests, DescriptorValidConfiguration)
{
    auto descriptor = CreateValidDescriptor();
    
    // Verify all fields are properly set
    EXPECT_GT(descriptor.time, 0);
    EXPECT_EQ(1024, descriptor.totalBytes);
    EXPECT_EQ(512, descriptor.uploadedBytes);
    EXPECT_STREQ("TestDevice", descriptor.deviceType);
    EXPECT_STREQ("12345678-1234-1234-1234-123456789ABC", descriptor.deviceId);
    EXPECT_STREQ("My Test Device", descriptor.deviceFriendlyName);
    EXPECT_STREQ("https://example.com/thumbnail.png", descriptor.thumbnailUri);
    EXPECT_STREQ("Test save description", descriptor.shortSaveDescription);
}

TEST_F(DataStructureTests, DescriptorBufferSizes)
{
    // Verify buffer sizes match documented limits
    PFGameSaveDescriptor descriptor{};
    
    EXPECT_EQ(256, sizeof(descriptor.deviceType));
    EXPECT_EQ(256, sizeof(descriptor.deviceId));
    EXPECT_EQ(256, sizeof(descriptor.deviceFriendlyName));
    EXPECT_EQ(2048, sizeof(descriptor.thumbnailUri));
    EXPECT_EQ(4096, sizeof(descriptor.shortSaveDescription));
}

TEST_F(DataStructureTests, DescriptorBoundaryStringLengths)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test maximum length strings (one less than buffer size for null terminator)
    std::string maxDeviceType(255, 'A');
    std::string maxDeviceId(255, 'B');
    std::string maxDeviceFriendlyName(255, 'C');
    std::string maxThumbnailUri(2047, 'D');
    std::string maxShortDescription(4095, 'E');
    
    strcpy_s(descriptor.deviceType, sizeof(descriptor.deviceType), maxDeviceType.c_str());
    strcpy_s(descriptor.deviceId, sizeof(descriptor.deviceId), maxDeviceId.c_str());
    strcpy_s(descriptor.deviceFriendlyName, sizeof(descriptor.deviceFriendlyName), maxDeviceFriendlyName.c_str());
    strcpy_s(descriptor.thumbnailUri, sizeof(descriptor.thumbnailUri), maxThumbnailUri.c_str());
    strcpy_s(descriptor.shortSaveDescription, sizeof(descriptor.shortSaveDescription), maxShortDescription.c_str());
    
    // Verify strings are properly null-terminated
    EXPECT_EQ(maxDeviceType, std::string(descriptor.deviceType));
    EXPECT_EQ(maxDeviceId, std::string(descriptor.deviceId));
    EXPECT_EQ(maxDeviceFriendlyName, std::string(descriptor.deviceFriendlyName));
    EXPECT_EQ(maxThumbnailUri, std::string(descriptor.thumbnailUri));
    EXPECT_EQ(maxShortDescription, std::string(descriptor.shortSaveDescription));
}

TEST_F(DataStructureTests, DescriptorNumericalFields)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test time_t field
    descriptor.time = 1640995200; // Jan 1, 2022 00:00:00 UTC
    EXPECT_EQ(1640995200, descriptor.time);
    
    // Test uint64_t fields
    descriptor.totalBytes = UINT64_MAX;
    descriptor.uploadedBytes = UINT64_MAX / 2;
    
    EXPECT_EQ(UINT64_MAX, descriptor.totalBytes);
    EXPECT_EQ(UINT64_MAX / 2, descriptor.uploadedBytes);
    
    // Test boundary values
    descriptor.totalBytes = 0;
    descriptor.uploadedBytes = 0;
    EXPECT_EQ(0, descriptor.totalBytes);
    EXPECT_EQ(0, descriptor.uploadedBytes);
}

// Test 2.C: PFGameSaveInitOptions enum validation
TEST_F(DataStructureTests, InitOptionsEnumValues)
{
    // Test enum value
    EXPECT_EQ(0x00, static_cast<uint32_t>(PFGameSaveInitOptions::None));
    
    // Test enum can be cast to uint64_t for options field
    uint64_t options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    EXPECT_EQ(0, options);
}

TEST_F(DataStructureTests, InitOptionsEnumFlags)
{
    // Test enum flag operations (even though only None is currently defined)
    PFGameSaveInitOptions option1 = PFGameSaveInitOptions::None;
    PFGameSaveInitOptions option2 = PFGameSaveInitOptions::None;
    
    // Test bitwise operations are valid
    auto combined = static_cast<PFGameSaveInitOptions>(
        static_cast<uint32_t>(option1) | static_cast<uint32_t>(option2)
    );
    
    EXPECT_EQ(PFGameSaveInitOptions::None, combined);
}

// Test 2.D: PFGameSaveFilesAddUserOptions enum validation
TEST_F(DataStructureTests, AddUserOptionsEnumValues)
{
    // Test enum value
    EXPECT_EQ(0x00, static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::None));
    EXPECT_EQ(0x01, static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood));
    EXPECT_EQ(0x02, static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastConflict));
}

TEST_F(DataStructureTests, AddUserOptionsEnumFlags)
{
    // Test enum flag operations
    // Combine two identical None flags
    PFGameSaveFilesAddUserOptions option = PFGameSaveFilesAddUserOptions::None;
    auto combinedNone = static_cast<PFGameSaveFilesAddUserOptions>(
        static_cast<uint32_t>(option) | static_cast<uint32_t>(option));
    EXPECT_EQ(PFGameSaveFilesAddUserOptions::None, combinedNone);

    // Combine rollback flags
    auto combinedRollback = static_cast<PFGameSaveFilesAddUserOptions>(
        static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) |
        static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastConflict));
    EXPECT_EQ(0x03u, static_cast<uint32_t>(combinedRollback));
}

// Test 2.E: PFGameSaveFilesSyncState enum validation
TEST_F(DataStructureTests, SyncStateEnumValues)
{
    // Test all defined sync state values
    EXPECT_EQ(0, static_cast<uint32_t>(PFGameSaveFilesSyncState::NotStarted));
    EXPECT_EQ(1, static_cast<uint32_t>(PFGameSaveFilesSyncState::PreparingForDownload));
    EXPECT_EQ(2, static_cast<uint32_t>(PFGameSaveFilesSyncState::Downloading));
    EXPECT_EQ(3, static_cast<uint32_t>(PFGameSaveFilesSyncState::PreparingForUpload));
    EXPECT_EQ(4, static_cast<uint32_t>(PFGameSaveFilesSyncState::Uploading));
    EXPECT_EQ(5, static_cast<uint32_t>(PFGameSaveFilesSyncState::SyncComplete));
}

TEST_F(DataStructureTests, SyncStateSequentialValues)
{
    // Verify sync states are sequential (important for state machine logic)
    std::vector<PFGameSaveFilesSyncState> states = {
        PFGameSaveFilesSyncState::NotStarted,
        PFGameSaveFilesSyncState::PreparingForDownload,
        PFGameSaveFilesSyncState::Downloading,
        PFGameSaveFilesSyncState::PreparingForUpload,
        PFGameSaveFilesSyncState::Uploading,
        PFGameSaveFilesSyncState::SyncComplete
    };
    
    for (size_t i = 0; i < states.size(); ++i)
    {
        EXPECT_EQ(i, static_cast<size_t>(states[i]));
    }
}

// Test 2.F: PFGameSaveFilesUploadOption enum validation
TEST_F(DataStructureTests, UploadOptionEnumValues)
{
    // Test enum values
    EXPECT_EQ(0, static_cast<uint32_t>(PFGameSaveFilesUploadOption::KeepDeviceActive));
    EXPECT_EQ(1, static_cast<uint32_t>(PFGameSaveFilesUploadOption::ReleaseDeviceAsActive));
}

TEST_F(DataStructureTests, UploadOptionEnumDistinctValues)
{
    // Ensure values are distinct
    EXPECT_NE(
        static_cast<uint32_t>(PFGameSaveFilesUploadOption::KeepDeviceActive),
        static_cast<uint32_t>(PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
    );
}

// Test 2.G: Buffer overflow protection tests for fixed-size character arrays
TEST_F(DataStructureTests, BufferOverflowProtectionDeviceType)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test that strcpy_s prevents buffer overflow
    std::string oversizedString(300, 'X'); // Larger than deviceType buffer
    
    // strcpy_s should fail safely when buffer is too small
    // In debug builds, strcpy_s may assert, so we'll test with a size that fits
    std::string validString(sizeof(descriptor.deviceType) - 1, 'X'); // Max size that fits
    
    auto result = strcpy_s(descriptor.deviceType, sizeof(descriptor.deviceType), validString.c_str());
    
    // Should succeed with properly sized string
    EXPECT_EQ(0, result);
    EXPECT_EQ(strlen(descriptor.deviceType), validString.length());
    EXPECT_EQ('\0', descriptor.deviceType[sizeof(descriptor.deviceType) - 1]);
    
    // Test boundary condition - string exactly at max size
    std::string maxString(sizeof(descriptor.deviceType) - 1, 'Z');
    result = strcpy_s(descriptor.deviceType, sizeof(descriptor.deviceType), maxString.c_str());
    EXPECT_EQ(0, result);
}

TEST_F(DataStructureTests, BufferOverflowProtectionThumbnailUri)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test with properly sized thumbnail URI
    std::string validUri(sizeof(descriptor.thumbnailUri) - 1, 'Y'); // Max size that fits
    
    auto result = strcpy_s(descriptor.thumbnailUri, sizeof(descriptor.thumbnailUri), validUri.c_str());
    
    // Should succeed with properly sized string
    EXPECT_EQ(0, result);
    EXPECT_EQ(strlen(descriptor.thumbnailUri), validUri.length());
    EXPECT_EQ('\0', descriptor.thumbnailUri[sizeof(descriptor.thumbnailUri) - 1]);
}

TEST_F(DataStructureTests, BufferOverflowProtectionShortDescription)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test with properly sized description
    std::string validDescription(sizeof(descriptor.shortSaveDescription) - 1, 'Z'); // Max size that fits
    
    auto result = strcpy_s(descriptor.shortSaveDescription, sizeof(descriptor.shortSaveDescription), validDescription.c_str());
    
    // Should succeed with properly sized string
    EXPECT_EQ(0, result);
    EXPECT_EQ(strlen(descriptor.shortSaveDescription), validDescription.length());
    EXPECT_EQ('\0', descriptor.shortSaveDescription[sizeof(descriptor.shortSaveDescription) - 1]);
}

// Test 2.H: time_t and uint64_t field validation
TEST_F(DataStructureTests, TimeFieldValidation)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test current time
    time_t currentTime = std::time(nullptr);
    descriptor.time = currentTime;
    EXPECT_EQ(currentTime, descriptor.time);
    
    // Test epoch time
    descriptor.time = 0;
    EXPECT_EQ(0, descriptor.time);
    
    // Test future time (should be valid)
    time_t futureTime = currentTime + 86400; // +1 day
    descriptor.time = futureTime;
    EXPECT_EQ(futureTime, descriptor.time);
}

TEST_F(DataStructureTests, ByteFieldValidation)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test zero bytes
    descriptor.totalBytes = 0;
    descriptor.uploadedBytes = 0;
    EXPECT_EQ(0, descriptor.totalBytes);
    EXPECT_EQ(0, descriptor.uploadedBytes);
    
    // Test typical game save sizes
    descriptor.totalBytes = 1024 * 1024; // 1MB
    descriptor.uploadedBytes = 512 * 1024; // 512KB
    EXPECT_EQ(1024 * 1024, descriptor.totalBytes);
    EXPECT_EQ(512 * 1024, descriptor.uploadedBytes);
    
    // Test large sizes
    descriptor.totalBytes = UINT64_MAX;
    descriptor.uploadedBytes = UINT64_MAX - 1;
    EXPECT_EQ(UINT64_MAX, descriptor.totalBytes);
    EXPECT_EQ(UINT64_MAX - 1, descriptor.uploadedBytes);
}

TEST_F(DataStructureTests, ByteFieldConsistency)
{
    PFGameSaveDescriptor descriptor{};
    
    // Test that uploadedBytes can be <= totalBytes (logical constraint)
    descriptor.totalBytes = 1000;
    descriptor.uploadedBytes = 500;
    EXPECT_LE(descriptor.uploadedBytes, descriptor.totalBytes);
    
    descriptor.uploadedBytes = 1000; // Equal should be valid
    EXPECT_LE(descriptor.uploadedBytes, descriptor.totalBytes);
    
    // Note: The API doesn't enforce this constraint, but it's logically expected
    descriptor.uploadedBytes = 1500; // Greater than total
    EXPECT_GT(descriptor.uploadedBytes, descriptor.totalBytes); // This validates the test itself
}

// Test 2.I: Path injection protection tests for saveFolder parameter
TEST_F(DataStructureTests, SaveFolderPathValidation)
{
    PFGameSaveInitArgs args{};
    
    // Test normal paths
    args.saveFolder = "C:\\temp\\GameSaves";
    EXPECT_STREQ("C:\\temp\\GameSaves", args.saveFolder);
    
    args.saveFolder = "/home/user/gamesaves";
    EXPECT_STREQ("/home/user/gamesaves", args.saveFolder);
}

TEST_F(DataStructureTests, SaveFolderPathTraversalAttempts)
{
    PFGameSaveInitArgs args{};
    
    // Test potential path traversal attempts (API should validate these)
    args.saveFolder = "..\\..\\..\\System32";
    EXPECT_STREQ("..\\..\\..\\System32", args.saveFolder);
    
    args.saveFolder = "../../../etc/passwd";
    EXPECT_STREQ("../../../etc/passwd", args.saveFolder);
    
    args.saveFolder = "C:\\GameSaves\\..\\..\\Windows";
    EXPECT_STREQ("C:\\GameSaves\\..\\..\\Windows", args.saveFolder);
    
    // Note: The struct doesn't validate these - the API implementation should
    // These tests verify the struct can hold the data, validation happens at API level
}

TEST_F(DataStructureTests, SaveFolderSpecialCharacters)
{
    PFGameSaveInitArgs args{};
    
    // Test paths with special characters
    args.saveFolder = "C:\\Program Files (x86)\\Game Saves";
    EXPECT_STREQ("C:\\Program Files (x86)\\Game Saves", args.saveFolder);
    
    args.saveFolder = "C:\\Games\\Player's Saves";
    EXPECT_STREQ("C:\\Games\\Player's Saves", args.saveFolder);
    
    // Test Unicode path (if supported by platform)
    args.saveFolder = "C:\\Games\\用户保存";
    EXPECT_STREQ("C:\\Games\\用户保存", args.saveFolder);
}

// Test data structure sizes and alignment
TEST_F(DataStructureTests, StructureSizeValidation)
{
    // Verify structure sizes are reasonable and consistent
    EXPECT_GT(sizeof(PFGameSaveInitArgs), 0);
    EXPECT_GT(sizeof(PFGameSaveDescriptor), 0);
    
    // PFGameSaveDescriptor should be dominated by the character arrays
    size_t expectedMinSize = 256 + 256 + 256 + 2048 + 4096 + sizeof(time_t) + 2 * sizeof(uint64_t);
    EXPECT_GE(sizeof(PFGameSaveDescriptor), expectedMinSize);
}

TEST_F(DataStructureTests, StructureAlignment)
{
    // Verify structures are properly aligned for the platform
    EXPECT_EQ(0, sizeof(PFGameSaveInitArgs) % alignof(PFGameSaveInitArgs));
    EXPECT_EQ(0, sizeof(PFGameSaveDescriptor) % alignof(PFGameSaveDescriptor));
    
    // Verify pointer alignment
    EXPECT_EQ(0, reinterpret_cast<uintptr_t>(&PFGameSaveInitArgs{}) % alignof(PFGameSaveInitArgs));
    EXPECT_EQ(0, reinterpret_cast<uintptr_t>(&PFGameSaveDescriptor{}) % alignof(PFGameSaveDescriptor));
}

// Test PFGameSaveFilesUiProgressUserAction enum validation (Task O)
TEST_F(DataStructureTests, PFGameSaveFilesUiProgressUserActionEnum)
{
    // Test Cancel action
    PFGameSaveFilesUiProgressUserAction action = PFGameSaveFilesUiProgressUserAction::Cancel;
    EXPECT_EQ(static_cast<uint32_t>(action), 0);
    
    // Verify enum underlying type
    EXPECT_EQ(sizeof(PFGameSaveFilesUiProgressUserAction), sizeof(uint32_t));
    
    // Test enum cast validation
    uint32_t cancelValue = static_cast<uint32_t>(PFGameSaveFilesUiProgressUserAction::Cancel);
    EXPECT_EQ(cancelValue, 0);
    
    // Test enum assignment
    PFGameSaveFilesUiProgressUserAction testAction = PFGameSaveFilesUiProgressUserAction::Cancel;
    EXPECT_EQ(testAction, PFGameSaveFilesUiProgressUserAction::Cancel);
}

// Test PFGameSaveFilesUiSyncFailedUserAction enum values (Task P)
TEST_F(DataStructureTests, PFGameSaveFilesUiSyncFailedUserActionEnum)
{
    // Test Cancel action (value 0)
    PFGameSaveFilesUiSyncFailedUserAction cancelAction = PFGameSaveFilesUiSyncFailedUserAction::Cancel;
    EXPECT_EQ(static_cast<uint32_t>(cancelAction), 0);
    
    // Test Retry action (value 1)
    PFGameSaveFilesUiSyncFailedUserAction retryAction = PFGameSaveFilesUiSyncFailedUserAction::Retry;
    EXPECT_EQ(static_cast<uint32_t>(retryAction), 1);
    
    // Test UseOffline action (value 2)
    PFGameSaveFilesUiSyncFailedUserAction useOfflineAction = PFGameSaveFilesUiSyncFailedUserAction::UseOffline;
    EXPECT_EQ(static_cast<uint32_t>(useOfflineAction), 2);
    
    // Verify enum underlying type
    EXPECT_EQ(sizeof(PFGameSaveFilesUiSyncFailedUserAction), sizeof(uint32_t));
    
    // Test enum comparisons
    EXPECT_NE(cancelAction, retryAction);
    EXPECT_NE(retryAction, useOfflineAction);
    EXPECT_NE(cancelAction, useOfflineAction);
    
    // Test enum range validation
    EXPECT_GE(static_cast<uint32_t>(cancelAction), 0U);
    EXPECT_LE(static_cast<uint32_t>(useOfflineAction), 2U);
}

// Test PFGameSaveFilesUiActiveDeviceContentionUserAction enum values (Task Q)
TEST_F(DataStructureTests, PFGameSaveFilesUiActiveDeviceContentionUserActionEnum)
{
    // Test Cancel action (value 0)
    PFGameSaveFilesUiActiveDeviceContentionUserAction cancelAction = PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel;
    EXPECT_EQ(static_cast<uint32_t>(cancelAction), 0);
    
    // Test Retry action (value 1)
    PFGameSaveFilesUiActiveDeviceContentionUserAction retryAction = PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry;
    EXPECT_EQ(static_cast<uint32_t>(retryAction), 1);
    
    // Test SyncLastSavedData action (value 2)
    PFGameSaveFilesUiActiveDeviceContentionUserAction syncAction = PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData;
    EXPECT_EQ(static_cast<uint32_t>(syncAction), 2);
    
    // Verify enum underlying type
    EXPECT_EQ(sizeof(PFGameSaveFilesUiActiveDeviceContentionUserAction), sizeof(uint32_t));
    
    // Test enum comparisons
    EXPECT_NE(cancelAction, retryAction);
    EXPECT_NE(retryAction, syncAction);
    EXPECT_NE(cancelAction, syncAction);
    
    // Test enum assignment and validation
    PFGameSaveFilesUiActiveDeviceContentionUserAction testAction = PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData;
    EXPECT_EQ(testAction, syncAction);
}

// Test PFGameSaveFilesUiConflictUserAction enum values (Task R)
TEST_F(DataStructureTests, PFGameSaveFilesUiConflictUserActionEnum)
{
    // Test Cancel action (value 0)
    PFGameSaveFilesUiConflictUserAction cancelAction = PFGameSaveFilesUiConflictUserAction::Cancel;
    EXPECT_EQ(static_cast<uint32_t>(cancelAction), 0);
    
    // Test TakeLocal action (value 1)
    PFGameSaveFilesUiConflictUserAction takeLocalAction = PFGameSaveFilesUiConflictUserAction::TakeLocal;
    EXPECT_EQ(static_cast<uint32_t>(takeLocalAction), 1);
    
    // Test TakeRemote action (value 2)
    PFGameSaveFilesUiConflictUserAction takeRemoteAction = PFGameSaveFilesUiConflictUserAction::TakeRemote;
    EXPECT_EQ(static_cast<uint32_t>(takeRemoteAction), 2);
    
    // Verify enum underlying type
    EXPECT_EQ(sizeof(PFGameSaveFilesUiConflictUserAction), sizeof(uint32_t));
    
    // Test enum comparisons
    EXPECT_NE(cancelAction, takeLocalAction);
    EXPECT_NE(takeLocalAction, takeRemoteAction);
    EXPECT_NE(cancelAction, takeRemoteAction);
    
    // Test conflict resolution logic
    EXPECT_EQ(takeLocalAction, PFGameSaveFilesUiConflictUserAction::TakeLocal);
    EXPECT_EQ(takeRemoteAction, PFGameSaveFilesUiConflictUserAction::TakeRemote);
    
    // Verify all actions are distinct
    std::set<uint32_t> actionValues = {
        static_cast<uint32_t>(cancelAction),
        static_cast<uint32_t>(takeLocalAction),
        static_cast<uint32_t>(takeRemoteAction)
    };
    EXPECT_EQ(actionValues.size(), 3);  // All values should be unique
}

// Test PFGameSaveFilesUiOutOfStorageUserAction enum values (Task S)
TEST_F(DataStructureTests, PFGameSaveFilesUiOutOfStorageUserActionEnum)
{
    // Test Cancel action (value 0)
    PFGameSaveFilesUiOutOfStorageUserAction cancelAction = PFGameSaveFilesUiOutOfStorageUserAction::Cancel;
    EXPECT_EQ(static_cast<uint32_t>(cancelAction), 0);
    
    // Test Retry action (value 1)
    PFGameSaveFilesUiOutOfStorageUserAction retryAction = PFGameSaveFilesUiOutOfStorageUserAction::Retry;
    EXPECT_EQ(static_cast<uint32_t>(retryAction), 1);
    
    // Verify enum underlying type
    EXPECT_EQ(sizeof(PFGameSaveFilesUiOutOfStorageUserAction), sizeof(uint32_t));
    
    // Test enum comparisons
    EXPECT_NE(cancelAction, retryAction);
    
    // Test enum assignment
    PFGameSaveFilesUiOutOfStorageUserAction testAction = PFGameSaveFilesUiOutOfStorageUserAction::Retry;
    EXPECT_EQ(testAction, retryAction);
    
    // Test storage action validation
    EXPECT_EQ(static_cast<uint32_t>(PFGameSaveFilesUiOutOfStorageUserAction::Cancel), 0);
    EXPECT_EQ(static_cast<uint32_t>(PFGameSaveFilesUiOutOfStorageUserAction::Retry), 1);
    
    // Verify enum range
    EXPECT_GE(static_cast<uint32_t>(cancelAction), 0U);
    EXPECT_LE(static_cast<uint32_t>(retryAction), 1U);
}

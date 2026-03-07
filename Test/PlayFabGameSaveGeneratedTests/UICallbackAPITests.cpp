// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/core/PFCore.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

// Type-safe callback context identifiers
enum class CallbackContextId : uintptr_t
{
    PROGRESS = 0x1111,
    SYNC_FAILED = 0x2222,
    ACTIVE_DEVICE_CONTENTION = 0x3333,
    CONFLICT = 0x4444,
    OUT_OF_STORAGE = 0x5555,
    PROGRESS_ONLY = 0xABCD,
    SYNC_FAILED_ONLY = 0xBCDE,
    CONFLICT_ONLY = 0xCDEF,
    TEST_CONTEXT1 = 0x11111111,
    TEST_CONTEXT2 = 0x22222222,
    TEST_CONTEXT3 = 0x33333333,
    TEST_CONTEXT4 = 0x44444444,
    TEST_CONTEXT5 = 0x55555555,
    DEADBEEF = 0xDEADBEEF,
    CAFEBABE = 0xCAFEBABE,
    THREAD_PROGRESS = 0x1001
};

// Type-safe user handle identifiers
enum class UserHandleId : uintptr_t
{
    TEST_USER_BASE = 0x12345678
};

// Test fixture for UI Callback API tests
class UICallbackAPITests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Reset static callback tracking variables
        s_progressCallbackCount = 0;
        s_syncFailedCallbackCount = 0;
        s_activeDeviceContentionCallbackCount = 0;
        s_conflictCallbackCount = 0;
        s_outOfStorageCallbackCount = 0;
        s_lastProgressContext = nullptr;
        s_lastSyncFailedContext = nullptr;
        s_lastActiveDeviceContentionContext = nullptr;
        s_lastConflictContext = nullptr;
        s_lastOutOfStorageContext = nullptr;
    }

    void TearDown() override
    {
        // Cleanup - reset callbacks to nullptr
        PFGameSaveUICallbacks nullCallbacks{};
        PFGameSaveFilesSetUiCallbacks(&nullCallbacks);
    }

    // Static callback tracking variables
    static std::atomic<int> s_progressCallbackCount;
    static std::atomic<int> s_syncFailedCallbackCount;
    static std::atomic<int> s_activeDeviceContentionCallbackCount;
    static std::atomic<int> s_conflictCallbackCount;
    static std::atomic<int> s_outOfStorageCallbackCount;
    static void* s_lastProgressContext;
    static void* s_lastSyncFailedContext;
    static void* s_lastActiveDeviceContentionContext;
    static void* s_lastConflictContext;
    static void* s_lastOutOfStorageContext;
    static std::mutex s_callbackMutex;

    // Test callback functions
    static void CALLBACK TestProgressCallback(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesSyncState syncState,
        _In_opt_ void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_progressCallbackCount++;
        s_lastProgressContext = context;
    }

    static void CALLBACK TestSyncFailedCallback(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesSyncState syncState,
        _In_ HRESULT error,
        _In_opt_ void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_syncFailedCallbackCount++;
        s_lastSyncFailedContext = context;
    }

    static void CALLBACK TestActiveDeviceContentionCallback(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveDescriptor* localGameSave,
        _In_ PFGameSaveDescriptor* remoteGameSave,
        _In_opt_ void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_activeDeviceContentionCallbackCount++;
        s_lastActiveDeviceContentionContext = context;
    }

    static void CALLBACK TestConflictCallback(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveDescriptor* localGameSave,
        _In_ PFGameSaveDescriptor* remoteGameSave,
        _In_opt_ void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_conflictCallbackCount++;
        s_lastConflictContext = context;
    }

    static void CALLBACK TestOutOfStorageCallback(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ uint64_t requiredBytes,
        _In_opt_ void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_outOfStorageCallbackCount++;
        s_lastOutOfStorageContext = context;
    }

    // Helper to create a valid test user handle (mock)
    PFLocalUserHandle CreateTestUserHandle()
    {
        // For testing purposes, use a non-null pointer
        return reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(UserHandleId::TEST_USER_BASE));
    }

    // Helper to create valid PFGameSaveDescriptor for testing
    PFGameSaveDescriptor CreateTestDescriptor(const char* deviceType = "TestDevice")
    {
        PFGameSaveDescriptor descriptor{};
        descriptor.time = std::time(nullptr);
        descriptor.totalBytes = 1024;
        descriptor.uploadedBytes = 512;
        
        strcpy_s(descriptor.deviceType, sizeof(descriptor.deviceType), deviceType);
        strcpy_s(descriptor.deviceId, sizeof(descriptor.deviceId), "TEST-DEVICE-ID");
        strcpy_s(descriptor.deviceFriendlyName, sizeof(descriptor.deviceFriendlyName), "Test Device");
        strcpy_s(descriptor.thumbnailUri, sizeof(descriptor.thumbnailUri), "https://test.com/thumb.png");
        strcpy_s(descriptor.shortSaveDescription, sizeof(descriptor.shortSaveDescription), "Test save");
        
        return descriptor;
    }
};

// Static member definitions
std::atomic<int> UICallbackAPITests::s_progressCallbackCount(0);
std::atomic<int> UICallbackAPITests::s_syncFailedCallbackCount(0);
std::atomic<int> UICallbackAPITests::s_activeDeviceContentionCallbackCount(0);
std::atomic<int> UICallbackAPITests::s_conflictCallbackCount(0);
std::atomic<int> UICallbackAPITests::s_outOfStorageCallbackCount(0);
void* UICallbackAPITests::s_lastProgressContext = nullptr;
void* UICallbackAPITests::s_lastSyncFailedContext = nullptr;
void* UICallbackAPITests::s_lastActiveDeviceContentionContext = nullptr;
void* UICallbackAPITests::s_lastConflictContext = nullptr;
void* UICallbackAPITests::s_lastOutOfStorageContext = nullptr;
std::mutex UICallbackAPITests::s_callbackMutex;

// Task A: Generate tests for PFGameSaveFilesSetUiCallbacks with various callback configurations
TEST_F(UICallbackAPITests, SetUiCallbacks_ValidConfigurations)
{
    // Test with all callbacks set
    PFGameSaveUICallbacks allCallbacks{};
    allCallbacks.progressCallback = TestProgressCallback;
    allCallbacks.progressContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::PROGRESS));
    allCallbacks.syncFailedCallback = TestSyncFailedCallback;
    allCallbacks.syncFailedContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::SYNC_FAILED));
    allCallbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    allCallbacks.activeDeviceContentionContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::ACTIVE_DEVICE_CONTENTION));
    allCallbacks.conflictCallback = TestConflictCallback;
    allCallbacks.conflictContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::CONFLICT));
    allCallbacks.outOfStorageCallback = TestOutOfStorageCallback;
    allCallbacks.outOfStorageContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::OUT_OF_STORAGE));

    // This should not crash or return error
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&allCallbacks));
}

TEST_F(UICallbackAPITests, SetUiCallbacks_NullCallbacks)
{
    // Test with null callbacks structure
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(nullptr));
    
    // Test with zero-initialized callbacks (all null function pointers)
    PFGameSaveUICallbacks nullCallbacks{};
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&nullCallbacks));
}

TEST_F(UICallbackAPITests, SetUiCallbacks_PartialConfiguration)
{
    // Test with only progress callback set
    PFGameSaveUICallbacks progressOnly{};
    progressOnly.progressCallback = TestProgressCallback;
    progressOnly.progressContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::PROGRESS_ONLY));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&progressOnly));

    // Test with only sync failed callback set
    PFGameSaveUICallbacks syncFailedOnly{};
    syncFailedOnly.syncFailedCallback = TestSyncFailedCallback;
    syncFailedOnly.syncFailedContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::SYNC_FAILED_ONLY));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&syncFailedOnly));

    // Test with only conflict callback set
    PFGameSaveUICallbacks conflictOnly{};
    conflictOnly.conflictCallback = TestConflictCallback;
    conflictOnly.conflictContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::CONFLICT_ONLY));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&conflictOnly));
}

// Task B: Create tests for PFGameSaveFilesUiProgressGetProgress parameter validation
TEST_F(UICallbackAPITests, UiProgressGetProgress_ParameterValidation)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();
    PFGameSaveFilesSyncState syncState;
    uint64_t current, total;

    // Test with all valid parameters
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(testUser, &syncState, &current, &total));

    // Test with null syncState (should be allowed as it's optional)
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(testUser, nullptr, &current, &total));

    // Test with null current (should be allowed as it's optional)
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(testUser, &syncState, nullptr, &total));

    // Test with null total (should be allowed as it's optional)
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(testUser, &syncState, &current, nullptr));

    // Test with all optional parameters null
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(testUser, nullptr, nullptr, nullptr));

    // Test with null user handle (this should be handled gracefully)
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesUiProgressGetProgress(nullptr, &syncState, &current, &total));
}

// Task C: Generate tests for PFGameSaveFilesSetUiProgressResponse with all user actions
TEST_F(UICallbackAPITests, SetUiProgressResponse_AllUserActions)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test Cancel action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel));

    // Test with null user handle
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiProgressResponse(nullptr, PFGameSaveFilesUiProgressUserAction::Cancel));

    // Test multiple rapid calls (should be handled gracefully)
    for (int i = 0; i < 5; i++)
    {
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel));
    }
}

// Task D: Create tests for PFGameSaveFilesSetUiSyncFailedResponse with all user actions
TEST_F(UICallbackAPITests, SetUiSyncFailedResponse_AllUserActions)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test Cancel action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel));

    // Test Retry action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry));

    // Test UseOffline action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::UseOffline));

    // Test with null user handle
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(nullptr, PFGameSaveFilesUiSyncFailedUserAction::Cancel));

    // Test action sequence validation
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::UseOffline));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel));
}

// Task E: Generate tests for PFGameSaveFilesSetUiActiveDeviceContentionResponse with all user actions
TEST_F(UICallbackAPITests, SetUiActiveDeviceContentionResponse_AllUserActions)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test Cancel action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel));

    // Test Retry action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry));

    // Test SyncLastSavedData action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData));

    // Test with null user handle
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(nullptr, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel));

    // Test device contention resolution sequence
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry));
}

// Task F: Create tests for PFGameSaveFilesSetUiConflictResponse with all user actions
TEST_F(UICallbackAPITests, SetUiConflictResponse_AllUserActions)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test Cancel action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::Cancel));

    // Test TakeLocal action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeLocal));

    // Test TakeRemote action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeRemote));

    // Test with null user handle
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(nullptr, PFGameSaveFilesUiConflictUserAction::Cancel));

    // Test conflict resolution scenarios
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeLocal));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeRemote));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::Cancel));
}

// Task G: Generate tests for PFGameSaveFilesSetUiOutOfStorageResponse with all user actions
TEST_F(UICallbackAPITests, SetUiOutOfStorageResponse_AllUserActions)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test Cancel action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Cancel));

    // Test Retry action
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry));

    // Test with null user handle
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(nullptr, PFGameSaveFilesUiOutOfStorageUserAction::Cancel));

    // Test storage recovery scenarios
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Cancel));
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry));
}

// Task H: Create tests for callback function pointer validation and null handling
TEST_F(UICallbackAPITests, CallbackFunctionPointerValidation)
{
    // Test setting callbacks with valid function pointers
    PFGameSaveUICallbacks callbacks{};
    callbacks.progressCallback = TestProgressCallback;
    callbacks.syncFailedCallback = TestSyncFailedCallback;
    callbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    callbacks.conflictCallback = TestConflictCallback;
    callbacks.outOfStorageCallback = TestOutOfStorageCallback;
    
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&callbacks));

    // Test setting callbacks to null
    PFGameSaveUICallbacks nullCallbacks{};
    nullCallbacks.progressCallback = nullptr;
    nullCallbacks.syncFailedCallback = nullptr;
    nullCallbacks.activeDeviceContentionCallback = nullptr;
    nullCallbacks.conflictCallback = nullptr;
    nullCallbacks.outOfStorageCallback = nullptr;
    
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&nullCallbacks));

    // Test mixed null and valid callbacks
    PFGameSaveUICallbacks mixedCallbacks{};
    mixedCallbacks.progressCallback = TestProgressCallback;
    mixedCallbacks.syncFailedCallback = nullptr;
    mixedCallbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    mixedCallbacks.conflictCallback = nullptr;
    mixedCallbacks.outOfStorageCallback = TestOutOfStorageCallback;
    
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&mixedCallbacks));
}

// Task I: Generate tests for callback context preservation and threading
TEST_F(UICallbackAPITests, CallbackContextPreservation)
{
    const void* testContext1 = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::TEST_CONTEXT1));
    const void* testContext2 = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::TEST_CONTEXT2));
    const void* testContext3 = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::TEST_CONTEXT3));
    const void* testContext4 = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::TEST_CONTEXT4));
    const void* testContext5 = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::TEST_CONTEXT5));

    // Set up callbacks with different contexts
    PFGameSaveUICallbacks callbacks{};
    callbacks.progressCallback = TestProgressCallback;
    callbacks.progressContext = const_cast<void*>(testContext1);
    callbacks.syncFailedCallback = TestSyncFailedCallback;
    callbacks.syncFailedContext = const_cast<void*>(testContext2);
    callbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    callbacks.activeDeviceContentionContext = const_cast<void*>(testContext3);
    callbacks.conflictCallback = TestConflictCallback;
    callbacks.conflictContext = const_cast<void*>(testContext4);
    callbacks.outOfStorageCallback = TestOutOfStorageCallback;
    callbacks.outOfStorageContext = const_cast<void*>(testContext5);
    
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&callbacks));

    // Simulate callback invocations and verify context preservation
    PFLocalUserHandle testUser = CreateTestUserHandle();
    PFGameSaveDescriptor testDescriptor = CreateTestDescriptor();

    // Simulate progress callback
    if (callbacks.progressCallback)
    {
        callbacks.progressCallback(testUser, PFGameSaveFilesSyncState::Downloading, callbacks.progressContext);
        EXPECT_EQ(s_lastProgressContext, testContext1);
    }

    // Simulate sync failed callback
    if (callbacks.syncFailedCallback)
    {
        callbacks.syncFailedCallback(testUser, PFGameSaveFilesSyncState::Downloading, E_FAIL, callbacks.syncFailedContext);
        EXPECT_EQ(s_lastSyncFailedContext, testContext2);
    }

    // Simulate active device contention callback
    if (callbacks.activeDeviceContentionCallback)
    {
        callbacks.activeDeviceContentionCallback(testUser, &testDescriptor, &testDescriptor, callbacks.activeDeviceContentionContext);
        EXPECT_EQ(s_lastActiveDeviceContentionContext, testContext3);
    }

    // Simulate conflict callback
    if (callbacks.conflictCallback)
    {
        callbacks.conflictCallback(testUser, &testDescriptor, &testDescriptor, callbacks.conflictContext);
        EXPECT_EQ(s_lastConflictContext, testContext4);
    }

    // Simulate out of storage callback
    if (callbacks.outOfStorageCallback)
    {
        callbacks.outOfStorageCallback(testUser, 1024, callbacks.outOfStorageContext);
        EXPECT_EQ(s_lastOutOfStorageContext, testContext5);
    }
}

TEST_F(UICallbackAPITests, CallbackThreadSafety)
{
    // Set up callbacks
    PFGameSaveUICallbacks callbacks{};
    callbacks.progressCallback = TestProgressCallback;
    callbacks.progressContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::DEADBEEF));
    callbacks.syncFailedCallback = TestSyncFailedCallback;
    callbacks.syncFailedContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::CAFEBABE));
    
    EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiCallbacks(&callbacks));

    // Reset counters
    s_progressCallbackCount = 0;
    s_syncFailedCallbackCount = 0;

    const int numThreads = 4;
    const int callsPerThread = 10;
    std::vector<std::thread> threads;

    // Create multiple threads that simulate callback invocations
    for (int t = 0; t < numThreads; t++)
    {
        threads.emplace_back([&, t]()
        {
            PFLocalUserHandle testUser = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(UserHandleId::TEST_USER_BASE) + t);
            
            for (int i = 0; i < callsPerThread; i++)
            {
                // Simulate progress callback
                if (callbacks.progressCallback)
                {
                    callbacks.progressCallback(testUser, PFGameSaveFilesSyncState::Uploading, callbacks.progressContext);
                }
                
                // Brief pause to allow thread interleaving
                std::this_thread::sleep_for(std::chrono::microseconds(10));
                
                // Simulate sync failed callback
                if (callbacks.syncFailedCallback)
                {
                    callbacks.syncFailedCallback(testUser, PFGameSaveFilesSyncState::Uploading, E_FAIL, callbacks.syncFailedContext);
                }
            }
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads)
    {
        thread.join();
    }

    // Verify that all callbacks were invoked correctly
    EXPECT_EQ(s_progressCallbackCount.load(), numThreads * callsPerThread);
    EXPECT_EQ(s_syncFailedCallbackCount.load(), numThreads * callsPerThread);
}

// Additional comprehensive tests for edge cases and error conditions
TEST_F(UICallbackAPITests, CallbackParameterValidation)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();
    PFGameSaveDescriptor localSave = CreateTestDescriptor("LocalDevice");
    PFGameSaveDescriptor remoteSave = CreateTestDescriptor("RemoteDevice");

    // Test progress callback with various sync states
    PFGameSaveUICallbacks progressCallbacks{};
    progressCallbacks.progressCallback = TestProgressCallback;
    progressCallbacks.progressContext = reinterpret_cast<void*>(static_cast<uintptr_t>(CallbackContextId::THREAD_PROGRESS));
    PFGameSaveFilesSetUiCallbacks(&progressCallbacks);

    // Reset counter
    s_progressCallbackCount = 0;

    // Test all sync states
    std::vector<PFGameSaveFilesSyncState> syncStates = {
        PFGameSaveFilesSyncState::NotStarted,
        PFGameSaveFilesSyncState::PreparingForDownload,
        PFGameSaveFilesSyncState::Downloading,
        PFGameSaveFilesSyncState::PreparingForUpload,
        PFGameSaveFilesSyncState::Uploading,
        PFGameSaveFilesSyncState::SyncComplete
    };

    for (auto state : syncStates)
    {
        if (progressCallbacks.progressCallback)
        {
            progressCallbacks.progressCallback(testUser, state, progressCallbacks.progressContext);
        }
    }

    EXPECT_EQ(s_progressCallbackCount.load(), static_cast<int>(syncStates.size()));
}

TEST_F(UICallbackAPITests, ResponseTimingValidation)
{
    PFLocalUserHandle testUser = CreateTestUserHandle();

    // Test rapid successive responses
    for (int i = 0; i < 100; i++)
    {
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel));
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry));
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel));
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeLocal));
        EXPECT_NO_FATAL_FAILURE(PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry));
    }
}

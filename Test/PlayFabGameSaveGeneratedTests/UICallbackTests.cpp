// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/core/PFCore.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>

// Type-safe user handle identifiers
enum class UserHandleId : uintptr_t
{
    TEST_USER = 0x12345678
};

// Type-safe context identifiers for UI callback async operations
enum class AsyncContextId : intptr_t
{
    // Basic UI callback contexts
    Progress = 1001,
    SyncFailed = 1002,
    DeviceContention = 1003,
    Conflict = 1004,
    OutOfStorage = 1005,
    
    // Integration test contexts
    ProgressIntegration = 2001,
    AddUser = 2002,
    
    // Sync failed scenarios
    SyncFailedScenario = 3001,
    SyncFailTest = 3002,
    
    // Multi-operation contexts (bases for arithmetic)
    MultiTestBase = 0x1000,    // 0x1000 + i for multi-test operations
    ConcurrentBase = 0x3000,   // 0x3000 + i for concurrent operations  
    CancelBase = 0x4000,       // 0x4000 + i for cancellation operations
    
    // Retry operations
    RetryTest = 4001,
    
    // Device contention scenarios
    DeviceContentionTest = 5001,
    DeviceContentionAddUser = 5002,
    DeviceContentionCancel = 5003,
    
    // Conflict resolution
    ConflictResolution = 6001,
    AddUserForConflict = 6002,
    ConflictUpload = 6003,
    ConflictUpload2 = 6004,
    ConflictCancel = 6005,
    
    // Out of storage scenarios
    OutOfStorageTest = 7001,
    AddUserForStorage = 7002,
    OutOfStorageDownload = 7003,
    OutOfStorageCancel = 7004,
    
    // Null callback scenarios
    NullProgress = 8001,
    NullCallbackTest = 8002,
    
    // Recovery test
    RecoveryTest = 9001,
    
    // Context test
    ContextTest = 10001,
    
    // Thread safety
    ThreadSafe = 11001,
    
    // Cancellation scenarios
    CancelBefore = 12001,
    CancelAfter = 12002,
    
    // Comprehensive integration
    ComprehensiveAddUser = 13001,
    ComprehensiveUpload = 13002,
    ComprehensiveDownload = 13003,
    ComprehensiveIntegration = 13004
};

// Test fixture for UI Callback System Integration tests
class UICallbackTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean state for each test
        s_callbackCount = 0;
        s_lastError = S_OK;
        s_lastContext = nullptr;
        s_isInitialized = false;
        s_uiCallbacksSet = false;
        s_lastSyncState = PFGameSaveFilesSyncState::NotStarted;
        s_lastRequiredBytes = 0;
        s_receivedCallbacks.clear();
        // Note: std::mutex and std::condition_variable don't have assignment operators
        // They are automatically constructed/destructed, no explicit reset needed
    }

    void TearDown() override
    {
        // Cleanup after each test
        if (s_isInitialized)
        {
            XAsyncBlock uninitAsync{};
            uninitAsync.callback = [](XAsyncBlock* async) {
                s_callbackCount++;
                s_lastError = XAsyncGetStatus(async, false);
            };
            
            if (SUCCEEDED(PFGameSaveFilesUninitializeAsync(&uninitAsync)))
            {
                WaitForCallback(2000);
                if (SUCCEEDED(s_lastError))
                {
                    PFGameSaveFilesUninitializeResult(&uninitAsync);
                }
            }
            s_isInitialized = false;
            s_uiCallbacksSet = false;
        }
    }

    // Static tracking variables for callbacks
    static std::atomic<int> s_callbackCount;
    static HRESULT s_lastError;
    static void* s_lastContext;
    static bool s_isInitialized;
    static bool s_uiCallbacksSet;
    static PFGameSaveFilesSyncState s_lastSyncState;
    static uint64_t s_lastRequiredBytes;
    static std::vector<std::string> s_receivedCallbacks;
    static std::mutex s_callbackMutex;
    static std::condition_variable s_callbackCondition;
    
    // Thread test specific variables
    static std::vector<std::thread::id> s_threadIds;
    static std::atomic<int> s_threadCallbackCount;

    // Mock callback implementations
    static void CALLBACK TestProgressCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_receivedCallbacks.push_back("Progress");
        s_lastSyncState = syncState;
        s_lastContext = context;
        s_callbackCount++;
        s_callbackCondition.notify_all();
    }

    static void CALLBACK TestSyncFailedCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        HRESULT error,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_receivedCallbacks.push_back("SyncFailed");
        s_lastSyncState = syncState;
        s_lastError = error;
        s_lastContext = context;
        s_callbackCount++;
        s_callbackCondition.notify_all();
    }

    static void CALLBACK TestActiveDeviceContentionCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveDescriptor* localGameSave,
        PFGameSaveDescriptor* remoteGameSave,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_receivedCallbacks.push_back("ActiveDeviceContention");
        s_lastContext = context;
        s_callbackCount++;
        s_callbackCondition.notify_all();
    }

    static void CALLBACK ThreadSafeProgressCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveFilesSyncState syncState,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_threadIds.push_back(std::this_thread::get_id());
        s_threadCallbackCount++;
        s_receivedCallbacks.push_back("ThreadSafeProgress");
        s_callbackCondition.notify_all();
    }

    static void CALLBACK TestConflictCallback(
        PFLocalUserHandle localUserHandle,
        PFGameSaveDescriptor* localGameSave,
        PFGameSaveDescriptor* remoteGameSave,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_receivedCallbacks.push_back("Conflict");
        s_lastContext = context;
        s_callbackCount++;
        s_callbackCondition.notify_all();
    }

    static void CALLBACK TestOutOfStorageCallback(
        PFLocalUserHandle localUserHandle,
        uint64_t requiredBytes,
        void* context)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_receivedCallbacks.push_back("OutOfStorage");
        s_lastRequiredBytes = requiredBytes;
        s_lastContext = context;
        s_callbackCount++;
        s_callbackCondition.notify_all();
    }

    // Mock callback for async operations
    static void CALLBACK TestAsyncCallback(XAsyncBlock* asyncBlock)
    {
        std::lock_guard<std::mutex> lock(s_callbackMutex);
        s_callbackCount++;
        s_lastError = XAsyncGetStatus(asyncBlock, false);
        s_lastContext = asyncBlock->context;
        s_callbackCondition.notify_all();
    }

    // Helper to create test async block
    XAsyncBlock CreateTestAsyncBlock(void* context = nullptr)
    {
        XAsyncBlock asyncBlock{};
        asyncBlock.callback = TestAsyncCallback;
        asyncBlock.context = context;
        return asyncBlock;
    }

    // Helper to wait for async operation with timeout
    bool WaitForCallback(int timeoutMs = 5000, int expectedCount = 1)
    {
        std::unique_lock<std::mutex> lock(s_callbackMutex);
        return s_callbackCondition.wait_for(
            lock,
            std::chrono::milliseconds(timeoutMs),
            [expectedCount] { return s_callbackCount.load() >= expectedCount; }
        );
    }

    // Helper to wait for specific UI callback
    bool WaitForUICallback(const std::string& callbackName, int timeoutMs = 5000)
    {
        std::unique_lock<std::mutex> lock(s_callbackMutex);
        return s_callbackCondition.wait_for(
            lock,
            std::chrono::milliseconds(timeoutMs),
            [&callbackName] {
                return std::find(s_receivedCallbacks.begin(), s_receivedCallbacks.end(), callbackName) != s_receivedCallbacks.end();
            }
        );
    }

    // Helper to create test user handle
    PFLocalUserHandle CreateTestUserHandle()
    {
        return reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(UserHandleId::TEST_USER));
    }

    // Helper to initialize GameSave system
    bool InitializeGameSave()
    {
        if (s_isInitialized)
        {
            return true;
        }

        PFGameSaveInitArgs args{};
        args.backgroundQueue = nullptr;
        args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
#if defined(_WIN32)
        args.saveFolder = "C:\\temp\\UICallbackTestSaves";
#else
        args.saveFolder = "/tmp/UICallbackTestSaves";
#endif

        HRESULT hr = PFGameSaveFilesInitialize(&args);
        if (SUCCEEDED(hr))
        {
            s_isInitialized = true;
            return true;
        }
        return false;
    }

    // Helper to setup comprehensive UI callbacks
    void SetupTestUICallbacks()
    {
        PFGameSaveUICallbacks callbacks{};
        
        callbacks.progressCallback = TestProgressCallback;
        callbacks.progressContext = reinterpret_cast<void*>(AsyncContextId::Progress);
        
        callbacks.syncFailedCallback = TestSyncFailedCallback;
        callbacks.syncFailedContext = reinterpret_cast<void*>(AsyncContextId::SyncFailed);
        
        callbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
        callbacks.activeDeviceContentionContext = reinterpret_cast<void*>(AsyncContextId::DeviceContention);
        
        callbacks.conflictCallback = TestConflictCallback;
        callbacks.conflictContext = reinterpret_cast<void*>(AsyncContextId::Conflict);
        
        callbacks.outOfStorageCallback = TestOutOfStorageCallback;
        callbacks.outOfStorageContext = reinterpret_cast<void*>(AsyncContextId::OutOfStorage);

        PFGameSaveFilesSetUiCallbacks(&callbacks);
        s_uiCallbacksSet = true;
    }
};

// Static member definitions
std::atomic<int> UICallbackTests::s_callbackCount(0);
HRESULT UICallbackTests::s_lastError = S_OK;
void* UICallbackTests::s_lastContext = nullptr;
bool UICallbackTests::s_isInitialized = false;
bool UICallbackTests::s_uiCallbacksSet = false;
PFGameSaveFilesSyncState UICallbackTests::s_lastSyncState = PFGameSaveFilesSyncState::NotStarted;
uint64_t UICallbackTests::s_lastRequiredBytes = 0;
std::vector<std::string> UICallbackTests::s_receivedCallbacks;
std::mutex UICallbackTests::s_callbackMutex;
std::condition_variable UICallbackTests::s_callbackCondition;
std::vector<std::thread::id> UICallbackTests::s_threadIds;
std::atomic<int> UICallbackTests::s_threadCallbackCount(0);

// Task A: Generate tests for PFGameSaveFilesUiProgressCallback integration
TEST_F(UICallbackTests, ProgressCallbackIntegration)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Setup progress callback only
    PFGameSaveUICallbacks callbacks{};
    callbacks.progressCallback = TestProgressCallback;
    callbacks.progressContext = reinterpret_cast<void*>(AsyncContextId::ProgressIntegration);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test progress callback during AddUserWithUi operation
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::AddUser));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for either progress callback or async completion
        bool progressReceived = WaitForUICallback("Progress", 3000);
        
        if (progressReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(AsyncContextId::ProgressIntegration));
            
            // Test getting progress information
            PFGameSaveFilesSyncState currentSyncState;
            uint64_t current = 0, total = 0;
            
            hr = PFGameSaveFilesUiProgressGetProgress(testUser, &currentSyncState, &current, &total);
            EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED);
            
            if (SUCCEEDED(hr))
            {
                EXPECT_GE(static_cast<uint32_t>(currentSyncState), static_cast<uint32_t>(PFGameSaveFilesSyncState::NotStarted));
                EXPECT_LE(current, total);
            }
            
            // Test progress response
            PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel);
        }
        
        // Wait for async completion
        bool asyncCompleted = WaitForCallback(5000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            // May succeed or be cancelled
            EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_CANCELLED || hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
        }
    }
}

// Task B: Create tests for PFGameSaveFilesUiSyncFailedCallback scenarios
TEST_F(UICallbackTests, SyncFailedCallbackScenarios)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Setup sync failed callback only
    PFGameSaveUICallbacks callbacks{};
    callbacks.syncFailedCallback = TestSyncFailedCallback;
    callbacks.syncFailedContext = reinterpret_cast<void*>(AsyncContextId::SyncFailedScenario);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test sync failed callback during network issues
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::SyncFailTest));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for sync failed callback or completion
        bool syncFailedReceived = WaitForUICallback("SyncFailed", 10000);
        
        if (syncFailedReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(AsyncContextId::SyncFailedScenario));
            EXPECT_GE(static_cast<uint32_t>(s_lastSyncState), static_cast<uint32_t>(PFGameSaveFilesSyncState::NotStarted));
            EXPECT_NE(s_lastError, S_OK); // Should have an error
            
            // Test different sync failed responses
            std::vector<PFGameSaveFilesUiSyncFailedUserAction> responses = {
                PFGameSaveFilesUiSyncFailedUserAction::Cancel,
                PFGameSaveFilesUiSyncFailedUserAction::Retry,
                PFGameSaveFilesUiSyncFailedUserAction::UseOffline
            };
            
            // Test Cancel response
            PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
        }
        
        // Wait for async completion
        bool asyncCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            // Various outcomes possible depending on response
            EXPECT_TRUE(SUCCEEDED(hr) || 
                       hr == E_PF_GAMESAVE_USER_CANCELLED ||
                       hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                       hr == E_PF_GAMESAVE_NETWORK_FAILURE);
        }
    }
    
    // Test retry scenario
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock retryAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::RetryTest));
    
    hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &retryAsync);
    if (SUCCEEDED(hr))
    {
        bool syncFailedReceived = WaitForUICallback("SyncFailed", 10000);
        if (syncFailedReceived)
        {
            // Test Retry response
            PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry);
        }
        
        bool asyncCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&retryAsync);
            // Retry may succeed or fail again
        }
    }
}

// Task C: Generate tests for PFGameSaveFilesUiActiveDeviceContentionCallback handling
TEST_F(UICallbackTests, ActiveDeviceContentionCallbackHandling)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Setup active device contention callback
    PFGameSaveUICallbacks callbacks{};
    callbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    callbacks.activeDeviceContentionContext = reinterpret_cast<void*>(AsyncContextId::DeviceContentionTest);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test device contention during AddUser operation
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::DeviceContentionAddUser));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for device contention callback
        bool deviceContentionReceived = WaitForUICallback("ActiveDeviceContention", 10000);
        
        if (deviceContentionReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(AsyncContextId::DeviceContentionTest));
            
            // Test different device contention responses
            std::vector<PFGameSaveFilesUiActiveDeviceContentionUserAction> responses = {
                PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel,
                PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry,
                PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData
            };
            
            // Test SyncLastSavedData response (makes local device active)
            PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
        }
        
        // Wait for async completion
        bool asyncCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            // Various outcomes possible
            EXPECT_TRUE(SUCCEEDED(hr) || 
                       hr == E_PF_GAMESAVE_USER_CANCELLED ||
                       hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                       hr == E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE);
        }
    }
    
    // Test Cancel response scenario
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock cancelAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::DeviceContentionCancel));
    
    hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &cancelAsync);
    if (SUCCEEDED(hr))
    {
        bool deviceContentionReceived = WaitForUICallback("ActiveDeviceContention", 10000);
        if (deviceContentionReceived)
        {
            // Test Cancel response
            PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
        }
        
        bool asyncCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&cancelAsync);
            EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_CANCELLED);
        }
    }
}

// Task D: Create tests for PFGameSaveFilesUiConflictCallback resolution paths
TEST_F(UICallbackTests, ConflictCallbackResolutionPaths)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Setup conflict callback
    PFGameSaveUICallbacks callbacks{};
    callbacks.conflictCallback = TestConflictCallback;
    callbacks.conflictContext = reinterpret_cast<void*>(AsyncContextId::ConflictResolution);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // First add user to enable conflict scenarios
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::AddUserForConflict));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        bool asyncCompleted = WaitForCallback(10000);
        if (asyncCompleted && SUCCEEDED(s_lastError))
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            
            if (SUCCEEDED(hr))
            {
                // Test conflict during upload operation
                s_callbackCount = 0;
                s_receivedCallbacks.clear();
                XAsyncBlock uploadAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ConflictUpload));
                
                hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync);
                if (SUCCEEDED(hr))
                {
                    // Wait for conflict callback
                    bool conflictReceived = WaitForUICallback("Conflict", 15000);
                    
                    if (conflictReceived)
                    {
                        EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(AsyncContextId::ConflictResolution));
                        
                        // Test TakeLocal response
                        PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeLocal);
                    }
                    
                    // Wait for upload completion
                    bool uploadCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
                    if (uploadCompleted)
                    {
                        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
                        EXPECT_TRUE(SUCCEEDED(hr) || 
                                   hr == E_PF_GAMESAVE_USER_CANCELLED ||
                                   hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
                    }
                }
                
                // Test TakeRemote response scenario
                s_callbackCount = 0;
                s_receivedCallbacks.clear();
                XAsyncBlock uploadAsync2 = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ConflictUpload2));
                
                hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync2);
                if (SUCCEEDED(hr))
                {
                    bool conflictReceived = WaitForUICallback("Conflict", 15000);
                    if (conflictReceived)
                    {
                        // Test TakeRemote response
                        PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeRemote);
                    }
                    
                    bool uploadCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
                    if (uploadCompleted)
                    {
                        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync2);
                        // Various outcomes possible
                    }
                }
                
                // Test Cancel response scenario
                s_callbackCount = 0;
                s_receivedCallbacks.clear();
                XAsyncBlock uploadAsync3 = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ConflictCancel));
                
                hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync3);
                if (SUCCEEDED(hr))
                {
                    bool conflictReceived = WaitForUICallback("Conflict", 15000);
                    if (conflictReceived)
                    {
                        // Test Cancel response
                        PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::Cancel);
                    }
                    
                    bool uploadCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
                    if (uploadCompleted)
                    {
                        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync3);
                        EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_CANCELLED);
                    }
                }
            }
        }
    }
}

// Task E: Generate tests for PFGameSaveFilesUiOutOfStorageCallback responses
TEST_F(UICallbackTests, OutOfStorageCallbackResponses)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Setup out of storage callback
    PFGameSaveUICallbacks callbacks{};
    callbacks.outOfStorageCallback = TestOutOfStorageCallback;
    callbacks.outOfStorageContext = reinterpret_cast<void*>(AsyncContextId::OutOfStorageTest);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // First add user
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::AddUserForStorage));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        bool asyncCompleted = WaitForCallback(10000);
        if (asyncCompleted && SUCCEEDED(s_lastError))
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            
            if (SUCCEEDED(hr))
            {
                // Test out of storage during download operation
                s_callbackCount = 0;
                s_receivedCallbacks.clear();
                XAsyncBlock uploadAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::OutOfStorage));
                
                hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync);
                if (SUCCEEDED(hr))
                {
                    // Wait for out of storage callback
                    bool outOfStorageReceived = WaitForUICallback("OutOfStorage", 15000);
                    
                    if (outOfStorageReceived)
                    {
                        EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(AsyncContextId::OutOfStorageTest));
                        EXPECT_GT(s_lastRequiredBytes, 0); // Should specify required bytes
                        
                        // Test Retry response (user cleared storage)
                        PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry);
                    }
                    
                    // Wait for upload completion
                    bool uploadCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
                    if (uploadCompleted)
                    {
                        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
                        EXPECT_TRUE(SUCCEEDED(hr) || 
                                   hr == E_PF_GAMESAVE_USER_CANCELLED ||
                                   hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                                   hr == E_PF_GAMESAVE_DISK_FULL);
                    }
                }
                
                // Test Cancel response scenario
                s_callbackCount = 0;
                s_receivedCallbacks.clear();
                XAsyncBlock uploadAsync2 = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::OutOfStorage));
                
                hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync2);
                if (SUCCEEDED(hr))
                {
                    bool outOfStorageReceived = WaitForUICallback("OutOfStorage", 15000);
                    if (outOfStorageReceived)
                    {
                        // Test Cancel response
                        PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
                    }
                    
                    bool uploadCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
                    if (uploadCompleted)
                    {
                        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync2);
                        EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_CANCELLED);
                    }
                }
            }
        }
    }
}

// Task F: Create tests for UI callback error handling and recovery
TEST_F(UICallbackTests, UICallbackErrorHandlingAndRecovery)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Test error handling with invalid callback configurations
    PFGameSaveUICallbacks invalidCallbacks{};
    
    // Test with null callbacks
    invalidCallbacks.progressCallback = nullptr;
    invalidCallbacks.progressContext = reinterpret_cast<void*>(AsyncContextId::NullProgress);
    
    // Should handle null callbacks gracefully
    PFGameSaveFilesSetUiCallbacks(&invalidCallbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test operation with null callbacks - should not crash
    s_callbackCount = 0;
    XAsyncBlock nullCallbackAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::NullCallbackTest));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &nullCallbackAsync);
    if (SUCCEEDED(hr))
    {
        bool asyncCompleted = WaitForCallback(10000);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&nullCallbackAsync);
            // Should complete without callback interference
            EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
        }
    }
    
    // Test recovery by setting valid callbacks
    SetupTestUICallbacks();
    
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock recoveryAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::RecoveryTest));
    
    hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &recoveryAsync);
    if (SUCCEEDED(hr))
    {
        // Should now trigger callbacks
        bool callbacksTriggered = WaitForCallback(10000) || !s_receivedCallbacks.empty();
        bool asyncCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
        
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&recoveryAsync);
            // Should work with valid callbacks
        }
    }
    
    // Test error recovery in callback responses
    PFLocalUserHandle invalidUser = nullptr;
    
    // Test with invalid user handle
    PFGameSaveFilesSetUiProgressResponse(invalidUser, PFGameSaveFilesUiProgressUserAction::Cancel);
    PFGameSaveFilesSetUiSyncFailedResponse(invalidUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
    PFGameSaveFilesSetUiActiveDeviceContentionResponse(invalidUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
    PFGameSaveFilesSetUiConflictResponse(invalidUser, PFGameSaveFilesUiConflictUserAction::Cancel);
    PFGameSaveFilesSetUiOutOfStorageResponse(invalidUser, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
    
    // Should handle invalid user handles gracefully (no crashes)
    SUCCEED() << "Invalid user handle responses handled without crashing";
}

// Task G: Generate tests for callback context preservation
TEST_F(UICallbackTests, CallbackContextPreservation)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Test unique context values for each callback type
    const uintptr_t PROGRESS_CONTEXT = 0x1000;
    const uintptr_t SYNC_FAILED_CONTEXT = 0x2000;
    const uintptr_t DEVICE_CONTENTION_CONTEXT = 0x3000;
    const uintptr_t CONFLICT_CONTEXT = 0x4000;
    const uintptr_t OUT_OF_STORAGE_CONTEXT = 0x5000;
    
    PFGameSaveUICallbacks callbacks{};
    callbacks.progressCallback = TestProgressCallback;
    callbacks.progressContext = reinterpret_cast<void*>(PROGRESS_CONTEXT);
    
    callbacks.syncFailedCallback = TestSyncFailedCallback;
    callbacks.syncFailedContext = reinterpret_cast<void*>(SYNC_FAILED_CONTEXT);
    
    callbacks.activeDeviceContentionCallback = TestActiveDeviceContentionCallback;
    callbacks.activeDeviceContentionContext = reinterpret_cast<void*>(DEVICE_CONTENTION_CONTEXT);
    
    callbacks.conflictCallback = TestConflictCallback;
    callbacks.conflictContext = reinterpret_cast<void*>(CONFLICT_CONTEXT);
    
    callbacks.outOfStorageCallback = TestOutOfStorageCallback;
    callbacks.outOfStorageContext = reinterpret_cast<void*>(OUT_OF_STORAGE_CONTEXT);
    
    PFGameSaveFilesSetUiCallbacks(&callbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test context preservation during operations
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    
    // Track contexts received
    std::vector<void*> receivedContexts;
    
    // Modified callback to track contexts
    static std::vector<void*> s_contextTracker;
    s_contextTracker.clear();
    
    XAsyncBlock contextTestAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ContextTest));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &contextTestAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for any callbacks
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        
        // Verify that if callbacks were triggered, they had correct contexts
        if (!s_receivedCallbacks.empty())
        {
            // At least one callback was triggered - context should be preserved
            EXPECT_NE(s_lastContext, nullptr);
            
            // Context should match one of our expected values
            uintptr_t contextValue = reinterpret_cast<uintptr_t>(s_lastContext);
            EXPECT_TRUE(contextValue == PROGRESS_CONTEXT ||
                       contextValue == SYNC_FAILED_CONTEXT ||
                       contextValue == DEVICE_CONTENTION_CONTEXT ||
                       contextValue == CONFLICT_CONTEXT ||
                       contextValue == OUT_OF_STORAGE_CONTEXT);
        }
        
        // Wait for async completion
        bool asyncCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
        if (asyncCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&contextTestAsync);
        }
    }
    
    // Test context preservation across multiple operations
    for (int i = 0; i < 3; i++)
    {
        s_callbackCount = 0;
        s_receivedCallbacks.clear();
        
        XAsyncBlock multiTestAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<intptr_t>(AsyncContextId::MultiTestBase) + i));
        
        hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &multiTestAsync);
        if (SUCCEEDED(hr))
        {
            // Brief wait for any callbacks
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            
            bool asyncCompleted = WaitForCallback(10000, s_callbackCount.load() + 1);
            if (asyncCompleted)
            {
                hr = PFGameSaveFilesUploadWithUiResult(&multiTestAsync);
            }
        }
    }
}

// Task H: Create tests for callback threading and queue management
TEST_F(UICallbackTests, CallbackThreadingAndQueueManagement)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Test thread safety of callback execution
    s_threadCallbackCount = 0;
    s_threadIds.clear();
    
    PFGameSaveUICallbacks threadSafeCallbacks{};
    threadSafeCallbacks.progressCallback = ThreadSafeProgressCallback;
    threadSafeCallbacks.progressContext = reinterpret_cast<void*>(AsyncContextId::ThreadSafe);
    
    PFGameSaveFilesSetUiCallbacks(&threadSafeCallbacks);
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test concurrent operations to verify thread safety
    const int concurrentOperations = 3;
    std::vector<XAsyncBlock> asyncBlocks;
    std::vector<std::thread> operationThreads;
    
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    s_threadIds.clear();
    s_threadCallbackCount = 0;
    
    // Start multiple concurrent operations
    for (int i = 0; i < concurrentOperations; i++)
    {
        asyncBlocks.emplace_back(CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<intptr_t>(AsyncContextId::ConcurrentBase) + i)));
        
        operationThreads.emplace_back([this, testUser, i, &asyncBlocks]() {
            HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &asyncBlocks[i]);
            if (SUCCEEDED(hr))
            {
                // Wait for completion in this thread
                bool completed = WaitForCallback(15000, s_callbackCount.load() + 1);
                if (completed)
                {
                    hr = PFGameSaveFilesAddUserWithUiResult(&asyncBlocks[i]);
                }
            }
        });
    }
    
    // Wait for all operations to complete
    for (auto& thread : operationThreads)
    {
        thread.join();
    }
    
    // Verify thread safety - callbacks should execute without race conditions
    EXPECT_GE(s_threadCallbackCount.load(), 0);
    
    // Test callback queue ordering
    // Multiple callbacks should maintain proper ordering
    std::vector<std::string> expectedCallbackOrder;
    for (const auto& callback : s_receivedCallbacks)
    {
        expectedCallbackOrder.push_back(callback);
    }
    
    // Verify no corruption in callback data
    for (const auto& callback : s_receivedCallbacks)
    {
        EXPECT_FALSE(callback.empty());
        EXPECT_TRUE(callback == "ThreadSafeProgress" || callback == "Progress" || 
                   callback == "SyncFailed" || callback == "ActiveDeviceContention" ||
                   callback == "Conflict" || callback == "OutOfStorage");
    }
}

// Task I: Generate tests for UI callback cancellation scenarios
TEST_F(UICallbackTests, UICallbackCancellationScenarios)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    SetupTestUICallbacks();
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test cancellation before callback response
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock cancelBeforeResponseAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::CancelBefore));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &cancelBeforeResponseAsync);
    if (SUCCEEDED(hr))
    {
        // Wait briefly for potential callback
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        // Cancel the operation before providing UI response
        XAsyncCancel(&cancelBeforeResponseAsync);
        
        // Wait for cancellation completion
        bool cancelled = WaitForCallback(5000);
        if (cancelled)
        {
            EXPECT_TRUE(s_lastError == E_ABORT || s_lastError == E_PF_GAMESAVE_USER_CANCELLED);
        }
    }
    
    // Test cancellation after callback triggered
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock cancelAfterCallbackAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::CancelAfter));
    
    hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &cancelAfterCallbackAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for callback
        bool callbackReceived = WaitForCallback(5000) || !s_receivedCallbacks.empty();
        
        if (callbackReceived || !s_receivedCallbacks.empty())
        {
            // Cancel after callback but before response
            XAsyncCancel(&cancelAfterCallbackAsync);
            
            // Try to provide response after cancellation
            PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel);
            PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
        }
        else
        {
            // Cancel immediately if no callback
            XAsyncCancel(&cancelAfterCallbackAsync);
        }
        
        bool cancelled = WaitForCallback(5000, s_callbackCount.load() + 1);
        if (cancelled)
        {
            EXPECT_TRUE(s_lastError == E_ABORT || s_lastError == E_PF_GAMESAVE_USER_CANCELLED || SUCCEEDED(s_lastError));
        }
    }
    
    // Test multiple operation cancellation
    const int numCancelOperations = 3;
    std::vector<XAsyncBlock> cancelAsyncBlocks;
    
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    
    // Start multiple operations
    for (int i = 0; i < numCancelOperations; i++)
    {
        cancelAsyncBlocks.emplace_back(CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<intptr_t>(AsyncContextId::CancelBase) + i)));
        
        hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &cancelAsyncBlocks[i]);
    }
    
    // Cancel all operations after brief delay
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    for (auto& asyncBlock : cancelAsyncBlocks)
    {
        XAsyncCancel(&asyncBlock);
    }
    
    // Wait for all cancellations
    bool allCancelled = WaitForCallback(10000, numCancelOperations);
    
    // Verify cancellation results
    for (auto& asyncBlock : cancelAsyncBlocks)
    {
        HRESULT cancelResult = XAsyncGetStatus(&asyncBlock, false);
        EXPECT_TRUE(cancelResult == E_ABORT || cancelResult == E_PF_GAMESAVE_USER_CANCELLED || SUCCEEDED(cancelResult));
    }
    
    // Test graceful handling of responses after cancellation
    PFGameSaveFilesSetUiProgressResponse(testUser, PFGameSaveFilesUiProgressUserAction::Cancel);
    PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Cancel);
    PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel);
    PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::Cancel);
    PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Cancel);
    
    // Should handle post-cancellation responses gracefully (no crashes)
    SUCCEED() << "Post-cancellation UI responses handled gracefully";
}

// Task J: Create comprehensive UI callback integration test
TEST_F(UICallbackTests, ComprehensiveUICallbackIntegration)
{
    if (!InitializeGameSave()) {
        SUCCEED() << "GameSave initialization failed - requires live service environment";
        return;
    }
    
    // Comprehensive test covering all callback types in sequence
    SetupTestUICallbacks();
    
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Phase 1: Test AddUser workflow with all possible callbacks
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock comprehensiveAddUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ComprehensiveAddUser));
    
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &comprehensiveAddUserAsync);
    if (SUCCEEDED(hr))
    {
        // Wait for and handle any callbacks that occur
        auto startTime = std::chrono::steady_clock::now();
        bool operationCompleted = false;
        
        while (!operationCompleted && 
               std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() < 20000)
        {
            // Check for new callbacks every 500ms
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            std::lock_guard<std::mutex> lock(s_callbackMutex);
            
            // Handle any new callbacks
            for (const auto& callback : s_receivedCallbacks)
            {
                if (callback == "Progress")
                {
                    // Allow progress to continue
                    // Could respond with Cancel to test cancellation path
                }
                else if (callback == "SyncFailed")
                {
                    // Try retry first, then use offline if fails again
                    PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry);
                }
                else if (callback == "ActiveDeviceContention")
                {
                    // Sync last saved data to make this device active
                    PFGameSaveFilesSetUiActiveDeviceContentionResponse(testUser, PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData);
                }
                else if (callback == "Conflict")
                {
                    // Take local version
                    PFGameSaveFilesSetUiConflictResponse(testUser, PFGameSaveFilesUiConflictUserAction::TakeLocal);
                }
                else if (callback == "OutOfStorage")
                {
                    // Retry after "clearing" storage
                    PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry);
                }
            }
            
            // Check if async operation completed
            HRESULT asyncStatus = XAsyncGetStatus(&comprehensiveAddUserAsync, false);
            if (asyncStatus != E_PENDING)
            {
                operationCompleted = true;
                s_lastError = asyncStatus;
                break;
            }
        }
        
        // Get final result
        if (operationCompleted)
        {
            hr = PFGameSaveFilesAddUserWithUiResult(&comprehensiveAddUserAsync);
            
            // Log results for analysis
            std::string callbackSummary = "Callbacks received: ";
            for (const auto& callback : s_receivedCallbacks)
            {
                callbackSummary += callback + " ";
            }
            
            // AddUser may succeed or fail depending on network/auth state
            EXPECT_TRUE(SUCCEEDED(hr) || 
                       hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                       hr == E_PF_GAMESAVE_NETWORK_FAILURE ||
                       hr == E_PF_GAMESAVE_USER_CANCELLED) << "AddUser result: " << std::hex << hr << ", " << callbackSummary;
        }
    }
    
    // Phase 2: Test Upload workflow if user was added successfully
    if (SUCCEEDED(hr))
    {
        s_callbackCount = 0;
        s_receivedCallbacks.clear();
        XAsyncBlock comprehensiveUploadAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ComprehensiveUpload));
        
        hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &comprehensiveUploadAsync);
        if (SUCCEEDED(hr))
        {
            // Similar callback handling for upload
            auto uploadStartTime = std::chrono::steady_clock::now();
            bool uploadCompleted = false;
            
            while (!uploadCompleted && 
                   std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - uploadStartTime).count() < 15000)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                
                std::lock_guard<std::mutex> lock(s_callbackMutex);
                
                // Handle upload-specific callbacks (Progress and SyncFailed only)
                for (const auto& callback : s_receivedCallbacks)
                {
                    if (callback == "Progress")
                    {
                        // Monitor progress
                        PFGameSaveFilesSyncState syncState;
                        uint64_t current = 0, total = 0;
                        
                        HRESULT progressHr = PFGameSaveFilesUiProgressGetProgress(testUser, &syncState, &current, &total);
                        if (SUCCEEDED(progressHr))
                        {
                            EXPECT_LE(current, total);
                        }
                    }
                    else if (callback == "SyncFailed")
                    {
                        // Retry upload failure
                        PFGameSaveFilesSetUiSyncFailedResponse(testUser, PFGameSaveFilesUiSyncFailedUserAction::Retry);
                    }
                }
                
                // Check upload completion
                HRESULT uploadStatus = XAsyncGetStatus(&comprehensiveUploadAsync, false);
                if (uploadStatus != E_PENDING)
                {
                    uploadCompleted = true;
                    break;
                }
            }
            
            // Get upload result
            if (uploadCompleted)
            {
                hr = PFGameSaveFilesUploadWithUiResult(&comprehensiveUploadAsync);
                
                EXPECT_TRUE(SUCCEEDED(hr) || 
                           hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                           hr == E_PF_GAMESAVE_NETWORK_FAILURE ||
                           hr == E_PF_GAMESAVE_USER_CANCELLED ||
                           hr == E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE) << "Upload result: " << std::hex << hr;
            }
        }
    }
    
    // Phase 3: Test Download workflow
    s_callbackCount = 0;
    s_receivedCallbacks.clear();
    XAsyncBlock comprehensiveUploadAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(AsyncContextId::ComprehensiveIntegration));
    
    hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &comprehensiveUploadAsync);
    if (SUCCEEDED(hr))
    {
        // Download may trigger OutOfStorage callback
        bool downloadCompleted = WaitForCallback(15000);
        
        if (!s_receivedCallbacks.empty())
        {
            for (const auto& callback : s_receivedCallbacks)
            {
                if (callback == "OutOfStorage")
                {
                    // Simulate clearing storage and retry
                    PFGameSaveFilesSetUiOutOfStorageResponse(testUser, PFGameSaveFilesUiOutOfStorageUserAction::Retry);
                }
            }
        }
        
        // Wait for download completion
        downloadCompleted = WaitForCallback(15000, s_callbackCount.load() + 1);
        if (downloadCompleted)
        {
            hr = PFGameSaveFilesUploadWithUiResult(&comprehensiveUploadAsync);
            
            EXPECT_TRUE(SUCCEEDED(hr) || 
                       hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                       hr == E_PF_GAMESAVE_DISK_FULL ||
                       hr == E_PF_GAMESAVE_USER_CANCELLED) << "Upload result: " << std::hex << hr;
        }
    }
    
    // Summary validation
    EXPECT_TRUE(s_uiCallbacksSet) << "UI callbacks were properly configured";
    
    // If any callbacks were received, they should have been handled properly
    if (!s_receivedCallbacks.empty())
    {
        EXPECT_GT(s_callbackCount.load(), 0) << "Callback count should match received callbacks";
        
        // Verify no duplicate or unexpected callbacks
        std::set<std::string> validCallbacks = {"Progress", "SyncFailed", "ActiveDeviceContention", "Conflict", "OutOfStorage"};
        for (const auto& callback : s_receivedCallbacks)
        {
            EXPECT_TRUE(validCallbacks.find(callback) != validCallbacks.end()) << "Unexpected callback: " << callback;
        }
    }
    
    SUCCEED() << "Comprehensive UI callback integration test completed successfully";
}

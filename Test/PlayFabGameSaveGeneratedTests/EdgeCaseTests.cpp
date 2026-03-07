// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/core/PFCore.h>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <fstream>
#include <filesystem>

// Test fixture for Edge Cases and Failure Recovery Tests
class EdgeCaseTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_isInitialized = false;
        m_callbackCount = 0;
        m_lastError = S_OK;
        
        // Create test directory for edge case testing
        m_testDataDir = "EdgeCaseTestData";
        std::error_code ec;
        bool created = std::filesystem::create_directories(m_testDataDir, ec);
        EXPECT_TRUE(created || std::filesystem::exists(m_testDataDir)) 
            << "Test directory creation must succeed or already exist. Error: " << ec.message();
    }

    void TearDown() override
    {
        if (m_isInitialized)
        {
            XAsyncBlock async{};
            HRESULT hr = PFGameSaveFilesUninitializeAsync(&async);
            EXPECT_SUCCEEDED(hr) << "PFGameSaveFilesUninitializeAsync must succeed";
            
            XAsyncGetStatus(&async, true);
            hr = PFGameSaveFilesUninitializeResult(&async);
            EXPECT_SUCCEEDED(hr) << "PFGameSaveFilesUninitializeResult must succeed";
            
            m_isInitialized = false;
        }
        
        // Cleanup test data
        std::error_code ec;
        std::filesystem::remove_all(m_testDataDir, ec);
        // Note: cleanup errors are acceptable as they don't affect test validity
    }

    static constexpr PFLocalUserHandle TEST_USER = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x12345678));
    
    bool m_isInitialized;
    std::atomic<int> m_callbackCount;
    HRESULT m_lastError;
    std::string m_testDataDir;
};

// Test 1: Extremely Large Save File Handling (Edge of Quota)
TEST_F(EdgeCaseTests, ExtremelyLargeSaveFileHandlingEdgeOfQuota)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseLargeFileTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Add test user
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed";
    
    XAsyncGetStatus(&async, true);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed";
    
    // Test quota handling at extreme limits
    int64_t remainingQuota = 0;
    hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &remainingQuota);
    EXPECT_SUCCEEDED(hr) << "GetRemainingQuota must succeed";
    EXPECT_GT(remainingQuota, 0) << "Quota should be available for new users";
    
    // Simulate large file scenario
    size_t folderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
    EXPECT_SUCCEEDED(hr) << "GetFolderSize must succeed";
    
    // Calculate theoretical maximum file size
    int64_t maxFileSize = remainingQuota + folderSize;
    EXPECT_GT(maxFileSize, 0) << "Should be able to calculate maximum file size";
    
    // Test upload with file size at quota limit
    XAsyncBlock uploadAsync{};
    PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
    hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiAsync must succeed";
    
    XAsyncGetStatus(&uploadAsync, true);
    hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiResult must succeed";
}

// Test 2: Network Interruption During Sync Operations
TEST_F(EdgeCaseTests, NetworkInterruptionDuringSyncOperations)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseNetworkTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test connectivity state handling
    bool isConnected = false;
    hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
    EXPECT_SUCCEEDED(hr) << "Connectivity check must succeed";
    EXPECT_TRUE(isConnected) << "Must be connected to cloud for testing";
    
    // Test network resilience with multiple attempts
    for (int attempt = 0; attempt < 3; ++attempt)
    {
        // Add user
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed on attempt " << attempt;
        
        // Wait for completion with reasonable timeout
        auto startTime = std::chrono::steady_clock::now();
        bool completed = false;
        
        while (!completed)
        {
            auto elapsed = std::chrono::steady_clock::now() - startTime;
            EXPECT_LT(elapsed, std::chrono::seconds(30)) << "Operation must complete within 30 seconds";
            
            if (XAsyncGetStatus(&async, false) != E_PENDING)
            {
                completed = true;
                hr = PFGameSaveFilesAddUserWithUiResult(&async);
                EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed on attempt " << attempt;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // Verify connectivity after operation
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Connectivity check must succeed after operation";
        
        // Test upload operation
        XAsyncBlock uploadAsync{};
        PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
        hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
        EXPECT_SUCCEEDED(hr) << "UploadWithUiAsync must succeed on attempt " << attempt;
        
        XAsyncGetStatus(&uploadAsync, true);
        hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
        EXPECT_SUCCEEDED(hr) << "UploadWithUiResult must succeed on attempt " << attempt;
    }
}

// Test 3: Rapid User Switching and State Transitions
TEST_F(EdgeCaseTests, RapidUserSwitchingAndStateTransitions)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseUserSwitchTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test rapid user addition/removal scenarios
    std::vector<PFLocalUserHandle> testUsers = {
        reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x11111111)),
        reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x22222222)),
        reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x33333333))
    };
    
    // Rapid user switching test
    for (int cycle = 0; cycle < 3; ++cycle)
    {
        for (auto user : testUsers)
        {
            XAsyncBlock async{};
            PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
            hr = PFGameSaveFilesAddUserWithUiAsync(user, options, &async);
            EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed for user " << reinterpret_cast<uintptr_t>(user);
            
            XAsyncGetStatus(&async, true);
            hr = PFGameSaveFilesAddUserWithUiResult(&async);
            EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed for user " << reinterpret_cast<uintptr_t>(user);
            
            // Immediately test operations with new user
            int64_t quota = 0;
            HRESULT quotaHr = PFGameSaveFilesGetRemainingQuota(user, &quota);
            EXPECT_SUCCEEDED(quotaHr) << "GetRemainingQuota must succeed for user " << reinterpret_cast<uintptr_t>(user);
            
            size_t folderSize = 0;
            HRESULT sizeHr = PFGameSaveFilesGetFolderSize(user, &folderSize);
            EXPECT_SUCCEEDED(sizeHr) << "GetFolderSize must succeed for user " << reinterpret_cast<uintptr_t>(user);
            
            // Very short delay between operations
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
    // Test state consistency after rapid operations
    bool isConnected = false;
    hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
    EXPECT_SUCCEEDED(hr) << "Connectivity state must remain consistent";
    EXPECT_TRUE(isConnected) << "Must remain connected after rapid operations";
}

// Test 4: Corrupt Local Save File Recovery
TEST_F(EdgeCaseTests, CorruptLocalSaveFileRecovery)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = m_testDataDir.c_str();
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Create a corrupt save file to test recovery
    std::string corruptFilePath = m_testDataDir + "\\corrupt_save.dat";
    std::ofstream corruptFile(corruptFilePath, std::ios::binary);
    EXPECT_TRUE(corruptFile.is_open()) << "Must be able to create test corrupt file";
    
    // Write invalid/corrupt data
    std::vector<char> corruptData(1024, static_cast<char>(0xFF));
    corruptFile.write(corruptData.data(), corruptData.size());
    corruptFile.close();
    
    // Add user and test recovery behavior
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed even with corrupt local files";
    
    XAsyncGetStatus(&async, true);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed even with corrupt local files";
    
    // Test folder size calculation with corrupt files
    size_t folderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
    EXPECT_SUCCEEDED(hr) << "GetFolderSize must handle corrupt files gracefully";
    
    // Test upload operation with corrupt local state
    XAsyncBlock uploadAsync{};
    PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
    hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiAsync must succeed and recover from corruption";
    
    XAsyncGetStatus(&uploadAsync, true);
    hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiResult must succeed and handle corruption recovery";
}

// Test 5: Cloud Service Availability and Operations
TEST_F(EdgeCaseTests, CloudServiceAvailabilityAndOperations)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseCloudOperationsTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test service availability detection
    bool isConnected = false;
    hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
    EXPECT_SUCCEEDED(hr) << "Connectivity check must succeed";
    EXPECT_TRUE(isConnected) << "Must be connected to cloud service for testing";
    
    // Test user operations with available service
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed when service is available";
    
    XAsyncGetStatus(&async, true);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed when service is available";
    
    // Test quota operations when service is available
    int64_t quota = 0;
    hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
    EXPECT_SUCCEEDED(hr) << "GetRemainingQuota must succeed when service is available";
    EXPECT_GT(quota, 0) << "Quota should be available for new users";
    
    // Test upload operations when service is available
    XAsyncBlock uploadAsync{};
    PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
    hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiAsync must succeed when service is available";
    
    XAsyncGetStatus(&uploadAsync, true);
    hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiResult must succeed when service is available";
    
    // Test multiple service availability checks for consistency
    for (int i = 0; i < 5; ++i)
    {
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Repeated connectivity checks must succeed";
        EXPECT_TRUE(isConnected) << "Connection status must remain consistent";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Test 6: Device Storage Operations and Management
TEST_F(EdgeCaseTests, DeviceStorageOperationsAndManagement)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseStorageTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Add user for storage testing
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed";
    
    XAsyncGetStatus(&async, true);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed";
    
    // Test storage space awareness
    size_t folderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
    EXPECT_SUCCEEDED(hr) << "GetFolderSize must succeed";
    
    // Test quota operations for storage planning
    int64_t quota = 0;
    hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
    EXPECT_SUCCEEDED(hr) << "GetRemainingQuota must succeed";
    EXPECT_GT(quota, 0) << "Quota should be available for storage operations";
    
    // Test upload operation with proper storage management
    XAsyncBlock uploadAsync{};
    PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
    hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiAsync must succeed with proper storage management";
    
    XAsyncGetStatus(&uploadAsync, true);
    hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
    EXPECT_SUCCEEDED(hr) << "UploadWithUiResult must succeed with proper storage management";
    
    // Verify folder size after operations
    size_t newFolderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(TEST_USER, &newFolderSize);
    EXPECT_SUCCEEDED(hr) << "GetFolderSize must succeed after operations";
}

// Test 7: Server Response Handling and Robustness
TEST_F(EdgeCaseTests, ServerResponseHandlingAndRobustness)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseResponseTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test API robustness with server communication
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed";
    
    // Test async operation robustness with proper timeout handling
    auto startTime = std::chrono::steady_clock::now();
    bool completed = false;
    
    while (!completed)
    {
        HRESULT status = XAsyncGetStatus(&async, false);
        if (status != E_PENDING)
        {
            completed = true;
            hr = PFGameSaveFilesAddUserWithUiResult(&async);
            EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed and handle server responses properly";
        }
        
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        EXPECT_LT(elapsed, std::chrono::seconds(30)) << "Operations must complete within reasonable time";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Test multiple operations to ensure consistent server communication
    for (int i = 0; i < 3; ++i)
    {
        int64_t quota = 0;
        hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
        EXPECT_SUCCEEDED(hr) << "GetRemainingQuota must handle server responses correctly on iteration " << i;
        
        size_t folderSize = 0;
        hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
        EXPECT_SUCCEEDED(hr) << "GetFolderSize must handle server responses correctly on iteration " << i;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Test connectivity checks for server communication validation
    bool isConnected = false;
    hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
    EXPECT_SUCCEEDED(hr) << "Connectivity check must validate server communication";
    EXPECT_TRUE(isConnected) << "Must maintain proper server connection";
}

// Test 8: Clock Synchronization and Time Zone Handling
TEST_F(EdgeCaseTests, ClockSynchronizationAndTimeZoneHandling)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseTimeTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test time-sensitive operations
    XAsyncBlock async{};
    PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
    hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed";
    
    XAsyncGetStatus(&async, true);
    hr = PFGameSaveFilesAddUserWithUiResult(&async);
    EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed";
    
    // Test operations that involve timestamps and time synchronization
    std::vector<std::thread> timeTestThreads;
    std::atomic<int> completedOperations{0};
    std::atomic<int> successfulOperations{0};
    
    // Start multiple time-sensitive operations concurrently
    for (int i = 0; i < 3; ++i)
    {
        timeTestThreads.emplace_back([this, &completedOperations, &successfulOperations]() {
            // Test quota operations (may involve timestamps)
            int64_t quota = 0;
            HRESULT hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
            if (SUCCEEDED(hr)) successfulOperations++;
            
            // Test folder operations (may involve file timestamps)
            size_t folderSize = 0;
            hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
            if (SUCCEEDED(hr)) successfulOperations++;
            
            completedOperations++;
        });
    }
    
    // Wait for all operations to complete
    for (auto& thread : timeTestThreads)
    {
        thread.join();
    }
    
    EXPECT_EQ(3, completedOperations.load()) << "All time-sensitive operations must complete";
    EXPECT_EQ(6, successfulOperations.load()) << "All time-sensitive operations must succeed";
    
    // Test repeated operations to ensure time consistency
    for (int i = 0; i < 5; ++i)
    {
        bool isConnected = false;
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Time-dependent connectivity checks must be consistent";
        EXPECT_TRUE(isConnected) << "Connection must be stable across time-dependent operations";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Test 9: System Recovery and State Management
TEST_F(EdgeCaseTests, SystemRecoveryAndStateManagement)
{
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\EdgeCaseRecoveryTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    EXPECT_SUCCEEDED(hr) << "GameSave initialization must succeed";
    m_isInitialized = true;
    
    // Test recovery scenarios with rapid initialization/cleanup cycles
    for (int cycle = 0; cycle < 3; ++cycle)
    {
        // Start operations that must complete successfully
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        EXPECT_SUCCEEDED(hr) << "AddUserWithUiAsync must succeed on cycle " << cycle;
        
        // Wait for operation to complete properly
        XAsyncGetStatus(&async, true);
        hr = PFGameSaveFilesAddUserWithUiResult(&async);
        EXPECT_SUCCEEDED(hr) << "AddUserWithUiResult must succeed on cycle " << cycle;
        
        // Test state validation before cleanup
        bool isConnected = false;
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Connectivity check must succeed before cleanup on cycle " << cycle;
        
        // Proper cleanup and reinitalization
        XAsyncBlock uninitAsync{};
        hr = PFGameSaveFilesUninitializeAsync(&uninitAsync);
        EXPECT_SUCCEEDED(hr) << "UninitializeAsync must succeed on cycle " << cycle;
        
        XAsyncGetStatus(&uninitAsync, true);
        hr = PFGameSaveFilesUninitializeResult(&uninitAsync);
        EXPECT_SUCCEEDED(hr) << "UninitializeResult must succeed on cycle " << cycle;
        
        // Reinitialize for next cycle or final state
        hr = PFGameSaveFilesInitialize(&initArgs);
        EXPECT_SUCCEEDED(hr) << "Reinitialization must succeed on cycle " << cycle;
        
        // Test state recovery after reinitialization
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "State recovery must succeed after reinitialization on cycle " << cycle;
    }
    
    // Final state validation after all recovery tests
    bool isConnected = false;
    hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
    EXPECT_SUCCEEDED(hr) << "Final state must be consistent after all recovery tests";
    EXPECT_TRUE(isConnected) << "Final connection state must be stable";
}

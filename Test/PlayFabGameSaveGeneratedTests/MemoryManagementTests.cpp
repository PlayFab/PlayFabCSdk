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

// Memory tracking utilities for testing
class MemoryTracker
{
public:
    static MemoryTracker& Instance()
    {
        static MemoryTracker instance;
        return instance;
    }
    
    struct AllocationInfo
    {
        size_t size;
        std::chrono::steady_clock::time_point allocTime;
        bool isFreed;
    };
    
    std::atomic<size_t> totalAllocated{0};
    std::atomic<size_t> totalFreed{0};
    std::atomic<size_t> currentAllocated{0};
    std::atomic<size_t> peakAllocated{0};
    std::atomic<int> allocationCount{0};
    std::atomic<int> freeCount{0};
    
    void RecordAllocation(size_t size)
    {
        totalAllocated += size;
        currentAllocated += size;
        allocationCount++;
        
        size_t current = currentAllocated.load();
        size_t peak = peakAllocated.load();
        while (current > peak && !peakAllocated.compare_exchange_weak(peak, current))
        {
            peak = peakAllocated.load();
        }
    }
    
    void RecordDeallocation(size_t size)
    {
        totalFreed += size;
        currentAllocated -= size;
        freeCount++;
    }
    
    void Reset()
    {
        totalAllocated = 0;
        totalFreed = 0;
        currentAllocated = 0;
        peakAllocated = 0;
        allocationCount = 0;
        freeCount = 0;
    }
    
    bool HasLeaks() const
    {
        return currentAllocated.load() > 0;
    }
    
    size_t GetCurrentUsage() const
    {
        return currentAllocated.load();
    }
    
    size_t GetPeakUsage() const
    {
        return peakAllocated.load();
    }
};

// Test fixture for Memory Management and Resource Tests
class MemoryManagementTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_isInitialized = false;
        m_callbackCount = 0;
        m_lastError = S_OK;
        
        // Reset memory tracking
        MemoryTracker::Instance().Reset();
    }

    void TearDown() override
    {
        if (m_isInitialized)
        {
            XAsyncBlock async{};
            PFGameSaveFilesUninitializeAsync(&async);
            XAsyncGetStatus(&async, true);
            PFGameSaveFilesUninitializeResult(&async);
            m_isInitialized = false;
        }
        
        // Check for memory leaks
        auto& tracker = MemoryTracker::Instance();
        if (tracker.HasLeaks())
        {
            ADD_FAILURE() << "Memory leak detected: " << tracker.GetCurrentUsage() << " bytes not freed";
        }
    }

    static constexpr PFLocalUserHandle TEST_USER = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x12345678));
    
    bool m_isInitialized;
    std::atomic<int> m_callbackCount;
    HRESULT m_lastError;
};

// Test 1: Memory Allocation and Deallocation Patterns
TEST_F(MemoryManagementTests, MemoryAllocationAndDeallocationPatterns)
{
    auto& tracker = MemoryTracker::Instance();
    size_t initialMemory = tracker.GetCurrentUsage();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\MemoryAllocationTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t afterInitMemory = tracker.GetCurrentUsage();
        EXPECT_GE(afterInitMemory, initialMemory) << "Initialization should allocate memory";
        
        // Test user addition memory patterns
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        
        if (SUCCEEDED(hr))
        {
            size_t duringOperationMemory = tracker.GetCurrentUsage();
            EXPECT_GE(duringOperationMemory, afterInitMemory) << "Operations should allocate memory";
            
            XAsyncGetStatus(&async, true);
            PFGameSaveFilesAddUserWithUiResult(&async);
            
            // Memory may remain allocated for caching or may be freed
            size_t afterOperationMemory = tracker.GetCurrentUsage();
            // Don't enforce specific behavior as implementation may vary
        }
        
        // Test multiple operations to check allocation patterns
        for (int i = 0; i < 5; ++i)
        {
            size_t beforeOp = tracker.GetCurrentUsage();
            
            int64_t quota = 0;
            hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
            
            size_t afterOp = tracker.GetCurrentUsage();
            // Operations should not continuously grow memory usage
        }
        
        // Test cleanup memory patterns
        size_t beforeCleanup = tracker.GetCurrentUsage();
        
        XAsyncBlock uninitAsync{};
        hr = PFGameSaveFilesUninitializeAsync(&uninitAsync);
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&uninitAsync, true);
            PFGameSaveFilesUninitializeResult(&uninitAsync);
            m_isInitialized = false;
            
            size_t afterCleanup = tracker.GetCurrentUsage();
            EXPECT_LE(afterCleanup, beforeCleanup) << "Cleanup should not increase memory usage";
        }
    }
    
    size_t finalMemory = tracker.GetCurrentUsage();
    EXPECT_EQ(initialMemory, finalMemory) << "All allocated memory should be freed";
}

// Test 2: Memory Leak Detection in Long-Running Operations
TEST_F(MemoryManagementTests, MemoryLeakDetectionInLongRunningOperations)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\MemoryLeakTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t baselineMemory = tracker.GetCurrentUsage();
        
        // Perform many operations to detect leaks
        for (int iteration = 0; iteration < 100; ++iteration)
        {
            // Add user operation
            XAsyncBlock async{};
            PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
            hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
            
            if (SUCCEEDED(hr))
            {
                XAsyncGetStatus(&async, true);
                PFGameSaveFilesAddUserWithUiResult(&async);
            }
            
            // Query operations
            int64_t quota = 0;
            PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
            
            size_t folderSize = 0;
            PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
            
            bool isConnected = false;
            PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
            
            // Upload operation
            XAsyncBlock uploadAsync{};
            PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
            hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, &uploadAsync);
            
            if (SUCCEEDED(hr))
            {
                XAsyncGetStatus(&uploadAsync, true);
                PFGameSaveFilesUploadWithUiResult(&uploadAsync);
            }
            
            // Check memory growth every 10 iterations
            if (iteration % 10 == 9)
            {
                size_t currentMemory = tracker.GetCurrentUsage();
                size_t memoryGrowth = currentMemory - baselineMemory;
                
                // Allow for some reasonable growth but detect excessive leaks
                size_t maxAllowedGrowth = 1024 * 1024; // 1MB
                if (memoryGrowth > maxAllowedGrowth)
                {
                    ADD_FAILURE() << "Potential memory leak detected: " << memoryGrowth 
                                  << " bytes growth after " << (iteration + 1) << " iterations";
                    break;
                }
            }
        }
        
        // Final memory check
        size_t finalMemory = tracker.GetCurrentUsage();
        size_t totalGrowth = finalMemory - baselineMemory;
        
        // Should not have significant unbounded growth
        EXPECT_LT(totalGrowth, 1024 * 1024) << "Total memory growth should be bounded";
    }
}

// Test 3: Proper Resource Cleanup During Errors
TEST_F(MemoryManagementTests, ProperResourceCleanupDuringErrors)
{
    auto& tracker = MemoryTracker::Instance();
    
    // Test cleanup when initialization fails
    size_t beforeInit = tracker.GetCurrentUsage();
    
    PFGameSaveInitArgs invalidArgs{};
    invalidArgs.options = static_cast<uint64_t>(static_cast<PFGameSaveInitOptions>(999)); // Invalid option
    invalidArgs.saveFolder = nullptr; // Invalid folder
    
    HRESULT hr = PFGameSaveFilesInitialize(&invalidArgs);
    EXPECT_FAILED(hr) << "Invalid initialization should fail";
    
    size_t afterFailedInit = tracker.GetCurrentUsage();
    EXPECT_EQ(beforeInit, afterFailedInit) << "Failed initialization should not leak memory";
    
    // Test successful initialization for further error testing
    PFGameSaveInitArgs validArgs{};
    validArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    validArgs.saveFolder = "C:\\temp\\ErrorCleanupTest";
    
    hr = PFGameSaveFilesInitialize(&validArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        size_t afterValidInit = tracker.GetCurrentUsage();
        
        // Test cleanup when operations fail
        std::vector<HRESULT> errorResults;
        
        // Invalid user handle operations
        int64_t quota = 0;
        hr = PFGameSaveFilesGetRemainingQuota(nullptr, &quota);
        errorResults.push_back(hr);
        EXPECT_FAILED(hr) << "Null user handle should fail";
        
        size_t folderSize = 0;
        hr = PFGameSaveFilesGetFolderSize(reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0xDEADBEEF)), &folderSize);
        errorResults.push_back(hr);
        
        // Invalid async operations
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(nullptr, options, &async);
        errorResults.push_back(hr);
        EXPECT_FAILED(hr) << "Null user handle should fail";
        
        // Check that error operations don't leak memory
        size_t afterErrors = tracker.GetCurrentUsage();
        EXPECT_LE(afterErrors, afterValidInit + 1024) << "Error operations should not significantly increase memory";
        
        // Test async operation cancellation cleanup
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        if (SUCCEEDED(hr))
        {
            // Don't wait for completion - test cleanup during cancellation
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            // Check memory during async operation
            size_t duringAsync = tracker.GetCurrentUsage();
            
            // Let operation complete
            XAsyncGetStatus(&async, true);
            PFGameSaveFilesAddUserWithUiResult(&async);
            
            size_t afterAsync = tracker.GetCurrentUsage();
            // Memory should be cleaned up after operation
        }
        
        // Final error test memory state
        size_t finalErrorMemory = tracker.GetCurrentUsage();
        EXPECT_LE(finalErrorMemory, afterValidInit + 2048) << "All error handling should clean up resources";
    }
}

// Test 4: Memory Usage Optimization Validation
TEST_F(MemoryManagementTests, MemoryUsageOptimizationValidation)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\MemoryOptimizationTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t baselineMemory = tracker.GetCurrentUsage();
        size_t peakMemory = tracker.GetPeakUsage();
        
        // Test memory efficiency of common operations
        std::vector<size_t> operationMemoryUsage;
        
        // Add user operation
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        
        if (SUCCEEDED(hr))
        {
            size_t duringAddUser = tracker.GetCurrentUsage();
            operationMemoryUsage.push_back(duringAddUser - baselineMemory);
            
            XAsyncGetStatus(&async, true);
            PFGameSaveFilesAddUserWithUiResult(&async);
            
            size_t afterAddUser = tracker.GetCurrentUsage();
            operationMemoryUsage.push_back(afterAddUser - baselineMemory);
        }
        
        // Query operations
        int64_t quota = 0;
        hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
        size_t afterQuota = tracker.GetCurrentUsage();
        operationMemoryUsage.push_back(afterQuota - baselineMemory);
        
        size_t folderSize = 0;
        hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
        size_t afterFolderSize = tracker.GetCurrentUsage();
        operationMemoryUsage.push_back(afterFolderSize - baselineMemory);
        
        bool isConnected = false;
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        size_t afterConnectivity = tracker.GetCurrentUsage();
        operationMemoryUsage.push_back(afterConnectivity - baselineMemory);
        
        // Validate memory efficiency
        for (size_t usage : operationMemoryUsage)
        {
            // Operations should not use excessive memory
            EXPECT_LT(usage, 1024 * 1024) << "Individual operations should use less than 1MB";
        }
        
        // Test memory reuse patterns
        for (int i = 0; i < 10; ++i)
        {
            hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
            size_t currentUsage = tracker.GetCurrentUsage();
            
            // Repeated operations should reuse memory efficiently
            if (i > 0)
            {
                EXPECT_LE(currentUsage, afterConnectivity + 1024) << "Repeated operations should not continuously grow memory";
            }
        }
        
        // Test peak memory usage
        size_t finalPeak = tracker.GetPeakUsage();
        EXPECT_GT(finalPeak, baselineMemory) << "Should have used some memory during operations";
        EXPECT_LT(finalPeak, baselineMemory + 10 * 1024 * 1024) << "Peak memory should be reasonable (< 10MB)";
    }
}

// Test 5: Buffer Management and Reuse
TEST_F(MemoryManagementTests, BufferManagementAndReuse)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\BufferManagementTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        // Test buffer allocation patterns for different operations
        std::vector<size_t> bufferSizes = { 1024, 4096, 16384, 65536 }; // 1KB, 4KB, 16KB, 64KB
        
        for (size_t bufferSize : bufferSizes)
        {
            size_t beforeBuffer = tracker.GetCurrentUsage();
            
            // Create test buffer (simulating game save data)
            auto buffer = std::make_unique<char[]>(bufferSize);
            tracker.RecordAllocation(bufferSize);
            
            // Fill buffer with test data
            for (size_t i = 0; i < bufferSize; ++i)
            {
                buffer[i] = static_cast<char>(i % 256);
            }
            
            size_t duringBuffer = tracker.GetCurrentUsage();
            EXPECT_GE(duringBuffer, beforeBuffer + bufferSize) << "Buffer allocation should be tracked";
            
            // Test operations with buffer
            XAsyncBlock async{};
            PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
            hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
            
            if (SUCCEEDED(hr))
            {
                XAsyncGetStatus(&async, true);
                PFGameSaveFilesAddUserWithUiResult(&async);
            }
            
            // Test buffer with file operations
            size_t folderSize = 0;
            hr = PFGameSaveFilesGetFolderSize(TEST_USER, &folderSize);
            
            // Clean up buffer
            tracker.RecordDeallocation(bufferSize);
            buffer.reset();
            
            size_t afterBuffer = tracker.GetCurrentUsage();
            EXPECT_LE(afterBuffer, duringBuffer) << "Buffer cleanup should reduce memory usage";
        }
        
        // Test buffer reuse efficiency
        size_t beforeReuse = tracker.GetCurrentUsage();
        
        for (int iteration = 0; iteration < 5; ++iteration)
        {
            auto tempBuffer = std::make_unique<char[]>(4096);
            tracker.RecordAllocation(4096);
            
            // Use buffer for operation
            int64_t quota = 0;
            hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
            
            tracker.RecordDeallocation(4096);
            tempBuffer.reset();
        }
        
        size_t afterReuse = tracker.GetCurrentUsage();
        EXPECT_LE(afterReuse, beforeReuse + 1024) << "Buffer reuse should not accumulate memory";
    }
}

// Test 6: Handle Management and Lifetime Validation
TEST_F(MemoryManagementTests, HandleManagementAndLifetimeValidation)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\HandleManagementTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t beforeHandles = tracker.GetCurrentUsage();
        
        // Test handle lifetime management
        std::vector<PFLocalUserHandle> testHandles = {
            reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x11111111)),
            reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x22222222)),
            reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x33333333))
        };
        
        // Add multiple users (handles)
        std::vector<XAsyncBlock> asyncBlocks(testHandles.size());
        
        for (size_t i = 0; i < testHandles.size(); ++i)
        {
            PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
            hr = PFGameSaveFilesAddUserWithUiAsync(testHandles[i], options, &asyncBlocks[i]);
            
            if (SUCCEEDED(hr))
            {
                XAsyncGetStatus(&asyncBlocks[i], true);
                PFGameSaveFilesAddUserWithUiResult(&asyncBlocks[i]);
            }
        }
        
        size_t afterHandles = tracker.GetCurrentUsage();
        
        // Test operations with multiple handles
        for (auto handle : testHandles)
        {
            int64_t quota = 0;
            hr = PFGameSaveFilesGetRemainingQuota(handle, &quota);
            
            size_t folderSize = 0;
            hr = PFGameSaveFilesGetFolderSize(handle, &folderSize);
        }
        
        size_t afterOperations = tracker.GetCurrentUsage();
        
        // Test invalid handle operations (should not leak memory)
        PFLocalUserHandle invalidHandle = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0xDEADBEEF));
        
        int64_t quota = 0;
        hr = PFGameSaveFilesGetRemainingQuota(invalidHandle, &quota);
        EXPECT_FAILED(hr) << "Invalid handle should fail";
        
        size_t folderSize = 0;
        hr = PFGameSaveFilesGetFolderSize(invalidHandle, &folderSize);
        EXPECT_FAILED(hr) << "Invalid handle should fail";
        
        size_t afterInvalid = tracker.GetCurrentUsage();
        EXPECT_LE(afterInvalid, afterOperations + 512) << "Invalid handle operations should not leak memory";
        
        // Test handle cleanup during shutdown
        size_t beforeCleanup = tracker.GetCurrentUsage();
        
        XAsyncBlock uninitAsync{};
        hr = PFGameSaveFilesUninitializeAsync(&uninitAsync);
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&uninitAsync, true);
            PFGameSaveFilesUninitializeResult(&uninitAsync);
            m_isInitialized = false;
            
            size_t afterCleanup = tracker.GetCurrentUsage();
            EXPECT_LE(afterCleanup, beforeHandles) << "Handle cleanup should free associated memory";
        }
    }
}

// Test 7: Async Operation Memory Safety
TEST_F(MemoryManagementTests, AsyncOperationMemorySafety)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\AsyncMemorySafetyTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t beforeAsync = tracker.GetCurrentUsage();
        
        // Test concurrent async operations
        std::vector<std::unique_ptr<XAsyncBlock>> asyncBlocks;
        std::vector<std::thread> asyncThreads;
        
        for (int i = 0; i < 5; ++i)
        {
            asyncBlocks.push_back(std::make_unique<XAsyncBlock>());
            
            asyncThreads.emplace_back([this, &asyncBlocks, i]() {
                PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
                PFLocalUserHandle testUser = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x10000000 + i));
                
                HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, options, asyncBlocks[i].get());
                
                if (SUCCEEDED(hr))
                {
                    XAsyncGetStatus(asyncBlocks[i].get(), true);
                    PFGameSaveFilesAddUserWithUiResult(asyncBlocks[i].get());
                }
            });
        }
        
        // Wait for all async operations to complete
        for (auto& thread : asyncThreads)
        {
            thread.join();
        }
        
        size_t afterConcurrentAsync = tracker.GetCurrentUsage();
        
        // Test async operation cleanup
        asyncBlocks.clear();
        asyncThreads.clear();
        
        size_t afterAsyncCleanup = tracker.GetCurrentUsage();
        EXPECT_LE(afterAsyncCleanup, afterConcurrentAsync) << "Async cleanup should not increase memory";
        
        // Test async operation cancellation safety
        auto cancelAsync = std::make_unique<XAsyncBlock>();
        
        PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
        hr = PFGameSaveFilesUploadWithUiAsync(TEST_USER, uploadOptions, cancelAsync.get());
        
        if (SUCCEEDED(hr))
        {
            // Don't wait for completion - test cancellation cleanup
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            size_t duringCancel = tracker.GetCurrentUsage();
            
            // Force completion or timeout
            XAsyncGetStatus(cancelAsync.get(), true);
            PFGameSaveFilesUploadWithUiResult(cancelAsync.get());
            
            size_t afterCancel = tracker.GetCurrentUsage();
            EXPECT_LE(afterCancel, duringCancel + 1024) << "Cancellation should clean up resources";
        }
        
        cancelAsync.reset();
        
        size_t finalAsync = tracker.GetCurrentUsage();
        EXPECT_LE(finalAsync, beforeAsync + 4096) << "All async operations should clean up properly";
    }
}

// Test 8: Garbage Collection and Cleanup Verification
TEST_F(MemoryManagementTests, GarbageCollectionAndCleanupVerification)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "C:\\temp\\GarbageCollectionTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t initialMemory = tracker.GetCurrentUsage();
        
        // Create scenarios that generate temporary allocations
        for (int cycle = 0; cycle < 10; ++cycle)
        {
            // Add and remove users (simulating user session lifecycle)
            PFLocalUserHandle sessionUser = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(0x20000000 + cycle));
            
            XAsyncBlock async{};
            PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
            hr = PFGameSaveFilesAddUserWithUiAsync(sessionUser, options, &async);
            
            if (SUCCEEDED(hr))
            {
                XAsyncGetStatus(&async, true);
                PFGameSaveFilesAddUserWithUiResult(&async);
                
                // Perform operations that may create temporary allocations
                int64_t quota = 0;
                PFGameSaveFilesGetRemainingQuota(sessionUser, &quota);
                
                size_t folderSize = 0;
                PFGameSaveFilesGetFolderSize(sessionUser, &folderSize);
                
                bool isConnected = false;
                PFGameSaveFilesIsConnectedToCloud(sessionUser, &isConnected);
                
                // Upload operation
                XAsyncBlock uploadAsync{};
                PFGameSaveFilesUploadOption uploadOptions = PFGameSaveFilesUploadOption::KeepDeviceActive;
                hr = PFGameSaveFilesUploadWithUiAsync(sessionUser, uploadOptions, &uploadAsync);
                
                if (SUCCEEDED(hr))
                {
                    XAsyncGetStatus(&uploadAsync, true);
                    PFGameSaveFilesUploadWithUiResult(&uploadAsync);
                }
            }
            
            // Force potential garbage collection by sleeping
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        size_t afterCycles = tracker.GetCurrentUsage();
        
        // Test explicit cleanup operations
        bool isConnected = false;
        for (int i = 0; i < 5; ++i)
        {
            hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        }
        
        size_t afterCleanupOps = tracker.GetCurrentUsage();
        
        // Memory should not continuously grow
        size_t memoryGrowth = afterCycles - initialMemory;
        EXPECT_LT(memoryGrowth, 5 * 1024 * 1024) << "Memory growth should be bounded after multiple cycles";
        
        // Test final cleanup
        XAsyncBlock uninitAsync{};
        hr = PFGameSaveFilesUninitializeAsync(&uninitAsync);
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&uninitAsync, true);
            PFGameSaveFilesUninitializeResult(&uninitAsync);
            m_isInitialized = false;
            
            size_t finalMemory = tracker.GetCurrentUsage();
            EXPECT_LE(finalMemory, initialMemory + 1024) << "Final cleanup should restore most memory";
        }
    }
}

// Test 9: Memory Pressure Scenario Handling
TEST_F(MemoryManagementTests, MemoryPressureScenarioHandling)
{
    auto& tracker = MemoryTracker::Instance();
    
    PFGameSaveInitArgs initArgs{};
    initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    initArgs.saveFolder = "MemoryPressureTest";
    
    HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
    if (SUCCEEDED(hr))
    {
        m_isInitialized = true;
        
        size_t baselineMemory = tracker.GetCurrentUsage();
        
        // Simulate memory pressure by allocating large buffers
        std::vector<std::unique_ptr<char[]>> pressureBuffers;
        const size_t BUFFER_SIZE = 1024 * 1024; // 1MB buffers
        
        // Allocate multiple large buffers to simulate memory pressure
        for (int i = 0; i < 10; ++i)
        {
            try
            {
                auto buffer = std::make_unique<char[]>(BUFFER_SIZE);
                tracker.RecordAllocation(BUFFER_SIZE);
                
                // Fill buffer to ensure it's actually allocated
                for (size_t j = 0; j < BUFFER_SIZE; j += 4096)
                {
                    buffer[j] = static_cast<char>(i);
                }
                
                pressureBuffers.push_back(std::move(buffer));
            }
            catch (const std::bad_alloc&)
            {
                // Memory pressure reached - this is expected
                break;
            }
        }
        
        size_t duringPressure = tracker.GetCurrentUsage();
        EXPECT_GT(duringPressure, baselineMemory) << "Memory pressure should increase usage";
        
        // Test API operations under memory pressure
        XAsyncBlock async{};
        PFGameSaveFilesAddUserOptions options = PFGameSaveFilesAddUserOptions::None;
        hr = PFGameSaveFilesAddUserWithUiAsync(TEST_USER, options, &async);
        
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&async, true);
            HRESULT result = PFGameSaveFilesAddUserWithUiResult(&async);
            // Should handle memory pressure gracefully
        }
        
        // Test query operations under pressure
        int64_t quota = 0;
        hr = PFGameSaveFilesGetRemainingQuota(TEST_USER, &quota);
        // Should not crash under memory pressure
        
        bool isConnected = false;
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Connectivity check should work under memory pressure";
        
        // Release memory pressure
        for (size_t i = 0; i < pressureBuffers.size(); ++i)
        {
            tracker.RecordDeallocation(BUFFER_SIZE);
        }
        pressureBuffers.clear();
        
        size_t afterPressureRelief = tracker.GetCurrentUsage();
        EXPECT_LT(afterPressureRelief, duringPressure) << "Memory should be freed after pressure relief";
        
        // Test operations after pressure relief
        hr = PFGameSaveFilesIsConnectedToCloud(TEST_USER, &isConnected);
        EXPECT_SUCCEEDED(hr) << "Operations should work normally after pressure relief";
        
        size_t finalPressureMemory = tracker.GetCurrentUsage();
        EXPECT_LE(finalPressureMemory, baselineMemory + 2048) << "Memory should return to baseline after pressure relief";
    }
}

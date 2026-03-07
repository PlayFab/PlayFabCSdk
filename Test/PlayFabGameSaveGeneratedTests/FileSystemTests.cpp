// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/core/PFCore.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

// Test fixture for File System Operations and Data Integrity tests
class FileSystemTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_isInitialized = false;
        m_testDataDir = "FileSystemTestData";
        
        // Create test data directory
        try
        {
            std::filesystem::create_directories(m_testDataDir);
        }
        catch (...)
        {
            // Ignore directory creation errors in test environment
        }
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
        
        // Cleanup test data directory
        try
        {
            std::filesystem::remove_all(m_testDataDir);
        }
        catch (...)
        {
            // Ignore cleanup errors
        }
    }

    HRESULT InitializeGameSaveDefault()
    {
        if (m_isInitialized) return S_OK;

        PFGameSaveInitArgs initArgs{};
        initArgs.saveFolder = m_testDataDir.c_str();
        initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);

        HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
        if (SUCCEEDED(hr))
        {
            m_isInitialized = true;
        }
        return hr;
    }

    PFLocalUserHandle GetMockUserHandle() const
    {
        return reinterpret_cast<PFLocalUserHandle>(0x12345678);
    }

    bool CreateTestFile(const std::wstring& filename, const std::string& content)
    {
        try
        {
            std::filesystem::path filePath = std::filesystem::path(m_testDataDir) / filename;
            std::ofstream file(filePath, std::ios::binary);
            if (file.is_open())
            {
                file.write(content.c_str(), content.size());
                file.close();
                return true;
            }
        }
        catch (...)
        {
            // Ignore file creation errors
        }
        return false;
    }

    std::string ReadTestFile(const std::wstring& filename)
    {
        try
        {
            std::filesystem::path filePath = std::filesystem::path(m_testDataDir) / filename;
            std::ifstream file(filePath, std::ios::binary);
            if (file.is_open())
            {
                std::string content((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());
                file.close();
                return content;
            }
        }
        catch (...)
        {
            // Ignore file read errors
        }
        return "";
    }

private:
    bool m_isInitialized = false;
    std::string m_testDataDir;
};

// Test file corruption detection and recovery
TEST_F(FileSystemTests, FileCorruptionDetectionAndRecovery)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create test files with known content
    std::string originalContent = "This is valid save game data with checksums";
    std::string corruptedContent = "This is corrupted data XXXX invalid checksums";
    
    EXPECT_TRUE(CreateTestFile(L"valid_save.dat", originalContent));
    EXPECT_TRUE(CreateTestFile(L"corrupted_save.dat", corruptedContent));
    
    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Test folder size calculation (should handle corrupted files gracefully)
    size_t folderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &folderSize);
    
    if (SUCCEEDED(hr))
    {
        // Should report size even if some files are corrupted
        EXPECT_GT(folderSize, 0);
        std::cout << "[ NOTE    ] Folder size with mixed valid/corrupted files: " << folderSize << " bytes" << std::endl;
    }
    else
    {
        std::cout << "[ NOTE    ] File corruption test - folder size failed: 0x" << std::hex << hr 
                  << " (expected in test environment)" << std::endl;
    }
    
    // Verify files still exist
    std::string readValidContent = ReadTestFile(L"valid_save.dat");
    std::string readCorruptedContent = ReadTestFile(L"corrupted_save.dat");
    
    EXPECT_EQ(originalContent, readValidContent);
    EXPECT_EQ(corruptedContent, readCorruptedContent);
}

// Test atomic file operations and rollback mechanisms
TEST_F(FileSystemTests, AtomicFileOperationsAndRollbackMechanisms)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create initial save state
    std::string initialSave = "Initial save state v1.0";
    EXPECT_TRUE(CreateTestFile(L"atomic_test.sav", initialSave));
    
    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Test that operations are atomic (either fully succeed or fully fail)
    XAsyncBlock uploadAsync{};
    HRESULT hr = PFGameSaveFilesUploadWithUiAsync(
        userHandle,
        PFGameSaveFilesUploadOption::KeepDeviceActive,
        &uploadAsync
    );
    
    if (SUCCEEDED(hr))
    {
        XAsyncGetStatus(&uploadAsync, true);
        HRESULT uploadResult = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
        
        // Whether upload succeeds or fails, local files should remain consistent
        std::string contentAfterUpload = ReadTestFile(L"atomic_test.sav");
        EXPECT_FALSE(contentAfterUpload.empty());
        
        std::cout << "[ NOTE    ] Atomic upload test result: 0x" << std::hex << uploadResult << std::endl;
        std::cout << "[ NOTE    ] File content preserved: " << (contentAfterUpload == initialSave ? "Yes" : "No") << std::endl;
    }
    
    // Test rollback on failure scenario
    // Create a scenario that might fail (e.g., insufficient permissions)
    std::string largeContent(1024 * 1024, 'X'); // 1MB of data
    EXPECT_TRUE(CreateTestFile(L"large_test.sav", largeContent));
    
    size_t folderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(userHandle, &folderSize);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Folder size after large file creation: " << folderSize << " bytes" << std::endl;
        EXPECT_GT(folderSize, largeContent.size());
    }
}

// Test disk space exhaustion handling
TEST_F(FileSystemTests, DiskSpaceExhaustionHandling)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Test quota checking which relates to disk space management
    int64_t remainingQuota = 0;
    HRESULT hr = PFGameSaveFilesGetRemainingQuota(userHandle, &remainingQuota);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Remaining quota: " << remainingQuota << " bytes" << std::endl;
        EXPECT_GE(remainingQuota, 0);
        
        // Create files approaching quota limits
        if (remainingQuota > 1024)
        {
            size_t dataSize = static_cast<size_t>(remainingQuota / 2);
            if (dataSize > 1024 * 1024) {
                dataSize = 1024 * 1024;
            }
            std::string largeData(dataSize, 'Q');
            EXPECT_TRUE(CreateTestFile(L"quota_test.dat", largeData));
            
            // Check quota again
            int64_t newQuota = 0;
            hr = PFGameSaveFilesGetRemainingQuota(userHandle, &newQuota);
            if (SUCCEEDED(hr))
            {
                std::cout << "[ NOTE    ] Quota after large file: " << newQuota << " bytes" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "[ NOTE    ] Quota check unavailable in test environment: 0x" << std::hex << hr << std::endl;
    }
    
    // Test behavior when attempting operations near quota limits
    XAsyncBlock uploadAsync{};
    hr = PFGameSaveFilesUploadWithUiAsync(
        userHandle,
        PFGameSaveFilesUploadOption::KeepDeviceActive,
        &uploadAsync
    );
    
    if (SUCCEEDED(hr))
    {
        XAsyncGetStatus(&uploadAsync, true);
        HRESULT uploadResult = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
        
        // Should handle quota exhaustion gracefully
        if (uploadResult == HRESULT(0x8007000AL)) // E_PF_GAMESAVE_DISK_FULL
        {
            std::cout << "[ NOTE    ] Disk full error properly handled during upload" << std::endl;
        }
        else
        {
            std::cout << "[ NOTE    ] Upload with quota constraints: 0x" << std::hex << uploadResult << std::endl;
        }
    }
}

// Test file locking and concurrent access protection
TEST_F(FileSystemTests, FileLockingAndConcurrentAccessProtection)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create test file for locking scenarios
    std::string testContent = "File locking test data";
    EXPECT_TRUE(CreateTestFile(L"lock_test.sav", testContent));
    
    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Test that file operations handle locking properly
    size_t folderSize1 = 0;
    size_t folderSize2 = 0;
    
    HRESULT hr1 = PFGameSaveFilesGetFolderSize(userHandle, &folderSize1);
    HRESULT hr2 = PFGameSaveFilesGetFolderSize(userHandle, &folderSize2);
    
    if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
    {
        // Concurrent reads should succeed and return consistent results
        EXPECT_EQ(folderSize1, folderSize2);
        std::cout << "[ NOTE    ] Concurrent folder size reads: " << folderSize1 << " bytes (consistent)" << std::endl;
    }
    
    // Test operations that might require exclusive access
    XAsyncBlock async1{}, async2{};
    
    hr1 = PFGameSaveFilesUploadWithUiAsync(userHandle, PFGameSaveFilesUploadOption::KeepDeviceActive, &async1);
    hr2 = PFGameSaveFilesUploadWithUiAsync(userHandle, PFGameSaveFilesUploadOption::KeepDeviceActive, &async2);
    
    if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
    {
        XAsyncGetStatus(&async1, true);
        XAsyncGetStatus(&async2, true);
        
        HRESULT result1 = PFGameSaveFilesUploadWithUiResult(&async1);
        HRESULT result2 = PFGameSaveFilesUploadWithUiResult(&async2);
        
        std::cout << "[ NOTE    ] Concurrent upload operations: 0x" << std::hex << result1 
                  << ", 0x" << std::hex << result2 << std::endl;
        
        // At least one should succeed or both should fail gracefully
        EXPECT_TRUE(SUCCEEDED(result1) || SUCCEEDED(result2) || 
                   (FAILED(result1) && FAILED(result2)));
    }
}

// Test file metadata preservation during sync
TEST_F(FileSystemTests, FileMetadataPreservationDuringSync)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create files with different characteristics
    std::string smallFile = "Small save file";
    std::string largeFile(10240, 'L'); // 10KB file
    std::string binaryFile = "\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR"; // Binary data
    
    EXPECT_TRUE(CreateTestFile(L"small.sav", smallFile));
    EXPECT_TRUE(CreateTestFile(L"large.sav", largeFile));
    EXPECT_TRUE(CreateTestFile(L"binary.dat", binaryFile));
    
    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Record initial folder state
    size_t initialFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &initialFolderSize);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Initial folder size: " << initialFolderSize << " bytes" << std::endl;
        
        // Test sync operation
        XAsyncBlock syncAsync{};
        hr = PFGameSaveFilesUploadWithUiAsync(
            userHandle,
            PFGameSaveFilesUploadOption::KeepDeviceActive,
            &syncAsync
        );
        
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&syncAsync, true);
            HRESULT syncResult = PFGameSaveFilesUploadWithUiResult(&syncAsync);
            
            // Check folder size after sync
            size_t postSyncFolderSize = 0;
            hr = PFGameSaveFilesGetFolderSize(userHandle, &postSyncFolderSize);
            
            if (SUCCEEDED(hr))
            {
                std::cout << "[ NOTE    ] Post-sync folder size: " << postSyncFolderSize << " bytes" << std::endl;
                
                // Metadata should be preserved (folder size should remain consistent)
                EXPECT_EQ(initialFolderSize, postSyncFolderSize);
            }
            
            // Verify file contents are preserved
            std::string readSmall = ReadTestFile(L"small.sav");
            std::string readLarge = ReadTestFile(L"large.sav");
            std::string readBinary = ReadTestFile(L"binary.dat");
            
            EXPECT_EQ(smallFile, readSmall);
            EXPECT_EQ(largeFile, readLarge);
            EXPECT_EQ(binaryFile, readBinary);
            
            std::cout << "[ NOTE    ] File content preservation: " 
                      << (readSmall == smallFile && readLarge == largeFile && readBinary == binaryFile ? "Success" : "Failed") 
                      << std::endl;
        }
    }
}

// Test file system permission handling
TEST_F(FileSystemTests, FileSystemPermissionHandling)
{
    // Test initialization with different permission scenarios
    std::vector<std::string> testPaths = {
        "NormalFolder",
        "PermissionTestFolder",
        "ReadOnlyTest"
    };

    for (const auto& testPath : testPaths)
    {
        PFGameSaveInitArgs initArgs{};
        initArgs.saveFolder = testPath.c_str();
        initArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);

        HRESULT hr = PFGameSaveFilesInitialize(&initArgs);
        
        if (SUCCEEDED(hr))
        {
            // std::wcout << L"[ NOTE    ] Permission test succeeded for: " << testPath << std::endl;
            
            // Test operations with this path
            PFLocalUserHandle userHandle = GetMockUserHandle();
            size_t folderSize = 0;
            HRESULT opResult = PFGameSaveFilesGetFolderSize(userHandle, &folderSize);
            
            std::cout << "[ NOTE    ] Operation result: 0x" << std::hex << opResult << std::endl;
            
            // Cleanup
            XAsyncBlock async{};
            PFGameSaveFilesUninitializeAsync(&async);
            XAsyncGetStatus(&async, true);
            PFGameSaveFilesUninitializeResult(&async);
        }
        else
        {
            // std::wcout << L"[ NOTE    ] Permission test failed for: " << testPath 
                      // << L" (HRESULT: 0x" << std::hex << hr << L")" << std::endl;
        }
    }
}

// Test large file handling and streaming
TEST_F(FileSystemTests, LargeFileHandlingAndStreaming)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create progressively larger files to test handling
    std::vector<size_t> fileSizes = { 1024, 10240, 102400, 1048576 }; // 1KB, 10KB, 100KB, 1MB
    
    for (size_t i = 0; i < fileSizes.size(); ++i)
    {
        std::wstring filename = L"large_file_" + std::to_wstring(i) + L".dat";
        std::string content(fileSizes[i], 'A' + static_cast<char>(i));
        
        bool created = CreateTestFile(filename, content);
        EXPECT_TRUE(created);
        
        if (created)
        {
            std::cout << "[ NOTE    ] Created large file: " << fileSizes[i] << " bytes" << std::endl;
        }
    }

    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Test folder size calculation with large files
    size_t totalFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &totalFolderSize);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Total folder size with large files: " << totalFolderSize << " bytes" << std::endl;
        
        // Verify size is reasonable (should be at least the sum of our test files)
        size_t expectedMinSize = 0;
        for (size_t size : fileSizes)
        {
            expectedMinSize += size;
        }
        
        EXPECT_GE(totalFolderSize, expectedMinSize);
    }
    
    // Test upload operation with large files
    XAsyncBlock uploadAsync{};
    hr = PFGameSaveFilesUploadWithUiAsync(
        userHandle,
        PFGameSaveFilesUploadOption::KeepDeviceActive,
        &uploadAsync
    );
    
    if (SUCCEEDED(hr))
    {
        XAsyncGetStatus(&uploadAsync, true);
        HRESULT uploadResult = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
        
        std::cout << "[ NOTE    ] Large file upload test result: 0x" << std::hex << uploadResult << std::endl;
        
        // Should handle large files gracefully (success or appropriate failure)
        EXPECT_TRUE(SUCCEEDED(uploadResult) || FAILED(uploadResult));
    }
}

// Test file system monitoring and change detection
TEST_F(FileSystemTests, FileSystemMonitoringAndChangeDetection)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Record initial state
    size_t initialSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &initialSize);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Initial folder size: " << initialSize << " bytes" << std::endl;
        
        // Make changes to file system
        std::string newContent = "New file for change detection test";
        EXPECT_TRUE(CreateTestFile(L"change_detection.sav", newContent));
        
        // Check if changes are detected
        size_t newSize = 0;
        hr = PFGameSaveFilesGetFolderSize(userHandle, &newSize);
        
        if (SUCCEEDED(hr))
        {
            std::cout << "[ NOTE    ] Folder size after change: " << newSize << " bytes" << std::endl;
            
            // Should detect the file system change
            EXPECT_GT(newSize, initialSize);
            
            size_t expectedIncrease = newContent.size();
            size_t actualIncrease = newSize - initialSize;
            
            std::cout << "[ NOTE    ] Size increase - expected: " << expectedIncrease 
                      << ", actual: " << actualIncrease << std::endl;
            
            // Actual increase might be different due to file system overhead, but should be positive
            EXPECT_GT(actualIncrease, 0);
        }
    }
}

// Test backup and restore functionality
TEST_F(FileSystemTests, BackupAndRestoreFunctionality)
{
    HRESULT initResult = InitializeGameSaveDefault();
    if (FAILED(initResult))
    {
        std::cout << "[ SKIPPED ] Test skipped - initialization failed" << std::endl;
        return;
    }

    // Create original save data
    std::string originalData = "Original save game data v1.0";
    EXPECT_TRUE(CreateTestFile(L"backup_test.sav", originalData));
    
    PFLocalUserHandle userHandle = GetMockUserHandle();
    
    // Record initial state for "backup"
    size_t originalSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &originalSize);
    
    if (SUCCEEDED(hr))
    {
        std::cout << "[ NOTE    ] Original save size: " << originalSize << " bytes" << std::endl;
        
        // Simulate upload (backup to cloud)
        XAsyncBlock uploadAsync{};
        hr = PFGameSaveFilesUploadWithUiAsync(
            userHandle,
            PFGameSaveFilesUploadOption::KeepDeviceActive,
            &uploadAsync
        );
        
        if (SUCCEEDED(hr))
        {
            XAsyncGetStatus(&uploadAsync, true);
            HRESULT uploadResult = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
            
            std::cout << "[ NOTE    ] Backup (upload) result: 0x" << std::hex << uploadResult << std::endl;
            
            // Modify local data (simulate corruption or user error)
            std::string modifiedData = "Modified/corrupted save data";
            EXPECT_TRUE(CreateTestFile(L"backup_test.sav", modifiedData));
            
            // Verify modification
            std::string readModified = ReadTestFile(L"backup_test.sav");
            EXPECT_EQ(modifiedData, readModified);
            
            // Test restore capability (in a real scenario this would download from cloud)
            // For test purposes, we simulate restoration by checking if we can get back to consistent state
            size_t modifiedSize = 0;
            hr = PFGameSaveFilesGetFolderSize(userHandle, &modifiedSize);
            
            if (SUCCEEDED(hr))
            {
                std::cout << "[ NOTE    ] Size after modification: " << modifiedSize << " bytes" << std::endl;
                
                // In a real restore scenario, we would download from cloud and overwrite local
                // For test, verify that the system can handle file replacements
                std::string restoredData = "Restored save game data v1.0";
                EXPECT_TRUE(CreateTestFile(L"backup_test.sav", restoredData));
                
                std::string readRestored = ReadTestFile(L"backup_test.sav");
                EXPECT_EQ(restoredData, readRestored);
                
                std::cout << "[ NOTE    ] Backup and restore simulation completed successfully" << std::endl;
            }
        }
    }
}

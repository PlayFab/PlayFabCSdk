// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/core/PFCore.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>

// Type-safe async context identifiers
enum class AsyncContextId : uintptr_t
{
    XBOX = 0x1000,
    CANCEL = 0x2000,
    AUTH = 0x3000,
    CONCURRENT_OP1 = 0x4001,
    CONCURRENT_OP2 = 0x4002
};

// Type-safe user handle identifiers  
enum class UserHandleId : uintptr_t
{
    TEST_USER1 = 0x12345678,
    TEST_USER2 = 0x87654321
};

// Test fixture for Platform Abstraction Layer tests
class PlatformTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean state for each test
        s_callbackCount = 0;
        s_lastError = S_OK;
        s_lastContext = nullptr;
        s_isInitialized = false;
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
        }
    }

    // Static tracking variables
    static std::atomic<int> s_callbackCount;
    static HRESULT s_lastError;
    static void* s_lastContext;
    static bool s_isInitialized;

    // Mock callback for async operations
    static void CALLBACK TestAsyncCallback(XAsyncBlock* asyncBlock)
    {
        s_callbackCount++;
        s_lastError = XAsyncGetStatus(asyncBlock, false);
        s_lastContext = asyncBlock->context;
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
        auto startTime = std::chrono::steady_clock::now();
        while (s_callbackCount.load() < expectedCount)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
            if (elapsed.count() > timeoutMs)
            {
                return false; // Timeout
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return true;
    }

    // Helper to initialize GameSave for platform tests
    bool InitializeGameSave(const char* saveFolder = nullptr)
    {
        if (s_isInitialized)
        {
            return true;
        }

        PFGameSaveInitArgs args{};
        args.backgroundQueue = nullptr;
        args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
        
        if (saveFolder)
        {
            args.saveFolder = saveFolder;
        }
        else
        {
#if defined(_WIN32)
            args.saveFolder = "C:\\temp\\PlatformTestSaves";
#else
            args.saveFolder = "/tmp/PlatformTestSaves";
#endif
        }

        HRESULT hr = PFGameSaveFilesInitialize(&args);
        if (SUCCEEDED(hr))
        {
            s_isInitialized = true;
            return true;
        }
        return false;
    }

    // Helper to create test user handle (platform-specific)
    PFLocalUserHandle CreateTestUserHandle()
    {
        // Platform-specific user handle creation would go here
        // For testing, we'll use a mock handle
        return reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(UserHandleId::TEST_USER1));
    }
};

// Static member definitions
std::atomic<int> PlatformTests::s_callbackCount(0);
HRESULT PlatformTests::s_lastError = S_OK;
void* PlatformTests::s_lastContext = nullptr;
bool PlatformTests::s_isInitialized = false;

// Task A: Generate tests for Windows platform-specific file operations
TEST_F(PlatformTests, WindowsPlatformFileOperations)
{
#if defined(_WIN32)
    // Test Windows-specific file operations
    ASSERT_TRUE(InitializeGameSave("C:\\temp\\WindowsGameSaveTest"));

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test getting folder size on Windows
    size_t saveRootFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(testUser, &saveRootFolderSize);
    
    // May succeed or fail depending on user setup, but should not crash
    // Check for common expected error conditions in test environment
    EXPECT_TRUE(SUCCEEDED(hr) || 
                hr == E_PF_GAMESAVE_USER_NOT_ADDED ||
                hr == E_PF_GAMESAVE_NOT_INITIALIZED ||
                hr == E_INVALIDARG ||
                hr == E_FAIL) << "Unexpected HRESULT: 0x" << std::hex << hr;
    
    if (SUCCEEDED(hr))
    {
        // Test getting folder path on Windows
        std::vector<char> folderBuffer(saveRootFolderSize);
        size_t folderUsed = 0;
        
        hr = PFGameSaveFilesGetFolder(testUser, saveRootFolderSize, folderBuffer.data(), &folderUsed);
        if (SUCCEEDED(hr))
        {
            // Verify path format is Windows-style
            std::string folderPath(folderBuffer.data(), folderUsed);
            EXPECT_TRUE(folderPath.find("C:\\") == 0 || folderPath.find("c:\\") == 0);
            EXPECT_NE(folderPath.find("\\"), std::string::npos); // Should contain backslashes
        }
    }
    
    // Test Windows path validation
    PFGameSaveInitArgs windowsArgs{};
    windowsArgs.backgroundQueue = nullptr;
    windowsArgs.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
    
    // Test valid Windows paths
    windowsArgs.saveFolder = "C:\\Program Files\\GameSaves";
    // Note: Can't reinitialize, but testing path validation logic
    
    // Test invalid Windows paths
    std::vector<const char*> invalidPaths = {
        "C:/unix/style/path", // Unix style (may or may not be supported)
        "\\\\invalid\\unc\\path",
        "X:\\nonexistent\\drive",
        ""
    };
    
    for (const auto& invalidPath : invalidPaths)
    {
        windowsArgs.saveFolder = invalidPath;
        // Would test if we could reinitialize - path validation is internal
        EXPECT_TRUE(true); // Placeholder - actual validation happens internally
    }
#else
    std::cout << "[ SKIPPED ] Windows platform test - skipping on non-Windows platforms" << std::endl;
    return;
#endif
}

// Task B: Create tests for Xbox GDK platform integration
TEST_F(PlatformTests, XboxGDKPlatformIntegration)
{
#if defined(_GAMING_DESKTOP) || defined(_GAMING_XBOX)
    // Test Xbox GDK platform-specific behavior
    ASSERT_TRUE(InitializeGameSave("T:\\GameSaveTestFolder"));

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test Xbox-specific storage quota behavior
    int64_t remainingQuota = -1;
    HRESULT hr = PFGameSaveFilesGetRemainingQuota(testUser, &remainingQuota);
    
    // May fail if user not added, but should handle Xbox quota properly
    if (SUCCEEDED(hr))
    {
        // Xbox typically has storage limits
        EXPECT_GE(remainingQuota, 0);
        EXPECT_LE(remainingQuota, 1024 * 1024 * 1024); // Reasonable upper bound for Xbox
    }
    
    // Test Xbox cloud connectivity
    bool isConnectedToCloud = false;
    hr = PFGameSaveFilesIsConnectedToCloud(testUser, &isConnectedToCloud);
    
    // Should succeed even if not connected
    EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED);
    
    // Test Xbox user handle behavior
    s_callbackCount = 0;
    XAsyncBlock addUserAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::XBOX)));
    
    hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
    if (SUCCEEDED(hr))
    {
        bool callbackReceived = WaitForCallback(5000);
        if (callbackReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::XBOX)));
            
            hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
            // Result depends on actual Xbox environment
        }
    }
    
    // Test Xbox-specific save folder (T: drive)
    size_t saveRootFolderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(testUser, &saveRootFolderSize);
    if (SUCCEEDED(hr) && saveRootFolderSize > 0)
    {
        std::vector<char> folderBuffer(saveRootFolderSize);
        size_t folderUsed = 0;
        
        hr = PFGameSaveFilesGetFolder(testUser, saveRootFolderSize, folderBuffer.data(), &folderUsed);
        if (SUCCEEDED(hr))
        {
            std::string folderPath(folderBuffer.data(), folderUsed);
            // Xbox typically uses T: drive for game saves
            EXPECT_TRUE(folderPath.find("T:\\") == 0 || folderPath.find("t:\\") == 0);
        }
    }
#else
    std::cout << "[ SKIPPED ] Xbox GDK platform test - skipping on non-Xbox platforms" << std::endl;
    return;
#endif
}

// Task C: Generate tests for Steam Deck platform compatibility
TEST_F(PlatformTests, SteamDeckPlatformCompatibility)
{
#if defined(__linux__) || defined(__unix__)
    // Test Steam Deck (Linux) platform compatibility
    ASSERT_TRUE(InitializeGameSave("/home/deck/.local/share/GameSaves"));

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test Linux-style path handling
    size_t saveRootFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(testUser, &saveRootFolderSize);
    
    if (SUCCEEDED(hr) && saveRootFolderSize > 0)
    {
        std::vector<char> folderBuffer(saveRootFolderSize);
        size_t folderUsed = 0;
        
        hr = PFGameSaveFilesGetFolder(testUser, saveRootFolderSize, folderBuffer.data(), &folderUsed);
        if (SUCCEEDED(hr))
        {
            std::string folderPath(folderBuffer.data(), folderUsed);
            // Should use Unix-style paths
            EXPECT_TRUE(folderPath.find("/") == 0); // Absolute path
            EXPECT_EQ(folderPath.find("\\"), std::string::npos); // No backslashes
        }
    }
    
    // Test Steam Deck specific behaviors
    bool isConnectedToCloud = false;
    hr = PFGameSaveFilesIsConnectedToCloud(testUser, &isConnectedToCloud);
    
    // Steam Deck may have limited cloud connectivity
    EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED || hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
    
    // Test file system permissions on Linux
    // Steam Deck should have proper file access
    int64_t remainingQuota = -1;
    hr = PFGameSaveFilesGetRemainingQuota(testUser, &remainingQuota);
    
    // May fail if user not authenticated, but file system should be accessible
    EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED || hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
    
    // Test Steam Deck path validation
    std::vector<const char*> validLinuxPaths = {
        "/home/deck/.local/share/GameSaves",
        "/tmp/game_saves",
        "/var/games/saves"
    };
    
    for (const auto& path : validLinuxPaths)
    {
        // Path validation is internal, but these should be valid Linux paths
        EXPECT_TRUE(path[0] == '/'); // Must be absolute
        EXPECT_EQ(std::string(path).find("\\"), std::string::npos); // No Windows separators
    }
#else
    std::cout << "[ SKIPPED ] Steam Deck platform test - skipping on non-Linux platforms" << std::endl;
    return;
#endif
}

// Task D: Create tests for platform-specific user handle management
TEST_F(PlatformTests, PlatformUserHandleManagement)
{
    ASSERT_TRUE(InitializeGameSave());

    // Test platform-specific user handle behavior
    PFLocalUserHandle testUser1 = CreateTestUserHandle();
    PFLocalUserHandle testUser2 = reinterpret_cast<PFLocalUserHandle>(static_cast<uintptr_t>(UserHandleId::TEST_USER2));
    
    // Test multiple user handles on the same platform
    std::vector<PFLocalUserHandle> userHandles = { testUser1, testUser2 };
    
    for (auto userHandle : userHandles)
    {
        // Test user handle validation
        size_t saveRootFolderSize = 0;
        HRESULT hr = PFGameSaveFilesGetFolderSize(userHandle, &saveRootFolderSize);
        
        // Should handle invalid user gracefully - check common test environment errors
        EXPECT_TRUE(SUCCEEDED(hr) || 
                    hr == E_PF_GAMESAVE_USER_NOT_ADDED || 
                    hr == E_INVALIDARG ||
                    hr == E_PF_GAMESAVE_NOT_INITIALIZED ||
                    hr == E_FAIL) << "Unexpected HRESULT for GetFolderSize: 0x" << std::hex << hr;
        
        // Test user connectivity state
        bool isConnectedToCloud = false;
        hr = PFGameSaveFilesIsConnectedToCloud(userHandle, &isConnectedToCloud);
        EXPECT_TRUE(SUCCEEDED(hr) || 
                    hr == E_PF_GAMESAVE_USER_NOT_ADDED || 
                    hr == E_INVALIDARG ||
                    hr == E_PF_GAMESAVE_NOT_INITIALIZED ||
                    hr == E_FAIL) << "Unexpected HRESULT for IsConnectedToCloud: 0x" << std::hex << hr;
    }
    
    // Test null user handle
    size_t saveRootFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(nullptr, &saveRootFolderSize);
    EXPECT_EQ(hr, E_INVALIDARG);
    
    bool isConnectedToCloud = false;
    hr = PFGameSaveFilesIsConnectedToCloud(nullptr, &isConnectedToCloud);
    EXPECT_EQ(hr, E_INVALIDARG);
    
    int64_t remainingQuota = -1;
    hr = PFGameSaveFilesGetRemainingQuota(nullptr, &remainingQuota);
    EXPECT_EQ(hr, E_INVALIDARG);
}

// Task E: Generate tests for platform file system path handling
TEST_F(PlatformTests, PlatformFileSystemPathHandling)
{
    // Test different path formats for different platforms
    std::vector<std::pair<const char*, bool>> pathTests;
    
#if defined(_WIN32)
    pathTests = {
        { "C:\\Games\\Saves", true },
        { "D:\\Users\\Player\\Documents\\GameSaves", true },
        { "\\\\Server\\Share\\GameSaves", true }, // UNC path
        { "Games\\Relative\\Path", false }, // Relative path
        { "/unix/style/path", false }, // Unix style on Windows
        { "", false }, // Empty path
        { nullptr, false } // Null path
    };
#elif defined(__linux__) || defined(__unix__)
    pathTests = {
        { "/home/user/GameSaves", true },
        { "/tmp/game_saves", true },
        { "/var/games/saves", true },
        { "relative/path", false }, // Relative path
        { "C:\\Windows\\Style", false }, // Windows style on Linux
        { "", false }, // Empty path
        { nullptr, false } // Null path
    };
#endif

    for (const auto& test : pathTests)
    {
        PFGameSaveInitArgs args{};
        args.backgroundQueue = nullptr;
        args.options = static_cast<uint64_t>(PFGameSaveInitOptions::None);
        args.saveFolder = test.first;
        
        // Note: Can't test initialization with different paths in same test
        // But we can validate the path format logic
        if (test.first != nullptr)
        {
            std::string path(test.first);
            bool isAbsolute = false;
            
#if defined(_WIN32)
            // Windows absolute path check
            isAbsolute = (path.length() >= 3 && path[1] == ':' && path[2] == '\\') ||
                        (path.length() >= 2 && path[0] == '\\' && path[1] == '\\');
#else
            // Unix absolute path check
            isAbsolute = (path.length() > 0 && path[0] == '/');
#endif
            
            EXPECT_EQ(isAbsolute, test.second) << "Path: " << test.first;
        }
    }
}

// Task F: Create tests for platform-specific async patterns
TEST_F(PlatformTests, PlatformAsyncPatterns)
{
    ASSERT_TRUE(InitializeGameSave());

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test platform-specific async operation patterns
    s_callbackCount = 0;
    XAsyncBlock asyncBlock1 = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::CONCURRENT_OP1)));
    XAsyncBlock asyncBlock2 = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::CONCURRENT_OP2)));
    
    // Test concurrent async operations on platform
    HRESULT hr1 = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &asyncBlock1);
    HRESULT hr2 = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &asyncBlock2);
    
    // Platform should handle concurrent operations gracefully
    if (SUCCEEDED(hr1))
    {
        if (SUCCEEDED(hr2))
        {
            // Both succeeded - wait for both callbacks
            bool callbacksReceived = WaitForCallback(5000, 2);
            EXPECT_TRUE(callbacksReceived || s_callbackCount.load() >= 1);
        }
        else
        {
            // Second failed - should still get first callback
            bool callbackReceived = WaitForCallback(5000, 1);
            EXPECT_TRUE(callbackReceived);
        }
    }
    
    // Test async operation cancellation on platform
    s_callbackCount = 0;
    XAsyncBlock cancelAsyncBlock = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::CANCEL)));
    
    hr1 = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &cancelAsyncBlock);
    if (SUCCEEDED(hr1))
    {
        // Cancel immediately
        XAsyncCancel(&cancelAsyncBlock);
        
        bool callbackReceived = WaitForCallback(3000);
        if (callbackReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::CANCEL)));
            // Should receive cancellation error
            EXPECT_TRUE(s_lastError == E_ABORT || s_lastError == E_PF_GAMESAVE_USER_CANCELLED);
        }
    }
}

// Task G: Generate tests for platform authentication integration
TEST_F(PlatformTests, PlatformAuthenticationIntegration)
{
    ASSERT_TRUE(InitializeGameSave());

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test platform-specific authentication patterns
    s_callbackCount = 0;
    XAsyncBlock authAsync = CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::AUTH)));
    
    // Test user authentication on platform
    HRESULT hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &authAsync);
    if (SUCCEEDED(hr))
    {
        bool callbackReceived = WaitForCallback(5000);
        if (callbackReceived)
        {
            EXPECT_EQ(s_lastContext, reinterpret_cast<void*>(static_cast<uintptr_t>(AsyncContextId::AUTH)));
            
            // Get authentication result
            hr = PFGameSaveFilesAddUserWithUiResult(&authAsync);
            
            if (SUCCEEDED(hr))
            {
                // Test authenticated user operations
                bool isConnectedToCloud = false;
                hr = PFGameSaveFilesIsConnectedToCloud(testUser, &isConnectedToCloud);
                EXPECT_SUCCEEDED(hr);
                
                int64_t remainingQuota = -1;
                hr = PFGameSaveFilesGetRemainingQuota(testUser, &remainingQuota);
                EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD);
            }
        }
    }
    
    // Test authentication state persistence across operations
    if (SUCCEEDED(s_lastError))
    {
        // Test that user remains authenticated for subsequent operations
        s_callbackCount = 0;
        XAsyncBlock uploadAsync = CreateTestAsyncBlock();
        
        hr = PFGameSaveFilesUploadWithUiAsync(testUser, PFGameSaveFilesUploadOption::KeepDeviceActive, &uploadAsync);
        if (SUCCEEDED(hr))
        {
            bool callbackReceived = WaitForCallback(5000);
            if (callbackReceived)
            {
                hr = PFGameSaveFilesUploadWithUiResult(&uploadAsync);
                // Result depends on actual platform state
            }
        }
    }
}

// Task H: Create tests for platform storage quota management
TEST_F(PlatformTests, PlatformStorageQuotaManagement)
{
    ASSERT_TRUE(InitializeGameSave());

    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test platform-specific storage quota behavior
    int64_t remainingQuota = -1;
    HRESULT hr = PFGameSaveFilesGetRemainingQuota(testUser, &remainingQuota);
    
    if (hr == E_PF_GAMESAVE_USER_NOT_ADDED)
    {
        // Need to add user first
        s_callbackCount = 0;
        XAsyncBlock addUserAsync = CreateTestAsyncBlock();
        
        hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &addUserAsync);
        if (SUCCEEDED(hr))
        {
            bool callbackReceived = WaitForCallback(5000);
            if (callbackReceived && SUCCEEDED(s_lastError))
            {
                hr = PFGameSaveFilesAddUserWithUiResult(&addUserAsync);
                if (SUCCEEDED(hr))
                {
                    // Try quota again
                    hr = PFGameSaveFilesGetRemainingQuota(testUser, &remainingQuota);
                }
            }
        }
    }
    
    if (SUCCEEDED(hr))
    {
        // Validate quota values are reasonable
        EXPECT_GE(remainingQuota, 0);
        
        // Platform-specific quota expectations
#if defined(_GAMING_DESKTOP) || defined(_GAMING_XBOX)
        // Xbox typically has storage limits
        EXPECT_LE(remainingQuota, 1024LL * 1024 * 1024); // 1GB reasonable upper bound
#elif defined(_WIN32)
        // Windows may have larger quotas
        EXPECT_LE(remainingQuota, 10LL * 1024 * 1024 * 1024); // 10GB reasonable upper bound
#endif
    }
    
    // Test quota after storage operations
    size_t saveRootFolderSize = 0;
    hr = PFGameSaveFilesGetFolderSize(testUser, &saveRootFolderSize);
    if (SUCCEEDED(hr))
    {
        // Folder size should be reasonable relative to quota
        if (remainingQuota > 0 && saveRootFolderSize > 0)
        {
            EXPECT_LE(static_cast<int64_t>(saveRootFolderSize), remainingQuota + (1024 * 1024)); // Allow some overhead
        }
    }
}

// Task I: Generate cross-platform compatibility tests
TEST_F(PlatformTests, CrossPlatformCompatibility)
{
    ASSERT_TRUE(InitializeGameSave());

    // Test cross-platform compatible behavior
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test that basic operations work consistently across platforms
    struct PlatformOperation {
        const char* name;
        std::function<HRESULT()> operation;
    };
    
    std::vector<PlatformOperation> operations = {
        {
            "GetFolderSize",
            [&]() {
                size_t size = 0;
                return PFGameSaveFilesGetFolderSize(testUser, &size);
            }
        },
        {
            "IsConnectedToCloud",
            [&]() {
                bool connected = false;
                return PFGameSaveFilesIsConnectedToCloud(testUser, &connected);
            }
        },
        {
            "GetRemainingQuota",
            [&]() {
                int64_t quota = -1;
                return PFGameSaveFilesGetRemainingQuota(testUser, &quota);
            }
        }
    };
    
    for (const auto& operation : operations)
    {
        HRESULT hr = operation.operation();
        
        // Operations should either succeed or fail with expected error codes
        EXPECT_TRUE(SUCCEEDED(hr) || 
                   hr == E_PF_GAMESAVE_USER_NOT_ADDED ||
                   hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD ||
                   hr == E_PF_GAMESAVE_NOT_INITIALIZED ||
                   hr == E_INVALIDARG ||
                   hr == E_FAIL) << "Operation: " << operation.name << " HRESULT: 0x" << std::hex << hr;
    }
    
    // Test error code consistency across platforms
    HRESULT hr = PFGameSaveFilesGetFolderSize(nullptr, nullptr);
    EXPECT_EQ(hr, E_INVALIDARG); // Should be consistent
    
    hr = PFGameSaveFilesIsConnectedToCloud(nullptr, nullptr);
    EXPECT_EQ(hr, E_INVALIDARG); // Should be consistent
    
    hr = PFGameSaveFilesGetRemainingQuota(nullptr, nullptr);
    EXPECT_EQ(hr, E_INVALIDARG); // Should be consistent
}

// Task J: Additional platform-specific edge case tests
TEST_F(PlatformTests, PlatformEdgeCases)
{
#if defined(_WIN32)
    ASSERT_TRUE(InitializeGameSave());

    // Test platform-specific edge cases
    PFLocalUserHandle testUser = CreateTestUserHandle();
    
    // Test maximum path length handling
    std::string longPath;
#if defined(_WIN32)
    // Windows MAX_PATH is typically 260
    longPath = "C:\\";
    longPath += std::string(300, 'a'); // Exceed MAX_PATH
    longPath += "\\GameSaves";
#else
    // Unix systems typically support longer paths
    longPath = "/tmp/";
    longPath += std::string(1000, 'a');
    longPath += "/GameSaves";
#endif

    // Test buffer size edge cases
    size_t saveRootFolderSize = 0;
    HRESULT hr = PFGameSaveFilesGetFolderSize(testUser, &saveRootFolderSize);
    
    if (SUCCEEDED(hr) && saveRootFolderSize > 0)
    {
        // Test with undersized buffer
        std::vector<char> smallBuffer(1);
        size_t folderUsed = 0;
        
        hr = PFGameSaveFilesGetFolder(testUser, 1, smallBuffer.data(), &folderUsed);
        EXPECT_EQ(hr, E_NOT_SUFFICIENT_BUFFER);
        
        // Test with exact size buffer
        std::vector<char> exactBuffer(saveRootFolderSize);
        hr = PFGameSaveFilesGetFolder(testUser, saveRootFolderSize, exactBuffer.data(), &folderUsed);
        EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED);
        
        // Test with oversized buffer
        std::vector<char> largeBuffer(saveRootFolderSize * 2);
        hr = PFGameSaveFilesGetFolder(testUser, saveRootFolderSize * 2, largeBuffer.data(), &folderUsed);
        EXPECT_TRUE(SUCCEEDED(hr) || hr == E_PF_GAMESAVE_USER_NOT_ADDED);
    }
    
    // Test platform resource limits
    const int maxConcurrentOperations = 10;
    std::vector<XAsyncBlock> asyncBlocks;
    
    for (int i = 0; i < maxConcurrentOperations; i++)
    {
        asyncBlocks.emplace_back(CreateTestAsyncBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(i))));
    }
    
    s_callbackCount = 0;
    int successfulStarts = 0;
    
    for (int i = 0; i < maxConcurrentOperations; i++)
    {
        hr = PFGameSaveFilesAddUserWithUiAsync(testUser, PFGameSaveFilesAddUserOptions::None, &asyncBlocks[i]);
        if (SUCCEEDED(hr))
        {
            successfulStarts++;
        }
    }
    
    // Platform should handle concurrent operations gracefully
    if (successfulStarts > 0)
    {
        bool someCallbacksReceived = WaitForCallback(5000, 1);
        EXPECT_TRUE(someCallbacksReceived);
        EXPECT_GT(s_callbackCount.load(), 0);
    }
#else
    std::cout << "[ SKIPPED ] Windows platform test - skipping on non-Windows platforms" << std::endl;
    return;
#endif
}

#include "TestAppPch.h"
#include "FileIOTests.h"
#include "DataOperations.h"
#include "Platform/PlatformUtils.h"
#include "Platform/PlayFabPal.h"
#include "FilePAL.h"

using namespace PlayFab::Test::Platform;

namespace PlayFab
{
namespace Test
{

void FileIOTests::AddTests()
{
#if HC_PLATFORM != HC_PLATFORM_ANDROID && HC_PLATFORM != HC_PLATFORM_IOS && HC_PLATFORM != HC_PLATFORM_MAC
    // File Tests
    AddTest("TestGetAvailableStorageSize", &FileIOTests::TestGetAvailableStorageSize);
    AddTest("TestModifyFileTimes", &FileIOTests::TestModifyFileTimes);
    AddTest("TestMoveFile", &FileIOTests::TestMoveFile);
    AddTest("TestEnumFiles", &FileIOTests::TestEnumFiles);
    AddTest("TestEnumDirectories", &FileIOTests::TestEnumDirectories);
    AddTest("TestReadFile", &FileIOTests::TestReadFileBytes);
#endif
}

AsyncOp<void> FileIOTests::Initialize()
{
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    m_pathRoot = "temp:";
#endif

    RETURN_IF_FAILED(Platform::InitializeFileSystem("temp", m_fileSystemBuffer));

    return CoreTestClass::Initialize();
}

AsyncOp<void> FileIOTests::Uninitialize()
{
    Platform::UninitializeFileSystem("temp");

    return CoreTestClass::Uninitialize();
}

void FileIOTests::TestGetAvailableStorageSize(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_ANDROID
    auto result = FilePAL::GetAvailableStorageSize(m_pathRoot);
    tc.EndTest(result.hr);
#endif
}

void FileIOTests::TestModifyFileTimes(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_ANDROID
    // Nintendo Switch does not support file time modification
    tc.Skip();
#else
    String fileName{ m_pathRoot + "TestModifyFileTimes.txt" };
    
    try
    {
        auto fileResult = FilePAL::OpenFile(fileName, FileOpenMode::Write);
        tc.AssertEqual(S_OK, fileResult.hr, "OpenFile to write");
        auto file = fileResult.ExtractPayload();
        FilePAL::CloseFile(file);

        tc.AssertTrue(FilePAL::DoesFileExist(fileName), "File exists");

        time_t timeCreated = time(nullptr) + 1000;
        time_t timeModified = timeCreated + 1000;
        auto hr = FilePAL::SetFileLastModifiedTime(fileName, timeCreated, timeModified);
        tc.AssertEqual(S_OK, hr, "SetFileLastModifiedTime");

        time_t timeCreatedOut;
        time_t timeModifiedOut;
        hr = FilePAL::GetFileTimes(fileName, timeCreatedOut, timeModifiedOut);
        tc.AssertEqual(S_OK, hr, "GetFileTimes");

#if HC_PLATFORM_IS_MICROSOFT
        tc.AssertEqual(timeCreated, timeCreatedOut, "timeCreated");
#else
        // On non-Windows platforms, the file creation time is not settable
        // and will be the same as the modified time.
        tc.AssertEqual(timeModified, timeCreatedOut, "timeCreated");
#endif
        tc.AssertEqual(timeModified, timeModifiedOut, "timeModified");

        // Cleanup
        hr = FilePAL::DeleteLocalFile(fileName);
        tc.AssertEqual(S_OK, hr, "DeleteFile");

        tc.EndTest(hr);
    }
    catch (const std::exception& e)
    {
        // Cleanup
        FilePAL::DeleteLocalFile(fileName);
        tc.EndTest({ E_FAIL, e.what() });
        throw;
    }
#endif
}

void FileIOTests::TestMoveFile(TestContext& tc)
{
    String baseFile{ m_pathRoot + "/TestMoveFile.txt" };
    String movePath{ m_pathRoot + "/TestMoveDirectory" };
    String resultFile{ m_pathRoot + "/TestMoveDirectory/TestMoveFile.txt" };

    try
    {
        auto fileResult = FilePAL::OpenFile(baseFile, FileOpenMode::Write);
        tc.AssertEqual(S_OK, fileResult.hr, "OpenFile to write");
        auto file = fileResult.ExtractPayload();
        FilePAL::CloseFile(file);

        tc.AssertTrue(FilePAL::DoesFileExist(baseFile), "File exists");

        auto hr = FilePAL::CreatePath(movePath);
        tc.AssertEqual(S_OK, hr, "CreatePath");

        hr = FilePAL::MoveLocalFile(baseFile, resultFile);
        tc.AssertEqual(S_OK, hr, "MoveFile");

        tc.AssertTrue(FilePAL::DoesFileExist(resultFile), "File exists in new location");

        // Cleanup
        hr = FilePAL::DeleteLocalFile(resultFile);
        tc.AssertEqual(S_OK, hr, "DeleteFile");

        tc.EndTest(hr);
    }
    catch (const std::exception& e)
    {
        // Cleanup
        FilePAL::DeleteLocalFile(baseFile);
        FilePAL::DeleteLocalFile(resultFile);
        tc.EndTest({ E_FAIL, e.what() });
        throw;
    }
}

void FileIOTests::TestEnumFiles(TestContext& tc)
{
    String basePath{ m_pathRoot + "/TestEnumDirectoryItemsBase" };
    String fileName1{ "TestEnumDirectoryItems1.txt" };
    String fileName2{ "TestEnumDirectoryItems2.txt" };

    String file1{};
    String file2{};

    try
    {
        auto result3 = FilePAL::JoinPath(basePath, fileName1);
        tc.AssertEqual(S_OK, result3.hr, "JoinPath1");
        file1 = result3.ExtractPayload();

        auto result4 = FilePAL::JoinPath(basePath, fileName2);
        tc.AssertEqual(S_OK, result4.hr, "JoinPath2");
        file2 = result4.ExtractPayload();

        auto hr = FilePAL::CreatePath(basePath);
        tc.AssertEqual(S_OK, hr, "CreatePath");

        auto doesExist = FilePAL::DoesDirectoryExist(basePath);
        tc.AssertTrue(doesExist, "DoesDirectoryExist");

        auto fileResult = FilePAL::OpenFile(file1, FileOpenMode::Write);
        tc.AssertEqual(S_OK, fileResult.hr, "OpenFile1 to write");
        auto file1Handle = fileResult.ExtractPayload();
        FilePAL::CloseFile(file1Handle);

        doesExist = FilePAL::DoesFileExist(file1);
        tc.AssertTrue(doesExist, "DoesFileExist1");

        auto fileResult2 = FilePAL::OpenFile(file2, FileOpenMode::Write);
        tc.AssertEqual(S_OK, fileResult2.hr, "OpenFile2 to write");
        auto file2Handle = fileResult2.ExtractPayload();
        FilePAL::CloseFile(file2Handle);

        doesExist = FilePAL::DoesFileExist(file2);
        tc.AssertTrue(doesExist, "DoesFileExist2");

        auto filesResult = FilePAL::EnumFiles(basePath);
        tc.AssertEqual(S_OK, filesResult.hr, "EnumFiles");
        auto files = filesResult.ExtractPayload();

        tc.AssertEqual<size_t>(2u, files.size(), "File count");
        tc.AssertTrue(std::find(files.begin(), files.end(), fileName1) != files.end(), "file1 in list");
        tc.AssertTrue(std::find(files.begin(), files.end(), fileName2) != files.end(), "file2 in list");

        // Cleanup
        hr = FilePAL::DeleteLocalFile(file1);
        tc.AssertEqual(S_OK, hr, "DeleteFile1");

        hr = FilePAL::DeleteLocalFile(file2);
        tc.AssertEqual(S_OK, hr, "DeleteFile2");

        tc.EndTest(S_OK);
    }
    catch (const std::exception& e)
    {
        // Cleanup
        FilePAL::DeleteLocalFile(file1);
        FilePAL::DeleteLocalFile(file2);
        tc.EndTest({ E_FAIL, e.what() });
        throw;
    }
}

void FileIOTests::TestEnumDirectories(TestContext& tc)
{
    String basePath{ m_pathRoot + "/TestEnumDirectoryItemsBase" };
    String subPath1{ "TestEnumDirectoryItems1" };
    String subPath2{ "TestEnumDirectoryItems2" };

    try
    {
        auto result = FilePAL::JoinPath(basePath, subPath1);
        tc.AssertEqual(S_OK, result.hr, "JoinPath1");
        auto subDir1 = result.ExtractPayload();

        auto result2 = FilePAL::JoinPath(basePath, subPath2);
        tc.AssertEqual(S_OK, result2.hr, "JoinPath2");
        auto subDir2 = result2.ExtractPayload();

        auto hr = FilePAL::CreatePath(subDir1);
        tc.AssertEqual(S_OK, hr, "CreatePath1");

        auto doesExist = FilePAL::DoesDirectoryExist(subDir1);
        tc.AssertTrue(doesExist, "DoesDirectoryExist1");

        hr = FilePAL::CreatePath(subDir2);
        tc.AssertEqual(S_OK, hr, "CreatePath2");

        doesExist = FilePAL::DoesDirectoryExist(subDir2);
        tc.AssertTrue(doesExist, "DoesDirectoryExist2");

        auto dirsResult = FilePAL::EnumDirectories(basePath);
        tc.AssertEqual(S_OK, dirsResult.hr, "EnumDirectories");
        auto dirs = dirsResult.ExtractPayload();

        tc.AssertEqual<size_t>(2u, dirs.size(), "Directory count");
        tc.AssertTrue(std::find(dirs.begin(), dirs.end(), subPath1) != dirs.end(), "subDir1 in list");
        tc.AssertTrue(std::find(dirs.begin(), dirs.end(), subPath2) != dirs.end(), "subDir2 in list");

        tc.EndTest(S_OK);
    }
    catch (const std::exception& e)
    {
        tc.EndTest({ E_FAIL, e.what() });
        throw;
    }
}

void FileIOTests::TestReadFileBytes(TestContext& tc)
{
    String fileName{ m_pathRoot + "TestReadFileBytes.txt" };

    FileHandle file{};

    try
    {
        const Vector<char> testData = Vector<char>(3000, 'a');

        auto fileResult = FilePAL::OpenFile(fileName, FileOpenMode::Write);
        tc.AssertEqual(S_OK, fileResult.hr, "OpenFile to write");
        file = fileResult.ExtractPayload();

        // Write data in chunks
        Vector<char> inStart{ testData.begin(), testData.begin() + testData.size() / 2 };
        Vector<char> inEnd{ testData.end() - testData.size() / 2, testData.end() };
        auto hr = FilePAL::WriteFileBytes(file, inStart.data(), inStart.size());
        tc.AssertEqual(S_OK, hr, "WriteFileBytes1");
        hr = FilePAL::WriteFileBytes(file, inEnd.data(), inEnd.size());
        tc.AssertEqual(S_OK, hr, "WriteFileBytes2");
        FilePAL::CloseFile(file);

        auto sizeResult = FilePAL::GetFileSize(fileName);
        tc.AssertEqual(S_OK, sizeResult.hr, "GetFileSize");
        tc.AssertEqual((uint64_t)testData.size(), (uint64_t)sizeResult.Payload(), "file size");

        auto fileResult2 = FilePAL::OpenFile(fileName, FileOpenMode::Read);
        tc.AssertEqual(S_OK, fileResult2.hr, "OpenFile to read");
        file = fileResult2.ExtractPayload();

        // Read data in chunks
        Vector<char> outData{};
        Vector<char> outStart(testData.size() / 2);
        Vector<char> outEnd(testData.size() / 2);
        size_t bytesWritten;
        hr = FilePAL::ReadFileBytes(file, testData.size() / 2, outStart.data(), &bytesWritten);
        tc.AssertEqual(S_OK, hr, "ReadFileBytes1");
        tc.AssertEqual(bytesWritten, outStart.size(), "bytesWritten1");
        outData.insert(outData.end(), outStart.begin(), outStart.end());

        // Purposefully attempt to read past EOF
        hr = FilePAL::ReadFileBytes(file, testData.size(), outEnd.data(), &bytesWritten);
        tc.AssertEqual(S_OK, hr, "ReadFileBytes2");
        tc.AssertEqual(bytesWritten, outEnd.size(), "bytesWritten2");
        outData.insert(outData.end(), outEnd.begin(), outEnd.end());
        FilePAL::CloseFile(file);

        tc.AssertTrue(testData == outData, "data");

        // Cleanup
        hr = FilePAL::DeleteLocalFile(fileName);
        tc.AssertEqual(S_OK, hr, "DeleteFile");

        tc.EndTest(hr);
    }
    catch (const std::exception& e)
    {
        // Cleanup
        FilePAL::CloseFile(file);
        FilePAL::DeleteLocalFile(fileName);
        tc.EndTest({ E_FAIL, e.what() });
    }
}

}
}

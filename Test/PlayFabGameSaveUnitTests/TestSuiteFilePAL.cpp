// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// REGRESSION TESTS for bug 4544470 (PLAYFAB SAVES - upload reported 0x80070057 E_INVALIDARG).
//
// The defect:
//   FilePAL reported every local file failure as E_INVALIDARG. Two sites did it:
//
//     FilePAL::OpenFile     - RETURN_HR_IF_FALSE(E_INVALIDARG, fileHandle->file)
//     FilePAL::GetFileSize  - RETURN_HR_IF_FALSE(E_INVALIDARG, DoesFileExist(filePath))
//
//   ...and DoesFileExist decided existence by OPENING the file:
//
//     std::ifstream file(path); return file.good();
//
//   so a file that existed but was momentarily held open by another writer (the title's own save
//   code, anti-virus, a platform cloud-sync agent) was reported as "does not exist".
//
//   GameSave surfaces these codes verbatim out of PFGameSaveFilesUploadWithUiResult, so a title
//   saw "you passed a bad argument" for what was actually a transient, retryable local-IO
//   condition, and had no way to tell the two apart.
//
// What these tests pin down:
//   1. A locked file is still reported as existing, and its size is still readable.
//   2. Every failure path reports a real file error, never E_INVALIDARG.
//   3. E_INVALIDARG is still returned for genuine argument errors (malformed paths).

#include "pch.h"
#include "actions.h"
#include "MemoryManager.h"

// The SDK's FilePAL, not the test app's own FilePALTestApp helper that pch.h pulls in.
#include <FilePAL.h>

using namespace PlayFab;

namespace
{

// HRESULTs the fixed FilePAL is expected to produce. Spelled out so a regression that reverts to
// E_INVALIDARG (or to a bare E_FAIL) fails loudly rather than silently widening what callers see.
constexpr HRESULT kFileNotFound = __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
constexpr HRESULT kSharingViolation = __HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION);
constexpr HRESULT kAccessDenied = __HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);

// Holds a file open with no sharing at all, the way a game's save writer or an AV scanner does
// while it has the file. This is the condition that used to make DoesFileExist answer "missing".
class ScopedFileLock
{
public:
    explicit ScopedFileLock(const std::wstring& path)
    {
        m_handle = CreateFileW(
            path.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,                  // deny all sharing
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr);
    }

    ~ScopedFileLock()
    {
        Release();
    }

    void Release()
    {
        if (m_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
    }

    bool IsHeld() const { return m_handle != INVALID_HANDLE_VALUE; }

private:
    HANDLE m_handle{ INVALID_HANDLE_VALUE };
};

} // anonymous namespace

class TestSuiteFilePAL : public testing::Test
{
protected:
    void SetUp() override
    {
        m_root = std::filesystem::temp_directory_path() /
            ("pfgs_filepal_" + std::to_string(GetCurrentProcessId()) + "_" +
             std::to_string(reinterpret_cast<uintptr_t>(this)));

        std::error_code ec;
        std::filesystem::create_directories(m_root, ec);
        ASSERT_FALSE(ec) << "could not create temp dir " << m_root.string();
    }

    void TearDown() override
    {
        std::error_code ec;
        std::filesystem::remove_all(m_root, ec);
    }

    // Creates a file with known contents and returns its path in both flavors the test needs.
    std::filesystem::path MakeFile(const char* name, const std::string& contents)
    {
        std::filesystem::path p = m_root / name;
        std::ofstream f(p, std::ios::binary);
        f << contents;
        f.close();
        EXPECT_TRUE(std::filesystem::exists(p));
        return p;
    }

    static String ToPalPath(const std::filesystem::path& p)
    {
        return String(p.string().c_str());
    }

    std::filesystem::path m_root;
};

// ---------------------------------------------------------------------------------------------
// DoesFileExist
// ---------------------------------------------------------------------------------------------

// THE core regression. Before the fix DoesFileExist opened the file, so this returned false and
// every existence-guarded caller (GetFileSize above all) rejected a perfectly healthy save file.
TEST_F(TestSuiteFilePAL, DoesFileExist_LockedFileIsStillReportedAsExisting)
{
    std::filesystem::path p = MakeFile("locked.bin", "hello");

    ScopedFileLock lock(p.wstring());
    ASSERT_TRUE(lock.IsHeld()) << "test could not acquire the exclusive lock it depends on";

    EXPECT_TRUE(FilePAL::DoesFileExist(ToPalPath(p)))
        << "a file held open by another writer must still be reported as existing";
}

TEST_F(TestSuiteFilePAL, DoesFileExist_MissingFileIsReportedMissing)
{
    EXPECT_FALSE(FilePAL::DoesFileExist(ToPalPath(m_root / "not_here.bin")));
}

// Directories are not files. The old ifstream implementation also rejected them (open fails with
// EACCES); the replacement must not start accepting them.
TEST_F(TestSuiteFilePAL, DoesFileExist_DirectoryIsNotAFile)
{
    std::filesystem::path sub = m_root / "subdir";
    std::error_code ec;
    std::filesystem::create_directories(sub, ec);
    ASSERT_FALSE(ec);

    EXPECT_FALSE(FilePAL::DoesFileExist(ToPalPath(sub)));
    EXPECT_TRUE(FilePAL::DoesDirectoryExist(ToPalPath(sub)));
}

TEST_F(TestSuiteFilePAL, DoesFileExist_PresentFile)
{
    std::filesystem::path p = MakeFile("present.bin", "data");
    EXPECT_TRUE(FilePAL::DoesFileExist(ToPalPath(p)));
}

// ---------------------------------------------------------------------------------------------
// GetFileSize
// ---------------------------------------------------------------------------------------------

// A lock blocks reading the CONTENTS, not the metadata. The upload path calls GetFileSize before
// it opens the file, and failing here is what produced the E_INVALIDARG in the bug report.
TEST_F(TestSuiteFilePAL, GetFileSize_LockedFileStillReportsItsSize)
{
    const std::string contents = "0123456789";
    std::filesystem::path p = MakeFile("locked_size.bin", contents);

    ScopedFileLock lock(p.wstring());
    ASSERT_TRUE(lock.IsHeld());

    Result<uint64_t> result = FilePAL::GetFileSize(ToPalPath(p));
    ASSERT_EQ(S_OK, result.hr) << "GetFileSize must not fail merely because the file is locked";
    EXPECT_EQ(static_cast<uint64_t>(contents.size()), result.Payload());
}

TEST_F(TestSuiteFilePAL, GetFileSize_MissingFileReportsFileNotFoundNotInvalidArg)
{
    Result<uint64_t> result = FilePAL::GetFileSize(ToPalPath(m_root / "not_here.bin"));

    EXPECT_NE(E_INVALIDARG, result.hr) << "a missing file is not an argument error";
    EXPECT_EQ(kFileNotFound, result.hr);
}

TEST_F(TestSuiteFilePAL, GetFileSize_PresentFileReportsCorrectSize)
{
    const std::string contents = "abcdefghijklmnop";
    std::filesystem::path p = MakeFile("size.bin", contents);

    Result<uint64_t> result = FilePAL::GetFileSize(ToPalPath(p));
    ASSERT_EQ(S_OK, result.hr);
    EXPECT_EQ(static_cast<uint64_t>(contents.size()), result.Payload());
}

// ---------------------------------------------------------------------------------------------
// OpenFile
// ---------------------------------------------------------------------------------------------

TEST_F(TestSuiteFilePAL, OpenFile_MissingFileReportsFileNotFoundNotInvalidArg)
{
    Result<FileHandle> result = FilePAL::OpenFile(ToPalPath(m_root / "not_here.bin"), FileOpenMode::Read);

    EXPECT_NE(E_INVALIDARG, result.hr) << "a missing file is not an argument error";
    EXPECT_EQ(kFileNotFound, result.hr);
}

// The exact shape of the SteamDeck repro: the transfer opens a save file the title is still
// holding. It must come back as a sharing/access failure the caller can classify as retryable,
// never as E_INVALIDARG.
TEST_F(TestSuiteFilePAL, OpenFile_LockedFileReportsSharingFailureNotInvalidArg)
{
    std::filesystem::path p = MakeFile("locked_open.bin", "payload");

    ScopedFileLock lock(p.wstring());
    ASSERT_TRUE(lock.IsHeld());

    Result<FileHandle> result = FilePAL::OpenFile(ToPalPath(p), FileOpenMode::Read);

    ASSERT_TRUE(FAILED(result.hr)) << "opening a deny-all-sharing file should fail";
    EXPECT_NE(E_INVALIDARG, result.hr) << "a locked file is not an argument error";
    EXPECT_TRUE(result.hr == kSharingViolation || result.hr == kAccessDenied)
        << "expected a sharing/access failure, got 0x" << std::hex << result.hr;
}

// The other half of the OpenFile contract, and the gap that made the behavior implicit: an
// argument that cannot name a file at all is a caller error and must stay E_INVALIDARG rather than
// degrading to a generic E_FAIL. An empty path reaches the CRT as EINVAL with no Win32 last error
// set, so it was the one input that fell through every mapping.
TEST_F(TestSuiteFilePAL, OpenFile_EmptyPathIsStillInvalidArg)
{
    Result<FileHandle> result = FilePAL::OpenFile(String(""), FileOpenMode::Read);

    ASSERT_TRUE(FAILED(result.hr));
    EXPECT_EQ(E_INVALIDARG, result.hr)
        << "an unusable argument must not degrade to E_FAIL, got 0x" << std::hex << result.hr;
}

// The malformed-path half of the contract, for the entry point that previously left it implicit.
//
// Unlike MoveLocalFile / CreatePath, OpenFile has no IsValidPath() pre-check - it hands the path
// straight to the CRT. It still answers E_INVALIDARG because a name the filesystem cannot
// represent surfaces as EINVAL, which is mapped as an argument error. Worth pinning precisely
// because the route there is incidental rather than designed: it depends on the CRT's choice of
// errno, so a toolchain change could silently turn this into a generic E_FAIL.
//
// Note the parent directory must exist for this to test what it claims. With a missing parent the
// CRT reports ERROR_PATH_NOT_FOUND for the *directory* and never evaluates the filename, so the
// test would pass for the wrong reason.
TEST_F(TestSuiteFilePAL, OpenFile_MalformedPathIsStillInvalidArg)
{
    ASSERT_TRUE(std::filesystem::exists(m_root)) << "parent must exist or this tests the wrong thing";

    Result<FileHandle> result = FilePAL::OpenFile(
        String((m_root / "bad<>|?*.bin").string().c_str()), FileOpenMode::Read);

    ASSERT_TRUE(FAILED(result.hr));
    EXPECT_NE(E_FAIL, result.hr) << "must be classified, not a generic failure";
    EXPECT_EQ(E_INVALIDARG, result.hr)
        << "expected an argument error, got 0x" << std::hex << result.hr;
}

// Once the writer lets go, the very same path opens. This is what makes the new
// E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE a genuinely retryable condition rather than a fatal one.
TEST_F(TestSuiteFilePAL, OpenFile_SucceedsOnceTheLockIsReleased){
    std::filesystem::path p = MakeFile("relock.bin", "payload");

    ScopedFileLock lock(p.wstring());
    ASSERT_TRUE(lock.IsHeld());
    EXPECT_TRUE(FAILED(FilePAL::OpenFile(ToPalPath(p), FileOpenMode::Read).hr));

    lock.Release();

    Result<FileHandle> result = FilePAL::OpenFile(ToPalPath(p), FileOpenMode::Read);
    ASSERT_EQ(S_OK, result.hr);

    FileHandle handle = result.ExtractPayload();
    FilePAL::CloseFile(handle);
}

TEST_F(TestSuiteFilePAL, OpenFile_ReadThenWriteRoundTrip)
{
    std::filesystem::path p = m_root / "roundtrip.bin";

    Result<FileHandle> writeResult = FilePAL::OpenFile(ToPalPath(p), FileOpenMode::Write);
    ASSERT_EQ(S_OK, writeResult.hr);
    FileHandle writeHandle = writeResult.ExtractPayload();
    const char payload[] = "round trip";
    ASSERT_EQ(S_OK, FilePAL::WriteFileBytes(writeHandle, payload, sizeof(payload) - 1));
    FilePAL::CloseFile(writeHandle);

    Result<uint64_t> sizeResult = FilePAL::GetFileSize(ToPalPath(p));
    ASSERT_EQ(S_OK, sizeResult.hr);
    EXPECT_EQ(sizeof(payload) - 1, sizeResult.Payload());
}

// ---------------------------------------------------------------------------------------------
// MoveLocalFile
// ---------------------------------------------------------------------------------------------

TEST_F(TestSuiteFilePAL, MoveLocalFile_MissingSourceIsNotInvalidArg)
{
    HRESULT hr = FilePAL::MoveLocalFile(
        ToPalPath(m_root / "not_here.bin"),
        ToPalPath(m_root / "dest.bin"));

    ASSERT_TRUE(FAILED(hr));
    EXPECT_NE(E_INVALIDARG, hr) << "a missing source file is not an argument error";
}

// The other half of the contract: E_INVALIDARG must still mean what it says. A destination path
// containing characters that are illegal on this platform is a real argument error, and the fix
// must not have flattened that distinction away.
TEST_F(TestSuiteFilePAL, MoveLocalFile_MalformedDestinationIsStillInvalidArg)
{
    std::filesystem::path src = MakeFile("movable.bin", "data");

    HRESULT hr = FilePAL::MoveLocalFile(
        ToPalPath(src),
        String((m_root / "bad").string().c_str()) + "<>|?*.bin");

    EXPECT_EQ(E_INVALIDARG, hr);
}

TEST_F(TestSuiteFilePAL, MoveLocalFile_SucceedsForAHealthyFile)
{
    std::filesystem::path src = MakeFile("src.bin", "data");
    std::filesystem::path dest = m_root / "dest.bin";

    ASSERT_EQ(S_OK, FilePAL::MoveLocalFile(ToPalPath(src), ToPalPath(dest)));
    EXPECT_FALSE(FilePAL::DoesFileExist(ToPalPath(src)));
    EXPECT_TRUE(FilePAL::DoesFileExist(ToPalPath(dest)));
}

// ---------------------------------------------------------------------------------------------
// CreatePath / DeletePath still validate their arguments
// ---------------------------------------------------------------------------------------------

TEST_F(TestSuiteFilePAL, CreatePath_MalformedPathIsStillInvalidArg)
{
    EXPECT_EQ(E_INVALIDARG, FilePAL::CreatePath(String((m_root / "bad<>|?*").string().c_str())));
}

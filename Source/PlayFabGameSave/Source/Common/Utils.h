// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "PFGameSaveFilesForDebug.h"

namespace PlayFab
{
namespace GameSave
{

struct ISchedulableTask;

// Folder name used by the mock service to store test data.
// Also excluded during local-state folder scanning to avoid mixing mock data with real saves.
constexpr const char* kMockSaveFolderName = "MockPFGameSave";

// Prefix of the per-attempt upload staging directories created inside the cloudsync folder.
// One directory per upload attempt, so a retry can never delete the zips of an attempt whose
// transfers are still alive.
constexpr const char* kUploadStagingFolderPrefix = "up-";

String CreateGUID();
// Unit tests only (core unit test plan H-1, U25): the next CreateGUID seeds the generator again, as
// in a newly started process.
void ResetGuidGeneratorForTest();
String RemoveRootPath(String folder, const String& rootFolder);
HRESULT JoinPathHelper(_In_ const String& pathA, _In_ const String& pathB, _Out_ String& pathResult);
HRESULT ReadEntireFile(_In_ const String& filePath, _Out_ Vector<char>& fileBuffer);
HRESULT WriteEntireFile(_In_ const String& filePath, _In_ const Vector<char>& fileBuffer);
HRESULT GetCloudSyncFolder(_In_ const String& saveFolder, _Out_ String& cloudSyncFolder);
void CleanupTempCloudSyncFiles();
HRESULT CreateUploadStagingFolder(_In_ const String& saveFolder, _Out_ String& stagingFolder);
void DeleteUploadStagingFolder(_In_ const String& stagingFolder);
void SweepUploadStagingFolders(_In_ const String& saveFolder);

// Re-reports a local-file access failure as E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE. Non-file
// failures are returned unchanged.
HRESULT MapLocalFileFailure(HRESULT hr);

// StrCpy for the UTF-8 strings a title shows (PFGameSaveDescriptor fields: device names, types and
// IDs, descriptions, often written by another device): when `source` doesn't fit, the copy ends at
// the last whole character instead of in the middle of a multi-byte sequence. Always NUL-terminated.
inline void StrCpyUtf8(_Out_writes_z_(destinationSize) char* destination, _In_ size_t destinationSize, _In_z_ const char* source)
{
    StrCpy(destination, destinationSize, source);
    if (destinationSize == 0)
    {
        return;
    }
    const size_t length = strnlen(destination, destinationSize);
    if (source[length] == '\0')
    {
        return; // it fit
    }
    // Truncated: find the lead byte of the last sequence and drop it if it's incomplete.
    size_t lead = length;
    while (lead > 0 && (static_cast<unsigned char>(destination[lead - 1]) & 0xC0) == 0x80)
    {
        lead--;
    }
    if (lead == 0)
    {
        return;
    }
    const unsigned char first = static_cast<unsigned char>(destination[lead - 1]);
    const size_t needed = (first & 0x80) == 0x00 ? 1 : (first & 0xE0) == 0xC0 ? 2 : (first & 0xF0) == 0xE0 ? 3 : (first & 0xF8) == 0xF0 ? 4 : 1;
    if (length - (lead - 1) < needed)
    {
        destination[lead - 1] = '\0';
    }
}
HRESULT EnsureGameStorageMarker(_In_ const String& saveFolder);

String GetLocalDeviceID(const String& saveFolder);
// Like GetLocalDeviceID, but fails (instead of asserting) once the SDK has been uninitialized.
HRESULT TryGetLocalDeviceID(const String& saveFolder, String& deviceId);
void EnsureDeviceIdPersisted(const String& saveFolder);
bool GetForceOutOfStorageError();
bool GetForceSyncFailedError();
// Debug fault injection (PFGameSaveFilesSetFaultInjectionForDebug): the action configured for
// `point` (None when unset); hr is the failure to report for Fail. Counts a hit.
// Until a fault has been set in this process it returns None at the cost of one atomic load (no
// state lookup, string, lock or map search): the call sites are on hot paths of shipping builds.
// Release builds always return None (no fault injection).
PFGameSaveFaultAction CheckFaultPointForDebug(_In_z_ const char* point, _Out_ HRESULT& hr);
// Records that a fault has been set (GameSaveGlobalState::SetDebugFault): from then on, for the rest of
// the process, every fault check takes the full path. Never cleared, so behavior with faults is
// unchanged.
void NoteFaultInjectionSetForDebug() noexcept;
bool IsAnyFaultInjectionSetForDebug() noexcept;
// PauseOnce: true while the one call the point caught is held (no hit is counted).
bool IsFaultPointHoldingForDebug(_In_z_ const char* point);
// A debug fault point inside a step's stage machine (core unit test plan H-5). Returns true when the
// stage must stop at `point` and return `result`: Fail gives the configured hr, Abandon gives E_ABORT
// (and sets *abandoned), Pause reschedules `task` to poll until resumed and gives pauseResult (what
// the stage returns while it waits). Returns false (continue) when the point isn't armed.
bool StopAtStageFaultPointForDebug(_In_z_ const char* point, _In_ ISchedulableTask& task, _In_ HRESULT pauseResult, _Out_ HRESULT& result, _Out_opt_ bool* abandoned = nullptr);
bool GetForceNullPendingManifest();
void ClearForceNullPendingManifest();
bool GetWriteManifestsToDisk();
int64_t GetDebugManifestOffset();
int64_t GetDebugPerPlayerQuotaOverride(); // 0 = no override

PlayFab::GameSaveWrapper::ManifestStatus ConvertToManifestStatusEnum(String str);
String ConvertToManifestStatusString(PlayFab::GameSaveWrapper::ManifestStatus n);
int64_t StringToInt64(String str);
uint64_t StringToUint64(String str);
String Uint64ToString(uint64_t n);


class ScopeTracer
{
public:
    ScopeTracer(const String& traceMessage);
    ~ScopeTracer();
private:
    String m_traceMessage;
};

#if defined(_DEBUG)
class SingleThreadProviderValidation
{
public:
    void Set();
    void Clear();
    void AssertUponSchedule();

private:
    String m_activeThreadId;
    String m_pendingScheduleThreadId;
};


class SingleThreadProviderValidationScope
{
public:
    SingleThreadProviderValidationScope(SingleThreadProviderValidation& singleThreadProvider);
    ~SingleThreadProviderValidationScope();

private:
    SingleThreadProviderValidation& m_singleThreadProvider;
};

// Marks the current thread as waking a task parked on a prompt (UICallbackManager: an answer,
// XAsyncCancel, uninitialize). Those wakes come from whichever thread answers or cancels, and a prompt
// can be raised inside the provider's DoWork, so the wake can land while that DoWork is still
// returning. The parked task touches no state until its next DoWork (which takes the sync mutex), so
// SingleThreadProviderValidation::AssertUponSchedule doesn't check (or record) these schedules.
class ScopedUiWakeForDebug
{
public:
    ScopedUiWakeForDebug() noexcept;
    ~ScopedUiWakeForDebug() noexcept;
    ScopedUiWakeForDebug(const ScopedUiWakeForDebug&) = delete;
    ScopedUiWakeForDebug& operator=(const ScopedUiWakeForDebug&) = delete;
};
#endif

} // namespace GameSave
} // namespace PlayFab
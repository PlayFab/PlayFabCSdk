// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

// Folder name used by the mock service to store test data.
// Also excluded during local-state folder scanning to avoid mixing mock data with real saves.
constexpr const char* kMockSaveFolderName = "MockPFGameSave";

String CreateGUID();
String RemoveRootPath(String folder, const String& rootFolder);
HRESULT JoinPathHelper(_In_ const String& pathA, _In_ const String& pathB, _Out_ String& pathResult);
HRESULT ReadEntireFile(_In_ const String& filePath, _Out_ Vector<char>& fileBuffer);
HRESULT WriteEntireFile(_In_ const String& filePath, _In_ const Vector<char>& fileBuffer);
HRESULT GetCloudSyncFolder(_In_ const String& saveFolder, _Out_ String& cloudSyncFolder);
void CleanupTempCloudSyncFiles();
HRESULT EnsureGameStorageMarker(_In_ const String& saveFolder);

String GetLocalDeviceID(const String& saveFolder);
void EnsureDeviceIdPersisted(const String& saveFolder);
bool GetForceOutOfStorageError();
bool GetForceSyncFailedError();
bool GetForceNullPendingManifest();
void ClearForceNullPendingManifest();
bool GetWriteManifestsToDisk();
int64_t GetDebugManifestOffset();

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
#endif

} // namespace GameSave
} // namespace PlayFab
// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

String CreateGUID();
String RemoveRootPath(String folder, const String& rootFolder);
HRESULT JoinPathHelper(_In_ const String& pathA, _In_ const String& pathB, _Out_ String& pathResult);
HRESULT ReadEntireFile(_In_ const String& filePath, _Out_ Vector<char>& fileBuffer);
HRESULT WriteEntireFile(_In_ const String& filePath, _In_ const Vector<char>& fileBuffer);

String GetLocalDeviceID(const String& saveFolder);
bool GetForceOutOfStorageError();
bool GetForceSyncFailedError();
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

#if _DEBUG
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
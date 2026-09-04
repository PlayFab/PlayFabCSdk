#pragma once
// XGameSaveEnumAPI — Flat C DLL wrapping the GRTS XGameSaveProviderEnumerator COM interface.
// Designed for P/Invoke from the C# GameTestController.

#include <stdint.h>
#include <windows.h>

#ifdef XGAMESAVEENUMAPI_EXPORTS
#define XGSE_API __declspec(dllexport)
#else
#define XGSE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XGameSaveEnumContext* XGameSaveEnumHandle;

typedef struct XGameSaveProviderStatus
{
    wchar_t  Xuid[64];
    wchar_t  Scid[16];
    wchar_t  Aumid[256];
    wchar_t  Location[MAX_PATH];
    uint64_t TotalBytes;
    uint64_t UploadedBytes;
    FILETIME LastModified;
    HRESULT  LastSyncHr;
    int32_t  InSync;
    int32_t  IsActive;
} XGameSaveProviderStatus;

// Lifecycle
XGSE_API HRESULT XGameSaveEnumCreate(_Out_ XGameSaveEnumHandle* handle);
XGSE_API void    XGameSaveEnumClose(_In_ XGameSaveEnumHandle handle);

// Re-query GRTS for latest state (call before reads when polling)
XGSE_API HRESULT XGameSaveEnumRefresh(_In_ XGameSaveEnumHandle handle);

// Query all providers
XGSE_API HRESULT XGameSaveEnumGetCount(_In_ XGameSaveEnumHandle handle, _Out_ uint32_t* count);
XGSE_API HRESULT XGameSaveEnumGetProviders(
    _In_ XGameSaveEnumHandle handle,
    uint32_t maxCount,
    _Out_writes_to_(maxCount, *actualCount) XGameSaveProviderStatus* providers,
    _Out_ uint32_t* actualCount);

// Find status for a specific xuid+scid.
// Returns S_OK if found, HRESULT_FROM_WIN32(ERROR_NOT_FOUND) if no match.
XGSE_API HRESULT XGameSaveEnumFindProvider(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid,
    _Out_ XGameSaveProviderStatus* status);

// Cleanup operations
XGSE_API HRESULT XGameSaveEnumDeleteLocalData(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid);
XGSE_API HRESULT XGameSaveEnumDeleteLocalAndCloudData(
    _In_ XGameSaveEnumHandle handle,
    _In_z_ const wchar_t* xuid,
    _In_z_ const wchar_t* scid);

#ifdef __cplusplus
}
#endif

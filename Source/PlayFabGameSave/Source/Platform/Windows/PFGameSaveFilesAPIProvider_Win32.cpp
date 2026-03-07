// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "PFGameSaveFilesAPIProvider_Win32.h"
#include "SetSaveDescriptionProvider.h"
#include <shlobj.h>
#include <knownfolders.h>

namespace PlayFab
{
namespace GameSave
{

// Disallowed environment-rooted exact paths
static const char* c_disallowedExactEnv[] =
{
    "%AppData%",
    "%LocalAppData%",
    "%ProgramFiles%",
    "%ProgramFiles(x86)%"
};

// Environment / well-known prefixes (path starts-with) that are disallowed.
static const char* c_disallowedPrefixEnv[] =
{
    "%AppData%\\Microsoft",
    "%LocalAppData%\\Microsoft",
    "%CommonProgramFiles%",
    "%CommonProgramFiles(x86)%",
    "%DriverData%",
    "%ProgramData%",
    "%SystemRoot%",
    "%ProgramFiles%\\WindowsApps",
    "WindowsApps",
    "%ONEDRIVECONSUMER%",
    "%ONEDRIVECOMMERCIAL%"
};

// Approximate known folders via environment patterns
// These expand to canonical locations we also want to block if used directly.
static const char* c_disallowedKnownFolderLikeEnv[] =
{
    "%USERPROFILE%",
    "%USERPROFILE%\\Desktop",
    "%USERPROFILE%\\Documents",
    "%USERPROFILE%\\Downloads",
    "%LOCALAPPDATA%",
    "%LOCALAPPDATA%\\Low",
    "%ProgramFiles%",
    "%ProgramFiles(x86)%",
    "%ProgramW6432%",
    "%PUBLIC%",
    "%APPDATA%",
    "%ProgramData%",
    "%SystemRoot%",
    "%SystemRoot%\\System32",
    "%SystemRoot%\\SysWOW64"
};

// Case-insensitive equality
static bool PathEqualsUtf8(const std::string& a, const std::string& b)
{
    return _stricmp(a.c_str(), b.c_str()) == 0;
}

// Case-insensitive prefix check; boundary aware
static bool PathHasPrefixUtf8(const std::string& path, const std::string& prefix)
{
    size_t n = prefix.size();
    if (path.size() < n) return false;
    if (_strnicmp(path.c_str(), prefix.c_str(), n) != 0) return false;
    return path.size() == n || path[n] == '\\';
}

// Removed <PathCch.h> and pathcch.lib dependency. Implementing lightweight canonicalizer below.
// Removed <shlwapi.h>; deprecated PathCanonicalizeA no longer used.
// Minimal internal replacement for PathCchCanonicalize to avoid dependency on pathcch.lib.
// This handles the subset of behaviors required by this file:
//  * Accepts absolute drive paths (e.g. C:\foo\..\bar) and UNC paths (\\server\share\folder\..)
//  * Normalizes separators to backslashes
//  * Collapses repeated separators (except preserves leading UNC double backslash)
//  * Eliminates '.' segments
//  * Resolves '..' segments without crossing above the root (returns E_INVALIDARG if that would occur)
//  * Removes trailing backslash except when path is a root (e.g. C:\ or \\server\share\)
// It purposefully does NOT implement Win32 extended path prefixes (\\?\) or relative path canonicalization.
// If more completeness is required in future, consider reinstating PathCchCanonicalize or expanding logic.
static HRESULT MyPathCchCanonicalize(_Out_writes_(cchPathOut) wchar_t* pszPathOut, size_t cchPathOut, _In_ const wchar_t* pszPathIn)
{
    if (!pszPathOut || !pszPathIn || cchPathOut == 0)
    {
        return E_INVALIDARG;
    }

    std::wstring in(pszPathIn);
    if (in.empty())
    {
        return E_INVALIDARG;
    }

    // Normalize forward slashes to backslashes
    for (auto& ch : in) if (ch == L'/') ch = L'\\';

    // Reject extended path prefix (not supported in this minimal impl)
    if (in.rfind(L"\\\\?\\", 0) == 0)
    {
        return E_INVALIDARG; // unsupported scenario for our needs
    }

    std::wstring root; // Will include trailing backslash
    size_t pos = 0;
    bool isUnc = false;
    if (in.size() >= 2 && iswalpha(in[0]) && in[1] == L':')
    {
        // Drive path
        root.assign(in.substr(0, 2));
        if (in.size() >= 3 && (in[2] == L'\\'))
        {
            root.push_back(L'\\');
            pos = 3;
        }
        else
        {
            root.push_back(L'\\'); // ensure drive root ends with backslash
            pos = 2;
        }
    }
    else if (in.rfind(L"\\\\", 0) == 0)
    {
        // UNC path: \\server\share\...
        isUnc = true;
        size_t first = in.find(L'\\', 2);
        if (first == std::wstring::npos) return E_INVALIDARG;
        size_t second = in.find(L'\\', first + 1);
        if (second == std::wstring::npos)
        {
            // Path like \\server\share (add trailing backslash as root)
            root = in + L'\\';
            pos = in.size();
        }
        else
        {
            root = in.substr(0, second + 1); // include trailing backslash
            pos = second + 1;
        }
    }
    else
    {
        return E_INVALIDARG; // not an absolute path
    }

    std::vector<std::wstring> segments;
    while (pos < in.size())
    {
        size_t next = in.find(L'\\', pos);
        std::wstring segment = (next == std::wstring::npos) ? in.substr(pos) : in.substr(pos, next - pos);
        pos = (next == std::wstring::npos) ? in.size() : next + 1;
        if (segment.empty() || segment == L".")
        {
            continue; // skip redundant
        }
        if (segment == L"..")
        {
            if (segments.empty())
            {
                // Attempt to go above root -> invalid
                return E_INVALIDARG;
            }
            segments.pop_back();
            continue;
        }
        segments.push_back(std::move(segment));
    }

    std::wstring out = root;
    for (size_t i = 0; i < segments.size(); ++i)
    {
        out.append(segments[i]);
        if (i + 1 < segments.size()) out.push_back(L'\\');
    }

    // Remove trailing backslash if not root and not UNC root
    if (!segments.empty() && !out.empty() && out.back() == L'\\')
    {
        out.pop_back();
    }

    if (out.size() + 1 > cchPathOut) // include null terminator
    {
        return E_INVALIDARG;
    }

    wcscpy_s(pszPathOut, cchPathOut, out.c_str());
    return S_OK;
}

// UTF-8 <-> wide helpers (localized)
static std::wstring Utf8ToWide(const char* s)
{
    if (!s) return std::wstring();
    int wlen = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
    if (wlen <= 0) return std::wstring();
    // Allocate space for characters + null terminator, then trim null.
    std::wstring w(static_cast<size_t>(wlen), L'\0');
    if (MultiByteToWideChar(CP_UTF8, 0, s, -1, w.data(), wlen) == 0)
    {
        return std::wstring();
    }
    if (!w.empty())
    {
        w.resize(w.size() - 1); // drop terminating null (string maintains its own)
    }
    return w;
}
static std::string WideToUtf8(const wchar_t* ws)
{
    if (!ws) return std::string();
    int len = WideCharToMultiByte(CP_UTF8, 0, ws, -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return std::string();
    std::string s(static_cast<size_t>(len), '\0');
    if (WideCharToMultiByte(CP_UTF8, 0, ws, -1, s.data(), len, nullptr, nullptr) == 0)
    {
        return std::string();
    }
    if (!s.empty())
    {
        s.resize(s.size() - 1);
    }
    return s;
}

// Expand an environment-based path template and canonicalize using wide safe APIs; returns canonical UTF-8 path or empty
static std::string ExpandAndCanonicalize(const char* templ)
{
    if (!templ) return std::string();
    std::wstring templW = Utf8ToWide(templ);
    if (templW.empty()) return std::string();
    wchar_t expandedW[MAX_PATH];
    DWORD ret = ExpandEnvironmentStringsW(templW.c_str(), expandedW, MAX_PATH);
    if (ret == 0 || ret > ARRAYSIZE(expandedW))
    {
        return std::string();
    }
    wchar_t canonW[MAX_PATH];
    if (FAILED(MyPathCchCanonicalize(canonW, ARRAYSIZE(canonW), expandedW)))
    {
        return std::string();
    }
    for (wchar_t* p = canonW; *p; ++p) if (*p == L'/') *p = L'\\';
    size_t len = wcslen(canonW);
    if (len > 3 && canonW[len - 1] == L'\\') canonW[len - 1] = L'\0';
    return WideToUtf8(canonW);
}

// saveFolder validation.  
// This code is designed for Windows.  Other platforms can define their own behavior as needed.
// Titles may provide a custom root via PFGameSaveInitArgs::saveFolder. Reject obviously unsafe or
// system protected locations to prevent accidental roaming of large / sensitive trees or conflicts
// with OS managed storage. Logic inspired by partner code but adapted to existing coding patterns.
bool IsDisallowedSaveRoot(const char* path)
{
    if (!path || !*path)
    {
        TRACE_WARNING("saveFolder empty or null");
        return true; // empty invalid
    }

    std::string input(path);
    // Trim trailing separator (except root like C:\)
    if (input.size() > 3 && (input.back() == '\\' || input.back() == '/'))
    {
        input.pop_back();
    }

    // Expand & canonicalize using wide APIs (safer). Avoid deprecated PathCanonicalizeA.
    // NOTE: Requires linking against Pathcch.lib. If that is undesirable, revert to PathCanonicalizeA
    // (with deprecation warning) or implement a custom canonicalizer.
    std::wstring inputW = Utf8ToWide(input.c_str());
    if (inputW.empty())
    {
        TRACE_WARNING("saveFolder Utf8ToWide conversion failed");
        return true;
    }
    wchar_t expandedW[MAX_PATH];
    DWORD ret = ExpandEnvironmentStringsW(inputW.c_str(), expandedW, MAX_PATH);
    if (ret == 0 || ret > ARRAYSIZE(expandedW))
    {
        TRACE_WARNING("saveFolder ExpandEnvironmentStringsW failed or buffer too small");
        return true;
    }
    wchar_t canonW[MAX_PATH];
    if (FAILED(MyPathCchCanonicalize(canonW, ARRAYSIZE(canonW), expandedW)))
    {
        TRACE_WARNING("saveFolder PathCchCanonicalize failed");
        return true;
    }
    if (wcslen(canonW) < 3 || canonW[1] != L':' || (canonW[2] != L'\\' && canonW[2] != L'/'))
    {
        TRACE_WARNING("saveFolder not rooted");
        return true;
    }
    for (wchar_t* p = canonW; *p; ++p) if (*p == L'/') *p = L'\\';
    size_t clen = wcslen(canonW);
    if (clen > 3 && canonW[clen - 1] == L'\\') canonW[clen - 1] = L'\0';
    std::string candidate = WideToUtf8(canonW);

    // Exact env disallowed
    for (auto templ : c_disallowedExactEnv)
    {
        std::string dis = ExpandAndCanonicalize(templ);
        if (!dis.empty() && PathEqualsUtf8(candidate, dis))
        {
            TRACE_WARNING("saveFolder '%s' matches disallowed exact env '%s'", candidate.c_str(), templ);
            return true;
        }
    }
    // Prefix disallowed
    for (auto templ : c_disallowedPrefixEnv)
    {
        std::string dis = ExpandAndCanonicalize(templ);
        if (!dis.empty() && PathHasPrefixUtf8(candidate, dis))
        {
            TRACE_WARNING("saveFolder '%s' under disallowed prefix '%s'", candidate.c_str(), templ);
            return true;
        }
    }
    // Known-folder-like env templates (approximation) exact matches
    for (auto templ : c_disallowedKnownFolderLikeEnv)
    {
        std::string dis = ExpandAndCanonicalize(templ);
        if (!dis.empty() && PathEqualsUtf8(candidate, dis))
        {
            TRACE_WARNING("saveFolder '%s' matches disallowed known-folder-like '%s'", candidate.c_str(), templ);
            return true;
        }
    }
    return false;
}

HRESULT GameSaveAPIProviderWin32::Initialize(_In_ PFGameSaveInitArgs* args) noexcept
{
    UNREFERENCED_PARAMETER(args);

    TRACE_INFORMATION("GameSaveAPIProviderWin32::Initialize");
    if( args->saveFolder != nullptr )
    {
        if (IsDisallowedSaveRoot(args->saveFolder))
        {
            TRACE_ERROR("saveFolder '%s' is disallowed / invalid", args->saveFolder);
            return E_INVALIDARG;
        }
        SharedPtr<GameSaveGlobalState> globalState;
        HRESULT hr = GameSaveGlobalState::Get(globalState);
        if (SUCCEEDED(hr))
        {
            globalState->SetInitArgsSaveRootFolder(args->saveFolder);
        }
    }
    else
    {
        TRACE_ERROR("saveFolder is null. Must be provided on this platform");
        return E_INVALIDARG;
    }

    return S_OK;
}

HRESULT GameSaveAPIProviderWin32::UninitializeAsync(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return GameSaveGlobalState::CleanupAsync(async);
}

HRESULT GameSaveAPIProviderWin32::UninitializeResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncGetStatus(async, false);
}

HRESULT GameSaveAPIProviderWin32::SetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback, _In_opt_ void* context
) noexcept
{
    auto& uiInfo = GetGameSaveUiCallbackInfo();
    uiInfo.activeDeviceChangedCallbackQueue = callbackQueue;
    uiInfo.activeDeviceChangedCallback = callback;
    uiInfo.activeDeviceChangedContext = context;

    return S_OK;
}

HRESULT GameSaveAPIProviderWin32::SetUiCallbacks(
    _In_ PFGameSaveUICallbacks* callbacks
) noexcept
{
    auto& uiInfo = GetGameSaveUiCallbackInfo();

    uiInfo.progressCallback = callbacks->progressCallback;
    uiInfo.progressContext = callbacks->progressContext;

    uiInfo.syncFailedCallback = callbacks->syncFailedCallback;
    uiInfo.syncFailedContext = callbacks->syncFailedContext;

    uiInfo.activeDeviceContentionCallback = callbacks->activeDeviceContentionCallback;
    uiInfo.activeDeviceContentionContext = callbacks->activeDeviceContentionContext;

    uiInfo.conflictCallback = callbacks->conflictCallback;
    uiInfo.conflictContext = callbacks->conflictContext;

    uiInfo.outOfStorageCallback = callbacks->outOfStorageCallback;
    uiInfo.outOfStorageContext = callbacks->outOfStorageContext;

    return S_OK;
}

HRESULT GameSaveAPIProviderWin32::UiProgressGetProgress(
    _In_ PFLocalUserHandle localUserHandle, 
    _Out_opt_ PFGameSaveFilesSyncState* syncState, 
    _Out_opt_ uint64_t* current, 
    _Out_opt_ uint64_t* total) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

    return GameSaveEntityApiImpl(localUserHandle, [&syncState, &current, &total](SharedPtr<FolderSyncManager> folderSync)
    {
        FolderSyncManagerProgress progress = folderSync->GetSyncProgress();

        if (syncState)
        {
            *syncState = progress.syncState;
        }

        if (current)
        {
            *current = progress.current;
        }

        if (total)
        {
            *total = progress.total;
        }

        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::GetFolderSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* saveRootFolderSize
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(saveRootFolderSize);

    return GameSaveEntityApiImpl(localUserHandle, [&saveRootFolderSize](SharedPtr<FolderSyncManager> folderSync)
    {
        const String& folder = folderSync->GetFolder();
        *saveRootFolderSize = folder.size() + 1;
        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::GetFolder(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t saveRootFolderSize,
    _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
    _Out_opt_ size_t* saveRootFolderUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(saveRootFolderBuffer);

    return GameSaveEntityApiImpl(localUserHandle, [saveRootFolderSize, &saveRootFolderBuffer, &saveRootFolderUsed](SharedPtr<FolderSyncManager> folderSync)
    {
        const String& folder = folderSync->GetFolder();
        RETURN_HR_IF(E_INVALIDARG, saveRootFolderSize < folder.size() + 1);
        memcpy(saveRootFolderBuffer, folder.data(), folder.size() + 1);

        if (saveRootFolderUsed)
        {
            *saveRootFolderUsed = folder.size() + 1;
        }

        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::AddUserWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesAddUserOptions options,
    _Inout_ XAsyncBlock* async) noexcept
{

    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

    TRACE_TASK(__FUNCTION__);
    return GameSaveEntityAsyncApiImpl(async, XASYNC_IDENTITY(AddUserWithUiAsync), true, localUserHandle, 
    [async, options](SharedPtr<FolderSyncManager> folderSync, RunContext&& rc)
    {
        FolderSyncManagerProgress progress = folderSync->GetSyncProgress();
        if (progress.syncState != PFGameSaveFilesSyncState::NotStarted)
        {
            return E_PF_GAMESAVE_USER_ALREADY_ADDED;
        }

        folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::PreparingForDownload, 0, 0);
        folderSync->SetAddUserOptions(options);

        auto provider = MakeDownloadAsyncProvider(
            std::move(rc),
            async,
            XASYNC_IDENTITY(AddUserWithUiAsync),
            std::move(folderSync)
        );

        return XAsyncProviderBase::Run(std::move(provider));
    });
}

HRESULT GameSaveAPIProviderWin32::AddUserWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncGetStatus(async, false);
}

HRESULT GameSaveAPIProviderWin32::GetRemainingQuota(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ int64_t* remainingQuota) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(remainingQuota);

    *remainingQuota = 0;
    return GameSaveEntityApiImpl(localUserHandle, [&remainingQuota](SharedPtr<FolderSyncManager> folderSync)
    {
        if (folderSync->IsForcedDisconnectFromCloud())
        {
            // This error is returned regardless if the network is restored as the user is still considered offline.
            return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
        }

        *remainingQuota = folderSync->GetRemainingQuota();
        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::IsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(isConnectedToCloud);

    *isConnectedToCloud = false;
    return GameSaveEntityApiImpl(localUserHandle, [&isConnectedToCloud](SharedPtr<FolderSyncManager> folderSync)
    {
        *isConnectedToCloud = (folderSync->IsForcedDisconnectFromCloud() == false);
        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::UploadWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUploadOption option,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    TRACE_TASK("PFGameSaveFilesUploadWithUiAsync");

    return GameSaveEntityAsyncApiImpl(async, XASYNC_IDENTITY(PFGameSaveFilesUploadWithUiAsync), false, localUserHandle,
    [option, async](SharedPtr<FolderSyncManager> folderSync, RunContext&& rc)
    {
        if (!folderSync->IsForcedDisconnectFromCloud())
        {
            FolderSyncManagerProgress progress = folderSync->GetSyncProgress();
            if (progress.syncState == PFGameSaveFilesSyncState::NotStarted)
            {
                return E_PF_GAMESAVE_USER_NOT_ADDED;
            }

            if (folderSync->IsDeviceReleasedAsActive())
            {
                return E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE;
            }

            if (progress.syncState == PFGameSaveFilesSyncState::PreparingForDownload ||
                progress.syncState == PFGameSaveFilesSyncState::Downloading)
            {
                return E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS;
            }

            folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::PreparingForUpload, 0, 0);
        }

        auto provider = MakeStartFileSyncProvider(
            std::move(rc),
            option,
            async,
            XASYNC_IDENTITY(PFGameSaveFilesUploadWithUiAsync),
            std::move(folderSync));

        return XAsyncProviderBase::Run(std::move(provider));
    });
}

HRESULT GameSaveAPIProviderWin32::UploadWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncGetStatus(async, false);
}

HRESULT UiActionHelper(PFLocalUserHandle localUserHandle, UIAction action)
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    return GameSaveEntityApiImpl(localUserHandle, [action](SharedPtr<FolderSyncManager> folderSync)
    {
        UIAction actionChoice = action;
        if (actionChoice == UIAction::UISyncFailedUseOffline)
        {
            auto syncState = folderSync->GetSyncProgress().syncState;
            if (syncState != PFGameSaveFilesSyncState::NotStarted &&
                syncState != PFGameSaveFilesSyncState::PreparingForDownload)
            {
                // UISyncFailedUseOffline is only valid preparing to download
                // If the user is uploading and it fails and the game gives us UISyncFailedUseOffline, just cancel the upload instead
                // since we don't want to go into offline mode during upload
                TRACE_ERROR_HR(E_INVALIDARG, "PFGameSaveFilesUiSyncFailedUserAction::UseOffline can't be used outside of PFGameSaveFilesAddUserWithUiAsync");
                actionChoice = UIAction::UISyncFailedCancel;
            }
        }
        return folderSync->GetUIManager().SetAction(actionChoice);
    });
}

HRESULT GameSaveAPIProviderWin32::SetUiProgressResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiProgressUserAction action) noexcept
{
    switch (action)
    {
    case PFGameSaveFilesUiProgressUserAction::Cancel:
        return UiActionHelper(localUserHandle, UIAction::UIProgressCancel);
    default:
        return E_INVALIDARG;
    }
}

HRESULT GameSaveAPIProviderWin32::SetUiSyncFailedResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiSyncFailedUserAction action) noexcept
{
    switch (action)
    {
    case PFGameSaveFilesUiSyncFailedUserAction::Retry:
        return UiActionHelper(localUserHandle, UIAction::UISyncFailedRetry);
    case PFGameSaveFilesUiSyncFailedUserAction::Cancel:
        return UiActionHelper(localUserHandle, UIAction::UISyncFailedCancel);
    case PFGameSaveFilesUiSyncFailedUserAction::UseOffline:
        return UiActionHelper(localUserHandle, UIAction::UISyncFailedUseOffline);
    default: 
        return E_INVALIDARG;
    }
}

HRESULT GameSaveAPIProviderWin32::SetUiActiveDeviceContentionResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action) noexcept
{
    switch (action)
    {
    case PFGameSaveFilesUiActiveDeviceContentionUserAction::Retry:
        return UiActionHelper(localUserHandle, UIAction::UIActiveDeviceContentionRetry);
    case PFGameSaveFilesUiActiveDeviceContentionUserAction::Cancel:
        return UiActionHelper(localUserHandle, UIAction::UIActiveDeviceContentionCancel);
    case PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData:
        return UiActionHelper(localUserHandle, UIAction::UIActiveDeviceContentionSyncLastSaved);
    default:
        return E_INVALIDARG;
    }
}

HRESULT GameSaveAPIProviderWin32::SetUiConflictResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiConflictUserAction action) noexcept
{
    switch (action)
    {
    case PFGameSaveFilesUiConflictUserAction::Cancel:
        return UiActionHelper(localUserHandle, UIAction::UIConflictCancel);
    case PFGameSaveFilesUiConflictUserAction::TakeLocal:
        return UiActionHelper(localUserHandle, UIAction::UIConflictTakeLocal);
    case PFGameSaveFilesUiConflictUserAction::TakeRemote:
        return UiActionHelper(localUserHandle, UIAction::UIConflictTakeRemote);
    default:
        return E_INVALIDARG;
    }
}

HRESULT GameSaveAPIProviderWin32::SetUiOutOfStorageResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiOutOfStorageUserAction action) noexcept
{
    switch (action)
    {
    case PFGameSaveFilesUiOutOfStorageUserAction::Cancel:
        return UiActionHelper(localUserHandle, UIAction::UIOutOfStorageCancel);
    case PFGameSaveFilesUiOutOfStorageUserAction::Retry:
        return UiActionHelper(localUserHandle, UIAction::UIOutOfStorageSpaceCleared);
    default:
        return E_INVALIDARG;
    }
}

HRESULT GameSaveAPIProviderWin32::SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetDebugDeviceIdOverride(deviceId);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetDebugManifestOffset(offset);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetDebugMockDataOverride(mockDataFolder);
        PlayFab::GameSaveWrapper::GameSaveServiceSelector::useMocks = true;
        PlayFab::GameSave::GameSaveTelemetryManager::useMocks = true;
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(jsonSize);

    return GameSaveEntityApiImpl(localUserHandle, [&jsonSize](SharedPtr<FolderSyncManager> folderSync)
    {
        const String& stats = folderSync->GetStatsJsonForDebug();
        *jsonSize = stats.size() + 1;
        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::GetStatsJsonForDebug(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t jsonSize,
    _Out_writes_(jsonSize) char* jsonBuffer,
    _Out_opt_ size_t* jsonSizeUsed) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(jsonBuffer);

    return GameSaveEntityApiImpl(localUserHandle, [jsonSize, &jsonBuffer, &jsonSizeUsed](SharedPtr<FolderSyncManager> folderSync)
    {
        const String& stats = folderSync->GetStatsJsonForDebug();
        RETURN_HR_IF(E_INVALIDARG, jsonSize < stats.size() + 1);
        memcpy(jsonBuffer, stats.data(), stats.size() + 1);
        if (jsonSizeUsed)
        {
            *jsonSizeUsed = stats.size() + 1;
        }

        return S_OK;
    });
}

HRESULT GameSaveAPIProviderWin32::SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetForceOutOfStorageError(forceError);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetForceSyncFailedError(forceError);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept
{
    try
    {
        SharedPtr<GameSaveGlobalState> state;
        HRESULT hr = GameSaveGlobalState::Get(state);
        RETURN_IF_FAILED(hr);
        state->SetWriteManifestsToDisk(writeManifests);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::PauseUploadForDebug() noexcept
{
    try
    {
        PlayFab::GameSaveWrapper::GameSaveServiceMock::PauseUpload();
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::SetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode) noexcept
{
    try
    {
        PlayFab::GameSaveWrapper::GameSaveServiceMock::SetForcedOffline(mode);
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::ResumeUploadForDebug() noexcept
{
    try
    {
        PlayFab::GameSaveWrapper::GameSaveServiceMock::ResumeUpload();
        return S_OK;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

HRESULT GameSaveAPIProviderWin32::ResetCloudAsync(_In_ PFLocalUserHandle localUserHandle, _In_ XAsyncBlock* async) noexcept
{
    return GameSaveEntityAsyncApiImpl(async, XASYNC_IDENTITY(ResetCloudAsync), true, localUserHandle,
    [async](SharedPtr<FolderSyncManager> folderSync, RunContext&& rc)
    {
        auto provider = MakeStartFileResetCloudAsyncProvider(
            std::move(rc),
            async,
            XASYNC_IDENTITY(ResetCloudAsync),
            std::move(folderSync));

        return XAsyncProviderBase::Run(std::move(provider));
    });
}

HRESULT GameSaveAPIProviderWin32::ResetCloudResult(_Inout_ XAsyncBlock* async) noexcept
{
    return XAsyncGetStatus(async, false);
}

HRESULT GameSaveAPIProviderWin32::SetSaveDescriptionAsync(
    _In_ PFLocalUserHandle localUserHandle, 
    _In_ const char* shortSaveDescription,
    _In_ XAsyncBlock* async) noexcept
{
    String shortSaveDescriptionStr = shortSaveDescription;

    return GameSaveEntityAsyncApiImpl(async, XASYNC_IDENTITY(SetSaveDescriptionAsync), true, localUserHandle,
    [async, shortSaveDescriptionStr](SharedPtr<FolderSyncManager> folderSync, RunContext&& rc)
    {
        // If an upload is already in progress (or about to start), the service will reject a
        // standalone UpdateManifest description change with
        // E_PF_GAME_SAVE_MANIFEST_DESCRIPTION_UPDATE_NOT_ALLOWED because the manifest version is
        // transitioning via the upload/finalize pipeline. In that case, just cache the new
        // description locally so the pending FinalizeManifest (UploadStep) will include it.
        FolderSyncManagerProgress progress = folderSync->GetSyncProgress();
        if (progress.syncState == PFGameSaveFilesSyncState::PreparingForUpload ||
            progress.syncState == PFGameSaveFilesSyncState::Uploading)
        {
            if (!folderSync->HasStartedFinalizeManifest())
            {
                folderSync->SetLastShortSaveDescription(shortSaveDescriptionStr);
                TRACE_INFORMATION("Deferring SetSaveDescription (upload in progress pre-finalize). Will apply at FinalizeManifest");
                XAsyncComplete(async, S_OK, 0);
                return S_OK;
            }
            // Finalize already started; fall through to provider which will internally wait for finalize to complete before performing UpdateManifest
            TRACE_INFORMATION("SetSaveDescription called during in-flight FinalizeManifest; provider will wait until finalize completes");
        }

        folderSync->ResetSetSaveDescriptionStep();

        auto provider = MakeStartSetSaveDescriptionProvider(
            std::move(rc),
            async,
            XASYNC_IDENTITY(SetSaveDescriptionAsync),
            std::move(folderSync),
            std::move(shortSaveDescriptionStr));

        return XAsyncProviderBase::Run(std::move(provider));
    });
}

HRESULT GameSaveAPIProviderWin32::SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept
{
    return XAsyncGetStatus(async, false);
}

} // namespace GameSave
} // namespace PlayFab

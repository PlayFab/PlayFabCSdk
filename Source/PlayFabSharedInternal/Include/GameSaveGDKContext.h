// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK

#include <atomic>
#include <playfab/core/PFLocalUser.h>
#include <playfab/gamesave/PFGameSaveFiles.h>

// Shared GDK game save context.
//
// The storage for PFXPALGameSaveContext is allocated by PlayFabCore (PFCoreGlobalState) and handed
// to the game save GRTS provider through PFPlatformGetGameSaveContext(). Both components therefore
// have to agree on the exact layout. This header is the single definition both sides include -
// duplicating the structs in each component let them drift apart, which silently corrupted memory
// past the end of PFCore's storage.

constexpr auto PF_GDK_MAX_USERS = 16;

struct PFXPALGameSaveUserState
{
    PFLocalUserHandle localUser;
    XUserHandle xUser;
    void* configHandle;
    char saveFolder[1024];
    bool isConnectedToCloud{ true };
    // Serializes AddUserWithUiAsync for this user. Allowing a disconnected user to re-enter
    // AddUser (the documented reconnect) removed the implicit serialization that the
    // E_PF_GAMESAVE_USER_ALREADY_ADDED gate used to provide, so overlapping calls could both
    // reach the deferred commit and free each other's live config/xUser handles while GRTS
    // still had operations outstanding against them. This flag rejects re-entrant calls
    // explicitly instead of relying on isConnectedToCloud as an implicit lock (it isn't
    // updated until PFXPALGetFolderComplete runs, so it can't serialize anything).
    bool addUserInProgress{ false };
};

struct PFXPALGameSaveContext
{
    PFGameSaveFilesUiProgressCallback* progressCallback;
    PFGameSaveFilesUiSyncFailedCallback* syncFailedCallback;
    PFGameSaveFilesUiActiveDeviceContentionCallback* activeDeviceContentionCallback;
    PFGameSaveFilesUiConflictCallback* conflictCallback;
    PFGameSaveFilesUiOutOfStorageCallback* outOfStorageCallback;
    // Active device changed callback registration (atomic for thread-safety)
    std::atomic<PFGameSaveFilesActiveDeviceChangedCallback*> activeDeviceChangedCallback{ nullptr };
    std::atomic<void*> activeDeviceChangedContext{ nullptr };
    std::atomic<XTaskQueueHandle> activeDeviceChangedCallbackQueue{ nullptr };
    PFXPALGameSaveUserState users[PF_GDK_MAX_USERS];
};

#endif // HC_PLATFORM == HC_PLATFORM_GDK

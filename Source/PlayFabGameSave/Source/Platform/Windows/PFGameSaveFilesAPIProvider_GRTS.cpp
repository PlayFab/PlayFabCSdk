// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "PFGameSaveFilesAPIProvider_GRTS.h"
#include <playfab/core/PFLocalUser_Xbox.h>
#include <playfab/core/PFEntity.h>
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <PFXGameSave.h>
#include <XGameRuntimeInit.h>
// Shared with PlayFabCore, which owns the storage returned by PFPlatformGetGameSaveContext().
#include "GameSaveGDKContext.h"

namespace PlayFab
{
namespace GameSave
{

#ifndef HANDLE_XASYNC_FAILURE
#define HANDLE_XASYNC_FAILURE(hr) do { if (FAILED(hr)) { XAsyncComplete(asyncBlock, hr, 0); return; }} while (0, 0)
#endif

static const HRESULT E_GS_USER_CANCELED = 0x80830004;

// Serializes user-slot lookup, reservation and commit in PFXPALCallGetFolderWithUiAsync. The slot
// array lives in shared PFCore storage and the search/claim sequence is a read-modify-write, so
// concurrent AddUser calls for different users could otherwise both pick the same empty slot and
// the loser's commit would overwrite (and leak/free) state the winner's in-flight GRTS operation
// still owns.
static std::mutex& GameSaveUserSlotMutex()
{
    static std::mutex s_slotMutex;
    return s_slotMutex;
}


// Fwd decl
void CALLBACK PFXPALGetFolderComplete(XAsyncBlock* async);

PFLocalUserHandle PFXPALGetLocalUserFromXUser(_In_ XUserHandle requestingUser, PFXPALGameSaveContext** pgsContext)
{
    void* gsContextPtr = nullptr;
    if (SUCCEEDED(PFPlatformGetGameSaveContext(&gsContextPtr)))
    {
        PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);
        if (pgsContext != nullptr)
        {
            *pgsContext = gsContext;
        }

        // Scanning the slots races UninitializeAsync and the add-user completion path, both of
        // which close these handles, so the read has to be serialized with them.
        std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
        for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
        {
            if (XUserCompare(requestingUser, gsContext->users[i].xUser) == 0)
            {
                return gsContext->users[i].localUser;
            }
        }
    }

    return nullptr;
}

XUserHandle PFXPALGetXUserFromLocalUser(_In_ PFLocalUserHandle localUserHandle)
{
    void* gsContextPtr = nullptr;
    if (SUCCEEDED(PFPlatformGetGameSaveContext(&gsContextPtr)))
    {
        PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);
        std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
        for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
        {
            if (PFLocalUserHandleCompare(localUserHandle, gsContext->users[i].localUser) == 0)
            {
                return gsContext->users[i].xUser;
            }
        }
    }

    return nullptr;
}

// Assumes GameSaveUserSlotMutex() is already held by the caller.
PFXPALGameSaveUserState* PFXPALGetStateFromLocalUserUnsafe(_In_ PFLocalUserHandle localUserHandle)
{
    void* gsContextPtr = nullptr;
    HRESULT hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    if (SUCCEEDED(hr))
    {
        PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);
        for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
        {
            if (PFLocalUserHandleCompare(localUserHandle, gsContext->users[i].localUser) == 0)
            {
                return &gsContext->users[i];
            }
        }
    }

    return nullptr;
}

// Value snapshot of a user slot, taken under GameSaveUserSlotMutex().
//
// Returning a raw PFXPALGameSaveUserState* from a locked lookup was unsafe: the lock was released
// on return, so callers read configHandle / saveFolder / isConnectedToCloud while the add-user
// completion path was still StrCpy-ing into saveFolder and UninitializeAsync was clearing and
// freeing the same fields under the mutex. Copying the fields out under the lock removes the torn
// and mid-teardown reads.
//
// Limitation, deliberately not solved here: this does NOT extend the lifetime of configHandle.
// UninitializeAsync can still call PFXGameSaveFreeConfig on it after the snapshot is taken and
// before the caller hands it to GRTS. Closing that window needs per-slot in-flight ownership so
// teardown cannot free a config with operations outstanding, which is a larger change than this
// fix set covers.
struct PFXPALGameSaveUserStateSnapshot
{
    void* configHandle{ nullptr };
    bool isConnectedToCloud{ true };
    char saveFolder[sizeof(PFXPALGameSaveUserState::saveFolder)]{};
};

bool PFXPALTryGetUserStateSnapshot(_In_ PFLocalUserHandle localUserHandle, _Out_ PFXPALGameSaveUserStateSnapshot& snapshot)
{
    std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };

    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUserUnsafe(localUserHandle);
    if (state == nullptr)
    {
        snapshot = {};
        return false;
    }

    snapshot.configHandle = state->configHandle;
    snapshot.isConnectedToCloud = state->isConnectedToCloud;
    StrCpy(snapshot.saveFolder, sizeof(snapshot.saveFolder), state->saveFolder);
    return true;
}

void ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(const PFXGameSaveDescriptor* descIn, PFGameSaveDescriptor* descOut)
{
    if (descIn == nullptr || descOut == nullptr)
    {
        return;
    }

    TRACE_INFORMATION("ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor: input time=%lld, totalBytes=%llu, uploadedBytes=%llu", 
        static_cast<long long>(descIn->time), descIn->totalBytes, descIn->uploadedBytes);

    descOut->time = descIn->time;
    descOut->totalBytes = descIn->totalBytes;
    descOut->uploadedBytes = descIn->uploadedBytes;
    if(descIn->deviceType != nullptr)
    {
        StrCpy(descOut->deviceType, sizeof(descOut->deviceType), descIn->deviceType);
    }
    else
    {
        descOut->deviceType[0] = '\0';
    }

    if(descIn->deviceId != nullptr)
    {
        StrCpy(descOut->deviceId, sizeof(descOut->deviceId), descIn->deviceId);
    }
    else
    {
        descOut->deviceId[0] = '\0';
    }

    if(descIn->friendlyName != nullptr)
    {
        StrCpy(descOut->deviceFriendlyName, sizeof(descOut->deviceFriendlyName), descIn->friendlyName);
    }
    else
    {
        descOut->deviceFriendlyName[0] = '\0';
    }

    if(descIn->thumbnailUri != nullptr)
    {
        StrCpy(descOut->thumbnailUri, sizeof(descOut->thumbnailUri), descIn->thumbnailUri);
    }
    else
    {
        descOut->thumbnailUri[0] = '\0';
    }

    if (descIn->shortSaveDescription != nullptr)
    {
        StrCpy(descOut->shortSaveDescription, sizeof(descOut->shortSaveDescription), descIn->shortSaveDescription);
    }
    else
    {
        descOut->shortSaveDescription[0] = '\0';
    }

    TRACE_INFORMATION("ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor: output time=%lld, totalBytes=%llu, uploadedBytes=%llu", 
        descOut->time, descOut->totalBytes, descOut->uploadedBytes);
}

HRESULT GameSaveAPIProviderGRTS::Initialize(_In_ PFGameSaveInitArgs* args) noexcept
{
    if (args && args->saveFolder && args->saveFolder[0] != '\0')
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::Initialize: saveFolder='%s'", args->saveFolder);
        this->m_initSaveFolder = String(args->saveFolder);

        // Normalize forward slashes to backslashes. Some engines (e.g. Unreal Engine) hand us
        // paths with forward slashes by default, but the in-proc XGameSave (GRTS) path requires
        // Windows-style backslashes. A forward-slash path otherwise silently fails to resolve
        // correctly (no init error is surfaced), so normalize it here at the single point where
        // the title-provided save folder is captured.
        for (auto& ch : this->m_initSaveFolder)
        {
            if (ch == '/')
            {
                ch = '\\';
            }
        }

        if (this->m_initSaveFolder != args->saveFolder)
        {
            TRACE_INFORMATION("GameSaveAPIProviderGRTS::Initialize: normalized saveFolder='%s'", this->m_initSaveFolder.c_str());
        }
    }
    else
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::Initialize: no saveFolder specified");
    }
    return S_OK;
}

struct PFXPALGetFolderContext
{
    ~PFXPALGetFolderContext()
    {
        if (compositeQueue)
        {
            XTaskQueueCloseHandle(compositeQueue);
        }
        // xuser is an owned duplicate (see PFLocalUserTryGetXUser). It is nulled once ownership
        // is transferred to the user slot, so anything still here belongs to a call that never
        // committed it - close it rather than leak it on the early-return/failure paths.
        if (xuser)
        {
            XUserCloseHandle(xuser);
        }
    }

    PFLocalUserHandle localUserHandle{};
    PFGameSaveFilesAddUserOptions addUserOptions{ PFGameSaveFilesAddUserOptions::None };
    XAsyncBlock getFolderAsyncBlock{};
    XAsyncBlock loginLocalUserAsyncBlock{};
    XTaskQueueHandle compositeQueue{ nullptr };
    XAsyncBlock* clientAsyncBlock{};

    bool useEntityAuth{ false };
    // True when AddUserWithUiAsync already set addUserInProgress on this user's slot (the
    // reconnect path). Lets the slot-claim re-check distinguish "somebody else owns this slot"
    // from "this call owns it".
    bool claimedInFlightSlot{ false };
    // True while this call still owns the slot's addUserInProgress claim. Exchanged to false by
    // whoever releases it (the folder completion handler, or XAsyncOp::Cleanup when the operation
    // failed before that handler could ever run), so the claim is released exactly once and never
    // stolen from a later AddUser.
    std::atomic<bool> inFlightClaimOwned{ false };
    XUserHandle xuser{ nullptr }; 
    XAsyncBlock entityTokenAsyncBlock{}; // async to fetch cached entity token
    char apiEndpoint[1024]{};
    char playfabTitleId[256]{};
    char saveFolderOverride[1024]{}; 
    char entityId[128]{};     
    char entityToken[2048]{};

    // Cancellation / progress tracking flags
    bool getFolderAsyncStarted{ false };
    bool entityTokenAsyncStarted{ false };
    bool loginLocalUserAsyncStarted{ false }; // track login async to support cancellation
    bool cancelRequested{ false };
    // True when this call reused an already-populated user slot (offline -> online reconnect)
    // rather than claiming a fresh one. The completion handler uses this to avoid tearing down
    // a pre-existing offline user when the reconnect attempt fails.
    bool reusedExistingSlot{ false };
};

// Build config request based on context state & start folder retrieval
static HRESULT PFXPALCallGetFolderWithUiAsync(PFXPALGetFolderContext* context)
{
    HRESULT hr;

    PFXGameSaveConfigRequest configRequest{};
    configRequest.requestingUser = context->xuser; // may be null if not using XUser path
    configRequest.titleId = context->playfabTitleId;
    configRequest.apiUrl = context->apiEndpoint;
	
    // Apply rollback intent flags if present
    configRequest.flags = static_cast<uint32_t>(PFXGameSaveConfigRequestInitFlags::InitFlagNone);
    if ((context->addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) == PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood)
    {
        configRequest.flags |= static_cast<uint32_t>(PFXGameSaveConfigRequestInitFlags::InitFlagRollbackToLastKnownGood);
        TRACE_INFORMATION("AddUserWithUiAsync GRTS: Rollback intent = LastKnownGood");
    }
    if ((context->addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastConflict) == PFGameSaveFilesAddUserOptions::RollbackToLastConflict)
    {
        configRequest.flags |= static_cast<uint32_t>(PFXGameSaveConfigRequestInitFlags::InitFlagRollbackToLastConflict);
        TRACE_INFORMATION("AddUserWithUiAsync GRTS: Rollback intent = LastConflict");
    }

    if (context->useEntityAuth)
    {
        configRequest.flags |= static_cast<uint32_t>(PFXGameSaveConfigRequestInitFlags::InitFlagUseEntityAuth);
        configRequest.entityId = context->entityId;
        configRequest.entityToken = context->entityToken;
    }
    
    if (context->saveFolderOverride[0] != '\0')
    {
        configRequest.flags |= static_cast<uint32_t>(PFXGameSaveConfigRequestInitFlags::InitFlagUseFileLocation);
        configRequest.fileLocation = context->saveFolderOverride;

        // Ensure folder exists before calling GRTS
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, context->saveFolderOverride, -1, nullptr, 0);
        std::wstring widePath(static_cast<size_t>(wideLen - 1), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, context->saveFolderOverride, -1, &widePath[0], wideLen);
        BOOL created = CreateDirectoryW(widePath.c_str(), nullptr);
        DWORD lastError = GetLastError();
        TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: CreateDirectoryW('%s') result=%d, lastError=%u", 
            context->saveFolderOverride, created, lastError);

        if (!created && lastError != ERROR_ALREADY_EXISTS)
        {
            TRACE_WARNING("PFXPALCallGetFolderWithUiAsync: Failed to create directory '%s', error=%u", 
                context->saveFolderOverride, lastError);
            return HRESULT_FROM_WIN32(lastError);
        }
    }

    // Log all config request parameters before calling PFXGameSaveInitializeConfig
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.requestingUser=%p", configRequest.requestingUser);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.titleId=%s", configRequest.titleId ? configRequest.titleId : "(null)");
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.apiUrl=%s", configRequest.apiUrl ? configRequest.apiUrl : "(null)");
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.flags=0x%08x", configRequest.flags);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.entityId=%s (len=%zu)", 
        configRequest.entityId ? configRequest.entityId : "(null)",
        configRequest.entityId ? strlen(configRequest.entityId) : 0);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.entityToken len=%zu", 
        configRequest.entityToken ? strlen(configRequest.entityToken) : 0);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: configRequest.fileLocation=%s", configRequest.fileLocation ? configRequest.fileLocation : "(null)");

    PFXGameSaveConfigHandle configHandle{};
    hr = PFXGameSaveInitializeConfig(&configRequest, &configHandle);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: PFXGameSaveInitializeConfig hr=0x%08x", hr);
    RETURN_IF_FAILED(hr);

    void* gsContextPtr = nullptr;
    hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);

    // Slot search, reservation and commit form one read-modify-write over shared state, so they
    // have to be serialized end-to-end: two concurrent AddUser calls for different users would
    // otherwise both observe the same empty slot and the second commit would overwrite the first
    // call's live localUser/xUser/configHandle while GRTS still had work outstanding on them.
    std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };

    // Find existing slot for this local user or claim an empty one; fail if all slots are occupied.
    ULONG indexFound = PF_GDK_MAX_USERS;       // sentinel meaning not found yet
    ULONG firstEmptySlot = PF_GDK_MAX_USERS;   // sentinel meaning no empty slot yet
    for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
    {
        if (gsContext->users[i].localUser == nullptr && firstEmptySlot == PF_GDK_MAX_USERS)
        {
            firstEmptySlot = i;
        }
        if (PFLocalUserHandleCompare(context->localUserHandle, gsContext->users[i].localUser) == 0)
        {
            indexFound = i;
            break;
        }
    }

    const bool reusingExistingSlot = (indexFound != PF_GDK_MAX_USERS);
    context->reusedExistingSlot = reusingExistingSlot;
    if (reusingExistingSlot && gsContext->users[indexFound].addUserInProgress && !context->claimedInFlightSlot)
    {
        // Another AddUser already owns this slot. Re-checked here (under the lock) because the
        // initial-add case has no slot to claim at AddUserWithUiAsync time, so two concurrent adds
        // for the same brand-new user can both reach this point; committing would free the
        // winner's live config/xUser while GRTS still has work outstanding on them.
        TRACE_WARNING("PFXPALCallGetFolderWithUiAsync: slot %u already has an add-user in flight, rejecting", indexFound);
        PFXGameSaveFreeConfig(configHandle);
        return E_PF_GAMESAVE_USER_ALREADY_ADDED;
    }

    if (!reusingExistingSlot)
    {
        if (firstEmptySlot == PF_GDK_MAX_USERS)
        {
            TRACE_WARNING("AddUserWithUiAsync GRTS: no free user slots (max %u). Failing initialization.", PF_GDK_MAX_USERS);
            PFXGameSaveFreeConfig(configHandle);
            return E_FAIL; // All slots full
        }
        indexFound = firstEmptySlot;
        TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: using new slot index=%u", indexFound);

        PFLocalUserHandle duplicatedHandle{};
        hr = PFLocalUserDuplicateHandle(context->localUserHandle, &duplicatedHandle);
        if (FAILED(hr))
        {
            PFXGameSaveFreeConfig(configHandle);
            return hr;
        }
        gsContext->users[indexFound].localUser = duplicatedHandle;
        gsContext->users[indexFound].xUser = nullptr;
        gsContext->users[indexFound].configHandle = nullptr;
    }
    else
    {
        // Reconnect (offline -> online re-sync). Keep the existing slot - localUser, xUser and
        // configHandle - intact until the replacement folder retrieval has actually started.
        // Tearing it down here leaked the previous config (it was nulled without being freed)
        // and, if the start then failed, destroyed a perfectly good offline user state.
        TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: reusing existing slot index=%u (deferring state commit until inner async starts)", indexFound);
    }

    // Clean up after a failed start. Only unwind the slot when this call created it; on a
    // reconnect the pre-existing (offline) state must survive an unsuccessful start.
    auto cleanupOnStartFailure = [&]()
    {
        PFXGameSaveFreeConfig(configHandle);
        if (!reusingExistingSlot)
        {
            PFLocalUserCloseHandle(gsContext->users[indexFound].localUser);
            gsContext->users[indexFound].localUser = nullptr;
            gsContext->users[indexFound].xUser = nullptr;
            gsContext->users[indexFound].configHandle = nullptr;
        }
    };

    if (context->clientAsyncBlock->queue != nullptr)
    {
        XTaskQueuePortHandle workPort{ nullptr };
        hr = XTaskQueueGetPort(context->clientAsyncBlock->queue, XTaskQueuePort::Work, &workPort);
        if (SUCCEEDED(hr))
        {
            hr = XTaskQueueCreateComposite(workPort, workPort, &context->compositeQueue);
        }
        if (FAILED(hr))
        {
            cleanupOnStartFailure();
            return hr;
        }
    }

    context->getFolderAsyncBlock.callback = PFXPALGetFolderComplete;
    context->getFolderAsyncBlock.queue = context->compositeQueue;
    context->getFolderAsyncBlock.context = context;
    hr = PFXGameSaveFilesGetFolderWithUiAsync(configHandle, &context->getFolderAsyncBlock);
    TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: PFXGameSaveFilesGetFolderWithUiAsync hr=0x%08x", hr);
    if (SUCCEEDED(hr))
    {
        // Inner async started: only now commit the replacement config into the slot, then free
        // the superseded one. Ordering it this way means a failed start leaves the previous
        // (offline) state usable, and the old config is never dropped without being freed.
        void* supersededConfig = gsContext->users[indexFound].configHandle;
        XUserHandle supersededXUser = gsContext->users[indexFound].xUser;
        gsContext->users[indexFound].xUser = context->xuser; // may be null if no XUser path
        gsContext->users[indexFound].configHandle = configHandle;
        if (supersededConfig != nullptr && supersededConfig != configHandle)
        {
            PFXGameSaveFreeConfig(static_cast<PFXGameSaveConfigHandle>(supersededConfig));
        }
        // PFLocalUserTryGetXUser returns an XUserDuplicateHandle copy that this provider owns
        // (UninitializeAsync closes the slot's copy), so the handle being replaced has to be
        // closed here or every reconnect leaks an XUser handle.
        if (supersededXUser != nullptr && supersededXUser != context->xuser)
        {
            XUserCloseHandle(supersededXUser);
        }
        // Ownership of the duplicate now belongs to the slot; clear it here so the context
        // destructor doesn't also close it.
        context->xuser = nullptr;

        // The slot is now committed to this call. Mark it in-flight so a concurrent AddUser
        // for the same user is rejected rather than racing this one to the deferred commit
        // (covers the initial-add case, where no state existed at AddUserWithUiAsync time).
        gsContext->users[indexFound].addUserInProgress = true;
        context->inFlightClaimOwned = true;

        context->getFolderAsyncStarted = true;
        // If cancel already requested before the inner async was started, propagate immediately
        if (context->cancelRequested)
        {
            TRACE_INFORMATION("PFXPALCallGetFolderWithUiAsync: cancelling due to earlier cancel request");
            XAsyncCancel(&context->getFolderAsyncBlock);
        }
    }
    else
    {
        cleanupOnStartFailure();
        return hr;
    }

    return S_OK;
}

static void CALLBACK PFXPALEntityTokenComplete(XAsyncBlock* async)
{
    // Called after PFEntityGetEntityTokenAsync completes; continue config creation
    PFXPALGetFolderContext* context = static_cast<PFXPALGetFolderContext*>(async->context);
    HRESULT hr = XAsyncGetStatus(async, false);
    TRACE_INFORMATION("PFXPALEntityTokenComplete: XAsyncGetStatus hr=0x%08x, cancelRequested=%s", 
        hr, context->cancelRequested ? "true" : "false");
    if (context->cancelRequested || hr == E_ABORT)
    {
        // Cancellation requested: complete outer async with user cancelled code.
        TRACE_INFORMATION("PFXPALEntityTokenComplete: cancellation or abort, completing with E_ABORT");
        XAsyncComplete(context->clientAsyncBlock, E_ABORT, 0);
        return;
    }
    if (FAILED(hr))
    {
        // Fall back without entity auth
        TRACE_INFORMATION("PFXPALEntityTokenComplete: entity token failed, falling back without entity auth");
        context->useEntityAuth = false;
    }
    else
    {
        size_t resultSize{};
        if (SUCCEEDED(PFEntityGetEntityTokenResultSize(async, &resultSize)) && resultSize > 0)
        {
            std::vector<uint8_t> buffer(resultSize);
            const PFEntityToken* tokenStruct{};
            size_t used{};
            if (SUCCEEDED(PFEntityGetEntityTokenResult(async, buffer.size(), buffer.data(), &tokenStruct, &used)) && tokenStruct && tokenStruct->token && tokenStruct->token[0] != '\0')
            {
                StrCpy(context->entityToken, sizeof(context->entityToken), tokenStruct->token);
                context->useEntityAuth = true;
                TRACE_INFORMATION("PFXPALEntityTokenComplete: entity token obtained successfully, useEntityAuth=true");
            }
            else
            {
                TRACE_INFORMATION("PFXPALEntityTokenComplete: failed to get valid entity token result");
            }
        }
        else
        {
            TRACE_INFORMATION("PFXPALEntityTokenComplete: failed to get entity token result size");
        }
    }

    // entity handle already released after starting token async

    // Continue with config creation (ignore failure to obtain token: fallback)
    TRACE_INFORMATION("PFXPALEntityTokenComplete: continuing with config creation");
    hr = PFXPALCallGetFolderWithUiAsync(context);
    if (FAILED(hr))
    {
        TRACE_INFORMATION("PFXPALEntityTokenComplete: PFXPALCallGetFolderWithUiAsync failed hr=0x%08x", hr);
        XAsyncComplete(context->clientAsyncBlock, hr, 0);
    }
}

static void CALLBACK PFXPALLocalUserLoginComplete(XAsyncBlock* async)
{
    PFXPALGetFolderContext* context = static_cast<PFXPALGetFolderContext*>(async->context);

    HRESULT hr = XAsyncGetStatus(async, false);
    TRACE_INFORMATION("PFXPALLocalUserLoginComplete: XAsyncGetStatus hr=0x%08x, cancelRequested=%s", 
        hr, context->cancelRequested ? "true" : "false");
    if (context->cancelRequested || hr == E_ABORT)
    {
        // Cancellation requested: complete outer async with user cancelled code.
        TRACE_INFORMATION("PFXPALLocalUserLoginComplete: cancellation or abort, completing with E_ABORT");
        XAsyncComplete(context->clientAsyncBlock, E_ABORT, 0);
        return;
    }
    if (FAILED(hr))
    {
        // Fall back without entity auth
        TRACE_INFORMATION("PFXPALLocalUserLoginComplete: login failed, falling back without entity auth");
        context->useEntityAuth = false;
    }
    else
    {
        // Must retrieve the login result to complete XAsync provider cleanup.
        // Without this, the provider is never finalized and PFUninitializeAsync
        // will hang waiting for the unretrieved result payload.
        PFEntityHandle loginEntityHandle{};
        HRESULT getResultHr = PFLocalUserLoginGetResult(async, &loginEntityHandle, 0, nullptr, nullptr, nullptr);
        TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFLocalUserLoginGetResult hr=0x%08x", getResultHr);
        if (loginEntityHandle)
        {
            PFEntityCloseHandle(loginEntityHandle);
        }

        PFEntityHandle localEntityHandle{};
        hr = PFLocalUserTryGetEntityHandle(context->localUserHandle, &localEntityHandle);
        TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFLocalUserTryGetEntityHandle hr=0x%08x", hr);
        if (SUCCEEDED(hr) && localEntityHandle)
        {
            // Entity key
            size_t keySize{};
            HRESULT keyResult = PFEntityGetEntityKeySize(localEntityHandle, &keySize);
            if (SUCCEEDED(keyResult) && keySize > 0 && keySize < 4096)
            {
                std::vector<uint8_t> keyBuffer(keySize);
                const PFEntityKey* key{};
                size_t keyUsed{};
                keyResult = PFEntityGetEntityKey(localEntityHandle, keyBuffer.size(), keyBuffer.data(), &key, &keyUsed);
                if (SUCCEEDED(keyResult) && key && key->id && key->id[0] != '\0')
                {
                    StrCpy(context->entityId, sizeof(context->entityId), key->id);
                    TRACE_INFORMATION("PFXPALLocalUserLoginComplete: entity key obtained successfully, entityId='%s'", key->id);
                }
                else
                {
                    TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFEntityGetEntityKey failed hr=0x%08x", keyResult);
                }
            }
            else
            {
                TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFEntityGetEntityKeySize failed hr=0x%08x or invalid size=%zu", keyResult, keySize);
            }

            // Async token fetch
            context->entityTokenAsyncBlock.queue = context->clientAsyncBlock->queue;
            context->entityTokenAsyncBlock.context = context;
            context->entityTokenAsyncBlock.callback = PFXPALEntityTokenComplete;
            hr = PFEntityGetEntityTokenAsync(localEntityHandle, &context->entityTokenAsyncBlock);
            PFEntityCloseHandle(localEntityHandle); // Close local entity handle immediately pass or fail; async has its own ref internally.
            TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFEntityGetEntityTokenAsync hr=0x%08x", hr);
            if (SUCCEEDED(hr))
            {
                context->entityTokenAsyncStarted = true; // mark started for cancellation tracking
                return; // continuation in callback
            }
            // If token async failed to start, fall through to proceed without entity auth
        }
        else
        {
            TRACE_INFORMATION("PFXPALLocalUserLoginComplete: failed to get entity handle");
        }
    }

    // Continue with config creation (ignore failure to obtain token: fallback)
    TRACE_INFORMATION("PFXPALLocalUserLoginComplete: continuing with config creation");
    hr = PFXPALCallGetFolderWithUiAsync(context);
    if (FAILED(hr))
    {
        TRACE_INFORMATION("PFXPALLocalUserLoginComplete: PFXPALCallGetFolderWithUiAsync failed hr=0x%08x", hr);
        XAsyncComplete(context->clientAsyncBlock, hr, 0);
    }
}

// Performs the AddUser initialization logic
// Steps:
// 1. Retrieve service config & copy titleId/apiEndpoint into context
// 2. Attempt XUser path; if success, proceed to start config+folder
// 3. Fallback: acquire entity handle, key, start async token fetch (continues in callback)
// 4. If entity path unavailable, proceed without auth and start config+folder
static HRESULT PFXPALAddUserBegin(PFXPALGetFolderContext* context)
{
    TRACE_INFORMATION("PFXPALAddUserBegin: starting user initialization");
    RETURN_HR_INVALIDARG_IF_NULL(context);

    HRESULT hr;
    PFServiceConfigHandle serviceConfigHandle{};
    hr = PFLocalUserGetServiceConfigHandle(context->localUserHandle, &serviceConfigHandle);
    TRACE_INFORMATION("PFXPALAddUserBegin: PFLocalUserGetServiceConfigHandle hr=0x%08x", hr);
    RETURN_IF_FAILED(hr);

    char apiEndpoint[1024];
    char playfabTitleId[256];
    hr = PFServiceConfigGetAPIEndpoint(serviceConfigHandle, sizeof(apiEndpoint), apiEndpoint, nullptr);
    if (SUCCEEDED(hr))
    {
        hr = PFServiceConfigGetTitleId(serviceConfigHandle, sizeof(playfabTitleId), playfabTitleId, nullptr);
    }
    PFServiceConfigCloseHandle(serviceConfigHandle);
    TRACE_INFORMATION("PFXPALAddUserBegin: service config retrieval hr=0x%08x", hr);
    RETURN_IF_FAILED(hr);

    // Store common fields in context for later config construction
    StrCpy(context->apiEndpoint, sizeof(context->apiEndpoint), apiEndpoint);
    StrCpy(context->playfabTitleId, sizeof(context->playfabTitleId), playfabTitleId);
    TRACE_INFORMATION("PFXPALAddUserBegin: titleId='%s', apiEndpoint='%s'", playfabTitleId, apiEndpoint);

    // Preferred: XUser path
    XUserHandle xuser{};
    hr = PFLocalUserTryGetXUser(context->localUserHandle, &xuser);
    TRACE_INFORMATION("PFXPALAddUserBegin: PFLocalUserTryGetXUser hr=0x%08x", hr);
    if (SUCCEEDED(hr))
    {
        TRACE_INFORMATION("PFXPALAddUserBegin: using XUser path");
        // NOT borrowed: PFLocalUserTryGetXUser returns an XUserDuplicateHandle copy that the
        // caller owns. Ownership is transferred to the user slot once the folder retrieval
        // starts, and the slot's copy is closed by UninitializeAsync.
        context->xuser = xuser;
        hr = PFXPALCallGetFolderWithUiAsync(context);
        TRACE_INFORMATION("PFXPALAddUserBegin: PFXPALCallGetFolderWithUiAsync hr=0x%08x", hr);
        RETURN_IF_FAILED(hr);
        return S_OK;
    }

    TRACE_INFORMATION("PFXPALAddUserBegin: XUser not available, trying entity auth fallback");
    // Fallback: entity auth
    context->loginLocalUserAsyncBlock.queue = context->clientAsyncBlock->queue;
    context->loginLocalUserAsyncBlock.context = context;
    context->loginLocalUserAsyncBlock.callback = PFXPALLocalUserLoginComplete;
    hr = PFLocalUserLoginAsync(context->localUserHandle, true, &context->loginLocalUserAsyncBlock);
    TRACE_INFORMATION("PFXPALAddUserBegin: PFLocalUserLoginAsync hr=0x%08x", hr);
    if (SUCCEEDED(hr))
    {
        context->loginLocalUserAsyncStarted = true; // track started so we can cancel if needed
        return S_OK; // continuation in callback
    }
    // If token async failed to start, fall through to proceed without entity auth

    TRACE_INFORMATION("PFXPALAddUserBegin: entity auth not available, proceeding without auth");
    // Proceed without entity auth
    hr = PFXPALCallGetFolderWithUiAsync(context);
    TRACE_INFORMATION("PFXPALAddUserBegin: final PFXPALCallGetFolderWithUiAsync hr=0x%08x", hr);
    RETURN_IF_FAILED(hr);
    return S_OK;
}

void CALLBACK PFXPALGetFolderComplete(XAsyncBlock* async)
{
    PFXPALGetFolderContext* context{ static_cast<PFXPALGetFolderContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, false);
    TRACE_INFORMATION("PFXPALGetFolderComplete: XAsyncGetStatus hr=0x%08x, cancelRequested=%s", 
        hr, context->cancelRequested ? "true" : "false");

    // The slot is shared with concurrent AddUser calls and UninitializeAsync, which scan, commit and
    // free slot state under this same mutex. Every read, write and handle close below therefore has
    // to be serialized too - otherwise this teardown can close a PFLocalUserHandle another caller is
    // comparing against. The lookup itself must happen INSIDE the lock: taking it afterwards leaves
    // a window where UninitializeAsync clears the slot and another AddUser reuses it, so 'state'
    // would point at the new user's slot and the rollback below would free the new user's handles.
    // The lock is released before each XAsyncComplete so the title's completion callback can call
    // back into the game save APIs without deadlocking.
    std::unique_lock<std::mutex> slotLock{ GameSaveUserSlotMutex() };

    // Looked up once up front so every exit below can release the add-user in-flight claim.
    // Leaving it set would lock the user out of AddUserWithUiAsync permanently.
    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUserUnsafe(context->localUserHandle);

    auto releaseInFlight = [state, context]() // must be called with slotLock held
    {
        if (state != nullptr && context->inFlightClaimOwned.exchange(false))
        {
            state->addUserInProgress = false;
        }
    };

    // Undo the slot commit performed by PFXPALCallGetFolderWithUiAsync. Must be called with
    // slotLock held. A committed-but-failed slot keeps a non-null configHandle, which makes every
    // later AddUserWithUiAsync return E_PF_GAMESAVE_USER_ALREADY_ADDED for the rest of the
    // process. On a reconnect the pre-existing offline state has to survive instead.
    auto rollbackSlot = [state, context](const char* previousSaveFolder)
    {
        if (state == nullptr)
        {
            return;
        }

        if (context->reusedExistingSlot)
        {
            if (previousSaveFolder != nullptr)
            {
                StrCpy(state->saveFolder, sizeof(state->saveFolder), previousSaveFolder);
            }
            state->isConnectedToCloud = false;
            return;
        }

        state->saveFolder[0] = '\0';
        if (state->configHandle != nullptr)
        {
            PFXGameSaveFreeConfig(static_cast<PFXGameSaveConfigHandle>(state->configHandle));
            state->configHandle = nullptr;
        }
        if (state->xUser != nullptr)
        {
            XUserCloseHandle(state->xUser);
            state->xUser = nullptr;
        }
        if (state->localUser != nullptr)
        {
            PFLocalUserCloseHandle(state->localUser);
            state->localUser = nullptr;
        }
        state->isConnectedToCloud = true; // reset to the struct default for a fresh attempt
    };

    // Only a GAME-initiated cancel is an unconditional hard abort. A bare E_ABORT is NOT
    // (see the offline-abort race handling below).
    if (context->cancelRequested)
    {
        // Game cancelled the XAsync task. Return E_ABORT with no save path. The slot may already
        // have been committed by the start path, so roll it back - otherwise an initial AddUser
        // leaves a zombie slot that rejects every retry, and a reconnect loses its offline folder.
        TRACE_INFORMATION("PFXPALGetFolderComplete: game cancelled, completing with E_ABORT");
        rollbackSlot(state != nullptr ? state->saveFolder : nullptr);
        releaseInFlight();
        slotLock.unlock();
        XAsyncComplete(asyncBlock, E_ABORT, 0);
        return;
    }

    bool isConnectedToCloud = true;
    if (hr == E_GS_USER_CANCELED)
    {
        // GRTS returns E_GS_USER_CANCELED when user chose UseOffline or cancelled via stock UI.
        // We still retrieve the folder and return S_OK, but note the user is offline.
        TRACE_INFORMATION("PFXPALGetFolderComplete: E_GS_USER_CANCELED (user went offline via stock UI)");
        isConnectedToCloud = false;
        hr = S_OK;
    }
    else if (hr == E_ABORT)
    {
        // ADO 63266145 / 63185188. ConnectedStorage can deliver a LATE, no-op cancel AFTER the
        // offline flow has already completed successfully. ETL for the "Play offline" path shows:
        //   PFContextSyncRetry(RetrySync:false) -> PFContextOfflinePreserveVersionWithData
        //   -> PFCopyFilesResult(OfflineCopy) -> PFContextCreated -> PFActivatorCanceled
        //     (state already Complete => Cancel() is a no-op)
        // Two UI callbacks are queued (progress + retry); once the retry resolves offline, the
        // other can resolve late against an already-completed operation and drive E_ABORT, which
        // previously overrode a perfectly good offline result. The user's choice WAS honored and
        // the offline container IS ready, so do not blindly fail: probe for the save folder and
        // only treat this as a real abort if no folder is available.
        TRACE_INFORMATION("PFXPALGetFolderComplete: E_ABORT without game cancel - probing for offline save folder (late no-op cancel race)");
        isConnectedToCloud = false;
        hr = S_OK;
    }
    if (FAILED(hr))
    {
        // The status call reported a genuine failure (not the offline/cancel cases converted to
        // S_OK above). The slot was already committed when the inner async started, so roll it
        // back here too - the result-call failure path below does the same thing.
        TRACE_INFORMATION("PFXPALGetFolderComplete: add-user failed (status hr=0x%08x), rolling back slot", hr);
        rollbackSlot(state != nullptr ? state->saveFolder : nullptr);
        releaseInFlight();
        slotLock.unlock();
        XAsyncComplete(asyncBlock, hr, 0);
        return;
    }

    if (state == nullptr)
    {
        hr = E_PF_GAMESAVE_USER_NOT_ADDED;
        TRACE_INFORMATION("PFXPALGetFolderComplete: user state not found, failing with E_PF_GAMESAVE_USER_NOT_ADDED");
        slotLock.unlock();
        HANDLE_XASYNC_FAILURE(hr);
    }

    // PFXGameSaveFilesGetFolderWithUiResult is documented to overwrite state->saveFolder, but
    // clear it explicitly so the "did we get a usable folder" probe below can only ever be
    // satisfied by a value THIS call produced - on a reconnect the buffer still holds the
    // previous session's offline folder, which would otherwise pass the check.
    char previousSaveFolder[sizeof(state->saveFolder)];
    StrCpy(previousSaveFolder, sizeof(previousSaveFolder), state->saveFolder);
    state->saveFolder[0] = '\0';

    hr = PFXGameSaveFilesGetFolderWithUiResult(async, sizeof(state->saveFolder), state->saveFolder);

    // The "user went offline" signal can surface at EITHER stage: on the async status
    // (XAsyncGetStatus, handled above) or here on the result call. Observed on desktop GRTS:
    // the status is S_OK and the cancel is reported only by the result call.
    const bool cancelLike = (hr == E_GS_USER_CANCELED || hr == E_ABORT);
    if (cancelLike)
    {
        isConnectedToCloud = false;
    }

    if (!isConnectedToCloud && (SUCCEEDED(hr) || (cancelLike && state->saveFolder[0] != '\0')))
    {
        // Offline path (user chose "Play offline", or the late no-op cancel race). Accept the
        // cancel-like HRESULT as success because GRTS handed back a usable offline save folder.
        //
        // Without this, the offline-success branch was unreachable: it set hr = S_OK and then this
        // result call overwrote hr with E_GS_USER_CANCELED, so the failure handling below failed
        // the whole add-user. That is why every offline AddUser surfaced as a hard failure
        // (ADO 63266145) and isConnectedToCloud was never recorded as false (ADO 63185188 #3).
        TRACE_INFORMATION("PFXPALGetFolderComplete: OFFLINE SUCCESS (result hr=0x%08x, folder='%s')",
            hr, state->saveFolder);
        hr = S_OK;
    }

    // Single failure path for EVERY failing result, not just the cancel-like ones. A generic
    // failure (E_FAIL, E_OUTOFMEMORY, transport error) leaves isConnectedToCloud true and
    // cancelLike false, and previously fell straight through to HANDLE_XASYNC_FAILURE without
    // any cleanup - leaving a non-null configHandle with no user added (the exact state that
    // makes the next AddUser return E_PF_GAMESAVE_USER_ALREADY_ADDED), and on a reconnect
    // leaving a clobbered save folder behind.
    if (FAILED(hr))
    {
        TRACE_INFORMATION("PFXPALGetFolderComplete: add-user failed (result hr=0x%08x), rolling back slot (reconnect=%d)",
            hr, context->reusedExistingSlot);
        // On a reconnect this restores the previous offline folder and marks the user offline; on
        // an initial add it releases everything the start path registered.
        rollbackSlot(previousSaveFolder);

        releaseInFlight();
        slotLock.unlock();
        // Preserve the existing E_ABORT contract for user-cancel style failures; propagate the
        // real HRESULT for genuine errors so callers can tell them apart.
        XAsyncComplete(asyncBlock, cancelLike ? E_ABORT : hr, 0);
        return;
    }

    state->isConnectedToCloud = isConnectedToCloud;
    releaseInFlight();
    TRACE_INFORMATION("PFXPALGetFolderComplete: folder obtained, isConnectedToCloud=%s, completing with S_OK",
        state->isConnectedToCloud ? "true" : "false");
    slotLock.unlock();
    XAsyncComplete(asyncBlock, S_OK, 0);
}

HRESULT GameSaveAPIProviderGRTS::AddUserWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesAddUserOptions options,
    _Inout_ XAsyncBlock* async
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: options=%d", static_cast<int>(options));
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

    // Check if the user is already added by looking for existing state with an active config.
    // Without this guard, calling GRTS a second time can hang indefinitely on Xbox
    // waiting for a TCUI that never resolves. This matches the Win32 provider behavior
    // which returns E_PF_GAMESAVE_USER_ALREADY_ADDED in the same situation.
    //
    // EXCEPTION - offline reconnect. PFGameSaveFiles.h documents:
    //   "When disconnected from cloud, PFGameSaveFilesAddUserWithUiAsync() can be called again if
    //    you want to try connect to the cloud. ... No need to re-init gamesave but you can if desired."
    // So when the user is currently disconnected from cloud (they chose "Play offline"), a repeat
    // AddUser is the documented way to re-sync once the network is back and must NOT be rejected.
    // Rejecting it broke the entire offline->online re-sync path (ADO 63185673 / 63185188).
    //
    // The lookup, the addUserInProgress test and the claim must all happen under the slot mutex:
    // they are one test-and-set over shared state, and splitting them lets two callers both decide
    // they own the reconnect. The scope ends before any async work starts because
    // PFXPALCallGetFolderWithUiAsync takes the same (non-recursive) lock.
    bool claimedInFlightSlot = false;
    {
        std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
        PFXPALGameSaveUserState* existingState = PFXPALGetStateFromLocalUserUnsafe(localUserHandle);

        // Reject overlapping AddUser calls for the same user. See PFXPALGameSaveUserState::
        // addUserInProgress - without this, two reconnect attempts can both fall through the
        // disconnected exception below, both take the reusingExistingSlot path, and the second
        // will free the first call's still-live config/xUser (use-after-free inside GRTS).
        if (existingState != nullptr && existingState->addUserInProgress)
        {
            TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: an add-user is already in flight for this user, rejecting re-entrant call");
            return E_PF_GAMESAVE_USER_ALREADY_ADDED;
        }

        if (existingState != nullptr && existingState->configHandle != nullptr)
        {
            if (existingState->isConnectedToCloud)
            {
                TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: user already added (configHandle=%p, saveFolder='%s'), returning E_PF_GAMESAVE_USER_ALREADY_ADDED",
                    existingState->configHandle, existingState->saveFolder);
                return E_PF_GAMESAVE_USER_ALREADY_ADDED;
            }

            TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: user already added but DISCONNECTED FROM CLOUD - allowing documented reconnect re-sync (configHandle=%p, saveFolder='%s')",
                existingState->configHandle, existingState->saveFolder);

            // Claim the in-flight slot before any async work starts, so a second call racing this
            // one is rejected above rather than racing us to the deferred commit.
            existingState->addUserInProgress = true;
            claimedInFlightSlot = true;
        }
    }

    std::unique_ptr<PFXPALGetFolderContext> context = std::make_unique<PFXPALGetFolderContext>();
    context->addUserOptions = options; // Store options for later use
    context->claimedInFlightSlot = claimedInFlightSlot;
    context->inFlightClaimOwned = claimedInFlightSlot;
    if (!m_initSaveFolder.empty())
    {
        StrCpy(context->saveFolderOverride, sizeof(context->saveFolderOverride), m_initSaveFolder.c_str());
        // Ensure trailing backslash
        size_t len = strlen(context->saveFolderOverride);
        if (len > 0 && len < sizeof(context->saveFolderOverride) - 2 &&
            context->saveFolderOverride[len - 1] != '\\' && context->saveFolderOverride[len - 1] != '/')
        {
            context->saveFolderOverride[len] = '\\';
            context->saveFolderOverride[len + 1] = '\0';
        }
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: using save folder override='%s'", context->saveFolderOverride);
    }
    context->clientAsyncBlock = async;
    context->localUserHandle = localUserHandle;

    HRESULT hr = XAsyncBegin(
        async,
        context.get(),
        &PFGameSaveFilesAddUserWithUiAsync,
        "PFGameSaveFilesAddUserWithUiAsync",
        [](_In_ XAsyncOp op, _In_ XAsyncProviderData const* data)
        {
            try
            {
                PFXPALGetFolderContext* context{ static_cast<PFXPALGetFolderContext*>(data->context) };

                if (op == XAsyncOp::Begin)
                {
                    TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: XAsyncOp::Begin");
                    RETURN_IF_FAILED(PFXPALAddUserBegin(context));
                    return S_OK;
                }
                else if (op == XAsyncOp::Cancel)
                {
                    TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: XAsyncOp::Cancel");
                    // Mark cancellation intent and propagate to any started inner async operations
                    context->cancelRequested = true;
                    if (context->loginLocalUserAsyncStarted)
                    {
                        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: cancelling loginLocalUserAsync");
                        XAsyncCancel(&context->loginLocalUserAsyncBlock);
                    }
                    if (context->entityTokenAsyncStarted)
                    {
                        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: cancelling entityTokenAsync");
                        XAsyncCancel(&context->entityTokenAsyncBlock);
                    }
                    if (context->getFolderAsyncStarted)
                    {
                        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: cancelling getFolderAsync");
                        XAsyncCancel(&context->getFolderAsyncBlock);
                    }
                    // If nothing started yet, complete immediately
                    if (!context->loginLocalUserAsyncStarted && !context->entityTokenAsyncStarted && !context->getFolderAsyncStarted)
                    {
                        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: no operations started, completing cancel immediately");
                        XAsyncComplete(data->async, E_ABORT, 0);
                    }
                }
                else if (op == XAsyncOp::Cleanup)
                {
                    TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: XAsyncOp::Cleanup");
                    std::unique_ptr<PFXPALGetFolderContext> contextPtr{ static_cast<PFXPALGetFolderContext*>(data->context) };

                    // Last chance to release the in-flight claim. PFXPALGetFolderComplete normally
                    // does it, but it never runs when the operation failed before the GRTS folder
                    // async started (config init, queue creation, entity-token failure, ...) - and
                    // a stuck claim locks the user out of AddUser for the rest of the process.
                    if (contextPtr->inFlightClaimOwned.exchange(false))
                    {
                        std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
                        PFXPALGameSaveUserState* claimedState = PFXPALGetStateFromLocalUserUnsafe(contextPtr->localUserHandle);
                        if (claimedState != nullptr)
                        {
                            TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: releasing add-user claim during cleanup");
                            claimedState->addUserInProgress = false;
                        }
                    }
                }
            }
            catch (...)
            {
                return CurrentExceptionToHR();
            }

            return S_OK;
        });

    if (SUCCEEDED(hr))
    {
        context.release();
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: XAsyncBegin succeeded, returning S_OK");
    }
    else
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiAsync: XAsyncBegin failed hr=0x%08x", hr);
        // XAsyncBegin failing means Cleanup never ran, so release the claim here instead.
        if (context != nullptr && context->inFlightClaimOwned.exchange(false))
        {
            std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
            PFXPALGameSaveUserState* claimedState = PFXPALGetStateFromLocalUserUnsafe(localUserHandle);
            if (claimedState != nullptr)
            {
                claimedState->addUserInProgress = false;
            }
        }
    }
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::AddUserWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    HRESULT hr = XAsyncGetStatus(async, false);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::AddUserWithUiResult: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::GetFolderSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* saveRootFolderSize
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

    UNREFERENCED_PARAMETER(localUserHandle);
    if (saveRootFolderSize != nullptr)
    {
        PFXPALGameSaveUserStateSnapshot state{};
        if (!PFXPALTryGetUserStateSnapshot(localUserHandle, state))
        {
            TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolderSize: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
            return E_PF_GAMESAVE_USER_NOT_ADDED;
        }

        *saveRootFolderSize = strlen(state.saveFolder) + 1;
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolderSize: returning size=%zu", *saveRootFolderSize);
    }
    else
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolderSize: saveRootFolderSize is null");
    }
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::GetFolder(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t saveRootFolderSize,
    _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
    _Out_opt_ size_t* saveRootFolderUsed
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolder: saveRootFolderSize=%zu", saveRootFolderSize);
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(saveRootFolderBuffer);

    PFXPALGameSaveUserStateSnapshot state{};
    if (!PFXPALTryGetUserStateSnapshot(localUserHandle, state))
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolder: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    size_t folderSize = strlen(state.saveFolder) + 1;
    if (saveRootFolderSize < folderSize)
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolder: buffer too small, need %zu but got %zu", folderSize, saveRootFolderSize);
        return E_INVALIDARG;
    }

    memcpy(saveRootFolderBuffer, state.saveFolder, folderSize);
    if (saveRootFolderUsed)
    {
        *saveRootFolderUsed = folderSize;
    }

    TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetFolder: returning folder='%s', used=%zu", state.saveFolder, folderSize);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::UploadWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUploadOption option,
    _Inout_ XAsyncBlock* async
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: option=%d", static_cast<int>(option));
    PFXPALGameSaveUserStateSnapshot state{};
    if (!PFXPALTryGetUserStateSnapshot(localUserHandle, state))
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    // ADO 63185188 (#1/#2): while the user is disconnected from cloud (they chose "Play offline"),
    // an upload must NOT re-enter the WithUi sync path - doing so re-raises the stock sync-failure
    // dialog on EVERY save. The documented contract in PFGameSaveFiles.h is:
    //   "While disconnected from cloud, PFGameSaveFilesUploadWithUiAsync() will not do anything
    //    but return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD"
    // The Win32 provider already honors this; GRTS did not. Fail silently and leave the data on
    // disk so it uploads on the next reconnect / launch.
    if (!state.isConnectedToCloud)
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: disconnected from cloud - skipping upload+UI, returning E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD");
        return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
    }

    PFXGameSaveUploadOptions pfxoptions = PFXGameSaveUploadOptions::UploadKeepActive;
    if (option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
    {
        pfxoptions = PFXGameSaveUploadOptions::UploadReleaseActive;
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: using UploadReleaseActive");
    }
    else
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: using UploadKeepActive");
    }
    PFXGameSaveConfigHandle configHandle = static_cast<PFXGameSaveConfigHandle>(state.configHandle);
    HRESULT hr = PFXGameSaveFilesUploadWithUiAsync(configHandle, pfxoptions, async);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiAsync: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::UploadWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    HRESULT hr = PFXGameSaveFilesUploadWithUiResult(async);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UploadWithUiResult: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetSaveDescriptionAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ const char* shortSaveDescription,
    _In_ XAsyncBlock* async) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetSaveDescriptionAsync: shortSaveDescription='%s'", 
        shortSaveDescription ? shortSaveDescription : "(null)");
    PFXPALGameSaveUserStateSnapshot state{};
    if (!PFXPALTryGetUserStateSnapshot(localUserHandle, state))
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetSaveDescriptionAsync: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    HRESULT hr = PFXGameSaveFilesSetSaveDescriptionAsync(
        static_cast<PFXGameSaveConfigHandle>(state.configHandle),
        shortSaveDescription,
        async);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetSaveDescriptionAsync: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept
{
    HRESULT hr = PFXGameSaveFilesSetSaveDescriptionResult(async);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetSaveDescriptionResult: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::GetRemainingQuota(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ int64_t* remainingQuota
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetRemainingQuota called");
    PFXPALGameSaveUserStateSnapshot state{};
    bool haveState = PFXPALTryGetUserStateSnapshot(localUserHandle, state);
    PFXGameSaveConfigHandle configHandle = haveState ? static_cast<PFXGameSaveConfigHandle>(state.configHandle) : nullptr;
    if(configHandle == nullptr )
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetRemainingQuota: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    HRESULT hr = PFXGameSaveFilesGetRemainingQuota(configHandle, remainingQuota);
    if (SUCCEEDED(hr) && remainingQuota)
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetRemainingQuota: quota=%lld", *remainingQuota);
    }
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::GetRemainingQuota: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::IsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(isConnectedToCloud);

    PFXPALGameSaveUserStateSnapshot state{};
    if (!PFXPALTryGetUserStateSnapshot(localUserHandle, state))
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::IsConnectedToCloud: user not added, returning E_PF_GAMESAVE_USER_NOT_ADDED");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    *isConnectedToCloud = state.isConnectedToCloud;
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::IsConnectedToCloud: connected=%s", state.isConnectedToCloud ? "true" : "false");
    return S_OK;
}

// Global bridge for active device changed notifications from PFX -> PF
static void CALLBACK PFXPALActiveDeviceChangedCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveDescriptor* activeDevice,
    _In_opt_ void* ctx)
{
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(ctx);
    if (!gsContext)
    {
        TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: no context");
        return;
    }
    auto cb = gsContext->activeDeviceChangedCallback.load(std::memory_order_acquire);
    if (!cb)
    {
        TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: no registered callback");
        return; // no registered callback
    }
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, nullptr);
    if (localUser == nullptr)
    {
        TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: user not tracked");
        return; // user not tracked
    }
    PFGameSaveDescriptor pfDesc{};
    ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(activeDevice, &pfDesc);
    
    TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: converted descriptor, time=%lld, totalBytes=%llu", 
        pfDesc.time, pfDesc.totalBytes);
    
    // If a queue handle is supplied, schedule via TaskQueue; otherwise invoke directly.
    // (Passing a null XTaskQueueHandle to XTaskQueueSubmitDelayedCallback is not valid, so we guard.)
    XTaskQueueHandle queueHandle = gsContext->activeDeviceChangedCallbackQueue.load(std::memory_order_relaxed);
    void* userCtx = gsContext->activeDeviceChangedContext.load(std::memory_order_relaxed);

    if (queueHandle)
    {
        TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: using task queue for callback");
        struct ActiveDeviceChangedDispatchContext
        {
            PFLocalUserHandle localUser;
            PFGameSaveDescriptor descriptor;
            PFXPALGameSaveContext* gsContext;
            ActiveDeviceChangedDispatchContext(PFLocalUserHandle l, const PFGameSaveDescriptor& d, PFXPALGameSaveContext* g) noexcept : localUser(l), descriptor(d), gsContext(g) {}
        };

        // Ownership / lifetime notes for ActiveDeviceChangedDispatchContext:
        // 1. We create a UniquePtr (contextPtr) holding the newly allocated context via MakeUnique.
        // 2. If XTaskQueueSubmitDelayedCallback succeeds:
        //      a. We call contextPtr.release(); the XTaskQueue runtime now "owns" the raw pointer
        //         purely by virtue of passing it as the void* context argument to the queued callback.
        //      b. When the queued work runs (or is cancelled), dispatchFn wraps the raw pointer
        //         back into a UniquePtr<CtxT>, ensuring deterministic destruction.
        // 3. If XTaskQueueSubmitDelayedCallback fails:
        //      a. We reclaim the pointer (still owned by contextPtr), move it into a reclaim UniquePtr,
        //         and synchronously invoke the user callback (behaving as though no queue was provided).
        // 4. Cancellation path: The queue infrastructure invokes dispatchFn with cancelled=true. We
        //      still reclaim and destroy the context (RAII) but we skip invoking the user callback.
        // 5. Descriptor safety: We copy the descriptor into the context; dispatchFn makes another
        //      local copy (descCopy) before invoking the user callback to avoid issues if the title
        //      mutates memory pointed to by the descriptor or re-enters SetActiveDeviceChangedCallback.
        // This mirrors RunContext's TaskQueue submission pattern for clarity & consistency.
        auto dispatchFn = [](void* c, bool cancelled) noexcept
        {
            using CtxT = ActiveDeviceChangedDispatchContext;
            UniquePtr<CtxT> ctx{ static_cast<CtxT*>(c) };
            if (!cancelled && ctx->gsContext)
            {
                // Load the callback with acquire semantics to pair with publisher's release.
                auto cb = ctx->gsContext->activeDeviceChangedCallback.load(std::memory_order_acquire);
                if (cb)
                {
                    PFGameSaveDescriptor descCopy = ctx->descriptor; // defensive copy
                    void* userCtx = ctx->gsContext->activeDeviceChangedContext.load(std::memory_order_relaxed);
                    cb(ctx->localUser, &descCopy, userCtx);
                }
            }
        };

        auto contextPtr = MakeUnique<ActiveDeviceChangedDispatchContext>(localUser, pfDesc, gsContext);
        ActiveDeviceChangedDispatchContext* raw = contextPtr.get();
        HRESULT hr = XTaskQueueSubmitDelayedCallback(
            queueHandle,
            XTaskQueuePort::Work,
            0,
            raw,
            dispatchFn);
        TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: XTaskQueueSubmitDelayedCallback hr=0x%08x", hr);
        if (FAILED(hr))
        {
            // Failed to enqueue: reclaim ownership and invoke synchronously (treat as if queue not provided)
            UniquePtr<ActiveDeviceChangedDispatchContext> reclaim{ contextPtr.release() };
            PFGameSaveDescriptor descCopy = reclaim->descriptor; // defensive copy
            auto cb2 = gsContext->activeDeviceChangedCallback.load(std::memory_order_acquire);
            void* userCtx2 = gsContext->activeDeviceChangedContext.load(std::memory_order_relaxed);
            if (cb2)
            {
                cb2(localUser, &descCopy, userCtx2);
            }
            return;
        }
        contextPtr.release(); // ownership transferred to queue callback
        return;
    }

    // No queue supplied: invoke synchronously
    TRACE_INFORMATION("PFXPALActiveDeviceChangedCallback: invoking callback synchronously");
    cb(localUser, &pfDesc, userCtx);
}

HRESULT GameSaveAPIProviderGRTS::SetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback, _In_opt_ void* context
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetActiveDeviceChangedCallback: callback=%s", 
        callback ? "provided" : "null");
    // Store callback in shared context so we can invoke when PFX layer notifies us.
    void* gsContextPtr = nullptr;
    HRESULT hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);
    // Order: store context & queue first, then publish callback with release so reader sees initialized fields.
    gsContext->activeDeviceChangedContext.store(context, std::memory_order_relaxed);
    gsContext->activeDeviceChangedCallbackQueue.store(callbackQueue, std::memory_order_relaxed);
    gsContext->activeDeviceChangedCallback.store(callback, std::memory_order_release);

    // Register or unregister global bridge with PFX layer
    if (callback)
    {
        hr = PFXGameSaveSetActiveDeviceChangedCallback(PFXPALActiveDeviceChangedCallback, gsContext);
    }
    else
    {
        hr = PFXGameSaveSetActiveDeviceChangedCallback(nullptr, gsContext);
    }
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetActiveDeviceChangedCallback: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::UninitializeAsync(
    _Inout_ XAsyncBlock* async
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UninitializeAsync");
    void* gsContextPtr = nullptr;
    HRESULT hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);

    ULONG usersProcessed = 0;
    {
        // Same invariant as the rest of the slot access: an in-flight AddUser completion runs the
        // identical test-then-free sequence on these handles under this mutex, so tearing them
        // down unlocked would double-free. Released before CleanupAsync.
        std::lock_guard<std::mutex> slotLock{ GameSaveUserSlotMutex() };
        for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
        {
            if (gsContext->users[i].localUser != nullptr)
            {
                PFLocalUserCloseHandle(gsContext->users[i].localUser);
                gsContext->users[i].localUser = nullptr;
                usersProcessed++;
            }

            if (gsContext->users[i].xUser != nullptr)
            {
                XUserCloseHandle(gsContext->users[i].xUser);
                gsContext->users[i].xUser = nullptr;
            }

            if (gsContext->users[i].configHandle != nullptr)
            {
                PFXGameSaveConfigHandle serviceConfigHandle = static_cast<PFXGameSaveConfigHandle>(gsContext->users[i].configHandle);
                PFXGameSaveFreeConfig(serviceConfigHandle);
                gsContext->users[i].configHandle = nullptr;
            }

            gsContext->users[i].addUserInProgress = false;
        }
    }

    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UninitializeAsync: cleaned up %u users", usersProcessed);
    hr = GameSaveGlobalState::CleanupAsync(async);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UninitializeAsync: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::UninitializeResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    HRESULT hr = XAsyncGetStatus(async, false);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::UninitializeResult: hr=0x%08x", hr);
    return hr;
}

void CALLBACK MyPFXPALGameSaveProgressUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveSyncState syncState,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXPALGameSaveProgressUiCallback: syncState=%d", static_cast<int>(syncState));
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        if (gsContext->progressCallback)
        {
            PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(syncState);
            gsContext->progressCallback(localUser, pfSyncState, context);
        }
    }
    else
    {
        TRACE_INFORMATION("MyPFXPALGameSaveProgressUiCallback: local user not found");
    }
}

void CALLBACK MyPFXPALGameSaveSyncFailedUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveSyncState syncState,
    _In_ HRESULT error,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXPALGameSaveSyncFailedUiCallback: syncState=%d, error=0x%08x", 
        static_cast<int>(syncState), error);
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        if (gsContext->syncFailedCallback)
        {
            PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(syncState);
            gsContext->syncFailedCallback(localUser, pfSyncState, error, context);
        }
    }
    else
    {
        TRACE_INFORMATION("MyPFXPALGameSaveSyncFailedUiCallback: local user not found");
    }
}

void CALLBACK MyPFXPALGameSaveActiveDeviceContentionUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveDescriptor* localGameSave, 
    _In_ PFXGameSaveDescriptor* remoteGameSave,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXPALGameSaveActiveDeviceContentionUiCallback");
    if (localGameSave)
    {
        TRACE_INFORMATION("MyPFXPALGameSaveActiveDeviceContentionUiCallback: local time=%lld, totalBytes=%llu", 
            static_cast<long long>(localGameSave->time), localGameSave->totalBytes);
    }
    if (remoteGameSave)
    {
        TRACE_INFORMATION("MyPFXPALGameSaveActiveDeviceContentionUiCallback: remote time=%lld, totalBytes=%llu", 
            remoteGameSave->time, remoteGameSave->totalBytes);
    }
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveDescriptor pfLocalGameSave = {};
        PFGameSaveDescriptor pfRemoteGameSave = {};
        if (gsContext->activeDeviceContentionCallback)
        {
            ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(localGameSave, &pfLocalGameSave);
            ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(remoteGameSave, &pfRemoteGameSave);
            gsContext->activeDeviceContentionCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
        }
    }
    else
    {
        TRACE_INFORMATION("MyPFXPALGameSaveActiveDeviceContentionUiCallback: local user not found");
    }
}

void CALLBACK MyPFXGameSaveActiveDeviceContentionUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ time_t localTime,
    _In_ time_t remoteTime,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXGameSaveActiveDeviceContentionUiCallback: localTime=%lld, remoteTime=%lld", 
        static_cast<long long>(localTime), static_cast<long long>(remoteTime));
    PFXGameSaveDescriptor localGameSave{};
    localGameSave.time = localTime;

    PFXGameSaveDescriptor remoteGameSave{};
    remoteGameSave.time = remoteTime;

    return MyPFXPALGameSaveActiveDeviceContentionUiCallback(
        requestingUser,
        &localGameSave,
        &remoteGameSave,
        context);
}


void CALLBACK MyPFXPALGameSaveConflictUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveDescriptor* localGameSave, 
    _In_ PFXGameSaveDescriptor* remoteGameSave,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXPALGameSaveConflictUiCallback");
    if (localGameSave)
    {
        TRACE_INFORMATION("MyPFXPALGameSaveConflictUiCallback: local time=%lld, totalBytes=%llu", 
            localGameSave->time, localGameSave->totalBytes);
    }
    if (remoteGameSave)
    {
        TRACE_INFORMATION("MyPFXPALGameSaveConflictUiCallback: remote time=%lld, totalBytes=%llu", 
            remoteGameSave->time, remoteGameSave->totalBytes);
    }
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveDescriptor pfLocalGameSave = {};
        PFGameSaveDescriptor pfRemoteGameSave = {};
        if (gsContext->conflictCallback)
        {
            ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(localGameSave, &pfLocalGameSave);
            ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(remoteGameSave, &pfRemoteGameSave);
            gsContext->conflictCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
        }
    }
    else
    {
        TRACE_INFORMATION("MyPFXPALGameSaveConflictUiCallback: local user not found");
    }
}

void CALLBACK MyPFXGameSaveConflictUiCallback(
    _In_ XUserHandle requestingUser, 
    _In_ time_t localModifiedTime,
    _In_ time_t remoteModifiedTime, 
    _In_ uint64_t localSize, 
    _In_ uint64_t remoteSize, 
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXGameSaveConflictUiCallback: localTime=%lld, remoteTime=%lld, localSize=%llu, remoteSize=%llu", 
        localModifiedTime, remoteModifiedTime, localSize, remoteSize);
    PFXGameSaveDescriptor localGameSave{};
    localGameSave.time = localModifiedTime;
    localGameSave.totalBytes = localSize;

    PFXGameSaveDescriptor remoteGameSave{};
    remoteGameSave.time = remoteModifiedTime;
    remoteGameSave.totalBytes = remoteSize;

    return MyPFXPALGameSaveConflictUiCallback(
        requestingUser,
        &localGameSave,
        &remoteGameSave,
        context);
}


void CALLBACK MyPFXPALGameSaveOutOfStorageUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ bool isUserPartition,
    uint64_t requiredBytes,
    _In_opt_ void* context)
{
    TRACE_INFORMATION("MyPFXPALGameSaveOutOfStorageUiCallback: isUserPartition=%s, requiredBytes=%llu", 
        isUserPartition ? "true" : "false", requiredBytes);
    UNREFERENCED_PARAMETER(isUserPartition); // titles don't need to know this bit of info

    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        if (gsContext->outOfStorageCallback)
        {
            gsContext->outOfStorageCallback(localUser, requiredBytes, context);
        }
    }
    else
    {
        TRACE_INFORMATION("MyPFXPALGameSaveOutOfStorageUiCallback: local user not found");
    }
}

HRESULT GameSaveAPIProviderGRTS::SetUiCallbacks(
    _In_ PFGameSaveUICallbacks* callbacks
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiCallbacks: callbacks: progress=%s, syncFailed=%s, activeDeviceContention=%s, conflict=%s, outOfStorage=%s",
        callbacks->progressCallback ? "set" : "null",
        callbacks->syncFailedCallback ? "set" : "null", 
        callbacks->activeDeviceContentionCallback ? "set" : "null",
        callbacks->conflictCallback ? "set" : "null",
        callbacks->outOfStorageCallback ? "set" : "null");
    
    // Remember the callbacks in the PFXPALGameSaveContext object
    void* context = nullptr;
    RETURN_IF_FAILED(PFPlatformGetGameSaveContext(&context));
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(context);
    gsContext->progressCallback = callbacks->progressCallback;
    gsContext->syncFailedCallback = callbacks->syncFailedCallback;
    gsContext->activeDeviceContentionCallback = callbacks->activeDeviceContentionCallback;
    gsContext->conflictCallback = callbacks->conflictCallback;
    gsContext->outOfStorageCallback = callbacks->outOfStorageCallback;

    // Set PAL callbacks that will translate from PFX data to PF data and call the title's callbacks
    PFXGameSaveUiCallbacks pfxcallbacks = {};
    pfxcallbacks.progressCallback = MyPFXPALGameSaveProgressUiCallback;
    pfxcallbacks.progressContext = callbacks->progressContext;
    pfxcallbacks.syncFailedCallback = MyPFXPALGameSaveSyncFailedUiCallback;
    pfxcallbacks.syncFailedContext = callbacks->syncFailedContext;
    pfxcallbacks.activeDeviceContentionCallback = MyPFXGameSaveActiveDeviceContentionUiCallback; 
    pfxcallbacks.activeDeviceContentionContext = callbacks->activeDeviceContentionContext;
    pfxcallbacks.activeDeviceContentionWithDescriptorCallback = MyPFXPALGameSaveActiveDeviceContentionUiCallback;
    pfxcallbacks.activeDeviceContentionWithDescriptorContext = callbacks->activeDeviceContentionContext;
    pfxcallbacks.conflictCallback = MyPFXGameSaveConflictUiCallback; 
    pfxcallbacks.conflictContext = callbacks->conflictContext;
    pfxcallbacks.conflictWithDescriptorCallback = MyPFXPALGameSaveConflictUiCallback;
    pfxcallbacks.conflictWithDescriptorContext = callbacks->conflictContext;
    pfxcallbacks.outOfStorageCallback = MyPFXPALGameSaveOutOfStorageUiCallback;
    pfxcallbacks.outOfStorageContext = callbacks->outOfStorageContext;

    HRESULT hr = PFXGameSaveSetUiCallbacks(&pfxcallbacks);
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiCallbacks: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::UiProgressGetProgress(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_opt_ PFGameSaveFilesSyncState* syncState,
    _Out_opt_ uint64_t* current,
    _Out_opt_ uint64_t* total) noexcept
{
    // Value-initialize and only publish to the caller's buffers on success: on failure GRTS leaves
    // these untouched, so copying them out would hand the title indeterminate state alongside a
    // failed HRESULT.
    PFXGameSaveSyncState pfxSyncState{};
    uint64_t localCurrent{ 0 };
    uint64_t localTotal{ 0 };
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveProgressUiGetProgress(requestingUser, &pfxSyncState, &localCurrent, &localTotal);
    PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(pfxSyncState);

    if (SUCCEEDED(hr))
    {
        if (syncState != nullptr)
        {
            *syncState = pfSyncState;
        }
        if (current != nullptr)
        {
            *current = localCurrent;
        }
        if (total != nullptr)
        {
            *total = localTotal;
        }

        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UiProgressGetProgress: syncState=%d, current=%llu, total=%llu", 
            static_cast<int>(pfSyncState), localCurrent, localTotal);
    }
    else
    {
        TRACE_INFORMATION("GameSaveAPIProviderGRTS::UiProgressGetProgress: hr=0x%08x", hr);
    }
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiProgressResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiProgressUserAction action) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiProgressResponse: action=%d", static_cast<int>(action));
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveSetProgressUiResponse(requestingUser, static_cast<PFXGameSaveProgressUiResponse>(action));
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiProgressResponse: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiSyncFailedResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiSyncFailedUserAction action) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiSyncFailedResponse: action=%d", static_cast<int>(action));
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveSetSyncFailedUiResponse(requestingUser, static_cast<PFXGameSaveSyncFailedUiResponse>(action));
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiSyncFailedResponse: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiActiveDeviceContentionResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiActiveDeviceContentionResponse: action=%d", static_cast<int>(action));
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveSetActiveDeviceContentionUiResponse(requestingUser, static_cast<PFXGameSaveActiveDeviceContentionUiResponse>(action));
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiActiveDeviceContentionResponse: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiConflictResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiConflictUserAction action) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiConflictResponse: action=%d", static_cast<int>(action));
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveSetConflictUiResponse(requestingUser, static_cast<PFXGameSaveConflictUiResponse>(action));
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiConflictResponse: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiOutOfStorageResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiOutOfStorageUserAction action) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiOutOfStorageResponse: action=%d", static_cast<int>(action));
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveSetOutOfStorageUiResponse(requestingUser, static_cast<PFXGameSaveOutOfStorageUiResponse>(action));
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::SetUiOutOfStorageResponse: hr=0x%08x", hr);
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetMockDeviceIdForDebug called with deviceId=%s", deviceId ? deviceId : "(null)");
    UNREFERENCED_PARAMETER(deviceId);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetMockManifestOffsetForDebug called with offset=%zu", offset);
    UNREFERENCED_PARAMETER(offset);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetMockDataFolderForDebug called with mockDataFolder=%s", mockDataFolder ? mockDataFolder : "(null)");
    UNREFERENCED_PARAMETER(mockDataFolder);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(jsonSize);
    if (jsonSize != nullptr)
    {
        *jsonSize = 0;
    }
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::GetStatsJsonForDebug(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t jsonSize,
    _Out_writes_(jsonSize) char* jsonBuffer,
    _Out_opt_ size_t* jsonSizeUsed
) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(jsonSize);
    UNREFERENCED_PARAMETER(jsonBuffer);
    UNREFERENCED_PARAMETER(jsonSizeUsed);
    if(jsonSizeUsed != nullptr)
    {
        *jsonSizeUsed = 0;
    }

    if (jsonSize > 0 && jsonBuffer != nullptr)
    {
        *jsonBuffer = '\0';
    }
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::GetSaveDescriptionSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* descriptionSize) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(descriptionSize);
    return E_NOTIMPL;
}

HRESULT GameSaveAPIProviderGRTS::GetSaveDescriptionForDebug(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t descriptionSize,
    _Out_writes_(descriptionSize) char* descriptionBuffer,
    _Out_opt_ size_t* descriptionSizeUsed
) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(descriptionSize);
    UNREFERENCED_PARAMETER(descriptionBuffer);
    UNREFERENCED_PARAMETER(descriptionSizeUsed);
    return E_NOTIMPL;
}

HRESULT GameSaveAPIProviderGRTS::SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetForceOutOfStorageErrorForDebug called with forceError=%d", forceError);
    UNREFERENCED_PARAMETER(forceError);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetForceSyncFailedErrorForDebug called with forceError=%d", forceError);
    UNREFERENCED_PARAMETER(forceError);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetForceNullPendingManifestForDebug(_In_ bool force) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetForceNullPendingManifestForDebug called with force=%d", force);
    UNREFERENCED_PARAMETER(force);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept
{
    TRACE_INFORMATION("[GAME SAVE] SetWriteManifestsToDiskForDebug called with writeManifests=%d", writeManifests);
    UNREFERENCED_PARAMETER(writeManifests);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::PauseUploadForDebug() noexcept
{
    TRACE_INFORMATION("[GAME SAVE] PauseUploadForDebug called");
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode)  noexcept
{
    UNREFERENCED_PARAMETER(mode);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::ResumeUploadForDebug() noexcept
{
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::ResetCloudAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ XAsyncBlock* async
) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::ResetCloudAsync: not implemented, returning E_NOTIMPL");
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(async);
    return E_NOTIMPL;
}


HRESULT GameSaveAPIProviderGRTS::ResetCloudResult(_Inout_ XAsyncBlock* async) noexcept
{
    return XAsyncGetStatus(async, false);
}

} // GameSave
} // PlayFab
// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "PFGameSaveFilesAPIProvider_GRTS.h"
#include <playfab/core/PFLocalUser_Xbox.h>
#include <playfab/core/PFEntity.h>
#include <atomic>
#include <stdint.h>
#include <PFXGameSave.h>
#include <XGameRuntimeInit.h>

namespace PlayFab
{
namespace GameSave
{

#ifndef HANDLE_XASYNC_FAILURE
#define HANDLE_XASYNC_FAILURE(hr) do { if (FAILED(hr)) { XAsyncComplete(asyncBlock, hr, 0); return; }} while (0, 0)
#endif

static const HRESULT E_GS_USER_CANCELED = 0x80830004;
constexpr auto PF_GDK_MAX_USERS = 16;

struct PFXPALGameSaveUserState
{
    PFLocalUserHandle localUser;
    XUserHandle xUser;
    void* configHandle;
    char saveFolder[1024];
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

PFXPALGameSaveUserState* PFXPALGetStateFromLocalUser(_In_ PFLocalUserHandle localUserHandle)
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

void ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(const PFXGameSaveDescriptor* descIn, PFGameSaveDescriptor* descOut)
{
    if (descIn == nullptr || descOut == nullptr)
    {
        return;
    }

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
}

HRESULT GameSaveAPIProviderGRTS::Initialize(_In_ PFGameSaveInitArgs* args) noexcept
{
    TRACE_INFORMATION("GameSaveAPIProviderGRTS::Initialize");
    if (args && args->saveFolder && args->saveFolder[0] != '\0')
    {
        this->m_initSaveFolder = String(args->saveFolder);
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
    }

    PFLocalUserHandle localUserHandle{};
    PFGameSaveFilesAddUserOptions addUserOptions{ PFGameSaveFilesAddUserOptions::None };
    XAsyncBlock getFolderAsyncBlock{};
    XAsyncBlock loginLocalUserAsyncBlock{};
    XTaskQueueHandle compositeQueue{ nullptr };
    XAsyncBlock* clientAsyncBlock{};

    bool useEntityAuth{ false };
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
    }

    PFXGameSaveConfigHandle configHandle{};
    hr = PFXGameSaveInitializeConfig(&configRequest, &configHandle);
    RETURN_IF_FAILED(hr);

    void* gsContextPtr = nullptr;
    hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);

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

    if (indexFound == PF_GDK_MAX_USERS)
    {
        if (firstEmptySlot == PF_GDK_MAX_USERS)
        {
            TRACE_WARNING("AddUserWithUiAsync GRTS: no free user slots (max %u). Failing initialization.", PF_GDK_MAX_USERS);
            return E_FAIL; // All slots full
        }
        indexFound = firstEmptySlot;
    }

    PFLocalUserHandle duplicatedHandle{};
    RETURN_IF_FAILED(PFLocalUserDuplicateHandle(context->localUserHandle, &duplicatedHandle));
    gsContext->users[indexFound].localUser = duplicatedHandle;
    gsContext->users[indexFound].xUser = context->xuser; // may be null if no XUser path
    gsContext->users[indexFound].configHandle = configHandle;

    if (context->clientAsyncBlock->queue != nullptr)
    {
        XTaskQueuePortHandle workPort{ nullptr };
        RETURN_IF_FAILED(XTaskQueueGetPort(context->clientAsyncBlock->queue, XTaskQueuePort::Work, &workPort));
        RETURN_IF_FAILED(XTaskQueueCreateComposite(workPort, workPort, &context->compositeQueue));
    }

    context->getFolderAsyncBlock.callback = PFXPALGetFolderComplete;
    context->getFolderAsyncBlock.queue = context->compositeQueue;
    context->getFolderAsyncBlock.context = context;
    hr = PFXGameSaveFilesGetFolderWithUiAsync(configHandle, &context->getFolderAsyncBlock);
    if (SUCCEEDED(hr))
    {
        context->getFolderAsyncStarted = true;
        // If cancel already requested before the inner async was started, propagate immediately
        if (context->cancelRequested)
        {
            XAsyncCancel(&context->getFolderAsyncBlock);
        }
    }
    RETURN_IF_FAILED(hr);

    return S_OK;
}

static void CALLBACK PFXPALEntityTokenComplete(XAsyncBlock* async)
{
    // Called after PFEntityGetEntityTokenAsync completes; continue config creation
    PFXPALGetFolderContext* context = static_cast<PFXPALGetFolderContext*>(async->context);
    HRESULT hr = XAsyncGetStatus(async, false);
    if (context->cancelRequested || hr == E_ABORT)
    {
        // Cancellation requested: complete outer async with user cancelled code.
        XAsyncComplete(context->clientAsyncBlock, E_ABORT, 0);
        return;
    }
    if (FAILED(hr))
    {
        // Fall back without entity auth
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
            }
        }
    }

    // entity handle already released after starting token async

    // Continue with config creation (ignore failure to obtain token: fallback)
    hr = PFXPALCallGetFolderWithUiAsync(context);
    if (FAILED(hr))
    {
        XAsyncComplete(context->clientAsyncBlock, hr, 0);
    }
}

static void CALLBACK PFXPALLocalUserLoginComplete(XAsyncBlock* async)
{
    PFXPALGetFolderContext* context = static_cast<PFXPALGetFolderContext*>(async->context);

    HRESULT hr = XAsyncGetStatus(async, false);
    if (context->cancelRequested || hr == E_ABORT)
    {
        // Cancellation requested: complete outer async with user cancelled code.
        XAsyncComplete(context->clientAsyncBlock, E_ABORT, 0);
        return;
    }
    if (FAILED(hr))
    {
        // Fall back without entity auth
        context->useEntityAuth = false;
    }
    else
    {
        PFEntityHandle localEntityHandle{};
        hr = PFLocalUserTryGetEntityHandle(context->localUserHandle, &localEntityHandle);
        if (SUCCEEDED(hr) && localEntityHandle)
        {
            // Entity key
            size_t keySize{};
            if (SUCCEEDED(PFEntityGetEntityKeySize(localEntityHandle, &keySize)) && keySize > 0 && keySize < 4096)
            {
                std::vector<uint8_t> keyBuffer(keySize);
                const PFEntityKey* key{};
                size_t keyUsed{};
                if (SUCCEEDED(PFEntityGetEntityKey(localEntityHandle, keyBuffer.size(), keyBuffer.data(), &key, &keyUsed)) && key && key->id && key->id[0] != '\0')
                {
                    StrCpy(context->entityId, sizeof(context->entityId), key->id);
                }
            }

            // Async token fetch
            context->entityTokenAsyncBlock.queue = context->clientAsyncBlock->queue;
            context->entityTokenAsyncBlock.context = context;
            context->entityTokenAsyncBlock.callback = PFXPALEntityTokenComplete;
            hr = PFEntityGetEntityTokenAsync(localEntityHandle, &context->entityTokenAsyncBlock);
            PFEntityCloseHandle(localEntityHandle); // Close local entity handle immediately pass or fail; async has its own ref internally.
            if (SUCCEEDED(hr))
            {
                context->entityTokenAsyncStarted = true; // mark started for cancellation tracking
                return; // continuation in callback
            }
            // If token async failed to start, fall through to proceed without entity auth
        }
    }

    // Continue with config creation (ignore failure to obtain token: fallback)
    hr = PFXPALCallGetFolderWithUiAsync(context);
    if (FAILED(hr))
    {
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
    RETURN_HR_INVALIDARG_IF_NULL(context);

    HRESULT hr;
    PFServiceConfigHandle serviceConfigHandle{};
    hr = PFLocalUserGetServiceConfigHandle(context->localUserHandle, &serviceConfigHandle);
    RETURN_IF_FAILED(hr);

    char apiEndpoint[1024];
    char playfabTitleId[256];
    hr = PFServiceConfigGetAPIEndpoint(serviceConfigHandle, sizeof(apiEndpoint), apiEndpoint, nullptr);
    if (SUCCEEDED(hr))
    {
        hr = PFServiceConfigGetTitleId(serviceConfigHandle, sizeof(playfabTitleId), playfabTitleId, nullptr);
    }
    PFServiceConfigCloseHandle(serviceConfigHandle);
    RETURN_IF_FAILED(hr);

    // Store common fields in context for later config construction
    StrCpy(context->apiEndpoint, sizeof(context->apiEndpoint), apiEndpoint);
    StrCpy(context->playfabTitleId, sizeof(context->playfabTitleId), playfabTitleId);

    // Preferred: XUser path
    XUserHandle xuser{};
    hr = PFLocalUserTryGetXUser(context->localUserHandle, &xuser);
    if (SUCCEEDED(hr))
    {

        context->xuser = xuser; // borrowed
        RETURN_IF_FAILED(PFXPALCallGetFolderWithUiAsync(context));
        return S_OK;
    }

    // Fallback: entity auth
    context->loginLocalUserAsyncBlock.queue = context->clientAsyncBlock->queue;
    context->loginLocalUserAsyncBlock.context = context;
    context->loginLocalUserAsyncBlock.callback = PFXPALLocalUserLoginComplete;
    hr = PFLocalUserLoginAsync(context->localUserHandle, true, &context->loginLocalUserAsyncBlock);
    if (SUCCEEDED(hr))
    {
        context->loginLocalUserAsyncStarted = true; // track started so we can cancel if needed
        return S_OK; // continuation in callback
    }
    // If token async failed to start, fall through to proceed without entity auth

    // Proceed without entity auth
    RETURN_IF_FAILED(PFXPALCallGetFolderWithUiAsync(context));
    return S_OK;
}

void CALLBACK PFXPALGetFolderComplete(XAsyncBlock* async)
{
    PFXPALGetFolderContext* context{ static_cast<PFXPALGetFolderContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, false);
    if (context->cancelRequested || hr == E_ABORT || hr == E_GS_USER_CANCELED)
    {
        XAsyncComplete(asyncBlock, E_ABORT, 0);
        return;
    }
    HANDLE_XASYNC_FAILURE(hr);

    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(context->localUserHandle);
    if (state == nullptr)
    {
        hr = E_PF_GAMESAVE_USER_NOT_ADDED;
        HANDLE_XASYNC_FAILURE(hr);
    }

    hr = PFXGameSaveFilesGetFolderWithUiResult(async, sizeof(state->saveFolder), state->saveFolder);
    if (hr == E_GS_USER_CANCELED)
    {
        XAsyncComplete(asyncBlock, E_ABORT, 0);
        return;
    }
    HANDLE_XASYNC_FAILURE(hr);

    XAsyncComplete(asyncBlock, hr, 0);
}

HRESULT GameSaveAPIProviderGRTS::AddUserWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesAddUserOptions options,
    _Inout_ XAsyncBlock* async
) noexcept
{
    UNREFERENCED_PARAMETER(options);
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);

    std::unique_ptr<PFXPALGetFolderContext> context = std::make_unique<PFXPALGetFolderContext>();
    if (!m_initSaveFolder.empty())
    {
        StrCpy(context->saveFolderOverride, sizeof(context->saveFolderOverride), m_initSaveFolder.c_str());
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
                    RETURN_IF_FAILED(PFXPALAddUserBegin(context));
                    return S_OK;
                }
                else if (op == XAsyncOp::Cancel)
                {
                    // Mark cancellation intent and propagate to any started inner async operations
                    context->cancelRequested = true;
                    if (context->loginLocalUserAsyncStarted)
                    {
                        XAsyncCancel(&context->loginLocalUserAsyncBlock);
                    }
                    if (context->entityTokenAsyncStarted)
                    {
                        XAsyncCancel(&context->entityTokenAsyncBlock);
                    }
                    if (context->getFolderAsyncStarted)
                    {
                        XAsyncCancel(&context->getFolderAsyncBlock);
                    }
                    // If nothing started yet, complete immediately
                    if (!context->loginLocalUserAsyncStarted && !context->entityTokenAsyncStarted && !context->getFolderAsyncStarted)
                    {
                        XAsyncComplete(data->async, E_ABORT, 0);
                    }
                }
                else if (op == XAsyncOp::Cleanup)
                {
                    std::unique_ptr<PFXPALGetFolderContext> contextPtr{ static_cast<PFXPALGetFolderContext*>(data->context) };
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
    }
    return hr;
}

HRESULT GameSaveAPIProviderGRTS::AddUserWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncGetStatus(async, false);
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
        PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
        if (state == nullptr)
        {
            return E_PF_GAMESAVE_USER_NOT_ADDED;
        }

        *saveRootFolderSize = strlen(state->saveFolder) + 1;
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
    RETURN_HR_INVALIDARG_IF_NULL(localUserHandle);
    RETURN_HR_INVALIDARG_IF_NULL(saveRootFolderBuffer);

    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
    if (state == nullptr)
    {
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    size_t folderSize = strlen(state->saveFolder) + 1;
    if (saveRootFolderSize < folderSize)
    {
        return E_INVALIDARG;
    }

    memcpy(saveRootFolderBuffer, state->saveFolder, folderSize);
    if (saveRootFolderUsed)
    {
        *saveRootFolderUsed = folderSize;
    }

    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::UploadWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUploadOption option,
    _Inout_ XAsyncBlock* async
) noexcept
{
    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
    if (state == nullptr)
    {
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    PFXGameSaveUploadOptions pfxoptions = PFXGameSaveUploadOptions::UploadKeepActive;
    if (option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
    {
        pfxoptions = PFXGameSaveUploadOptions::UploadReleaseActive;
    }
    PFXGameSaveConfigHandle configHandle = static_cast<PFXGameSaveConfigHandle>(state->configHandle);
    return PFXGameSaveFilesUploadWithUiAsync(configHandle, pfxoptions, async);
}

HRESULT GameSaveAPIProviderGRTS::UploadWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return PFXGameSaveFilesUploadWithUiResult(async);
}

HRESULT GameSaveAPIProviderGRTS::SetSaveDescriptionAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ const char* shortSaveDescription,
    _In_ XAsyncBlock* async) noexcept
{
    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
    if (state == nullptr)
    {
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    return PFXGameSaveFilesSetSaveDescriptionAsync(
        static_cast<PFXGameSaveConfigHandle>(state->configHandle),
        shortSaveDescription,
        async);
}

HRESULT GameSaveAPIProviderGRTS::SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept
{
    return PFXGameSaveFilesSetSaveDescriptionResult(async);
}

HRESULT GameSaveAPIProviderGRTS::GetRemainingQuota(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ int64_t* remainingQuota
) noexcept
{
    PFXPALGameSaveUserState* state = PFXPALGetStateFromLocalUser(localUserHandle);
    PFXGameSaveConfigHandle configHandle = (state != nullptr) ? (PFXGameSaveConfigHandle)state->configHandle : nullptr;
    return PFXGameSaveFilesGetRemainingQuota(configHandle, remainingQuota);
}

HRESULT GameSaveAPIProviderGRTS::IsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud
) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(isConnectedToCloud);
    *isConnectedToCloud = true;
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
        return;
    }
    auto cb = gsContext->activeDeviceChangedCallback.load(std::memory_order_acquire);
    if (!cb)
    {
        return; // no registered callback
    }
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, nullptr);
    if (localUser == nullptr)
    {
        return; // user not tracked
    }
    PFGameSaveDescriptor pfDesc{};
    ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(activeDevice, &pfDesc);
    
    // If a queue handle is supplied, schedule via TaskQueue; otherwise invoke directly.
    // (Passing a null XTaskQueueHandle to XTaskQueueSubmitDelayedCallback is not valid, so we guard.)
    XTaskQueueHandle queueHandle = gsContext->activeDeviceChangedCallbackQueue.load(std::memory_order_relaxed);
    void* userCtx = gsContext->activeDeviceChangedContext.load(std::memory_order_relaxed);

    if (queueHandle)
    {
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
    cb(localUser, &pfDesc, userCtx);
}

HRESULT GameSaveAPIProviderGRTS::SetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback, _In_opt_ void* context
) noexcept
{
    // Store callback in shared context so we can invoke when PFX layer notifies us.
    void* gsContextPtr = nullptr;
    HRESULT hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);
    // Order: store context & queue first, then publish callback with release so reader sees initialized fields.
    gsContext->activeDeviceChangedContext.store(context, std::memory_order_relaxed);
    gsContext->activeDeviceChangedCallbackQueue.store(callbackQueue, std::memory_order_relaxed);
    gsContext->activeDeviceChangedCallback.store(callback, std::memory_order_release);

    // Register global bridge with PFX layer
    return PFXGameSaveSetActiveDeviceChangedCallback(callback ? PFXPALActiveDeviceChangedCallback : nullptr, gsContext);
}

HRESULT GameSaveAPIProviderGRTS::UninitializeAsync(
    _Inout_ XAsyncBlock* async
) noexcept
{
    void* gsContextPtr = nullptr;
    HRESULT hr = PFPlatformGetGameSaveContext(&gsContextPtr);
    RETURN_IF_FAILED(hr);
    PFXPALGameSaveContext* gsContext = static_cast<PFXPALGameSaveContext*>(gsContextPtr);

    for (ULONG i = 0; i < PF_GDK_MAX_USERS; ++i)
    {
        if (gsContext->users[i].localUser != nullptr)
        {
            PFLocalUserCloseHandle(gsContext->users[i].localUser);
            gsContext->users[i].localUser = nullptr;
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
    }

    return GameSaveGlobalState::CleanupAsync(async);
}

HRESULT GameSaveAPIProviderGRTS::UninitializeResult(
    _Inout_ XAsyncBlock* async
) noexcept
{
    return XAsyncGetStatus(async, false);
}

void CALLBACK MyPFXPALGameSaveProgressUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveSyncState syncState,
    _In_opt_ void* context)
{
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(syncState);
        gsContext->progressCallback(localUser, pfSyncState, context);
    }
}

void CALLBACK MyPFXPALGameSaveSyncFailedUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveSyncState syncState,
    _In_ HRESULT error,
    _In_opt_ void* context)
{
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(syncState);
        gsContext->syncFailedCallback(localUser, pfSyncState, error, context);
    }
}

void CALLBACK MyPFXPALGameSaveActiveDeviceContentionUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ PFXGameSaveDescriptor* localGameSave, 
    _In_ PFXGameSaveDescriptor* remoteGameSave,
    _In_opt_ void* context)
{
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveDescriptor pfLocalGameSave = {};
        PFGameSaveDescriptor pfRemoteGameSave = {};
        ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(localGameSave, &pfLocalGameSave);
        ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(remoteGameSave, &pfRemoteGameSave);
        gsContext->activeDeviceContentionCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
    }
}

void CALLBACK MyPFXGameSaveActiveDeviceContentionUiCallback(
    _In_ XUserHandle requestingUser,
    _In_ time_t localTime,
    _In_ time_t remoteTime,
    _In_opt_ void* context)
{
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
    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        PFGameSaveDescriptor pfLocalGameSave = {};
        PFGameSaveDescriptor pfRemoteGameSave = {};
        ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(localGameSave, &pfLocalGameSave);
        ConvertPFXGameSaveDescriptorToPFGameSaveDescriptor(remoteGameSave, &pfRemoteGameSave);
        gsContext->conflictCallback(localUser, &pfLocalGameSave, &pfRemoteGameSave, context);
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
    UNREFERENCED_PARAMETER(isUserPartition); // titles don't need to know this bit of info

    PFXPALGameSaveContext* gsContext;
    PFLocalUserHandle localUser = PFXPALGetLocalUserFromXUser(requestingUser, &gsContext);
    if (localUser != nullptr)
    {
        gsContext->outOfStorageCallback(localUser, requiredBytes, context);
    }
}

HRESULT GameSaveAPIProviderGRTS::SetUiCallbacks(
    _In_ PFGameSaveUICallbacks* callbacks
) noexcept
{
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

    return PFXGameSaveSetUiCallbacks(&pfxcallbacks);
}

HRESULT GameSaveAPIProviderGRTS::UiProgressGetProgress(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_opt_ PFGameSaveFilesSyncState* syncState,
    _Out_opt_ uint64_t* current,
    _Out_opt_ uint64_t* total) noexcept
{
    PFXGameSaveSyncState pfxSyncState;
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    HRESULT hr = PFXGameSaveProgressUiGetProgress(requestingUser, &pfxSyncState, current, total);
    PFGameSaveFilesSyncState pfSyncState = static_cast<PFGameSaveFilesSyncState>(pfxSyncState);
    if (syncState != nullptr)
    {
        *syncState = pfSyncState;
    }

    return hr;
}

HRESULT GameSaveAPIProviderGRTS::SetUiProgressResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiProgressUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    return PFXGameSaveSetProgressUiResponse(requestingUser, static_cast<PFXGameSaveProgressUiResponse>(action));
}

HRESULT GameSaveAPIProviderGRTS::SetUiSyncFailedResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiSyncFailedUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    return PFXGameSaveSetSyncFailedUiResponse(requestingUser, static_cast<PFXGameSaveSyncFailedUiResponse>(action));
}

HRESULT GameSaveAPIProviderGRTS::SetUiActiveDeviceContentionResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    return PFXGameSaveSetActiveDeviceContentionUiResponse(requestingUser, static_cast<PFXGameSaveActiveDeviceContentionUiResponse>(action));
}

HRESULT GameSaveAPIProviderGRTS::SetUiConflictResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiConflictUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    return PFXGameSaveSetConflictUiResponse(requestingUser, static_cast<PFXGameSaveConflictUiResponse>(action));
}

HRESULT GameSaveAPIProviderGRTS::SetUiOutOfStorageResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiOutOfStorageUserAction action) noexcept
{
    XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
    return PFXGameSaveSetOutOfStorageUiResponse(requestingUser, static_cast<PFXGameSaveOutOfStorageUiResponse>(action));
}

HRESULT GameSaveAPIProviderGRTS::SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept
{
    UNREFERENCED_PARAMETER(deviceId);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept
{
    UNREFERENCED_PARAMETER(offset);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept
{
    UNREFERENCED_PARAMETER(mockDataFolder);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept
{
    UNREFERENCED_PARAMETER(localUserHandle);
    UNREFERENCED_PARAMETER(jsonSize);
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
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept
{
    UNREFERENCED_PARAMETER(forceError);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept
{
    UNREFERENCED_PARAMETER(forceError);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept
{
    UNREFERENCED_PARAMETER(writeManifests);
    return S_OK;
}

HRESULT GameSaveAPIProviderGRTS::PauseUploadForDebug() noexcept
{
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
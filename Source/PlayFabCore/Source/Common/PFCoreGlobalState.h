#pragma once

#include <playfab/core/PFLocalUser.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFEventPipeline.h>
#include <playfab/core/PFHttpConfig.h>
#include "ServiceConfig.h"
#include "EventPipeline/EventPipeline.h"
#include "HandleTable.h"
#include "TokenExpiredHandler.h"
#include "TokenRefreshedHandler.h"
#if HC_PLATFORM == HC_PLATFORM_ANDROID
#include "Platform/Android/PlatformComponents_Android.h"
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK // Temporary while GDK is still a separate platform
#include <PFXGameSave.h>
#include <playfab/gamesave/PFGameSaveFiles.h>

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
    PFXPALGameSaveUserState users[PF_GDK_MAX_USERS];
};
#endif

namespace PlayFab
{

using ServiceConfigHandleTable = HandleTable<PFServiceConfigHandle, ServiceConfig>;
using LocalUserHandleTable = HandleTable<PFLocalUserHandle, class LocalUser>;
using EntityHandleTable = HandleTable<PFEntityHandle, Entity>;
using EventPipelineHandleTable = HandleTable<PFEventPipelineHandle, EventPipeline>;

class SteamAPIFunctions;

namespace Core
{
class LocalUserCache; // Forward declaration
}

// PFCoreGlobalState singleton
class PFCoreGlobalState : public ITerminationListener
{
public:
    ~PFCoreGlobalState() noexcept;

    static HRESULT Create(XTaskQueueHandle backgroundQueue, HCInitArgs* args) noexcept;
    static HRESULT Get(SharedPtr<PFCoreGlobalState>& state) noexcept;
    static HRESULT CleanupAsync(XAsyncBlock* async) noexcept;

    RunContext RunContext() const noexcept;
    ServiceConfigHandleTable& ServiceConfigs() noexcept;
    LocalUserHandleTable& LocalUsers() noexcept;
    EntityHandleTable& Entities() noexcept;
    EventPipelineHandleTable& ClientEventPipelines() noexcept;
    TokenExpiredHandler TokenExpiredHandler() const noexcept;
    TokenRefreshedHandler TokenRefreshedHandler() const noexcept;
    SharedPtr<PFHttpRetrySettings> HttpRetrySettings() const noexcept;
    SharedPtr<PFHttpSettings> HttpSettings() const noexcept;
    PFPlatformType const GetPlatformType() const noexcept;
    bool IsGRTSAvailable() const noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK
    PFXPALGameSaveContext* GameSaveContext() noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
    SharedPtr<PlatformComponents_Android> PlatformComponents() const noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
    SharedPtr<SteamAPIFunctions> m_steamAPI; // used to hold struct SteamAPIFunctions
#endif

private:
    PFCoreGlobalState(XTaskQueueHandle backgroundQueue) noexcept;

    void OnTerminated(void* context) noexcept override;
    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data);

    PFPlatformType m_platformType;
    bool m_isGRTSAvailable{};
    PlayFab::RunContext m_runContext;
    ServiceConfigHandleTable m_serviceConfigs;
    LocalUserHandleTable m_localUsers;
    EntityHandleTable m_entities;
    EventPipelineHandleTable m_clientEventPipelines;
    PlayFab::TokenExpiredHandler m_tokenExpiredHandler;
    PlayFab::TokenRefreshedHandler m_tokenRefreshedHandler;
    SharedPtr<PFHttpRetrySettings> m_httpRetrySettings;
    SharedPtr<PFHttpSettings> m_httpSettings;

    // Local User Cache
    UnorderedMap<String, WeakPtr<LocalUser>> m_localUserCache; // LocalId -> WeakPtr<LocalUser>
#if HC_PLATFORM == HC_PLATFORM_GDK
    PFXPALGameSaveContext m_gamesave{};
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
    SharedPtr<PlatformComponents_Android> m_platformComponents;
#endif

    friend struct PFCoreGlobalStateBootstrapper;
    friend class Core::LocalUserCache;
};

}

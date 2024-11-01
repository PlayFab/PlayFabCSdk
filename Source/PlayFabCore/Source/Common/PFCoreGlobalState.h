#pragma once

#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFEventPipeline.h>
#include <playfab/core/PFHttpConfig.h>
#include "ServiceConfig.h"
#include "EventPipeline/EventPipeline.h"
#include "HandleTable.h"
#include "TokenExpiredHandler.h"
#include "TokenRefreshedHandler.h"

namespace PlayFab
{

using ServiceConfigHandleTable = HandleTable<PFServiceConfigHandle, ServiceConfig>;
using EntityHandleTable = HandleTable<PFEntityHandle, Entity>;
using EventPipelineHandleTable = HandleTable<PFEventPipelineHandle, EventPipeline>;

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
    EntityHandleTable& Entities() noexcept;
    EventPipelineHandleTable& ClientEventPipelines() noexcept;
    TokenExpiredHandler TokenExpiredHandler() const noexcept;
    TokenRefreshedHandler TokenRefreshedHandler() const noexcept;
    SharedPtr<PFHttpRetrySettings> HttpRetrySettings() const noexcept;
    SharedPtr<PFHttpSettings> HttpSettings() const noexcept;

private:
    PFCoreGlobalState(XTaskQueueHandle backgroundQueue) noexcept;

    void OnTerminated(void* context) noexcept override;
    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data);

    PlayFab::RunContext m_runContext;
    ServiceConfigHandleTable m_serviceConfigs;
    EntityHandleTable m_entities;
    EventPipelineHandleTable m_clientEventPipelines;
    PlayFab::TokenExpiredHandler m_tokenExpiredHandler;
    PlayFab::TokenRefreshedHandler m_tokenRefreshedHandler;
    SharedPtr<PFHttpRetrySettings> m_httpRetrySettings;
    SharedPtr<PFHttpSettings> m_httpSettings;

    friend struct PFCoreGlobalStateBootstrapper;
};

}

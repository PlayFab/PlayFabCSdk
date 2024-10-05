#pragma once

#include "Common/Entity.h"
#include "EventPipelineTypes.h"
#include <playfab/core/cpp/EventPipeline.h>

namespace PlayFab
{

class EventPipeline
{
public:
    EventPipeline(
        RunContext rc,
        SharedPtr<Entity> uploadingEntity,
        PFEventPipelineType eventPipelineType,
        uint32_t maxEventsPerBatch,
        uint32_t maxWaitTimeInSeconds,
        uint32_t pollDelayInMs,
        HCCompressionLevel compressionLevel,
        bool retryOnDisconnect,
        size_t bufferSize,
        PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
        PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
        void* handlerContext
    ) noexcept;

    EventPipeline(
        RunContext rc,
        SharedPtr<ServiceConfig> serviceConfig,
        const char* telemetryKey,
        PFEventPipelineType eventPipelineType,
        uint32_t maxEventsPerBatch,
        uint32_t maxWaitTimeInSeconds,
        uint32_t pollDelayInMs,
        HCCompressionLevel compressionLevel,
        bool retryOnDisconnect,
        size_t bufferSize,
        PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
        PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
        void* handlerContext
    ) noexcept;

    EventPipeline(EventPipeline const&) = delete;
    EventPipeline& operator=(EventPipeline) = delete;
    virtual ~EventPipeline();

    HRESULT EmitEvent(Event&& event) noexcept;

    HRESULT AddUploadingEntity(SharedPtr<Entity> uploadingEntity) noexcept;

    HRESULT RemoveUploadingEntity() noexcept;

    HRESULT UpdateConfiguration(PFEventPipelineConfig eventPipelineConfig) noexcept;

private:
    void Initialize(RunContext rc, PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler, PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler, void* handlerContext);

    EventPipelineEventHandlers m_eventHandlers;
    std::optional<PFRegistrationToken> m_batchUploadedHandlerToken;
    std::optional<PFRegistrationToken> m_batchFailedHandlerToken;

    SharedPtr<class EventBuffer> m_buffer;
    SharedPtr<class EventUploader> m_uploader;
};

}

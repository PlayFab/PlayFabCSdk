#include "stdafx.h"
#include "playfab/core/PFEventPipeline.h"
#include "EventPipeline/EventPipeline.h"
#include "ApiHelpers.h"

using namespace PlayFab;

HRESULT EventPipelineCreationHelper(
    PFEventPipelineType eventPipelineType,
    PFEventPipelineConfig* eventPipelineConfig,
    PFEntityHandle entityHandle,
    PFEventPipelineTelemetryKeyConfig* eventPipelineTelemetryKeyConfig,
    XTaskQueueHandle queue,
    PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
    PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
    void* handlerContext,
    GlobalState& state,
    PFEventPipelineHandle* eventPipelineHandle
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(eventPipelineHandle);

    // Telemetry Key is not valid for PlayStream Event Pipeline
    if (eventPipelineType == PFEventPipelineType::PlayStream)
    {
        RETURN_HR_IF(E_INVALIDARG, !entityHandle);
    }
    else
    {
        RETURN_HR_IF(E_INVALIDARG, !entityHandle && !eventPipelineTelemetryKeyConfig);
        RETURN_HR_IF(E_INVALIDARG, eventPipelineTelemetryKeyConfig && (!eventPipelineTelemetryKeyConfig->telemetryKey || !eventPipelineTelemetryKeyConfig->serviceConfigHandle));
    }

    SharedPtr<Entity> entity;
    SharedPtr<ServiceConfig> serviceConfig;

    if (entityHandle)
    {
        RETURN_IF_FAILED(state.Entities().FromHandle(entityHandle, entity));
    }
    else
    {
        RETURN_IF_FAILED(state.ServiceConfigs().FromHandle(eventPipelineTelemetryKeyConfig->serviceConfigHandle, serviceConfig));
    }

    uint32_t maxEvents, maxWaitTime, pollDelay;
    PFHCCompressionLevel compressionLevel;

    if (eventPipelineType == PFEventPipelineType::PlayStream)
    {
        maxEvents = !eventPipelineConfig || !eventPipelineConfig->maxEventsPerBatch ? PFPlayStreamEventPipelineMaxEventsPerBatchDefault : *eventPipelineConfig->maxEventsPerBatch;
        maxWaitTime = !eventPipelineConfig || !eventPipelineConfig->maxWaitTimeInSeconds ? PFPlayStreamEventPipelineMaxWaitTimeInSecondsDefault : *eventPipelineConfig->maxWaitTimeInSeconds;
        pollDelay = !eventPipelineConfig || !eventPipelineConfig->pollDelayInMs ? PFPlayStreamEventPipelinePollDelayInMsDefault : *eventPipelineConfig->pollDelayInMs;
    }
    else
    {
        maxEvents = !eventPipelineConfig || !eventPipelineConfig->maxEventsPerBatch ? PFTelemetryEventPipelineMaxEventsPerBatchDefault : *eventPipelineConfig->maxEventsPerBatch;
        maxWaitTime = !eventPipelineConfig || !eventPipelineConfig->maxWaitTimeInSeconds ? PFTelemetryEventPipelineMaxWaitTimeInSecondsDefault : *eventPipelineConfig->maxWaitTimeInSeconds;
        pollDelay = !eventPipelineConfig || !eventPipelineConfig->pollDelayInMs ? PFTelemetryEventPipelinePollDelayInMsDefault : *eventPipelineConfig->pollDelayInMs;
    }

    compressionLevel = !eventPipelineConfig || !eventPipelineConfig->compressionLevel ? PFHCCompressionLevel::None : *eventPipelineConfig->compressionLevel;

    SharedPtr<EventPipeline> pipeline;

    if (entityHandle)
    {
        pipeline = MakeShared<EventPipeline>(
            state.RunContext().DeriveOnQueue(queue),
            entity,
            eventPipelineType,
            maxEvents,
            maxWaitTime,
            pollDelay,
            compressionLevel,
            batchUploadedEventHandler,
            batchFailedEventHandler,
            handlerContext
        );
    }
    else
    {
        pipeline = MakeShared<EventPipeline>(
            state.RunContext().DeriveOnQueue(queue),
            serviceConfig,
            eventPipelineTelemetryKeyConfig->telemetryKey,
            eventPipelineType,
            maxEvents,
            maxWaitTime,
            pollDelay,
            compressionLevel,
            batchUploadedEventHandler,
            batchFailedEventHandler,
            handlerContext
        );
    }

    return state.ClientEventPipelines().MakeHandle(std::move(pipeline), *eventPipelineHandle);
}

PF_API PFEventPipelineCreateTelemetryPipelineHandleWithKey(
    _In_ PFEventPipelineTelemetryKeyConfig* eventPipelineTelemetryKeyConfig,
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ PFEventPipelineBatchUploadSucceededEventHandler* eventPipelineBatchUploadedEventHandler,
    _In_opt_ PFEventPipelineBatchUploadFailedEventHandler* eventPipelineBatchFailedEventHandler,
    _In_opt_ void* handlerContext,
    _Out_ PFEventPipelineHandle* eventPipelineHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineCreateTelemetryPipelineHandleWithKey), [&](GlobalState& state)
    {
        return EventPipelineCreationHelper(
            PFEventPipelineType::Telemetry,
            nullptr,
            nullptr,
            eventPipelineTelemetryKeyConfig,
            queue,
            eventPipelineBatchUploadedEventHandler,
            eventPipelineBatchFailedEventHandler,
            handlerContext,
            state,
            eventPipelineHandle
        );
    });   
}

PF_API PFEventPipelineCreateTelemetryPipelineHandleWithEntity(
    _In_ PFEntityHandle entityHandle,
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ PFEventPipelineBatchUploadSucceededEventHandler* eventPipelineBatchUploadedEventHandler,
    _In_opt_ PFEventPipelineBatchUploadFailedEventHandler* eventPipelineBatchFailedEventHandler,
    _In_opt_ void* handlerContext,
    _Out_ PFEventPipelineHandle* eventPipelineHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineCreateTelemetryPipelineHandleWithEntity), [&](GlobalState& state)
    {
        return EventPipelineCreationHelper(
            PFEventPipelineType::Telemetry,
            nullptr,
            entityHandle,
            nullptr,
            queue,
            eventPipelineBatchUploadedEventHandler,
            eventPipelineBatchFailedEventHandler,
            handlerContext,
            state,
            eventPipelineHandle
        );
    });
}

PF_API PFEventPipelineCreatePlayStreamPipelineHandle(
    _In_ PFEntityHandle entityHandle,
    _In_opt_ XTaskQueueHandle queue,
    _In_opt_ PFEventPipelineBatchUploadSucceededEventHandler* eventPipelineBatchUploadedEventHandler,
    _In_opt_ PFEventPipelineBatchUploadFailedEventHandler* eventPipelineBatchFailedEventHandler,
    _In_opt_ void* handlerContext,
    _Out_ PFEventPipelineHandle* eventPipelineHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineCreatePlayStreamPipelineHandle), [&](GlobalState& state)
    {
        return EventPipelineCreationHelper(
            PFEventPipelineType::PlayStream,
            nullptr,
            entityHandle,
            nullptr,
            queue,
            eventPipelineBatchUploadedEventHandler,
            eventPipelineBatchFailedEventHandler,
            handlerContext,
            state,
            eventPipelineHandle
        );
    });
}

PF_API PFEventPipelineDuplicateHandle(
    _In_ PFEventPipelineHandle eventPipelineHandle,
    _In_ PFEventPipelineHandle* duplicatedEventPipelineHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineDuplicateHandle), [&](GlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(duplicatedEventPipelineHandle);

        SharedPtr<EventPipeline> pipeline;
        RETURN_IF_FAILED(state.ClientEventPipelines().FromHandle(eventPipelineHandle, pipeline));
        return state.ClientEventPipelines().MakeHandle(std::move(pipeline), *duplicatedEventPipelineHandle);
    });
}

PF_API_(void) PFEventPipelineCloseHandle(
    _In_ PFEventPipelineHandle eventPipelineHandle
) noexcept
{
    ApiImpl(XASYNC_IDENTITY(PFEventPipelineCloseHandle), [&](GlobalState& state)
    {
        state.ClientEventPipelines().CloseHandle(eventPipelineHandle);
        return S_OK;
    });
}

PF_API PFEventPipelineEmitEvent(
    _In_ PFEventPipelineHandle eventPipelineHandle,
    _In_ PFEvent const* event
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineEmitEvent), [&](GlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(event);

        SharedPtr<EventPipeline> pipeline;
        RETURN_IF_FAILED(state.ClientEventPipelines().FromHandle(eventPipelineHandle, pipeline));

        return pipeline->EmitEvent(*event);
    });
}

PF_API PFEventPipelineAddUploadingEntity(
    _In_ PFEventPipelineHandle eventPipelineHandle,
    _In_ PFEntityHandle entityHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineAddUploadingEntity), [&](GlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(entityHandle);

        SharedPtr<EventPipeline> pipeline;
        RETURN_IF_FAILED(state.ClientEventPipelines().FromHandle(eventPipelineHandle, pipeline));

        SharedPtr<Entity> entity;
        RETURN_IF_FAILED(state.Entities().FromHandle(entityHandle, entity));

        return pipeline->AddUploadingEntity(entity);
    });
}

PF_API PFEventPipelineRemoveUploadingEntity(
    _In_ PFEventPipelineHandle eventPipelineHandle
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineRemoveUploadingEntity), [&](GlobalState& state)
        {
            SharedPtr<EventPipeline> pipeline;
            RETURN_IF_FAILED(state.ClientEventPipelines().FromHandle(eventPipelineHandle, pipeline));

            return pipeline->RemoveUploadingEntity();
        });
}

PF_API PFEventPipelineUpdateConfiguration(
    _In_ PFEventPipelineHandle eventPipelineHandle,
    _In_ PFEventPipelineConfig eventPipelineConfig
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFEventPipelineUpdateConfiguration), [&](GlobalState& state)
        {
            SharedPtr<EventPipeline> pipeline;
            RETURN_IF_FAILED(state.ClientEventPipelines().FromHandle(eventPipelineHandle, pipeline));

            return pipeline->UpdateConfiguration(eventPipelineConfig);
        });
}
#include <playfab/core/PFEventPipeline.h>
#include "PlayFabException.h"
#include <algorithm>

namespace PlayFab
{

enum class PFEventPipelineType : uint32_t
{
    PlayStream,
    Telemetry
};

namespace Wrappers
{

class EventPipeline
{
public:
    // Creates a EventPipeline by wrapping a PFEventPipelineHandle (transfers ownership of the handle)
    static EventPipeline Wrap(PFEventPipelineHandle handle)
    {
        return EventPipeline{ handle };
    }

    // Creates a EventPipeline by duplicating a PFEventPipelineHandle (caller still owns provided handle)
    static EventPipeline Duplicate(PFEventPipelineHandle handle)
    {
        PFEventPipelineHandle duplicatedHandle;
        THROW_IF_FAILED(PFEventPipelineDuplicateHandle(handle, &duplicatedHandle));
        return EventPipeline{ duplicatedHandle };
    }

    EventPipeline() { };

    EventPipeline(
        PFEventPipelineTelemetryKeyConfig* eventPipelineTelemetryKeyConfig,
        XTaskQueueHandle queue
    )
    {
        THROW_IF_FAILED(PFEventPipelineCreateTelemetryPipelineHandleWithKey(eventPipelineTelemetryKeyConfig, queue, nullptr, nullptr, nullptr, &m_handle));
    }

    EventPipeline(
        PFEventPipelineType pipelineType,
        PFEntityHandle entityHandle,
        XTaskQueueHandle queue
    )
    {
        if (pipelineType == PFEventPipelineType::PlayStream)
        {
            THROW_IF_FAILED(PFEventPipelineCreatePlayStreamPipelineHandle(entityHandle, queue, nullptr, nullptr, nullptr, &m_handle));
        }
        else
        {
            THROW_IF_FAILED(PFEventPipelineCreateTelemetryPipelineHandleWithEntity(entityHandle, queue, nullptr, nullptr, nullptr, &m_handle));
        }
    }

    EventPipeline(
        PFEventPipelineTelemetryKeyConfig* eventPipelineTelemetryKeyConfig,
        XTaskQueueHandle queue,
        PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
        PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
        void* handlerContext
    )
    {
        THROW_IF_FAILED(PFEventPipelineCreateTelemetryPipelineHandleWithKey(eventPipelineTelemetryKeyConfig, queue, batchUploadedEventHandler, batchFailedEventHandler, handlerContext, &m_handle));
    }

    EventPipeline(
        PFEventPipelineType pipelineType,
        PFEntityHandle entityHandle,
        XTaskQueueHandle queue,
        PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
        PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
        void* handlerContext
    )
    {
        if (pipelineType == PFEventPipelineType::PlayStream)
        {
            THROW_IF_FAILED(PFEventPipelineCreatePlayStreamPipelineHandle(entityHandle, queue, batchUploadedEventHandler, batchFailedEventHandler, handlerContext, &m_handle));
        }
        else
        {
            THROW_IF_FAILED(PFEventPipelineCreateTelemetryPipelineHandleWithEntity(entityHandle, queue, batchUploadedEventHandler, batchFailedEventHandler, handlerContext, &m_handle));
        }
    }

    EventPipeline(EventPipeline const& other)
    {
        THROW_IF_FAILED(PFEventPipelineDuplicateHandle(other.m_handle, &m_handle));
    }

    EventPipeline(EventPipeline&& other)
    {
        std::swap(m_handle, other.m_handle);
    }

    EventPipeline& operator=(EventPipeline other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    virtual ~EventPipeline()
    {
        if (m_handle)
        {
            PFEventPipelineCloseHandle(m_handle);
        }
    }

    PFEventPipelineHandle Handle() const noexcept
    {
        return m_handle;
    }

    HRESULT EmitEvent(PFEvent const* event) const noexcept
    {
        return PFEventPipelineEmitEvent(m_handle, event);
    }

    HRESULT AddUploadingEntity(PFEntityHandle entityHandle) const noexcept
    {
        return PFEventPipelineAddUploadingEntity(m_handle, entityHandle);
    }

    HRESULT RemoveUploadingEntity() const noexcept
    {
        return PFEventPipelineRemoveUploadingEntity(m_handle);
    }

    HRESULT UpdateConfiguration(PFEventPipelineConfig eventPipelineConfig) const noexcept
    {
        return PFEventPipelineUpdateConfiguration(m_handle, eventPipelineConfig);
    }

private:
    EventPipeline(PFEventPipelineHandle handle) : m_handle{ handle }
    {
    }

    PFEventPipelineHandle m_handle{ nullptr };
};

}
}

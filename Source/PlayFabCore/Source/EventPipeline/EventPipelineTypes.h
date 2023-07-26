#pragma once

#include <playfab/core/PFEventPipeline.h>

namespace PlayFab
{

class Event : public Wrappers::ModelWrapper<PFEvent, Allocator>
{
public:
    using ModelType = PFEvent;

    Event() = default;
    Event(PFEvent const& src);
    Event(Event const& src);
    Event(Event&& src);
    Event& operator=(Event src);
    ~Event() = default;

    friend void swap(Event& lhs, Event& rhs);

    time_t EmitTime() const;
    void SetClientId(String&& clientId);

private:
    void SetModelPointers();

    std::optional<EntityKey> m_entity;
    String m_eventNamespace;
    String m_name;
    String m_clientId;
    String m_payloadJson;
    time_t m_emitTime;
};

class UploadedEvent : public Wrappers::ModelWrapper<PFUploadedEvent, Allocator>
{
public:
    using ModelType = PFUploadedEvent;

    UploadedEvent(String&& clientId, String&& serviceId);
    UploadedEvent(UploadedEvent const& src);
    UploadedEvent(UploadedEvent&& src);
    UploadedEvent& operator=(UploadedEvent src);
    ~UploadedEvent() = default;

    friend void swap(UploadedEvent& lhs, UploadedEvent& rhs);

private:
    void SetModelPointers();

    String m_clientId;
    String m_serviceId;
};

class UploadedBatch : public Wrappers::ModelVector<UploadedEvent, Allocator>
{
public:
    UploadedBatch(Vector<Event> const& events, Vector<String>&& serviceIds);
};

class FailedBatch : public Wrappers::ModelVector<Event, Allocator>
{
public:
    FailedBatch(Vector<Event>&& events);
};

// Class the manages client pipeline event handlers. This pattern is a little tricky because the client doesn't directly control lifetime
// of the EventUploader that will be invoking the handlers. This class allows the EventPipeline (which the client does control the lifetime of)
// to unregister the client's handlers during EventPipeline destruction, ensuring they will never be invoked after pipeline is destroyed.
// This means that if there are still events awaiting upload when the client cleans up the pipeline, they will still be uploaded in the background
// but the client will not receive callbacks upon completion.
class EventPipelineEventHandlers
{
public:
    EventPipelineEventHandlers() noexcept;
    EventPipelineEventHandlers(EventPipelineEventHandlers const&) = default;
    EventPipelineEventHandlers(EventPipelineEventHandlers&&) = default;
    EventPipelineEventHandlers& operator=(EventPipelineEventHandlers) = delete;
    ~EventPipelineEventHandlers() = default;

    HRESULT RegisterBatchUploadSucceededHandler(
        RunContext&& rc,
        void* handlerContext,
        PFEventPipelineBatchUploadSucceededEventHandler* succeededHandler,
        PFRegistrationToken* token
    ) noexcept;

    HRESULT RegisterBatchUploadFailedHandler(
        RunContext&& rc,
        void* handlerContext,
        PFEventPipelineBatchUploadFailedEventHandler* failedHandler,
        PFRegistrationToken* token
    ) noexcept;

    void UnregisterHandler(PFRegistrationToken token) noexcept;

    void InvokeBatchSucceeded(UploadedBatch&& batch);
    void InvokeBatchFailed(HRESULT translatedError, String&& errorMessage, FailedBatch&& batch);

private:
    class SharedState;

    SharedPtr<SharedState> m_state;
};

}

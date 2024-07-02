#include "stdafx.h"
#include "EventPipelineTypes.h"

namespace PlayFab
{

Event::Event(PFEvent const& src) :
    Wrappers::ModelWrapper<PFEvent, Allocator>{ src },
    m_entity{ src.entity ? std::optional<EntityKey>{*src.entity} : std::nullopt },
    m_eventNamespace{ SafeString(src.eventNamespace) },
    m_name{ SafeString(src.name) },
    m_clientId{ SafeString(src.clientId) },
    m_payloadJson{ SafeString(src.payloadJson) },
    m_emitTime{ time(nullptr) }
{
    SetModelPointers();
}

Event::Event(Event const& src) : Event{ src.Model() }
{
}

Event::Event(Event&& src) noexcept : Event{}
{
    swap(*this, src);
}

Event& Event::operator=(Event src)
{
    swap(*this, src);
    return *this;
}

void swap(Event& lhs, Event& rhs)
{
    using std::swap;
    swap(lhs.m_model, rhs.m_model);
    swap(lhs.m_entity, rhs.m_entity);
    swap(lhs.m_eventNamespace, rhs.m_eventNamespace);
    swap(lhs.m_name, rhs.m_name);
    swap(lhs.m_clientId, rhs.m_clientId);
    swap(lhs.m_payloadJson, rhs.m_payloadJson);
    swap(lhs.m_emitTime, rhs.m_emitTime);
    rhs.SetModelPointers();
    lhs.SetModelPointers();
}

time_t Event::EmitTime() const
{
    return m_emitTime;
}

void Event::SetClientId(String&& clientId)
{
    m_clientId = std::move(clientId);
    m_model.clientId = m_clientId.empty() ? nullptr : m_clientId.data();
}

void Event::SetModelPointers()
{
    this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    this->m_model.eventNamespace = m_eventNamespace.empty() ? nullptr : m_eventNamespace.data();
    this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    this->m_model.clientId = m_clientId.empty() ? nullptr : m_clientId.data();
    this->m_model.payloadJson = m_payloadJson.empty() ? nullptr : m_payloadJson.data();
}

UploadedEvent::UploadedEvent(String&& clientId, String&& serviceId)
    : m_clientId{ std::move(clientId) },
    m_serviceId{ std::move(serviceId) }
{
    SetModelPointers();
}

UploadedEvent::UploadedEvent(UploadedEvent const& src) : UploadedEvent{ src.Model().clientId, src.Model().serviceId }
{
}

UploadedEvent::UploadedEvent(UploadedEvent&& src) noexcept
{
    swap(*this, src);
}

UploadedEvent& UploadedEvent::operator=(UploadedEvent src)
{
    swap(*this, src);
    return *this;
}

void swap(UploadedEvent& lhs, UploadedEvent& rhs)
{
    using std::swap;
    swap(lhs.m_model, rhs.m_model);
    swap(lhs.m_clientId, rhs.m_clientId);
    swap(lhs.m_serviceId, rhs.m_serviceId);
    rhs.SetModelPointers();
    lhs.SetModelPointers();
}

void UploadedEvent::SetModelPointers()
{
    this->m_model.clientId = m_clientId.empty() ? nullptr : m_clientId.data();
    this->m_model.serviceId = m_serviceId.empty() ? nullptr : m_serviceId.data();
}

UploadedBatch::UploadedBatch(Vector<Event> const& events, Vector<String>&& serviceIds)
{
    assert(events.size() == serviceIds.size());

    reserve(events.size());
    for (size_t i = 0; i < events.size(); ++i)
    {
        push_back(UploadedEvent{ events[i].Model().clientId, std::move(serviceIds[i])});
    }
}

FailedBatch::FailedBatch(Vector<Event>&& events)
{
    reserve(events.size());
    std::for_each(events.begin(), events.end(), [this](Event& e) { push_back(e); });
}

// EventPipelineEventHandlers and its internal SharedState
class BatchUploadSucceededHandler
{
public:
    BatchUploadSucceededHandler(RunContext&& rc, void* handlerContext, PFEventPipelineBatchUploadSucceededEventHandler* clientHandler) noexcept;

    RunContext RunContext() const noexcept;
    void Invoke(UploadedBatch const& batch) const noexcept;

private:
    PlayFab::RunContext m_rc;
    void* const m_context;
    PFEventPipelineBatchUploadSucceededEventHandler* const m_handler;
};

class BatchUploadFailedHandler
{
public:
    BatchUploadFailedHandler(RunContext&& rc, void* handlerContext, PFEventPipelineBatchUploadFailedEventHandler* clientHandler) noexcept;

    RunContext RunContext() const noexcept;
    void Invoke(HRESULT translatedError, String const& errorMessage, FailedBatch const& batch) const noexcept;

private:
    PlayFab::RunContext m_rc;
    void* const m_context;
    PFEventPipelineBatchUploadFailedEventHandler* const m_handler;
};

class EventPipelineEventHandlers::SharedState : public std::enable_shared_from_this<EventPipelineEventHandlers::SharedState>
{
public:
    SharedState() = default;
    SharedState(SharedState const&) = delete;
    SharedState(SharedState&&) = delete;
    SharedState& operator=(SharedState const&) = delete;
    SharedState& operator=(SharedState&&) = delete;
    ~SharedState() noexcept = default;

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
    mutable std::recursive_mutex m_mutex; // recursive to allow unregistering within callbacks

    Map<PFRegistrationToken, SharedPtr<BatchUploadSucceededHandler>> m_batchUploadedSucceededHandlers;
    Map<PFRegistrationToken, SharedPtr<BatchUploadFailedHandler>> m_batchUploadFailedHandlers;

    static PFRegistrationToken const s_firstToken{ 0xBAC1100000000000 };
    PFRegistrationToken m_nextToken{ s_firstToken };
};

BatchUploadSucceededHandler::BatchUploadSucceededHandler(PlayFab::RunContext&& rc, void* handlerContext, PFEventPipelineBatchUploadSucceededEventHandler* clientHandler) noexcept :
    m_rc{ std::move(rc) },
    m_context{ handlerContext },
    m_handler{ clientHandler }
{
}

RunContext BatchUploadSucceededHandler::RunContext() const noexcept
{
    return m_rc;
}

void BatchUploadSucceededHandler::Invoke(UploadedBatch const& batch) const noexcept
{
    try
    {
        m_handler(m_context, batch.data(), batch.size());
    }
    catch (...)
    {
        TRACE_WARNING("Caught unhandled exception in client PFEventPipelineBatchUploadSucceededEventHandler");
    }
}

BatchUploadFailedHandler::BatchUploadFailedHandler(PlayFab::RunContext&& rc, void* handlerContext, PFEventPipelineBatchUploadFailedEventHandler* clientHandler) noexcept :
    m_rc{ std::move(rc) },
    m_context{ handlerContext },
    m_handler{ clientHandler }
{
}

RunContext BatchUploadFailedHandler::RunContext() const noexcept
{
    return m_rc;
}

void BatchUploadFailedHandler::Invoke(HRESULT translatedError, String const& errorMessage, FailedBatch const& batch) const noexcept
{
    try
    {
        m_handler(m_context, translatedError, errorMessage.data(), batch.data(), batch.size());
    }
    catch (...)
    {
        TRACE_WARNING("Caught unhandled exception in client PFEventPipelineBatchUploadSucceededEventHandler");
    }
}

HRESULT EventPipelineEventHandlers::SharedState::RegisterBatchUploadSucceededHandler(
    RunContext&& rc,
    void* handlerContext,
    PFEventPipelineBatchUploadSucceededEventHandler* succeededHandler,
    PFRegistrationToken* token
) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    RETURN_HR_INVALIDARG_IF_NULL(succeededHandler);
    RETURN_HR_INVALIDARG_IF_NULL(token);

    *token = m_nextToken++;
    m_batchUploadedSucceededHandlers.emplace(*token, MakeShared<BatchUploadSucceededHandler>(std::move(rc), handlerContext, succeededHandler));

    return S_OK;
}

HRESULT EventPipelineEventHandlers::SharedState::RegisterBatchUploadFailedHandler(
    RunContext&& rc,
    void* handlerContext,
    PFEventPipelineBatchUploadFailedEventHandler* failedHandler,
    PFRegistrationToken* token
) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    RETURN_HR_INVALIDARG_IF_NULL(failedHandler);
    RETURN_HR_INVALIDARG_IF_NULL(token);

    *token = m_nextToken++;
    m_batchUploadFailedHandlers.emplace(*token, MakeShared<BatchUploadFailedHandler>(std::move(rc), handlerContext, failedHandler));

    return S_OK;
}

void EventPipelineEventHandlers::SharedState::UnregisterHandler(PFRegistrationToken token) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    TRACE_VERBOSE(__FUNCTION__);

    auto successHandlersIter = m_batchUploadedSucceededHandlers.find(token);
    if (successHandlersIter != m_batchUploadedSucceededHandlers.end())
    {
        successHandlersIter->second->RunContext().CancellationToken().Cancel();
        m_batchUploadedSucceededHandlers.erase(successHandlersIter);
        return;
    }

    auto failedHandlersIter = m_batchUploadFailedHandlers.find(token);
    if (failedHandlersIter != m_batchUploadFailedHandlers.end())
    {
        failedHandlersIter->second->RunContext().CancellationToken().Cancel();
        m_batchUploadFailedHandlers.erase(failedHandlersIter);
        return;
    }

    TRACE_WARNING("%s: Attempted to unregister handler with unrecognized token", __FUNCTION__);
}

void EventPipelineEventHandlers::SharedState::InvokeBatchSucceeded(UploadedBatch&& batch)
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };
    auto sharedThis{ shared_from_this() };

    for (auto& pair : m_batchUploadedSucceededHandlers)
    {
        // EventPipelineEventHandlers::SharedState handles submitting the work to the TaskQueue so that it can synchronize the event handler invocation
        // and unregistration of the handler

        auto handler = pair.second;
        handler->RunContext().TaskQueueSubmitWork([sharedThis, handler, batch]()
        {
            std::unique_lock<std::recursive_mutex> lock{ sharedThis->m_mutex }; // lock to avoid races with unregistering the handler
            if (!handler->RunContext().CancellationToken().IsCancelled())
            {
                handler->Invoke(batch);
            }
        });
    }
}

void EventPipelineEventHandlers::SharedState::InvokeBatchFailed(HRESULT translatedError, String&& errorMessage, FailedBatch&& batch)
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };
    auto sharedThis{ shared_from_this() };

    for (auto& pair : m_batchUploadFailedHandlers)
    {
        // EventPipelineEventHandlers::SharedState handles submitting the work to the TaskQueue so that it can synchronize the event handler invocation
        // and unregistration of the handler

        auto handler = pair.second;
        handler->RunContext().TaskQueueSubmitWork([sharedThis, handler, translatedError, errorMessage, batch]()
        {
            std::unique_lock<std::recursive_mutex> lock{ sharedThis->m_mutex }; // lock to avoid races with unregistering the handler
            if (!handler->RunContext().CancellationToken().IsCancelled())
            {
                handler->Invoke(translatedError, errorMessage, batch);
            }
        });
    }
}

EventPipelineEventHandlers::EventPipelineEventHandlers() noexcept :
    m_state{ MakeShared<SharedState>() }
{
}

HRESULT EventPipelineEventHandlers::RegisterBatchUploadSucceededHandler(
    RunContext&& rc,
    void* handlerContext,
    PFEventPipelineBatchUploadSucceededEventHandler* succeededHandler,
    PFRegistrationToken* token
) noexcept
{
    return m_state->RegisterBatchUploadSucceededHandler(std::move(rc), handlerContext, succeededHandler, token);
}

HRESULT EventPipelineEventHandlers::RegisterBatchUploadFailedHandler(
    RunContext&& rc,
    void* handlerContext,
    PFEventPipelineBatchUploadFailedEventHandler* failedHandler,
    PFRegistrationToken* token
) noexcept
{
    return m_state->RegisterBatchUploadFailedHandler(std::move(rc), handlerContext, failedHandler, token);
}

void EventPipelineEventHandlers::UnregisterHandler(PFRegistrationToken token) noexcept
{
    return m_state->UnregisterHandler(token);
}

void EventPipelineEventHandlers::InvokeBatchSucceeded(UploadedBatch&& batch)
{
    m_state->InvokeBatchSucceeded(std::move(batch));
}

void EventPipelineEventHandlers::InvokeBatchFailed(HRESULT translatedError, String&& errorMessage, FailedBatch&& batch)
{
    m_state->InvokeBatchFailed(translatedError, std::move(errorMessage), std::move(batch));
}

}
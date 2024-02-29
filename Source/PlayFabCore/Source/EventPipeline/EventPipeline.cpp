#include "stdafx.h"
#include "EventPipeline.h"
#include "Generated/Events.h"
#include "JsonUtils.h"

namespace PlayFab
{

class WriteEventsResponse : protected ServiceResponse
{
public:
    WriteEventsResponse(ServiceResponse&& serviceResponse);

    HRESULT TranslatedHResult() { return m_translatedHResult; }

    PlayFab::Events::WriteEventsResponse const& ResultModel() { return m_resultModel; };

    Vector<int> FailedEventIndexes() { return m_failedEventIndexes; }

    ServiceErrorCode ServiceErrorCode() { return ServiceResponse::ErrorCode; }

    String ErrorMessage() { return ServiceResponse::ErrorMessage; }

    int HttpCode() { return ServiceResponse::HttpCode ; }

private:
    HRESULT m_translatedHResult;
    PlayFab::Events::WriteEventsResponse m_resultModel;
    Vector<int> m_failedEventIndexes;
};

WriteEventsResponse::WriteEventsResponse(ServiceResponse&& serviceResponse) :
    ServiceResponse{ serviceResponse }
{
    int httpCode = ServiceResponse::HttpCode;

    if (httpCode >= 200 && httpCode < 300)
    {
        m_resultModel.FromJson(serviceResponse.Data);
    }
    else
    {
        m_translatedHResult = ServiceErrorToHR(ServiceResponse::ErrorCode);

        if (httpCode >= 400 && httpCode < 500)
        {
            if (ServiceResponse::ErrorCode == ServiceErrorCode::InvalidParams)
            {
                PlayFab::Vector<int> failedEventIndexes{};

                for (rapidjson::Value::ConstMemberIterator iter = ServiceResponse::ErrorDetails.MemberBegin(); iter != ServiceResponse::ErrorDetails.MemberEnd(); ++iter)
                {
                    char* errorDetail = const_cast<char*>(iter->name.GetString());
                    const char* prefix = "Events[";

                    if (strncmp(prefix, errorDetail, strlen(prefix)) == 0)
                    {
                        char* nextToken;
                        char* eventIndex = StrTok(errorDetail, "]", &nextToken);
                        eventIndex = StrTok(errorDetail, "[", &nextToken);
                        eventIndex = StrTok(NULL, " ", &nextToken);

                        int index = std::atoi(eventIndex);
                        failedEventIndexes.push_back(index);
                    }
                }

                m_failedEventIndexes = failedEventIndexes;
            }
        }
    }
};

// Simple, thread-safe EventBuffer based on std::queue.
// Potential for optimization here with either LHC's LocklessQueue or XPlatCpp CELL-adapted EventBuffer
class EventBuffer
{
public:
    EventBuffer(size_t bufferSize);
    EventBuffer(EventBuffer const&) = delete;
    EventBuffer& operator=(EventBuffer const&) = delete;
    ~EventBuffer() = default;

    HRESULT PushBack(Event&& event) noexcept;
    HRESULT PushBack(Vector<Event>&& events) noexcept;
    HRESULT PushFront(Event&& event) noexcept;
    HRESULT PushFront(Vector<Event>&& events) noexcept;
    bool PopFront(Event& eventContents) noexcept;
    void SetConfiguration(PFEventPipelineConfig eventPipelineConfig);

private:
    std::mutex m_mutex;
    std::mutex m_configMutex;
    Deque<Event> m_deque;
    size_t m_bufferSize;
    
};

class EventUploader : public ITaskQueueWork, public std::enable_shared_from_this<EventUploader>
{
public:
    EventUploader(
        RunContext rc,
        SharedPtr<Entity> uploadingEntity,
        PFEventPipelineType eventPipelineType,
        SharedPtr<EventBuffer> buffer,
        uint32_t maxEventsPerBatch,
        uint32_t maxWaitTimeInSeconds,
        uint32_t pollDelayInMs,
        PFHCCompressionLevel compressionLevel,
        bool retryOnDisconnect,
        EventPipelineEventHandlers eventHandlers
    );

    EventUploader(
        RunContext rc,
        SharedPtr<ServiceConfig> serviceConfig,
        const char* telemetryKey,
        PFEventPipelineType eventPipelineType,
        SharedPtr<EventBuffer> buffer,
        uint32_t maxEventsPerBatch,
        uint32_t maxWaitTimeInSeconds,
        uint32_t pollDelayInMs,
        PFHCCompressionLevel compressionLevel,
        bool retryOnDisconnect,
        EventPipelineEventHandlers eventHandlers
    );

    void Start();
    void Stop();
    void SetUploadingEntity(SharedPtr<Entity> entity);
    void SetConfiguration(PFEventPipelineConfig eventPipelineConfig); 
    void ExponentialBackoff(uint32_t retryCount);
    PFEventPipelineType EventPipelineType() { return m_eventPipelineType; }
    PlayFab::String TelemetryKey() { return m_telemetryKey; }
    SharedPtr<ServiceConfig> ServiceConfig() { return m_serviceConfig; }

private:
    // ITaskQueueWork
    void Run() noexcept override;

    AsyncOp<WriteEventsResponse> WriteEvents(const Vector<Event>& events, RunContext rc);
    Wrappers::ModelVector<Wrappers::PFEventsEventContentsWrapper<Allocator>, Allocator> ConvertEvents(Vector<Event> payload);
    void ProcessResponse(Result<WriteEventsResponse> result, Vector<Event> payload, EventPipelineEventHandlers eventHandlers, bool isRetry);
    void ExecutePendingRetries();
    JsonValue BuildRequestBody(Vector<Event> payload);

    RunContext m_rc;
    SharedPtr<Entity> m_entity;
    SharedPtr<PlayFab::ServiceConfig> m_serviceConfig;
    PlayFab::String m_telemetryKey;
    PFEventPipelineType m_eventPipelineType;
    SharedPtr<EventBuffer> m_buffer;
    uint32_t m_maxEventsPerBatch;
    uint32_t m_maxWaitTimeInSeconds;
    uint32_t m_pollDelayInMs;
    PFHCCompressionLevel m_compressionLevel;
    bool m_retryOnDisconnect;
    uint32_t m_retryCount = 0;
    EventPipelineEventHandlers const m_eventHandlers;
    Vector<Event> m_pendingPayload;
    SharedPtr<Queue<Vector<Event>>> m_retryPayloads;
    time_t m_oldestEventTimeStamp;
    bool m_telemetryKeyInvalid{ false };
    bool m_telemetryKeyDeactivated{ false };
    std::mutex m_mutex;
    std::mutex m_configMutex;
};

void EventPipeline::Initialize(
    RunContext rc,
    PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler, 
    PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
    void* handlerContext
)
{
    if (batchUploadedEventHandler)
    {
        m_batchUploadedHandlerToken.emplace();
        m_eventHandlers.RegisterBatchUploadSucceededHandler(rc.Derive(), handlerContext, batchUploadedEventHandler, &m_batchUploadedHandlerToken.value());
    }
    if (batchFailedEventHandler)
    {
        m_batchFailedHandlerToken.emplace();
        m_eventHandlers.RegisterBatchUploadFailedHandler(rc.Derive(), handlerContext, batchFailedEventHandler, &m_batchFailedHandlerToken.value());
    }

    m_uploader->Start();
}

EventPipeline::EventPipeline(
    RunContext rc,
    SharedPtr<Entity> uploadingEntity,
    PFEventPipelineType eventPipelineType,
    uint32_t maxEventsPerBatch,
    uint32_t maxWaitTimeInSeconds,
    uint32_t pollDelayInMs,
    PFHCCompressionLevel compressionLevel,
    bool retryOnDisconnect,
    size_t bufferSize,
    PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
    PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
    void* handlerContext
) noexcept :
    m_buffer{ MakeShared<EventBuffer>(bufferSize) },
    m_uploader{ MakeShared<EventUploader>(rc.Derive(), uploadingEntity, eventPipelineType, m_buffer, maxEventsPerBatch, maxWaitTimeInSeconds, pollDelayInMs, compressionLevel, retryOnDisconnect, m_eventHandlers)}
{
    Initialize(rc, batchUploadedEventHandler, batchFailedEventHandler, handlerContext);
}

EventPipeline::EventPipeline(
    RunContext rc,
    SharedPtr<ServiceConfig> serviceConfig,
    const char* telemetryKey,
    PFEventPipelineType eventPipelineType,
    uint32_t maxEventsPerBatch,
    uint32_t maxWaitTimeInSeconds,
    uint32_t pollDelayInMs,
    PFHCCompressionLevel compressionLevel,
    bool retryOnDisconnect,
    size_t bufferSize,
    PFEventPipelineBatchUploadSucceededEventHandler* batchUploadedEventHandler,
    PFEventPipelineBatchUploadFailedEventHandler* batchFailedEventHandler,
    void* handlerContext
) noexcept :
    m_buffer{ MakeShared<EventBuffer>(bufferSize) },
    m_uploader{ MakeShared<EventUploader>(rc.Derive(), serviceConfig, telemetryKey, eventPipelineType, m_buffer, maxEventsPerBatch, maxWaitTimeInSeconds, pollDelayInMs, compressionLevel, retryOnDisconnect, m_eventHandlers) }
{
    Initialize(rc, batchUploadedEventHandler, batchFailedEventHandler, handlerContext);
}

EventPipeline::~EventPipeline()
{
    if (m_batchUploadedHandlerToken.has_value())
    {
        m_eventHandlers.UnregisterHandler(*m_batchUploadedHandlerToken);
    }
    if (m_batchFailedHandlerToken.has_value())
    {
        m_eventHandlers.UnregisterHandler(*m_batchFailedHandlerToken);
    }

    m_uploader->Stop();
}

// Should we do client side payload validation?
HRESULT EventPipeline::EmitEvent(Event&& event) noexcept
{
    return m_buffer->PushBack(std::move(event));
}

HRESULT EventPipeline::AddUploadingEntity(SharedPtr<Entity> uploadingEntity) noexcept
{
    m_uploader->SetUploadingEntity(std::move(uploadingEntity));

    return S_OK;
}

HRESULT EventPipeline::RemoveUploadingEntity() noexcept
{
    // Telemetry Key is valid only for Telemetry Event Pipeline, does not make sense to remove entity for PlayStream Pipeline.
    RETURN_HR_IF(E_FAIL, m_uploader->EventPipelineType() == PFEventPipelineType::PlayStream);

    // If there is no valid fallback auth mechanism (Telemetry Key) return error to the client.
    RETURN_HR_IF(E_FAIL, !m_uploader->ServiceConfig() || m_uploader->TelemetryKey().empty() );

    m_uploader->SetUploadingEntity(nullptr);

    return S_OK;
}

HRESULT EventPipeline::UpdateConfiguration(PFEventPipelineConfig eventPipelineConfig) noexcept
{
    m_uploader->SetConfiguration(eventPipelineConfig);
    m_buffer->SetConfiguration(eventPipelineConfig);

    return S_OK;
}

EventBuffer::EventBuffer(size_t bufferSize) : m_bufferSize{bufferSize} {}

HRESULT EventBuffer::PushBack(Event&& event) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().eventNamespace);
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().name);
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().payloadJson);
    RETURN_HR_IF(E_PF_CORE_EVENT_PIPELINE_BUFFER_FULL, m_deque.size() >= m_bufferSize);

    // If the client didn't provide an eventId, use a default based on a counter. ID will not be unique accross multiple sessions and/or clients,
    // but the service will assign a unique ID upon upload.
    static std::atomic<uint64_t> s_eventId{ 0 };

    if (!event.Model().clientId || strlen(event.Model().clientId) == 0)
    {
        Stringstream eventIdStream;
        eventIdStream << "PlayFabCoreSDK_Event" << ++s_eventId;
        event.SetClientId(eventIdStream.str());
    }

    std::lock_guard<std::mutex> lock{ m_mutex };
    m_deque.push_back(std::move(event));

    return S_OK;
}

HRESULT EventBuffer::PushBack(Vector<Event>&& events) noexcept
{
    for (size_t i = 0; i < events.size(); i++)
    {
        PushBack(std::move(events[i]));
    }

    return S_OK;
}

HRESULT EventBuffer::PushFront(Event&& event) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().eventNamespace);
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().name);
    RETURN_HR_INVALIDARG_IF_NULL(event.Model().payloadJson);
    // We will not drop events retried after network failure even if buffer is over limit size

    // If the client didn't provide an eventId, use a default based on a counter. ID will not be unique accross multiple sessions and/or clients,
    // but the service will assign a unique ID upon upload.
    static std::atomic<uint64_t> s_eventId{ 0 };

    if (!event.Model().clientId || strlen(event.Model().clientId) == 0)
    {
        Stringstream eventIdStream;
        eventIdStream << "PlayFabCoreSDK_Event" << ++s_eventId;
        event.SetClientId(eventIdStream.str());
    }

    std::lock_guard<std::mutex> lock{ m_mutex };
    m_deque.push_front(std::move(event));

    return S_OK;
}

HRESULT EventBuffer::PushFront(Vector<Event>&& events) noexcept
{
    for (size_t i = 0; i < events.size(); i++)
    {
        PushFront(std::move(events[i]));
    }

    return S_OK;
}

bool EventBuffer::PopFront(Event& event) noexcept
{
    std::lock_guard<std::mutex> lock{ m_mutex };
    if (m_deque.empty())
    {
        return false;
    }

    std::swap(event, m_deque.front());
    m_deque.pop_front();
    return true;
}

EventUploader::EventUploader(
    RunContext rc,
    SharedPtr<Entity> uploadingEntity,
    PFEventPipelineType eventPipelineType,
    SharedPtr<EventBuffer> buffer,
    uint32_t maxEventsPerBatch,
    uint32_t maxWaitTimeInSeconds,
    uint32_t pollDelayInMs,
    PFHCCompressionLevel compressionLevel,
    bool retryOnDisconnect,
    EventPipelineEventHandlers eventHandlers
) :
    m_rc{ std::move(rc) },
    m_entity{ std::move(uploadingEntity) },
    m_eventPipelineType{ std::move(eventPipelineType) },
    m_buffer{ std::move(buffer) },
    m_maxEventsPerBatch{ maxEventsPerBatch },
    m_maxWaitTimeInSeconds{ maxWaitTimeInSeconds },
    m_pollDelayInMs{ pollDelayInMs },
    m_compressionLevel{ compressionLevel },
    m_retryOnDisconnect{ retryOnDisconnect },
    m_eventHandlers{ std::move(eventHandlers) },
    m_retryPayloads{ MakeShared<Queue<Vector<Event>>>() } 
{
}

EventUploader::EventUploader(
    RunContext rc,
    SharedPtr<PlayFab::ServiceConfig> serviceConfig,
    const char* telemetryKey,
    PFEventPipelineType eventPipelineType,
    SharedPtr<EventBuffer> buffer,
    uint32_t maxEventsPerBatch,
    uint32_t maxWaitTimeInSeconds,
    uint32_t pollDelayInMs,
    PFHCCompressionLevel compressionLevel,
    bool retryOnDisconnect,
    EventPipelineEventHandlers eventHandlers
) :
    m_rc{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_telemetryKey{ telemetryKey },
    m_eventPipelineType{ std::move(eventPipelineType) },
    m_buffer{ std::move(buffer) },
    m_maxEventsPerBatch{ maxEventsPerBatch },
    m_maxWaitTimeInSeconds{ maxWaitTimeInSeconds },
    m_pollDelayInMs{ pollDelayInMs },
    m_compressionLevel{ compressionLevel },
    m_retryOnDisconnect{ retryOnDisconnect },
    m_eventHandlers{ std::move(eventHandlers) },
    m_retryPayloads{ MakeShared<Queue<Vector<Event>>>() }
{
}

void EventUploader::Start()
{
    m_rc.TaskQueueSubmitWork(shared_from_this(), 0);
}

void EventUploader::Stop()
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    // Cancel the uploader rather than terminating the queue. This allowed the next scheduled poll run as expected so that the remaining events
    // get uploaded.
    m_rc.CancellationToken().Cancel();
}

void EventUploader::SetUploadingEntity(SharedPtr<Entity> entity)
{
    std::unique_lock<std::mutex> lock{ m_configMutex };

    m_entity = entity;
}

void EventBuffer::SetConfiguration(PFEventPipelineConfig eventPipelineConfig)
{
    std::unique_lock<std::mutex> lock{ m_configMutex };

    m_bufferSize = !eventPipelineConfig.bufferSize ? PFEventPipelineBufferSizeDefault : *eventPipelineConfig.bufferSize;
}

void EventUploader::SetConfiguration(PFEventPipelineConfig eventPipelineConfig)
{
    std::unique_lock<std::mutex> lock{ m_configMutex };

    if (m_eventPipelineType == PFEventPipelineType::PlayStream)
    {
        m_maxEventsPerBatch = !eventPipelineConfig.maxEventsPerBatch ? PFPlayStreamEventPipelineMaxEventsPerBatchDefault : *eventPipelineConfig.maxEventsPerBatch;
        m_maxWaitTimeInSeconds = !eventPipelineConfig.maxWaitTimeInSeconds ? PFPlayStreamEventPipelineMaxWaitTimeInSecondsDefault : *eventPipelineConfig.maxWaitTimeInSeconds;
        m_pollDelayInMs = !eventPipelineConfig.pollDelayInMs ? PFPlayStreamEventPipelinePollDelayInMsDefault : *eventPipelineConfig.pollDelayInMs;
    }
    else
    {
        m_maxEventsPerBatch = !eventPipelineConfig.maxEventsPerBatch ? PFTelemetryEventPipelineMaxEventsPerBatchDefault : *eventPipelineConfig.maxEventsPerBatch;
        m_maxWaitTimeInSeconds = !eventPipelineConfig.maxWaitTimeInSeconds ? PFTelemetryEventPipelineMaxWaitTimeInSecondsDefault : *eventPipelineConfig.maxWaitTimeInSeconds;
        m_pollDelayInMs = !eventPipelineConfig.pollDelayInMs ? PFTelemetryEventPipelinePollDelayInMsDefault : *eventPipelineConfig.pollDelayInMs;
        m_retryOnDisconnect = !eventPipelineConfig.retryOnDisconnect ? PFTelemetryEventPipelineRetryOnDisconnectDefault : *eventPipelineConfig.retryOnDisconnect;
    }

    m_compressionLevel = !eventPipelineConfig.compressionLevel ? PFHCCompressionLevel::None : *eventPipelineConfig.compressionLevel;
}

JsonValue EventUploader::BuildRequestBody(Vector<Event> events)
{
    Events::WriteEventsRequest request;
    Wrappers::ModelVector<Wrappers::PFEventsEventContentsWrapper<Allocator>, Allocator> eventContents = ConvertEvents(events);

    request.SetEvents(eventContents);
    JsonValue requestBody{ request.ToJson() };

    return requestBody;
}

AsyncOp<WriteEventsResponse> EventUploader::WriteEvents(
    const Vector<Event>& events,
    RunContext rc
)
{
    std::unique_lock<std::mutex> lock{ m_configMutex };
    JsonValue requestBody = BuildRequestBody(events);

    const char* path{ m_eventPipelineType == PFEventPipelineType::PlayStream ? "/Event/WriteEvents" : "/Event/WriteTelemetryEvents" };
    CacheId retryCacheId = m_eventPipelineType == PFEventPipelineType::PlayStream ? CacheId::EventsWriteEvents : CacheId::EventsWriteTelemetryEvents;

    if (m_entity)
    {
        auto requestOp = m_entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
            m_entity,
            path,
            requestBody,
            retryCacheId,
            rc.Derive(),
            m_compressionLevel
        );

        // Release lock after making entity request to avoid crashes if entity is removed.
        lock.unlock();

        return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
            {
                RETURN_IF_FAILED(result.hr);
                auto serviceResponse = result.ExtractPayload();
                return WriteEventsResponse{ std::move(serviceResponse) };
            }
        );
    }
    else if (!m_telemetryKey.empty() && (!m_telemetryKeyInvalid && !m_telemetryKeyDeactivated))
    {
        auto requestOp = m_serviceConfig->HttpClient()->MakeTelemetryKeyRequest(
            m_telemetryKey,
            path,
            requestBody,
            retryCacheId,
            rc.Derive(),
            m_compressionLevel
        );

        return requestOp.Then([](Result<ServiceResponse> result) -> Result<WriteEventsResponse>
            {   
                RETURN_IF_FAILED(result.hr);
                auto serviceResponse = result.ExtractPayload();
                return WriteEventsResponse{ std::move(serviceResponse) };
            }
        );
    }
    else
    {
        // Simulating getting an error response back from the service and return events to failed event handler.
        ServiceResponse response;
        response.HttpCode = 403;
        response.ErrorCode = m_telemetryKeyDeactivated ? ServiceErrorCode::TelemetryKeyDeactivated : ServiceErrorCode::TelemetryKeyInvalid;

        return AsyncOp<WriteEventsResponse>{ WriteEventsResponse{ std::move(response) }};
    }
}

void EventUploader::ProcessResponse(Result<WriteEventsResponse> result, Vector<Event> payload, EventPipelineEventHandlers eventHandlers, bool isRetry)
{
    if (FAILED(result.hr) && m_retryOnDisconnect)
    {
        // We assume network failure and if retries are enabled, we count the retries to delay the normal periodic schedule and add the events back to the buffer
        m_retryCount++;
        m_buffer->PushFront(std::move(payload));
    }
    else
    {
        WriteEventsResponse response = result.ExtractPayload();
        m_retryCount = 0; // reset network failure delay

        if (response.HttpCode() < 200 || response.HttpCode() >= 300)
        {
            if (response.HttpCode() >= 400 && response.HttpCode() < 500)  // Client-side error
            {
                if (response.ServiceErrorCode() == ServiceErrorCode::TelemetryKeyInvalid || response.ServiceErrorCode() == ServiceErrorCode::TelemetryKeyDeactivated)
                {
                    if (response.ServiceErrorCode() == ServiceErrorCode::TelemetryKeyInvalid)
                    {
                        if (!m_telemetryKeyInvalid)
                        {
                            m_telemetryKeyInvalid = true;
                            TRACE_ERROR("EventPipeline EventUploader failed, Telemetry Key is invalid (hr=0x%08x)", result.hr);
                        }
                    }
                    else
                    {
                        if (!m_telemetryKeyDeactivated)
                        {
                            m_telemetryKeyDeactivated = true;
                            TRACE_ERROR("EventPipeline EventUploader failed, Telemetry Key is deactivated (hr=0x%08x)", result.hr);
                        }
                    }
                }
                else if (response.ServiceErrorCode() == ServiceErrorCode::InvalidParams)  // If Invalid Params, we can identify failed events, so strip out failed events from payload and do a retry with the remaining
                {
                    if (!isRetry)
                    {
                        Vector<int> failedEventIndexes = response.FailedEventIndexes();
                        Vector<Event> failedEvents, retryEvents;

                        for (size_t i = 0; i < payload.size(); i++)
                        {
                            if (std::find(failedEventIndexes.begin(), failedEventIndexes.end(), static_cast<int>(i)) != failedEventIndexes.end())
                            {
                                failedEvents.push_back(std::move(payload[i]));
                            }
                            else
                            {
                                retryEvents.push_back(std::move(payload[i]));
                            }
                        }

                        if (!retryEvents.empty() && retryEvents.size() < payload.size())
                        {
                            m_retryPayloads->push(std::move(retryEvents));
                        }

                        payload = std::move(failedEvents);
                    }
                }

                const char* eventsApi = m_eventPipelineType == PFEventPipelineType::PlayStream ? "WriteEvents" : "WriteTelemetryEvents";
                TRACE_ERROR("EventPipeline EventUploader %s failed, invoking event handler. (hr=0x%08x)", eventsApi, result.hr);

                eventHandlers.InvokeBatchFailed(response.TranslatedHResult(), response.ErrorMessage(), FailedBatch{ std::move(payload) });
            }
            else    // Server-side error
            {
                if (isRetry)    // If this is a retry, return the whole batch back to the failure handler
                {
                    const char* eventsApi = m_eventPipelineType == PFEventPipelineType::PlayStream ? "WriteEvents" : "WriteTelemetryEvents";
                    TRACE_ERROR("EventPipeline EventUploader %s failed, invoking event handler. (hr=0x%08x)", eventsApi, result.hr);

                    eventHandlers.InvokeBatchFailed(response.TranslatedHResult(), response.ErrorMessage(), FailedBatch{ std::move(payload) });
                }
                else            // Insert into list of retry payloads
                {
                    m_retryPayloads->push(std::move(payload));
                }
            }
        }
        else if (response.ResultModel().Model().assignedEventIdsCount != payload.size())
        {
            // The service only responds with service assignedEventIds and the mapping of clientIds to serviceIds is just based on the order of the ids in the request
            // and response. We don't have a way to handle a case where the lists are different sizes, so we'll consider the entire batch failed.

            constexpr char s_mismatchedAssignedEventIdsError[]{ "EventPipeline EventUploader WriteEventsResponse contained mismatched number of AssignedEventIds." };

            TRACE_ERROR("%s, invoking event handler", s_mismatchedAssignedEventIdsError);
            eventHandlers.InvokeBatchFailed(E_FAIL, s_mismatchedAssignedEventIdsError, FailedBatch{ std::move(payload) });
        }
        else
        {
            // WriteEvents Succeeded
            auto& model = response.ResultModel().Model();
            Vector<String> assignedEventIds{ model.assignedEventIds, model.assignedEventIds + model.assignedEventIdsCount };
            eventHandlers.InvokeBatchSucceeded(UploadedBatch{ std::move(payload), std::move(assignedEventIds) });
        }
    }
}

void EventUploader::ExecutePendingRetries()
{
    if (!m_retryPayloads->empty())  // Are there any payloads pending for retry? If so, proceed to do the retry
    {
        for (size_t i = 0; i < m_retryPayloads->size(); i++)
        {
            Vector<Event> payload = m_retryPayloads->front();

            m_retryPayloads->pop();

            WriteEvents(std::move(payload), m_rc.Derive()).Finally([payload, sharedThis = shared_from_this(), eventHandlers = m_eventHandlers ](Result<WriteEventsResponse> result) mutable
                {
                    sharedThis->ProcessResponse(std::move(result), std::move(payload), eventHandlers, true);
                }
            );
        }
    }
}

// Translate Event to Events::EventContents
Wrappers::ModelVector<Wrappers::PFEventsEventContentsWrapper<Allocator>, Allocator> EventUploader::ConvertEvents(Vector<Event> payload)
{
    Wrappers::ModelVector<Wrappers::PFEventsEventContentsWrapper<Allocator>, Allocator> events;

    for (auto& payloadEvent : payload)
    {
        Wrappers::PFEventsEventContentsWrapper<Allocator> eventModel;
        eventModel.SetEntity(payloadEvent.Model().entity ? std::optional<EntityKey> { *payloadEvent.Model().entity } : std::nullopt);
        eventModel.SetEventNamespace(payloadEvent.Model().eventNamespace);
        eventModel.SetName(payloadEvent.Model().name);
        eventModel.SetOriginalId(payloadEvent.Model().clientId);
        eventModel.SetOriginalTimestamp(payloadEvent.EmitTime());
        eventModel.SetPayloadJSON(payloadEvent.Model().payloadJson);
        events.push_back(eventModel);
    }

    return events;
}

void EventUploader::Run() noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    // Check if we've been cancelled. If so, drain the buffer and upload all remaining events, but don't schedule another poll.
    bool cancelled = m_rc.CancellationToken().IsCancelled();

    for (bool haveEvents = true; haveEvents; )
    {
        ExecutePendingRetries();

        Event event;
        haveEvents = m_buffer->PopFront(event);

        if (haveEvents)
        {
            if (m_pendingPayload.empty())
            {
                m_oldestEventTimeStamp = event.EmitTime();
            }

            m_pendingPayload.push_back(std::move(event));
        }

        std::unique_lock<std::mutex> configLock{ m_configMutex };

        // Upload the batch if: a) The oldest event in the batch has been pending for at least m_maxWaitTimeInSeconds, b) the batch size has reached
        // m_maxEventsPerBatch, or c) the uploader has been cancelled and this was the last event in the buffer
        if ((!m_pendingPayload.empty() && std::time(nullptr) - m_oldestEventTimeStamp >= m_maxWaitTimeInSeconds) ||
            (m_pendingPayload.size() >= m_maxEventsPerBatch) ||
            (!m_pendingPayload.empty() && cancelled && !haveEvents)
        )
        {
            configLock.unlock();

            // Keep track of eventIds of the batch for use in callbacks
            Vector<Event> payload{ std::move(m_pendingPayload) };
            assert(m_pendingPayload.empty());
            auto payloadCopy = payload;
            WriteEvents(std::move(payload), m_rc.Derive()).Finally([payload = std::move(payloadCopy), sharedThis = shared_from_this(), eventHandlers = m_eventHandlers](Result<WriteEventsResponse> result) mutable
                {   
                    sharedThis->ProcessResponse(std::move(result), std::move(payload), eventHandlers, false);
                }
            );
        }
    }

    // Release lock before scheduling next poll just in case Run() is synchronously re-entered (ie. with an immediate queue)
    lock.unlock();

    // Using a polling model to check EventBuffer again after we've emptied it. This is the same algorithm used by XPlatCpp Event Pipeline.
    // Only schedule next poll if the EventUploader has not been cancelled. If it has been cancelled, 'this' may be destroyed here.
    // If a network failure has been detected, a backoff exponential delay will be added to the scheduling
    if (!cancelled)
    {
        m_rc.TaskQueueSubmitWork(shared_from_this(), m_pollDelayInMs + (m_retryCount*m_retryCount*1000) );
    }
}
}

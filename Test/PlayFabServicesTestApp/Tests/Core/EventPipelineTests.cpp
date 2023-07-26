#include "TestAppPch.h"
#include "EventPipelineTests.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/PlatformUtils.h"
#include "Platform/PlayFabPal.h"

using namespace PlayFab::Platform;

namespace PlayFab
{
namespace Test
{

void EventPipelineTests::AddTests()
{
    AddTest("TestEmitPlayStreamEvents", &EventPipelineTests::TestEmitPlayStreamEvents);
    AddTest("TestEmitTelemetryEvents", &EventPipelineTests::TestEmitTelemetryEvents);
    AddTest("TestEmitBarebonesEventPipeline", &EventPipelineTests::TestEmitBarebonesEventPipeline);
    AddTest("TestEmitMalformedPlayStreamEvent", &EventPipelineTests::TestEmitMalformedPlayStreamEvent);
    AddTest("TestEmitMalformedTelemetryEvent", &EventPipelineTests::TestEmitMalformedTelemetryEvent);
    AddTest("TestInvalidParamsRetryPlayStreamEvent", &EventPipelineTests::TestInvalidParamsRetryPlayStreamEvent);
    AddTest("TestInvalidParamsRetryTelemetryEvent", &EventPipelineTests::TestInvalidParamsRetryTelemetryEvent);
    AddTest("TestEmitMalformedJSONPlayStreamEvent", &EventPipelineTests::TestEmitMalformedJSONPlayStreamEvent);
    AddTest("TestEmitMalformedJSONTelemetryEvent", &EventPipelineTests::TestEmitMalformedJSONTelemetryEvent);
    AddTest("TestEmitTelemetryEventsWithInvalidTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithInvalidTelemetryKey);
    AddTest("TestEmitTelemetryEventsWithInactiveTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithInactiveTelemetryKey);
    AddTest("TestEmitTelemetryEventsWithTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithTelemetryKey);
    AddTest("TestEmitTelemetryEventsAddRemoveEntity", &EventPipelineTests::TestEmitTelemetryEventsAddRemoveEntity);
    AddTest("TestEmitTelemetryEventsUpdateBatchSize", &EventPipelineTests::TestEmitTelemetryEventsUpdateBatchSize);
    AddTest("TestRemoveEntityFromPlayStreamPipeline", &EventPipelineTests::TestRemoveEntityFromPlayStreamPipeline);
    AddTest("TestRemoveEntityNoFallbackAuth", &EventPipelineTests::TestRemoveEntityNoFallbackAuth);
}

AsyncOp<void> EventPipelineTests::Initialize()
{
    return CoreTestClass::Initialize().Then([this](Result<void> result) -> AsyncOp<UserPtr>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return Platform::GetDefaultPlatformUser(RunContext());
    })
    .Then([&](Result<UserPtr> result) -> AsyncOp<LoginResult>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        return Platform::LoginDefaultTitlePlayer(ServiceConfig(), result.ExtractPayload(), RunContext());
    })
    .Then([&](Result<LoginResult> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        m_defaultTitlePlayer.emplace(std::move(result.ExtractPayload().entity));
        return S_OK;
    });
}

AsyncOp<void> EventPipelineTests::Uninitialize()
{
    m_defaultTitlePlayer.reset();
    return CoreTestClass::Uninitialize();
}

Entity EventPipelineTests::DefaultTitlePlayer() const
{
    assert(m_defaultTitlePlayer.has_value());
    return *m_defaultTitlePlayer;
}

class EventPipelineTestState
{
public:
    EventPipelineTestState(TestContext& tc, Entity uploadingEntity, RunContext pipelineRunContext);
    EventPipelineTestState(TestContext& tc, PFEventPipelineTelemetryKeyConfig* telemetryKeyConfig, RunContext pipelineRunContext);
    virtual ~EventPipelineTestState() = default;

    TestContext& TestContext() const;
    EventPipeline const& PlayStreamEventPipeline() const;
    EventPipeline const& TelemetryEventPipeline() const;
    EventPipeline const& BarebonesEventPipeline() const;
    EventPipeline const& NoAuthEventPipeline() const;

protected:
    virtual void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount);
    virtual void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount);

private:
    static void CALLBACK OnBatchUploadedHandler(void* context, PFUploadedEvent const* const* events, size_t eventsCount);
    static void CALLBACK OnBatchUploadFailedHandler(void* context, HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount);

    Test::TestContext& m_tc;
    EventPipeline m_playStreamEventPipeline;
    EventPipeline m_telemetryEventPipeline;
    EventPipeline m_barebonesEventPipeline;
    EventPipeline m_noAuthEventPipeline;
};

void EventPipelineTests::TestEmitTelemetryEventsWithInvalidTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.EventsWithInvalidTelemetryKey",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is an inactive telemetry key
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };

            try
            {
                TestContext().AssertEqual(E_PF_TELEMETRY_KEY_INVALID, hr, "Unexpected error received");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            m_completedEvents += eventsCount;

            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        bool unexpectedResponse = false;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ "invalidKey", serviceConfig.Handle()};
    UniquePtr<State> state = MakeUnique<State>(tc, &telemetryKeyConfig, RunContext().Derive());

    HRESULT hr = state->EmitEvents();

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitTelemetryEventsWithInactiveTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.EventsWithInactiveTelemetryKey",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is an inactive telemetry key
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };

            try
            {
                TestContext().AssertEqual(E_PF_TELEMETRY_KEY_DEACTIVATED, hr, "Unexpected error received");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            m_completedEvents += eventsCount;

            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        bool unexpectedResponse = false;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kInactiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(tc, &telemetryKeyConfig, RunContext().Derive());

    HRESULT hr = state->EmitEvents();

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitTelemetryEventsWithTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEntityKey pfEntityKey { "my-unique-ID", "external"};

            PFEvent event
            {
                &pfEntityKey,
                "custom.playfab.events.PlayFab.Test.EventsWithTelemetryKey",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().RecordResult(Result<void>{ hr, errorMessage });

            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void ValidateEvents(Vector<int>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto& id : eventClientIds)
            {
                TestContext().AssertTrue(m_eventIdsToWrite.find(id) != m_eventIdsToWrite.end(), "Unexpected EventId");
            }

            m_completedEvents += eventClientIds.size();
            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(tc, &telemetryKeyConfig, RunContext().Derive());

    HRESULT hr = state->EmitEvents();

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestRemoveEntityNoFallbackAuth(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT RemoveUploadingEntity()
        {
            RETURN_IF_FAILED(TelemetryEventPipeline().RemoveUploadingEntity());

            return S_OK;
        }
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->RemoveUploadingEntity();

    state.reset(); // Destroy pipeline so no callback arrive after we end the test

    if (FAILED(hr))
    {
        hr == E_FAIL ? tc.EndTest(S_OK) : tc.EndTest(hr);
    }
    else
    {
        tc.EndTest(E_FAIL);
    }
}

void EventPipelineTests::TestEmitTelemetryEventsAddRemoveEntity(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents(std::string eventSuffix)
        {
            std::string eventName = "custom.playfab.events.PlayFab.Test." + eventSuffix;

            PFEvent event
            {
                nullptr,
                eventName.c_str(),
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            Set<int> eventIdsToWrite = eventSuffix == "EventsWithEntity" ? m_eventIdsToWriteWithEntity : m_eventIdsToWriteWithoutEntity;

            for (auto& eventId : eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

        HRESULT AddUploadingEntity(Entity uploadingEntity)
        {
            RETURN_IF_FAILED(NoAuthEventPipeline().AddUploadingEntity(std::move(uploadingEntity.Handle())));

            return S_OK;
        }

        HRESULT RemoveUploadingEntity()
        {
            RETURN_IF_FAILED(NoAuthEventPipeline().RemoveUploadingEntity());

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().RecordResult(Result<void>{ hr, errorMessage });

            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void ValidateEvents(Vector<int>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto& id : eventClientIds)
            {
                bool foundClientIdWithEntity = m_eventIdsToWriteWithEntity.find(id) != m_eventIdsToWriteWithEntity.end();
                bool foundClientIdWithoutEntity = m_eventIdsToWriteWithoutEntity.find(id) != m_eventIdsToWriteWithoutEntity.end();

                TestContext().AssertTrue(foundClientIdWithEntity || foundClientIdWithoutEntity, "Unexpected EventId");
            }

            m_completedEvents += eventClientIds.size();
            if (m_completedEvents == m_eventIdsToWriteWithEntity.size() + m_eventIdsToWriteWithoutEntity.size())
            {
                TestContext().EndTest(S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWriteWithEntity{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        Set<int> const m_eventIdsToWriteWithoutEntity{ 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(tc, &telemetryKeyConfig, RunContext().Derive());

    HRESULT hr = state->AddUploadingEntity(DefaultTitlePlayer());

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        hr = state->EmitEvents("EventsWithEntity");
        
        if (FAILED(hr))
        {
            state.reset(); // Destroy pipeline so no callback arrive after we end the test
            tc.EndTest(hr);
        }
        else
        {
            Platform::Sleep(3000);

            hr = state->RemoveUploadingEntity();

            if (FAILED(hr))
            {
                state.reset(); // Destroy pipeline so no callback arrive after we end the test
                tc.EndTest(hr);
            }
            else
            {
                hr = state->EmitEvents("EventsWithoutEntity");

                if (FAILED(hr))
                {
                    state.reset(); // Destroy pipeline so no callback arrive after we end the test
                    tc.EndTest(hr);
                }
                else
                {
                    state.release(); // Reclaimed in Pipeline callbacks
                }
            }
        }
    }
}

void EventPipelineTests::TestEmitTelemetryEventsUpdateBatchSize(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.UpdateBatchSize",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

        HRESULT UpdateConfiguration()
        {
            uint32_t batchSize = 4;
            PFEventPipelineConfig config{ &batchSize };
            RETURN_IF_FAILED(NoAuthEventPipeline().UpdateConfiguration(config));

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().RecordResult(Result<void>{ hr, errorMessage });

            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void ValidateEvents(Vector<int>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto& id : eventClientIds)
            {
                TestContext().AssertTrue(m_eventIdsToWrite.find(id) != m_eventIdsToWrite.end(), "Unexpected EventId");
            }

            m_completedEvents += eventClientIds.size();
            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(tc, &telemetryKeyConfig, RunContext().Derive());

    HRESULT hr = state->UpdateConfiguration();

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        hr = state->EmitEvents();

        if (FAILED(hr))
        {
            state.reset(); // Destroy pipeline so no callback arrive after we end the test
            tc.EndTest(hr);
        }
        else
        {
            state.release(); // Reclaimed in Pipeline callbacks
        }
    }
}

void EventPipelineTests::TestEmitTelemetryEvents(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().RecordResult(Result<void>{ hr, errorMessage });

            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void ValidateEvents(Vector<int>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto& id : eventClientIds)
            {
                TestContext().AssertTrue(m_eventIdsToWrite.find(id) != m_eventIdsToWrite.end(), "Unexpected EventId");
            }

            m_completedEvents += eventClientIds.size();
            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        size_t m_completedEvents{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitPlayStreamEvents(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests",
                "PlayStreamEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
            {
                return atoi(event->clientId);
            });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().RecordResult(Result<void>{ hr, errorMessage });

            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFEvent const* event)
            {
                return atoi(event->clientId);
            });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void ValidateEvents(Vector<int>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto& id : eventClientIds)
            {
                TestContext().AssertTrue(m_eventIdsToWrite.find(id) != m_eventIdsToWrite.end(), "Unexpected EventId");
            }

            m_completedEvents += eventClientIds.size();
            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                TestContext().EndTest(S_OK);

                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        size_t m_completedEvents{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitBarebonesEventPipeline(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            PFEvent event
            {
                nullptr,
                "com.playfab.events.PlayFab.Test.BarebonesEventPipelineTests",
                "PlayStreamEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWrite)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(BarebonesEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:

        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
        tc.EndTest(S_OK);
    }
}

void EventPipelineTests::TestEmitMalformedPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedEvent));

            return S_OK;
        }

    private:
        PFEvent malformedEvent
        {
            nullptr,
            "PlayFab::Test", // Namespace can't contain ':' so this should be rejected
            "MalformedPlayStreamEvent",
            nullptr,
            "{}"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is a malformed event
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "Unexpected error received");
                TestContext().AssertEqual<size_t>(1, eventsCount, "Unexpected eventsCount found");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }
            
            TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);
            UniquePtr<State> reclaim{ this };
        }

        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitMalformedJSONTelemetryEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&event));
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&malformedJSONEvent));
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&event));

            return S_OK;
        }

    private:
        PFEvent event
        {
            nullptr,
            "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests",
            "TelemetryEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedJSONEvent
        {
            nullptr,
            "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests",
            "MalformedJSONTelemetryEvent",
            nullptr,
            "{"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is a malformed event
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_JSON_CONTENT, hr, "Unexpected error received");
                TestContext().AssertEqual<size_t>(3, eventsCount, "Unexpected eventsCount found");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);
            UniquePtr<State> reclaim{ this };
        }

        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitMalformedJSONPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&event));
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedJSONEvent));
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&event));

            return S_OK;
        }

    private:
        PFEvent event
        {
            nullptr,
            "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests",
            "PlayStreamEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedJSONEvent
        {
            nullptr,
            "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests", // Namespace can't contain ':' so this should be rejected
            "MalformedJSONPlayStreamEvent",
            nullptr,
            "{"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is a malformed event
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_JSON_CONTENT, hr, "Unexpected error received");
                TestContext().AssertEqual<size_t>(3, eventsCount, "Unexpected eventsCount found");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);
            UniquePtr<State> reclaim{ this };
        }

        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestEmitMalformedTelemetryEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&malformedEvent));

            return S_OK;
        }

    private:
        PFEvent malformedEvent
        {
            nullptr,
            "PlayFab::Test", // Namespace can't contain ':' so this should be rejected
            "MalformedTelemetryEvent",
            nullptr,
            "{}"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
        {
            // Batch should fail validation because there is a malformed event
            TestContext().EndTest(E_FAIL);
            UniquePtr<State> reclaim{ this };
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "Unexpected error received");
                TestContext().AssertEqual<size_t>(1, eventsCount, "Unexpected eventsCount found");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            TestContext().EndTest(unexpectedResponse ? E_FAIL : S_OK);
            UniquePtr<State> reclaim{ this };
        }

        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestInvalidParamsRetryPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            for (auto& eventId : m_retryEventIdsToWrite)
            {
                event.clientId = eventId.c_str();
                RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&event));
            }

            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedEvent));
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedEventLongNamespace));
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedEventLongEventId));

            return S_OK;
        }

    private:
        PFEvent event
        {
            nullptr,
            "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests",
            "PlayStreamEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEvent
        {
            nullptr,
            "PlayFab::Test", // Namespace can't contain ':' so this should be rejected
            "MalformedPlayStreamEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEventLongNamespace
        {
            nullptr,
            "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests.long.namespace", // Namespace can't contain ':' so this should be rejected
            "MalformedPlayStreamEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEventLongEventId
        {
            nullptr,
            "com.playfab.events.PlayFab.Test.PlayStreamEventPipelineTests",
            "MalformedTelemetryEvent",
            "MalformedPlayStreamEventIdLongerThan32Characters",
            "{}"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<const char*> uploadedEvents(eventsCount);

            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return event->clientId;
                });

            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "translatedError");
                TestContext().AssertEqual<size_t>(3, eventsCount, "eventsCount");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            if (unexpectedResponse)
            {
                TestContext().EndTest(E_FAIL);
                UniquePtr<State> reclaim{ this };
            }
            else
            {
                if (handlerCompleted)
                {
                    TestContext().EndTest(S_OK);
                    UniquePtr<State> reclaim{ this };
                }
                else
                {
                    handlerCompleted = true;
                }
            }
        }

        void ValidateEvents(Vector<const char*>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            try
            {
                TestContext().AssertEqual(int(eventClientIds.size()), 2, "Unexpected events size");

                for (auto id : eventClientIds)
                {
                    std::string str(id);
                    TestContext().AssertTrue(m_retryEventIdsToWrite.find(str) != m_retryEventIdsToWrite.end(), "Unexpected EventId");
                }
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            if (unexpectedResponse)
            {
                TestContext().EndTest(S_OK);
                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
            else
            {
                if (handlerCompleted)
                {
                    TestContext().EndTest(S_OK);
                    lock.unlock();
                    UniquePtr<State> reclaim{ this };
                }
                else
                {
                    handlerCompleted = true;
                }
            }
        }

        std::mutex m_mutex;
        Set<std::string> const m_retryEventIdsToWrite{ "retryEventId1", "retryEventId2" };
        bool handlerCompleted = false;
        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestInvalidParamsRetryTelemetryEvent(TestContext& tc)
{   
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT EmitEvents()
        {
            for (auto& eventId : m_retryEventIdsToWrite)
            {
                event.clientId = eventId.c_str();
                RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&event));
            }

            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&malformedEvent));
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&malformedEventLongNamespace));
            RETURN_IF_FAILED(TelemetryEventPipeline().EmitEvent(&malformedEventLongEventId));

            return S_OK;
        }

    private:
        PFEvent event
        {
            nullptr,
            "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests",
            "TelemetryEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEvent
        {
            nullptr,
            "PlayFab::Test", // Namespace can't contain ':' so this should be rejected
            "MalformedTelemetryEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEventLongNamespace
        {
            nullptr,
            "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests.long.namespace", // Namespace can't contain ':' so this should be rejected
            "MalformedTelemetryEvent",
            nullptr,
            "{}"
        };

        PFEvent malformedEventLongEventId
        {
            nullptr,
            "custom.playfab.events.PlayFab.Test.TelemetryEventPipelineTests",
            "MalformedTelemetryEvent",
            "MalformedTelemetryEventIdLongerThan32Characters",
            "{}"
        };

        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount)
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<const char*> uploadedEvents(eventsCount);

            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return event->clientId;
                });

            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            try
            {
                TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "translatedError");
                TestContext().AssertEqual<size_t>(3, eventsCount, "eventsCount");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }

            if (unexpectedResponse)
            {
                TestContext().EndTest(E_FAIL);
                UniquePtr<State> reclaim{ this };
            }
            else
            {
                if (handlerCompleted)
                {
                    TestContext().EndTest(S_OK);
                    UniquePtr<State> reclaim{ this };
                }
                else
                {
                    handlerCompleted = true;
                }
            }
        }

        void ValidateEvents(Vector<const char *>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            try
            {
                TestContext().AssertEqual(int(eventClientIds.size()), 2, "Unexpected events size");

                for (auto id : eventClientIds)
                {
                    std::string str(id);
                    TestContext().AssertTrue(m_retryEventIdsToWrite.find(str) != m_retryEventIdsToWrite.end(), "Unexpected EventId");
                }
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
                unexpectedResponse = true;
            }
            
            if (unexpectedResponse)
            {
                TestContext().EndTest(S_OK);
                lock.unlock();
                UniquePtr<State> reclaim{ this };
            }
            else
            {
                if (handlerCompleted)
                {
                    TestContext().EndTest(S_OK);
                    lock.unlock();
                    UniquePtr<State> reclaim{ this };
                }
                else
                {
                    handlerCompleted = true;
                }
            }
        }

        std::mutex m_mutex;
        Set<std::string> const m_retryEventIdsToWrite{ "retryEventId1", "retryEventId2" };
        bool handlerCompleted = false;
        bool unexpectedResponse = false;
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->EmitEvents();
    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(hr);
    }
    else
    {
        state.release(); // Reclaimed in Pipeline callbacks
    }
}

void EventPipelineTests::TestRemoveEntityFromPlayStreamPipeline(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT RemoveUploadingEntity()
        {
            RETURN_IF_FAILED(PlayStreamEventPipeline().RemoveUploadingEntity());

            return S_OK;
        }
    };

    UniquePtr<State> state = MakeUnique<State>(tc, DefaultTitlePlayer(), RunContext().Derive());

    HRESULT hr = state->RemoveUploadingEntity();

    if (FAILED(hr))
    {
        state.reset(); // Destroy pipeline so no callback arrive after we end the test
        tc.EndTest(S_OK);
    }
    else
    {
        state.reset(); // Reclaimed in Pipeline callbacks
    }
}

// EventPipelineTestState Implementation
EventPipelineTestState::EventPipelineTestState(Test::TestContext& tc, Entity uploadingEntity, RunContext pipelineRunContext)
    : m_tc{ tc },
    m_playStreamEventPipeline
    {
        PFEventPipelineType::PlayStream,
        std::move(uploadingEntity.Handle()),
        pipelineRunContext.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    },
    m_telemetryEventPipeline
    {
        PFEventPipelineType::Telemetry,
        std::move(uploadingEntity.Handle()),
        pipelineRunContext.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    },
    m_barebonesEventPipeline
    {
        PFEventPipelineType::PlayStream,
        std::move(uploadingEntity.Handle()),
        pipelineRunContext.TaskQueueHandle()
    }
{
}

// EventPipelineTestState Implementation
EventPipelineTestState::EventPipelineTestState(Test::TestContext& tc, PFEventPipelineTelemetryKeyConfig* telemetryKeyConfig, RunContext pipelineRunContext)
    : m_tc{ tc },
    m_noAuthEventPipeline {
        telemetryKeyConfig,
        pipelineRunContext.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    }
{
}

TestContext& EventPipelineTestState::TestContext() const
{
    return m_tc;
}

EventPipeline const& EventPipelineTestState::PlayStreamEventPipeline() const
{
    return m_playStreamEventPipeline;
}

EventPipeline const& EventPipelineTestState::TelemetryEventPipeline() const
{
    return m_telemetryEventPipeline;
}

EventPipeline const& EventPipelineTestState::BarebonesEventPipeline() const
{
    return m_barebonesEventPipeline;
}

EventPipeline const& EventPipelineTestState::NoAuthEventPipeline() const
{
    return m_noAuthEventPipeline;
}

void EventPipelineTestState::OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/)
{
    // Override to handle batch uploaded events
}

void EventPipelineTestState::OnBatchUploadFailed(HRESULT /*hr*/, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t /*eventsCount*/)
{
    // Override to handle batch upload failed events
}

void CALLBACK EventPipelineTestState::OnBatchUploadedHandler(void* context, PFUploadedEvent const* const* events, size_t eventsCount)
{
    auto testState = static_cast<EventPipelineTestState*>(context);
    testState->OnBatchUploaded(events, eventsCount);
}

void CALLBACK EventPipelineTestState::OnBatchUploadFailedHandler(void* context, HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
{
    auto testState = static_cast<EventPipelineTestState*>(context);
    testState->OnBatchUploadFailed(hr, errorMessage, events, eventsCount);
}

}
}

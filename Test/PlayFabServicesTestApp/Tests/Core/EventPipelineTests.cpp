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

EventPipelineTests::EventPipelineTests(TestTitleData testTitleData) :
    CoreTestClass{ testTitleData }
{
}

void EventPipelineTests::AddTests()
{
    // Telemetry Pipeline Tests
    AddTest("TestEmitTelemetryEvents", &EventPipelineTests::TestEmitTelemetryEvents);
    AddTest("TestEmitMalformedTelemetryEvent", &EventPipelineTests::TestEmitMalformedTelemetryEvent);
    AddTest("TestInvalidParamsRetryTelemetryEvent", &EventPipelineTests::TestInvalidParamsRetryTelemetryEvent);
    AddTest("TestEmitMalformedJSONTelemetryEvent", &EventPipelineTests::TestEmitMalformedJSONTelemetryEvent);
    AddTest("TestEmitTelemetryEventsWithInvalidTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithInvalidTelemetryKey);
    AddTest("TestEmitTelemetryEventsWithInactiveTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithInactiveTelemetryKey);
    AddTest("TestEmitTelemetryEventsWithTelemetryKey", &EventPipelineTests::TestEmitTelemetryEventsWithTelemetryKey);
    AddTest("TestEmitTelemetryEventsAddRemoveEntity", &EventPipelineTests::TestEmitTelemetryEventsAddRemoveEntity);
    AddTest("TestEmitTelemetryEventsUpdateBatchSize", &EventPipelineTests::TestEmitTelemetryEventsUpdateBatchSize);
    AddTest("TestRemoveEntityNoFallbackAuth", &EventPipelineTests::TestRemoveEntityNoFallbackAuth);
    AddTest("TestEmitCompressedTelemetryEvents", &EventPipelineTests::TestEmitCompressedTelemetryEvents);

    // PlayStream Pipeline Tests
    AddTest("TestEmitPlayStreamEvents", &EventPipelineTests::TestEmitPlayStreamEvents);
    AddTest("TestEmitMalformedPlayStreamEvent", &EventPipelineTests::TestEmitMalformedPlayStreamEvent);
    AddTest("TestInvalidParamsRetryPlayStreamEvent", &EventPipelineTests::TestInvalidParamsRetryPlayStreamEvent);
    AddTest("TestEmitMalformedJSONPlayStreamEvent", &EventPipelineTests::TestEmitMalformedJSONPlayStreamEvent);
    AddTest("TestRemoveEntityFromPlayStreamPipeline", &EventPipelineTests::TestRemoveEntityFromPlayStreamPipeline);
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
    m_currentTestState.reset();
    return CoreTestClass::Uninitialize();
}

Entity EventPipelineTests::DefaultTitlePlayer() const
{
    assert(m_defaultTitlePlayer.has_value());
    return *m_defaultTitlePlayer;
}

void EventPipelineTests::BeginTest(UniquePtr<EventPipelineTestState>&& state)
{
    if (m_currentTestState)
    {
        TRACE_ERROR("Previous test was not cleaned up, its state will be destroyed now");
    }

    m_currentTestState = std::move(state);
    HRESULT hr = m_currentTestState->BeginTest();
    if (FAILED(hr))
    {
        m_currentTestState->TestContext().EndTest(hr);
        m_currentTestState.reset();
    }
}

void EventPipelineTests::CompleteTest()
{
    TestContext& tc = m_currentTestState->TestContext();
    m_currentTestState.reset();
    tc.EndTest(S_OK);
}

void EventPipelineTests::TestEmitTelemetryEventsWithInvalidTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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
        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/) override
        {
            // Batch should fail validation because there is an inactive telemetry key
            TestContext().RecordResult(Result<void>{ E_FAIL, "Batch upload succeeded with invalid key" });
            m_testClass.CompleteTest();
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            TestContext().AssertEqual(E_PF_TELEMETRY_KEY_INVALID, hr, "Unexpected error received");

            m_completedEvents += eventsCount;
            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ "invalidKey", serviceConfig.Handle()};
    auto state = MakeUnique<State>(*this, tc, &telemetryKeyConfig, RunContext().Derive());

    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitTelemetryEventsWithInactiveTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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
        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t /*eventsCount*/) override
        {
            // Batch should fail validation because there is an inactive telemetry key
            TestContext().RecordResult(Result<void>{ E_FAIL, "Batch upload succeeded with inactive key" });
            m_testClass.CompleteTest();
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };

            try
            {
                TestContext().AssertEqual(E_PF_TELEMETRY_KEY_DEACTIVATED, hr, "Upload result");
            }
            catch (Exception& e)
            {
                TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
            }

            m_completedEvents += eventsCount;

            if (m_completedEvents == m_eventIdsToWrite.size())
            {
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kInactiveTelemetryKey, serviceConfig.Handle() };
    auto state = MakeUnique<State>(*this, tc, &telemetryKeyConfig, RunContext().Derive());

    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitTelemetryEventsWithTelemetryKey(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount) override
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
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    auto state = MakeUnique<State>(*this, tc, &telemetryKeyConfig, RunContext().Derive());

    BeginTest(std::move(state));
}

void EventPipelineTests::TestRemoveEntityNoFallbackAuth(TestContext& tc)
{
    EventPipeline pipeline{ PFEventPipelineType::Telemetry, DefaultTitlePlayer().Handle(), nullptr };
    HRESULT hr = pipeline.RemoveUploadingEntity();
    tc.EndTest<void>(FAILED(hr) ? S_OK : E_FAIL);
}

void EventPipelineTests::TestEmitTelemetryEventsAddRemoveEntity(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
        {
            RETURN_IF_FAILED(NoAuthEventPipeline().AddUploadingEntity(m_testClass.DefaultTitlePlayer().Handle()));

            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.EventsWithEntity",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWriteWithEntity)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

    private:
        HRESULT RemoveUploadingEntityAndEmitEvents()
        {
            RETURN_IF_FAILED(NoAuthEventPipeline().RemoveUploadingEntity());

            PFEvent event
            {
                nullptr,
                "custom.playfab.events.PlayFab.Test.EventsWithoutEntity",
                "TelemetryEvent",
                nullptr,
                "{}"
            };

            for (auto& eventId : m_eventIdsToWriteWithoutEntity)
            {
                std::string clientId = std::to_string(eventId);
                event.clientId = clientId.c_str();
                RETURN_IF_FAILED(NoAuthEventPipeline().EmitEvent(&event));
            }

            return S_OK;
        }

        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount) override
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
            auto completedEvents = m_completedEvents;
            lock.unlock();

            if (completedEvents == m_eventIdsToWriteWithEntity.size())
            {
                HRESULT hr = RemoveUploadingEntityAndEmitEvents();
                if (FAILED(hr))
                {
                    TestContext().RecordResult<void>(hr);
;                   m_testClass.CompleteTest();
                }
            }
            else if (completedEvents == m_eventIdsToWriteWithEntity.size() + m_eventIdsToWriteWithoutEntity.size())
            {
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWriteWithEntity{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        Set<int> const m_eventIdsToWriteWithoutEntity{ 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(*this, tc, &telemetryKeyConfig, RunContext().Derive());

    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitTelemetryEventsUpdateBatchSize(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
        {
            uint32_t batchSize = 4;
            PFEventPipelineConfig config{ &batchSize };
            RETURN_IF_FAILED(NoAuthEventPipeline().UpdateConfiguration(config));

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
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        size_t m_completedEvents{ 0 };
    };

    PlayFab::Test::ServiceConfig serviceConfig = ServiceConfig();
    PFEventPipelineTelemetryKeyConfig telemetryKeyConfig{ kActiveTelemetryKey, serviceConfig.Handle() };
    UniquePtr<State> state = MakeUnique<State>(*this, tc, &telemetryKeyConfig, RunContext().Derive());

    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitTelemetryEvents(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
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
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        size_t m_completedEvents{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitPlayStreamEvents(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
            {
                return atoi(event->clientId);
            });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount) override
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
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1 };
        size_t m_completedEvents{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitMalformedPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
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
            TestContext().RecordResult<void>(E_FAIL);
            m_testClass.CompleteTest();
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "Unexpected error received");
            TestContext().AssertEqual<size_t>(1, eventsCount, "Unexpected eventsCount found");
            m_testClass.CompleteTest();
        }
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitMalformedJSONTelemetryEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
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

        void OnBatchUploaded(PFUploadedEvent const* const* /*events*/, size_t eventsCount)
        {
            m_eventsUploaded += eventsCount;

            // Batch should fail validation because there is a malformed event
            if (m_eventsUploaded >= 3)
            {
                m_testClass.CompleteTest();
            }
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            m_eventsUploaded += eventsCount;

            TestContext().AssertEqual(E_PF_INVALID_JSON_CONTENT, hr, "Unexpected error received");
            if (m_eventsUploaded >= 3)
            {
                TestContext().EndTest(S_OK);
            }
        }

        std::atomic<size_t> m_eventsUploaded{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitMalformedJSONPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
        {
            RETURN_IF_FAILED(PlayStreamEventPipeline().EmitEvent(&malformedJSONEvent));

            return S_OK;
        }

    private:
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
            TestContext().RecordResult<void>(E_FAIL);
            m_testClass.CompleteTest();
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            TestContext().AssertEqual(E_PF_INVALID_JSON_CONTENT, hr, "Unexpected error received");
            TestContext().AssertEqual<size_t>(1, eventsCount, "Unexpected eventsCount found");
            m_testClass.CompleteTest();
        }
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestEmitMalformedTelemetryEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest()
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
            TestContext().RecordResult<void>(E_FAIL);
            m_testClass.CompleteTest();
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount)
        {
            TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "Unexpected error received");
            TestContext().AssertEqual<size_t>(1, eventsCount, "Unexpected eventsCount found");

            m_testClass.CompleteTest();
        }
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestInvalidParamsRetryPlayStreamEvent(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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

        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_eventsUploaded += eventsCount;
            Vector<const char*> uploadedEvents(eventsCount);

            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return event->clientId;
                });

            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount) override
        {
            m_eventsUploaded += eventsCount;
            
            TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "translatedError");

            if (m_eventsUploaded >= 5)
            {
                m_testClass.CompleteTest();
            }
        }

        void ValidateEvents(Vector<const char*>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto id : eventClientIds)
            {
                TestContext().AssertTrue(m_retryEventIdsToWrite.find(id) != m_retryEventIdsToWrite.end(), "Unexpected EventId");
            }

            if (m_eventsUploaded >= 5)
            {
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<std::string> const m_retryEventIdsToWrite{ "retryEventId1", "retryEventId2" };
        std::atomic<size_t> m_eventsUploaded{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestInvalidParamsRetryTelemetryEvent(TestContext& tc)
{   
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
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

        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_eventsUploaded += eventsCount;
            Vector<const char*> uploadedEvents(eventsCount);

            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return event->clientId;
                });

            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* /*errorMessage*/, PFEvent const* const* /*events*/, size_t eventsCount) override
        {
            m_eventsUploaded += eventsCount;

            TestContext().AssertEqual(E_PF_INVALID_PARAMS, hr, "translatedError");

            if (m_eventsUploaded >= 5)
            {
                m_testClass.CompleteTest();
            }
        }

        void ValidateEvents(Vector<const char *>&& eventClientIds, std::unique_lock<std::mutex>&& lock)
        {
            for (auto id : eventClientIds)
            {
                std::string str(id);
                TestContext().AssertTrue(m_retryEventIdsToWrite.find(str) != m_retryEventIdsToWrite.end(), "Unexpected EventId");
            }

            if (m_eventsUploaded >= 5)
            {
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<std::string> const m_retryEventIdsToWrite{ "retryEventId1", "retryEventId2" };
        std::atomic<size_t> m_eventsUploaded{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

void EventPipelineTests::TestRemoveEntityFromPlayStreamPipeline(TestContext& tc)
{
    EventPipeline pipeline{ PFEventPipelineType::PlayStream, DefaultTitlePlayer().Handle(), nullptr };
    HRESULT hr = pipeline.RemoveUploadingEntity();
    tc.EndTest<void>(FAILED(hr) ? S_OK : E_FAIL);
}

void EventPipelineTests::TestEmitCompressedTelemetryEvents(TestContext& tc)
{
    class State : public EventPipelineTestState
    {
    public:
        using EventPipelineTestState::EventPipelineTestState;

        HRESULT BeginTest() override
        {
            HCCompressionLevel compressionLevel{ HCCompressionLevel::Medium };
            PFEventPipelineConfig config{ nullptr, nullptr, nullptr, &compressionLevel };
            RETURN_IF_FAILED(TelemetryEventPipeline().UpdateConfiguration(config));

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
        void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount) override
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            Vector<int> uploadedEvents(eventsCount);
            std::transform(events, events + eventsCount, uploadedEvents.begin(), [&](PFUploadedEvent const* event)
                {
                    return atoi(event->clientId);
                });
            ValidateEvents(std::move(uploadedEvents), std::move(lock));
        }

        void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount) override
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
                lock.unlock();
                m_testClass.CompleteTest();
            }
        }

        std::mutex m_mutex;
        Set<int> const m_eventIdsToWrite{ 0, 1 };
        size_t m_completedEvents{ 0 };
    };

    UniquePtr<State> state = MakeUnique<State>(*this, tc, DefaultTitlePlayer(), RunContext().Derive());
    BeginTest(std::move(state));
}

// EventPipelineTestState Implementation
EventPipelineTestState::EventPipelineTestState(EventPipelineTests& testClass, Test::TestContext& testContext, Entity uploadingEntity, RunContext rc)
    : m_testClass{ testClass },
    m_rc{ std::move(rc) },
    m_testContext{ testContext },
    m_playStreamEventPipeline
    {
        PFEventPipelineType::PlayStream,
        std::move(uploadingEntity.Handle()),
        m_rc.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    },
    m_telemetryEventPipeline
    {
        PFEventPipelineType::Telemetry,
        std::move(uploadingEntity.Handle()),
        m_rc.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    },
    m_barebonesEventPipeline
    {
        PFEventPipelineType::PlayStream,
        std::move(uploadingEntity.Handle()),
        m_rc.TaskQueueHandle()
    }
{
}

// EventPipelineTestState Implementation
EventPipelineTestState::EventPipelineTestState(EventPipelineTests& testClass, Test::TestContext& testContext, PFEventPipelineTelemetryKeyConfig* telemetryKeyConfig, RunContext rc)
    : m_testClass{ testClass },
    m_rc{ std::move(rc) },
    m_testContext{ testContext },
    m_noAuthEventPipeline {
        telemetryKeyConfig,
        m_rc.TaskQueueHandle(),
        &EventPipelineTestState::OnBatchUploadedHandler,
        &EventPipelineTestState::OnBatchUploadFailedHandler,
        this
    }
{
}

TestContext& EventPipelineTestState::TestContext() const
{
    return m_testContext;
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
    try
    {
        testState->OnBatchUploaded(events, eventsCount);
    }
    catch (Exception& e)
    {
        testState->TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
        testState->m_testClass.CompleteTest();
    }
}

void CALLBACK EventPipelineTestState::OnBatchUploadFailedHandler(void* context, HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount)
{
    auto testState = static_cast<EventPipelineTestState*>(context);
    try
    {
        testState->OnBatchUploadFailed(hr, errorMessage, events, eventsCount);
    }
    catch (Exception& e)
    {
        testState->TestContext().RecordResult(Result<void>{ E_FAIL, e.what() });
        testState->m_testClass.CompleteTest();
    }
}

}
}

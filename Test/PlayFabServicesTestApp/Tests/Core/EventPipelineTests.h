#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
namespace Test
{

constexpr char kActiveTelemetryKey[]{ "AQAAAAAADhjXAIWNJNeN2T4" };
constexpr char kInactiveTelemetryKey[]{ "AQAAAAAADhjXPF5gRF669kI" };


class EventPipelineTests;

// Base class for async test state used by tests
class EventPipelineTestState
{
public:
    EventPipelineTestState(EventPipelineTests& testClass, TestContext& testContext, Entity uploadingEntity, RunContext rc);
    EventPipelineTestState(EventPipelineTests& testClass, TestContext& testContext, PFEventPipelineTelemetryKeyConfig* telemetryKeyConfig, RunContext rc);
    virtual ~EventPipelineTestState() = default;

    virtual HRESULT BeginTest() = 0;

    TestContext& TestContext() const;
    EventPipeline const& PlayStreamEventPipeline() const;
    EventPipeline const& TelemetryEventPipeline() const;
    EventPipeline const& BarebonesEventPipeline() const;
    EventPipeline const& NoAuthEventPipeline() const;

protected:
    virtual void OnBatchUploaded(PFUploadedEvent const* const* events, size_t eventsCount);
    virtual void OnBatchUploadFailed(HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount);

    EventPipelineTests& m_testClass;

private:
    static void CALLBACK OnBatchUploadedHandler(void* context, PFUploadedEvent const* const* events, size_t eventsCount);
    static void CALLBACK OnBatchUploadFailedHandler(void* context, HRESULT hr, const char* errorMessage, PFEvent const* const* events, size_t eventsCount);

    RunContext m_rc;
    Test::TestContext& m_testContext;
    EventPipeline const m_playStreamEventPipeline;
    EventPipeline const m_telemetryEventPipeline;
    EventPipeline const m_barebonesEventPipeline;
    EventPipeline const m_noAuthEventPipeline;
};

class EventPipelineTests : public CoreTestClass
{
public:
    EventPipelineTests(TestTitleData testTitleData);
    ~EventPipelineTests() = default;

    Entity DefaultTitlePlayer() const;

    // Helpers methods for async tests
    void BeginTest(UniquePtr<EventPipelineTestState>&& state);
    void CompleteTest();

private:
    // TestClass overrides
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    // Test Methods
    void TestEmitTelemetryEvents(TestContext& testContext);
    void TestEmitPlayStreamEvents(TestContext& testContext);
    void TestEmitMalformedPlayStreamEvent(TestContext& testContext);
    void TestEmitMalformedTelemetryEvent(TestContext& testContext);
    void TestInvalidParamsRetryTelemetryEvent(TestContext& testContext);
    void TestInvalidParamsRetryPlayStreamEvent(TestContext& testContext);
    void TestEmitMalformedJSONPlayStreamEvent(TestContext& testContext);
    void TestEmitMalformedJSONTelemetryEvent(TestContext& testContext);
    void TestEmitTelemetryEventsWithInvalidTelemetryKey(TestContext& testContext);
    void TestEmitTelemetryEventsWithInactiveTelemetryKey(TestContext& testContext);
    void TestEmitTelemetryEventsWithTelemetryKey(TestContext& testContext);
    void TestEmitTelemetryEventsAddRemoveEntity(TestContext& testContext);
    void TestEmitTelemetryEventsUpdateBatchSize(TestContext& testContext);
    void TestRemoveEntityFromPlayStreamPipeline(TestContext& testContext);
    void TestRemoveEntityNoFallbackAuth(TestContext& testContext);
    void TestEmitCompressedTelemetryEvents(TestContext& testContext);

private:
    std::optional<Entity> m_defaultTitlePlayer;

    friend class EventPipelineTestState;
    UniquePtr<EventPipelineTestState> m_currentTestState;
};

}
}

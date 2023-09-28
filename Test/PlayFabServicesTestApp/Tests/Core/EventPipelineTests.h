#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
namespace Test
{

constexpr char kActiveTelemetryKey[]{ "AQAAAAAADhjXAIWNJNeN2T4" };
constexpr char kInactiveTelemetryKey[]{ "AQAAAAAADhjXPF5gRF669kI" };

class EventPipelineTests : public CoreTestClass
{
public:
    using CoreTestClass::CoreTestClass;

private:
    // TestClass overrides
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    Entity DefaultTitlePlayer() const;

    // Test Methods
    void TestEmitTelemetryEvents(TestContext& testContext);
    void TestEmitPlayStreamEvents(TestContext& testContext);
    void TestEmitBarebonesEventPipeline(TestContext& testContext);
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
};

}
}

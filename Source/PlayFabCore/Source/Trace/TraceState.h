#pragma once

#include <httpClient/trace.h>
#include "RunContext.h"
#include "Platform/LocalStorage.h"

namespace PlayFab
{

// Static TraceSettings that need to be configured prior to calling PFInitialize
struct TraceSettings
{
    bool enableTraceToFile{ false };
    char traceFileDirectory[FILENAME_MAX]{ 0 };
    bool traceToDebugger{ false };
};

TraceSettings& GetTraceSettings();

// Interface for a trace output
struct TraceOutput
{
    virtual ~TraceOutput() = default;

    virtual void TraceMessage(const char* message) = 0;
    virtual void Stop() {};
};

// Global TraceState that is independent from PFCoreGlobalState so that initialization and cleanup can be Traced.
// Tracing is routed through LHC trace infrastructure to leverage existing tracing features (message formatting,
// client callbacks, etc.)
class TraceState : public ITerminationListener
{
public:
    TraceState(const TraceState&) = delete;
    TraceState& operator=(TraceState) = delete;
    ~TraceState() noexcept;

    static HRESULT Create(RunContext&& runContext, LocalStorage localStorage) noexcept;
    static HRESULT Get(SharedPtr<TraceState>& state) noexcept;
    static HRESULT CleanupAsync(XAsyncBlock* async) noexcept;

private:
    TraceState(RunContext&& initContext, RunContext&& traceContext, LocalStorage localStorage) noexcept;

    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data) noexcept;

    static void CALLBACK TraceCallback(
        _In_z_ const char* areaName,
        _In_ HCTraceLevel level,
        _In_ uint64_t threadId,
        _In_ uint64_t timestamp,
        _In_z_ const char* message
    ) noexcept;

    // ITerminationListener
    void OnTerminated(void* context) noexcept override;

    RunContext m_runContext;
    Vector<SharedPtr<TraceOutput>> m_outputs;
};



}

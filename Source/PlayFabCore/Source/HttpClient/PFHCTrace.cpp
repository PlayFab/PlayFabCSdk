#include "stdafx.h"
#include <playfab/httpClient/PFHCTrace.h>
#include <playfab/core/PFTrace.h>
#include <httpClient/trace.h>
#include "GlobalState.h"
#include "Trace/TraceState.h"

using namespace PlayFab;

PF_API PFHCSettingsSetTraceLevel(
    _In_ PFHCTraceLevel traceLevel
) noexcept
{
    return HCSettingsSetTraceLevel(static_cast<HCTraceLevel>(traceLevel));
}

PF_API PFHCSettingsGetTraceLevel(
    _Out_ PFHCTraceLevel* traceLevel
) noexcept
{
    HCTraceLevel hcTraceLevel;
    HRESULT hr = HCSettingsGetTraceLevel(&hcTraceLevel);
    *traceLevel = static_cast<PFHCTraceLevel>(hcTraceLevel);
    return hr;
}

PF_API_(void) PFHCTraceSetClientCallback(_In_opt_ PFHCTraceCallback* callback) noexcept
{
    auto& settings = GetTraceSettings();
    settings.traceCallback = (HCTraceCallback*)callback;
}

PF_API_(void) PFHCTraceSetTraceToDebugger(_In_ bool traceToDebugger) noexcept
{
    // This impl differs between LHC to so it routes the debug trace to PFCore's trace's formatting.
    // This can be called before or after PFInit.  If its called after then TraceState::EnableTraceToDebugger 
    // will setup the internal debug output. Tracing to debugger cannot be disabled after PFInit.
    auto& settings = GetTraceSettings();
    settings.traceToDebugger = traceToDebugger;
}

// Platform Hooks

PF_API PFHCTraceSetPlatformCallbacks(
    _In_ PFHCTracePlatformThisThreadIdCallback* threadIdCallback,
    _In_opt_ void* threadIdContext,
    _In_ PFHCTracePlatformWriteMessageToDebuggerCallback* writeToDebuggerCallback,
    _In_opt_ void* writeToDebuggerContext
) noexcept
{
    return HCTraceSetPlatformCallbacks(
        threadIdCallback, 
        threadIdContext, 
        (HCTracePlatformWriteMessageToDebuggerCallback*)writeToDebuggerCallback, 
        writeToDebuggerContext
    );
}

// Implementation

PF_API_(void) PFHCTraceImplMessage(
    struct PFHCTraceImplArea const* area,
    PFHCTraceLevel level,
    _Printf_format_string_ char const* format,
    ...
) noexcept
{
    va_list varArgs{};
    va_start(varArgs, format);
    HCTraceImplMessage_v((HCTraceImplArea*)area, (HCTraceLevel)level, format, varArgs);
    va_end(varArgs);
}

PF_API_(uint64_t) PFHCTraceImplScopeId() noexcept
{
    return HCTraceImplScopeId();
}
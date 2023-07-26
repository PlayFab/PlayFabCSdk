#include "stdafx.h"
#include "DebugTraceOutput.h"

namespace PlayFab
{

void DebugTraceOutput::TraceMessage(const char* message) noexcept
{
    TraceSettings& settings = GetTraceSettings();
    if (settings.traceToDebugger)
    {
#if defined(_WIN32)
        OutputDebugStringA(message);
#endif
    }
}

}

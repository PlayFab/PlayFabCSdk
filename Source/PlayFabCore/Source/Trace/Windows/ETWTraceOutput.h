#pragma once

#include "Trace/TraceState.h"
#include <TraceLoggingProvider.h>

namespace PlayFab
{

class ETWTraceOutput : public TraceOutput
{
public:
    ETWTraceOutput();
    void TraceMessage(const char* message) noexcept override;
};

}
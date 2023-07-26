#pragma once

#include "TraceState.h"

namespace PlayFab
{

// TraceOutput that redirects to debug output
struct DebugTraceOutput : public TraceOutput
{
    void TraceMessage(const char* message) noexcept override;
};

}

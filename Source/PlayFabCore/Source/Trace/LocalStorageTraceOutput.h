#pragma once

#include "TraceState.h"

namespace PlayFab
{

// TraceOutput that redirects to local storage
struct LocalStorageTraceOutput : public TraceOutput
{
public:
    LocalStorageTraceOutput(RunContext rc, LocalStorage localStorage, String traceFileDirectory) noexcept;

    void TraceMessage(const char* message) noexcept override;
    void Stop() noexcept override;

private:
    RunContext m_rc;
    LocalStorage m_localStorage;
    String m_traceFilePath;

    static constexpr char s_traceFilename[] = "PlayFabTrace.txt";
};

}

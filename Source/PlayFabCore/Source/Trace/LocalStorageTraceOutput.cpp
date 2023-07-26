#include "stdafx.h"
#include "LocalStorageTraceOutput.h"

namespace PlayFab
{

LocalStorageTraceOutput::LocalStorageTraceOutput(RunContext rc, LocalStorage localStorage, String traceFileDirectory) noexcept :
    m_rc{ std::move(rc) },
    m_localStorage{ std::move(localStorage) }
{
    Stringstream filepath;
    filepath << traceFileDirectory << s_traceFilename;

    m_traceFilePath = filepath.str();

    // Clear any existing trace file
    m_localStorage.Clear(m_traceFilePath, m_rc).Wait();
}

void LocalStorageTraceOutput::TraceMessage(const char* message) noexcept
{
    // Write each message to LocalStorage as they arrive. If performance becomes a concern, we could do some in memory buffering
    // and only write to LocalStorage when we have a sufficiently large payload.

    m_localStorage.Write(m_traceFilePath, Vector<uint8_t>(message, message + std::strlen(message)), m_rc).Finally([](Result<void> writeResult)
    {
        // Local Storage write failed. Don't trace this as that would recursively call TraceMessage, just eat the error.
    });
}

void LocalStorageTraceOutput::Stop() noexcept
{
    m_localStorage.Flush(m_rc).Wait();
}

}

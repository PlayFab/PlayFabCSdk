#include "stdafx.h"
#include <playfab/core/PFTrace.h>
#include "Common/PFCoreGlobalState.h"
#include "Trace/TraceState.h"

using namespace PlayFab;

namespace
{
static_assert(static_cast<uint32_t>(PFTraceLevel::Off) == static_cast<uint32_t>(HCTraceLevel::Off));
static_assert(static_cast<uint32_t>(PFTraceLevel::Error) == static_cast<uint32_t>(HCTraceLevel::Error));
static_assert(static_cast<uint32_t>(PFTraceLevel::Warning) == static_cast<uint32_t>(HCTraceLevel::Warning));
static_assert(static_cast<uint32_t>(PFTraceLevel::Important) == static_cast<uint32_t>(HCTraceLevel::Important));
static_assert(static_cast<uint32_t>(PFTraceLevel::Information) == static_cast<uint32_t>(HCTraceLevel::Information));
static_assert(static_cast<uint32_t>(PFTraceLevel::Verbose) == static_cast<uint32_t>(HCTraceLevel::Verbose));

std::atomic<PFTraceLevel> s_traceLevel{ PFTraceLevel::Verbose };
}

PF_API PFSettingsSetTraceLevel(
    _In_ PFTraceLevel traceLevel
) noexcept
{
    if (traceLevel > PFTraceLevel::Verbose)
    {
        return E_INVALIDARG;
    }

    s_traceLevel.store(traceLevel);
    return S_OK;
}

PF_API_(bool) PFTraceIsTraceLevelEnabled(
    _In_ HCTraceLevel traceLevel
) noexcept
{
    PFTraceLevel configuredLevel = s_traceLevel.load();
    return configuredLevel != PFTraceLevel::Off &&
        static_cast<uint32_t>(traceLevel) <= static_cast<uint32_t>(configuredLevel);
}

PF_API PFTraceEnableTraceToFile(
    _In_z_ const char* traceFileDirectory
) noexcept
{
    try
    {
        SharedPtr<PFCoreGlobalState> state;
        PFCoreGlobalState::Get(state);
        RETURN_HR_IF(E_PF_CORE_ALREADY_INITIALIZED, state);

        auto& settings = GetTraceSettings();
        settings.enableTraceToFile = true;
        RETURN_HR_IF(E_INVALIDARG, strlen(traceFileDirectory) >= sizeof(settings.traceFileDirectory));
        StrCpy(settings.traceFileDirectory, sizeof(settings.traceFileDirectory), traceFileDirectory);

        return S_OK;
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFTraceEnableTraceToFile), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

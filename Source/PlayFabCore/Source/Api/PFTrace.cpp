#include "stdafx.h"
#include <playfab/core/PFTrace.h>
#include "Common/PFCoreGlobalState.h"
#include "Trace/TraceState.h"

using namespace PlayFab;

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
        StrCpy(settings.traceFileDirectory, sizeof(settings.traceFileDirectory), traceFileDirectory);

        return S_OK;
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFTraceEnableTraceToFile), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

#pragma once

#include <httpClient/trace.h>
#include <playfab/core/PFPal.h>

extern "C"
{

PF_API_(bool) PFTraceIsTraceLevelEnabled(
    _In_ HCTraceLevel traceLevel
) noexcept;

}

HC_DECLARE_TRACE_AREA(PlayFab);

namespace PlayFab
{

#define PF_TRACE_IF_ENABLED(level, traceStatement) \
    do \
    { \
        if (PFTraceIsTraceLevelEnabled(level)) \
        { \
            traceStatement; \
        } \
    } while (0)

#if HC_TRACE_ERROR_ENABLE
#define TRACE_ERROR(msg, ...)           PF_TRACE_IF_ENABLED(HCTraceLevel::Error, HC_TRACE_ERROR(PlayFab, msg, ##__VA_ARGS__))
#define TRACE_ERROR_HR(failedHr, msg)   PF_TRACE_IF_ENABLED(HCTraceLevel::Error, HC_TRACE_ERROR_HR(PlayFab, failedHr, msg))
#else
#define TRACE_ERROR(msg, ...)
#define TRACE_ERROR_HR(failedHr, msg)
#endif

#if HC_TRACE_WARNING_ENABLE
#define TRACE_WARNING(msg, ...)         PF_TRACE_IF_ENABLED(HCTraceLevel::Warning, HC_TRACE_WARNING(PlayFab, msg, ##__VA_ARGS__))
#define TRACE_WARNING_HR(failedHr, msg) PF_TRACE_IF_ENABLED(HCTraceLevel::Warning, HC_TRACE_WARNING_HR(PlayFab, failedHr, msg))
#else
#define TRACE_WARNING(msg, ...)
#define TRACE_WARNING_HR(failedHr, msg)
#endif

#if HC_TRACE_IMPORTANT_ENABLE
#define TRACE_IMPORTANT(msg, ...)       PF_TRACE_IF_ENABLED(HCTraceLevel::Important, HC_TRACE_IMPORTANT(PlayFab, msg, ##__VA_ARGS__))
#else
#define TRACE_IMPORTANT(msg, ...)
#endif

#if HC_TRACE_INFORMATION_ENABLE
#define TRACE_INFORMATION(msg, ...)     PF_TRACE_IF_ENABLED(HCTraceLevel::Information, HC_TRACE_INFORMATION(PlayFab, msg, ##__VA_ARGS__))
#else
#define TRACE_INFORMATION(msg, ...)
#endif

#if HC_TRACE_VERBOSE_ENABLE
#define TRACE_VERBOSE(msg, ...)         PF_TRACE_IF_ENABLED(HCTraceLevel::Verbose, HC_TRACE_VERBOSE(PlayFab, msg, ##__VA_ARGS__))
#else
#define TRACE_VERBOSE(msg, ...)
#endif

}

#pragma once

#include <playfab/httpClient/PFHCTrace.h>

PFHC_DECLARE_TRACE_AREA(PlayFab);

namespace PlayFab
{

#define TRACE_ERROR(msg, ...)           PFHC_TRACE_ERROR(PlayFab, msg, ##__VA_ARGS__)
#define TRACE_ERROR_HR(failedHr, msg)   PFHC_TRACE_ERROR_HR(PlayFab, failedHr, msg)

#define TRACE_WARNING(msg, ...)         PFHC_TRACE_WARNING(PlayFab, msg, ##__VA_ARGS__)
#define TRACE_WARNING_HR(failedHr, msg) PFHC_TRACE_WARNING_HR(PlayFab, failedHr, msg)

#define TRACE_IMPORTANT(msg, ...)       PFHC_TRACE_IMPORTANT(PlayFab, msg, ##__VA_ARGS__)

#define TRACE_INFORMATION(msg, ...)     PFHC_TRACE_INFORMATION(PlayFab, msg, ##__VA_ARGS__)

#define TRACE_VERBOSE(msg, ...)         PFHC_TRACE_VERBOSE(PlayFab, msg, ##__VA_ARGS__)

}

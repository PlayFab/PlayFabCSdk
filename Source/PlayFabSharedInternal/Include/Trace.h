#pragma once

#include <httpClient/trace.h>

HC_DECLARE_TRACE_AREA(PlayFab);

namespace PlayFab
{

#define TRACE_ERROR(msg, ...)           HC_TRACE_ERROR(PlayFab, msg, ##__VA_ARGS__)
#define TRACE_ERROR_HR(failedHr, msg)   HC_TRACE_ERROR_HR(PlayFab, failedHr, msg)

#define TRACE_WARNING(msg, ...)         HC_TRACE_WARNING(PlayFab, msg, ##__VA_ARGS__)
#define TRACE_WARNING_HR(failedHr, msg) HC_TRACE_WARNING_HR(PlayFab, failedHr, msg)

#define TRACE_IMPORTANT(msg, ...)       HC_TRACE_IMPORTANT(PlayFab, msg, ##__VA_ARGS__)

#define TRACE_INFORMATION(msg, ...)     HC_TRACE_INFORMATION(PlayFab, msg, ##__VA_ARGS__)

#define TRACE_VERBOSE(msg, ...)         HC_TRACE_VERBOSE(PlayFab, msg, ##__VA_ARGS__)

}

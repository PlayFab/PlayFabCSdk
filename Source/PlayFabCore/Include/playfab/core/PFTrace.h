// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>

extern "C"
{

enum class PFTraceLevel : uint32_t
{
    Off = 0,
    Error = 1,
    Warning = 2,
    Important = 3,
    Information = 4,
    Verbose = 5,
};

/// <summary>
/// Sets the trace level for events in the PlayFab trace area.
/// </summary>
/// <param name="traceLevel">The maximum verbosity to emit.</param>
PF_API PFSettingsSetTraceLevel(
    _In_ PFTraceLevel traceLevel
) noexcept;

/// <summary>
/// Enables PlayFab trace logging to a file.
/// </summary>
PF_API PFTraceEnableTraceToFile(
    _In_z_ const char* traceFileDirectory
) noexcept;

}

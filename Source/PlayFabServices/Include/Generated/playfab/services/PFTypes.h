// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
extern "C"
{

#pragma push_macro("IN")
#undef IN

// PlayFab Services HRESULT error codes (FACILITY_XBOX + 0x5A00 + offset)

// Service errors
#define E_PF_SERVICES_NOT_INITIALIZED           MAKE_E_HC(0x5A00L) // 0x89235A00
#define E_PF_SERVICES_ALREADY_INITIALIZED       MAKE_E_HC(0x5A01L) // 0x89235A01

/// <summary>
/// OperationTypes enum.
/// </summary>
enum class PFOperationTypes : uint32_t
{
    Created,
    Updated,
    Deleted,
    None
};

#pragma pop_macro("IN")

}

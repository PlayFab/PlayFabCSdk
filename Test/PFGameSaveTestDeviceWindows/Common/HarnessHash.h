// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include <filesystem>
#include <string>
#include <cstdint>

// Cross-platform hash interface
// Platform-specific implementations are in Platform/<Platform>/HarnessHash_<Platform>.cpp

// HRESULT is defined per-platform; this header assumes it's available
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef int32_t HRESULT;
#endif

namespace HarnessHash
{
    HRESULT ComputeSha256(
        const std::filesystem::path& filePath,
        std::string& hexValue,
        std::string& errorMessage);
}

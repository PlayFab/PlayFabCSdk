// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#pragma once

#include "gtest/gtest.h"
#include <windows.h>
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <array>
#include <assert.h>
#include <atomic>
#include <filesystem>

// PlayFab Services include
#include <playfab/core/PFCore.h>

// Platform specific includes
#if defined(_GAMING_DESKTOP)
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#endif

// PlayFabSharedInternal includes
#include <ResultMacros.h>
#include <Memory.h>
#include <Types.h>
#include <Result.h>
#include <Error.h>
#include <PlatformUtils.h>
#include <Trace.h>
#include <RunContext.h>
#include <AsyncOp.h>
#include <XAsyncOperation.h>

#include <playfab/core/PFCore.h>
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>
#include <playfab/gamesave/PFGameSaveFilesTypes.h>

// Test helper macros
#define EXPECT_SUCCEEDED(hr) EXPECT_TRUE(SUCCEEDED(hr))
#define EXPECT_FAILED(hr) EXPECT_TRUE(FAILED(hr))
#define GTEST_SKIP() do { std::cout << "[ SKIPPED ] Test skipped in this environment" << std::endl; return; } while(0)

// Define ASSERT_SUCCEEDED macro for tests 
#ifndef ASSERT_SUCCEEDED
#define ASSERT_SUCCEEDED(hr) ASSERT_TRUE(SUCCEEDED(hr))
#endif

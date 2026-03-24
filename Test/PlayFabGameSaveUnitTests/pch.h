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
#include "FilePAL.h"
#include "HttpMock.h"

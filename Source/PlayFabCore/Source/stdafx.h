#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <array>
#include <assert.h>
#include <atomic>

// libHttpClient headers
#include <httpClient/pal.h>
#include <httpClient/httpClient.h>
#include <httpClient/async.h>

#include <playfab/core/PFErrors.h>

// PlayFabSharedInternal
#include <Nlohmann.h>
#include <ResultMacros.h>
#include <Memory.h>
#include <Types.h>
#include <Result.h>
#include <Error.h>
#include <PlatformUtils.h>
#include <Trace.h>
#include <RunContext.h>
#include <AsyncOp.h>

// PlayFabCore
#include "Generated/Types.h"

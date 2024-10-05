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
#include <map>

// libHttpClient headers
#include <httpClient/pal.h>
#include <XAsync.h>
#include <XAsyncProvider.h>

// PlayFabSharedInternal
#include <RapidJson.h>
#include <ResultMacros.h>
#include <Memory.h>
#include <Types.h>
#include <Result.h>
#include <Error.h>
#include <PlatformUtils.h>
#include <Trace.h>
#include <RunContext.h>

// PlayFabCore headers
#include <playfab/core/PFCore.h>
#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>

// PlayFabServices headers
#include "Generated/Types.h"

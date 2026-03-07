// Copyright (C) Microsoft Corporation. All rights reserved.
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
#include <XAsync.h>
#include <XAsyncProvider.h>

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

// PlayFabCore headers
#include <playfab/core/PFCore.h>
#include <playfab/core/cpp/Entity.h>
#include <playfab/core/cpp/ServiceConfig.h>
#include <playfab/core/cpp/LocalUser.h>
#include "playfab/core/cpp/EventPipeline.h"


namespace PlayFab
{
    using LocalUser = Wrappers::LocalUser<Allocator>;
}

// PlayFabGameSave headers
#include "Wrappers/Types.h"
#include <playfab/gamesave/PFGameSaveFiles.h>
#include <ApiXAsyncProvider.h>
#include <GameSaveAsyncProvider.h>
#include <XAsyncProvider.h>
#include "ISchedulableTask.h"
#include "HttpRequest.h"
#include "FilePAL.h"
#include "GameSaveTelemetryManager.h"
#include "GameSaveUICallbackInfo.h"
#include "Generated/CacheId.h"
#include "GameSaveHttpClient.h"
#include "JsonUtils.h"
#include "SdkVersion.h"
#include "RunContext.h"
#include "XAsyncOperation.h"
#include "GameSavePlatform.h"
#include "UICallbackManager.h"
#include "ApiXAsyncProvider.h"
#include "BaseModel.h"
#include "CoreTypes.h"
#include "Types.h"
#include "Wrappers/CoreTypes.h"
#include "ExtendedManifestTypes.h"
#include "GameSaveServiceSelector.h"
#include "Utils.h"
#include "GameSaveGlobalState.h"

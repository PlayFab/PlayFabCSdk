// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

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

// PlayFab Services include
#include <playfab/services/PFServices.h>

// Platform specific includes
#if defined(_GAMING_DESKTOP)
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
#include "GDK/GDKPch.h"
#endif

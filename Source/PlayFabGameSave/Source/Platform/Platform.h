// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "PFGameSaveFilesAPIProvider.h"

namespace PlayFab
{
namespace GameSave
{

UniquePtr<GameSaveAPIProvider> PlatformGetAPIProvider(bool forceInproc) noexcept;

} // namespace GameSave
} // namespace PlayFab
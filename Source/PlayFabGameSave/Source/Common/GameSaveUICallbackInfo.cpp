// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"

using namespace PlayFab;

namespace PlayFab
{
namespace GameSave
{

GameSaveUiCallbackInfo& GetGameSaveUiCallbackInfo() noexcept
{
    static GameSaveUiCallbackInfo info{};
    return info;
}

} // namespace GameSave
} // namespace PlayFab

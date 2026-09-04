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

std::mutex& GetGameSaveUiCallbackMutex() noexcept
{
    static std::mutex mtx;
    return mtx;
}

} // namespace GameSave
} // namespace PlayFab

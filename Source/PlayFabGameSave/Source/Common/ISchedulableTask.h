// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

struct ISchedulableTask
{
    virtual ~ISchedulableTask() = default;
    virtual void ScheduleNow() = 0;
};

void TRACE_TASK(const String& msg);

} // namespace GameSave
} // namespace PlayFab
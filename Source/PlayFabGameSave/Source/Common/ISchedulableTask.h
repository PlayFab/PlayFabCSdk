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
    // Pumps the task again after delayMs (debug fault-injection pauses poll with it). Providers that
    // can't delay pump immediately.
    virtual void ScheduleAfter(uint32_t delayMs) { UNREFERENCED_PARAMETER(delayMs); ScheduleNow(); }

    // True once the operation driving this task has been cancelled (XAsyncCancel). A cancelled task
    // must not be parked waiting for a UI response: see UICallbackManager::BeginUIWait.
    virtual bool IsCancellationRequested() const noexcept { return false; }
};

void TRACE_TASK(const String& msg);

} // namespace GameSave
} // namespace PlayFab
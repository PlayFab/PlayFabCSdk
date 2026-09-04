// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "RunContext.h"

namespace PlayFab
{
namespace GameSave
{

class ActiveDevicePollWorker : public ITaskQueueWork, public std::enable_shared_from_this<ActiveDevicePollWorker>
{
public:
    static SharedPtr<ActiveDevicePollWorker> MakeAndStart(
        _In_ Entity const& entity,
        _In_ LocalUser const& localUser,
        _In_ RunContext&& rc
    ) noexcept;

    ~ActiveDevicePollWorker();

    // Stops the poll loop. The worker keeps itself alive by resubmitting to the task queue after
    // every poll, so a replacement worker (each successful AddUser creates one) would otherwise
    // leave the previous instance polling forever - duplicating active-device-changed callbacks
    // and issuing one redundant ListManifests per accumulated worker every interval.
    void Stop() noexcept { m_stopped.store(true); }

#if defined(_DEBUG)
    static void SetInterval(uint32_t interval) { s_interval = interval; }
    static void SetActiveDevicePollForceChange(bool changed) { s_debugForceChange = changed; }
#endif

private:
    ActiveDevicePollWorker(_In_ Entity const& entity, _In_ LocalUser const& localUser, _In_ RunContext&& rc);

    // ITaskQueueWork
    void Run() override;

    // Check validity of token and refresh if needed. Handles all errors and invokes TokenExpiredHandler as needed
    HRESULT CheckActiveDevice() noexcept;

    std::mutex m_mutex;
    std::atomic<bool> m_stopped{ false };
    Entity m_entity;
    LocalUser m_localUser;
    PlayFab::RunContext m_rc;

    static uint32_t s_interval;
    static bool s_debugForceChange;
};

} // namespace GameSave
} // namespace PlayFab

// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "ActiveDevicePollWorker.h"
#include "FolderSyncManager.h"
#include "ProgressHelpers.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

uint32_t ActiveDevicePollWorker::s_interval = 1000 * 60 * 10; // 10 Minutes
bool ActiveDevicePollWorker::s_debugForceChange = false;

#if _DEBUG 
extern "C" PF_API_ATTRIBUTES HRESULT PFGameSaveFilesSetActiveDevicePollForceChangeForDebug()
{
    ActiveDevicePollWorker::SetActiveDevicePollForceChange(true);
    return S_OK;
}

extern "C" PF_API_ATTRIBUTES HRESULT PFGameSaveFilesSetActiveDevicePollIntervalForDebug(uint32_t interval)
{
    ActiveDevicePollWorker::SetInterval(interval);
    return S_OK;
}
#endif

ActiveDevicePollWorker::ActiveDevicePollWorker(
    _In_ Entity const& entity,
    _In_ LocalUser const& localUser,
    _In_ PlayFab::RunContext&& rc) :
    m_entity{ entity },
    m_localUser{ localUser },
    m_rc{ std::move(rc) }
{
}

ActiveDevicePollWorker::~ActiveDevicePollWorker()
{
}

SharedPtr<ActiveDevicePollWorker> ActiveDevicePollWorker::MakeAndStart(
    _In_ Entity const& entity,
    _In_ LocalUser const& localUser,
    _In_ RunContext&& rc
) noexcept
{
    Allocator<ActiveDevicePollWorker> a;
    SharedPtr<ActiveDevicePollWorker> worker{ new (a.allocate(1)) ActiveDevicePollWorker{ entity, localUser, std::move(rc) }, Deleter<ActiveDevicePollWorker>{}, a };

    worker->m_rc.TaskQueueSubmitWork(worker, ActiveDevicePollWorker::s_interval);
    return worker;
}

void ActiveDevicePollWorker::Run()
{
    CheckActiveDevice();
}

HRESULT ActiveDevicePollWorker::CheckActiveDevice() noexcept
{
    SharedPtr<GameSaveGlobalState> state;
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    SharedPtr<FolderSyncManager> folderSync = state->GetFolderSyncManagerFromLocalUser(m_localUser.Handle(), false);
    if (folderSync == nullptr)
    {
        return E_FAIL;
    }

    if (folderSync->IsForcedDisconnectFromCloud())
    {
        return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
    }

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.activeDeviceChangedCallback == nullptr)
    {
        // No callback set so just reschedule ourselves in case it gets set later
        m_rc.TaskQueueSubmitWork(shared_from_this(), s_interval);
        return S_OK;
    }
    auto pThis = shared_from_this();

    ListManifestsRequest request{};
    GameSaveServiceSelector::ListManifests(m_entity, request, m_rc)
    .Finally([folderSync, pThis](Result<ListManifestsResponse> result)
    {            
        if (SUCCEEDED(result.hr)) // ignore network failures here
        {
            ManifestWrapVector manifests = result.Payload().GetManifests();
            
            String saveFolder = folderSync->GetFolder();
            String localDeviceId = GetLocalDeviceID(saveFolder);

            ManifestWrap latestFinalizedPFManifest;
            latestFinalizedPFManifest = ManifestWrap();
            LockStep::TryGetLatestFinalizedManifest(manifests, latestFinalizedPFManifest);
            const ManifestWrap* latestPendingManifest = LockStep::TryGetLatestPendingManifest(manifests, latestFinalizedPFManifest);
            bool deviceIdChanged = false;
            if (latestPendingManifest != nullptr)
            {
                // compare with the device id in the manifests
                const String& latestPendingDeviceId = latestPendingManifest->GetMetadata()->GetDeviceId();
                if (latestPendingDeviceId != localDeviceId)
                {
                    deviceIdChanged = true;
                }
            }
            else
            {
                // no latestPendingManifest means another device has taken the active device, uploaded and released the active device
                deviceIdChanged = true;
                latestPendingManifest = &latestFinalizedPFManifest; // treat the latestFinalizedPFManifest as the last pending for purposes of this function
            }

            if (deviceIdChanged || ActiveDevicePollWorker::s_debugForceChange)
            {
                // if different device id, then:
                // a) force go offline
                folderSync->SetForcedDisconnectFromCloud(true);

                // b) trigger active device change callback
                PFGameSaveDescriptor activeDevice{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(*latestPendingManifest, activeDevice);

                UICallbackManager::TriggerActiveDeviceChangedCallback(pThis->m_rc, pThis->m_localUser, activeDevice); // no issue if callback not set
            }
        }

        // Reschedule ourselves.
        // Note that with this implementation the TokenExpiredHandler will be invoked every s_interval until TODO
        pThis->m_rc.TaskQueueSubmitWork(pThis, s_interval);
    });

    return S_OK;
}


} // namespace GameSave
} // namespace PlayFab

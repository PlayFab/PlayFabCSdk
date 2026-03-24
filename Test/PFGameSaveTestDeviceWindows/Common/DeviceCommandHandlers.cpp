#include "pch.h"
#include "DeviceCommandHandlers.h"
#include "CaptureSaveContainerSnapshot.h"
#include "ConsumeDiskSpace.h"
#include "CopyTargetFolderToSaveFolder.h"
#include "DeleteLocalFolder.h"
#include "DeleteSaveRoot.h"
#include "GatherSnapshot.h"
#include "GatherLogs.h"
#include "GetDebugStats.h"
#include "WriteGameSaveData.h"
#include "CommandHandlerShared.h"
#include "ChaosModeHandler.h"
#include "HttpMockHandlers.h"
#include "PFCommandHandlers.h"
#include "SmokeCommandHandlers.h"
#include "XblHandlers.h"
#include "XGameRuntimeHandlers.h"
#include "XTaskQueueHandlers.h"
#include "XUserHandlers.h"
#include <string>
#include <string_view>
#include <unordered_map>

namespace
{
    using CommandHandlerShared::CreateBaseResult;
    using CommandHandlerShared::MarkSuccess;

    using CommandHandler = CommandResultPayload(*)(
        DeviceGameSaveState* state,
        const std::string& commandId,
        const std::string& command,
        const nlohmann::json& parameters,
        const std::string& deviceId);

    CommandResultPayload HandleDefaultCommand(
        [[maybe_unused]] DeviceGameSaveState* state,
        const std::string& commandId,
        const std::string& command,
        [[maybe_unused]] const nlohmann::json& parameters,
        const std::string& deviceId)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        LogToWindow("Command: [" + deviceId + "] " + command);
        MarkSuccess(payload.result);
        payload.elapsedMs = 0;
        return payload;
    }
}

CommandResultPayload BuildActionResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    static const std::unordered_map<std::string_view, CommandHandler> s_handlers{
        { "XGameRuntimeInitialize", HandleXGameRuntimeInitialize },
        { "XblInitialize", HandleXblInitialize },
        { "XblContextCreateHandle", HandleXblContextCreateHandle },
        { "XblContextCloseHandle", HandleXblContextCloseHandle },
        { "XblCleanupAsync", HandleXblCleanupAsync },
        { "XUserGetId", HandleXUserGetId },
        { "XGameInviteRegisterForEvent", HandleXGameInviteRegisterForEvent },
        { "XGameInviteRegisterForPendingEvent", HandleXGameInviteRegisterForPendingEvent },
        { "WaitForXGameInviteRegisterForEvent", HandleWaitForXGameInviteRegisterForEvent },
        { "WaitForXGameInviteRegisterForPendingEvent", HandleWaitForXGameInviteRegisterForPendingEvent },
        { "XblMultiplayerActivitySendInvitesAsync", HandleXblMultiplayerActivitySendInvitesAsync },
        { "InteractiveInviteLoop", HandleInteractiveInviteLoop },
        { "XGameUiShowPlayerPickerAsync", HandleXGameUiShowPlayerPickerAsync },
        { "XGameInviteUnregisterForEvent", HandleXGameInviteUnregisterForEvent },
        { "XGameInviteUnregisterForPendingEvent", HandleXGameInviteUnregisterForPendingEvent },
        { "XGameInviteAcceptPendingInvite", HandleXGameInviteAcceptPendingInvite },
        { "PFInitialize", HandlePFInitialize },
        { "PFServicesInitialize", HandlePFServicesInitialize },
        { "PFServiceConfigCreateHandle", HandlePFServiceConfigCreateHandle },
        { "PFServiceConfigCloseHandle", HandlePFServiceConfigCloseHandle },
        { "PFGameSaveFilesInitialize", HandlePFGameSaveFilesInitialize },
        { "PFGameSaveFilesSetUiCallbacks", HandlePFGameSaveFilesSetUiCallbacks },
        { "PFGameSaveFilesSetActiveDeviceChangedCallback", HandlePFGameSaveFilesSetActiveDeviceChangedCallback },
        { "PFGameSaveFilesResetCloudAsync", HandlePFGameSaveFilesResetCloudAsync },
        { "PFGameSaveFilesAddUserWithUiAsync", HandlePFGameSaveFilesAddUserWithUiAsync },
        { "PFGameSaveFilesSetUiSyncFailedResponse", HandlePFGameSaveFilesSetUiSyncFailedResponse },
        { "PFGameSaveFilesSetUiActiveDeviceContentionResponse", HandlePFGameSaveFilesSetUiActiveDeviceContentionResponse },
        { "PFGameSaveFilesSetUiConflictResponse", HandlePFGameSaveFilesSetUiConflictResponse },
        { "PFGameSaveFilesSetUiOutOfStorageResponse", HandlePFGameSaveFilesSetUiOutOfStorageResponse },
        { "PFGameSaveFilesSetUiProgressResponse", HandlePFGameSaveFilesSetUiProgressResponse },
        { "PFGameSaveFilesSetUiSyncFailedAutoResponse", HandlePFGameSaveFilesSetUiSyncFailedAutoResponse },
        { "PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse", HandlePFGameSaveFilesSetUiActiveDeviceContentionAutoResponse },
        { "PFGameSaveFilesSetUiConflictAutoResponse", HandlePFGameSaveFilesSetUiConflictAutoResponse },
        { "PFGameSaveFilesSetUiOutOfStorageAutoResponse", HandlePFGameSaveFilesSetUiOutOfStorageAutoResponse },
        { "PFGameSaveFilesSetUiProgressAutoResponse", HandlePFGameSaveFilesSetUiProgressAutoResponse },
        { "PFGameSaveFilesSetMockDeviceIdForDebug", HandlePFGameSaveFilesSetMockDeviceIdForDebug },
        { "PFGameSaveFilesSetForceOutOfStorageErrorForDebug", HandlePFGameSaveFilesSetForceOutOfStorageErrorForDebug },
        { "PFGameSaveFilesSetForceSyncFailedErrorForDebug", HandlePFGameSaveFilesSetForceSyncFailedErrorForDebug },
        { "PFGameSaveFilesSetWriteManifestsToDiskForDebug", HandlePFGameSaveFilesSetWriteManifestsToDiskForDebug },
        { "PFGameSaveFilesSetActiveDevicePollForceChangeForDebug", HandlePFGameSaveFilesSetActiveDevicePollForceChangeForDebug },
        { "PFGameSaveFilesSetActiveDevicePollIntervalForDebug", HandlePFGameSaveFilesSetActiveDevicePollIntervalForDebug },
        { "PFGameSaveFilesUploadWithUiAsync", HandlePFGameSaveFilesUploadWithUiAsync },
        { "PFGameSaveFilesGetFolder", HandlePFGameSaveFilesGetFolder },
        { "PFGameSaveFilesGetRemainingQuota", HandlePFGameSaveFilesGetRemainingQuota },
        { "PFGameSaveFilesIsConnectedToCloud", HandlePFGameSaveFilesIsConnectedToCloud },
        { "DoChaosMode", HandleDoChaosMode },
        { "ConfigureHttpMock", HandleConfigureHttpMock },
        { "ClearHttpMocks", HandleClearHttpMocks },
        { "CaptureSaveContainerSnapshot", HandleCaptureSaveContainerSnapshot },
        { "WriteGameSaveData", HandleWriteGameSaveData },
        { "GatherSnapshot", HandleGatherSnapshot },
        { "GatherSnapsnot", HandleGatherSnapshot },
        { "GatherLogs", HandleGatherLogs },
        { "GetDebugStats", HandleGetDebugStats },
        { "DeleteLocalFolder", HandleDeleteLocalFolder },
        { "DeleteSaveRoot", HandleDeleteSaveRoot },
        { "ConsumeDiskSpace", HandleConsumeDiskSpace },
        { "CopyTargetFolderToSaveFolder", HandleCopyTargetFolderToSaveFolder },
        { "ReleaseDiskSpace", HandleReleaseDiskSpace },
        { "XTaskQueueCreate", HandleXTaskQueueCreate },
        { "XTaskQueueCloseHandle", HandleXTaskQueueCloseHandle },
        { "XUserAddAsync", HandleXUserAddAsync },
        { "XUserCloseHandle", HandleXUserCloseHandle },
        { "XUserPlatformRemoteConnectSetEventHandlers", HandleXUserPlatformRemoteConnectSetEventHandlers },
        { "XUserPlatformSpopPromptSetEventHandlers", HandleXUserPlatformSpopPromptSetEventHandlers },
        { "XUserPlatformSpopPromptComplete", HandleXUserPlatformSpopPromptComplete },
        { "PFLocalUserCreateHandleWithXboxUser", HandlePFLocalUserCreateHandleWithXboxUser },
        { "PFLocalUserCreateHandleWithPersistedLocalId", HandlePFLocalUserCreateHandleWithPersistedLocalId },
        { "PFLocalUserCloseHandle", HandlePFLocalUserCloseHandle },
        { "PFGameSaveFilesUninitializeAsync", HandlePFGameSaveFilesUninitializeAsync },
        { "PFServicesUninitializeAsync", HandlePFServicesUninitializeAsync },
        { "PFUninitializeAsync", HandlePFUninitializeAsync },
        { "SmokeCommandEcho", HandleSmokeCommandEcho },
        { "SmokeDelay", HandleSmokeDelay },
        { "SmokeTestFailure", HandleSmokeTestFailure },
        { "WaitForUserInput", HandleWaitForUserInput },
        { "PFGameSaveFilesSetSaveDescriptionAsync", HandlePFGameSaveFilesSetSaveDescriptionAsync },
        { "PFGameSaveFilesGetSaveDescription", HandlePFGameSaveFilesGetSaveDescription }
    };

    auto it = s_handlers.find(command);
    if (it != s_handlers.end())
    {
        return it->second(state, commandId, command, parameters, deviceId);
    }

    return HandleDefaultCommand(state, commandId, command, parameters, deviceId);
}

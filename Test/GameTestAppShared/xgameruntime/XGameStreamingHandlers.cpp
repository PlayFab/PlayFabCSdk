#include "pch.h"

#include "XGameStreamingHandlers.h"
//
//#include "CommandHandlerShared.h"
//#include "DeviceGameSaveState.h"
//#include "DeviceLogging.h"
//#include <XGameStreaming.h>
//#include "CommandRegistry.h"
//
//static XTaskQueueRegistrationToken s_connectionStateChangedToken{};
//static XTaskQueueRegistrationToken s_clientPropertiesChangedToken{};
//
//CommandResultPayload HandleXGameStreamingInitialize(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingInitialize();
//            LogToWindowFormat("XGameStreamingInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingUninitialize(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingUninitialize();
//            LogToWindow("XGameStreamingUninitialize called");
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingIsStreaming(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            bool isStreaming = XGameStreamingIsStreaming();
//            LogToWindowFormat("XGameStreamingIsStreaming (result=%s)", isStreaming ? "true" : "false");
//            payload.result["isStreaming"] = isStreaming;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetClientCount(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            uint32_t clientCount = XGameStreamingGetClientCount();
//            LogToWindowFormat("XGameStreamingGetClientCount (count=%u)", clientCount);
//            payload.result["clientCount"] = clientCount;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetClients(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            uint32_t clientCount = XGameStreamingGetClientCount();
//            if (clientCount == 0)
//            {
//                LogToWindow("XGameStreamingGetClients: no clients connected");
//                payload.result["clientCount"] = 0;
//                return S_OK;
//            }
//            std::vector<XGameStreamingClientId> clients(clientCount);
//            uint32_t clientsUsed = 0;
//            HRESULT hr = XGameStreamingGetClients(clientCount, clients.data(), &clientsUsed);
//            LogToWindowFormat("XGameStreamingGetClients (hr=0x%08X, clientsUsed=%u)", static_cast<uint32_t>(hr), clientsUsed);
//            if (SUCCEEDED(hr))
//            {
//                payload.result["clientCount"] = clientsUsed;
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetConnectionState(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            XGameStreamingConnectionState connState = XGameStreamingGetConnectionState(XGameStreamingNullClientId);
//            LogToWindowFormat("XGameStreamingGetConnectionState (state=%u)", static_cast<uint32_t>(connState));
//            payload.result["connectionState"] = static_cast<uint32_t>(connState);
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingRegisterConnectionStateChanged(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingRegisterConnectionStateChanged(
//                state->taskQueue,
//                nullptr,
//                [](void*, XGameStreamingClientId client, XGameStreamingConnectionState connState)
//                {
//                    LogToWindowFormat("XGameStreaming ConnectionStateChanged callback (client=0x%016llX, state=%u)",
//                        static_cast<unsigned long long>(client), static_cast<uint32_t>(connState));
//                },
//                &s_connectionStateChangedToken);
//            LogToWindowFormat("XGameStreamingRegisterConnectionStateChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["token"] = static_cast<uint64_t>(s_connectionStateChangedToken.token);
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingUnregisterConnectionStateChanged(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            bool result = XGameStreamingUnregisterConnectionStateChanged(s_connectionStateChangedToken, true);
//            LogToWindowFormat("XGameStreamingUnregisterConnectionStateChanged (result=%s)", result ? "true" : "false");
//            s_connectionStateChangedToken = {};
//            payload.result["unregistered"] = result;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingHideTouchControls(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingHideTouchControls();
//            LogToWindow("XGameStreamingHideTouchControls called");
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingHideTouchControlsOnClient(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingHideTouchControlsOnClient(XGameStreamingAllClients);
//            LogToWindow("XGameStreamingHideTouchControlsOnClient called");
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingShowTouchControlLayout(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingShowTouchControlLayout(nullptr);
//            LogToWindow("XGameStreamingShowTouchControlLayout called");
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingShowTouchControlLayoutOnClient(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingShowTouchControlLayoutOnClient(XGameStreamingAllClients, nullptr);
//            LogToWindow("XGameStreamingShowTouchControlLayoutOnClient called");
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingRegisterClientPropertiesChanged(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingRegisterClientPropertiesChanged(
//                XGameStreamingAllClients,
//                state->taskQueue,
//                nullptr,
//                [](void*, XGameStreamingClientId client, uint32_t count, XGameStreamingClientProperty* props)
//                {
//                    (void)props;
//                    LogToWindowFormat("XGameStreaming ClientPropertiesChanged callback (client=0x%016llX, count=%u)",
//                        static_cast<unsigned long long>(client), count);
//                },
//                &s_clientPropertiesChangedToken);
//            LogToWindowFormat("XGameStreamingRegisterClientPropertiesChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["token"] = static_cast<uint64_t>(s_clientPropertiesChangedToken.token);
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingUnregisterClientPropertiesChanged(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            bool result = XGameStreamingUnregisterClientPropertiesChanged(
//                XGameStreamingAllClients, s_clientPropertiesChangedToken, true);
//            LogToWindowFormat("XGameStreamingUnregisterClientPropertiesChanged (result=%s)", result ? "true" : "false");
//            s_clientPropertiesChangedToken = {};
//            payload.result["unregistered"] = result;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetStreamPhysicalDimensions(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            uint32_t horizontalMm = 0;
//            uint32_t verticalMm = 0;
//            HRESULT hr = XGameStreamingGetStreamPhysicalDimensions(XGameStreamingNullClientId, &horizontalMm, &verticalMm);
//            LogToWindowFormat("XGameStreamingGetStreamPhysicalDimensions (hr=0x%08X, h=%u, v=%u)", static_cast<uint32_t>(hr), horizontalMm, verticalMm);
//            if (SUCCEEDED(hr))
//            {
//                payload.result["horizontalMm"] = horizontalMm;
//                payload.result["verticalMm"] = verticalMm;
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetStreamAddedLatency(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            uint32_t avgInputLatencyUs = 0;
//            uint32_t avgOutputLatencyUs = 0;
//            uint32_t stdDevUs = 0;
//            HRESULT hr = XGameStreamingGetStreamAddedLatency(XGameStreamingNullClientId, &avgInputLatencyUs, &avgOutputLatencyUs, &stdDevUs);
//            LogToWindowFormat("XGameStreamingGetStreamAddedLatency (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["averageInputLatencyUs"] = avgInputLatencyUs;
//                payload.result["averageOutputLatencyUs"] = avgOutputLatencyUs;
//                payload.result["standardDeviationUs"] = stdDevUs;
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetServerLocationNameSize(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            size_t nameSize = XGameStreamingGetServerLocationNameSize();
//            LogToWindowFormat("XGameStreamingGetServerLocationNameSize (size=%zu)", nameSize);
//            payload.result["nameSize"] = nameSize;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetServerLocationName(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            size_t nameSize = XGameStreamingGetServerLocationNameSize();
//            if (nameSize == 0)
//            {
//                LogToWindow("XGameStreamingGetServerLocationName: name size is 0");
//                payload.result["serverLocationName"] = "";
//                return S_OK;
//            }
//            std::vector<char> buffer(nameSize);
//            HRESULT hr = XGameStreamingGetServerLocationName(nameSize, buffer.data());
//            LogToWindowFormat("XGameStreamingGetServerLocationName (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["serverLocationName"] = std::string(buffer.data());
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingIsTouchInputEnabled(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            bool touchInputEnabled = false;
//            HRESULT hr = XGameStreamingIsTouchInputEnabled(XGameStreamingNullClientId, &touchInputEnabled);
//            LogToWindowFormat("XGameStreamingIsTouchInputEnabled (hr=0x%08X, enabled=%s)", static_cast<uint32_t>(hr), touchInputEnabled ? "true" : "false");
//            if (SUCCEEDED(hr))
//            {
//                payload.result["touchInputEnabled"] = touchInputEnabled;
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetLastFrameDisplayed(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            D3D12XBOX_FRAME_PIPELINE_TOKEN framePipelineToken = 0;
//            HRESULT hr = XGameStreamingGetLastFrameDisplayed(XGameStreamingNullClientId, &framePipelineToken);
//            LogToWindowFormat("XGameStreamingGetLastFrameDisplayed (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["framePipelineToken"] = static_cast<uint64_t>(framePipelineToken);
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetAssociatedFrame(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            D3D12XBOX_FRAME_PIPELINE_TOKEN framePipelineToken = 0;
//            HRESULT hr = XGameStreamingGetAssociatedFrame(nullptr, &framePipelineToken);
//            LogToWindowFormat("XGameStreamingGetAssociatedFrame (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetGamepadPhysicality(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            XGameStreamingGamepadPhysicality physicality{};
//            HRESULT hr = XGameStreamingGetGamepadPhysicality(nullptr, &physicality);
//            LogToWindowFormat("XGameStreamingGetGamepadPhysicality (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingUpdateTouchControlsState(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingUpdateTouchControlsState(0, nullptr);
//            LogToWindowFormat("XGameStreamingUpdateTouchControlsState (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingUpdateTouchControlsStateOnClient(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingUpdateTouchControlsStateOnClient(XGameStreamingAllClients, 0, nullptr);
//            LogToWindowFormat("XGameStreamingUpdateTouchControlsStateOnClient (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingShowTouchControlsWithStateUpdate(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingShowTouchControlsWithStateUpdate(nullptr, 0, nullptr);
//            LogToWindowFormat("XGameStreamingShowTouchControlsWithStateUpdate (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingShowTouchControlsWithStateUpdateOnClient(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingShowTouchControlsWithStateUpdateOnClient(XGameStreamingAllClients, nullptr, 0, nullptr);
//            LogToWindowFormat("XGameStreamingShowTouchControlsWithStateUpdateOnClient (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetTouchBundleVersionNameSize(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            size_t nameSize = XGameStreamingGetTouchBundleVersionNameSize(XGameStreamingNullClientId);
//            LogToWindowFormat("XGameStreamingGetTouchBundleVersionNameSize (size=%zu)", nameSize);
//            payload.result["nameSize"] = nameSize;
//            return S_OK;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetTouchBundleVersion(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            size_t nameSize = XGameStreamingGetTouchBundleVersionNameSize(XGameStreamingNullClientId);
//            XVersion version{};
//            std::vector<char> versionName(nameSize > 0 ? nameSize : 1);
//            HRESULT hr = XGameStreamingGetTouchBundleVersion(XGameStreamingNullClientId, &version, nameSize, versionName.data());
//            LogToWindowFormat("XGameStreamingGetTouchBundleVersion (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["major"] = version.major;
//                payload.result["minor"] = version.minor;
//                payload.result["build"] = version.build;
//                payload.result["revision"] = version.revision;
//                if (nameSize > 0)
//                {
//                    payload.result["versionName"] = std::string(versionName.data());
//                }
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetClientIPAddress(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            char ipAddress[ClientIPAddressMaxBytes]{};
//            HRESULT hr = XGameStreamingGetClientIPAddress(XGameStreamingNullClientId, ClientIPAddressMaxBytes, ipAddress);
//            LogToWindowFormat("XGameStreamingGetClientIPAddress (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["ipAddress"] = std::string(ipAddress);
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetSessionId(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            char sessionId[SessionIdMaxBytes]{};
//            size_t sessionIdUsed = 0;
//            HRESULT hr = XGameStreamingGetSessionId(XGameStreamingNullClientId, SessionIdMaxBytes, sessionId, &sessionIdUsed);
//            LogToWindowFormat("XGameStreamingGetSessionId (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["sessionId"] = std::string(sessionId);
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingGetDisplayDetails(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            XGameStreamingDisplayDetails displayDetails{};
//            HRESULT hr = XGameStreamingGetDisplayDetails(XGameStreamingNullClientId, 1920 * 1080, 16.0f / 9.0f, 9.0f / 16.0f, &displayDetails);
//            LogToWindowFormat("XGameStreamingGetDisplayDetails (hr=0x%08X)", static_cast<uint32_t>(hr));
//            if (SUCCEEDED(hr))
//            {
//                payload.result["preferredWidth"] = displayDetails.preferredWidth;
//                payload.result["preferredHeight"] = displayDetails.preferredHeight;
//                payload.result["maxPixels"] = displayDetails.maxPixels;
//                payload.result["maxWidth"] = displayDetails.maxWidth;
//                payload.result["maxHeight"] = displayDetails.maxHeight;
//            }
//            return hr;
//        });
//}
//
//CommandResultPayload HandleXGameStreamingSetResolution(
//    [[maybe_unused]] DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload&) -> HRESULT
//        {
//            HRESULT hr = XGameStreamingSetResolution(1920, 1080);
//            LogToWindowFormat("XGameStreamingSetResolution (hr=0x%08X)", static_cast<uint32_t>(hr));
//            return hr;
//        });
//}
//
//// Self-registration of commands
//static CommandRegistrar s_registrar({
//    { "XGameStreamingGetAssociatedFrame", HandleXGameStreamingGetAssociatedFrame },
//    { "XGameStreamingGetClientCount", HandleXGameStreamingGetClientCount },
//    { "XGameStreamingGetClientIPAddress", HandleXGameStreamingGetClientIPAddress },
//    { "XGameStreamingGetClients", HandleXGameStreamingGetClients },
//    { "XGameStreamingGetConnectionState", HandleXGameStreamingGetConnectionState },
//    { "XGameStreamingGetDisplayDetails", HandleXGameStreamingGetDisplayDetails },
//    { "XGameStreamingGetGamepadPhysicality", HandleXGameStreamingGetGamepadPhysicality },
//    { "XGameStreamingGetLastFrameDisplayed", HandleXGameStreamingGetLastFrameDisplayed },
//    { "XGameStreamingGetServerLocationName", HandleXGameStreamingGetServerLocationName },
//    { "XGameStreamingGetServerLocationNameSize", HandleXGameStreamingGetServerLocationNameSize },
//    { "XGameStreamingGetSessionId", HandleXGameStreamingGetSessionId },
//    { "XGameStreamingGetStreamAddedLatency", HandleXGameStreamingGetStreamAddedLatency },
//    { "XGameStreamingGetStreamPhysicalDimensions", HandleXGameStreamingGetStreamPhysicalDimensions },
//    { "XGameStreamingGetTouchBundleVersion", HandleXGameStreamingGetTouchBundleVersion },
//    { "XGameStreamingGetTouchBundleVersionNameSize", HandleXGameStreamingGetTouchBundleVersionNameSize },
//    { "XGameStreamingHideTouchControls", HandleXGameStreamingHideTouchControls },
//    { "XGameStreamingHideTouchControlsOnClient", HandleXGameStreamingHideTouchControlsOnClient },
//    { "XGameStreamingInitialize", HandleXGameStreamingInitialize },
//    { "XGameStreamingIsStreaming", HandleXGameStreamingIsStreaming },
//    { "XGameStreamingIsTouchInputEnabled", HandleXGameStreamingIsTouchInputEnabled },
//    { "XGameStreamingRegisterClientPropertiesChanged", HandleXGameStreamingRegisterClientPropertiesChanged },
//    { "XGameStreamingRegisterConnectionStateChanged", HandleXGameStreamingRegisterConnectionStateChanged },
//    { "XGameStreamingSetResolution", HandleXGameStreamingSetResolution },
//    { "XGameStreamingShowTouchControlLayout", HandleXGameStreamingShowTouchControlLayout },
//    { "XGameStreamingShowTouchControlLayoutOnClient", HandleXGameStreamingShowTouchControlLayoutOnClient },
//    { "XGameStreamingShowTouchControlsWithStateUpdate", HandleXGameStreamingShowTouchControlsWithStateUpdate },
//    { "XGameStreamingShowTouchControlsWithStateUpdateOnClient", HandleXGameStreamingShowTouchControlsWithStateUpdateOnClient },
//    { "XGameStreamingUninitialize", HandleXGameStreamingUninitialize },
//    { "XGameStreamingUnregisterClientPropertiesChanged", HandleXGameStreamingUnregisterClientPropertiesChanged },
//    { "XGameStreamingUnregisterConnectionStateChanged", HandleXGameStreamingUnregisterConnectionStateChanged },
//    { "XGameStreamingUpdateTouchControlsState", HandleXGameStreamingUpdateTouchControlsState },
//    { "XGameStreamingUpdateTouchControlsStateOnClient", HandleXGameStreamingUpdateTouchControlsStateOnClient }
//});

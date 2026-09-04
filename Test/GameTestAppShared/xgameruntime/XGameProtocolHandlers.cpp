#include "pch.h"

#include "XGameProtocolHandlers.h"

// #include "CommandHandlerShared.h"
// #include "DeviceGameSaveState.h"
// #include "DeviceLogging.h"
// #include <XGameProtocol.h>
// #include "CommandRegistry.h"

// CommandResultPayload HandleXGameProtocolRegisterForActivation(
//     [[maybe_unused]] DeviceGameSaveState* state,
//     const std::string& commandId,
//     const std::string& command,
//     [[maybe_unused]] const nlohmann::json& parameters,
//     const std::string& deviceId)
// {
//     return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//         [&](CommandResultPayload&) -> HRESULT
//         {
//             XTaskQueueRegistrationToken token{};
//             const HRESULT hr = XGameProtocolRegisterForActivation(state->taskQueue, nullptr,
//                 [](void*, const char*) {}, &token);
//             LogToWindowFormat("XGameProtocolRegisterForActivation (hr=0x%08X)", static_cast<uint32_t>(hr));
//             if (SUCCEEDED(hr)) { XGameProtocolUnregisterForActivation(token, true); }
//             return hr;
//         });
// }

// CommandResultPayload HandleXGameProtocolUnregisterForActivation(
//     [[maybe_unused]] DeviceGameSaveState* state,
//     const std::string& commandId,
//     const std::string& command,
//     [[maybe_unused]] const nlohmann::json& parameters,
//     const std::string& deviceId)
// {
//     return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//         [&](CommandResultPayload& payload) -> HRESULT
//         {
//             XTaskQueueRegistrationToken token{};
//             HRESULT hr = XGameProtocolRegisterForActivation(state->taskQueue, nullptr,
//                 [](void*, const char*) {}, &token);
//             RETURN_IF_FAILED(hr);
//             const bool result = XGameProtocolUnregisterForActivation(token, true);
//             LogToWindowFormat("XGameProtocolUnregisterForActivation (result=%s)", result ? "true" : "false");
//             payload.result["unregistered"] = result;
//             return S_OK;
//         });
// }

// // Self-registration of commands
// static CommandRegistrar s_registrar({
//     { "XGameProtocolRegisterForActivation", HandleXGameProtocolRegisterForActivation },
//     { "XGameProtocolUnregisterForActivation", HandleXGameProtocolUnregisterForActivation }
// });
 
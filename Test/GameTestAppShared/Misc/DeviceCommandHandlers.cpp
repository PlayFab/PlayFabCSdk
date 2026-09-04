// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "DeviceCommandHandlers.h"
#include "CommandHandlerShared.h"
#include "CommandRegistry.h"
#include "DeviceLogging.h"

CommandResultPayload BuildActionResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    auto handler = CommandRegistry::Instance().Find(command);
    if (handler)
    {
        try
        {
            return handler(state, commandId, command, parameters, deviceId);
        }
        catch (const std::exception& ex)
        {
            LogToWindow("EXCEPTION in handler [" + command + "]: " + ex.what());
            CommandResultPayload payload{};
            payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
            CommandHandlerShared::MarkFailure(payload.result, E_FAIL, std::string("Unhandled exception: ") + ex.what());
            return payload;
        }
        catch (...)
        {
            LogToWindow("UNKNOWN EXCEPTION in handler [" + command + "]");
            CommandResultPayload payload{};
            payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
            CommandHandlerShared::MarkFailure(payload.result, E_FAIL, "Unknown exception in handler");
            return payload;
        }
    }

    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
    LogToWindow("Unknown command: [" + deviceId + "] " + command);
    CommandHandlerShared::MarkFailure(payload.result, E_NOTIMPL, "Unknown command: " + command);
    payload.elapsedMs = 0;
    return payload;
}

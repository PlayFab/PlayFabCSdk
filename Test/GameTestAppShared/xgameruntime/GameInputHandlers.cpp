#include "pch.h"

#include "GameInputHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <GameInput.h>
#include "CommandRegistry.h"

CommandResultPayload HandleGameInputCreate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            IGameInput* gameInput = nullptr;
            const HRESULT hr = GameInputCreate(&gameInput);
            LogToWindowFormat("GameInputCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (gameInput) { gameInput->Release(); }
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "GameInputCreate", HandleGameInputCreate }
});

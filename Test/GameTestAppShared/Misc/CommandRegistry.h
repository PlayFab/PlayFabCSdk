// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "DeviceCommandHandlers.h"
#include <string>
#include <string_view>
#include <unordered_map>

// Function pointer type for command handlers
using CommandHandler = CommandResultPayload(*)(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Global command registry — handler files self-register via CommandRegistrar
class CommandRegistry
{
public:
    static CommandRegistry& Instance()
    {
        static CommandRegistry s_instance;
        return s_instance;
    }

    void Register(std::string_view name, CommandHandler handler)
    {
        m_handlers[name] = handler;
    }

    CommandHandler Find(const std::string& name) const
    {
        auto it = m_handlers.find(name);
        return (it != m_handlers.end()) ? it->second : nullptr;
    }

    const auto& GetAll() const { return m_handlers; }

private:
    CommandRegistry() = default;
    std::unordered_map<std::string_view, CommandHandler> m_handlers;
};
    
// RAII helper — place as a file-level static to auto-register commands at startup
struct CommandRegistrar
{
    CommandRegistrar(std::initializer_list<std::pair<std::string_view, CommandHandler>> commands)
    {
        for (auto& [name, handler] : commands)
        {
            CommandRegistry::Instance().Register(name, handler);
        }
    }
};

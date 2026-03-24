// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

// Cross-platform application entry point interface
// Platform-specific implementations are in Platform/<Platform>/DeviceApplication_<Platform>.cpp

// Platform-specific entry point - each platform defines its own
#ifdef _WIN32
#include <Windows.h>
int RunDeviceApplication(HINSTANCE instance, LPSTR commandLine, int showCommand);
#else
int RunDeviceApplication(const std::string& commandLine);
#endif

// Common application lifecycle functions
struct DeviceGameSaveState;

void DeviceApplicationInitialize(DeviceGameSaveState* state, const char* commandLine);
void DeviceApplicationRunLoop(DeviceGameSaveState* state);
void DeviceApplicationShutdown(DeviceGameSaveState* state);

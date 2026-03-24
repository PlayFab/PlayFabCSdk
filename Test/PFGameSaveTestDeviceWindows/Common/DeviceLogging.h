// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include <string>

// Cross-platform logging interface
// Platform-specific implementations are in Platform/<Platform>/DeviceLogging_<Platform>.cpp

// Core logging functions (cross-platform)
void InitializeHCTraceToVerboseLog();
void FlushDeviceLogFile();
bool TryGetCurrentLogFilePath(std::string& pathOut);
std::string ResolveLogFilePathForDevice(const std::string& deviceName);

void LogToWindow(const std::string& text);
void LogToWindowFormat(const char* format, ...);
void LogToWindowVerbose(const std::string& text);
void LogToWindowFormatVerbose(bool verboseOnly, const char* format, ...);
void EnableFileLoggingForDevice(const std::string& deviceName);
void CloseLogFile();

// Platform-specific UI functions
// These must be implemented per-platform
void DeviceLoggingInitializePlatform(void* platformContext);
void DeviceLoggingPaintPlatform(void* platformContext);
void UpdateWindowTitleWithDeviceName(const std::string& deviceName);
void DeviceLoggingHandleDestroy();

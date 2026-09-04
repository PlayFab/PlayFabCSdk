// Copyright (C) Microsoft Corporation. All rights reserved.
#include "pch.h"
#include "TestHarness/DeviceFileSystem.h"

std::string g_deviceFileSystemMountedPath;

DeviceFileSystemMountGuard DeviceFileSystemMount(const std::string& saveFolder)
{
    g_deviceFileSystemMountedPath = saveFolder;
    return DeviceFileSystemMountGuard();
}

void DeviceFileSystemUnmount()
{
    // no-op on Xbox
}

void DeviceFileSystemInitialize(const std::string& saveFolder)
{
    g_deviceFileSystemMountedPath = saveFolder;
}

std::string DeviceFileSystemGetMountedPath()
{
    return g_deviceFileSystemMountedPath;
}

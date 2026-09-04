// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include <string>

// Cross-platform file system interface
// Platform-specific implementations are in Platform/<Platform>/DeviceFileSystem_<Platform>.cpp

// Initialize the device file system with the specified save folder. This must be called before mounting the file system.
void DeviceFileSystemInitialize(const std::string& saveFolder);

// Get the mounted file system.
std::string DeviceFileSystemGetMountedPath();

// Unmount the file system for the device.
void DeviceFileSystemUnmount();

// Forward declare DeviceFileSystemMount so DeviceFileSystemMountGuard can friend it.
class DeviceFileSystemMountGuard;
DeviceFileSystemMountGuard DeviceFileSystemMount(const std::string& saveFolder);

// RAII guard returned by DeviceFileSystemMount. Calls DeviceFileSystemUnmount
// when it goes out of scope. Call detach() to transfer ownership and prevent
// the automatic unmount.
class DeviceFileSystemMountGuard
{
public:
    DeviceFileSystemMountGuard(const DeviceFileSystemMountGuard&) = delete;
    DeviceFileSystemMountGuard& operator=(const DeviceFileSystemMountGuard&) = delete;

    DeviceFileSystemMountGuard(DeviceFileSystemMountGuard&& other) noexcept
        : m_active{ other.m_active }
    {
        other.m_active = false;
    }

    DeviceFileSystemMountGuard& operator=(DeviceFileSystemMountGuard&& other) noexcept
    {
        if (this != &other)
        {
            if (m_active)
            {
                DeviceFileSystemUnmount();
            }
            m_active = other.m_active;
            other.m_active = false;
        }
        return *this;
    }

    ~DeviceFileSystemMountGuard()
    {
        if (m_active)
        {
            DeviceFileSystemUnmount();
        }
    }

    // Disables the automatic unmount. After calling detach(), the caller is
    // responsible for calling DeviceFileSystemUnmount() manually.
    void detach() noexcept { m_active = false; }

private:
    friend DeviceFileSystemMountGuard DeviceFileSystemMount(const std::string& saveFolder);
    DeviceFileSystemMountGuard() noexcept : m_active{ true } {}

    bool m_active;
};

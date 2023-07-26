// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

namespace PlayFab
{
namespace UnitTests
{

// RAII Win32 event wrapper
class Event
{
public:
    Event() noexcept;
    ~Event() noexcept;
    Event(const Event& other) = delete;
    Event& operator=(Event other) = delete;

    void Set() noexcept;
    bool Wait(DWORD timeoutMs = INFINITE) const noexcept;

private:
    HANDLE m_handle{ nullptr };
};

}
}

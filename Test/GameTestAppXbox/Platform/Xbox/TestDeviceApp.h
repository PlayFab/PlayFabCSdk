// Copyright (C) Microsoft Corporation. All rights reserved.
// TestDeviceApp.h - ATG-based test device application class
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

struct DeviceGameSaveState;

class TestDeviceApp final : public DX::IDeviceNotify
{
public:
    TestDeviceApp() noexcept(false);
    ~TestDeviceApp();

    TestDeviceApp(TestDeviceApp&&) = default;
    TestDeviceApp& operator=(TestDeviceApp&&) = default;

    TestDeviceApp(TestDeviceApp const&) = delete;
    TestDeviceApp& operator=(TestDeviceApp const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height, LPCWSTR cmdLine = nullptr);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering
    std::unique_ptr<DirectX::GraphicsMemory>    m_graphicsMemory;
    std::unique_ptr<DX::TextConsole>            m_console;

    std::unique_ptr<DirectX::DescriptorPile>    m_resourceDescriptors;

    enum Descriptors
    {
        Font,
        Reserve,
        Count = 8,
    };

    // Input
    std::unique_ptr<DirectX::GamePad>           m_gamePad;
    std::unique_ptr<DirectX::Keyboard>          m_keyboard;

    DirectX::GamePad::ButtonStateTracker        m_gamePadButtons;
    DirectX::Keyboard::KeyboardStateTracker     m_keyboardButtons;

    // Timing
    uint64_t                                    m_frame;
    DX::StepTimer                               m_timer;

    // Test device state
    DeviceGameSaveState*                        m_state{ nullptr };
    bool                                        m_initialized{ false };
};

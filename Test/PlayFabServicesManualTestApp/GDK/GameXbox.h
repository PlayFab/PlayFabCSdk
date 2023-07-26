//
// Game.h
//

#pragma once

#if !defined(_GAMING_DESKTOP)
#include "DeviceResourcesXbox.h"
#include "StepTimer.h"
#include "TestFramework/TestRunner.h"

void SetClearColor(float r, float g, float b);

// A basic game implementation that creates a D3D12 device and
// provides a render loop.
class Game
{
public:

    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    void SetClearColor(float r, float g, float b);

    // Initialization and management
    void Initialize(HWND window);

    // Basic render loop
    void Tick();

    // Messages
    void OnSuspending();
    void OnResuming();
    void OnConstrained() {}
    void OnUnConstrained() {}

    // Properties
    bool RequestHDRMode() const noexcept { return m_deviceResources ? (m_deviceResources->GetDeviceOptions() & DX::DeviceResources::c_EnableHDR) != 0 : false; }

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;

    // Rendering loop timer.
    uint64_t                                    m_frame;
    DX::StepTimer                               m_timer;

    // PlayFab TestRunner
    PlayFab::Test::TestRunner                   m_testRunner;
    FLOAT m_clearColor[4];
};
#endif // !defined(_GAMING_DESKTOP)

//--------------------------------------------------------------------------------------
// PlayFabGameSaveSample-XboxConsole.h
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once

#include "DeviceResources.h"
#include "LiveResources.h"
#include "LiveInfoHUD.h"
#include "StepTimer.h"
#include "SampleGUI.h"
#include "TextConsole.h"

// PlayFab Game Save
#include <playfab/core/PFLocalUser_Xbox.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFCore.h>
#include <playfab/services/PFServices.h>
#include <playfab/gamesave/PFGameSaveFiles.h>

#include <mutex>
#include <deque>
#include <random>

// A basic sample implementation that creates a D3D12 device and
// provides a render loop.
class Sample final : public DX::IDeviceNotify
{
public:
    Sample() noexcept(false);
    ~Sample();

    Sample(Sample &&) = default;
    Sample &operator=(Sample &&) = default;

    Sample(Sample const &) = delete;
    Sample &operator=(Sample const &) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic render loop
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
    void GetDefaultSize(int &width, int &height) const noexcept;

#ifdef _GAMING_DESKTOP
    // Desktop-only: direct key handling invoked from WndProc
    void OnKeyDown(WPARAM vk);
#endif

private:
    void Update(DX::StepTimer const &timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    void SetupUI();

    // Logging helpers
    void PostLog(const wchar_t *text);
    void PostLogf(const wchar_t *fmt, ...);

    // Game Save helpers
    HRESULT InitializePlayFab();
    HRESULT InitializeGameSaves();
    HRESULT SignInAndCreateLocalUser();
    HRESULT AddUserToGameSaves();
    void DoSave();
    void DoLoad();
    void DoDelete();
    HRESULT DoUpload(bool keepDeviceActive);
    void DoSetSaveDescription();

    // Device resources.
    std::unique_ptr<DX::DeviceResources> m_deviceResources;

    // Rendering loop timer.
    uint64_t m_frame;
    DX::StepTimer m_timer;

    // Input devices.
    std::unique_ptr<DirectX::GamePad> m_gamePad;
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;

    DirectX::GamePad::ButtonStateTracker m_gamePadButtons;
    DirectX::Keyboard::KeyboardStateTracker m_keyboardButtons;

    // DirectXTK objects.
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
    std::unique_ptr<DirectX::DescriptorPile> m_resourceDescriptors;

    // Xbox Live objects.
    std::shared_ptr<ATG::LiveResources> m_liveResources;

    std::unique_ptr<ATG::LiveInfoHUD> m_liveInfoHUD;

    XTaskQueueHandle m_mainAsyncQueue;

    // UI Objects
    std::unique_ptr<ATG::UIManager> m_ui;
    std::unique_ptr<DX::TextConsoleImage> m_log;

    // PF state
    XTaskQueueHandle m_queue{nullptr};
    PFServiceConfigHandle m_serviceConfig{nullptr};
    PFLocalUserHandle m_localUser{nullptr};
    XUserHandle m_xuser{nullptr};
    bool m_pfInitialized{false};
    bool m_gsInitialized{false};
    bool m_userAddedToGS{false};
    bool m_signInInProgress{false};
    bool m_localUserCreateInProgress{false};
    bool m_addUserToGSInProgress{false};
    std::string m_saveRoot;
    int64_t m_remainingQuota{0};

    // Log synchronization
    std::mutex m_logMutex;
    std::deque<std::wstring> m_logPending;

    // RNG for generating random save data
    std::mt19937 m_rng;

    enum Descriptors
    {
        Font,
        ConsoleFont,
        Background,
        ConsoleBackground,
        Reserve,
        Count = 32,
    };
};

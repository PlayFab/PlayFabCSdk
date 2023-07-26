//
// Game.cpp
//

#include "TestAppPch.h"

#if !defined(_GAMING_DESKTOP)
#include "GameXbox.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;
Game* g_game = nullptr;

void SetClearColor(float r, float g, float b)
{
    g_game->SetClearColor(r, g, b);
}

void Game::SetClearColor(float r, float g, float b)
{
    m_clearColor[0] = r;
    m_clearColor[1] = g;
    m_clearColor[2] = b;
    m_clearColor[3] = 1.0f;
}

Game::Game() noexcept(false) :
    m_frame(0)
{
    g_game = this;
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    SetClearColor(Colors::CornflowerBlue[0], Colors::CornflowerBlue[1], Colors::CornflowerBlue[2]);
    // Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_Enable4K_UHD to opt-in to 4k swapchains when supported.
    //   Add DX::DeviceResources::c_EnableQHD to opt-in to 1440p swapchains when supported.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
}

Game::~Game()
{
    m_testRunner.Cleanup();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window)
{
    m_deviceResources->SetWindow(window);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_testRunner.Initialize();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Frame %llu", m_frame);

    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();

    PIXEndEvent();
    m_frame++;
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    UNREFERENCED_PARAMETER(timer);
    PIXScopedEvent(PIX_COLOR_DEFAULT, L"Update");

    // TODO: Add your game logic here.
    if (m_testRunner.Update())
    {
        ExitGame();
    }
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // Add your rendering code here.
    commandList;

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();

    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto const rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto const dsvDescriptor = m_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, m_clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto const viewport = m_deviceResources->GetScreenViewport();
    auto const scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Occurs when the game is being suspended.
void Game::OnSuspending()
{
    m_deviceResources->Suspend();
}

// Occurs when the game is resuming.
void Game::OnResuming()
{
    m_deviceResources->Resume();
    m_timer.ResetElapsedTime();
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    // Initialize device dependent objects here (independent of window size).
    device;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // Initialize windows-size dependent objects here.
}
#pragma endregion
#endif // !defined(_GAMING_DESKTOP)
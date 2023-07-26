// Copyright (C) Microsoft Corporation. All rights reserved.

#pragma once

#include "../GDK/Keyboard.h"

#include <XGameRuntimeFeature.h>
#include <XNetworking.h>
#include <XGameRuntimeInit.h>
#include <xsapi-c/services_c.h>
#include "HttpMock.h"

namespace PlayFab
{
namespace Test
{
   
struct TestTitleData
{
    std::string titleId;
    std::string secretKey;
    std::string connectionString;
};

/// <summary>
/// Class the controls execution and flow of a Suite of tests
/// </summary>
class TestRunner
{
public:
    TestRunner() = default;

    void Initialize();
    bool Update(); // returns true if testing is complete

    void InitXBL(bool isControlPressed);
    void SigninWithXUser(bool isControlPressed);
    void InitPlayFab(bool isControlPressed);
    void LoginToPFWithXUser(bool isControlPressed);
    void LoginToPFWithXUserAsync(bool isControlPressed);
    void CallPFGetTime();
    void CallPFGetTimeAsync();
    void CallPFGetFiles();
    void CallPFGetFilesAsync();
    void CallPFEventPipelineCreate(bool isControlPressed, bool createTelemetryPipeline, bool useKey);

#if HC_PLATFORM != HC_PLATFORM_GDK
    void LoginToPFWithXToken(bool isControlPressed);
#endif

    bool Cleanup(); // returns true if all tests passed

private:
    bool LoadTitleData(TestTitleData& titleData);

    // Input devices.
    std::unique_ptr<DirectX::Keyboard>          m_keyboard;
    DirectX::Keyboard::KeyboardStateTracker     m_keyboardButtons;
    DirectX::Keyboard::KeyboardStateTracker     m_keyboardButtonsLast;

    bool m_xblInit{ false };
    bool m_pfInit{ false };
    bool m_bCleanup{ false };
    bool WasPressed(DirectX::Keyboard::Keys key);
    float m_blueColor{ 0.0f };
    float m_greenColor{ 0.0f };
    bool m_offlineTesting{ false };

    XUserHandle m_userHandle1{ nullptr };
    PFServiceConfigHandle m_serviceConfigHandle{ nullptr };
    PFEntityHandle m_entityHandle{ nullptr };
    std::vector<char> m_entityHandleBuffer;
    PFEntityKey const* m_pEntityKey{};
    std::shared_ptr<HttpMock> m_mock;
    PFRegistrationToken m_registrationTokenExpired{ 0 };
    PFRegistrationToken m_registrationTokenRefreshed{ 0 };
    PFEventPipelineHandle m_eventPipelineHandle{ nullptr };

    HRESULT AddUser(XUserAddOptions options);
    void SetUserHandle(XUserHandle user);

};

} // namespace Test
} // namespace PlayFab

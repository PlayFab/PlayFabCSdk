#pragma once

#include <playfab/services/cpp/PlatformSpecificTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_ANDROID
class ClientAndroidDevicePushNotificationRegistrationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper<Allocator>;

    ClientAndroidDevicePushNotificationRegistrationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientRefreshPSNAuthTokenOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper<Allocator>;

    ClientRefreshPSNAuthTokenOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_IOS
class ClientRegisterForIOSPushNotificationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper<Allocator>;

    ClientRegisterForIOSPushNotificationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerAwardSteamAchievementOperation : public XAsyncOperation<Wrappers::PFPlatformSpecificAwardSteamAchievementResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlatformSpecificAwardSteamAchievementRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlatformSpecificAwardSteamAchievementResultWrapper<Allocator>;

    ServerAwardSteamAchievementOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlatformSpecificAwardSteamAchievementResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

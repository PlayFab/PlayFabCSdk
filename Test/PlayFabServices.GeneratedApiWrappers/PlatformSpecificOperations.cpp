#include "TestAppPch.h"
#include "PlatformSpecificOperations.h"
#if HC_PLATFORM != HC_PLATFORM_GDK
#include <playfab/services/PFPlatformSpecific.h>
#endif

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_ANDROID

ClientAndroidDevicePushNotificationRegistrationOperation::ClientAndroidDevicePushNotificationRegistrationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientAndroidDevicePushNotificationRegistrationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientAndroidDevicePushNotificationRegistrationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientAndroidDevicePushNotificationRegistrationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

ClientRefreshPSNAuthTokenOperation::ClientRefreshPSNAuthTokenOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientRefreshPSNAuthTokenOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientRefreshPSNAuthTokenOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientRefreshPSNAuthTokenOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlatformSpecificClientRefreshPSNAuthTokenAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_IOS

ClientRegisterForIOSPushNotificationOperation::ClientRegisterForIOSPushNotificationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientRegisterForIOSPushNotificationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientRegisterForIOSPushNotificationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientRegisterForIOSPushNotificationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlatformSpecificClientRegisterForIOSPushNotificationAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

ServerAwardSteamAchievementOperation::ServerAwardSteamAchievementOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerAwardSteamAchievementOperation::ResultType> ServerAwardSteamAchievementOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerAwardSteamAchievementOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerAwardSteamAchievementOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlatformSpecificServerAwardSteamAchievementAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerAwardSteamAchievementOperation::ResultType> ServerAwardSteamAchievementOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlatformSpecificServerAwardSteamAchievementGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlatformSpecificAwardSteamAchievementResult* result;
    RETURN_IF_FAILED(PFPlatformSpecificServerAwardSteamAchievementGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

}
}

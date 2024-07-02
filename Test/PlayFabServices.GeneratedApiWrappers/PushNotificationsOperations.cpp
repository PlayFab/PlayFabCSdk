#include "TestAppPch.h"
#include "PushNotificationsOperations.h"
#if HC_PLATFORM != HC_PLATFORM_GDK
#include <playfab/services/PFPushNotifications.h>
#endif

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

ServerSendPushNotificationOperation::ServerSendPushNotificationOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSendPushNotificationOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSendPushNotificationOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSendPushNotificationOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPushNotificationsServerSendPushNotificationAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

ServerSendPushNotificationFromTemplateOperation::ServerSendPushNotificationFromTemplateOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSendPushNotificationFromTemplateOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSendPushNotificationFromTemplateOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSendPushNotificationFromTemplateOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPushNotificationsServerSendPushNotificationFromTemplateAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

}
}

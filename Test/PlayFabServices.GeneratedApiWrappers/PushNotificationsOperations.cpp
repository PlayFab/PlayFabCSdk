#include "TestAppPch.h"
#include "PushNotificationsOperations.h"
#include <playfab/services/PFPushNotifications.h>

namespace PlayFab
{
namespace Test
{

#if 0

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

#if 0

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
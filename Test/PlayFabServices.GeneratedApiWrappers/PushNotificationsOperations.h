#pragma once

#if HC_PLATFORM != HC_PLATFORM_GDK
#include <playfab/services/cpp/PushNotificationsTypeWrappers.h>
#endif
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerSendPushNotificationOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFPushNotificationsSendPushNotificationRequestWrapper<Allocator>;

    ServerSendPushNotificationOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerSendPushNotificationFromTemplateOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper<Allocator>;

    ServerSendPushNotificationFromTemplateOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

#pragma once

#include <playfab/services/cpp/PushNotificationsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if 0
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

#if 0
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

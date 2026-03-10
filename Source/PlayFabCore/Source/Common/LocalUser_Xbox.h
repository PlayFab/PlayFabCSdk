#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK // Temporary while GDK is still a separate platform
#include "Entity.h"
#include "Authentication/LoginHandler.h"
#include "PlayFabXUser.h"
#include <playfab/core/PFLocalUser.h>
#include "LocalUser.h"

namespace PlayFab
{

class XboxLocalUser : public LocalUser
{
public:
    static Result<SharedPtr<XboxLocalUser>> Make(
        SharedPtr<ServiceConfig> serviceConfig,
        XUserHandle user,
        void* clientContext
    );

    // Disable copy and assignemt to prevent misuse
    XboxLocalUser(XboxLocalUser const&) = delete;
    XboxLocalUser& operator=(XboxLocalUser&) = delete;
    ~XboxLocalUser() = default;

    String LocalId() const override;
    XUserHandle GetXUserHandle() const noexcept { return m_user.Handle(); }
    SharedPtr<Authentication::ILoginHandler> LoginHandler() const noexcept override;

private:
    XboxLocalUser(
        SharedPtr<ServiceConfig> serviceConfig,
        void* clientContext,
        XUser user,
        String localId
    );

    friend class XboxLocalUserLoginHandler;

    XUser m_user;
    String m_localId;
    SharedPtr<XboxLocalUserLoginHandler> m_loginHandler;
};

Result<SharedPtr<LocalUser>> MakeXboxUser(
    _In_ SharedPtr<ServiceConfig> serviceConfig,
    _In_ XUserHandle user,
    _In_opt_ void* clientContext
);

}
#endif // HC_PLATFORM_GDK
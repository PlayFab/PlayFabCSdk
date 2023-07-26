#pragma once

#include <playfab/core/PFEntity.h>
#include "EntityToken.h"

namespace PlayFab
{

class TokenRefreshedHandler
{
public:
    TokenRefreshedHandler() noexcept;
    TokenRefreshedHandler(TokenRefreshedHandler const&) noexcept = default;
    TokenRefreshedHandler(TokenRefreshedHandler&&) noexcept = default;
    TokenRefreshedHandler& operator=(TokenRefreshedHandler const&) noexcept = default;
    TokenRefreshedHandler& operator=(TokenRefreshedHandler&&) noexcept = default;
    ~TokenRefreshedHandler() = default;

    HRESULT RegisterClientHandler(
        RunContext&& rc,
        void* context,
        PFEntityTokenRefreshedEventHandler* handler,
        PFRegistrationToken* token
    ) noexcept;

    void UnregisterClientHandler(PFRegistrationToken token) noexcept;

    void Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept;

private:
    class SharedState;

    SharedPtr<SharedState> m_state;
};

}

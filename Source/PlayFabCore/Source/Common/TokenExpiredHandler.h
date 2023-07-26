#pragma once

#include <playfab/core/PFEntity.h>

namespace PlayFab
{

class TokenExpiredHandler
{
public:
    TokenExpiredHandler() noexcept;
    TokenExpiredHandler(TokenExpiredHandler const&) noexcept = default;
    TokenExpiredHandler(TokenExpiredHandler&&) noexcept = default;
    TokenExpiredHandler& operator=(TokenExpiredHandler const&) noexcept = default;
    TokenExpiredHandler& operator=(TokenExpiredHandler&&) noexcept = default;
    ~TokenExpiredHandler() = default;

    HRESULT RegisterClientHandler(
        RunContext&& rc,
        void* context,
        PFEntityTokenExpiredEventHandler* handler,
        PFRegistrationToken* token
    ) noexcept;

    void UnregisterClientHandler(PFRegistrationToken token) noexcept;

    void Invoke(EntityKey entityKey) const noexcept;

private:
    class SharedState;

    SharedPtr<SharedState> m_state;
};

}

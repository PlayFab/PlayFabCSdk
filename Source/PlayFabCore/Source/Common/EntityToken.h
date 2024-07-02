#pragma once

#include <playfab/core/PFEntity.h>
#include "Generated/AuthenticationTypes.h"

namespace PlayFab
{

// EntityToken used to Authenticate with PlayFab endpoints
// Continuing to expose token expiration time even though we don't require titles to directly act on it
class EntityToken : public PFEntityToken, public ClientOutputModel<PFEntityToken>
{
public:
    EntityToken(const Authentication::EntityTokenResponse& tokenResponse);
    EntityToken(const Authentication::GetEntityTokenResponse& tokenResponse);
    EntityToken(const EntityToken& src);
    EntityToken(EntityToken&& src) noexcept;
    EntityToken& operator=(const EntityToken& src);
    EntityToken& operator=(EntityToken&& src) noexcept;
    ~EntityToken() = default;

    // ClientOutputModel
    size_t RequiredBufferSize() const;
    Result<PFEntityToken const*> Copy(ModelBuffer& buffer) const;

private:
    String m_token;
    std::optional<time_t> m_expiration;
};

}

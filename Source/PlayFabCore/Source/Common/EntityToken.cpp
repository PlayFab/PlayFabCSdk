#include "stdafx.h"
#include "EntityToken.h"

namespace PlayFab
{

EntityToken::EntityToken(const Authentication::EntityTokenResponse& tokenResponse) :
    PFEntityToken{},
    m_token{ tokenResponse.Model().entityToken},
    m_expiration{ tokenResponse.Model().tokenExpiration ? std::optional<time_t>{ *tokenResponse.Model().tokenExpiration } : std::nullopt }
{
    token = m_token.empty() ? nullptr : m_token.data();
    expiration = m_expiration ? m_expiration.operator->() : nullptr;
}

EntityToken::EntityToken(const Authentication::GetEntityTokenResponse& tokenResponse) :
    PFEntityToken{},
    m_token{ tokenResponse.entityToken },
    m_expiration{ tokenResponse.tokenExpiration }
{
    token = m_token.empty() ? nullptr : m_token.data();
    expiration = m_expiration ? m_expiration.operator->() : nullptr;
}

EntityToken::EntityToken(const EntityToken& src) :
    m_token{ src.m_token },
    m_expiration{ src.m_expiration }
{
    token = m_token.empty() ? nullptr : m_token.data();
    expiration = m_expiration ? m_expiration.operator->() : nullptr;
}

EntityToken::EntityToken(EntityToken&& src) noexcept :
    m_token{ std::move(src.m_token) },
    m_expiration{ src.m_expiration }
{
    token = m_token.empty() ? nullptr : m_token.data();
    expiration = m_expiration ? m_expiration.operator->() : nullptr;
}

EntityToken& EntityToken::operator=(const EntityToken& src)
{
    m_token = src.m_token;
    token = m_token.empty() ? nullptr : m_token.data();

    m_expiration = src.m_expiration;
    expiration = m_expiration ? m_expiration.operator->() : nullptr;

    return *this;
}

EntityToken& EntityToken::operator=(EntityToken&& src) noexcept
{
    m_token = std::move(src.m_token);
    token = m_token.empty() ? nullptr : m_token.data();

    m_expiration = src.m_expiration;
    expiration = m_expiration ? m_expiration.operator->() : nullptr;

    return *this;
}

size_t EntityToken::RequiredBufferSize() const
{
    size_t requiredSize{ alignof(PFEntityToken) + sizeof(PFEntityToken) };
    if (token)
    {
        requiredSize += (std::strlen(token) + 1);
    }
    if (expiration)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    return requiredSize;
}

Result<PFEntityToken const*> EntityToken::Copy(ModelBuffer& buffer) const
{
    // Alloc
    auto allocResult = buffer.Alloc<PFEntityToken>(1);
    RETURN_IF_FAILED(allocResult.hr);
    
    auto outputPtr = allocResult.ExtractPayload();

    // Copy token
    auto tokenCopyResult = buffer.CopyTo(this->token);
    RETURN_IF_FAILED(tokenCopyResult.hr);
    outputPtr->token = tokenCopyResult.ExtractPayload();

    // Copy expiration
    auto expirationCopyResult = buffer.CopyTo(this->expiration);
    RETURN_IF_FAILED(expirationCopyResult.hr);
    outputPtr->expiration = expirationCopyResult.ExtractPayload();

    return std::move(outputPtr);
}

}

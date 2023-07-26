#pragma once

#include <playfab/core/PFEntity.h>
#include "ServiceConfig.h"
#include "EntityToken.h"
#include "TokenExpiredHandler.h"
#include "TokenRefreshedHandler.h"
#include "Authentication/LoginContext.h"

namespace PlayFab
{

class TokenRefreshWorker;

// An entity authenticated with PlayFab. An entity has a type, an ID, and an EntityToken. For more detail on PlayFab entities see the service
// documentation here: https://docs.microsoft.com/en-us/gaming/playfab/features/data/entities/quickstart.
class Entity : public std::enable_shared_from_this<Entity>
{
public:
    // Create a new Entity from an EntityTokenResponse and ClientSessionTicket returned from a Login call
    static Result<SharedPtr<Entity>> Make(
        Authentication::EntityTokenResponse&& entityTokenResponse,
        SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
        RunContext&& tokenRefreshContext,
        SharedPtr<Authentication::LoginContext> loginContext,
        TokenExpiredHandler tokenExpiredHandler,
        TokenRefreshedHandler tokenRefreshedHandler,
        std::optional<String> secretKey = std::nullopt
    ) noexcept;

    // Create a new Entity from a GetEntityTokenResponse
    static Result<SharedPtr<Entity>> Make(
        Authentication::GetEntityTokenResponse&& entityTokenResponse,
        SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
        RunContext&& tokenRefreshContext,
        TokenExpiredHandler tokenExpiredHandler,
        TokenRefreshedHandler tokenRefreshedHandler,
        std::optional<String> secretKey = std::nullopt
    ) noexcept;

    Entity(const Entity&) = delete;
    Entity(Entity&&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = delete;
    ~Entity() noexcept = default;

public:
    // ServiceConfig this Entity belongs to
    SharedPtr<ServiceConfig const> ServiceConfig() const;

    // EntityKey for this Entity
    EntityKey const& EntityKey() const;

    // Get the cached EntityToken
    Result<EntityToken> GetEntityToken() const;

    // Get the API SecretKey if one is available
    Result<String> GetSecretKey() const;

    // Update the cached LoginContext in the TokenRefreshWorker
    HRESULT OnLoginContextUpdated(SharedPtr<Authentication::LoginContext> loginContext);

    // Handlers to update cached tokens when they are refreshed
    HRESULT OnEntityTokenRefreshed(Authentication::EntityTokenResponse const& newToken);
    HRESULT OnEntityTokenRefreshed(Authentication::GetEntityTokenResponse const& newToken);

private:
    Entity(
        SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
        PlayFab::EntityKey&& entityKey,
        PlayFab::EntityToken&& entityToken,
        std::optional<String> secretKey,
        TokenRefreshedHandler tokenRefreshedHandler
    ) noexcept;

    mutable std::mutex m_mutex;
    PlayFab::EntityKey const m_key;
    PlayFab::EntityToken m_entityToken;
    std::optional<String> const m_secretKey;
    SharedPtr<PlayFab::ServiceConfig const> m_serviceConfig;
    SharedPtr<TokenRefreshWorker> m_tokenRefreshWorker;
    TokenRefreshedHandler const m_tokenRefreshedHandler;
};

}

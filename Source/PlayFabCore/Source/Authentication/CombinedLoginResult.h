#pragma once

#include "Generated/AuthenticationTypes.h"
#include "Entity.h"
#include "PFCoreGlobalState.h"

namespace PlayFab
{
namespace Authentication
{

// Struct to hold public result types. Although the entityHandle and loginResult are returned via separate parameters
// from the public API, XAsync only allows for a single result type per call
struct PFCombinedLoginResult
{
    PFAuthenticationLoginResult const* loginResult;
    PFEntityHandle entityHandle;
};

// Internal combined LoginResult. Contains both the Entity object and the (trimmed) LoginResult model.
// Holds reference to PFCoreGlobalState to create Entity object and handles to it.
class CombinedLoginResult : public ClientOutputModel<PFCombinedLoginResult>
{
public:
    static Result<CombinedLoginResult> FromJson(
        const JsonValue& loginResponse,
        SharedPtr<PFCoreGlobalState> PFCoreGlobalState,
        SharedPtr<ServiceConfig const> serviceConfig,
        SharedPtr<LoginContext> loginContext
    ) noexcept;

    CombinedLoginResult(const CombinedLoginResult&) = default;
    CombinedLoginResult(CombinedLoginResult&&) = default;
    CombinedLoginResult& operator=(const CombinedLoginResult&) = delete;
    virtual ~CombinedLoginResult() = default;

    LoginResult loginResult;
    SharedPtr<Entity> entity;

public: 
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    // Copy into a buffer in order to return to client. Note that this method creates a PFEntityHandle
    // that must later be closed by the client.
    Result<PFCombinedLoginResult const*> Copy(ModelBuffer& buffer) const override;

private:
    CombinedLoginResult(SharedPtr<PFCoreGlobalState> PFCoreGlobalState) noexcept;

    // Need global state to Copy, as that creates a PFEntityHandle for 'entity'
    SharedPtr<PFCoreGlobalState> const m_PFCoreGlobalState;
};

// Struct to hold public server result types. Although the entityToken and loginResult are returned via separate parameters
// from the public API, XAsync only allows for a single result type per call
struct PFServerCombinedLoginResult
{
    PFAuthenticationLoginResult const* loginResult;
    PFAuthenticationEntityTokenResponse const* entityTokenResponse;
};

// Internal combined ServerLoginResult. Contains both the EntityTokenResponse object and the (trimmed) LoginResult model.
class ServerCombinedLoginResult : public ClientOutputModel<PFServerCombinedLoginResult>
{
public:
    static Result<ServerCombinedLoginResult> FromJson(
        const JsonValue& loginResponse
    ) noexcept;
    
    ServerCombinedLoginResult() = default;
    ServerCombinedLoginResult(const ServerCombinedLoginResult&) = default;
    ServerCombinedLoginResult(ServerCombinedLoginResult&&) = default;
    ServerCombinedLoginResult& operator=(const ServerCombinedLoginResult&) = delete;
    virtual ~ServerCombinedLoginResult() = default;
    
    LoginResult loginResult;
    EntityTokenResponse entityTokenResponse;

public:
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    // Copy into a buffer in order to return to client. Note that this method creates a PFEntityHandle
    // that must later be closed by the client.
    Result<PFServerCombinedLoginResult const*> Copy(ModelBuffer& buffer) const override;
};

} // namespace Authentication
} // namespace PlayFab

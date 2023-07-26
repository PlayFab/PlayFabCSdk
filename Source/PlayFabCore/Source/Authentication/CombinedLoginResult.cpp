#include "stdafx.h"
#include "CombinedLoginResult.h"
#include "GlobalState.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{

CombinedLoginResult::CombinedLoginResult(SharedPtr<GlobalState> globalState) noexcept
    : m_globalState{ std::move(globalState) }
{
}

Result<CombinedLoginResult> CombinedLoginResult::FromJson(
    const JsonValue& loginResponse,
    SharedPtr<GlobalState> globalState,
    SharedPtr<ServiceConfig const> serviceConfig,
    SharedPtr<LoginContext> loginContext
) noexcept
{
    CombinedLoginResult result{ std::move(globalState) };

    RETURN_IF_FAILED(result.loginResult.FromJson(loginResponse));

    EntityTokenResponse entityToken;
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(loginResponse, "EntityToken", entityToken));

    auto makeEntityResult = Entity::Make(
        std::move(entityToken),
        std::move(serviceConfig),
        result.m_globalState->RunContext().Derive(),
        std::move(loginContext),
        result.m_globalState->TokenExpiredHandler(),
        result.m_globalState->TokenRefreshedHandler()
    );

    RETURN_IF_FAILED(makeEntityResult.hr);
    result.entity = makeEntityResult.ExtractPayload();

    return result;
}

size_t CombinedLoginResult::RequiredBufferSize() const
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += loginResult.RequiredBufferSize();
    return requiredSize;
}

Result<PFCombinedLoginResult const*> CombinedLoginResult::Copy(ModelBuffer& buffer) const
{
    auto allocResult = buffer.Alloc<PFCombinedLoginResult>(1);
    RETURN_IF_FAILED(allocResult.hr);

    auto output = allocResult.ExtractPayload();

    // Create PFEntityHandle for entity
    RETURN_IF_FAILED(m_globalState->Entities().MakeHandle(entity, output->entityHandle));

    // Copy LoginResult
    auto propCopyResult = buffer.CopyTo<LoginResult>(&loginResult.Model());
    RETURN_IF_FAILED(propCopyResult.hr);
    output->loginResult = propCopyResult.ExtractPayload();

    return output;
}

Result<ServerCombinedLoginResult> ServerCombinedLoginResult::FromJson(
    const JsonValue& loginResponse
) noexcept
{
    ServerCombinedLoginResult result{};

    RETURN_IF_FAILED(result.loginResult.FromJson(loginResponse));
    
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(loginResponse, "EntityToken", result.entityTokenResponse));

    return result;
}

size_t ServerCombinedLoginResult::RequiredBufferSize() const
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += entityTokenResponse.RequiredBufferSize();
    requiredSize += loginResult.RequiredBufferSize();
    return requiredSize;
}

Result<PFServerCombinedLoginResult const*> ServerCombinedLoginResult::Copy(ModelBuffer& buffer) const
{
    auto allocResult = buffer.Alloc<PFServerCombinedLoginResult>(1);
    RETURN_IF_FAILED(allocResult.hr);

    auto output = allocResult.ExtractPayload();

    // Copy LoginResult
    auto propCopyResult = buffer.CopyTo<LoginResult>(&loginResult.Model());
    RETURN_IF_FAILED(propCopyResult.hr);
    output->loginResult = propCopyResult.ExtractPayload();

    // Copy EntityToken
    auto entityTokenCopyResult = buffer.CopyTo<EntityTokenResponse>(&entityTokenResponse.Model());
    RETURN_IF_FAILED(entityTokenCopyResult.hr);
    output->entityTokenResponse = entityTokenCopyResult.ExtractPayload();

    return output;
}

} // namespace Authentication
} // namespace PlayFab

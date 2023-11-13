#include "stdafx.h"
#include "CombinedLoginResult.h"
#include "PFCoreGlobalState.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{

CombinedLoginResult::CombinedLoginResult(SharedPtr<PFCoreGlobalState> PFCoreGlobalState) noexcept
    : m_PFCoreGlobalState{ std::move(PFCoreGlobalState) }
{
}

Result<CombinedLoginResult> CombinedLoginResult::FromJson(
    const JsonValue& loginResponse,
    SharedPtr<PFCoreGlobalState> PFCoreGlobalState,
    SharedPtr<ServiceConfig const> serviceConfig,
    SharedPtr<LoginContext> loginContext
) noexcept
{
    CombinedLoginResult result{ std::move(PFCoreGlobalState) };

    RETURN_IF_FAILED(result.loginResult.FromJson(loginResponse));

    EntityTokenResponse entityToken;
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(loginResponse, "EntityToken", entityToken));

    auto makeEntityResult = Entity::Make(
        std::move(entityToken),
        std::move(serviceConfig),
        result.m_PFCoreGlobalState->RunContext().Derive(),
        std::move(loginContext),
        result.m_PFCoreGlobalState->TokenExpiredHandler(),
        result.m_PFCoreGlobalState->TokenRefreshedHandler()
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
    RETURN_IF_FAILED(m_PFCoreGlobalState->Entities().MakeHandle(entity, output->entityHandle));

    // Copy LoginResult
    auto propCopyResult = buffer.CopyTo<LoginResult>(&loginResult.Model());
    RETURN_IF_FAILED(propCopyResult.hr);
    output->loginResult = propCopyResult.ExtractPayload();

    return std::move(output);
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

    return std::move(output);
}

} // namespace Authentication
} // namespace PlayFab

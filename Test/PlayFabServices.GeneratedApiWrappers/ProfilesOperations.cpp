#include "TestAppPch.h"
#include "ProfilesOperations.h"
#include <playfab/services/PFProfiles.h>

namespace PlayFab
{
namespace Test
{


GetProfileOperation::GetProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetProfileOperation::ResultType> GetProfileOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetProfileOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetProfileOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFProfilesGetProfileAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetProfileOperation::ResultType> GetProfileOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFProfilesGetProfileGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFProfilesGetEntityProfileResponse* result;
    RETURN_IF_FAILED(PFProfilesGetProfileGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetProfilesOperation::GetProfilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetProfilesOperation::ResultType> GetProfilesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetProfilesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetProfilesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFProfilesGetProfilesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetProfilesOperation::ResultType> GetProfilesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFProfilesGetProfilesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFProfilesGetEntityProfilesResponse* result;
    RETURN_IF_FAILED(PFProfilesGetProfilesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetTitlePlayersFromMasterPlayerAccountIdsOperation::GetTitlePlayersFromMasterPlayerAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetTitlePlayersFromMasterPlayerAccountIdsOperation::ResultType> GetTitlePlayersFromMasterPlayerAccountIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetTitlePlayersFromMasterPlayerAccountIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetTitlePlayersFromMasterPlayerAccountIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetTitlePlayersFromMasterPlayerAccountIdsOperation::ResultType> GetTitlePlayersFromMasterPlayerAccountIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse* result;
    RETURN_IF_FAILED(PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


SetProfileLanguageOperation::SetProfileLanguageOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<SetProfileLanguageOperation::ResultType> SetProfileLanguageOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SetProfileLanguageOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SetProfileLanguageOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFProfilesSetProfileLanguageAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<SetProfileLanguageOperation::ResultType> SetProfileLanguageOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFProfilesSetProfileLanguageGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFProfilesSetProfileLanguageResponse* result;
    RETURN_IF_FAILED(PFProfilesSetProfileLanguageGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


SetProfilePolicyOperation::SetProfilePolicyOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<SetProfilePolicyOperation::ResultType> SetProfilePolicyOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SetProfilePolicyOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SetProfilePolicyOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFProfilesSetProfilePolicyAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<SetProfilePolicyOperation::ResultType> SetProfilePolicyOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFProfilesSetProfilePolicyGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFProfilesSetEntityProfilePolicyResponse* result;
    RETURN_IF_FAILED(PFProfilesSetProfilePolicyGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

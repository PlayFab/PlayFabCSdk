#pragma once

#include <playfab/services/cpp/ProfilesTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class GetProfileOperation : public XAsyncOperation<Wrappers::PFProfilesGetEntityProfileResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFProfilesGetEntityProfileRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFProfilesGetEntityProfileResponseWrapper<Allocator>;

    GetProfileOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFProfilesGetEntityProfileResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetProfilesOperation : public XAsyncOperation<Wrappers::PFProfilesGetEntityProfilesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFProfilesGetEntityProfilesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFProfilesGetEntityProfilesResponseWrapper<Allocator>;

    GetProfilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFProfilesGetEntityProfilesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetTitlePlayersFromMasterPlayerAccountIdsOperation : public XAsyncOperation<Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponseWrapper<Allocator>;

    GetTitlePlayersFromMasterPlayerAccountIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class SetProfileLanguageOperation : public XAsyncOperation<Wrappers::PFProfilesSetProfileLanguageResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFProfilesSetProfileLanguageRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFProfilesSetProfileLanguageResponseWrapper<Allocator>;

    SetProfileLanguageOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFProfilesSetProfileLanguageResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class SetProfilePolicyOperation : public XAsyncOperation<Wrappers::PFProfilesSetEntityProfilePolicyResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFProfilesSetEntityProfilePolicyRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFProfilesSetEntityProfilePolicyResponseWrapper<Allocator>;

    SetProfilePolicyOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFProfilesSetEntityProfilePolicyResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

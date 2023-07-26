#pragma once

#include <playfab/services/cpp/CharacterTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if 0
class ClientGetAllUsersCharactersOperation : public XAsyncOperation<Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterListUsersCharactersRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>;

    ClientGetAllUsersCharactersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetCharacterDataOperation : public XAsyncOperation<Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>;

    ClientGetCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGetCharacterReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>;

    ClientGetCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterClientGetCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientGrantCharacterToUserOperation : public XAsyncOperation<Wrappers::PFCharacterClientGrantCharacterToUserResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterClientGrantCharacterToUserRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterClientGrantCharacterToUserResultWrapper<Allocator>;

    ClientGrantCharacterToUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterClientGrantCharacterToUserResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ClientUpdateCharacterDataOperation : public XAsyncOperation<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterClientUpdateCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>;

    ClientUpdateCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerDeleteCharacterFromUserOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFCharacterDeleteCharacterFromUserRequestWrapper<Allocator>;

    ServerDeleteCharacterFromUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetAllUsersCharactersOperation : public XAsyncOperation<Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterListUsersCharactersRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>;

    ServerGetAllUsersCharactersOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterListUsersCharactersResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetCharacterDataOperation : public XAsyncOperation<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>;

    ServerGetCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetCharacterInternalDataOperation : public XAsyncOperation<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>;

    ServerGetCharacterInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetCharacterReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterGetCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>;

    ServerGetCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterServerGetCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGrantCharacterToUserOperation : public XAsyncOperation<Wrappers::PFCharacterServerGrantCharacterToUserResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterServerGrantCharacterToUserRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterServerGrantCharacterToUserResultWrapper<Allocator>;

    ServerGrantCharacterToUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterServerGrantCharacterToUserResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUpdateCharacterDataOperation : public XAsyncOperation<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterServerUpdateCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>;

    ServerUpdateCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUpdateCharacterInternalDataOperation : public XAsyncOperation<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterServerUpdateCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>;

    ServerUpdateCharacterInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerUpdateCharacterReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCharacterServerUpdateCharacterDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>;

    ServerUpdateCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCharacterUpdateCharacterDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

#include "TestAppPch.h"
#include "CharacterOperations.h"
#include <playfab/services/PFCharacter.h>

namespace PlayFab
{
namespace Test
{

#if 0

ClientGetAllUsersCharactersOperation::ClientGetAllUsersCharactersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetAllUsersCharactersOperation::ResultType> ClientGetAllUsersCharactersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetAllUsersCharactersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetAllUsersCharactersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterClientGetAllUsersCharactersAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetAllUsersCharactersOperation::ResultType> ClientGetAllUsersCharactersOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterClientGetAllUsersCharactersGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterListUsersCharactersResult* result;
    RETURN_IF_FAILED(PFCharacterClientGetAllUsersCharactersGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ClientGetCharacterDataOperation::ClientGetCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetCharacterDataOperation::ResultType> ClientGetCharacterDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetCharacterDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetCharacterDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterClientGetCharacterDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetCharacterDataOperation::ResultType> ClientGetCharacterDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterClientGetCharacterDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterClientGetCharacterDataResult* result;
    RETURN_IF_FAILED(PFCharacterClientGetCharacterDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ClientGetCharacterReadOnlyDataOperation::ClientGetCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetCharacterReadOnlyDataOperation::ResultType> ClientGetCharacterReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetCharacterReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetCharacterReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterClientGetCharacterReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetCharacterReadOnlyDataOperation::ResultType> ClientGetCharacterReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterClientGetCharacterReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterClientGetCharacterDataResult* result;
    RETURN_IF_FAILED(PFCharacterClientGetCharacterReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ClientGrantCharacterToUserOperation::ClientGrantCharacterToUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGrantCharacterToUserOperation::ResultType> ClientGrantCharacterToUserOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGrantCharacterToUserOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGrantCharacterToUserOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterClientGrantCharacterToUserAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGrantCharacterToUserOperation::ResultType> ClientGrantCharacterToUserOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterClientGrantCharacterToUserGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterClientGrantCharacterToUserResult* result;
    RETURN_IF_FAILED(PFCharacterClientGrantCharacterToUserGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ClientUpdateCharacterDataOperation::ClientUpdateCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientUpdateCharacterDataOperation::ResultType> ClientUpdateCharacterDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUpdateCharacterDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUpdateCharacterDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterClientUpdateCharacterDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientUpdateCharacterDataOperation::ResultType> ClientUpdateCharacterDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFCharacterUpdateCharacterDataResult result{};
    RETURN_IF_FAILED(PFCharacterClientUpdateCharacterDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if 0

ServerDeleteCharacterFromUserOperation::ServerDeleteCharacterFromUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerDeleteCharacterFromUserOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerDeleteCharacterFromUserOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerDeleteCharacterFromUserOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerDeleteCharacterFromUserAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

ServerGetAllUsersCharactersOperation::ServerGetAllUsersCharactersOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetAllUsersCharactersOperation::ResultType> ServerGetAllUsersCharactersOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetAllUsersCharactersOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetAllUsersCharactersOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerGetAllUsersCharactersAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetAllUsersCharactersOperation::ResultType> ServerGetAllUsersCharactersOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterServerGetAllUsersCharactersGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterListUsersCharactersResult* result;
    RETURN_IF_FAILED(PFCharacterServerGetAllUsersCharactersGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetCharacterDataOperation::ServerGetCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetCharacterDataOperation::ResultType> ServerGetCharacterDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetCharacterDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetCharacterDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerGetCharacterDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetCharacterDataOperation::ResultType> ServerGetCharacterDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterServerGetCharacterDataResult* result;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetCharacterInternalDataOperation::ServerGetCharacterInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetCharacterInternalDataOperation::ResultType> ServerGetCharacterInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetCharacterInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetCharacterInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerGetCharacterInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetCharacterInternalDataOperation::ResultType> ServerGetCharacterInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterInternalDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterServerGetCharacterDataResult* result;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterInternalDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetCharacterReadOnlyDataOperation::ServerGetCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetCharacterReadOnlyDataOperation::ResultType> ServerGetCharacterReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetCharacterReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetCharacterReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerGetCharacterReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetCharacterReadOnlyDataOperation::ResultType> ServerGetCharacterReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterServerGetCharacterDataResult* result;
    RETURN_IF_FAILED(PFCharacterServerGetCharacterReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGrantCharacterToUserOperation::ServerGrantCharacterToUserOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGrantCharacterToUserOperation::ResultType> ServerGrantCharacterToUserOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGrantCharacterToUserOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGrantCharacterToUserOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerGrantCharacterToUserAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGrantCharacterToUserOperation::ResultType> ServerGrantCharacterToUserOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCharacterServerGrantCharacterToUserGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCharacterServerGrantCharacterToUserResult* result;
    RETURN_IF_FAILED(PFCharacterServerGrantCharacterToUserGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerUpdateCharacterDataOperation::ServerUpdateCharacterDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateCharacterDataOperation::ResultType> ServerUpdateCharacterDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateCharacterDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateCharacterDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerUpdateCharacterDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateCharacterDataOperation::ResultType> ServerUpdateCharacterDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFCharacterUpdateCharacterDataResult result{};
    RETURN_IF_FAILED(PFCharacterServerUpdateCharacterDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if 0

ServerUpdateCharacterInternalDataOperation::ServerUpdateCharacterInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateCharacterInternalDataOperation::ResultType> ServerUpdateCharacterInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateCharacterInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateCharacterInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerUpdateCharacterInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateCharacterInternalDataOperation::ResultType> ServerUpdateCharacterInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFCharacterUpdateCharacterDataResult result{};
    RETURN_IF_FAILED(PFCharacterServerUpdateCharacterInternalDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if 0

ServerUpdateCharacterReadOnlyDataOperation::ServerUpdateCharacterReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateCharacterReadOnlyDataOperation::ResultType> ServerUpdateCharacterReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateCharacterReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateCharacterReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCharacterServerUpdateCharacterReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateCharacterReadOnlyDataOperation::ResultType> ServerUpdateCharacterReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFCharacterUpdateCharacterDataResult result{};
    RETURN_IF_FAILED(PFCharacterServerUpdateCharacterReadOnlyDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

}
}

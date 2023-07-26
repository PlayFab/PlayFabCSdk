#include "TestAppPch.h"
#include "DataOperations.h"
#include <playfab/services/PFData.h>

namespace PlayFab
{
namespace Test
{


AbortFileUploadsOperation::AbortFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<AbortFileUploadsOperation::ResultType> AbortFileUploadsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<AbortFileUploadsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT AbortFileUploadsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataAbortFileUploadsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<AbortFileUploadsOperation::ResultType> AbortFileUploadsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataAbortFileUploadsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataAbortFileUploadsResponse* result;
    RETURN_IF_FAILED(PFDataAbortFileUploadsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


DeleteFilesOperation::DeleteFilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<DeleteFilesOperation::ResultType> DeleteFilesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteFilesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteFilesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataDeleteFilesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<DeleteFilesOperation::ResultType> DeleteFilesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataDeleteFilesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataDeleteFilesResponse* result;
    RETURN_IF_FAILED(PFDataDeleteFilesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


FinalizeFileUploadsOperation::FinalizeFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<FinalizeFileUploadsOperation::ResultType> FinalizeFileUploadsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<FinalizeFileUploadsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT FinalizeFileUploadsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataFinalizeFileUploadsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<FinalizeFileUploadsOperation::ResultType> FinalizeFileUploadsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataFinalizeFileUploadsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataFinalizeFileUploadsResponse* result;
    RETURN_IF_FAILED(PFDataFinalizeFileUploadsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetFilesOperation::GetFilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetFilesOperation::ResultType> GetFilesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetFilesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetFilesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataGetFilesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetFilesOperation::ResultType> GetFilesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataGetFilesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataGetFilesResponse* result;
    RETURN_IF_FAILED(PFDataGetFilesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetObjectsOperation::GetObjectsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetObjectsOperation::ResultType> GetObjectsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetObjectsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetObjectsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataGetObjectsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetObjectsOperation::ResultType> GetObjectsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataGetObjectsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataGetObjectsResponse* result;
    RETURN_IF_FAILED(PFDataGetObjectsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


InitiateFileUploadsOperation::InitiateFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<InitiateFileUploadsOperation::ResultType> InitiateFileUploadsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<InitiateFileUploadsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT InitiateFileUploadsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataInitiateFileUploadsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<InitiateFileUploadsOperation::ResultType> InitiateFileUploadsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataInitiateFileUploadsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataInitiateFileUploadsResponse* result;
    RETURN_IF_FAILED(PFDataInitiateFileUploadsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


SetObjectsOperation::SetObjectsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<SetObjectsOperation::ResultType> SetObjectsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SetObjectsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SetObjectsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFDataSetObjectsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<SetObjectsOperation::ResultType> SetObjectsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFDataSetObjectsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFDataSetObjectsResponse* result;
    RETURN_IF_FAILED(PFDataSetObjectsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

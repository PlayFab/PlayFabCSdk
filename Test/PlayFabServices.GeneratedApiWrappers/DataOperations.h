#pragma once

#include <playfab/services/cpp/DataTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class AbortFileUploadsOperation : public XAsyncOperation<Wrappers::PFDataAbortFileUploadsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataAbortFileUploadsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataAbortFileUploadsResponseWrapper<Allocator>;

    AbortFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataAbortFileUploadsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteFilesOperation : public XAsyncOperation<Wrappers::PFDataDeleteFilesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataDeleteFilesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataDeleteFilesResponseWrapper<Allocator>;

    DeleteFilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataDeleteFilesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class FinalizeFileUploadsOperation : public XAsyncOperation<Wrappers::PFDataFinalizeFileUploadsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataFinalizeFileUploadsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataFinalizeFileUploadsResponseWrapper<Allocator>;

    FinalizeFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataFinalizeFileUploadsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetFilesOperation : public XAsyncOperation<Wrappers::PFDataGetFilesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataGetFilesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataGetFilesResponseWrapper<Allocator>;

    GetFilesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataGetFilesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetObjectsOperation : public XAsyncOperation<Wrappers::PFDataGetObjectsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataGetObjectsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataGetObjectsResponseWrapper<Allocator>;

    GetObjectsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataGetObjectsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class InitiateFileUploadsOperation : public XAsyncOperation<Wrappers::PFDataInitiateFileUploadsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataInitiateFileUploadsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataInitiateFileUploadsResponseWrapper<Allocator>;

    InitiateFileUploadsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataInitiateFileUploadsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class SetObjectsOperation : public XAsyncOperation<Wrappers::PFDataSetObjectsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFDataSetObjectsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFDataSetObjectsResponseWrapper<Allocator>;

    SetObjectsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFDataSetObjectsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

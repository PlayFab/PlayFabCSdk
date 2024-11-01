#include "stdafx.h"
#include <playfab/services/PFCloudScript.h>
#include "Generated/CloudScript.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::CloudScript;

PF_API PFCloudScriptClientExecuteCloudScriptAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptExecuteCloudScriptRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptClientExecuteCloudScriptAsync),
        std::bind(&CloudScriptAPI::ClientExecuteCloudScript, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFCloudScriptClientExecuteCloudScriptGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFCloudScriptClientExecuteCloudScriptGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCloudScriptExecuteCloudScriptResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFCloudScriptExecuteCloudScriptResult*>(buffer);

    return S_OK;
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFCloudScriptServerExecuteCloudScriptAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptExecuteCloudScriptServerRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptServerExecuteCloudScriptAsync),
        std::bind(&CloudScriptAPI::ServerExecuteCloudScript, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFCloudScriptServerExecuteCloudScriptGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFCloudScriptServerExecuteCloudScriptGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCloudScriptExecuteCloudScriptResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFCloudScriptExecuteCloudScriptResult*>(buffer);

    return S_OK;
}
#endif

PF_API PFCloudScriptExecuteEntityCloudScriptAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptExecuteEntityCloudScriptRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptExecuteEntityCloudScriptAsync),
        std::bind(&CloudScriptAPI::ExecuteEntityCloudScript, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFCloudScriptExecuteEntityCloudScriptGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFCloudScriptExecuteEntityCloudScriptGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCloudScriptExecuteCloudScriptResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFCloudScriptExecuteCloudScriptResult*>(buffer);

    return S_OK;
}

PF_API PFCloudScriptExecuteFunctionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptExecuteFunctionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptExecuteFunctionAsync),
        std::bind(&CloudScriptAPI::ExecuteFunction, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFCloudScriptExecuteFunctionGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFCloudScriptExecuteFunctionGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCloudScriptExecuteFunctionResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFCloudScriptExecuteFunctionResult*>(buffer);

    return S_OK;
}

#if 0
PF_API PFCloudScriptListEventHubFunctionsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptListFunctionsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptListEventHubFunctionsAsync),
        std::bind(&CloudScriptAPI::ListEventHubFunctions, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFCloudScriptListEventHubFunctionsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFCloudScriptListEventHubFunctionsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCloudScriptListEventHubFunctionsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFCloudScriptListEventHubFunctionsResult*>(buffer);

    return S_OK;
}
#endif

#if 0
PF_API PFCloudScriptRegisterEventHubFunctionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFCloudScriptRegisterEventHubFunctionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFCloudScriptRegisterEventHubFunctionAsync),
        std::bind(&CloudScriptAPI::RegisterEventHubFunction, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif


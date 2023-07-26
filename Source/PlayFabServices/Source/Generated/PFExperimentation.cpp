#include "stdafx.h"
#include <playfab/services/PFExperimentation.h>
#include "Generated/Experimentation.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::Experimentation;

PF_API PFExperimentationGetTreatmentAssignmentAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFExperimentationGetTreatmentAssignmentRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFExperimentationGetTreatmentAssignmentAsync),
        std::bind(&ExperimentationAPI::GetTreatmentAssignment, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFExperimentationGetTreatmentAssignmentGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFExperimentationGetTreatmentAssignmentGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFExperimentationGetTreatmentAssignmentResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFExperimentationGetTreatmentAssignmentResult*>(buffer);

    return S_OK;
}

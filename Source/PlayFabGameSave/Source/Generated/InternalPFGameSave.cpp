#include "stdafx.h"
#include "Generated/GameSave.h"
#include "ApiXAsyncProvider.h"
#include "GameSaveGlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::GameSave;
using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{


HRESULT PFGameSaveDeleteManifestAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveDeleteManifestRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveDeleteManifestAsync),
        std::bind(&GameSaveAPI::DeleteManifest, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveFinalizeManifestAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveFinalizeManifestRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveFinalizeManifestAsync),
        std::bind(&GameSaveAPI::FinalizeManifest, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveFinalizeManifestGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveFinalizeManifestGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveFinalizeManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveFinalizeManifestResponse*>(buffer);

    return S_OK;
}

HRESULT PFGameSaveGetConfigForTitleAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveGetConfigForTitleRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveGetConfigForTitleAsync),
        std::bind(&GameSaveAPI::GetConfigForTitle, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveGetConfigForTitleGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveGetConfigForTitleGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetConfigForTitleResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveGetConfigForTitleResponse*>(buffer);

    return S_OK;
}

HRESULT PFGameSaveGetManifestDownloadDetailsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveGetManifestDownloadDetailsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveGetManifestDownloadDetailsAsync),
        std::bind(&GameSaveAPI::GetManifestDownloadDetails, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveGetManifestDownloadDetailsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveGetManifestDownloadDetailsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetManifestDownloadDetailsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveGetManifestDownloadDetailsResponse*>(buffer);

    return S_OK;
}

#if 0
HRESULT PFGameSaveGetQuotaForPlayerAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveGetQuotaForPlayerRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveGetQuotaForPlayerAsync),
        std::bind(&GameSaveAPI::GetQuotaForPlayer, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveGetQuotaForPlayerGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveGetQuotaForPlayerGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetQuotaForPlayerResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveGetQuotaForPlayerResponse*>(buffer);

    return S_OK;
}
#endif

HRESULT PFGameSaveInitializeManifestAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveInitializeManifestRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveInitializeManifestAsync),
        std::bind(&GameSaveAPI::InitializeManifest, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveInitializeManifestGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveInitializeManifestGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveInitializeManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveInitializeManifestResponse*>(buffer);

    return S_OK;
}

HRESULT PFGameSaveInitiateUploadAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveInitiateUploadRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveInitiateUploadAsync),
        std::bind(&GameSaveAPI::InitiateUpload, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveInitiateUploadGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveInitiateUploadGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveInitiateUploadResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveInitiateUploadResponse*>(buffer);

    return S_OK;
}

HRESULT PFGameSaveListManifestsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveListManifestsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveListManifestsAsync),
        std::bind(&GameSaveAPI::ListManifests, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveListManifestsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveListManifestsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveListManifestsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveListManifestsResponse*>(buffer);

    return S_OK;
}

#if 0
HRESULT PFGameSaveRequestOnboardingAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveRequestOnboardingRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveRequestOnboardingAsync),
        std::bind(&GameSaveAPI::RequestOnboarding, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

HRESULT PFGameSaveRollbackToManifestAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveRollbackToManifestRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveRollbackToManifestAsync),
        std::bind(&GameSaveAPI::RollbackToManifest, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveRollbackToManifestGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveRollbackToManifestGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveRollbackToManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveRollbackToManifestResponse*>(buffer);

    return S_OK;
}

#if 0
HRESULT PFGameSaveUpdateConfigForTitleAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveUpdateConfigForTitleRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveUpdateConfigForTitleAsync),
        std::bind(&GameSaveAPI::UpdateConfigForTitle, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveUpdateConfigForTitleGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveUpdateConfigForTitleGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveUpdateConfigForTitleResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveUpdateConfigForTitleResponse*>(buffer);

    return S_OK;
}
#endif

HRESULT PFGameSaveUpdateManifestAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFGameSaveUpdateManifestRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GameSaveGlobalState> state{ nullptr };
    RETURN_IF_FAILED(GameSaveGlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFGameSaveUpdateManifestAsync),
        std::bind(&GameSaveAPI::UpdateManifest, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

HRESULT PFGameSaveUpdateManifestGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

HRESULT PFGameSaveUpdateManifestGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveUpdateManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFGameSaveUpdateManifestResponse*>(buffer);

    return S_OK;
}


} // namespace PlayFab
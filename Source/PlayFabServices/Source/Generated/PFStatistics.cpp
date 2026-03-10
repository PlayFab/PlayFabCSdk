#include "stdafx.h"
#include <playfab/services/PFStatistics.h>
#include "Generated/Statistics.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>
#include "ApiHelpers.h"

using namespace PlayFab;
using namespace PlayFab::Statistics;

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsCreateStatisticDefinitionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsCreateStatisticDefinitionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsCreateStatisticDefinitionAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsCreateStatisticDefinitionAsync),
            std::bind(&StatisticsAPI::CreateStatisticDefinition, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsDeleteStatisticDefinitionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsDeleteStatisticDefinitionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsDeleteStatisticDefinitionAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsDeleteStatisticDefinitionAsync),
            std::bind(&StatisticsAPI::DeleteStatisticDefinition, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFStatisticsDeleteStatisticsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsDeleteStatisticsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsDeleteStatisticsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsDeleteStatisticsAsync),
            std::bind(&StatisticsAPI::DeleteStatistics, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsDeleteStatisticsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsDeleteStatisticsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsDeleteStatisticsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsDeleteStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsDeleteStatisticsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsDeleteStatisticsResponse*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsGetStatisticDefinitionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsGetStatisticDefinitionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsGetStatisticDefinitionAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsGetStatisticDefinitionAsync),
            std::bind(&StatisticsAPI::GetStatisticDefinition, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsGetStatisticDefinitionGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticDefinitionGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsGetStatisticDefinitionGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsGetStatisticDefinitionResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticDefinitionGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsGetStatisticDefinitionResponse*>(buffer);

        return S_OK;
    });
}
#endif

PF_API PFStatisticsGetStatisticsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsGetStatisticsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsGetStatisticsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsGetStatisticsAsync),
            std::bind(&StatisticsAPI::GetStatistics, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsGetStatisticsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsGetStatisticsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsGetStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsGetStatisticsResponse*>(buffer);

        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsGetStatisticsForEntitiesAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsGetStatisticsForEntitiesRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsGetStatisticsForEntitiesAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsGetStatisticsForEntitiesAsync),
            std::bind(&StatisticsAPI::GetStatisticsForEntities, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsGetStatisticsForEntitiesGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticsForEntitiesGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsGetStatisticsForEntitiesGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsGetStatisticsForEntitiesResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsGetStatisticsForEntitiesGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsGetStatisticsForEntitiesResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsIncrementStatisticVersionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsIncrementStatisticVersionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsIncrementStatisticVersionAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsIncrementStatisticVersionAsync),
            std::bind(&StatisticsAPI::IncrementStatisticVersion, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsIncrementStatisticVersionGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFStatisticsIncrementStatisticVersionResponse* result
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsIncrementStatisticVersionGetResult), [&]()
    {
        return XAsyncGetResult(async, nullptr, sizeof(PFStatisticsIncrementStatisticVersionResponse), result, nullptr);
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFStatisticsListStatisticDefinitionsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsListStatisticDefinitionsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsListStatisticDefinitionsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsListStatisticDefinitionsAsync),
            std::bind(&StatisticsAPI::ListStatisticDefinitions, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsListStatisticDefinitionsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsListStatisticDefinitionsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsListStatisticDefinitionsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsListStatisticDefinitionsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsListStatisticDefinitionsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsListStatisticDefinitionsResponse*>(buffer);

        return S_OK;
    });
}
#endif

#if 0
PF_API PFStatisticsUnlinkAggregationSourceFromStatisticAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsUnlinkAggregationSourceFromStatisticRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsUnlinkAggregationSourceFromStatisticAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsUnlinkAggregationSourceFromStatisticAsync),
            std::bind(&StatisticsAPI::UnlinkAggregationSourceFromStatistic, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFStatisticsUpdateStatisticDefinitionAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsUpdateStatisticDefinitionRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsUpdateStatisticDefinitionAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsUpdateStatisticDefinitionAsync),
            std::bind(&StatisticsAPI::UpdateStatisticDefinition, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}
#endif

PF_API PFStatisticsUpdateStatisticsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFStatisticsUpdateStatisticsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    return AsyncApiImpl(async, XASYNC_IDENTITY(PFStatisticsUpdateStatisticsAsync), [&](SharedPtr<GlobalState> state)
    {
        auto provider = MakeProvider(
            state->RunContext().DeriveOnQueue(async->queue),
            async,
            XASYNC_IDENTITY(PFStatisticsUpdateStatisticsAsync),
            std::bind(&StatisticsAPI::UpdateStatistics, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
        );
        return XAsyncProviderBase::Run(std::move(provider));
    });
}

PF_API PFStatisticsUpdateStatisticsGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsUpdateStatisticsGetResultSize), [&]()
    {
        return XAsyncGetResultSize(async, bufferSize);
    });
}

PF_API PFStatisticsUpdateStatisticsGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsUpdateStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    return ResultApiImpl(XASYNC_IDENTITY(PFStatisticsUpdateStatisticsGetResult), [&]()
    {
        RETURN_HR_INVALIDARG_IF_NULL(result);

        RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
        *result = static_cast<PFStatisticsUpdateStatisticsResponse*>(buffer);

        return S_OK;
    });
}

}
#include "pch.h"
#include <ApiXAsyncProvider.h>
#include "ArchiveOperations.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

namespace PlayFab
{

class CompressionProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    CompressionProvider(
        PlayFab::RunContext&& rc,
        XAsyncBlock* async,
        const char(&identityName)[n],
        SharedPtr<ArchiveContext> archiveContext,
        bool compressing
    ) noexcept :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_archiveContext{ std::move(archiveContext) },
        m_compressing{ compressing }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        TRACE_INFORMATION("CompressionProvider Begin");
        HRESULT hr{};
        if (m_compressing)
        {
            hr = m_archiveContext->Initialize(ArchiveOpenMode::Compress);
        }
        else
        {
            hr = m_archiveContext->Initialize(ArchiveOpenMode::Decompress);
        }

        TRACE_INFORMATION("CompressionProvider Begin HR:0x%0.8x", hr);
        if (FAILED(hr))
        {
            Fail(hr);
            return hr;
        }

        return Schedule(0);
    }

    HRESULT DoWork(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);

        HRESULT hr{};
        TRACE_INFORMATION("CompressionProvider DoWork Compressing:%d", m_compressing);
        if (m_compressing)
        {
            hr = m_archiveContext->CompressBytes();
        }
        else
        {
            hr = m_archiveContext->DecompressBytes();
        }

        // If we're waiting for for current data to be used, schedule the provider to run again
        if (hr == E_PENDING)
        {
            RETURN_IF_FAILED(Schedule(0));
            return hr;
        }

        TRACE_INFORMATION("CompressionProvider HR:0x%0.8x", hr);
        if (FAILED(hr))
        {
            Fail(hr);
            return hr;
        }

        if (m_archiveContext->IsArchiveOperationDone())
        {
            TRACE_INFORMATION("CompressionProvider Done");
            Complete(0);
        }
        else
        {
            TRACE_INFORMATION("CompressionProvider Schedule");
            RETURN_IF_FAILED(Schedule(0));
        }
            
        return E_PENDING;
    }

private:
    SharedPtr<ArchiveContext> m_archiveContext;
    bool m_compressing{};
};

ArchiveStreamCompressionOp::ArchiveStreamCompressionOp(
    SharedPtr<ArchiveContext> context,
    bool compressing,
    PlayFab::RunContext runContext
) noexcept :
    XAsyncOperation<void>{ std::move(runContext) },
    m_compressing{ compressing },
    m_archiveContext{ context }
{
}

HRESULT ArchiveStreamCompressionOp::OnStarted(XAsyncBlock* async) noexcept
{
    auto provider = MakeUnique<CompressionProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(ArchiveStreamCompressionOp), m_archiveContext, m_compressing);
    return XAsyncProviderBase::Run(std::move(provider));
}

class WaitOnOperationProvider : public XAsyncProviderBase
{
public:
    template<size_t n>
    WaitOnOperationProvider(
        PlayFab::RunContext&& rc,
        XAsyncBlock* async,
        const char(&identityName)[n],
        std::mutex* resultMutex,
        std::condition_variable* resultCondition,
        bool* allComplete,
        HRESULT* result
    ) noexcept :
        XAsyncProviderBase{ std::move(rc), async, identityName },
        m_resultMutex{ resultMutex },
        m_resultCondition{ resultCondition },
        m_allComplete{ allComplete },
        m_result{ result }
    {
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);
        TRACE_INFORMATION("WaitOnOperationProvider Begin");
        return Schedule(0);
    }

    HRESULT DoWork(RunContext runContext) override
    {
        UNREFERENCED_PARAMETER(runContext);
        TRACE_INFORMATION("WaitOnOperationProvider DoWork");
        std::unique_lock<std::mutex> lock{ *m_resultMutex };
        if (m_resultCondition->wait_for(lock, std::chrono::milliseconds{ 100 }, [&] { return *m_allComplete; }))
        {
            if (FAILED(*m_result))
            {
                Fail(*m_result);
            }
            else
            {
                Complete(0);
            }

            return *m_result;
        }

        RETURN_IF_FAILED(Schedule(0));
        return E_PENDING;
    }

private:
    std::mutex* m_resultMutex;
    std::condition_variable* m_resultCondition;
    bool* m_allComplete;
    HRESULT* m_result;
};

CombinedOp::CombinedOp(
    Vector<AsyncOp<void>> operations,
    PlayFab::RunContext runContext
) noexcept :
    XAsyncOperation<void>{ std::move(runContext) },
    m_operations{ std::move(operations) }
{
}

AsyncOp<void> CombinedOp::MakeCombinedWithHttpResult(
    Vector<AsyncOp<void>> operations,
    PlayFab::RunContext runContext
) noexcept
{
    auto combinedOp = MakeUnique<CombinedOp>(std::move(operations), runContext);
    auto httpResult = combinedOp->m_httpResult;
    return RunOperation(std::move(combinedOp)).Then([&, httpResult](Result<void> result) -> AsyncOp<void>
    {
        return Result<void>{ result.hr, std::move(*httpResult.get()) };
    });
}

HRESULT CombinedOp::OnStarted(XAsyncBlock* async) noexcept
{
    TRACE_INFORMATION("CombinedOp::OnStarted");
    for (auto& operation : m_operations)
    {
        operation.Finally([&](Result<void> result)
        {
            TRACE_INFORMATION("CombinedOp::OnStarted Finally");
            bool allComplete{};

            {
                std::lock_guard<std::mutex> lock{ m_resultMutex };
                m_result = SUCCEEDED(m_result) ? result.hr : m_result;
                if (result.httpResult.has_value())
                {
                    m_httpResult.get()->emplace(result.httpResult.value());
                }
                m_allComplete = (m_operations.size() == ++m_resultCount);
                allComplete = m_allComplete;
            }
            TRACE_INFORMATION("CombinedOp::OnStarted Finally allComplete:%d", allComplete);

            if (allComplete)
            {
                m_resultCondition.notify_one();
            }
        });
    }

    auto provider = MakeUnique<WaitOnOperationProvider>(RunContext().DeriveOnQueue(async->queue), async, XASYNC_IDENTITY(CombinedOp), &m_resultMutex, &m_resultCondition, &m_allComplete, &m_result);
    return XAsyncProviderBase::Run(std::move(provider));
}

} // namespace PlayFab

#endif
#pragma once

#include "XAsyncOperation.h"
#include "RunContext.h"
#include "Compression.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

namespace PlayFab
{

class ArchiveStreamCompressionOp : public XAsyncOperation<void>
{
public:
    ArchiveStreamCompressionOp(
        SharedPtr<ArchiveContext> archiveHandle,
        bool compressing,
        PlayFab::RunContext runContext
    ) noexcept;

    ArchiveStreamCompressionOp(ArchiveStreamCompressionOp const&) = delete;
    ArchiveStreamCompressionOp& operator=(ArchiveStreamCompressionOp) = delete;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    bool m_compressing;
    SharedPtr<ArchiveContext> m_archiveContext;
};

class CombinedOp : public XAsyncOperation<void>
{
public:
    CombinedOp(
        Vector<AsyncOp<void>> operations,
        PlayFab::RunContext runContext
    ) noexcept;

    CombinedOp(CombinedOp const&) = delete;
    CombinedOp& operator=(CombinedOp) = delete;

    static AsyncOp<void> MakeCombinedWithHttpResult(
        Vector<AsyncOp<void>> operations,
        PlayFab::RunContext runContext
    ) noexcept;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Vector<AsyncOp<void>> m_operations;
    std::mutex m_resultMutex;
    std::condition_variable m_resultCondition;
    bool m_allComplete{};
    HRESULT m_result{};
    SharedPtr<std::optional<HttpResult>> m_httpResult{ MakeShared<std::optional<HttpResult>>() };
    uint32_t m_resultCount{};
};

}

#endif
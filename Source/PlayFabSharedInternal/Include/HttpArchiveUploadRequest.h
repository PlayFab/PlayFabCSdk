#pragma once

#include "HttpRequest.h"
#include "Compression.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && !HC_PLATFORM_IS_PLAYSTATION

namespace PlayFab
{

class HttpArchiveUploadOp
{
public:
    HttpArchiveUploadOp(
        String url,
        UnorderedMap<String, String> headers,
        SharedPtr<ArchiveContext> archiveContext,
        HCHttpCallProgressReportFunction progressCallback,
        void* progressCallbackContext,
        uint64_t uncompressedTotalSizeBytes,
        uint64_t uncompressedCurrentSizeBytes,
        PlayFab::RunContext runContext
    ) noexcept;

    HttpArchiveUploadOp(HttpArchiveUploadOp const&) = delete;
    HttpArchiveUploadOp& operator=(HttpArchiveUploadOp) = delete;

    AsyncOp<void> Run() noexcept;

private:
    PlayFab::RunContext m_runContext;
    String const m_url;
    UnorderedMap<String, String> m_headers;
    SharedPtr<ArchiveContext> m_archiveContext;
    HCHttpCallProgressReportFunction m_progressCallback{};
    void* m_progressCallbackContext{};
    uint64_t m_uncompressedTotalSizeBytes{};
    uint64_t m_uncompressedCurrentSizeBytes{};
};

class HttpArchiveBlockUploadOp : public XAsyncOperation<uint64_t>
{
public:
    HttpArchiveBlockUploadOp(
        String url,
        UnorderedMap<String, String> headers,
        SharedPtr<ArchiveContext> archiveContext,
        HCHttpCallProgressReportFunction progressCallback,
        void* progressCallbackContext,
        uint64_t uncompressedTotalSizeBytes,
        uint64_t uncompressedCurrentSizeBytes,
        PlayFab::RunContext runContext
    ) noexcept;

    HttpArchiveBlockUploadOp(HttpArchiveBlockUploadOp const&) = delete;
    HttpArchiveBlockUploadOp& operator=(HttpArchiveBlockUploadOp) = delete;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<uint64_t> GetResult(XAsyncBlock* async) noexcept override;

    String const m_url;
    UnorderedMap<String, String> m_headers;
    SharedPtr<ArchiveContext> m_archiveContext;
    HCHttpCallProgressReportFunction m_progressCallback{};
    void* m_progressCallbackContext{};
    uint64_t m_uncompressedTotalSizeBytes{};
    uint64_t m_uncompressedCurrentSizeBytes{};
};

}

#endif
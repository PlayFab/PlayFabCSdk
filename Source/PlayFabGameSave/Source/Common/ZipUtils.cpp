#include "stdafx.h"
#include "ZipUtils.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT ZipUtils::UnzipFilesFromSingleZip(
    _In_ const SharedPtr<ArchiveContext>& archiveContext,
    _In_ const String& fullCompressedFilePath) noexcept
{
    RETURN_IF_FAILED(archiveContext->Initialize(ArchiveOpenMode::Decompress, ArchiveSource::File, fullCompressedFilePath));
    HRESULT hr{};
    while (SUCCEEDED(hr) && !archiveContext->IsArchiveOperationDone())
    {
        hr = archiveContext->DecompressBytes();
    }
    archiveContext->Close();

    return hr;
}

HRESULT ZipUtils::ZipFilesIntoSingleZip(
    _In_ const SharedPtr<ArchiveContext>& archiveContext,
    _In_ const String& fullCompressedFilePath) noexcept
{
    RETURN_IF_FAILED(archiveContext->Initialize(ArchiveOpenMode::Compress, ArchiveSource::File, fullCompressedFilePath));
    HRESULT hr{};
    while (SUCCEEDED(hr) && !archiveContext->IsArchiveOperationDone())
    {
        hr = archiveContext->CompressBytes();
    }
    archiveContext->Close();

    return hr;
}

} // namespace GameSave
} // namespace PlayFab
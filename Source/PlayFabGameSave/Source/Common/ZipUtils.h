// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "Manifest.h"

namespace PlayFab
{
namespace GameSave
{

class ZipUtils
{
public:    
    static HRESULT ZipFilesIntoSingleZip(
        _In_ const SharedPtr<ArchiveContext>& archiveContext,
        _In_ const String& fullCompressedFilePath) noexcept;

    static HRESULT UnzipFilesFromSingleZip(
        _In_ const SharedPtr<ArchiveContext>& archiveContext,
        _In_ const String& fullCompressedFilePath) noexcept;
};


} // namespace GameSave
} // namespace PlayFab
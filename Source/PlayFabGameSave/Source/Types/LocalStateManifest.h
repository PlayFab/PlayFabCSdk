// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FileFolderSet.h"

namespace PlayFab
{
namespace GameSave
{
    
class LocalStateManifest
{
public:
    static HRESULT WriteLocalManifest(const String& rootPath, const SharedPtr<FileFolderSet>& localFileFolderSet, const String& shortSaveDescription);
};

} // namespace GameSave
} // namespace PlayFab
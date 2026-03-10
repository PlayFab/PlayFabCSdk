// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{
    
struct InfoManifestData
{
    String deviceId;

    static HRESULT ReadInfoManifest(_In_ const String& filePath, _Out_ InfoManifestData& infoManifestData);
    static HRESULT WriteInfoManifest(_In_ const String& filePath, _Out_ InfoManifestData& infoManifestData);
};

} // namespace GameSave
} // namespace PlayFab
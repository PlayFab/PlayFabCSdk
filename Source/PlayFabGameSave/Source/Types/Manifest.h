// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "FileFolderSet.h"

namespace PlayFab
{
namespace GameSave
{

class ManifestInternal
{
public:
    ManifestInternal(const ManifestWrap& latestFinalizedManifest);
    const ManifestWrap& GetManifest() const { return m_manifest; }
    uint64_t Version() const;
    const String& VersionString() const;
    bool HasVersion() const;

    void SetRemoteFileDetails(DownloadDetailsWrapVector&& remoteFileDetails) { m_remoteFileDetails = std::move(remoteFileDetails); m_gotRemoteFileDetails = true; }
    bool GotRemoteFileDetails() const { return m_gotRemoteFileDetails; }
    const DownloadDetailsWrapVector& GetRemoteFileDetails() { return m_remoteFileDetails; }
    static String GetDownloadUrlForFile(const String& fileName, const DownloadDetailsWrapVector& remoteFileDetails);

private:
    ManifestWrap m_manifest{};
    DownloadDetailsWrapVector m_remoteFileDetails;
    bool m_gotRemoteFileDetails{ false };

};

} // namespace GameSave
} // namespace PlayFab
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
    String GetDecodedManifestDescription() const;

    void SetRemoteFileDetails(DownloadDetailsWrapVector&& remoteFileDetails) { m_remoteFileDetails = std::move(remoteFileDetails); m_gotRemoteFileDetails = true; }
    bool GotRemoteFileDetails() const { return m_gotRemoteFileDetails; }
    // Drops the cached download details so the next pass re-issues GetManifestDownloadDetails and
    // obtains fresh download URLs. Needed when a URL from a previous fetch is no longer usable -
    // for example an expired SAS token - because retrying the identical URL fails identically
    // every time (Bug 63588284).
    //
    // Clearing m_gotRemoteFileDetails is the point of this call, not just freeing the vector:
    // CompareStage::GetManifestDownloadDetails skips straight to ReadLocalManifest while the flag
    // is set, so without resetting it a retry would silently bypass the extended manifest entirely.
    void ClearRemoteFileDetails() { m_remoteFileDetails.clear(); m_gotRemoteFileDetails = false; }
    const DownloadDetailsWrapVector& GetRemoteFileDetails() { return m_remoteFileDetails; }
    static String GetDownloadUrlForFile(const String& fileName, const DownloadDetailsWrapVector& remoteFileDetails);

private:
    ManifestWrap m_manifest{};
    DownloadDetailsWrapVector m_remoteFileDetails;
    bool m_gotRemoteFileDetails{ false };

};

} // namespace GameSave
} // namespace PlayFab
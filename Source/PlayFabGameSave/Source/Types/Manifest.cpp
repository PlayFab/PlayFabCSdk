// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "Manifest.h"
#include "ApiHelpers.h"
#include "FileFolderSet.h"
#include "PlatformUtils.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{
    
ManifestInternal::ManifestInternal(const ManifestWrap& latestFinalizedManifest) :
    m_manifest( latestFinalizedManifest )
{    
}

uint64_t ManifestInternal::Version() const
{
    return StringToUint64(m_manifest.GetVersion());
}

const String& ManifestInternal::VersionString() const
{
    return m_manifest.GetVersion();
}

bool ManifestInternal::HasVersion() const
{
    return !m_manifest.GetVersion().empty();
}

String ManifestInternal::GetDecodedManifestDescription() const
{
    const String& encodedDescription = m_manifest.GetManifestDescription();
    if (encodedDescription.empty())
    {
        return encodedDescription;
    }
    return Base64Decode(encodedDescription);
}

String ManifestInternal::GetDownloadUrlForFile(const String& fileName, const DownloadDetailsWrapVector& remoteFileDetails)
{
    for (size_t i = 0; i < remoteFileDetails.size(); i++)
    {
        const DownloadDetailsWrap& details = remoteFileDetails[i];
        if (details.GetFileName() == fileName)
        {
            return details.GetDownloadUrl();
        }
    }

    return "";
}

} // namespace GameSave
} // namespace PlayFab
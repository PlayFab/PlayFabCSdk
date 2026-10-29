// Copyright (C) Microsoft Corporation. All rights reserved.
// UploadStep's pure batching rules (SplitUploadsIntoZipBatches and PopFileDetailBelowSize). They only
// read the list of files to upload, so they live in their own translation unit: the unit test
// executable compiles this file directly (white-box seam, core unit test plan H-1) without the rest of
// the step. Moved unchanged from UploadStep.cpp.
#include "stdafx.h"
#include "UploadStep.h"

namespace PlayFab
{
namespace GameSave
{

const FileDetail* UploadStep::PopFileDetailBelowSize(
    _In_ Vector<const FileDetail*>& filesToUpload, 
    _In_ uint64_t sizeLimit)
{
    for (auto it = filesToUpload.begin(); it != filesToUpload.end(); it++)
    {
        const FileDetail* curFileDetail = *it;
        if (curFileDetail->fileSizeBytes <= sizeLimit)
        {
            // Remove element from vector, and return element
            if (filesToUpload.size() > 1)
            {
                std::iter_swap(it, filesToUpload.end() - 1);
                filesToUpload.pop_back();
            }
            else
            {
                filesToUpload.clear();
            }
            return curFileDetail;
        }
    }

    return nullptr;
}

Vector<Vector<const FileDetail*>> UploadStep::SplitUploadsIntoZipBatches(
    _In_ Vector<const FileDetail*> filesToUpload // copy of vector on purpose
    )
{
    Vector<Vector<const FileDetail*>> fileZipBatchSets;
    const uint64_t maxUncompressedSize = 1048576ull * 64; // 64 MB

    uint64_t uncompressedSizeInCurrentBatch{ 0 };

    Vector<const FileDetail*> fileZipBatchSet;
    while (filesToUpload.size() > 0)
    {
        uint64_t fileSizeLimit = maxUncompressedSize - uncompressedSizeInCurrentBatch;
        const FileDetail* fileDetailToAdd = PopFileDetailBelowSize(filesToUpload, fileSizeLimit);
        if (fileDetailToAdd)
        {
            uncompressedSizeInCurrentBatch += fileDetailToAdd->fileSizeBytes;
            fileZipBatchSet.push_back(fileDetailToAdd);
        }
        else
        {
            if (fileZipBatchSet.size() == 0 && filesToUpload.size() > 0)
            {
                // If there was nothing yet in set but files remain, meaning the remaining file(s) 
                // are bigger than size limit so just push it to a new batch all by itself
                TRACE_WARNING("[GAME SAVE] UploadStep: File size (%llu bytes) exceeds max batch size (%llu bytes), creating oversized single-file batch",
                    filesToUpload.back()->fileSizeBytes, maxUncompressedSize);
                fileZipBatchSet.push_back(filesToUpload.back());
                filesToUpload.pop_back();
            }

            if (fileZipBatchSet.size() > 0)
            {
                // No more files will fit in this batch, so start a new batch
                fileZipBatchSets.push_back(std::move(fileZipBatchSet));
                fileZipBatchSet.clear();
                uncompressedSizeInCurrentBatch = 0;
            }
        }
    }

    if (fileZipBatchSet.size() > 0)
    {
        // Add the remaining the final zip if needed
        fileZipBatchSets.push_back(std::move(fileZipBatchSet));
    }

    return fileZipBatchSets;
}

} // namespace GameSave
} // namespace PlayFab

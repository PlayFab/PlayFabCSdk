#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "DataTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Data
{

class DataAPI
{
public:
    DataAPI() = delete;
    DataAPI(const DataAPI& source) = delete;
    DataAPI& operator=(const DataAPI& source) = delete;
    ~DataAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<AbortFileUploadsResponse> AbortFileUploads(Entity const& entity, const AbortFileUploadsRequest& request, RunContext rc);
    static AsyncOp<DeleteFilesResponse> DeleteFiles(Entity const& entity, const DeleteFilesRequest& request, RunContext rc);
    static AsyncOp<FinalizeFileUploadsResponse> FinalizeFileUploads(Entity const& entity, const FinalizeFileUploadsRequest& request, RunContext rc);
    static AsyncOp<GetFilesResponse> GetFiles(Entity const& entity, const GetFilesRequest& request, RunContext rc);
    static AsyncOp<GetObjectsResponse> GetObjects(Entity const& entity, const GetObjectsRequest& request, RunContext rc);
    static AsyncOp<InitiateFileUploadsResponse> InitiateFileUploads(Entity const& entity, const InitiateFileUploadsRequest& request, RunContext rc);
    static AsyncOp<SetObjectsResponse> SetObjects(Entity const& entity, const SetObjectsRequest& request, RunContext rc);
};

} // namespace Data
} // namespace PlayFab

#include "pch.h"

#include "XblStorageHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblTitleStorageBlobMetadataResultHandle s_blobMetadataResult = nullptr;
static std::vector<uint8_t> s_storageBuffer;

// ============================================================================
// Title Storage (title_storage_c.h)
// ============================================================================

CommandResultPayload HandleXblTitleStorageGetQuotaAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string serviceConfigurationId;
            int64_t storageType = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "storageType", storageType, error))
            {
                storageType = 2;
            }

            HRESULT hr = XblTitleStorageGetQuotaAsync(
                state->xblContext,
                serviceConfigurationId.c_str(),
                static_cast<XblTitleStorageType>(storageType),
                &async);
            LogToWindowFormat("XblTitleStorageGetQuotaAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t usedBytes = 0;
            size_t quotaBytes = 0;
            HRESULT hr = XblTitleStorageGetQuotaResult(&async, &usedBytes, &quotaBytes);
            if (SUCCEEDED(hr))
            {
                payload.result["usedBytes"] = usedBytes;
                payload.result["quotaBytes"] = quotaBytes;
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageGetBlobMetadataAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string serviceConfigurationId;
            int64_t storageType = 0;
            std::string blobPath;
            int64_t xboxUserId = 0;
            int64_t skipItems = 0;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "storageType", storageType, error))
            {
                storageType = 2; // XblTitleStorageType::Universal
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "blobPath", blobPath, error))
            {
                blobPath = "";
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "skipItems", skipItems, error);
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error))
            {
                maxItems = 25;
            }

            HRESULT hr = XblTitleStorageGetBlobMetadataAsync(
                state->xblContext,
                serviceConfigurationId.c_str(),
                static_cast<XblTitleStorageType>(storageType),
                blobPath.c_str(),
                static_cast<uint64_t>(xboxUserId),
                static_cast<uint32_t>(skipItems),
                static_cast<uint32_t>(maxItems),
                &async);
            LogToWindowFormat("XblTitleStorageGetBlobMetadataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_blobMetadataResult) { XblTitleStorageBlobMetadataResultCloseHandle(s_blobMetadataResult); }
            HRESULT hr = XblTitleStorageGetBlobMetadataResult(&async, &s_blobMetadataResult);
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultGetItems(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_blobMetadataResult);

            const XblTitleStorageBlobMetadata* items = nullptr;
            size_t itemsCount = 0;
            const HRESULT hr = XblTitleStorageBlobMetadataResultGetItems(s_blobMetadataResult, &items, &itemsCount);
            LogToWindowFormat("XblTitleStorageBlobMetadataResultGetItems (count=%zu, hr=0x%08X)",
                itemsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["itemsCount"] = itemsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultHasNext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_blobMetadataResult);

            bool hasNext = false;
            const HRESULT hr = XblTitleStorageBlobMetadataResultHasNext(s_blobMetadataResult, &hasNext);
            LogToWindowFormat("XblTitleStorageBlobMetadataResultHasNext (hasNext=%s, hr=0x%08X)",
                hasNext ? "true" : "false", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["hasNext"] = hasNext;
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultGetNextAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_blobMetadataResult);

            std::string error;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error)) { return E_INVALIDARG; }

            HRESULT hr = XblTitleStorageBlobMetadataResultGetNextAsync(
                s_blobMetadataResult,
                static_cast<uint32_t>(maxItems),
                &async);
            LogToWindowFormat("XblTitleStorageBlobMetadataResultGetNextAsync (maxItems=%u, hr=0x%08X)",
                static_cast<uint32_t>(maxItems), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblTitleStorageBlobMetadataResultCloseHandle(s_blobMetadataResult);
            HRESULT hr = XblTitleStorageBlobMetadataResultGetNextResult(&async, &s_blobMetadataResult);
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_blobMetadataResult);

            XblTitleStorageBlobMetadataResultHandle duplicated = nullptr;
            const HRESULT hr = XblTitleStorageBlobMetadataResultDuplicateHandle(s_blobMetadataResult, &duplicated);
            LogToWindowFormat("XblTitleStorageBlobMetadataResultDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblTitleStorageBlobMetadataResultCloseHandle(s_blobMetadataResult);
                s_blobMetadataResult = duplicated;
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_blobMetadataResult);
            XblTitleStorageBlobMetadataResultCloseHandle(s_blobMetadataResult);
            s_blobMetadataResult = nullptr;
            LogToWindowFormat("XblTitleStorageBlobMetadataResultCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblTitleStorageDeleteBlobAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string serviceConfigurationId;
            int64_t storageType = 0;
            std::string blobPath;
            int64_t blobType = 0;
            int64_t xboxUserId = 0;
            std::string clientTimestamp;
            std::string displayName;
            std::string eTag;
            int64_t length = 0;
            bool deleteOnlyIfEtagMatches = false;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "storageType", storageType, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "blobPath", blobPath, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "blobType", blobType, error)) { return E_INVALIDARG; }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "clientTimestamp", clientTimestamp, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "displayName", displayName, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "eTag", eTag, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "length", length, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "deleteOnlyIfEtagMatches", deleteOnlyIfEtagMatches, error);

            XblTitleStorageBlobMetadata metadata{};
            strncpy_s(metadata.serviceConfigurationId, serviceConfigurationId.c_str(), _TRUNCATE);
            metadata.storageType = static_cast<XblTitleStorageType>(storageType);
            strncpy_s(metadata.blobPath, blobPath.c_str(), _TRUNCATE);
            metadata.blobType = static_cast<XblTitleStorageBlobType>(blobType);
            metadata.xboxUserId = static_cast<uint64_t>(xboxUserId);

            HRESULT hr = XblTitleStorageDeleteBlobAsync(
                state->xblContext,
                metadata,
                deleteOnlyIfEtagMatches,
                &async);
            LogToWindowFormat("XblTitleStorageDeleteBlobAsync (blobPath=%s, hr=0x%08X)",
                blobPath.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageDownloadBlobAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string serviceConfigurationId;
            int64_t storageType = 0;
            std::string blobPath;
            int64_t blobType = 0;
            int64_t xboxUserId = 0;
            std::string clientTimestamp;
            std::string displayName;
            std::string eTag;
            int64_t length = 0;
            int64_t bufferSize = 0;
            int64_t etagMatchCondition = 0;
            std::string selectQuery;
            int64_t preferredDownloadBlockSize = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "storageType", storageType, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "blobPath", blobPath, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "blobType", blobType, error)) { return E_INVALIDARG; }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "clientTimestamp", clientTimestamp, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "displayName", displayName, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "eTag", eTag, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "length", length, error);
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bufferSize", bufferSize, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "etagMatchCondition", etagMatchCondition, error)) { return E_INVALIDARG; }
            CommandHandlerShared::TryGetStringParameter(parameters, "selectQuery", selectQuery, error);
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "preferredDownloadBlockSize", preferredDownloadBlockSize, error)) { return E_INVALIDARG; }

            XblTitleStorageBlobMetadata metadata{};
            strncpy_s(metadata.serviceConfigurationId, serviceConfigurationId.c_str(), _TRUNCATE);
            metadata.storageType = static_cast<XblTitleStorageType>(storageType);
            strncpy_s(metadata.blobPath, blobPath.c_str(), _TRUNCATE);
            metadata.blobType = static_cast<XblTitleStorageBlobType>(blobType);
            metadata.xboxUserId = static_cast<uint64_t>(xboxUserId);

            s_storageBuffer.resize(static_cast<size_t>(bufferSize));

            HRESULT hr = XblTitleStorageDownloadBlobAsync(
                state->xblContext,
                metadata,
                s_storageBuffer.data(),
                static_cast<size_t>(bufferSize),
                static_cast<XblTitleStorageETagMatchCondition>(etagMatchCondition),
                selectQuery.empty() ? nullptr : selectQuery.c_str(),
                static_cast<size_t>(preferredDownloadBlockSize),
                &async);
            LogToWindowFormat("XblTitleStorageDownloadBlobAsync (blobPath=%s, hr=0x%08X)",
                blobPath.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XblTitleStorageBlobMetadata resultMetadata{};
            HRESULT hr = XblTitleStorageDownloadBlobResult(&async, &resultMetadata);
            if (SUCCEEDED(hr))
            {
                payload.result["blobPath"] = resultMetadata.blobPath;
                payload.result["blobType"] = static_cast<int>(resultMetadata.blobType);
                payload.result["storageType"] = static_cast<int>(resultMetadata.storageType);
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageUploadBlobAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            std::string serviceConfigurationId;
            int64_t storageType = 0;
            std::string blobPath;
            int64_t blobType = 0;
            int64_t xboxUserId = 0;
            std::string clientTimestamp;
            std::string displayName;
            std::string eTag;
            int64_t length = 0;
            std::string data;
            int64_t etagMatchCondition = 0;
            int64_t preferredUploadBlockSize = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "storageType", storageType, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "blobPath", blobPath, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "blobType", blobType, error)) { return E_INVALIDARG; }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "clientTimestamp", clientTimestamp, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "displayName", displayName, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "eTag", eTag, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "length", length, error);
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "data", data, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "etagMatchCondition", etagMatchCondition, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "preferredUploadBlockSize", preferredUploadBlockSize, error)) { return E_INVALIDARG; }

            XblTitleStorageBlobMetadata metadata{};
            strncpy_s(metadata.serviceConfigurationId, serviceConfigurationId.c_str(), _TRUNCATE);
            metadata.storageType = static_cast<XblTitleStorageType>(storageType);
            strncpy_s(metadata.blobPath, blobPath.c_str(), _TRUNCATE);
            metadata.blobType = static_cast<XblTitleStorageBlobType>(blobType);
            metadata.xboxUserId = static_cast<uint64_t>(xboxUserId);

            std::vector<uint8_t> blobData(data.begin(), data.end());

            HRESULT hr = XblTitleStorageUploadBlobAsync(
                state->xblContext,
                metadata,
                blobData.data(),
                blobData.size(),
                static_cast<XblTitleStorageETagMatchCondition>(etagMatchCondition),
                static_cast<size_t>(preferredUploadBlockSize),
                &async);
            LogToWindowFormat("XblTitleStorageUploadBlobAsync (blobPath=%s, hr=0x%08X)",
                blobPath.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XblTitleStorageBlobMetadata resultMetadata{};
            HRESULT hr = XblTitleStorageUploadBlobResult(&async, &resultMetadata);
            if (SUCCEEDED(hr))
            {
                payload.result["blobPath"] = resultMetadata.blobPath;
                payload.result["blobType"] = static_cast<int>(resultMetadata.blobType);
                payload.result["storageType"] = static_cast<int>(resultMetadata.storageType);
            }
            return hr;
        });
}

CommandResultPayload HandleXblTitleStorageBlobMetadataResultGetNextResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblTitleStorageBlobMetadataResultGetNextResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblTitleStorageGetQuotaResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblTitleStorageGetQuotaResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblTitleStorageGetBlobMetadataResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblTitleStorageGetBlobMetadataResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblTitleStorageDownloadBlobResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblTitleStorageDownloadBlobResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblTitleStorageUploadBlobResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblTitleStorageUploadBlobResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblTitleStorageBlobMetadataResultCloseHandle", HandleXblTitleStorageBlobMetadataResultCloseHandle },
    { "XblTitleStorageBlobMetadataResultDuplicateHandle", HandleXblTitleStorageBlobMetadataResultDuplicateHandle },
    { "XblTitleStorageBlobMetadataResultGetItems", HandleXblTitleStorageBlobMetadataResultGetItems },
    { "XblTitleStorageBlobMetadataResultGetNextAsync", HandleXblTitleStorageBlobMetadataResultGetNextAsync },
    { "XblTitleStorageBlobMetadataResultGetNextResult", HandleXblTitleStorageBlobMetadataResultGetNextResult },
    { "XblTitleStorageBlobMetadataResultHasNext", HandleXblTitleStorageBlobMetadataResultHasNext },
    { "XblTitleStorageDeleteBlobAsync", HandleXblTitleStorageDeleteBlobAsync },
    { "XblTitleStorageDownloadBlobAsync", HandleXblTitleStorageDownloadBlobAsync },
    { "XblTitleStorageDownloadBlobResult", HandleXblTitleStorageDownloadBlobResult },
    { "XblTitleStorageGetBlobMetadataAsync", HandleXblTitleStorageGetBlobMetadataAsync },
    { "XblTitleStorageGetBlobMetadataResult", HandleXblTitleStorageGetBlobMetadataResult },
    { "XblTitleStorageGetQuotaAsync", HandleXblTitleStorageGetQuotaAsync },
    { "XblTitleStorageGetQuotaResult", HandleXblTitleStorageGetQuotaResult },
    { "XblTitleStorageUploadBlobAsync", HandleXblTitleStorageUploadBlobAsync },
    { "XblTitleStorageUploadBlobResult", HandleXblTitleStorageUploadBlobResult }
});

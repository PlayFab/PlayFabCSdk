#include "pch.h"

#include "XGameSaveHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XGameSave.h>
#include "CommandRegistry.h"

static bool CALLBACK ContainerInfoEnumCallback(const XGameSaveContainerInfo* info, void* /*context*/)
{
    if (info)
    {
        LogToWindowFormat("  Container: %s (blobs=%u, size=%llu)", info->name, info->blobCount, static_cast<unsigned long long>(info->totalSize));
    }
    return true;
}

static bool CALLBACK BlobInfoEnumCallback(const XGameSaveBlobInfo* info, void* /*context*/)
{
    if (info)
    {
        LogToWindowFormat("  Blob: %s (size=%u)", info->name, info->size);
    }
    return true;
}

CommandResultPayload HandleXGameSaveInitializeProvider(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameSaveProviderHandle provider = nullptr;
            const HRESULT hr = XGameSaveInitializeProvider(state->xuser, "00000000-0000-0000-0000-000000000000", false, &provider);
            LogToWindowFormat("XGameSaveInitializeProvider (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->gameSaveProvider = provider;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveInitializeProviderAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveInitializeProviderAsync(state->xuser, "00000000-0000-0000-0000-000000000000", false, &async);
            LogToWindowFormat("XGameSaveInitializeProviderAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XGameSaveProviderHandle provider = nullptr;
            const HRESULT hr = XGameSaveInitializeProviderResult(&async, &provider);
            LogToWindowFormat("XGameSaveInitializeProviderResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->gameSaveProvider = provider;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveInitializeProviderResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveInitializeProviderResult: called inline by InitializeProviderAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveCloseProvider(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->gameSaveProvider)
            {
                XGameSaveCloseProvider(state->gameSaveProvider);
                state->gameSaveProvider = nullptr;
            }
            LogToWindow("XGameSaveCloseProvider");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveGetRemainingQuota(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int64_t remainingQuota = 0;
            const HRESULT hr = XGameSaveGetRemainingQuota(state->gameSaveProvider, &remainingQuota);
            LogToWindowFormat("XGameSaveGetRemainingQuota (hr=0x%08X, quota=%lld)", static_cast<uint32_t>(hr), static_cast<long long>(remainingQuota));
            RETURN_IF_FAILED(hr);
            payload.result["remainingQuota"] = remainingQuota;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveGetRemainingQuotaAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveGetRemainingQuotaAsync(state->gameSaveProvider, &async);
            LogToWindowFormat("XGameSaveGetRemainingQuotaAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            int64_t remainingQuota = 0;
            const HRESULT hr = XGameSaveGetRemainingQuotaResult(&async, &remainingQuota);
            LogToWindowFormat("XGameSaveGetRemainingQuotaResult (hr=0x%08X, quota=%lld)", static_cast<uint32_t>(hr), static_cast<long long>(remainingQuota));
            RETURN_IF_FAILED(hr);
            payload.result["remainingQuota"] = remainingQuota;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveGetRemainingQuotaResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveGetRemainingQuotaResult: called inline by GetRemainingQuotaAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveDeleteContainer(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveDeleteContainer(state->gameSaveProvider, "TestContainer");
            LogToWindowFormat("XGameSaveDeleteContainer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveDeleteContainerAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveDeleteContainerAsync(state->gameSaveProvider, "TestContainer", &async);
            LogToWindowFormat("XGameSaveDeleteContainerAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveDeleteContainerResult(&async);
            LogToWindowFormat("XGameSaveDeleteContainerResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveDeleteContainerResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveDeleteContainerResult: called inline by DeleteContainerAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveGetContainerInfo(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveGetContainerInfo(state->gameSaveProvider, "TestContainer", nullptr, ContainerInfoEnumCallback);
            LogToWindowFormat("XGameSaveGetContainerInfo (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveEnumerateContainerInfo(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveEnumerateContainerInfo(state->gameSaveProvider, nullptr, ContainerInfoEnumCallback);
            LogToWindowFormat("XGameSaveEnumerateContainerInfo (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveEnumerateContainerInfoByName(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveEnumerateContainerInfoByName(state->gameSaveProvider, nullptr, nullptr, ContainerInfoEnumCallback);
            LogToWindowFormat("XGameSaveEnumerateContainerInfoByName (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveCreateContainer(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameSaveContainerHandle container = nullptr;
            const HRESULT hr = XGameSaveCreateContainer(state->gameSaveProvider, "TestContainer", &container);
            LogToWindowFormat("XGameSaveCreateContainer (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->gameSaveContainer = container;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveCloseContainer(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->gameSaveContainer)
            {
                XGameSaveCloseContainer(state->gameSaveContainer);
                state->gameSaveContainer = nullptr;
            }
            LogToWindow("XGameSaveCloseContainer");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveEnumerateBlobInfo(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveEnumerateBlobInfo(state->gameSaveContainer, nullptr, BlobInfoEnumCallback);
            LogToWindowFormat("XGameSaveEnumerateBlobInfo (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveEnumerateBlobInfoByName(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveEnumerateBlobInfoByName(state->gameSaveContainer, nullptr, nullptr, BlobInfoEnumCallback);
            LogToWindowFormat("XGameSaveEnumerateBlobInfoByName (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveReadBlobData(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t countOfBlobs = 0;
            const HRESULT hr = XGameSaveReadBlobData(state->gameSaveContainer, nullptr, &countOfBlobs, 0, nullptr);
            LogToWindowFormat("XGameSaveReadBlobData (hr=0x%08X, count=%u)", static_cast<uint32_t>(hr), countOfBlobs);
            payload.result["countOfBlobs"] = countOfBlobs;
            return hr;
        });
}

CommandResultPayload HandleXGameSaveReadBlobDataAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveReadBlobDataAsync(state->gameSaveContainer, nullptr, 0, &async);
            LogToWindowFormat("XGameSaveReadBlobDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            uint32_t countOfBlobs = 0;
            const HRESULT hr = XGameSaveReadBlobDataResult(&async, 0, nullptr, &countOfBlobs);
            LogToWindowFormat("XGameSaveReadBlobDataResult (hr=0x%08X, count=%u)", static_cast<uint32_t>(hr), countOfBlobs);
            payload.result["countOfBlobs"] = countOfBlobs;
            return hr;
        });
}

CommandResultPayload HandleXGameSaveReadBlobDataResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveReadBlobDataResult: called inline by ReadBlobDataAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveCreateUpdate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameSaveUpdateHandle update = nullptr;
            const HRESULT hr = XGameSaveCreateUpdate(state->gameSaveContainer, "Test Container", &update);
            LogToWindowFormat("XGameSaveCreateUpdate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->gameSaveUpdate = update;
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveCloseUpdate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->gameSaveUpdate)
            {
                XGameSaveCloseUpdate(state->gameSaveUpdate);
                state->gameSaveUpdate = nullptr;
            }
            LogToWindow("XGameSaveCloseUpdate");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveSubmitBlobWrite(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            static const uint8_t testData[] = { 0x54, 0x65, 0x73, 0x74 };
            const HRESULT hr = XGameSaveSubmitBlobWrite(state->gameSaveUpdate, "TestBlob", testData, sizeof(testData));
            LogToWindowFormat("XGameSaveSubmitBlobWrite (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveSubmitBlobDelete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveSubmitBlobDelete(state->gameSaveUpdate, "TestBlob");
            LogToWindowFormat("XGameSaveSubmitBlobDelete (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveSubmitUpdate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveSubmitUpdate(state->gameSaveUpdate);
            LogToWindowFormat("XGameSaveSubmitUpdate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveSubmitUpdateAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveSubmitUpdateAsync(state->gameSaveUpdate, &async);
            LogToWindowFormat("XGameSaveSubmitUpdateAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameSaveSubmitUpdateResult(&async);
            LogToWindowFormat("XGameSaveSubmitUpdateResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXGameSaveSubmitUpdateResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveSubmitUpdateResult: called inline by SubmitUpdateAsync handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XGameSaveCloseContainer", HandleXGameSaveCloseContainer },
    { "XGameSaveCloseProvider", HandleXGameSaveCloseProvider },
    { "XGameSaveCloseUpdate", HandleXGameSaveCloseUpdate },
    { "XGameSaveCreateContainer", HandleXGameSaveCreateContainer },
    { "XGameSaveCreateUpdate", HandleXGameSaveCreateUpdate },
    { "XGameSaveDeleteContainer", HandleXGameSaveDeleteContainer },
    { "XGameSaveDeleteContainerAsync", HandleXGameSaveDeleteContainerAsync },
    { "XGameSaveDeleteContainerResult", HandleXGameSaveDeleteContainerResult },
    { "XGameSaveEnumerateBlobInfo", HandleXGameSaveEnumerateBlobInfo },
    { "XGameSaveEnumerateBlobInfoByName", HandleXGameSaveEnumerateBlobInfoByName },
    { "XGameSaveEnumerateContainerInfo", HandleXGameSaveEnumerateContainerInfo },
    { "XGameSaveEnumerateContainerInfoByName", HandleXGameSaveEnumerateContainerInfoByName },
    { "XGameSaveGetContainerInfo", HandleXGameSaveGetContainerInfo },
    { "XGameSaveGetRemainingQuota", HandleXGameSaveGetRemainingQuota },
    { "XGameSaveGetRemainingQuotaAsync", HandleXGameSaveGetRemainingQuotaAsync },
    { "XGameSaveGetRemainingQuotaResult", HandleXGameSaveGetRemainingQuotaResult },
    { "XGameSaveInitializeProvider", HandleXGameSaveInitializeProvider },
    { "XGameSaveInitializeProviderAsync", HandleXGameSaveInitializeProviderAsync },
    { "XGameSaveInitializeProviderResult", HandleXGameSaveInitializeProviderResult },
    { "XGameSaveReadBlobData", HandleXGameSaveReadBlobData },
    { "XGameSaveReadBlobDataAsync", HandleXGameSaveReadBlobDataAsync },
    { "XGameSaveReadBlobDataResult", HandleXGameSaveReadBlobDataResult },
    { "XGameSaveSubmitBlobDelete", HandleXGameSaveSubmitBlobDelete },
    { "XGameSaveSubmitBlobWrite", HandleXGameSaveSubmitBlobWrite },
    { "XGameSaveSubmitUpdate", HandleXGameSaveSubmitUpdate },
    { "XGameSaveSubmitUpdateAsync", HandleXGameSaveSubmitUpdateAsync },
    { "XGameSaveSubmitUpdateResult", HandleXGameSaveSubmitUpdateResult }
});

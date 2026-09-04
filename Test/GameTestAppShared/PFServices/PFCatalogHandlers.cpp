#include "pch.h"
#include "PFCatalogHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFCatalog.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFCatalogCreateDraftItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));

            // Build a minimal catalog item with required NEUTRAL title
            std::string error;
            std::string titleStr;
            if (!TryGetStringParameter(parameters, "title", titleStr, error))
            {
                titleStr = "Test Draft Item";
            }
            std::string typeStr;
            if (!TryGetStringParameter(parameters, "type", typeStr, error))
            {
                typeStr = "catalogItem";
            }

            PFStringDictionaryEntry titleEntry{};
            titleEntry.key = "NEUTRAL";
            titleEntry.value = titleStr.c_str();

            PFCatalogCatalogItem item{};
            item.title = &titleEntry;
            item.titleCount = 1;
            item.type = typeStr.c_str();

            PFCatalogCreateDraftItemRequest request{};
            request.item = &item;
            bool publish = false;
            TryParseBoolParameter(parameters, "publish", publish, error);
            request.publish = publish;

            const HRESULT hr = PFCatalogCreateDraftItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogCreateDraftItemAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogCreateDraftItemGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogCreateDraftItemResponse* response{ nullptr };
            HRESULT hr = PFCatalogCreateDraftItemGetResult(&async, buffer.size(), buffer.data(), &response, nullptr);
            LogToWindowFormat("PFCatalogCreateDraftItemAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            if (SUCCEEDED(hr) && response && response->item)
            {
                if (response->item->id)
                {
                    payload.result["itemId"] = response->item->id;
                    // Store itemId on state for subsequent commands
                    state->currentScenarioId = response->item->id;
                    LogToWindowFormat("PFCatalogCreateDraftItemAsync: itemId=%s", response->item->id);
                }
            }
            return hr;
        });
}

CommandResultPayload HandlePFCatalogCreateDraftItemGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogCreateDraftItemGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogCreateDraftItemGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogCreateDraftItemGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogCreateUploadUrlsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogCreateUploadUrlsRequest request{};
            const HRESULT hr = PFCatalogCreateUploadUrlsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogCreateUploadUrlsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogCreateUploadUrlsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogCreateUploadUrlsResponse* result{ nullptr };
            HRESULT hr = PFCatalogCreateUploadUrlsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogCreateUploadUrlsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogCreateUploadUrlsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogCreateUploadUrlsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogCreateUploadUrlsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogCreateUploadUrlsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogDeleteEntityItemReviewsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogDeleteEntityItemReviewsRequest request{};
            const HRESULT hr = PFCatalogDeleteEntityItemReviewsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogDeleteEntityItemReviewsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogDeleteItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFCatalogDeleteItemRequest request{};
            std::string error;
            std::string itemId;
            if (TryGetStringParameter(parameters, "itemId", itemId, error))
            {
                request.id = itemId.c_str();
            }
            else if (!state->currentScenarioId.empty())
            {
                request.id = state->currentScenarioId.c_str();
            }
            const HRESULT hr = PFCatalogDeleteItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogDeleteItemAsync (id=%s, hr=0x%08X)", request.id ? request.id : "null", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogGetCatalogConfigAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetCatalogConfigRequest request{};
            const HRESULT hr = PFCatalogGetCatalogConfigAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetCatalogConfigAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetCatalogConfigGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetCatalogConfigResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetCatalogConfigGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetCatalogConfigAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetCatalogConfigGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetCatalogConfigGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetCatalogConfigGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetCatalogConfigGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFCatalogGetDraftItemRequest request{};
            std::string error;
            std::string itemId;
            if (TryGetStringParameter(parameters, "itemId", itemId, error))
            {
                request.id = itemId.c_str();
            }
            else if (!state->currentScenarioId.empty())
            {
                request.id = state->currentScenarioId.c_str();
            }
            const HRESULT hr = PFCatalogGetDraftItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetDraftItemAsync (id=%s, hr=0x%08X)", request.id ? request.id : "null", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetDraftItemGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetDraftItemResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetDraftItemGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetDraftItemAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetDraftItemGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetDraftItemGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetDraftItemsRequest request{};
            const HRESULT hr = PFCatalogGetDraftItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetDraftItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetDraftItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetDraftItemsResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetDraftItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetDraftItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetDraftItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetDraftItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetDraftItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetEntityDraftItemsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetEntityDraftItemsRequest request{};
            const HRESULT hr = PFCatalogGetEntityDraftItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetEntityDraftItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetEntityDraftItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetEntityDraftItemsResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetEntityDraftItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetEntityDraftItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetEntityDraftItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetEntityDraftItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetEntityDraftItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetEntityDraftItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetEntityItemReviewAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetEntityItemReviewRequest request{};
            const HRESULT hr = PFCatalogGetEntityItemReviewAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetEntityItemReviewAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetEntityItemReviewGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetEntityItemReviewResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetEntityItemReviewGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetEntityItemReviewAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetEntityItemReviewGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetEntityItemReviewGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetEntityItemReviewGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetEntityItemReviewGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFCatalogGetItemRequest request{};
            std::string error;
            std::string itemId;
            if (TryGetStringParameter(parameters, "itemId", itemId, error))
            {
                request.id = itemId.c_str();
            }
            else if (!state->currentScenarioId.empty())
            {
                request.id = state->currentScenarioId.c_str();
            }
            const HRESULT hr = PFCatalogGetItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemAsync (id=%s, hr=0x%08X)", request.id ? request.id : "null", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemContainersAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemContainersRequest request{};
            const HRESULT hr = PFCatalogGetItemContainersAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemContainersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemContainersGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemContainersResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemContainersGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemContainersAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemContainersGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemContainersGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemContainersGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemContainersGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemModerationStateAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemModerationStateRequest request{};
            const HRESULT hr = PFCatalogGetItemModerationStateAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemModerationStateAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemModerationStateGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemModerationStateResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemModerationStateGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemModerationStateAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemModerationStateGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemModerationStateGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemModerationStateGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemModerationStateGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemPublishStatusAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemPublishStatusRequest request{};
            const HRESULT hr = PFCatalogGetItemPublishStatusAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemPublishStatusAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemPublishStatusGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemPublishStatusResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemPublishStatusGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemPublishStatusAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemPublishStatusGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemPublishStatusGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemPublishStatusGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemPublishStatusGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemReviewsRequest request{};
            const HRESULT hr = PFCatalogGetItemReviewsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemReviewsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemReviewsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemReviewsResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemReviewsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemReviewsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemReviewsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemReviewsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewSummaryAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemReviewSummaryRequest request{};
            const HRESULT hr = PFCatalogGetItemReviewSummaryAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemReviewSummaryAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemReviewSummaryGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemReviewSummaryResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemReviewSummaryGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemReviewSummaryAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewSummaryGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemReviewSummaryGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemReviewSummaryGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemReviewSummaryGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogGetItemsRequest request{};
            const HRESULT hr = PFCatalogGetItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogGetItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogGetItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogGetItemsResponse* result{ nullptr };
            HRESULT hr = PFCatalogGetItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogGetItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogGetItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogGetItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogGetItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogPublishDraftItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFCatalogPublishDraftItemRequest request{};
            std::string error;
            std::string itemId;
            if (TryGetStringParameter(parameters, "itemId", itemId, error))
            {
                request.id = itemId.c_str();
            }
            else if (!state->currentScenarioId.empty())
            {
                request.id = state->currentScenarioId.c_str();
            }
            const HRESULT hr = PFCatalogPublishDraftItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogPublishDraftItemAsync (id=%s, hr=0x%08X)", request.id ? request.id : "null", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogReportItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogReportItemRequest request{};
            const HRESULT hr = PFCatalogReportItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogReportItemAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogReportItemReviewAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogReportItemReviewRequest request{};
            const HRESULT hr = PFCatalogReportItemReviewAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogReportItemReviewAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogReviewItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogReviewItemRequest request{};
            const HRESULT hr = PFCatalogReviewItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogReviewItemAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogSearchItemsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFCatalogSearchItemsRequest request{};
            request.count = 10;
            const HRESULT hr = PFCatalogSearchItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogSearchItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogSearchItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogSearchItemsResponse* result{ nullptr };
            HRESULT hr = PFCatalogSearchItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogSearchItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogSearchItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogSearchItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogSearchItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogSearchItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogSetItemModerationStateAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogSetItemModerationStateRequest request{};
            const HRESULT hr = PFCatalogSetItemModerationStateAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogSetItemModerationStateAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogSubmitItemReviewVoteAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogSubmitItemReviewVoteRequest request{};
            const HRESULT hr = PFCatalogSubmitItemReviewVoteAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogSubmitItemReviewVoteAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogTakedownItemReviewsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogTakedownItemReviewsRequest request{};
            const HRESULT hr = PFCatalogTakedownItemReviewsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogTakedownItemReviewsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogUpdateCatalogConfigAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFCatalogUpdateCatalogConfigRequest request{};
            const HRESULT hr = PFCatalogUpdateCatalogConfigAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogUpdateCatalogConfigAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFCatalogUpdateDraftItemAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));

            std::string error;
            std::string titleStr;
            if (!TryGetStringParameter(parameters, "title", titleStr, error))
            {
                titleStr = "Updated Draft Item";
            }
            std::string typeStr;
            if (!TryGetStringParameter(parameters, "type", typeStr, error))
            {
                typeStr = "catalogItem";
            }

            PFStringDictionaryEntry titleEntry{};
            titleEntry.key = "NEUTRAL";
            titleEntry.value = titleStr.c_str();

            PFCatalogCatalogItem item{};
            item.title = &titleEntry;
            item.titleCount = 1;
            item.type = typeStr.c_str();
            // Use item ID from previous create if not provided
            std::string itemId;
            if (TryGetStringParameter(parameters, "itemId", itemId, error))
            {
                item.id = itemId.c_str();
            }
            else if (!state->currentScenarioId.empty())
            {
                item.id = state->currentScenarioId.c_str();
            }

            PFCatalogUpdateDraftItemRequest request{};
            request.item = &item;
            const HRESULT hr = PFCatalogUpdateDraftItemAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCatalogUpdateDraftItemAsync (id=%s, hr=0x%08X)", item.id ? item.id : "null", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCatalogUpdateDraftItemGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCatalogUpdateDraftItemResponse* result{ nullptr };
            HRESULT hr = PFCatalogUpdateDraftItemGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCatalogUpdateDraftItemAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCatalogUpdateDraftItemGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogUpdateDraftItemGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCatalogUpdateDraftItemGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCatalogUpdateDraftItemGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFCatalogCreateDraftItemAsync", HandlePFCatalogCreateDraftItemAsync },
    { "PFCatalogCreateDraftItemGetResult", HandlePFCatalogCreateDraftItemGetResult },
    { "PFCatalogCreateDraftItemGetResultSize", HandlePFCatalogCreateDraftItemGetResultSize },
    { "PFCatalogCreateUploadUrlsAsync", HandlePFCatalogCreateUploadUrlsAsync },
    { "PFCatalogCreateUploadUrlsGetResult", HandlePFCatalogCreateUploadUrlsGetResult },
    { "PFCatalogCreateUploadUrlsGetResultSize", HandlePFCatalogCreateUploadUrlsGetResultSize },
    { "PFCatalogDeleteEntityItemReviewsAsync", HandlePFCatalogDeleteEntityItemReviewsAsync },
    { "PFCatalogDeleteItemAsync", HandlePFCatalogDeleteItemAsync },
    { "PFCatalogGetCatalogConfigAsync", HandlePFCatalogGetCatalogConfigAsync },
    { "PFCatalogGetCatalogConfigGetResult", HandlePFCatalogGetCatalogConfigGetResult },
    { "PFCatalogGetCatalogConfigGetResultSize", HandlePFCatalogGetCatalogConfigGetResultSize },
    { "PFCatalogGetDraftItemAsync", HandlePFCatalogGetDraftItemAsync },
    { "PFCatalogGetDraftItemGetResult", HandlePFCatalogGetDraftItemGetResult },
    { "PFCatalogGetDraftItemGetResultSize", HandlePFCatalogGetDraftItemGetResultSize },
    { "PFCatalogGetDraftItemsAsync", HandlePFCatalogGetDraftItemsAsync },
    { "PFCatalogGetDraftItemsGetResult", HandlePFCatalogGetDraftItemsGetResult },
    { "PFCatalogGetDraftItemsGetResultSize", HandlePFCatalogGetDraftItemsGetResultSize },
    { "PFCatalogGetEntityDraftItemsAsync", HandlePFCatalogGetEntityDraftItemsAsync },
    { "PFCatalogGetEntityDraftItemsGetResult", HandlePFCatalogGetEntityDraftItemsGetResult },
    { "PFCatalogGetEntityDraftItemsGetResultSize", HandlePFCatalogGetEntityDraftItemsGetResultSize },
    { "PFCatalogGetEntityItemReviewAsync", HandlePFCatalogGetEntityItemReviewAsync },
    { "PFCatalogGetEntityItemReviewGetResult", HandlePFCatalogGetEntityItemReviewGetResult },
    { "PFCatalogGetEntityItemReviewGetResultSize", HandlePFCatalogGetEntityItemReviewGetResultSize },
    { "PFCatalogGetItemAsync", HandlePFCatalogGetItemAsync },
    { "PFCatalogGetItemContainersAsync", HandlePFCatalogGetItemContainersAsync },
    { "PFCatalogGetItemContainersGetResult", HandlePFCatalogGetItemContainersGetResult },
    { "PFCatalogGetItemContainersGetResultSize", HandlePFCatalogGetItemContainersGetResultSize },
    { "PFCatalogGetItemGetResult", HandlePFCatalogGetItemGetResult },
    { "PFCatalogGetItemGetResultSize", HandlePFCatalogGetItemGetResultSize },
    { "PFCatalogGetItemModerationStateAsync", HandlePFCatalogGetItemModerationStateAsync },
    { "PFCatalogGetItemModerationStateGetResult", HandlePFCatalogGetItemModerationStateGetResult },
    { "PFCatalogGetItemModerationStateGetResultSize", HandlePFCatalogGetItemModerationStateGetResultSize },
    { "PFCatalogGetItemPublishStatusAsync", HandlePFCatalogGetItemPublishStatusAsync },
    { "PFCatalogGetItemPublishStatusGetResult", HandlePFCatalogGetItemPublishStatusGetResult },
    { "PFCatalogGetItemPublishStatusGetResultSize", HandlePFCatalogGetItemPublishStatusGetResultSize },
    { "PFCatalogGetItemReviewSummaryAsync", HandlePFCatalogGetItemReviewSummaryAsync },
    { "PFCatalogGetItemReviewSummaryGetResult", HandlePFCatalogGetItemReviewSummaryGetResult },
    { "PFCatalogGetItemReviewSummaryGetResultSize", HandlePFCatalogGetItemReviewSummaryGetResultSize },
    { "PFCatalogGetItemReviewsAsync", HandlePFCatalogGetItemReviewsAsync },
    { "PFCatalogGetItemReviewsGetResult", HandlePFCatalogGetItemReviewsGetResult },
    { "PFCatalogGetItemReviewsGetResultSize", HandlePFCatalogGetItemReviewsGetResultSize },
    { "PFCatalogGetItemsAsync", HandlePFCatalogGetItemsAsync },
    { "PFCatalogGetItemsGetResult", HandlePFCatalogGetItemsGetResult },
    { "PFCatalogGetItemsGetResultSize", HandlePFCatalogGetItemsGetResultSize },
    { "PFCatalogPublishDraftItemAsync", HandlePFCatalogPublishDraftItemAsync },
    { "PFCatalogReportItemAsync", HandlePFCatalogReportItemAsync },
    { "PFCatalogReportItemReviewAsync", HandlePFCatalogReportItemReviewAsync },
    { "PFCatalogReviewItemAsync", HandlePFCatalogReviewItemAsync },
    { "PFCatalogSearchItemsAsync", HandlePFCatalogSearchItemsAsync },
    { "PFCatalogSearchItemsGetResult", HandlePFCatalogSearchItemsGetResult },
    { "PFCatalogSearchItemsGetResultSize", HandlePFCatalogSearchItemsGetResultSize },
    { "PFCatalogSetItemModerationStateAsync", HandlePFCatalogSetItemModerationStateAsync },
    { "PFCatalogSubmitItemReviewVoteAsync", HandlePFCatalogSubmitItemReviewVoteAsync },
    { "PFCatalogTakedownItemReviewsAsync", HandlePFCatalogTakedownItemReviewsAsync },
    { "PFCatalogUpdateCatalogConfigAsync", HandlePFCatalogUpdateCatalogConfigAsync },
    { "PFCatalogUpdateDraftItemAsync", HandlePFCatalogUpdateDraftItemAsync },
    { "PFCatalogUpdateDraftItemGetResult", HandlePFCatalogUpdateDraftItemGetResult },
    { "PFCatalogUpdateDraftItemGetResultSize", HandlePFCatalogUpdateDraftItemGetResultSize }
});

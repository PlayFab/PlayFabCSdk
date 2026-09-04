#include "pch.h"
#include "PFInventoryHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFInventory.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFInventoryAddInventoryItemsAsync(
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
            PFInventoryAddInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryAddInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryAddInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryAddInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryAddInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryAddInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryAddInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryAddInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryAddInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryAddInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryAddInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryDeleteInventoryCollectionAsync(
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
            PFInventoryDeleteInventoryCollectionRequest request{};
            const HRESULT hr = PFInventoryDeleteInventoryCollectionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryDeleteInventoryCollectionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFInventoryDeleteInventoryItemsAsync(
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
            PFInventoryDeleteInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryDeleteInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryDeleteInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryDeleteInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryDeleteInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryDeleteInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryDeleteInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryDeleteInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryDeleteInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryDeleteInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryDeleteInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryExecuteInventoryOperationsAsync(
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
            
            // Setup request with required fields
            PFInventoryExecuteInventoryOperationsRequest request{};
            
            // Create inventory item reference with test-item ID
            PFInventoryInventoryItemReference itemRef{};
            itemRef.id = "test-item";
            
            // Create add operation with amount = 1
            int32_t amount = 1;
            PFInventoryAddInventoryItemsOperation addOp{};
            addOp.amount = &amount;
            addOp.item = &itemRef;
            
            // Create inventory operation with add member set
            PFInventoryInventoryOperation operation{};
            operation.add = &addOp;
            
            // Set operations array
            const PFInventoryInventoryOperation* operations[] = { &operation };
            request.operations = operations;
            request.operationsCount = 1;
            
            // Set entity key from state
            PFEntityKey entityKey{};
            entityKey.id = state->entityId.c_str();
            entityKey.type = state->entityType.c_str();
            request.entity = &entityKey;
            
            const HRESULT hr = PFInventoryExecuteInventoryOperationsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryExecuteInventoryOperationsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryExecuteInventoryOperationsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryExecuteInventoryOperationsResponse* result{ nullptr };
            HRESULT hr = PFInventoryExecuteInventoryOperationsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryExecuteInventoryOperationsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryExecuteInventoryOperationsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryExecuteInventoryOperationsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryExecuteInventoryOperationsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryExecuteInventoryOperationsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryExecuteTransferOperationsAsync(
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
            PFInventoryExecuteTransferOperationsRequest request{};
            const HRESULT hr = PFInventoryExecuteTransferOperationsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryExecuteTransferOperationsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryExecuteTransferOperationsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryExecuteTransferOperationsResponse* result{ nullptr };
            HRESULT hr = PFInventoryExecuteTransferOperationsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryExecuteTransferOperationsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryExecuteTransferOperationsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryExecuteTransferOperationsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryExecuteTransferOperationsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryExecuteTransferOperationsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryCollectionIdsAsync(
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
            PFInventoryGetInventoryCollectionIdsRequest request{};
            const HRESULT hr = PFInventoryGetInventoryCollectionIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryGetInventoryCollectionIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryGetInventoryCollectionIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryGetInventoryCollectionIdsResponse* result{ nullptr };
            HRESULT hr = PFInventoryGetInventoryCollectionIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryGetInventoryCollectionIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryCollectionIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryCollectionIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryCollectionIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryCollectionIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryItemsAsync(
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
            PFInventoryGetInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryGetInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryGetInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryGetInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryGetInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryGetInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryGetInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryOperationStatusAsync(
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
            PFInventoryGetInventoryOperationStatusRequest request{};
            const HRESULT hr = PFInventoryGetInventoryOperationStatusAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryGetInventoryOperationStatusAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryGetInventoryOperationStatusGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryGetInventoryOperationStatusResponse* result{ nullptr };
            HRESULT hr = PFInventoryGetInventoryOperationStatusGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryGetInventoryOperationStatusAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryOperationStatusGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryOperationStatusGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetInventoryOperationStatusGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetInventoryOperationStatusGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetTransactionHistoryAsync(
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
            PFInventoryGetTransactionHistoryRequest request{};
            const HRESULT hr = PFInventoryGetTransactionHistoryAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryGetTransactionHistoryAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryGetTransactionHistoryGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryGetTransactionHistoryResponse* result{ nullptr };
            HRESULT hr = PFInventoryGetTransactionHistoryGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryGetTransactionHistoryAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryGetTransactionHistoryGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetTransactionHistoryGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryGetTransactionHistoryGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryGetTransactionHistoryGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryPurchaseInventoryItemsAsync(
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
            PFInventoryPurchaseInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryPurchaseInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryPurchaseInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryPurchaseInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryPurchaseInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryPurchaseInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryPurchaseInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryPurchaseInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryPurchaseInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryPurchaseInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryPurchaseInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemAppleAppStoreInventoryItemsAsync(
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
            PFInventoryRedeemAppleAppStoreInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemAppleAppStoreInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemAppleAppStoreInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemAppleAppStoreInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemAppleAppStoreInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemAppleAppStoreInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemAppleAppStoreInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemAppleAppStoreInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemGooglePlayInventoryItemsAsync(
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
            PFInventoryRedeemGooglePlayInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemGooglePlayInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemGooglePlayInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemGooglePlayInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemGooglePlayInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemGooglePlayInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemGooglePlayInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemGooglePlayInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemGooglePlayInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemGooglePlayInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemGooglePlayInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemMicrosoftStoreInventoryItemsAsync(
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
            PFInventoryRedeemMicrosoftStoreInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemMicrosoftStoreInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemMicrosoftStoreInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemMicrosoftStoreInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemMicrosoftStoreInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemMicrosoftStoreInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemMicrosoftStoreInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemMicrosoftStoreInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemNintendoEShopInventoryItemsAsync(
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
            PFInventoryRedeemNintendoEShopInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemNintendoEShopInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemNintendoEShopInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemNintendoEShopInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemNintendoEShopInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemNintendoEShopInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemNintendoEShopInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemNintendoEShopInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemNintendoEShopInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemNintendoEShopInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemNintendoEShopInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemPlayStationStoreInventoryItemsAsync(
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
            PFInventoryRedeemPlayStationStoreInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemPlayStationStoreInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemPlayStationStoreInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemPlayStationStoreInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemPlayStationStoreInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemPlayStationStoreInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemPlayStationStoreInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemPlayStationStoreInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemSteamInventoryItemsAsync(
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
            PFInventoryRedeemSteamInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryRedeemSteamInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryRedeemSteamInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryRedeemSteamInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryRedeemSteamInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryRedeemSteamInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryRedeemSteamInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryRedeemSteamInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemSteamInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryRedeemSteamInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryRedeemSteamInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventorySubtractInventoryItemsAsync(
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
            PFInventorySubtractInventoryItemsRequest request{};
            const HRESULT hr = PFInventorySubtractInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventorySubtractInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventorySubtractInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventorySubtractInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventorySubtractInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventorySubtractInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventorySubtractInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventorySubtractInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventorySubtractInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventorySubtractInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryTransferInventoryItemsAsync(
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
            PFInventoryTransferInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryTransferInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryTransferInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryTransferInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryTransferInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryTransferInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryTransferInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryTransferInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryTransferInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryTransferInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryTransferInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryUpdateInventoryItemsAsync(
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
            PFInventoryUpdateInventoryItemsRequest request{};
            const HRESULT hr = PFInventoryUpdateInventoryItemsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFInventoryUpdateInventoryItemsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFInventoryUpdateInventoryItemsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFInventoryUpdateInventoryItemsResponse* result{ nullptr };
            HRESULT hr = PFInventoryUpdateInventoryItemsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFInventoryUpdateInventoryItemsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFInventoryUpdateInventoryItemsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryUpdateInventoryItemsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFInventoryUpdateInventoryItemsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFInventoryUpdateInventoryItemsGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFInventoryAddInventoryItemsAsync", HandlePFInventoryAddInventoryItemsAsync },
    { "PFInventoryAddInventoryItemsGetResult", HandlePFInventoryAddInventoryItemsGetResult },
    { "PFInventoryAddInventoryItemsGetResultSize", HandlePFInventoryAddInventoryItemsGetResultSize },
    { "PFInventoryDeleteInventoryCollectionAsync", HandlePFInventoryDeleteInventoryCollectionAsync },
    { "PFInventoryDeleteInventoryItemsAsync", HandlePFInventoryDeleteInventoryItemsAsync },
    { "PFInventoryDeleteInventoryItemsGetResult", HandlePFInventoryDeleteInventoryItemsGetResult },
    { "PFInventoryDeleteInventoryItemsGetResultSize", HandlePFInventoryDeleteInventoryItemsGetResultSize },
    { "PFInventoryExecuteInventoryOperationsAsync", HandlePFInventoryExecuteInventoryOperationsAsync },
    { "PFInventoryExecuteInventoryOperationsGetResult", HandlePFInventoryExecuteInventoryOperationsGetResult },
    { "PFInventoryExecuteInventoryOperationsGetResultSize", HandlePFInventoryExecuteInventoryOperationsGetResultSize },
    { "PFInventoryExecuteTransferOperationsAsync", HandlePFInventoryExecuteTransferOperationsAsync },
    { "PFInventoryExecuteTransferOperationsGetResult", HandlePFInventoryExecuteTransferOperationsGetResult },
    { "PFInventoryExecuteTransferOperationsGetResultSize", HandlePFInventoryExecuteTransferOperationsGetResultSize },
    { "PFInventoryGetInventoryCollectionIdsAsync", HandlePFInventoryGetInventoryCollectionIdsAsync },
    { "PFInventoryGetInventoryCollectionIdsGetResult", HandlePFInventoryGetInventoryCollectionIdsGetResult },
    { "PFInventoryGetInventoryCollectionIdsGetResultSize", HandlePFInventoryGetInventoryCollectionIdsGetResultSize },
    { "PFInventoryGetInventoryItemsAsync", HandlePFInventoryGetInventoryItemsAsync },
    { "PFInventoryGetInventoryItemsGetResult", HandlePFInventoryGetInventoryItemsGetResult },
    { "PFInventoryGetInventoryItemsGetResultSize", HandlePFInventoryGetInventoryItemsGetResultSize },
    { "PFInventoryGetInventoryOperationStatusAsync", HandlePFInventoryGetInventoryOperationStatusAsync },
    { "PFInventoryGetInventoryOperationStatusGetResult", HandlePFInventoryGetInventoryOperationStatusGetResult },
    { "PFInventoryGetInventoryOperationStatusGetResultSize", HandlePFInventoryGetInventoryOperationStatusGetResultSize },
    { "PFInventoryGetTransactionHistoryAsync", HandlePFInventoryGetTransactionHistoryAsync },
    { "PFInventoryGetTransactionHistoryGetResult", HandlePFInventoryGetTransactionHistoryGetResult },
    { "PFInventoryGetTransactionHistoryGetResultSize", HandlePFInventoryGetTransactionHistoryGetResultSize },
    { "PFInventoryPurchaseInventoryItemsAsync", HandlePFInventoryPurchaseInventoryItemsAsync },
    { "PFInventoryPurchaseInventoryItemsGetResult", HandlePFInventoryPurchaseInventoryItemsGetResult },
    { "PFInventoryPurchaseInventoryItemsGetResultSize", HandlePFInventoryPurchaseInventoryItemsGetResultSize },
    { "PFInventoryRedeemAppleAppStoreInventoryItemsAsync", HandlePFInventoryRedeemAppleAppStoreInventoryItemsAsync },
    { "PFInventoryRedeemAppleAppStoreInventoryItemsGetResult", HandlePFInventoryRedeemAppleAppStoreInventoryItemsGetResult },
    { "PFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize", HandlePFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize },
    { "PFInventoryRedeemGooglePlayInventoryItemsAsync", HandlePFInventoryRedeemGooglePlayInventoryItemsAsync },
    { "PFInventoryRedeemGooglePlayInventoryItemsGetResult", HandlePFInventoryRedeemGooglePlayInventoryItemsGetResult },
    { "PFInventoryRedeemGooglePlayInventoryItemsGetResultSize", HandlePFInventoryRedeemGooglePlayInventoryItemsGetResultSize },
    { "PFInventoryRedeemMicrosoftStoreInventoryItemsAsync", HandlePFInventoryRedeemMicrosoftStoreInventoryItemsAsync },
    { "PFInventoryRedeemMicrosoftStoreInventoryItemsGetResult", HandlePFInventoryRedeemMicrosoftStoreInventoryItemsGetResult },
    { "PFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize", HandlePFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize },
    { "PFInventoryRedeemNintendoEShopInventoryItemsAsync", HandlePFInventoryRedeemNintendoEShopInventoryItemsAsync },
    { "PFInventoryRedeemNintendoEShopInventoryItemsGetResult", HandlePFInventoryRedeemNintendoEShopInventoryItemsGetResult },
    { "PFInventoryRedeemNintendoEShopInventoryItemsGetResultSize", HandlePFInventoryRedeemNintendoEShopInventoryItemsGetResultSize },
    { "PFInventoryRedeemPlayStationStoreInventoryItemsAsync", HandlePFInventoryRedeemPlayStationStoreInventoryItemsAsync },
    { "PFInventoryRedeemPlayStationStoreInventoryItemsGetResult", HandlePFInventoryRedeemPlayStationStoreInventoryItemsGetResult },
    { "PFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize", HandlePFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize },
    { "PFInventoryRedeemSteamInventoryItemsAsync", HandlePFInventoryRedeemSteamInventoryItemsAsync },
    { "PFInventoryRedeemSteamInventoryItemsGetResult", HandlePFInventoryRedeemSteamInventoryItemsGetResult },
    { "PFInventoryRedeemSteamInventoryItemsGetResultSize", HandlePFInventoryRedeemSteamInventoryItemsGetResultSize },
    { "PFInventorySubtractInventoryItemsAsync", HandlePFInventorySubtractInventoryItemsAsync },
    { "PFInventorySubtractInventoryItemsGetResult", HandlePFInventorySubtractInventoryItemsGetResult },
    { "PFInventorySubtractInventoryItemsGetResultSize", HandlePFInventorySubtractInventoryItemsGetResultSize },
    { "PFInventoryTransferInventoryItemsAsync", HandlePFInventoryTransferInventoryItemsAsync },
    { "PFInventoryTransferInventoryItemsGetResult", HandlePFInventoryTransferInventoryItemsGetResult },
    { "PFInventoryTransferInventoryItemsGetResultSize", HandlePFInventoryTransferInventoryItemsGetResultSize },
    { "PFInventoryUpdateInventoryItemsAsync", HandlePFInventoryUpdateInventoryItemsAsync },
    { "PFInventoryUpdateInventoryItemsGetResult", HandlePFInventoryUpdateInventoryItemsGetResult },
    { "PFInventoryUpdateInventoryItemsGetResultSize", HandlePFInventoryUpdateInventoryItemsGetResultSize }
});

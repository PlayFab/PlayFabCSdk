#include "stdafx.h"
#include "InventoryTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Inventory
{

JsonValue AlternateId::ToJson() const
{
    return AlternateId::ToJson(this->Model());
}

JsonValue AlternateId::ToJson(const PFInventoryAlternateId& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

JsonValue InventoryItemReference::ToJson() const
{
    return InventoryItemReference::ToJson(this->Model());
}

JsonValue InventoryItemReference::ToJson(const PFInventoryInventoryItemReference& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember<AlternateId>(output, "AlternateId", input.alternateId);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "StackId", input.stackId);
    return output;
}

JsonValue InitialValues::ToJson() const
{
    return InitialValues::ToJson(this->Model());
}

JsonValue InitialValues::ToJson(const PFInventoryInitialValues& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "DisplayProperties", input.displayProperties);
    return output;
}

JsonValue AddInventoryItemsRequest::ToJson() const
{
    return AddInventoryItemsRequest::ToJson(this->Model());
}

JsonValue AddInventoryItemsRequest::ToJson(const PFInventoryAddInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    return output;
}

HRESULT AddInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t AddInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryAddInventoryItemsResponse const*> AddInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AddInventoryItemsResponse>(&this->Model());
}

size_t AddInventoryItemsResponse::RequiredBufferSize(const PFInventoryAddInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT AddInventoryItemsResponse::Copy(const PFInventoryAddInventoryItemsResponse& input, PFInventoryAddInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeleteInventoryCollectionRequest::ToJson() const
{
    return DeleteInventoryCollectionRequest::ToJson(this->Model());
}

JsonValue DeleteInventoryCollectionRequest::ToJson(const PFInventoryDeleteInventoryCollectionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    return output;
}

JsonValue DeleteInventoryItemsRequest::ToJson() const
{
    return DeleteInventoryItemsRequest::ToJson(this->Model());
}

JsonValue DeleteInventoryItemsRequest::ToJson(const PFInventoryDeleteInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    return output;
}

HRESULT DeleteInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t DeleteInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryDeleteInventoryItemsResponse const*> DeleteInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeleteInventoryItemsResponse>(&this->Model());
}

size_t DeleteInventoryItemsResponse::RequiredBufferSize(const PFInventoryDeleteInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT DeleteInventoryItemsResponse::Copy(const PFInventoryDeleteInventoryItemsResponse& input, PFInventoryDeleteInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue AddInventoryItemsOperation::ToJson() const
{
    return AddInventoryItemsOperation::ToJson(this->Model());
}

JsonValue AddInventoryItemsOperation::ToJson(const PFInventoryAddInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    return output;
}

JsonValue DeleteInventoryItemsOperation::ToJson() const
{
    return DeleteInventoryItemsOperation::ToJson(this->Model());
}

JsonValue DeleteInventoryItemsOperation::ToJson(const PFInventoryDeleteInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    return output;
}

JsonValue PurchasePriceAmount::ToJson() const
{
    return PurchasePriceAmount::ToJson(this->Model());
}

JsonValue PurchasePriceAmount::ToJson(const PFInventoryPurchasePriceAmount& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    JsonUtils::ObjectAddMember(output, "StackId", input.stackId);
    return output;
}

JsonValue PurchaseInventoryItemsOperation::ToJson() const
{
    return PurchaseInventoryItemsOperation::ToJson(this->Model());
}

JsonValue PurchaseInventoryItemsOperation::ToJson(const PFInventoryPurchaseInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    JsonUtils::ObjectAddMemberArray<PurchasePriceAmount>(output, "PriceAmounts", input.priceAmounts, input.priceAmountsCount);
    JsonUtils::ObjectAddMember(output, "StoreId", input.storeId);
    return output;
}

JsonValue SubtractInventoryItemsOperation::ToJson() const
{
    return SubtractInventoryItemsOperation::ToJson(this->Model());
}

JsonValue SubtractInventoryItemsOperation::ToJson(const PFInventorySubtractInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    return output;
}

JsonValue TransferInventoryItemsOperation::ToJson() const
{
    return TransferInventoryItemsOperation::ToJson(this->Model());
}

JsonValue TransferInventoryItemsOperation::ToJson(const PFInventoryTransferInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "GivingItem", input.givingItem);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "ReceivingItem", input.receivingItem);
    return output;
}

JsonValue InventoryItem::ToJson() const
{
    return InventoryItem::ToJson(this->Model());
}

JsonValue InventoryItem::ToJson(const PFInventoryInventoryItem& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "DisplayProperties", input.displayProperties);
    JsonUtils::ObjectAddMemberTime(output, "ExpirationDate", input.expirationDate);
    JsonUtils::ObjectAddMember(output, "Id", input.id);
    JsonUtils::ObjectAddMember(output, "StackId", input.stackId);
    JsonUtils::ObjectAddMember(output, "Type", input.type);
    return output;
}

HRESULT InventoryItem::FromJson(const JsonValue& input)
{
    std::optional<int32_t> amount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Amount", amount));
    this->SetAmount(std::move(amount));

    JsonObject displayProperties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayProperties", displayProperties));
    this->SetDisplayProperties(std::move(displayProperties));

    std::optional<time_t> expirationDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "ExpirationDate", expirationDate));
    this->SetExpirationDate(std::move(expirationDate));

    String id{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Id", id));
    this->SetId(std::move(id));

    String stackId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StackId", stackId));
    this->SetStackId(std::move(stackId));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t InventoryItem::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryInventoryItem const*> InventoryItem::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InventoryItem>(&this->Model());
}

size_t InventoryItem::RequiredBufferSize(const PFInventoryInventoryItem& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.amount)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.displayProperties.stringValue)
    {
    requiredSize += (std::strlen(model.displayProperties.stringValue) + 1);
    }
    if (model.expirationDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.id)
    {
        requiredSize += (std::strlen(model.id) + 1);
    }
    if (model.stackId)
    {
        requiredSize += (std::strlen(model.stackId) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    return requiredSize;
}

HRESULT InventoryItem::Copy(const PFInventoryInventoryItem& input, PFInventoryInventoryItem& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.amount); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.amount = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.displayProperties.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayProperties.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.expirationDate); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.expirationDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.id); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.id = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.stackId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.stackId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateInventoryItemsOperation::ToJson() const
{
    return UpdateInventoryItemsOperation::ToJson(this->Model());
}

JsonValue UpdateInventoryItemsOperation::ToJson(const PFInventoryUpdateInventoryItemsOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember<InventoryItem>(output, "Item", input.item);
    return output;
}

JsonValue InventoryOperation::ToJson() const
{
    return InventoryOperation::ToJson(this->Model());
}

JsonValue InventoryOperation::ToJson(const PFInventoryInventoryOperation& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember<AddInventoryItemsOperation>(output, "Add", input.add);
    JsonUtils::ObjectAddMember<DeleteInventoryItemsOperation>(output, "Delete", input.deleteOp);
    JsonUtils::ObjectAddMember<PurchaseInventoryItemsOperation>(output, "Purchase", input.purchase);
    JsonUtils::ObjectAddMember<SubtractInventoryItemsOperation>(output, "Subtract", input.subtract);
    JsonUtils::ObjectAddMember<TransferInventoryItemsOperation>(output, "Transfer", input.transfer);
    JsonUtils::ObjectAddMember<UpdateInventoryItemsOperation>(output, "Update", input.update);
    return output;
}

JsonValue ExecuteInventoryOperationsRequest::ToJson() const
{
    return ExecuteInventoryOperationsRequest::ToJson(this->Model());
}

JsonValue ExecuteInventoryOperationsRequest::ToJson(const PFInventoryExecuteInventoryOperationsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMemberArray<InventoryOperation>(output, "Operations", input.operations, input.operationsCount);
    return output;
}

HRESULT ExecuteInventoryOperationsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t ExecuteInventoryOperationsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryExecuteInventoryOperationsResponse const*> ExecuteInventoryOperationsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ExecuteInventoryOperationsResponse>(&this->Model());
}

size_t ExecuteInventoryOperationsResponse::RequiredBufferSize(const PFInventoryExecuteInventoryOperationsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT ExecuteInventoryOperationsResponse::Copy(const PFInventoryExecuteInventoryOperationsResponse& input, PFInventoryExecuteInventoryOperationsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetInventoryCollectionIdsRequest::ToJson() const
{
    return GetInventoryCollectionIdsRequest::ToJson(this->Model());
}

JsonValue GetInventoryCollectionIdsRequest::ToJson(const PFInventoryGetInventoryCollectionIdsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetInventoryCollectionIdsResponse::FromJson(const JsonValue& input)
{
    CStringVector collectionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CollectionIds", collectionIds));
    this->SetCollectionIds(std::move(collectionIds));

    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    return S_OK;
}

size_t GetInventoryCollectionIdsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryGetInventoryCollectionIdsResponse const*> GetInventoryCollectionIdsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetInventoryCollectionIdsResponse>(&this->Model());
}

size_t GetInventoryCollectionIdsResponse::RequiredBufferSize(const PFInventoryGetInventoryCollectionIdsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.collectionIdsCount);
    for (size_t i = 0; i < model.collectionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.collectionIds[i]) + 1);
    }
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    return requiredSize;
}

HRESULT GetInventoryCollectionIdsResponse::Copy(const PFInventoryGetInventoryCollectionIdsResponse& input, PFInventoryGetInventoryCollectionIdsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.collectionIds, input.collectionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.collectionIds = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetInventoryItemsRequest::ToJson() const
{
    return GetInventoryItemsRequest::ToJson(this->Model());
}

JsonValue GetInventoryItemsRequest::ToJson(const PFInventoryGetInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Filter", input.filter);
    return output;
}

HRESULT GetInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    ModelVector<InventoryItem> items{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<InventoryItem>(input, "Items", items));
    this->SetItems(std::move(items));

    return S_OK;
}

size_t GetInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryGetInventoryItemsResponse const*> GetInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetInventoryItemsResponse>(&this->Model());
}

size_t GetInventoryItemsResponse::RequiredBufferSize(const PFInventoryGetInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    requiredSize += (alignof(PFInventoryInventoryItem*) + sizeof(PFInventoryInventoryItem*) * model.itemsCount);
    for (size_t i = 0; i < model.itemsCount; ++i)
    {
        requiredSize += InventoryItem::RequiredBufferSize(*model.items[i]);
    }
    return requiredSize;
}

HRESULT GetInventoryItemsResponse::Copy(const PFInventoryGetInventoryItemsResponse& input, PFInventoryGetInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<InventoryItem>(input.items, input.itemsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.items = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetMicrosoftStoreAccessTokensRequest::ToJson() const
{
    return GetMicrosoftStoreAccessTokensRequest::ToJson(this->Model());
}

JsonValue GetMicrosoftStoreAccessTokensRequest::ToJson(const PFInventoryGetMicrosoftStoreAccessTokensRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

HRESULT GetMicrosoftStoreAccessTokensResponse::FromJson(const JsonValue& input)
{
    String collectionsAccessToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CollectionsAccessToken", collectionsAccessToken));
    this->SetCollectionsAccessToken(std::move(collectionsAccessToken));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "CollectionsAccessTokenExpirationDate", this->m_model.collectionsAccessTokenExpirationDate));

    return S_OK;
}

size_t GetMicrosoftStoreAccessTokensResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryGetMicrosoftStoreAccessTokensResponse const*> GetMicrosoftStoreAccessTokensResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetMicrosoftStoreAccessTokensResponse>(&this->Model());
}

size_t GetMicrosoftStoreAccessTokensResponse::RequiredBufferSize(const PFInventoryGetMicrosoftStoreAccessTokensResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.collectionsAccessToken)
    {
        requiredSize += (std::strlen(model.collectionsAccessToken) + 1);
    }
    return requiredSize;
}

HRESULT GetMicrosoftStoreAccessTokensResponse::Copy(const PFInventoryGetMicrosoftStoreAccessTokensResponse& input, PFInventoryGetMicrosoftStoreAccessTokensResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.collectionsAccessToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.collectionsAccessToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetTransactionHistoryRequest::ToJson() const
{
    return GetTransactionHistoryRequest::ToJson(this->Model());
}

JsonValue GetTransactionHistoryRequest::ToJson(const PFInventoryGetTransactionHistoryRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMember(output, "ContinuationToken", input.continuationToken);
    JsonUtils::ObjectAddMember(output, "Count", input.count);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Filter", input.filter);
    return output;
}

HRESULT TransactionOperation::FromJson(const JsonValue& input)
{
    std::optional<int32_t> amount{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Amount", amount));
    this->SetAmount(std::move(amount));

    std::optional<double> durationInSeconds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DurationInSeconds", durationInSeconds));
    this->SetDurationInSeconds(std::move(durationInSeconds));

    String itemId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemId", itemId));
    this->SetItemId(std::move(itemId));

    String itemType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemType", itemType));
    this->SetItemType(std::move(itemType));

    String stackId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StackId", stackId));
    this->SetStackId(std::move(stackId));

    String type{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Type", type));
    this->SetType(std::move(type));

    return S_OK;
}

size_t TransactionOperation::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransactionOperation const*> TransactionOperation::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TransactionOperation>(&this->Model());
}

size_t TransactionOperation::RequiredBufferSize(const PFInventoryTransactionOperation& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.amount)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.durationInSeconds)
    {
        requiredSize += (alignof(double) + sizeof(double));
    }
    if (model.itemId)
    {
        requiredSize += (std::strlen(model.itemId) + 1);
    }
    if (model.itemType)
    {
        requiredSize += (std::strlen(model.itemType) + 1);
    }
    if (model.stackId)
    {
        requiredSize += (std::strlen(model.stackId) + 1);
    }
    if (model.type)
    {
        requiredSize += (std::strlen(model.type) + 1);
    }
    return requiredSize;
}

HRESULT TransactionOperation::Copy(const PFInventoryTransactionOperation& input, PFInventoryTransactionOperation& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.amount); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.amount = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.durationInSeconds); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.durationInSeconds = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.stackId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.stackId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.type); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.type = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT TransactionPurchaseDetails::FromJson(const JsonValue& input)
{
    String storeId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StoreId", storeId));
    this->SetStoreId(std::move(storeId));

    return S_OK;
}

size_t TransactionPurchaseDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransactionPurchaseDetails const*> TransactionPurchaseDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TransactionPurchaseDetails>(&this->Model());
}

size_t TransactionPurchaseDetails::RequiredBufferSize(const PFInventoryTransactionPurchaseDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.storeId)
    {
        requiredSize += (std::strlen(model.storeId) + 1);
    }
    return requiredSize;
}

HRESULT TransactionPurchaseDetails::Copy(const PFInventoryTransactionPurchaseDetails& input, PFInventoryTransactionPurchaseDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.storeId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.storeId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT TransactionRedeemDetails::FromJson(const JsonValue& input)
{
    String marketplace{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Marketplace", marketplace));
    this->SetMarketplace(std::move(marketplace));

    String marketplaceTransactionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MarketplaceTransactionId", marketplaceTransactionId));
    this->SetMarketplaceTransactionId(std::move(marketplaceTransactionId));

    String offerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OfferId", offerId));
    this->SetOfferId(std::move(offerId));

    return S_OK;
}

size_t TransactionRedeemDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransactionRedeemDetails const*> TransactionRedeemDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TransactionRedeemDetails>(&this->Model());
}

size_t TransactionRedeemDetails::RequiredBufferSize(const PFInventoryTransactionRedeemDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.marketplace)
    {
        requiredSize += (std::strlen(model.marketplace) + 1);
    }
    if (model.marketplaceTransactionId)
    {
        requiredSize += (std::strlen(model.marketplaceTransactionId) + 1);
    }
    if (model.offerId)
    {
        requiredSize += (std::strlen(model.offerId) + 1);
    }
    return requiredSize;
}

HRESULT TransactionRedeemDetails::Copy(const PFInventoryTransactionRedeemDetails& input, PFInventoryTransactionRedeemDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.marketplace); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.marketplace = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.marketplaceTransactionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.marketplaceTransactionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.offerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.offerId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT TransactionTransferDetails::FromJson(const JsonValue& input)
{
    String givingCollectionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GivingCollectionId", givingCollectionId));
    this->SetGivingCollectionId(std::move(givingCollectionId));

    std::optional<EntityKey> givingEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GivingEntity", givingEntity));
    if (givingEntity)
    {
        this->SetGivingEntity(std::move(*givingEntity));
    }

    String receivingCollectionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReceivingCollectionId", receivingCollectionId));
    this->SetReceivingCollectionId(std::move(receivingCollectionId));

    std::optional<EntityKey> receivingEntity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReceivingEntity", receivingEntity));
    if (receivingEntity)
    {
        this->SetReceivingEntity(std::move(*receivingEntity));
    }

    String transferId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransferId", transferId));
    this->SetTransferId(std::move(transferId));

    return S_OK;
}

size_t TransactionTransferDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransactionTransferDetails const*> TransactionTransferDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TransactionTransferDetails>(&this->Model());
}

size_t TransactionTransferDetails::RequiredBufferSize(const PFInventoryTransactionTransferDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.givingCollectionId)
    {
        requiredSize += (std::strlen(model.givingCollectionId) + 1);
    }
    if (model.givingEntity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.givingEntity);
    }
    if (model.receivingCollectionId)
    {
        requiredSize += (std::strlen(model.receivingCollectionId) + 1);
    }
    if (model.receivingEntity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.receivingEntity);
    }
    if (model.transferId)
    {
        requiredSize += (std::strlen(model.transferId) + 1);
    }
    return requiredSize;
}

HRESULT TransactionTransferDetails::Copy(const PFInventoryTransactionTransferDetails& input, PFInventoryTransactionTransferDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.givingCollectionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.givingCollectionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.givingEntity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.givingEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.receivingCollectionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.receivingCollectionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.receivingEntity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.receivingEntity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.transferId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transferId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT Transaction::FromJson(const JsonValue& input)
{
    String apiName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ApiName", apiName));
    this->SetApiName(std::move(apiName));

    String itemType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ItemType", itemType));
    this->SetItemType(std::move(itemType));

    ModelVector<TransactionOperation> operations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<TransactionOperation>(input, "Operations", operations));
    this->SetOperations(std::move(operations));

    String operationType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OperationType", operationType));
    this->SetOperationType(std::move(operationType));

    std::optional<TransactionPurchaseDetails> purchaseDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PurchaseDetails", purchaseDetails));
    if (purchaseDetails)
    {
        this->SetPurchaseDetails(std::move(*purchaseDetails));
    }

    std::optional<TransactionRedeemDetails> redeemDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RedeemDetails", redeemDetails));
    if (redeemDetails)
    {
        this->SetRedeemDetails(std::move(*redeemDetails));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Timestamp", this->m_model.timestamp));

    String transactionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionId", transactionId));
    this->SetTransactionId(std::move(transactionId));

    std::optional<TransactionTransferDetails> transferDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransferDetails", transferDetails));
    if (transferDetails)
    {
        this->SetTransferDetails(std::move(*transferDetails));
    }

    return S_OK;
}

size_t Transaction::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransaction const*> Transaction::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Transaction>(&this->Model());
}

size_t Transaction::RequiredBufferSize(const PFInventoryTransaction& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.apiName)
    {
        requiredSize += (std::strlen(model.apiName) + 1);
    }
    if (model.itemType)
    {
        requiredSize += (std::strlen(model.itemType) + 1);
    }
    requiredSize += (alignof(PFInventoryTransactionOperation*) + sizeof(PFInventoryTransactionOperation*) * model.operationsCount);
    for (size_t i = 0; i < model.operationsCount; ++i)
    {
        requiredSize += TransactionOperation::RequiredBufferSize(*model.operations[i]);
    }
    if (model.operationType)
    {
        requiredSize += (std::strlen(model.operationType) + 1);
    }
    if (model.purchaseDetails)
    {
        requiredSize += TransactionPurchaseDetails::RequiredBufferSize(*model.purchaseDetails);
    }
    if (model.redeemDetails)
    {
        requiredSize += TransactionRedeemDetails::RequiredBufferSize(*model.redeemDetails);
    }
    if (model.transactionId)
    {
        requiredSize += (std::strlen(model.transactionId) + 1);
    }
    if (model.transferDetails)
    {
        requiredSize += TransactionTransferDetails::RequiredBufferSize(*model.transferDetails);
    }
    return requiredSize;
}

HRESULT Transaction::Copy(const PFInventoryTransaction& input, PFInventoryTransaction& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.apiName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.apiName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.itemType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.itemType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<TransactionOperation>(input.operations, input.operationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operations = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.operationType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operationType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<TransactionPurchaseDetails>(input.purchaseDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.purchaseDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<TransactionRedeemDetails>(input.redeemDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.redeemDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.transactionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<TransactionTransferDetails>(input.transferDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transferDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetTransactionHistoryResponse::FromJson(const JsonValue& input)
{
    String continuationToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ContinuationToken", continuationToken));
    this->SetContinuationToken(std::move(continuationToken));

    ModelVector<Transaction> transactions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Transaction>(input, "Transactions", transactions));
    this->SetTransactions(std::move(transactions));

    return S_OK;
}

size_t GetTransactionHistoryResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryGetTransactionHistoryResponse const*> GetTransactionHistoryResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTransactionHistoryResponse>(&this->Model());
}

size_t GetTransactionHistoryResponse::RequiredBufferSize(const PFInventoryGetTransactionHistoryResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.continuationToken)
    {
        requiredSize += (std::strlen(model.continuationToken) + 1);
    }
    requiredSize += (alignof(PFInventoryTransaction*) + sizeof(PFInventoryTransaction*) * model.transactionsCount);
    for (size_t i = 0; i < model.transactionsCount; ++i)
    {
        requiredSize += Transaction::RequiredBufferSize(*model.transactions[i]);
    }
    return requiredSize;
}

HRESULT GetTransactionHistoryResponse::Copy(const PFInventoryGetTransactionHistoryResponse& input, PFInventoryGetTransactionHistoryResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.continuationToken); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.continuationToken = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<Transaction>(input.transactions, input.transactionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue PurchaseInventoryItemsRequest::ToJson() const
{
    return PurchaseInventoryItemsRequest::ToJson(this->Model());
}

JsonValue PurchaseInventoryItemsRequest::ToJson(const PFInventoryPurchaseInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    JsonUtils::ObjectAddMemberArray<PurchasePriceAmount>(output, "PriceAmounts", input.priceAmounts, input.priceAmountsCount);
    JsonUtils::ObjectAddMember(output, "StoreId", input.storeId);
    return output;
}

HRESULT PurchaseInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t PurchaseInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryPurchaseInventoryItemsResponse const*> PurchaseInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PurchaseInventoryItemsResponse>(&this->Model());
}

size_t PurchaseInventoryItemsResponse::RequiredBufferSize(const PFInventoryPurchaseInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT PurchaseInventoryItemsResponse::Copy(const PFInventoryPurchaseInventoryItemsResponse& input, PFInventoryPurchaseInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RedeemAppleAppStoreInventoryItemsRequest::ToJson() const
{
    return RedeemAppleAppStoreInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemAppleAppStoreInventoryItemsRequest::ToJson(const PFInventoryRedeemAppleAppStoreInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Receipt", input.receipt);
    return output;
}

HRESULT RedemptionFailure::FromJson(const JsonValue& input)
{
    String failureCode{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FailureCode", failureCode));
    this->SetFailureCode(std::move(failureCode));

    String failureDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FailureDetails", failureDetails));
    this->SetFailureDetails(std::move(failureDetails));

    String marketplaceTransactionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MarketplaceTransactionId", marketplaceTransactionId));
    this->SetMarketplaceTransactionId(std::move(marketplaceTransactionId));

    String offerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OfferId", offerId));
    this->SetOfferId(std::move(offerId));

    return S_OK;
}

size_t RedemptionFailure::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedemptionFailure const*> RedemptionFailure::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedemptionFailure>(&this->Model());
}

size_t RedemptionFailure::RequiredBufferSize(const PFInventoryRedemptionFailure& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.failureCode)
    {
        requiredSize += (std::strlen(model.failureCode) + 1);
    }
    if (model.failureDetails)
    {
        requiredSize += (std::strlen(model.failureDetails) + 1);
    }
    if (model.marketplaceTransactionId)
    {
        requiredSize += (std::strlen(model.marketplaceTransactionId) + 1);
    }
    if (model.offerId)
    {
        requiredSize += (std::strlen(model.offerId) + 1);
    }
    return requiredSize;
}

HRESULT RedemptionFailure::Copy(const PFInventoryRedemptionFailure& input, PFInventoryRedemptionFailure& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.failureCode); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failureCode = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.failureDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failureDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.marketplaceTransactionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.marketplaceTransactionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.offerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.offerId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT RedemptionSuccess::FromJson(const JsonValue& input)
{
    String marketplaceTransactionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MarketplaceTransactionId", marketplaceTransactionId));
    this->SetMarketplaceTransactionId(std::move(marketplaceTransactionId));

    String offerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OfferId", offerId));
    this->SetOfferId(std::move(offerId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "SuccessTimestamp", this->m_model.successTimestamp));

    return S_OK;
}

size_t RedemptionSuccess::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedemptionSuccess const*> RedemptionSuccess::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedemptionSuccess>(&this->Model());
}

size_t RedemptionSuccess::RequiredBufferSize(const PFInventoryRedemptionSuccess& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.marketplaceTransactionId)
    {
        requiredSize += (std::strlen(model.marketplaceTransactionId) + 1);
    }
    if (model.offerId)
    {
        requiredSize += (std::strlen(model.offerId) + 1);
    }
    return requiredSize;
}

HRESULT RedemptionSuccess::Copy(const PFInventoryRedemptionSuccess& input, PFInventoryRedemptionSuccess& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.marketplaceTransactionId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.marketplaceTransactionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.offerId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.offerId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT RedeemAppleAppStoreInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemAppleAppStoreInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemAppleAppStoreInventoryItemsResponse const*> RedeemAppleAppStoreInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemAppleAppStoreInventoryItemsResponse>(&this->Model());
}

size_t RedeemAppleAppStoreInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemAppleAppStoreInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemAppleAppStoreInventoryItemsResponse::Copy(const PFInventoryRedeemAppleAppStoreInventoryItemsResponse& input, PFInventoryRedeemAppleAppStoreInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GooglePlayProductPurchase::ToJson() const
{
    return GooglePlayProductPurchase::ToJson(this->Model());
}

JsonValue GooglePlayProductPurchase::ToJson(const PFInventoryGooglePlayProductPurchase& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "ProductId", input.productId);
    JsonUtils::ObjectAddMember(output, "Token", input.token);
    return output;
}

JsonValue RedeemGooglePlayInventoryItemsRequest::ToJson() const
{
    return RedeemGooglePlayInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemGooglePlayInventoryItemsRequest::ToJson(const PFInventoryRedeemGooglePlayInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<GooglePlayProductPurchase>(output, "Purchases", input.purchases, input.purchasesCount);
    return output;
}

HRESULT RedeemGooglePlayInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemGooglePlayInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemGooglePlayInventoryItemsResponse const*> RedeemGooglePlayInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemGooglePlayInventoryItemsResponse>(&this->Model());
}

size_t RedeemGooglePlayInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemGooglePlayInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemGooglePlayInventoryItemsResponse::Copy(const PFInventoryRedeemGooglePlayInventoryItemsResponse& input, PFInventoryRedeemGooglePlayInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RedeemMicrosoftStoreInventoryItemsRequest::ToJson() const
{
    return RedeemMicrosoftStoreInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemMicrosoftStoreInventoryItemsRequest::ToJson(const PFInventoryRedeemMicrosoftStoreInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMember(output, "CollectionsIdKey", input.collectionsIdKey);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
#if HC_PLATFORM != HC_PLATFORM_GDK
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
#endif
    return output;
}

HRESULT RedeemMicrosoftStoreInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemMicrosoftStoreInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemMicrosoftStoreInventoryItemsResponse const*> RedeemMicrosoftStoreInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemMicrosoftStoreInventoryItemsResponse>(&this->Model());
}

size_t RedeemMicrosoftStoreInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemMicrosoftStoreInventoryItemsResponse::Copy(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& input, PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RedeemNintendoEShopInventoryItemsRequest::ToJson() const
{
    return RedeemNintendoEShopInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemNintendoEShopInventoryItemsRequest::ToJson(const PFInventoryRedeemNintendoEShopInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "NintendoServiceAccountIdToken", input.nintendoServiceAccountIdToken);
    return output;
}

HRESULT RedeemNintendoEShopInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemNintendoEShopInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemNintendoEShopInventoryItemsResponse const*> RedeemNintendoEShopInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemNintendoEShopInventoryItemsResponse>(&this->Model());
}

size_t RedeemNintendoEShopInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemNintendoEShopInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemNintendoEShopInventoryItemsResponse::Copy(const PFInventoryRedeemNintendoEShopInventoryItemsResponse& input, PFInventoryRedeemNintendoEShopInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RedeemPlayStationStoreInventoryItemsRequest::ToJson() const
{
    return RedeemPlayStationStoreInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemPlayStationStoreInventoryItemsRequest::ToJson(const PFInventoryRedeemPlayStationStoreInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AuthorizationCode", input.authorizationCode);
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "RedirectUri", input.redirectUri);
    JsonUtils::ObjectAddMember(output, "ServiceLabel", input.serviceLabel);
    return output;
}

HRESULT RedeemPlayStationStoreInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemPlayStationStoreInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemPlayStationStoreInventoryItemsResponse const*> RedeemPlayStationStoreInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemPlayStationStoreInventoryItemsResponse>(&this->Model());
}

size_t RedeemPlayStationStoreInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemPlayStationStoreInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemPlayStationStoreInventoryItemsResponse::Copy(const PFInventoryRedeemPlayStationStoreInventoryItemsResponse& input, PFInventoryRedeemPlayStationStoreInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RedeemSteamInventoryItemsRequest::ToJson() const
{
    return RedeemSteamInventoryItemsRequest::ToJson(this->Model());
}

JsonValue RedeemSteamInventoryItemsRequest::ToJson(const PFInventoryRedeemSteamInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT RedeemSteamInventoryItemsResponse::FromJson(const JsonValue& input)
{
    ModelVector<RedemptionFailure> failed{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionFailure>(input, "Failed", failed));
    this->SetFailed(std::move(failed));

    ModelVector<RedemptionSuccess> succeeded{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<RedemptionSuccess>(input, "Succeeded", succeeded));
    this->SetSucceeded(std::move(succeeded));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t RedeemSteamInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryRedeemSteamInventoryItemsResponse const*> RedeemSteamInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RedeemSteamInventoryItemsResponse>(&this->Model());
}

size_t RedeemSteamInventoryItemsResponse::RequiredBufferSize(const PFInventoryRedeemSteamInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFInventoryRedemptionFailure*) + sizeof(PFInventoryRedemptionFailure*) * model.failedCount);
    for (size_t i = 0; i < model.failedCount; ++i)
    {
        requiredSize += RedemptionFailure::RequiredBufferSize(*model.failed[i]);
    }
    requiredSize += (alignof(PFInventoryRedemptionSuccess*) + sizeof(PFInventoryRedemptionSuccess*) * model.succeededCount);
    for (size_t i = 0; i < model.succeededCount; ++i)
    {
        requiredSize += RedemptionSuccess::RequiredBufferSize(*model.succeeded[i]);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT RedeemSteamInventoryItemsResponse::Copy(const PFInventoryRedeemSteamInventoryItemsResponse& input, PFInventoryRedeemSteamInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionFailure>(input.failed, input.failedCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.failed = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<RedemptionSuccess>(input.succeeded, input.succeededCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.succeeded = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SubtractInventoryItemsRequest::ToJson() const
{
    return SubtractInventoryItemsRequest::ToJson(this->Model());
}

JsonValue SubtractInventoryItemsRequest::ToJson(const PFInventorySubtractInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember(output, "DurationInSeconds", input.durationInSeconds);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "Item", input.item);
    return output;
}

HRESULT SubtractInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t SubtractInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventorySubtractInventoryItemsResponse const*> SubtractInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SubtractInventoryItemsResponse>(&this->Model());
}

size_t SubtractInventoryItemsResponse::RequiredBufferSize(const PFInventorySubtractInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT SubtractInventoryItemsResponse::Copy(const PFInventorySubtractInventoryItemsResponse& input, PFInventorySubtractInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue TransferInventoryItemsRequest::ToJson() const
{
    return TransferInventoryItemsRequest::ToJson(this->Model());
}

JsonValue TransferInventoryItemsRequest::ToJson(const PFInventoryTransferInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Amount", input.amount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DeleteEmptyStacks", input.deleteEmptyStacks);
    JsonUtils::ObjectAddMember(output, "GivingCollectionId", input.givingCollectionId);
    JsonUtils::ObjectAddMember<EntityKey>(output, "GivingEntity", input.givingEntity);
    JsonUtils::ObjectAddMember(output, "GivingETag", input.givingETag);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "GivingItem", input.givingItem);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InitialValues>(output, "NewStackValues", input.newStackValues);
    JsonUtils::ObjectAddMember(output, "ReceivingCollectionId", input.receivingCollectionId);
    JsonUtils::ObjectAddMember<EntityKey>(output, "ReceivingEntity", input.receivingEntity);
    JsonUtils::ObjectAddMember<InventoryItemReference>(output, "ReceivingItem", input.receivingItem);
    return output;
}

HRESULT TransferInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String givingETag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GivingETag", givingETag));
    this->SetGivingETag(std::move(givingETag));

    CStringVector givingTransactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GivingTransactionIds", givingTransactionIds));
    this->SetGivingTransactionIds(std::move(givingTransactionIds));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector receivingTransactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ReceivingTransactionIds", receivingTransactionIds));
    this->SetReceivingTransactionIds(std::move(receivingTransactionIds));

    return S_OK;
}

size_t TransferInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryTransferInventoryItemsResponse const*> TransferInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TransferInventoryItemsResponse>(&this->Model());
}

size_t TransferInventoryItemsResponse::RequiredBufferSize(const PFInventoryTransferInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.givingETag)
    {
        requiredSize += (std::strlen(model.givingETag) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.givingTransactionIdsCount);
    for (size_t i = 0; i < model.givingTransactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.givingTransactionIds[i]) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.receivingTransactionIdsCount);
    for (size_t i = 0; i < model.receivingTransactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.receivingTransactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT TransferInventoryItemsResponse::Copy(const PFInventoryTransferInventoryItemsResponse& input, PFInventoryTransferInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.givingETag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.givingETag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.givingTransactionIds, input.givingTransactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.givingTransactionIds = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.receivingTransactionIds, input.receivingTransactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.receivingTransactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateInventoryItemsRequest::ToJson() const
{
    return UpdateInventoryItemsRequest::ToJson(this->Model());
}

JsonValue UpdateInventoryItemsRequest::ToJson(const PFInventoryUpdateInventoryItemsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CollectionId", input.collectionId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ETag", input.eTag);
    JsonUtils::ObjectAddMember(output, "IdempotencyId", input.idempotencyId);
    JsonUtils::ObjectAddMember<InventoryItem>(output, "Item", input.item);
    return output;
}

HRESULT UpdateInventoryItemsResponse::FromJson(const JsonValue& input)
{
    String eTag{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ETag", eTag));
    this->SetETag(std::move(eTag));

    String idempotencyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IdempotencyId", idempotencyId));
    this->SetIdempotencyId(std::move(idempotencyId));

    CStringVector transactionIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TransactionIds", transactionIds));
    this->SetTransactionIds(std::move(transactionIds));

    return S_OK;
}

size_t UpdateInventoryItemsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFInventoryUpdateInventoryItemsResponse const*> UpdateInventoryItemsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateInventoryItemsResponse>(&this->Model());
}

size_t UpdateInventoryItemsResponse::RequiredBufferSize(const PFInventoryUpdateInventoryItemsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.eTag)
    {
        requiredSize += (std::strlen(model.eTag) + 1);
    }
    if (model.idempotencyId)
    {
        requiredSize += (std::strlen(model.idempotencyId) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.transactionIdsCount);
    for (size_t i = 0; i < model.transactionIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.transactionIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT UpdateInventoryItemsResponse::Copy(const PFInventoryUpdateInventoryItemsResponse& input, PFInventoryUpdateInventoryItemsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.eTag); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eTag = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.idempotencyId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.idempotencyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.transactionIds, input.transactionIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.transactionIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Inventory
} // namespace PlayFab

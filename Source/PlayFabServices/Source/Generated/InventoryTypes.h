#pragma once

#include <playfab/services/cpp/InventoryTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Inventory
{

// Inventory Classes
class AlternateId : public Wrappers::PFInventoryAlternateIdWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryAlternateIdWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryAlternateId& input);
};

class InventoryItemReference : public Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryInventoryItemReference& input);
};

class InitialValues : public Wrappers::PFInventoryInitialValuesWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryInitialValuesWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryInitialValues& input);
};

class AddInventoryItemsRequest : public Wrappers::PFInventoryAddInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryAddInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryAddInventoryItemsRequest& input);
};

class AddInventoryItemsResponse : public Wrappers::PFInventoryAddInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryAddInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryAddInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryAddInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryAddInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryAddInventoryItemsResponse& input, PFInventoryAddInventoryItemsResponse& output, ModelBuffer& buffer);
};

class DeleteInventoryCollectionRequest : public Wrappers::PFInventoryDeleteInventoryCollectionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryDeleteInventoryCollectionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryDeleteInventoryCollectionRequest& input);
};

class DeleteInventoryItemsRequest : public Wrappers::PFInventoryDeleteInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryDeleteInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryDeleteInventoryItemsRequest& input);
};

class DeleteInventoryItemsResponse : public Wrappers::PFInventoryDeleteInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryDeleteInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryDeleteInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryDeleteInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryDeleteInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryDeleteInventoryItemsResponse& input, PFInventoryDeleteInventoryItemsResponse& output, ModelBuffer& buffer);
};

class AddInventoryItemsOperation : public Wrappers::PFInventoryAddInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryAddInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryAddInventoryItemsOperation& input);
};

class DeleteInventoryItemsOperation : public Wrappers::PFInventoryDeleteInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryDeleteInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryDeleteInventoryItemsOperation& input);
};

class PurchasePriceAmount : public Wrappers::PFInventoryPurchasePriceAmountWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryPurchasePriceAmountWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryPurchasePriceAmount& input);
};

class PurchaseInventoryItemsOperation : public Wrappers::PFInventoryPurchaseInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryPurchaseInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryPurchaseInventoryItemsOperation& input);
};

class SubtractInventoryItemsOperation : public Wrappers::PFInventorySubtractInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventorySubtractInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventorySubtractInventoryItemsOperation& input);
};

class TransferInventoryItemsOperation : public Wrappers::PFInventoryTransferInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransferInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryTransferInventoryItemsOperation& input);
};

class InventoryItem : public Wrappers::PFInventoryInventoryItemWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFInventoryInventoryItem>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryInventoryItemWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryInventoryItem& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryInventoryItem const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryInventoryItem& model);
    static HRESULT Copy(const PFInventoryInventoryItem& input, PFInventoryInventoryItem& output, ModelBuffer& buffer);
};

class UpdateInventoryItemsOperation : public Wrappers::PFInventoryUpdateInventoryItemsOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryUpdateInventoryItemsOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryUpdateInventoryItemsOperation& input);
};

class InventoryOperation : public Wrappers::PFInventoryInventoryOperationWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryInventoryOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryInventoryOperation& input);
};

class ExecuteInventoryOperationsRequest : public Wrappers::PFInventoryExecuteInventoryOperationsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryExecuteInventoryOperationsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryExecuteInventoryOperationsRequest& input);
};

class ExecuteInventoryOperationsResponse : public Wrappers::PFInventoryExecuteInventoryOperationsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryExecuteInventoryOperationsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryExecuteInventoryOperationsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryExecuteInventoryOperationsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryExecuteInventoryOperationsResponse& model);
    static HRESULT Copy(const PFInventoryExecuteInventoryOperationsResponse& input, PFInventoryExecuteInventoryOperationsResponse& output, ModelBuffer& buffer);
};

class ExecuteTransferOperationsRequest : public Wrappers::PFInventoryExecuteTransferOperationsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryExecuteTransferOperationsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryExecuteTransferOperationsRequest& input);
};

class ExecuteTransferOperationsResponse : public Wrappers::PFInventoryExecuteTransferOperationsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryExecuteTransferOperationsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryExecuteTransferOperationsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryExecuteTransferOperationsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryExecuteTransferOperationsResponse& model);
    static HRESULT Copy(const PFInventoryExecuteTransferOperationsResponse& input, PFInventoryExecuteTransferOperationsResponse& output, ModelBuffer& buffer);
};

class GetInventoryCollectionIdsRequest : public Wrappers::PFInventoryGetInventoryCollectionIdsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryCollectionIdsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGetInventoryCollectionIdsRequest& input);
};

class GetInventoryCollectionIdsResponse : public Wrappers::PFInventoryGetInventoryCollectionIdsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryGetInventoryCollectionIdsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryCollectionIdsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryGetInventoryCollectionIdsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryGetInventoryCollectionIdsResponse& model);
    static HRESULT Copy(const PFInventoryGetInventoryCollectionIdsResponse& input, PFInventoryGetInventoryCollectionIdsResponse& output, ModelBuffer& buffer);
};

class GetInventoryItemsRequest : public Wrappers::PFInventoryGetInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGetInventoryItemsRequest& input);
};

class GetInventoryItemsResponse : public Wrappers::PFInventoryGetInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryGetInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryGetInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryGetInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryGetInventoryItemsResponse& input, PFInventoryGetInventoryItemsResponse& output, ModelBuffer& buffer);
};

class GetInventoryOperationStatusRequest : public Wrappers::PFInventoryGetInventoryOperationStatusRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryOperationStatusRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGetInventoryOperationStatusRequest& input);
};

class GetInventoryOperationStatusResponse : public Wrappers::PFInventoryGetInventoryOperationStatusResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryGetInventoryOperationStatusResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetInventoryOperationStatusResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryGetInventoryOperationStatusResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryGetInventoryOperationStatusResponse& model);
    static HRESULT Copy(const PFInventoryGetInventoryOperationStatusResponse& input, PFInventoryGetInventoryOperationStatusResponse& output, ModelBuffer& buffer);
};

class GetMicrosoftStoreAccessTokensRequest : public Wrappers::PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGetMicrosoftStoreAccessTokensRequest& input);
};

class GetMicrosoftStoreAccessTokensResponse : public Wrappers::PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryGetMicrosoftStoreAccessTokensResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryGetMicrosoftStoreAccessTokensResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryGetMicrosoftStoreAccessTokensResponse& model);
    static HRESULT Copy(const PFInventoryGetMicrosoftStoreAccessTokensResponse& input, PFInventoryGetMicrosoftStoreAccessTokensResponse& output, ModelBuffer& buffer);
};

class GetTransactionHistoryRequest : public Wrappers::PFInventoryGetTransactionHistoryRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetTransactionHistoryRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGetTransactionHistoryRequest& input);
};

class TransactionOperation : public Wrappers::PFInventoryTransactionOperationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransactionOperation>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransactionOperationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransactionOperation const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransactionOperation& model);
    static HRESULT Copy(const PFInventoryTransactionOperation& input, PFInventoryTransactionOperation& output, ModelBuffer& buffer);
};

class TransactionPurchaseDetails : public Wrappers::PFInventoryTransactionPurchaseDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransactionPurchaseDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransactionPurchaseDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransactionPurchaseDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransactionPurchaseDetails& model);
    static HRESULT Copy(const PFInventoryTransactionPurchaseDetails& input, PFInventoryTransactionPurchaseDetails& output, ModelBuffer& buffer);
};

class TransactionRedeemDetails : public Wrappers::PFInventoryTransactionRedeemDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransactionRedeemDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransactionRedeemDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransactionRedeemDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransactionRedeemDetails& model);
    static HRESULT Copy(const PFInventoryTransactionRedeemDetails& input, PFInventoryTransactionRedeemDetails& output, ModelBuffer& buffer);
};

class TransactionTransferDetails : public Wrappers::PFInventoryTransactionTransferDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransactionTransferDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransactionTransferDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransactionTransferDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransactionTransferDetails& model);
    static HRESULT Copy(const PFInventoryTransactionTransferDetails& input, PFInventoryTransactionTransferDetails& output, ModelBuffer& buffer);
};

class Transaction : public Wrappers::PFInventoryTransactionWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransaction>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransactionWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransaction const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransaction& model);
    static HRESULT Copy(const PFInventoryTransaction& input, PFInventoryTransaction& output, ModelBuffer& buffer);
};

class GetTransactionHistoryResponse : public Wrappers::PFInventoryGetTransactionHistoryResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryGetTransactionHistoryResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGetTransactionHistoryResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryGetTransactionHistoryResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryGetTransactionHistoryResponse& model);
    static HRESULT Copy(const PFInventoryGetTransactionHistoryResponse& input, PFInventoryGetTransactionHistoryResponse& output, ModelBuffer& buffer);
};

class PurchaseInventoryItemsRequest : public Wrappers::PFInventoryPurchaseInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryPurchaseInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryPurchaseInventoryItemsRequest& input);
};

class PurchaseInventoryItemsResponse : public Wrappers::PFInventoryPurchaseInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryPurchaseInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryPurchaseInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryPurchaseInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryPurchaseInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryPurchaseInventoryItemsResponse& input, PFInventoryPurchaseInventoryItemsResponse& output, ModelBuffer& buffer);
};

class RedeemAppleAppStoreInventoryItemsRequest : public Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemAppleAppStoreInventoryItemsRequest& input);
};

class RedemptionFailure : public Wrappers::PFInventoryRedemptionFailureWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedemptionFailure>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedemptionFailureWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedemptionFailure const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedemptionFailure& model);
    static HRESULT Copy(const PFInventoryRedemptionFailure& input, PFInventoryRedemptionFailure& output, ModelBuffer& buffer);
};

class RedemptionSuccess : public Wrappers::PFInventoryRedemptionSuccessWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedemptionSuccess>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedemptionSuccessWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedemptionSuccess const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedemptionSuccess& model);
    static HRESULT Copy(const PFInventoryRedemptionSuccess& input, PFInventoryRedemptionSuccess& output, ModelBuffer& buffer);
};

class RedeemAppleAppStoreInventoryItemsResponse : public Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemAppleAppStoreInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemAppleAppStoreInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemAppleAppStoreInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemAppleAppStoreInventoryItemsResponse& input, PFInventoryRedeemAppleAppStoreInventoryItemsResponse& output, ModelBuffer& buffer);
};

class GooglePlayProductPurchase : public Wrappers::PFInventoryGooglePlayProductPurchaseWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryGooglePlayProductPurchaseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryGooglePlayProductPurchase& input);
};

class RedeemGooglePlayInventoryItemsRequest : public Wrappers::PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemGooglePlayInventoryItemsRequest& input);
};

class RedeemGooglePlayInventoryItemsResponse : public Wrappers::PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemGooglePlayInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemGooglePlayInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemGooglePlayInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemGooglePlayInventoryItemsResponse& input, PFInventoryRedeemGooglePlayInventoryItemsResponse& output, ModelBuffer& buffer);
};

class RedeemMicrosoftStoreInventoryItemsRequest : public Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemMicrosoftStoreInventoryItemsRequest& input);
};

class RedeemMicrosoftStoreInventoryItemsResponse : public Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemMicrosoftStoreInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemMicrosoftStoreInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& input, PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& output, ModelBuffer& buffer);
};

class RedeemNintendoEShopInventoryItemsRequest : public Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemNintendoEShopInventoryItemsRequest& input);
};

class RedeemNintendoEShopInventoryItemsResponse : public Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemNintendoEShopInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemNintendoEShopInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemNintendoEShopInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemNintendoEShopInventoryItemsResponse& input, PFInventoryRedeemNintendoEShopInventoryItemsResponse& output, ModelBuffer& buffer);
};

class RedeemPlayStationStoreInventoryItemsRequest : public Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemPlayStationStoreInventoryItemsRequest& input);
};

class RedeemPlayStationStoreInventoryItemsResponse : public Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemPlayStationStoreInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemPlayStationStoreInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemPlayStationStoreInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemPlayStationStoreInventoryItemsResponse& input, PFInventoryRedeemPlayStationStoreInventoryItemsResponse& output, ModelBuffer& buffer);
};

class RedeemSteamInventoryItemsRequest : public Wrappers::PFInventoryRedeemSteamInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemSteamInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryRedeemSteamInventoryItemsRequest& input);
};

class RedeemSteamInventoryItemsResponse : public Wrappers::PFInventoryRedeemSteamInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryRedeemSteamInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryRedeemSteamInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryRedeemSteamInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryRedeemSteamInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryRedeemSteamInventoryItemsResponse& input, PFInventoryRedeemSteamInventoryItemsResponse& output, ModelBuffer& buffer);
};

class SubtractInventoryItemsRequest : public Wrappers::PFInventorySubtractInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventorySubtractInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventorySubtractInventoryItemsRequest& input);
};

class SubtractInventoryItemsResponse : public Wrappers::PFInventorySubtractInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventorySubtractInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventorySubtractInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventorySubtractInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventorySubtractInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventorySubtractInventoryItemsResponse& input, PFInventorySubtractInventoryItemsResponse& output, ModelBuffer& buffer);
};

class TransferInventoryItemsRequest : public Wrappers::PFInventoryTransferInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransferInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryTransferInventoryItemsRequest& input);
};

class TransferInventoryItemsResponse : public Wrappers::PFInventoryTransferInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryTransferInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryTransferInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryTransferInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryTransferInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryTransferInventoryItemsResponse& input, PFInventoryTransferInventoryItemsResponse& output, ModelBuffer& buffer);
};

class UpdateInventoryItemsRequest : public Wrappers::PFInventoryUpdateInventoryItemsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryUpdateInventoryItemsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFInventoryUpdateInventoryItemsRequest& input);
};

class UpdateInventoryItemsResponse : public Wrappers::PFInventoryUpdateInventoryItemsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFInventoryUpdateInventoryItemsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFInventoryUpdateInventoryItemsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFInventoryUpdateInventoryItemsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFInventoryUpdateInventoryItemsResponse& model);
    static HRESULT Copy(const PFInventoryUpdateInventoryItemsResponse& input, PFInventoryUpdateInventoryItemsResponse& output, ModelBuffer& buffer);
};

} // namespace Inventory

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization

} // namespace PlayFab

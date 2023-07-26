#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "InventoryTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace Inventory
{

class InventoryAPI
{
public:
    InventoryAPI() = delete;
    InventoryAPI(const InventoryAPI& source) = delete;
    InventoryAPI& operator=(const InventoryAPI& source) = delete;
    ~InventoryAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<AddInventoryItemsResponse> AddInventoryItems(Entity const& entity, const AddInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<void> DeleteInventoryCollection(Entity const& entity, const DeleteInventoryCollectionRequest& request, RunContext rc);
    static AsyncOp<DeleteInventoryItemsResponse> DeleteInventoryItems(Entity const& entity, const DeleteInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<ExecuteInventoryOperationsResponse> ExecuteInventoryOperations(Entity const& entity, const ExecuteInventoryOperationsRequest& request, RunContext rc);
    static AsyncOp<GetInventoryCollectionIdsResponse> GetInventoryCollectionIds(Entity const& entity, const GetInventoryCollectionIdsRequest& request, RunContext rc);
    static AsyncOp<GetInventoryItemsResponse> GetInventoryItems(Entity const& entity, const GetInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<GetMicrosoftStoreAccessTokensResponse> GetMicrosoftStoreAccessTokens(Entity const& entity, const GetMicrosoftStoreAccessTokensRequest& request, RunContext rc);
    static AsyncOp<GetTransactionHistoryResponse> GetTransactionHistory(Entity const& entity, const GetTransactionHistoryRequest& request, RunContext rc);
    static AsyncOp<PurchaseInventoryItemsResponse> PurchaseInventoryItems(Entity const& entity, const PurchaseInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemAppleAppStoreInventoryItemsResponse> RedeemAppleAppStoreInventoryItems(Entity const& entity, const RedeemAppleAppStoreInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemGooglePlayInventoryItemsResponse> RedeemGooglePlayInventoryItems(Entity const& entity, const RedeemGooglePlayInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemMicrosoftStoreInventoryItemsResponse> RedeemMicrosoftStoreInventoryItems(Entity const& entity, const RedeemMicrosoftStoreInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemNintendoEShopInventoryItemsResponse> RedeemNintendoEShopInventoryItems(Entity const& entity, const RedeemNintendoEShopInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemPlayStationStoreInventoryItemsResponse> RedeemPlayStationStoreInventoryItems(Entity const& entity, const RedeemPlayStationStoreInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<RedeemSteamInventoryItemsResponse> RedeemSteamInventoryItems(Entity const& entity, const RedeemSteamInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<SubtractInventoryItemsResponse> SubtractInventoryItems(Entity const& entity, const SubtractInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<TransferInventoryItemsResponse> TransferInventoryItems(Entity const& entity, const TransferInventoryItemsRequest& request, RunContext rc);
    static AsyncOp<UpdateInventoryItemsResponse> UpdateInventoryItems(Entity const& entity, const UpdateInventoryItemsRequest& request, RunContext rc);
};

} // namespace Inventory
} // namespace PlayFab

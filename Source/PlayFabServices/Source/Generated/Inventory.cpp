#include "stdafx.h"
#include "Inventory.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace Inventory
{


AsyncOp<AddInventoryItemsResponse> InventoryAPI::AddInventoryItems(
    Entity const& entity,
    const AddInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/AddInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryAddInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<AddInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            AddInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<AddInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> InventoryAPI::DeleteInventoryCollection(
    Entity const& entity,
    const DeleteInventoryCollectionRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/DeleteInventoryCollection" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryDeleteInventoryCollection,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<DeleteInventoryItemsResponse> InventoryAPI::DeleteInventoryItems(
    Entity const& entity,
    const DeleteInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/DeleteInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryDeleteInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<DeleteInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            DeleteInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<DeleteInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ExecuteInventoryOperationsResponse> InventoryAPI::ExecuteInventoryOperations(
    Entity const& entity,
    const ExecuteInventoryOperationsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/ExecuteInventoryOperations" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryExecuteInventoryOperations,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ExecuteInventoryOperationsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ExecuteInventoryOperationsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ExecuteInventoryOperationsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetInventoryCollectionIdsResponse> InventoryAPI::GetInventoryCollectionIds(
    Entity const& entity,
    const GetInventoryCollectionIdsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/GetInventoryCollectionIds" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryGetInventoryCollectionIds,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetInventoryCollectionIdsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetInventoryCollectionIdsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetInventoryCollectionIdsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetInventoryItemsResponse> InventoryAPI::GetInventoryItems(
    Entity const& entity,
    const GetInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/GetInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryGetInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetMicrosoftStoreAccessTokensResponse> InventoryAPI::GetMicrosoftStoreAccessTokens(
    Entity const& entity,
    const GetMicrosoftStoreAccessTokensRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/GetMicrosoftStoreAccessTokens" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryGetMicrosoftStoreAccessTokens,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetMicrosoftStoreAccessTokensResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetMicrosoftStoreAccessTokensResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetMicrosoftStoreAccessTokensResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTransactionHistoryResponse> InventoryAPI::GetTransactionHistory(
    Entity const& entity,
    const GetTransactionHistoryRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/GetTransactionHistory" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryGetTransactionHistory,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTransactionHistoryResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTransactionHistoryResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTransactionHistoryResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<PurchaseInventoryItemsResponse> InventoryAPI::PurchaseInventoryItems(
    Entity const& entity,
    const PurchaseInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/PurchaseInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryPurchaseInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<PurchaseInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            PurchaseInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<PurchaseInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RedeemAppleAppStoreInventoryItemsResponse> InventoryAPI::RedeemAppleAppStoreInventoryItems(
    Entity const& entity,
    const RedeemAppleAppStoreInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemAppleAppStoreInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemAppleAppStoreInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemAppleAppStoreInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemAppleAppStoreInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemAppleAppStoreInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RedeemGooglePlayInventoryItemsResponse> InventoryAPI::RedeemGooglePlayInventoryItems(
    Entity const& entity,
    const RedeemGooglePlayInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemGooglePlayInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemGooglePlayInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemGooglePlayInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemGooglePlayInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemGooglePlayInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

#if HC_PLATFORM != HC_PLATFORM_GDK
AsyncOp<RedeemMicrosoftStoreInventoryItemsResponse> InventoryAPI::RedeemMicrosoftStoreInventoryItems(
    Entity const& entity,
    const RedeemMicrosoftStoreInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemMicrosoftStoreInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemMicrosoftStoreInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemMicrosoftStoreInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemMicrosoftStoreInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemMicrosoftStoreInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}
#endif

AsyncOp<RedeemNintendoEShopInventoryItemsResponse> InventoryAPI::RedeemNintendoEShopInventoryItems(
    Entity const& entity,
    const RedeemNintendoEShopInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemNintendoEShopInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemNintendoEShopInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemNintendoEShopInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemNintendoEShopInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemNintendoEShopInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RedeemPlayStationStoreInventoryItemsResponse> InventoryAPI::RedeemPlayStationStoreInventoryItems(
    Entity const& entity,
    const RedeemPlayStationStoreInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemPlayStationStoreInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemPlayStationStoreInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemPlayStationStoreInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemPlayStationStoreInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemPlayStationStoreInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<RedeemSteamInventoryItemsResponse> InventoryAPI::RedeemSteamInventoryItems(
    Entity const& entity,
    const RedeemSteamInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/RedeemSteamInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryRedeemSteamInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RedeemSteamInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RedeemSteamInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RedeemSteamInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SubtractInventoryItemsResponse> InventoryAPI::SubtractInventoryItems(
    Entity const& entity,
    const SubtractInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/SubtractInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventorySubtractInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<SubtractInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            SubtractInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<SubtractInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<TransferInventoryItemsResponse> InventoryAPI::TransferInventoryItems(
    Entity const& entity,
    const TransferInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/TransferInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryTransferInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<TransferInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            TransferInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<TransferInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<UpdateInventoryItemsResponse> InventoryAPI::UpdateInventoryItems(
    Entity const& entity,
    const UpdateInventoryItemsRequest& request,
    RunContext rc
)
{
    const char* path{ "/Inventory/UpdateInventoryItems" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::InventoryUpdateInventoryItems,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<UpdateInventoryItemsResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            UpdateInventoryItemsResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<UpdateInventoryItemsResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Inventory
} // namespace PlayFab

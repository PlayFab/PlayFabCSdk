#include "TestAppPch.h"
#include "InventoryOperations.h"
#include <playfab/services/PFInventory.h>

namespace PlayFab
{
namespace Test
{


AddInventoryItemsOperation::AddInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<AddInventoryItemsOperation::ResultType> AddInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<AddInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT AddInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryAddInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<AddInventoryItemsOperation::ResultType> AddInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryAddInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryAddInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryAddInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


DeleteInventoryCollectionOperation::DeleteInventoryCollectionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteInventoryCollectionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteInventoryCollectionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteInventoryCollectionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryDeleteInventoryCollectionAsync(m_entity.Handle(), &m_request.Model(), async);
}



DeleteInventoryItemsOperation::DeleteInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<DeleteInventoryItemsOperation::ResultType> DeleteInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryDeleteInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<DeleteInventoryItemsOperation::ResultType> DeleteInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryDeleteInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryDeleteInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryDeleteInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ExecuteInventoryOperationsOperation::ExecuteInventoryOperationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ExecuteInventoryOperationsOperation::ResultType> ExecuteInventoryOperationsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ExecuteInventoryOperationsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ExecuteInventoryOperationsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryExecuteInventoryOperationsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ExecuteInventoryOperationsOperation::ResultType> ExecuteInventoryOperationsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryExecuteInventoryOperationsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryExecuteInventoryOperationsResponse* result;
    RETURN_IF_FAILED(PFInventoryExecuteInventoryOperationsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetInventoryCollectionIdsOperation::GetInventoryCollectionIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetInventoryCollectionIdsOperation::ResultType> GetInventoryCollectionIdsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetInventoryCollectionIdsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetInventoryCollectionIdsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryGetInventoryCollectionIdsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetInventoryCollectionIdsOperation::ResultType> GetInventoryCollectionIdsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryGetInventoryCollectionIdsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryGetInventoryCollectionIdsResponse* result;
    RETURN_IF_FAILED(PFInventoryGetInventoryCollectionIdsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetInventoryItemsOperation::GetInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetInventoryItemsOperation::ResultType> GetInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryGetInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetInventoryItemsOperation::ResultType> GetInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryGetInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryGetInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryGetInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX

GetMicrosoftStoreAccessTokensOperation::GetMicrosoftStoreAccessTokensOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetMicrosoftStoreAccessTokensOperation::ResultType> GetMicrosoftStoreAccessTokensOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetMicrosoftStoreAccessTokensOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetMicrosoftStoreAccessTokensOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryGetMicrosoftStoreAccessTokensAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetMicrosoftStoreAccessTokensOperation::ResultType> GetMicrosoftStoreAccessTokensOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryGetMicrosoftStoreAccessTokensGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryGetMicrosoftStoreAccessTokensResponse* result;
    RETURN_IF_FAILED(PFInventoryGetMicrosoftStoreAccessTokensGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

GetTransactionHistoryOperation::GetTransactionHistoryOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetTransactionHistoryOperation::ResultType> GetTransactionHistoryOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetTransactionHistoryOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetTransactionHistoryOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryGetTransactionHistoryAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetTransactionHistoryOperation::ResultType> GetTransactionHistoryOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryGetTransactionHistoryGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryGetTransactionHistoryResponse* result;
    RETURN_IF_FAILED(PFInventoryGetTransactionHistoryGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


PurchaseInventoryItemsOperation::PurchaseInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<PurchaseInventoryItemsOperation::ResultType> PurchaseInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<PurchaseInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT PurchaseInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryPurchaseInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<PurchaseInventoryItemsOperation::ResultType> PurchaseInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryPurchaseInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryPurchaseInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryPurchaseInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

RedeemAppleAppStoreInventoryItemsOperation::RedeemAppleAppStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemAppleAppStoreInventoryItemsOperation::ResultType> RedeemAppleAppStoreInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemAppleAppStoreInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemAppleAppStoreInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemAppleAppStoreInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemAppleAppStoreInventoryItemsOperation::ResultType> RedeemAppleAppStoreInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemAppleAppStoreInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemAppleAppStoreInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemAppleAppStoreInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID

RedeemGooglePlayInventoryItemsOperation::RedeemGooglePlayInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemGooglePlayInventoryItemsOperation::ResultType> RedeemGooglePlayInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemGooglePlayInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemGooglePlayInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemGooglePlayInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemGooglePlayInventoryItemsOperation::ResultType> RedeemGooglePlayInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemGooglePlayInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemGooglePlayInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemGooglePlayInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX

RedeemMicrosoftStoreInventoryItemsOperation::RedeemMicrosoftStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemMicrosoftStoreInventoryItemsOperation::ResultType> RedeemMicrosoftStoreInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemMicrosoftStoreInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemMicrosoftStoreInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemMicrosoftStoreInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemMicrosoftStoreInventoryItemsOperation::ResultType> RedeemMicrosoftStoreInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemMicrosoftStoreInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemMicrosoftStoreInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemMicrosoftStoreInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX

RedeemNintendoEShopInventoryItemsOperation::RedeemNintendoEShopInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemNintendoEShopInventoryItemsOperation::ResultType> RedeemNintendoEShopInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemNintendoEShopInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemNintendoEShopInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemNintendoEShopInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemNintendoEShopInventoryItemsOperation::ResultType> RedeemNintendoEShopInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemNintendoEShopInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemNintendoEShopInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemNintendoEShopInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX

RedeemPlayStationStoreInventoryItemsOperation::RedeemPlayStationStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemPlayStationStoreInventoryItemsOperation::ResultType> RedeemPlayStationStoreInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemPlayStationStoreInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemPlayStationStoreInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemPlayStationStoreInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemPlayStationStoreInventoryItemsOperation::ResultType> RedeemPlayStationStoreInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemPlayStationStoreInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemPlayStationStoreInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemPlayStationStoreInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

RedeemSteamInventoryItemsOperation::RedeemSteamInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RedeemSteamInventoryItemsOperation::ResultType> RedeemSteamInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RedeemSteamInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RedeemSteamInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryRedeemSteamInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RedeemSteamInventoryItemsOperation::ResultType> RedeemSteamInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryRedeemSteamInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryRedeemSteamInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryRedeemSteamInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


SubtractInventoryItemsOperation::SubtractInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<SubtractInventoryItemsOperation::ResultType> SubtractInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<SubtractInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT SubtractInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventorySubtractInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<SubtractInventoryItemsOperation::ResultType> SubtractInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventorySubtractInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventorySubtractInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventorySubtractInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


TransferInventoryItemsOperation::TransferInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<TransferInventoryItemsOperation::ResultType> TransferInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<TransferInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT TransferInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryTransferInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<TransferInventoryItemsOperation::ResultType> TransferInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryTransferInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryTransferInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryTransferInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


UpdateInventoryItemsOperation::UpdateInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<UpdateInventoryItemsOperation::ResultType> UpdateInventoryItemsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateInventoryItemsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateInventoryItemsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFInventoryUpdateInventoryItemsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<UpdateInventoryItemsOperation::ResultType> UpdateInventoryItemsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFInventoryUpdateInventoryItemsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFInventoryUpdateInventoryItemsResponse* result;
    RETURN_IF_FAILED(PFInventoryUpdateInventoryItemsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

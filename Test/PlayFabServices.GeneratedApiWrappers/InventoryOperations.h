#pragma once

#include <playfab/services/cpp/InventoryTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class AddInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryAddInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryAddInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryAddInventoryItemsResponseWrapper<Allocator>;

    AddInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryAddInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteInventoryCollectionOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFInventoryDeleteInventoryCollectionRequestWrapper<Allocator>;

    DeleteInventoryCollectionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryDeleteInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryDeleteInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryDeleteInventoryItemsResponseWrapper<Allocator>;

    DeleteInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryDeleteInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ExecuteInventoryOperationsOperation : public XAsyncOperation<Wrappers::PFInventoryExecuteInventoryOperationsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryExecuteInventoryOperationsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryExecuteInventoryOperationsResponseWrapper<Allocator>;

    ExecuteInventoryOperationsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryExecuteInventoryOperationsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetInventoryCollectionIdsOperation : public XAsyncOperation<Wrappers::PFInventoryGetInventoryCollectionIdsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryGetInventoryCollectionIdsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryGetInventoryCollectionIdsResponseWrapper<Allocator>;

    GetInventoryCollectionIdsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryGetInventoryCollectionIdsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryGetInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryGetInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryGetInventoryItemsResponseWrapper<Allocator>;

    GetInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryGetInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX
class GetMicrosoftStoreAccessTokensOperation : public XAsyncOperation<Wrappers::PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper<Allocator>;

    GetMicrosoftStoreAccessTokensOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class GetTransactionHistoryOperation : public XAsyncOperation<Wrappers::PFInventoryGetTransactionHistoryResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryGetTransactionHistoryRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryGetTransactionHistoryResponseWrapper<Allocator>;

    GetTransactionHistoryOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryGetTransactionHistoryResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class PurchaseInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryPurchaseInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryPurchaseInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryPurchaseInventoryItemsResponseWrapper<Allocator>;

    PurchaseInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryPurchaseInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if 0
class RedeemAppleAppStoreInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper<Allocator>;

    RedeemAppleAppStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
class RedeemGooglePlayInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper<Allocator>;

    RedeemGooglePlayInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX
class RedeemMicrosoftStoreInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper<Allocator>;

    RedeemMicrosoftStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX
class RedeemNintendoEShopInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper<Allocator>;

    RedeemNintendoEShopInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX
class RedeemPlayStationStoreInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper<Allocator>;

    RedeemPlayStationStoreInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class RedeemSteamInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryRedeemSteamInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryRedeemSteamInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryRedeemSteamInventoryItemsResponseWrapper<Allocator>;

    RedeemSteamInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryRedeemSteamInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class SubtractInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventorySubtractInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventorySubtractInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventorySubtractInventoryItemsResponseWrapper<Allocator>;

    SubtractInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventorySubtractInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class TransferInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryTransferInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryTransferInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryTransferInventoryItemsResponseWrapper<Allocator>;

    TransferInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryTransferInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class UpdateInventoryItemsOperation : public XAsyncOperation<Wrappers::PFInventoryUpdateInventoryItemsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFInventoryUpdateInventoryItemsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFInventoryUpdateInventoryItemsResponseWrapper<Allocator>;

    UpdateInventoryItemsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFInventoryUpdateInventoryItemsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

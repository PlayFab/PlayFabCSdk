#include "TestAppPch.h"
#include "InventoryTests.h"
#include "InventoryOperations.h"
#include "CatalogOperations.h"
#include "Platform/PlatformUtils.h"
#if HC_PLATFORM == HC_PLATFORM_GDK
#include "GDK/PlatformUser_GDK.h"
#endif
#include <playfab/httpClient/PFHttpClient.h>
#include <JsonUtils.h>

namespace PlayFab
{
namespace Test
{

constexpr char kFriendlyId[]{ "FriendlyId" };
constexpr char kPermanentCurrencyId[]{ "PermanentInventoryCurrencyId" };
constexpr char kPermanentItemId[]{ "PermanentInventoryItemId" };
constexpr char kPermanentItemType[]{ "catalogItem" };
constexpr int kCurrencyAmount{ 1234 };

Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator> AltCatalogId(String id)
{
    Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator> altId;
    altId.SetType("FriendlyId");
    altId.SetValue(id);

    return altId;
}

namespace
{
Wrappers::PFInventoryAlternateIdWrapper<Allocator> AltId(String id)
{
    Wrappers::PFInventoryAlternateIdWrapper<Allocator> altId;
    altId.SetType("FriendlyId");
    altId.SetValue(id);

    return altId;
}
}

AsyncOp<void> AddInventoryItem(Entity entity, String collectionId, RunContext rc, TestContext& tc)
{
    AddInventoryItemsOperation::RequestType request;
    request.SetAmount(1);
    request.SetCollectionId(collectionId);
    request.SetEntity(entity.EntityKey());
    Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
    item.SetAlternateId(AltId(kPermanentItemId));
    request.SetItem(item);

    return AddInventoryItemsOperation::Run(entity, request, rc).Then([&](Result<AddInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        // Add transaction (and possible CollectionCreated transaction if this is the first run on a title)
        auto count = result.Payload().Model().transactionIdsCount;
        tc.AssertTrue(2u == count || 1u == count, "transactionIdsCount");

        return S_OK;
    });
}

AsyncOp<void> DeleteCollection(Entity titleEntity, EntityKey player, String collectionId, RunContext rc)
{
    DeleteInventoryCollectionOperation::RequestType request;
    request.SetCollectionId(collectionId);
    request.SetEntity(player);

    return DeleteInventoryCollectionOperation::Run(titleEntity, request, rc);
}

AsyncOp<void> DeleteItem(Entity player, String collectionId, TestContext& tc, RunContext rc, uint32_t transactionCount = 2)
{
    DeleteInventoryItemsOperation::RequestType request;
    request.SetCollectionId(collectionId);
    request.SetEntity(player.EntityKey());
    Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
    Wrappers::PFInventoryAlternateIdWrapper<Allocator> altId;
    item.SetAlternateId(AltId(kPermanentItemId));
    request.SetItem(item);

    return DeleteInventoryItemsOperation::Run(player, request, rc).Then([&, transactionCount](Result<DeleteInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        // Delete transaction (and possible Subtract transaction)
        tc.AssertEqual(transactionCount, result.Payload().Model().transactionIdsCount, "transactionIdsCount");
        
        return S_OK;
    });
}

struct InventoryTestsState
{
    Wrappers::PFCatalogCatalogItemWrapper<Allocator> permanentItem;
    Wrappers::PFCatalogCatalogItemWrapper<Allocator> permanentCurrency;
    String currencyId;
};

// GDK runs for these tests depend on at least one Win32 run on the title for this currency/item setup via TitleEntity auth
AsyncOp<void> InventoryTests::Initialize()
{
    auto initResult = MakeShared<HRESULT>();

    return ServicesTestClass::Initialize()
    .Then([&, initResult](Result<void> result) -> Result<void>
    {
        *initResult = result.hr;

        RETURN_IF_FAILED(*initResult);

        Wrappers::PFCatalogCatalogItemWrapper<Allocator> currency;

        ModelVector<Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>> altCurrencyIds;
        altCurrencyIds.push_back(AltCatalogId(kPermanentCurrencyId));
        currency.SetAlternateIds(altCurrencyIds);
        EntityKey titleKey;
        titleKey.SetId(m_testTitleData.titleId);
        titleKey.SetType("title");
        currency.SetCreatorEntity(titleKey);
        currency.SetIsHidden(false);
        currency.SetStartDate(12345);
        StringDictionaryEntryVector title;
        title.insert_or_assign("NEUTRAL", "titleVal");
        currency.SetTitle(title);
        currency.SetType("currency");

        m_state = MakeShared<InventoryTestsState>();
        m_state->permanentCurrency = currency;
        
        return *initResult;
    })
#if HC_PLATFORM == HC_PLATFORM_WIN32
    .Then([&, initResult](Result<void> result) -> AsyncOp<CreateDraftItemOperation::ResultType>
    {
        *initResult = result.hr;

        RETURN_IF_FAILED(*initResult);

        // Create a permanent currency since the catalog publishing has possibly long processing times that can and do cause intermittent test failures
        CreateDraftItemOperation::RequestType request;
        request.SetItem(m_state->permanentCurrency);
        request.SetPublish(true);

        return CreateDraftItemOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<CreateDraftItemOperation::ResultType>) -> AsyncOp<GetItemOperation::ResultType>
    {
        GetItemOperation::RequestType request;
        request.SetAlternateId(AltCatalogId(kPermanentCurrencyId));
        request.SetEntity(TitleEntity().EntityKey());

        return GetItemOperation::Run(TitleEntity(), request, RunContext());
    })
#else
    .Then([&](Result<void>) -> AsyncOp<GetItemOperation::ResultType>
    {
        GetItemOperation::RequestType request;
        request.SetAlternateId(AltCatalogId(kPermanentCurrencyId));
        EntityKey titleKey;
        titleKey.SetId(m_testTitleData.titleId);
        titleKey.SetType("title");
        request.SetEntity(titleKey);

        return GetItemOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
#endif
    .Then([&](Result<GetItemOperation::ResultType> result) -> Result<void>
    {
        // Don't just fail because all non-purchase tests can still run
        if (Succeeded(result))
        {
            m_state->currencyId = result.ExtractPayload().Model().item->id;
        }
        else
        {
            m_state->currencyId = "couldNotGetCurrency";
        }

        Wrappers::PFCatalogCatalogItemWrapper<Allocator> item;

        ModelVector<Wrappers::PFCatalogCatalogAlternateIdWrapper<Allocator>> altItemIds;
        altItemIds.push_back(AltCatalogId(kPermanentItemId));
        item.SetAlternateIds(altItemIds);
        EntityKey titleKey;
        titleKey.SetId(m_testTitleData.titleId);
        titleKey.SetType("title");
        item.SetCreatorEntity(titleKey);
        item.SetIsHidden(false);
        Wrappers::PFCatalogCatalogPriceOptionsWrapper<Allocator> priceOptions;
        ModelVector<Wrappers::PFCatalogCatalogPriceWrapper<Allocator>> prices;
        Wrappers::PFCatalogCatalogPriceWrapper<Allocator> price;
        ModelVector<Wrappers::PFCatalogCatalogPriceAmountWrapper<Allocator>> amounts;
        Wrappers::PFCatalogCatalogPriceAmountWrapper<Allocator> amount;
        amount.SetItemId(m_state->currencyId);
        amount.SetAmount(kCurrencyAmount);
        amounts.push_back(amount);
        price.SetAmounts(amounts);
        prices.push_back(price);
        priceOptions.SetPrices(prices);
        item.SetPriceOptions(priceOptions);
        item.SetStartDate(12345);
        StringDictionaryEntryVector title;
        title.insert_or_assign("NEUTRAL", "titleVal");
        item.SetTitle(title);
        item.SetType(kPermanentItemType);

        m_state->permanentItem = item;
        
        return S_OK;
    })
#if HC_PLATFORM == HC_PLATFORM_WIN32
    .Then([&](Result<void>) -> AsyncOp<CreateDraftItemOperation::ResultType>
    {
        // Create a permanent buyable item since the catalog publishing has possibly long processing times that can and do cause intermittent test failures
        CreateDraftItemOperation::RequestType request;
        request.SetItem(m_state->permanentItem);
        request.SetPublish(true);

        return CreateDraftItemOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, initResult](Result<CreateDraftItemOperation::ResultType>) -> Result<void>
    {
        // We don't care if Creating/Publishing the permanent currency and item fails since it should already exist most times
        return *initResult;
    });
#else
    .Then([&, initResult](Result<void>) -> Result<void>
    {
        // We don't care if Creating/Publishing the permanent currency and item fails since it should already exist most times
        return *initResult;
    });
#endif
}

AsyncOp<void> InventoryTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void InventoryTests::TestAddInventoryItems(TestContext& tc)
{
    // Already covered in TestGetInventoryCollectionIds
    tc.EndTest(S_OK);
}

void InventoryTests::TestDeleteInventoryCollection(TestContext& tc)
{
    // Already covered in TestGetInventoryCollectionIds
    tc.EndTest(S_OK);
}

void InventoryTests::TestDeleteInventoryItems(TestContext& tc)
{
    // Already covered in TestExecuteInventoryOperations
    tc.EndTest(S_OK);
}

void InventoryTests::TestExecuteInventoryOperations(TestContext& tc)
{
    constexpr char kCollectionId[]{ "executeInventoryOperationsCollection" };

    ExecuteInventoryOperationsOperation::RequestType request;
    request.SetCollectionId(kCollectionId);
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    ModelVector<Wrappers::PFInventoryInventoryOperationWrapper<Allocator>> ops;
    Wrappers::PFInventoryAddInventoryItemsOperationWrapper<Allocator> addOp;
    Wrappers::PFInventoryInventoryOperationWrapper<Allocator> op;
    addOp.SetAmount(1);
    Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
    Wrappers::PFInventoryAlternateIdWrapper<Allocator> altId;
    altId.SetType(kFriendlyId);
    altId.SetValue(kPermanentItemId);
    item.SetAlternateId(altId);
    addOp.SetItem(item);
    op.SetAdd(addOp);
    ops.push_back(op);
    request.SetOperations(ops);

    ExecuteInventoryOperationsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ExecuteInventoryOperationsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        // Add transaction (and possible CollectionCreated transaction if this is the first run on a title)
        auto count = result.Payload().Model().transactionIdsCount;
        tc.AssertTrue(2u == count || 1u == count, "transactionIdsCount");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
        
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void InventoryTests::TestGetInventoryCollectionIds(TestContext& tc)
{
    constexpr char kCollectionId[]{ "getInventoryCollectionIdsCollection" };

    AddInventoryItem(DefaultTitlePlayer(), kCollectionId, RunContext(), tc).Then([&](Result<void> result) -> AsyncOp<GetInventoryCollectionIdsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        // Waiting a few seconds before continuing to ensure that item will be ready for other operations.
        Platform::Sleep(5000);

        GetInventoryCollectionIdsOperation::RequestType request;
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        request.SetCount(1);

        return GetInventoryCollectionIdsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetInventoryCollectionIdsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Can't guarantee which collections are returned
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.collectionIdsCount, "collectionIdsCount");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void InventoryTests::TestGetInventoryItems(TestContext& tc)
{
    constexpr char kCollectionId[]{ "getInventoryItemsCollection" };

    AddInventoryItem(DefaultTitlePlayer(), kCollectionId, RunContext(), tc).Then([&, kCollectionId](Result<void> result) -> AsyncOp<GetInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetInventoryItemsOperation::RequestType request;
        request.SetCollectionId(kCollectionId);
        request.SetEntity(DefaultTitlePlayer().EntityKey());

        return GetInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<GetInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        tc.AssertEqual(kPermanentItemType, model.items[0]->type, "items[0]->type");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void InventoryTests::TestGetMicrosoftStoreAccessTokens(TestContext& tc)
{
    // TODO: This test is currently failing due to the PlayFab service currently being reworked
    tc.Skip();
    /*GetMicrosoftStoreAccessTokensOperation::Run(DefaultTitlePlayer(), GetMicrosoftStoreAccessTokensOperation::RequestType{}, RunContext()).Then([&](Result<GetMicrosoftStoreAccessTokensOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        
        auto& model = result.Payload().Model();
        tc.AssertTrue(model.collectionsAccessToken, "collectionsAccessToken");
        tc.AssertTrue(model.collectionsAccessTokenExpirationDate, "collectionsAccessTokenExpirationDate");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });*/
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void InventoryTests::TestGetTransactionHistory(TestContext& tc)
{
    constexpr char kCollectionId[]{ "purchaseInventoryItemsCollection" };

    GetTransactionHistoryOperation::RequestType request;
    request.SetCollectionId(kCollectionId);
    request.SetEntity(DefaultTitlePlayer().EntityKey());

    GetTransactionHistoryOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<GetTransactionHistoryOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto count = result.Payload().Model().transactionsCount;
        tc.AssertTrue(count > 0u, "transactionsCount");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void InventoryTests::TestPurchaseInventoryItems(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    constexpr char kCollectionId[]{ "purchaseInventoryItemsCollection" };
    
    AddInventoryItemsOperation::RequestType request;
    request.SetAmount(kCurrencyAmount);
    request.SetCollectionId(kCollectionId);
    request.SetEntity(DefaultTitlePlayer().EntityKey());
    Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
    item.SetAlternateId(AltId(kPermanentCurrencyId));
    request.SetItem(item);

    AddInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<AddInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Add transaction (and possible CollectionCreated transaction if this is the first run on a title)
        auto count = result.Payload().Model().transactionIdsCount;
        tc.AssertTrue(2u == count || 1u == count, "transactionIdsCount");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<PurchaseInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        PurchaseInventoryItemsOperation::RequestType request;
        request.SetAmount(1);
        request.SetCollectionId(kCollectionId);
        request.SetDeleteEmptyStacks(true);
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
        item.SetAlternateId(AltId(kPermanentItemId));
        request.SetItem(item);
        ModelVector<Wrappers::PFInventoryPurchasePriceAmountWrapper<Allocator>> amounts;
        Wrappers::PFInventoryPurchasePriceAmountWrapper<Allocator> amount;
        amount.SetAmount(kCurrencyAmount);
        amount.SetItemId(m_state->currencyId);
        amounts.push_back(amount);
        request.SetPriceAmounts(amounts);

        return PurchaseInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<PurchaseInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Purchase transaction. The number of transactions returned can vary by platform but there should always be at least 1
        tc.AssertTrue(result.Payload().Model().transactionIdsCount >= 1u, "No transaction IDs returned");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    tc.Skip();
#endif
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void InventoryTests::TestRedeemAppleAppStoreInventoryItems(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void InventoryTests::TestRedeemGooglePlayInventoryItems(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void InventoryTests::TestRedeemMicrosoftStoreInventoryItems(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    constexpr char kCollectionId[]{ "redeemMicrosoftStoreInventoryItemsCollection" };

    assert(DefaultPlatformUser());
    RedeemMicrosoftStoreInventoryItemsOperation::RequestType request{ Wrappers::XUser::Duplicate(DefaultPlatformUser()->Handle()) };
    request.SetCollectionId(kCollectionId);
    request.SetEntity(DefaultTitlePlayer().EntityKey());
        
    return RedeemMicrosoftStoreInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext()).Finally([&](Result<RedeemMicrosoftStoreInventoryItemsOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    tc.Skip();
#endif
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX
void InventoryTests::TestRedeemNintendoEShopInventoryItems(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX
void InventoryTests::TestRedeemPlayStationStoreInventoryItems(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
void InventoryTests::TestRedeemSteamInventoryItems(TestContext& tc)
{
    const char* storeId = "100";
    const char* steamId = "76561199383523607";

    // https://partner.steamgames.com/doc/webapi/IInventoryService#AddItem
    const char* STEAMWORKS_ADDITEM_URL = "https://partner.steam-api.com/IInventoryService/AddItem/v1/";

    PFHCCallHandle callHandle{ nullptr };
    PFHCHttpCallCreate(&callHandle);
    PFHCHttpCallRequestSetUrl(callHandle, "POST", STEAMWORKS_ADDITEM_URL);
    PFHCHttpCallRequestSetHeader(callHandle, "Content-Type", "application/x-www-form-urlencoded", true);

    std::ostringstream payloadStringStream;
    payloadStringStream
        << "appid=" << m_testTitleData.steamAppId
        << "&key=" << m_testTitleData.steamPublisherKey
        << "&steamid=" << steamId
        << "&itemdefid[0]=" << storeId;

    PFHCHttpCallRequestSetRequestBodyString(callHandle, payloadStringStream.str().c_str());

    XAsyncBlock asyncBlock{};
    PFHCHttpCallPerformAsync(callHandle, &asyncBlock);
    PFHCHttpCallCloseHandle(callHandle);

    XAsyncGetStatus(&asyncBlock, true);

    GetTitlePlayer("SteamPlayerCustomID").Then([&](Result<Entity> result) -> AsyncOp<RedeemSteamInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        auto entity = result.Payload();
        RedeemSteamInventoryItemsOperation::RequestType request;
        return RedeemSteamInventoryItemsOperation::Run(entity, request, RunContext());
    })
    .Then([&](Result<RedeemSteamInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto count = result.Payload().Model().succeededCount;
        tc.AssertTrue(count > 0u, "transactionsCount");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

void InventoryTests::TestSubtractInventoryItems(TestContext& tc)
{
    constexpr char kCollectionId[]{ "subtractInventoryItemsCollection" };
    
    AddInventoryItem(DefaultTitlePlayer(), kCollectionId, RunContext(), tc).Then([&, kCollectionId](Result<void> result) -> AsyncOp<SubtractInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        SubtractInventoryItemsOperation::RequestType request;
        request.SetAmount(1);
        request.SetCollectionId(kCollectionId);
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
        item.SetAlternateId(AltId(kPermanentItemId));
        request.SetItem(item);

        return SubtractInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<SubtractInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Subtract transaction
        tc.AssertEqual(1u, result.Payload().Model().transactionIdsCount, "transactionIdsCount");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup. Specify 1 transaction since the item has already been subtracted
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext(), 1u);
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void InventoryTests::TestTransferInventoryItems(TestContext& tc)
{
    constexpr char kCollectionId[]{ "transferInventoryItemsCollection" };
    constexpr char kAltCollectionId[]{ "transferInventoryItemsAltCollection" };
    
    AddInventoryItem(DefaultTitlePlayer(), kCollectionId, RunContext(), tc).Then([&, kCollectionId, kAltCollectionId](Result<void> result) -> AsyncOp<TransferInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Waiting a few seconds before continuing to ensure that item will be ready for other operations.
        Platform::Sleep(5000);

        TransferInventoryItemsOperation::RequestType request;
        request.SetAmount(1);
        request.SetDeleteEmptyStacks(true);
        request.SetGivingCollectionId(kCollectionId);
        Wrappers::PFInventoryInventoryItemReferenceWrapper<Allocator> item;
        item.SetAlternateId(AltId(kPermanentItemId));
        request.SetGivingItem(item);
        request.SetReceivingCollectionId(kAltCollectionId);
        request.SetReceivingEntity(DefaultTitlePlayer().EntityKey());
        request.SetReceivingItem(item);

        return TransferInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<TransferInventoryItemsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        // Commenting assertions below due to known issues on the service side related to Transfers.

        // auto& model = result.Payload().Model();
        // Subtract and Delete transactions
        // tc.AssertEqual(2u, model.givingTransactionIdsCount, "givingTransactionIdsCount");
        
        // TransferExecuted transaction
        // tc.AssertEqual(1u, model.receivingTransactionIdsCount, "receivingTransactionIdsCount");

        return S_OK;
    })
    .Then([&, kAltCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kAltCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void InventoryTests::TestUpdateInventoryItems(TestContext& tc)
{
    constexpr char kCollectionId[]{ "updateInventoryItemsCollection" };

    AddInventoryItem(DefaultTitlePlayer(), kCollectionId, RunContext(), tc).Then([&, kCollectionId](Result<void> result) -> AsyncOp<GetInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        GetInventoryItemsOperation::RequestType request;
        request.SetCollectionId(kCollectionId);
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        request.SetCount(1);

        return GetInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, kCollectionId](Result<GetInventoryItemsOperation::ResultType> result) -> AsyncOp<UpdateInventoryItemsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.itemsCount, "itemsCount");
        tc.AssertEqual(kPermanentItemType, model.items[0]->type, "items[0]->type");

        UpdateInventoryItemsOperation::RequestType request;
        request.SetCollectionId(kCollectionId);
        request.SetEntity(DefaultTitlePlayer().EntityKey());
        Wrappers::PFInventoryInventoryItemWrapper<Allocator> updatedItem;
        updatedItem.SetId(model.items[0]->id);
        updatedItem.SetAmount(5);
        updatedItem.SetType(m_state->permanentItem.Model().type);
        request.SetItem(updatedItem);

        return UpdateInventoryItemsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<UpdateInventoryItemsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Update transaction
        tc.AssertEqual(1u, result.Payload().Model().transactionIdsCount, "transactionIdsCount");

        return S_OK;
    })
    .Then([&, kCollectionId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        return DeleteItem(DefaultTitlePlayer(), kCollectionId, tc, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}


}
}

#include "TestAppPch.h"
#include "InventoryTests.h"

namespace PlayFab
{
namespace Test
{

void InventoryTests::AddTests()
{
    AddTest("TestAddInventoryItems", &InventoryTests::TestAddInventoryItems);
    AddTest("TestDeleteInventoryCollection", &InventoryTests::TestDeleteInventoryCollection);
    AddTest("TestDeleteInventoryItems", &InventoryTests::TestDeleteInventoryItems);
    AddTest("TestExecuteInventoryOperations", &InventoryTests::TestExecuteInventoryOperations);
    AddTest("TestGetInventoryCollectionIds", &InventoryTests::TestGetInventoryCollectionIds);
    AddTest("TestGetInventoryItems", &InventoryTests::TestGetInventoryItems);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetMicrosoftStoreAccessTokens", &InventoryTests::TestGetMicrosoftStoreAccessTokens);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetTransactionHistory", &InventoryTests::TestGetTransactionHistory);
#endif
    AddTest("TestPurchaseInventoryItems", &InventoryTests::TestPurchaseInventoryItems);
#if HC_PLATFORM == HC_PLATFORM_IOS
    AddTest("TestRedeemAppleAppStoreInventoryItems", &InventoryTests::TestRedeemAppleAppStoreInventoryItems);
#endif
#if HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestRedeemGooglePlayInventoryItems", &InventoryTests::TestRedeemGooglePlayInventoryItems);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestRedeemMicrosoftStoreInventoryItems", &InventoryTests::TestRedeemMicrosoftStoreInventoryItems);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestRedeemNintendoEShopInventoryItems", &InventoryTests::TestRedeemNintendoEShopInventoryItems);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestRedeemPlayStationStoreInventoryItems", &InventoryTests::TestRedeemPlayStationStoreInventoryItems);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestRedeemSteamInventoryItems", &InventoryTests::TestRedeemSteamInventoryItems);
#endif
    AddTest("TestSubtractInventoryItems", &InventoryTests::TestSubtractInventoryItems);
    AddTest("TestTransferInventoryItems", &InventoryTests::TestTransferInventoryItems);
    AddTest("TestUpdateInventoryItems", &InventoryTests::TestUpdateInventoryItems);

}

}
}

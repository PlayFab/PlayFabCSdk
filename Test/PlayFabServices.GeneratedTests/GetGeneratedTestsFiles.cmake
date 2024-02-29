cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabServices.GeneratedTests.vcxitems
function(GET_PLAYFAB_SERVICES_GENERATED_TESTS_SOURCE_FILES
         OUT_PLAYFAB_SERVICES_GENERATED_TESTS_SOURCE_FILES
         PATH_TO_SOURCE
         )

    set(${OUT_PLAYFAB_SERVICES_GENERATED_TESTS_SOURCE_FILES}
        "${PATH_TO_SOURCE}/AccountManagementTests.cpp"
        "${PATH_TO_SOURCE}/CatalogTests.cpp"
        "${PATH_TO_SOURCE}/CloudScriptTests.cpp"
        "${PATH_TO_SOURCE}/DataTests.cpp"
        "${PATH_TO_SOURCE}/ExperimentationTests.cpp"
        "${PATH_TO_SOURCE}/FriendsTests.cpp"
        "${PATH_TO_SOURCE}/GroupsTests.cpp"
        "${PATH_TO_SOURCE}/InventoryTests.cpp"
        "${PATH_TO_SOURCE}/LocalizationTests.cpp"
        "${PATH_TO_SOURCE}/MultiplayerServerTests.cpp"
        "${PATH_TO_SOURCE}/PlatformSpecificTests.cpp"
        "${PATH_TO_SOURCE}/PlayerDataManagementTests.cpp"
        "${PATH_TO_SOURCE}/ProfilesTests.cpp"
        "${PATH_TO_SOURCE}/PushNotificationsTests.cpp"
        "${PATH_TO_SOURCE}/SegmentsTests.cpp"
        "${PATH_TO_SOURCE}/TitleDataManagementTests.cpp"
        "${PATH_TO_SOURCE}/AccountManagementTests.h"
        "${PATH_TO_SOURCE}/CatalogTests.h"
        "${PATH_TO_SOURCE}/CloudScriptTests.h"
        "${PATH_TO_SOURCE}/DataTests.h"
        "${PATH_TO_SOURCE}/ExperimentationTests.h"
        "${PATH_TO_SOURCE}/FriendsTests.h"
        "${PATH_TO_SOURCE}/GroupsTests.h"
        "${PATH_TO_SOURCE}/InventoryTests.h"
        "${PATH_TO_SOURCE}/LocalizationTests.h"
        "${PATH_TO_SOURCE}/MultiplayerServerTests.h"
        "${PATH_TO_SOURCE}/PlatformSpecificTests.h"
        "${PATH_TO_SOURCE}/PlayerDataManagementTests.h"
        "${PATH_TO_SOURCE}/ProfilesTests.h"
        "${PATH_TO_SOURCE}/PushNotificationsTests.h"
        "${PATH_TO_SOURCE}/SegmentsTests.h"
        "${PATH_TO_SOURCE}/TitleDataManagementTests.h"
        PARENT_SCOPE
        )

endfunction()

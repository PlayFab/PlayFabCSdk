cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabServices.GeneratedApiWrappers.vcxitems
function(GET_PLAYFAB_SERVICES_GENERATED_API_WRAPPERS_FILES
         OUT_PLAYFAB_SERVICES_GENERATED_API_WRAPPERS_FILES
         PATH_TO_SOURCE
         )

    set(${OUT_PLAYFAB_SERVICES_GENERATED_API_WRAPPERS_FILES}
        "${PATH_TO_SOURCE}/FriendsOperations.h"
        "${PATH_TO_SOURCE}/FriendsOperations.cpp"
        "${PATH_TO_SOURCE}/AccountManagementOperations.h"
        "${PATH_TO_SOURCE}/AccountManagementOperations.cpp"
        "${PATH_TO_SOURCE}/PlatformSpecificOperations.h"
        "${PATH_TO_SOURCE}/PlatformSpecificOperations.cpp"
        "${PATH_TO_SOURCE}/CloudScriptOperations.h"
        "${PATH_TO_SOURCE}/CloudScriptOperations.cpp"
        "${PATH_TO_SOURCE}/SegmentsOperations.h"
        "${PATH_TO_SOURCE}/SegmentsOperations.cpp"
        "${PATH_TO_SOURCE}/TitleDataManagementOperations.h"
        "${PATH_TO_SOURCE}/TitleDataManagementOperations.cpp"
        "${PATH_TO_SOURCE}/PlayerDataManagementOperations.h"
        "${PATH_TO_SOURCE}/PlayerDataManagementOperations.cpp"
        "${PATH_TO_SOURCE}/PushNotificationsOperations.h"
        "${PATH_TO_SOURCE}/PushNotificationsOperations.cpp"
        "${PATH_TO_SOURCE}/DataOperations.h"
        "${PATH_TO_SOURCE}/DataOperations.cpp"
        "${PATH_TO_SOURCE}/InventoryOperations.h"
        "${PATH_TO_SOURCE}/InventoryOperations.cpp"
        "${PATH_TO_SOURCE}/CatalogOperations.h"
        "${PATH_TO_SOURCE}/CatalogOperations.cpp"
        "${PATH_TO_SOURCE}/ExperimentationOperations.h"
        "${PATH_TO_SOURCE}/ExperimentationOperations.cpp"
        "${PATH_TO_SOURCE}/GroupsOperations.h"
        "${PATH_TO_SOURCE}/GroupsOperations.cpp"
        "${PATH_TO_SOURCE}/LocalizationOperations.h"
        "${PATH_TO_SOURCE}/LocalizationOperations.cpp"
        "${PATH_TO_SOURCE}/MultiplayerServerOperations.h"
        "${PATH_TO_SOURCE}/MultiplayerServerOperations.cpp"
        "${PATH_TO_SOURCE}/ProfilesOperations.h"
        "${PATH_TO_SOURCE}/ProfilesOperations.cpp"
        PARENT_SCOPE
        )

endfunction()

cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabServices.Common.vcxitems
function(GET_COMMON_SOURCE_FILES
         OUT_COMMON_SOURCE_FILES
         PATH_TO_SOURCE
         )

set(${OUT_COMMON_SOURCE_FILES}
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Api/PFServices.cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/GlobalState.cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/HttpClient.cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/Platform.cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/stdafx.cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Include/Generated/playfab/services/PFServices.h"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/GlobalState.h"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/HttpClient.h"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common/Platform.h"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/stdafx.h"
    PARENT_SCOPE
    )

endfunction()

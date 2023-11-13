cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabCore.ServiceWrappers.vcxitems
function(GET_SERVICE_WRAPPER_SOURCE_FILES
         OUT_SERVICE_WRAPPER_SOURCE_FILES
         PATH_TO_SOURCE
         )

set(${OUT_SERVICE_WRAPPER_SOURCE_FILES}
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/PFTypes.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/cpp/TypeWrappers.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Types.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Types.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/PFAuthenticationTypes.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/cpp/AuthenticationTypeWrappers.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/AuthenticationTypes.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/AuthenticationTypes.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Authentication.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Authentication.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/PFAuthentication.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/PFAuthentication.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/PFEventsTypes.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/cpp/EventsTypeWrappers.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/EventsTypes.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/EventsTypes.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Events.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/Events.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated/playfab/core/PFEvents.h"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Generated/PFEvents.cpp"
    PARENT_SCOPE
    )

endfunction()

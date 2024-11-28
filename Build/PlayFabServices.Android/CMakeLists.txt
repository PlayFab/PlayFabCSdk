cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ../libHttpClient)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)
endif()

project("PlayFabServices.Android")

# For PlayFab, we need to compile with c++20 rather than c++17 because of an issue with std::optional discussed here:
# https://discourse.llvm.org/t/code-which-compiles-with-g-but-not-with-clang/56868/4
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-z,now")
set(CMAKE_CXX_FLAGS_RELEASE "-gdwarf-5")
set(CMAKE_C_FLAGS_RELEASE "-gdwarf-5")

##################################
### Set up binary output paths ###
##################################

# Set final shared libraries output folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Debug/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Release/${PROJECT_NAME})

# Set final static libraries output folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Debug/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Release/${PROJECT_NAME})

# Set final runtime output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Debug/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/Release/${PROJECT_NAME})

###########################################
### Set up paths for source and include ###
###########################################

include("../PlayFabServices.CMake/GetCommonSourceFiles.cmake")
get_common_source_files(
    COMMON_SOURCE_FILES
    "${PATH_TO_SOURCE}"
)

include("../PlayFabServices.CMake/GetServiceWrapperFiles.cmake")
get_service_wrapper_source_files(
    SERVICE_WRAPPER_SOURCE_FILES
    "${PATH_TO_SOURCE}"
)

set(COMMON_INCLUDE_DIRS
    "${PATH_TO_SOURCE}/PlayFabCore/Include"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabServices/Source"
    "${PATH_TO_SOURCE}/PlayFabServices/Source/Common"
    "${PATH_TO_SOURCE}/PlayFabServices/Include"
    "${PATH_TO_SOURCE}/PlayFabServices/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_ROOT}/External/nlohmann/include"
    "${PATH_TO_LHC}/Include"
)

#########################
### Set up shared lib ###
#########################

# We'll usually want to build for full, but adding a case here if we want to build just for x84_64 or arm8
# TODO: Full flavor should just trigger the other two flavors. Weird that it creates a full directory. Something to look into.
# IF (EXISTS "${PATH_TO_ROOT}/Out/Android/PlayFabCore.Android/intermediates/cmake/full${CMAKE_BUILD_TYPE}")
#     add_subdirectory("../PlayFabCore.Android" ${PATH_TO_ROOT}/Out/Android/PlayFabCore.Android/intermediates/cmake/full${CMAKE_BUILD_TYPE}/obj/${ANDROID_ABI})
# ELSE ()
#     add_subdirectory("../PlayFabCore.Android" ${PATH_TO_ROOT}/Out/Android/PlayFabCore.Android/intermediates/cmake/${ANDROID_ABI}${CMAKE_BUILD_TYPE}/obj/${ANDROID_ABI})
# endif()

set(PlayFabSharedInternal.Android ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/PlayFabSharedInternal.Android/PlayFabSharedInternal.Android.a)
set(PlayFabCore.Android ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/PlayFabCore.Android/libPlayFabCore.Android.so)

# libHttpClient Shared Lib
string(TOLOWER "${CMAKE_BUILD_TYPE}" PATH_FLAVOR)
set(libHttpClient ${PATH_TO_LHC}/Binaries/Android/libHttpClient.Android/intermediates/cmake/${PATH_FLAVOR}/obj/${ANDROID_ABI}/libHttpClient.Android.so)

add_library(
    "PlayFabServices.Android"
    SHARED
    "${SERVICE_WRAPPER_SOURCE_FILES}"
    "${COMMON_SOURCE_FILES}"
)

target_include_directories(
    "PlayFabServices.Android"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
)

target_link_libraries(
    PlayFabServices.Android
    ${PlayFabSharedInternal.Android}
    ${PlayFabCore.Android}
    ${libHttpClient}
    ${log-lib}
)

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "PlayFabServices.Android"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
)

cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ../libHttpClient)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)
endif()

project("PlayFabCore.Android")

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

include("../PlayFabCore.CMake/GetCommonSourceFiles.cmake")
get_common_source_files(
    COMMON_SOURCE_FILES
    "${PATH_TO_SOURCE}"
)

include("../PlayFabCore.CMake/GetServiceWrapperFiles.cmake")
get_service_wrapper_source_files(
    SERVICE_WRAPPER_SOURCE_FILES
    "${PATH_TO_SOURCE}"
)

set(COMMON_INCLUDE_DIRS
    "${PATH_TO_SOURCE}/PlayFabCore/Source"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Common"
    "${PATH_TO_SOURCE}/PlayFabCore/Include"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_ROOT}/External/nlohmann/include"
    "${PATH_TO_LHC}/Include"
)

#########################
### Set up shared lib ###
#########################

set(PlayFabSharedInternal.Android ${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/PlayFabSharedInternal.Android/PlayFabSharedInternal.Android.a)

# libHttpClient Shared Lib
string(TOLOWER "${CMAKE_BUILD_TYPE}" PATH_FLAVOR)
set(libHttpClient ${PATH_TO_LHC}/Binaries/Android/libHttpClient.Android/intermediates/cmake/${PATH_FLAVOR}/obj/${ANDROID_ABI}/libHttpClient.Android.so)

add_library(
    "PlayFabCore.Android"
    SHARED
    "${SERVICE_WRAPPER_SOURCE_FILES}"
    "${COMMON_SOURCE_FILES}"
)

target_include_directories(
    "PlayFabCore.Android"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
)

find_library( # Sets the name of the path variable.
    log-lib

    # Specifies the name of the NDK library that
    # you want CMake to locate.
    log
)

find_library(
    zlib
    z
)

target_link_libraries(
    PlayFabCore.Android
    ${PlayFabSharedInternal.Android}
    ${libHttpClient}
    ${log-lib}
    ${zlib}
)

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "PlayFabCore.Android"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
)
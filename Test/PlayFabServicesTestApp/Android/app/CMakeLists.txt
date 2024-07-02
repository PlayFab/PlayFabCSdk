cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../../.." ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../../../Source" ABSOLUTE)
get_filename_component(PATH_TO_TEST "../../.." ABSOLUTE)

set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)

project("PlayFabSDKTestApp.Android")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

###########################################
### Set up paths for source and include ###
###########################################

# Set final output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabTestApp.Android)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabTestApp.Android)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabTestApp.Android)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabTestApp.Android)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabTestApp.Android)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabTestApp.Android)

# BINARY_DIR is a temp folder used by cmake itself.
# Binary folder can be remove freely.
# See more details: https://cmake.org/cmake/help/v3.4/command/add_subdirectory.html
set(BINARY_DIR ${PATH_TO_ROOT}/Int/CMake/PlayFabTestApp.Android)

include("../../GetPlayFabServicesTestAppCommon.cmake")
get_playfab_services_test_app_commmon_files(
    TEST_APP_COMMON_SOURCE_FILES
    "${PATH_TO_TEST}/PlayFabServicesTestApp"
    )

include("../../../PlayFabServices.GeneratedTests/GetGeneratedTestsFiles.cmake")
get_playfab_services_generated_tests_source_files(
    GENERATED_TESTS_SOURCE_FILES
    "${PATH_TO_TEST}/PlayFabServices.GeneratedTests"
    )

include("../../../PlayFabServices.GeneratedApiWrappers/GetGeneratedApiWrappers.cmake")
get_playfab_services_generated_api_wrappers_files(
    GENERATED_API_WRAPPER_FILES
    "${PATH_TO_TEST}/PlayFabServices.GeneratedApiWrappers"
    )

set(COMMON_INCLUDE_DIRS
    "${PATH_TO_SOURCE}/PlayFabCore/Include"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabServices/Include"
    "${PATH_TO_SOURCE}/PlayFabServices/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_LHC}/Include"
    "${PATH_TO_ROOT}/External/rapidjson/include"
    "${PATH_TO_TEST}/PlayFabServicesTestApp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/TestFramework"
    "${PATH_TO_TEST}/PlayFabServices.GeneratedApiWrappers"
    "${PATH_TO_TEST}/PlayFabServices.GeneratedTests"
)

#########################
### Set up exe        ###
#########################

add_library(
    "${PROJECT_NAME}"
    SHARED
    "${TEST_APP_COMMON_SOURCE_FILES}"
    "${GENERATED_TESTS_SOURCE_FILES}"
    "${GENERATED_API_WRAPPER_FILES}"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/AndroidTestApp.cpp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/AndroidTestApp.h"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/AndroidTestAppJni.cpp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/AndroidTestAppJni.h"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/PlatformUtilsAndroid.cpp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/PlatformUtilsAndroid.h"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Android/PlayFabAndroid.cpp"
)

if (${ANDROID_ABI} STREQUAL "x86_64") 
    set(ANDROID_ABI_DIRECTORY "x86_64")
else()
    set(ANDROID_ABI_DIRECTORY "arm8")
endif()

IF (EXISTS "${PATH_TO_ROOT}/Out/Android/PlayFabServices.Android/intermediates/cmake/full${CMAKE_BUILD_TYPE}")
    set(ANDROID_ABI_DIRECTORY "full")
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)

set(PLAYFAB_SERVICES_PATH "${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/PlayFabServices.Android/libPlayFabServices.Android.so")

set(PLAYFAB_CORE_PATH "${PATH_TO_ROOT}/Out/Android/${ANDROID_ABI}/${CMAKE_BUILD_TYPE}/PlayFabCore.Android/libPlayFabCore.Android.so")

# libHttpClient Shared Lib
string(TOLOWER "${CMAKE_BUILD_TYPE}" PATH_FLAVOR)
set(LIBHTTPCLIENT_PATH ${PATH_TO_LHC}/Binaries/Android/libHttpClient.Android/intermediates/cmake/${PATH_FLAVOR}/obj/${ANDROID_ABI}/libHttpClient.Android.so)

find_library(ANDROID_LOG_PATH log) # Android log library

target_link_libraries(
    ${PROJECT_NAME} 
    ${PLAYFAB_SERVICES_PATH}
    ${PLAYFAB_CORE_PATH}
    ${LIBHTTPCLIENT_PATH}
    ${ANDROID_LOG_PATH}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${COMMON_INCLUDE_DIRS}
)

include("${PATH_TO_ROOT}/Build/PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("${PATH_TO_ROOT}/Build/PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    ${PROJECT_NAME}
    ${FLAGS}
    ${FLAGS_DEBUG}
    ${FLAGS_RELEASE}
)

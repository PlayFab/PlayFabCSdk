cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ${LHC_PATH})
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/${LHC_PATH})
endif()

project("PlayFabCoreLinux")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fvisibility=hidden")
set(CMAKE_C_FLAGS "-fvisibility=hidden")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

###########################################
### Set up paths for source and include ###
###########################################

# Set final static libraries output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabCoreLinux)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabCoreLinux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabCoreLinux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabCoreLinux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabCoreLinux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabCoreLinux)

# BINARY_DIR is a temp folder used by cmake itself.
# Binary folder can be remove freely.
# See more details: https://cmake.org/cmake/help/v3.4/command/add_subdirectory.html
set(BINARY_DIR ${PATH_TO_ROOT}/Int/CMake/PlayFabCoreLinux)

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
    "${PATH_TO_SOURCE}/PlayFabCore/Include/playfab/core/cpp"
    "${PATH_TO_SOURCE}/PlayFabServices/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_ROOT}/External/rapidjson/include/rapidjson"
    "${PATH_TO_ROOT}/External/rapidjson/include"
    "${PATH_TO_LHC}/Include"
    )

set(LIBHTTPCLIENT_BINARY_PATH
    ${PATH_TO_LHC}/Binaries/${CMAKE_BUILD_TYPE}/x64/libHttpClient.Linux.C/libHttpClient.Linux.C.a
)
message(STATUS "LIBHTTPCLIENT_BINARY_PATH: ${LIBHTTPCLIENT_BINARY_PATH}")

set(LIBCRYPTO_BINARY_PATH
    ${PATH_TO_LHC}/Binaries/${CMAKE_BUILD_TYPE}/x64/libcrypto.Linux/libcrypto.a
)
message(STATUS "LIBCRYPTO_BINARY_PATH: ${LIBCRYPTO_BINARY_PATH}")

set(LIBSSL_BINARY_PATH
    ${PATH_TO_LHC}/Binaries/${CMAKE_BUILD_TYPE}/x64/libssl.Linux/libssl.a
)
message(STATUS "LIBSSL_BINARY_PATH: ${LIBSSL_BINARY_PATH}")

set(LIBCURL_BINARY_PATH
    ${PATH_TO_LHC}/Binaries/${CMAKE_BUILD_TYPE}/x64/libcurl.Linux/libcurl.a
)
message(STATUS "LIBCURL_BINARY_PATH: ${LIBCURL_BINARY_PATH}")

set(PLAYFAB_SHARED_INTERNAL_BINARY_PATH
    ${PATH_TO_ROOT}/Out/${CMAKE_BUILD_TYPE}/x64/PlayFabSharedInternal.Linux/PlayFabSharedInternal.Linux.a
)
message(STATUS "PLAYFAB_SHARED_INTERNAL_BINARY_PATH: ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}")

set(PLAYFAB_CORE_INTERNAL_BINARY_PATH
    ${PATH_TO_ROOT}/Out/${CMAKE_BUILD_TYPE}/x64/PlayFabCoreLinux/PlayFabCoreStatic.Linux.a
)
message(STATUS "PLAYFAB_CORE_INTERNAL_BINARY_PATH: ${PLAYFAB_CORE_INTERNAL_BINARY_PATH}")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DDBG -D_DBG -DDEBUG -D_DEBUG")
endif()
    
#########################
### Set up static lib ###
#########################

# make PlayFabCoreStatic.Linux.a 
add_library(
    "PlayFabCoreStatic.Linux"
    STATIC
    "${SERVICE_WRAPPER_SOURCE_FILES}"
    "${COMMON_SOURCE_FILES}"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/LocalStorage_Generic.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/Platform_Generic.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/LocalStorage_Generic.h"
    )

target_include_directories(
    "PlayFabCoreStatic.Linux"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
    )

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "PlayFabCoreStatic.Linux"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
    )

# make libPlayFabCoreLinux.so
# PLAYFAB_SHARED_INTERNAL_BINARY_PATH appears twice in target_link_libraries due to circular dependency
# https://stackoverflow.com/questions/12204820/cmake-and-order-dependent-linking-of-shared-libraries
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

add_library("${PROJECT_NAME}" SHARED main.cpp)
target_link_libraries("${PROJECT_NAME}" 
    PUBLIC ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
    PUBLIC ${PLAYFAB_CORE_INTERNAL_BINARY_PATH}
    PUBLIC ${LIBCURL_BINARY_PATH}
    PUBLIC ${LIBHTTPCLIENT_BINARY_PATH}
    PUBLIC ${LIBSSL_BINARY_PATH}
    PUBLIC ${LIBCRYPTO_BINARY_PATH} 
    PUBLIC ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include/Generated>
)
export(TARGETS ${PROJECT_NAME} FILE PlayFabCoreLinuxConfig.cmake)


cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ${LHC_PATH})
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/${LHC_PATH})
endif()

project("PlayFabServicesLinux")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fvisibility=hidden")
set(CMAKE_C_FLAGS "-fvisibility=hidden")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

###########################################
### Set up paths for source and include ###
###########################################

# Set final output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabServicesLinux)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabServicesLinux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabServicesLinux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabServicesLinux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/Debug/x64/PlayFabServicesLinux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/Release/x64/PlayFabServicesLinux)

# BINARY_DIR is a temp folder used by cmake itself.
# Binary folder can be remove freely.
# See more details: https://cmake.org/cmake/help/v3.4/command/add_subdirectory.html
set(BINARY_DIR ${PATH_TO_ROOT}/Int/CMake/PlayFabServicesLinux)

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
    "${PATH_TO_SOURCE}/PlayFabServices/Include/playfab/core/cpp"
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

set(PLAYFAB_SERVICES_INTERNAL_BINARY_PATH
    ${PATH_TO_ROOT}/Out/${CMAKE_BUILD_TYPE}/x64/PlayFabServicesLinux/PlayFabServicesStatic.Linux.a
)
message(STATUS "PLAYFAB_SERVICES_INTERNAL_BINARY_PATH: ${PLAYFAB_SERVICES_INTERNAL_BINARY_PATH}")

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DDBG -D_DBG -DDEBUG -D_DEBUG")
endif()

# make PlayFabServicesStatic.Linux.a 
add_library(
    "PlayFabServicesStatic.Linux"
    STATIC
    "${SERVICE_WRAPPER_SOURCE_FILES}"
    "${COMMON_SOURCE_FILES}"
    )

target_include_directories(
    "PlayFabServicesStatic.Linux"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
    )

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "PlayFabServicesStatic.Linux"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
    )

# make libPlayFabServicesLinux.so
# PLAYFAB_SHARED_INTERNAL_BINARY_PATH appears twice in target_link_libraries due to circular dependency
# https://stackoverflow.com/questions/12204820/cmake-and-order-dependent-linking-of-shared-libraries
add_library("${PROJECT_NAME}" SHARED main.cpp)
target_link_libraries("${PROJECT_NAME}" 
    PUBLIC ${PLAYFAB_CORE_INTERNAL_BINARY_PATH}
    PUBLIC ${PLAYFAB_SERVICES_INTERNAL_BINARY_PATH}
    PUBLIC ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
    PUBLIC ${LIBHTTPCLIENT_BINARY_PATH}
    PUBLIC ${LIBCURL_BINARY_PATH}
    PUBLIC ${LIBSSL_BINARY_PATH}
    PUBLIC ${LIBCRYPTO_BINARY_PATH} 
    PUBLIC ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
    PUBLIC ${PLAYFAB_CORE_INTERNAL_BINARY_PATH}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include/Generated>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabServices/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabServices/Include/Generated>
	$<BUILD_INTERFACE:${PATH_TO_LHC}/Include>
)
export(TARGETS ${PROJECT_NAME} FILE PlayFabServicesLinuxConfig.cmake)



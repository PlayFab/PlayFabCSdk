cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ${LHC_PATH})
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/${LHC_PATH})
endif()

project("PlayFabCore.Linux")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fvisibility=hidden")
set(CMAKE_C_FLAGS "-fvisibility=hidden")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-z,now") # https://www.redhat.com/en/blog/hardening-elf-binaries-using-relocation-read-only-relro

###########################################
### Set up paths for source and include ###
###########################################

# Set final static libraries output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/${PROJECT_NAME})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/${PROJECT_NAME})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/${PROJECT_NAME})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/${PROJECT_NAME})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/${PROJECT_NAME})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/${PROJECT_NAME})

# BINARY_DIR is a temp folder used by cmake itself.
# Binary folder can be remove freely.
# See more details: https://cmake.org/cmake/help/v3.4/command/add_subdirectory.html
set(BINARY_DIR ${PATH_TO_ROOT}/Int/CMake/${PROJECT_NAME})

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
    ${PATH_TO_LHC}/Out/x64/${CMAKE_BUILD_TYPE}/libHttpClient.Linux/libHttpClient.Linux.so
)
message(STATUS "LIBHTTPCLIENT_BINARY_PATH: ${LIBHTTPCLIENT_BINARY_PATH}")

set(PLAYFAB_SHARED_INTERNAL_BINARY_PATH
    ${PATH_TO_ROOT}/Out/x64/${CMAKE_BUILD_TYPE}/PlayFabSharedInternal.Linux/PlayFabSharedInternal.Linux.a
)
message(STATUS "PLAYFAB_SHARED_INTERNAL_BINARY_PATH: ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}")

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DDBG -D_DBG -DDEBUG -D_DEBUG")
endif()
    
#########################
### Set up shared lib ###
#########################
# make ${PROJECT_NAME}.so
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

add_library(
    "${PROJECT_NAME}"
    SHARED
    "${SERVICE_WRAPPER_SOURCE_FILES}"
    "${COMMON_SOURCE_FILES}"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/LocalStorage_Generic.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/Platform_Generic.cpp"
    "${PATH_TO_SOURCE}/PlayFabCore/Source/Platform/Generic/LocalStorage_Generic.h"
    )

target_link_libraries("${PROJECT_NAME}" 
    PRIVATE ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
    PRIVATE ${LIBHTTPCLIENT_BINARY_PATH}
)

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    ${PROJECT_NAME}
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
    )

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
    )

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include/Generated>
)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)


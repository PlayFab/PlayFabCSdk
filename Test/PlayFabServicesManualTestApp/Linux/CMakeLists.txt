cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../.." ABSOLUTE)

project("TestApp")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Removing -D_DEBUG flag due to a crash that's happening on Debug,
# The crash seems to be related with RTTI symbols not being loaded despite we're including the -frtti flag
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DDBG -D_DBG -DDEBUG")
endif()

###########################################
### Set up paths for source and include ###
###########################################

# Set final output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG Out/TestApp)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE Out/TestApp)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG Out/TestApp)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE Out/TestApp)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG Out/TestApp)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE Out/TestApp)
set(BINARY_DIR Int/TestApp)

#########################
### Set up exe        ###
#########################

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

add_executable("${PROJECT_NAME}")

# -------------------------------------------------------------------------------------------------
# Build against binaries built from source. This will still require PlayFabServices.Linux and its dependencies
# to be built separately. See ${PATH_TO_ROOT}/Build/PlayFabServices.Linux/README.md for details.

set(libHttpClient.Linux_DIR "${LHC_PATH}/Int/CMake/libHttpClient.Linux")
set(PlayFabCore.Linux_DIR "${PATH_TO_ROOT}/Int/CMake/PlayFabCore.Linux")
set(PlayFabServices.Linux_DIR "${PATH_TO_ROOT}/Int/CMake/PlayFabServices.Linux")

find_package(PlayFabServices.Linux REQUIRED)
find_package(PlayFabCore.Linux REQUIRED)
find_package(libHttpClient.Linux REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE libHttpClient.Linux
    PRIVATE PlayFabCore.Linux
    PRIVATE PlayFabServices.Linux
    )

# Set up include directories.  Using nlohmann so needed to add that include path
target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
	"${PATH_TO_ROOT}/External/nlohmann/include"
    "${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/include" # Required if building against release package
    )
# -------------------------------------------------------------------------------------------------


# -------------------------------------------------------------------------------------------------
# Building against a binary release at ${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/
# Use the following section rather than the "build from source" section above.

#target_link_libraries(${PROJECT_NAME}
#    PRIVATE ${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/bin/libHttpClient.Linux.so
#    PRIVATE ${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/bin/PlayFabCore.Linux.so
#    PRIVATE ${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/bin/PlayFabServices.Linux.so
#    )

# Set up include directories.  Using nlohmann so needed to add that include path
#target_include_directories(
#    "${PROJECT_NAME}"
#    PRIVATE
#	"${PATH_TO_ROOT}/External/nlohmann/include"
#    "${PATH_TO_ROOT}/../PlayFabServicesLinuxSDK/include" # Required if building against release package
#    )
# -------------------------------------------------------------------------------------------------

target_sources(
    "${PROJECT_NAME}"
    PRIVATE
    "main.cpp"
)

include("${PATH_TO_ROOT}/Build/PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("${PATH_TO_ROOT}/Build/PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "${PROJECT_NAME}"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
    )


cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../.." ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../../Source" ABSOLUTE)
get_filename_component(PATH_TO_TEST "../.." ABSOLUTE)

get_filename_component(PATH_TO_LHC ${LHC_PATH} ABSOLUTE)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    get_filename_component(PATH_TO_LHC ${PATH_TO_ROOT}/${LHC_PATH} ABSOLUTE)
endif()

project("PlayFabTestApp.Linux")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fvisibility=hidden")
set(CMAKE_C_FLAGS "-fvisibility=hidden")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DDBG -D_DBG -DDEBUG -D_DEBUG")
endif()

###########################################
### Set up paths for source and include ###
###########################################

# Set final output folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/PlayFabTestApp.Linux)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/PlayFabTestApp.Linux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/PlayFabTestApp.Linux)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/PlayFabTestApp.Linux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PATH_TO_ROOT}/Out/x64/Debug/PlayFabTestApp.Linux)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PATH_TO_ROOT}/Out/x64/Release/PlayFabTestApp.Linux)


# BINARY_DIR is a temp folder used by cmake itself.
# Binary folder can be remove freely.
# See more details: https://cmake.org/cmake/help/v3.4/command/add_subdirectory.html
set(BINARY_DIR ${PATH_TO_ROOT}/Int/CMake/PlayFabTestApp.Linux)

include("../GetPlayFabServicesTestAppCommon.cmake")
get_playfab_services_test_app_commmon_files(
    TEST_APP_COMMON_SOURCE_FILES
    "${PATH_TO_TEST}/PlayFabServicesTestApp"
    )

include("../../PlayFabServices.GeneratedTests/GetGeneratedTestsFiles.cmake")
get_playfab_services_generated_tests_source_files(
    GENERATED_TESTS_SOURCE_FILES
    "${PATH_TO_TEST}/PlayFabServices.GeneratedTests"
    )

include("../../PlayFabServices.GeneratedApiWrappers/GetGeneratedApiWrappers.cmake")
get_playfab_services_generated_api_wrappers_files(
    GENERATED_API_WRAPPER_FILES
    "${PATH_TO_TEST}/PlayFabServices.GeneratedApiWrappers"
    )

set(COMMON_INCLUDE_DIRS
    "${PATH_TO_ROOT}/External/rapidjson/include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_LHC}/Include"
    "${PATH_TO_TEST}/PlayFabServicesTestApp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/TestFramework"
    "${PATH_TO_TEST}/PlayFabServices.GeneratedApiWrappers"
    "${PATH_TO_TEST}/PlayFabServices.GeneratedTests"
    )

#########################
### Set up exe        ###
#########################

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")

add_executable("${PROJECT_NAME}")

set(libHttpClient.Linux_DIR "${LHC_PATH}/Int/CMake/libHttpClient.Linux")
set(PlayFabCore.Linux_DIR "${PATH_TO_ROOT}/Int/CMake/PlayFabCore.Linux")
set(PlayFabServices.Linux_DIR "${PATH_TO_ROOT}/Int/CMake/PlayFabServices.Linux")

set(PLAYFAB_SHARED_INTERNAL_BINARY_PATH
    ${PATH_TO_ROOT}/Out/x64/${CMAKE_BUILD_TYPE}/PlayFabSharedInternal.Linux/PlayFabSharedInternal.Linux.a
)

find_package(libHttpClient.Linux REQUIRED)
find_package(PlayFabCore.Linux REQUIRED)
find_package(PlayFabServices.Linux REQUIRED)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE libHttpClient.Linux
    PRIVATE PlayFabCore.Linux
    PRIVATE PlayFabServices.Linux
    PRIVATE ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}
    )
 
target_sources(
    "${PROJECT_NAME}"
    PRIVATE
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Linux/MainLinux.cpp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Linux/PlayFabLinux.cpp"
    "${PATH_TO_TEST}/PlayFabServicesTestApp/Platform/Linux/PlatfromUtilsLinux.cpp"
    "${TEST_APP_COMMON_SOURCE_FILES}"
    "${GENERATED_TESTS_SOURCE_FILES}"
    "${GENERATED_API_WRAPPER_FILES}"
)

target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
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

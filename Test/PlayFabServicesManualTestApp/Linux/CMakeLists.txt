cmake_minimum_required(VERSION 3.6)
project("TestApp")

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

find_package(PlayFabServicesLinux REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC PlayFabServicesLinux)
 
target_sources(
    "${PROJECT_NAME}"
    PRIVATE
    "main.cpp"
)

# Set up include directories.  Using rapidjson so needed to add that include path
get_filename_component(PATH_TO_ROOT "../../.." ABSOLUTE)
target_include_directories(
    "${PROJECT_NAME}"
    PRIVATE
	"${PATH_TO_ROOT}/External/rapidjson/include"
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


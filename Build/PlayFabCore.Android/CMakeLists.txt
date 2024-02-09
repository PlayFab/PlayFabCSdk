cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ../libHttpClient)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)
endif()

project("PlayFabCore.Android")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_C_COMPILERclang)
set(CMAKE_CXX_COMPILERclang++)
set(CMAKE_CXX_FLAGS"-fvisibility=hidden")
set(CMAKE_C_FLAGS"-fvisibility=hidden")

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
        "${PATH_TO_ROOT}/External/rapidjson/include"
        "${PATH_TO_LHC}/Include"
        )

#########################
### Set up shared lib ###
#########################

add_subdirectory("../PlayFabSharedInternal.Android" ${PATH_TO_ROOT}/Out/Android/PlayFabCore.Android/PlayFabSharedInternal/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
add_subdirectory("${PATH_TO_LHC}/Build/libHttpClient.Android" "${PATH_TO_LHC}/Binaries/Android/.cxx/libHttpClient.Android/cmake/${CMAKE_BUILD_TYPE_LOWER}/${ANDROID_ABI}")

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

target_link_libraries(
    PlayFabCore.Android
    PlayFabSharedInternal.Android
    libHttpClient.Android
    ${log-lib}
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
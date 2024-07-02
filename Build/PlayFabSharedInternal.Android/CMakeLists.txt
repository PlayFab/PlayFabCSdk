cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ../libHttpClient)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)
endif()

project("PlayFabSharedInternal.Android")

# For PlayFab, we need to compile with c++20 rather than c++17 because of an issue with std::optional discussed here:
# https://discourse.llvm.org/t/code-which-compiles-with-g-but-not-with-clang/56868/4
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")

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

include("../PlayFabSharedInternal.CMake/GetPlayFabSharedInternalFiles.cmake")
get_playfab_shared_internal_files(
    PLAYFAB_SHARED_INTERNAL_FILES
    "${PATH_TO_SOURCE}"
)

set(COMMON_INCLUDE_DIRS
    "${PATH_TO_SOURCE}/PlayFabCore/Include"
    "${PATH_TO_SOURCE}/PlayFabCore/Include/Generated"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Source/Common"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
    "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
    "${PATH_TO_ROOT}/External/rapidjson/include"
    "${PATH_TO_LHC}/Include"
)

#########################
### Set up static lib ###
#########################

add_library(
    "PlayFabSharedInternal.Android"
    STATIC
    "${PLAYFAB_SHARED_INTERNAL_FILES}"
)

target_include_directories(
    "PlayFabSharedInternal.Android"
    PRIVATE
    "${COMMON_INCLUDE_DIRS}"
)

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
    "PlayFabSharedInternal.Android"
    "${FLAGS}"
    "${FLAGS_DEBUG}"
    "${FLAGS_RELEASE}"
)

cmake_minimum_required(VERSION 3.6)

get_filename_component(PATH_TO_ROOT "../../" ABSOLUTE)
get_filename_component(PATH_TO_SOURCE "../../Source" ABSOLUTE)

set(PATH_TO_LHC ../libHttpClient)
if(NOT EXISTS "${PATH_TO_LHC}/README.md")
    set(PATH_TO_LHC ${PATH_TO_ROOT}/../libHttpClient)
endif()

project("PlayFabServices.Android")

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
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include"
        "${PATH_TO_SOURCE}/PlayFabSharedInternal/Include/Generated"
        "${PATH_TO_ROOT}/External/rapidjson/include/rapidjson"
        "${PATH_TO_ROOT}/External/rapidjson/include"
        "${PATH_TO_LHC}/Include"
        )

#########################
### Set up shared lib ###
#########################

IF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabSharedInternal.Android/cmake/x86_64${CMAKE_BUILD_TYPE})
    set(PLAYFAB_SHARED_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabSharedInternal.Android/cmake/x86_64${CMAKE_BUILD_TYPE}/x86_64/PlayFabSharedInternal.Android.a
    )
ELSEIF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabSharedInternal.Android/cmake/arm8${CMAKE_BUILD_TYPE})
    set(PLAYFAB_SHARED_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabSharedInternal.Android/cmake/arm8${CMAKE_BUILD_TYPE}/arm64-v8a/PlayFabSharedInternal.Android.a
    )
endif()

message(STATUS "PLAYFAB_SHARED_INTERNAL_BINARY_PATH: ${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}")

IF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabCore.Android/cmake/x86_64${CMAKE_BUILD_TYPE})
    set(PLAYFAB_CORE_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabCore.Android/cmake/x86_64${CMAKE_BUILD_TYPE}/x86_64/PlayFabCore.Android.a
    )
ELSEIF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabCore.Android/cmake/arm8${CMAKE_BUILD_TYPE})
    set(PLAYFAB_CORE_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabCore.Android/cmake/arm8${CMAKE_BUILD_TYPE}/arm64-v8a/PlayFabCore.Android.a
    )
endif()

message(STATUS "PLAYFAB_CORE_INTERNAL_BINARY_PATH: ${PLAYFAB_CORE_INTERNAL_BINARY_PATH}")

IF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabServices.Android/cmake/x86_64${CMAKE_BUILD_TYPE})
    set(PLAYFAB_SERVICES_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabServices.Android/cmake/x86_64${CMAKE_BUILD_TYPE}/x86_64/StaticPlayFabServices.Android.a
    )
ELSEIF (EXISTS ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabServices.Android/cmake/arm8${CMAKE_BUILD_TYPE})
    set(PLAYFAB_SERVICES_INTERNAL_BINARY_PATH
        ${PATH_TO_ROOT}/Out/Android/.cxx/PlayFabServices.Android/cmake/arm8${CMAKE_BUILD_TYPE}/arm64-v8a/StaticPlayFabServices.Android.a
    )
endif()

message(STATUS "PLAYFAB_SERVICES_INTERNAL_BINARY_PATH: ${PLAYFAB_SERVICES_INTERNAL_BINARY_PATH}")

add_library(
        "PlayFabServicesStatic.Android"
        STATIC
        "${SERVICE_WRAPPER_SOURCE_FILES}"
        "${COMMON_SOURCE_FILES}"
)

target_include_directories(
        "PlayFabServicesStatic.Android"
        PRIVATE
        "${COMMON_INCLUDE_DIRS}"
)

include("../PlayFabSharedInternal.CMake/GetLinuxFlags.cmake")
get_linux_flags(FLAGS FLAGS_DEBUG FLAGS_RELEASE)

include("../PlayFabSharedInternal.CMake/TargetSetFlags.cmake")
target_set_flags(
        "PlayFabServicesStatic.Android"
        "${FLAGS}"
        "${FLAGS_DEBUG}"
        "${FLAGS_RELEASE}"
)

add_library(libPFCore STATIC IMPORTED)
set_target_properties(libPFCore PROPERTIES IMPORTED_LOCATION "${PLAYFAB_CORE_INTERNAL_BINARY_PATH}")

add_library(libPFServices STATIC IMPORTED)
set_target_properties(libPFServices PROPERTIES IMPORTED_LOCATION "${PLAYFAB_SERVICES_INTERNAL_BINARY_PATH}")

add_library(libPFShared STATIC IMPORTED)
set_target_properties(libPFShared PROPERTIES IMPORTED_LOCATION "${PLAYFAB_SHARED_INTERNAL_BINARY_PATH}")

add_library("${PROJECT_NAME}" SHARED main.cpp)

add_dependencies("${PROJECT_NAME}" 
    libPFShared
    libPFCore
    libPFServices
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabCore/Include/Generated>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabServices/Include>
    $<BUILD_INTERFACE:${PATH_TO_SOURCE}/PlayFabServices/Include/Generated>
)
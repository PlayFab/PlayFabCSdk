# Vcpkg.cmake
# Defines VCPKG_INSTALLED_INCLUDE_DIR (Build/vcpkg_installed/<triplet>/include) for the triplet
# that matches the platform being configured.
#
# Triplet selection:
#   1. -D VCPKG_TARGET_TRIPLET=<triplet> when given.
#   2. Android: derived from ANDROID_ABI (arm64-v8a, armeabi-v7a, x86_64, x86).
#   3. Linux x86_64: x64-linux.
# Anything else must pass VCPKG_TARGET_TRIPLET explicitly.
#
# Include after project() so CMAKE_SYSTEM_NAME / CMAKE_SYSTEM_PROCESSOR are set.
# Restore the triplet first: Build/vcpkg/restore-vcpkg.sh --triplet <triplet>

set(_pf_vcpkg_triplet "${VCPKG_TARGET_TRIPLET}")

if(NOT _pf_vcpkg_triplet)
    if(ANDROID_ABI)
        if(ANDROID_ABI STREQUAL "arm64-v8a")
            set(_pf_vcpkg_triplet "arm64-android")
        elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
            set(_pf_vcpkg_triplet "arm-android")
        elseif(ANDROID_ABI STREQUAL "x86_64")
            set(_pf_vcpkg_triplet "x64-android")
        elseif(ANDROID_ABI STREQUAL "x86")
            set(_pf_vcpkg_triplet "x86-android")
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
        set(_pf_vcpkg_triplet "x64-linux")
    endif()
endif()

if(NOT _pf_vcpkg_triplet)
    message(FATAL_ERROR
        "Cannot determine the vcpkg triplet (ANDROID_ABI='${ANDROID_ABI}', "
        "CMAKE_SYSTEM_NAME='${CMAKE_SYSTEM_NAME}', CMAKE_SYSTEM_PROCESSOR='${CMAKE_SYSTEM_PROCESSOR}'). "
        "Pass -D VCPKG_TARGET_TRIPLET=<triplet>.")
endif()

get_filename_component(_pf_vcpkg_installed "${CMAKE_CURRENT_LIST_DIR}/../vcpkg_installed" ABSOLUTE)
set(_pf_vcpkg_include "${_pf_vcpkg_installed}/${_pf_vcpkg_triplet}/include")

if(NOT EXISTS "${_pf_vcpkg_include}")
    message(FATAL_ERROR
        "vcpkg include directory for triplet '${_pf_vcpkg_triplet}' not found: '${_pf_vcpkg_include}'. "
        "Run Build/vcpkg/restore-vcpkg.sh --triplet ${_pf_vcpkg_triplet} before configuring.")
endif()

set(VCPKG_INSTALLED_INCLUDE_DIR "${_pf_vcpkg_include}")
message(STATUS "vcpkg triplet '${_pf_vcpkg_triplet}': ${VCPKG_INSTALLED_INCLUDE_DIR}")

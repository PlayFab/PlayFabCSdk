function(TARGET_SET_FLAGS TARGET_NAME FLAGS FLAGS_DEBUG FLAGS_RELEASE)

    foreach(flag "${FLAGS}")
        target_compile_options("${TARGET_NAME}" PRIVATE "${flag}")
    endforeach()

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        foreach(flag "${FLAGS_DEBUG}")
            target_compile_options("${TARGET_NAME}" PRIVATE "${flag}")
        endforeach()
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        foreach(flag "${FLAGS_RELEASE}")
            target_compile_options("${TARGET_NAME}" PRIVATE "${flag}")
        endforeach()
    endif()

endfunction()

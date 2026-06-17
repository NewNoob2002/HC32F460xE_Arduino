set(CLANGD_COMPILE_COMMANDS_LINK "${CMAKE_CURRENT_SOURCE_DIR}/build/clangd")
if(EXISTS "${CLANGD_COMPILE_COMMANDS_LINK}" AND NOT IS_SYMLINK "${CLANGD_COMPILE_COMMANDS_LINK}")
    message(WARNING "Cannot update ${CLANGD_COMPILE_COMMANDS_LINK}: path exists and is not a symlink")
else()
    get_filename_component(CURRENT_BINARY_DIR_PARENT "${CMAKE_CURRENT_BINARY_DIR}" DIRECTORY)
    get_filename_component(CURRENT_BINARY_DIR_NAME "${CMAKE_CURRENT_BINARY_DIR}" NAME)
    get_filename_component(DEFAULT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build" ABSOLUTE)

    if(CURRENT_BINARY_DIR_PARENT STREQUAL DEFAULT_BUILD_DIR)
        set(CLANGD_COMPILE_COMMANDS_TARGET "${CURRENT_BINARY_DIR_NAME}")
    else()
        set(CLANGD_COMPILE_COMMANDS_TARGET "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    file(REMOVE "${CLANGD_COMPILE_COMMANDS_LINK}")
    file(CREATE_LINK
        "${CLANGD_COMPILE_COMMANDS_TARGET}"
        "${CLANGD_COMPILE_COMMANDS_LINK}"
        SYMBOLIC
        RESULT CLANGD_LINK_RESULT
    )
    if(CLANGD_LINK_RESULT EQUAL 0)
        message(STATUS "clangd compilation database: ${CLANGD_COMPILE_COMMANDS_LINK} -> ${CLANGD_COMPILE_COMMANDS_TARGET}")
    else()
        message(WARNING "Failed to update ${CLANGD_COMPILE_COMMANDS_LINK}: ${CLANGD_LINK_RESULT}")
    endif()
endif()

get_filename_component(C_COMPILER_NAME "${CMAKE_C_COMPILER}" NAME)
if(C_COMPILER_NAME MATCHES "^arm-none-eabi-gcc")
    set(ARM_GCC_SYSTEM_INCLUDE_DIRS)
    set(ARM_GXX_SYSTEM_INCLUDE_DIRS)

    foreach(GCC_INCLUDE_QUERY IN ITEMS -print-file-name=include -print-file-name=include-fixed)
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} ${GCC_INCLUDE_QUERY}
            OUTPUT_VARIABLE GCC_INCLUDE_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        if(GCC_INCLUDE_DIR AND EXISTS "${GCC_INCLUDE_DIR}")
            list(APPEND ARM_GCC_SYSTEM_INCLUDE_DIRS "${GCC_INCLUDE_DIR}")
        endif()
    endforeach()

    execute_process(
        COMMAND ${CMAKE_C_COMPILER} -print-sysroot
        OUTPUT_VARIABLE ARM_GCC_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(ARM_GCC_SYSROOT AND EXISTS "${ARM_GCC_SYSROOT}/include")
        list(APPEND ARM_GCC_SYSTEM_INCLUDE_DIRS "${ARM_GCC_SYSROOT}/include")
    endif()

    if(CMAKE_CXX_COMPILER)
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
            OUTPUT_VARIABLE ARM_GXX_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        set(ARM_GXX_INCLUDE_DIR "${ARM_GCC_SYSROOT}/include/c++/${ARM_GXX_VERSION}")
        foreach(ARM_GXX_SYSTEM_INCLUDE_DIR IN ITEMS
            "${ARM_GXX_INCLUDE_DIR}"
            "${ARM_GXX_INCLUDE_DIR}/arm-none-eabi"
            "${ARM_GXX_INCLUDE_DIR}/backward")
            if(EXISTS "${ARM_GXX_SYSTEM_INCLUDE_DIR}")
                list(APPEND ARM_GXX_SYSTEM_INCLUDE_DIRS "${ARM_GXX_SYSTEM_INCLUDE_DIR}")
            endif()
        endforeach()
    endif()

    set(ARM_GCC_SYSTEM_INCLUDE_OPTIONS)
    if(ARM_GCC_SYSTEM_INCLUDE_DIRS)
        list(REMOVE_DUPLICATES ARM_GCC_SYSTEM_INCLUDE_DIRS)
        foreach(ARM_SYSTEM_INCLUDE_DIR IN LISTS ARM_GCC_SYSTEM_INCLUDE_DIRS)
            list(APPEND ARM_GCC_SYSTEM_INCLUDE_OPTIONS "SHELL:-isystem \"${ARM_SYSTEM_INCLUDE_DIR}\"")
        endforeach()
        add_compile_options("$<$<COMPILE_LANGUAGE:C,ASM>:${ARM_GCC_SYSTEM_INCLUDE_OPTIONS}>")
    endif()

    if(ARM_GXX_SYSTEM_INCLUDE_DIRS OR ARM_GCC_SYSTEM_INCLUDE_DIRS)
        set(ARM_GXX_SYSTEM_INCLUDE_OPTIONS)
        foreach(ARM_SYSTEM_INCLUDE_DIR IN LISTS ARM_GXX_SYSTEM_INCLUDE_DIRS ARM_GCC_SYSTEM_INCLUDE_DIRS)
            list(APPEND ARM_GXX_SYSTEM_INCLUDE_OPTIONS "SHELL:-isystem \"${ARM_SYSTEM_INCLUDE_DIR}\"")
        endforeach()
        add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:${ARM_GXX_SYSTEM_INCLUDE_OPTIONS}>")
        message(STATUS "ARM GCC system includes: ${ARM_GCC_SYSTEM_INCLUDE_DIRS};${ARM_GXX_SYSTEM_INCLUDE_DIRS}")
    endif()
endif()
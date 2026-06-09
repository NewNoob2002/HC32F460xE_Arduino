set(HC32_CPU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

add_library(hc32_project_options INTERFACE)

target_compile_definitions(hc32_project_options INTERFACE
    HC32F460
    USE_DDL_DRIVER
    ARDUINO=111
    $<$<CONFIG:Debug>:__CORE_DEBUG>
)

target_compile_options(hc32_project_options INTERFACE
    ${HC32_CPU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<CONFIG:Debug>:-O2>
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Release>:-Os>
    $<$<CONFIG:Release>:-g0>
)

get_filename_component(C_COMPILER_NAME "${CMAKE_C_COMPILER}" NAME)
if(NOT C_COMPILER_NAME MATCHES "^arm-none-eabi-gcc")
    return()
endif()

set(ARM_GCC_SYSTEM_INCLUDE_DIRS)
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

set(ARM_GXX_SYSTEM_INCLUDE_DIRS)
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

list(REMOVE_DUPLICATES ARM_GCC_SYSTEM_INCLUDE_DIRS)
foreach(ARM_SYSTEM_INCLUDE_DIR IN LISTS ARM_GCC_SYSTEM_INCLUDE_DIRS)
    target_compile_options(hc32_project_options INTERFACE
        "$<$<COMPILE_LANGUAGE:C,ASM>:SHELL:-isystem \"${ARM_SYSTEM_INCLUDE_DIR}\">"
    )
endforeach()

foreach(ARM_SYSTEM_INCLUDE_DIR IN LISTS
    ARM_GXX_SYSTEM_INCLUDE_DIRS
    ARM_GCC_SYSTEM_INCLUDE_DIRS)
    target_compile_options(hc32_project_options INTERFACE
        "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-isystem \"${ARM_SYSTEM_INCLUDE_DIR}\">"
    )
endforeach()

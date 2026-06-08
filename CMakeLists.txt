cmake_minimum_required(VERSION 3.16)

# Set up toolchain BEFORE project()
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arm-none-eabi.cmake)
endif()

project(hc32_project C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
# CPU Flags
set(CPU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Interface library for common options
add_library(hc32_project_options INTERFACE)

target_include_directories(hc32_project_options INTERFACE
    App/inc
    Platform/CMSIS/Include
    Platform/CMSIS/Device/HDSC/hc32f4xx/Include
    Platform/Config
    Platform/Core/inc
    Platform/HAL
    Platform/Driver/inc
    Arduino
    Arduino/Core/avr
    Arduino/Core/drivers/adc
    Arduino/Core/drivers/i2c
    Arduino/Core/drivers/irqn
    Arduino/Core/drivers/panic
    Arduino/Core/drivers/spi
    Arduino/Core/drivers/sysclock
    Arduino/Core/drivers/timera
    Arduino/Core/drivers/usart
    Arduino/Core/drivers/panic
    Arduino/Core/variants/hc32f460xx
    Libraries
    Libraries/Parser/include
    Libraries/lwmem-2.2.4/lwmem/src/include
    Libraries/lwmem-2.2.4/lwmem/src/include/lwmem
    Libraries/lwmem-2.2.4/lwmem/src/include/system
    Libraries/easylogger/inc
    Libraries/Bq40z50
    Libraries/Mp2762a
    Libraries/MultiButton
    Libraries/MillisTaskManager
)

target_compile_definitions(hc32_project_options INTERFACE
    HC32F460
    USE_DDL_DRIVER
    ARDUINO=111
    $<$<CONFIG:Debug>:__CORE_DEBUG>
)

target_compile_options(hc32_project_options INTERFACE
    ${CPU_FLAGS}
    -ffunction-sections
    -fdata-sections
    -Wall
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<CONFIG:Debug>:-O2 -g3>
    $<$<CONFIG:Release>:-Os -g0>
)

# Linker script
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/HC32F460xE.ld")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections -Wl,--print-memory-usage -specs=nosys.specs -specs=nano.specs -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb")

# Main executable
add_executable(${PROJECT_NAME}.elf 
    Platform/Core/src/Syscalls_GCC.c
)

# Add subdirectories
add_subdirectory(Platform)
add_subdirectory(Arduino)
add_subdirectory(Libraries)
add_subdirectory(App)

# Link modules
target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    -Wl,--start-group
    hc32_libraries
    hc32_arduino
    hc32_platform
    -Wl,--end-group
)

# Generate hex and bin files
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)

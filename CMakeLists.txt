cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/arm-none-eabi.cmake")
endif()

set(PROJECT_VERSION 1.29)
set(FIRMWARE_NAME MCU_LED)
project(${FIRMWARE_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/Clangd.cmake)
include(cmake/ProjectOptions.cmake)

add_subdirectory(Platform/Config)
add_subdirectory(Platform)
add_subdirectory(Arduino)
add_subdirectory(Libraries)
add_subdirectory(App)

set(FIRMWARE_TARGET mcu_firmware)
set(FIRMWARE_OUTPUT_NAME "${FIRMWARE_NAME}_${PROJECT_VERSION}")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/HC32F460xE.ld")

add_executable(${FIRMWARE_TARGET})
set_target_properties(${FIRMWARE_TARGET} PROPERTIES
    OUTPUT_NAME "${FIRMWARE_OUTPUT_NAME}"
    SUFFIX ".elf"
)

target_link_libraries(${FIRMWARE_TARGET} PRIVATE
    -Wl,--start-group
    hc32_app
    hc32_libraries
    hc32_arduino
    hc32_platform
    -Wl,--end-group
)

target_link_options(${FIRMWARE_TARGET} PRIVATE
    ${HC32_CPU_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,--defsym=FLASH_ORIGIN=0x00008000
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_OUTPUT_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    --specs=nano.specs
    --specs=nosys.specs
)

add_custom_command(TARGET ${FIRMWARE_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
        $<TARGET_FILE:${FIRMWARE_TARGET}>
        ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_OUTPUT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary
        $<TARGET_FILE:${FIRMWARE_TARGET}>
        ${CMAKE_CURRENT_BINARY_DIR}/${FIRMWARE_OUTPUT_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${FIRMWARE_TARGET}>
    VERBATIM
)

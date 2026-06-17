cmake_minimum_required(VERSION 3.20)

project(HC32F460xE_Arduino LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_VERSION 2.4.3)
#2.4.3: shutdown page add wifi set interface,  add temparature reach 65 warming
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Config/mcu_version_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Platform/Config/mcu_version_config.h
    @ONLY
)
set(TARGET_NAME MCU_PRO_${PROJECT_VERSION})
set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/HC32F460xE.ld)

include(cmake/clangd.cmake)
include(cmake/ProjectOptions.cmake)

add_subdirectory(Platform)
add_subdirectory(Arduino)
add_subdirectory(Libraries)
add_subdirectory(Simulator/App)
add_subdirectory(USER)

add_executable(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} PRIVATE
    hc32_platform
    hc32_arduino
    hc32_libraries
    hc32_simulator_app
    hc32_user
)
target_link_options(${TARGET_NAME} PRIVATE
    ${CPU_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,--defsym=FLASH_ORIGIN=0x00008000
    # -Wl,--defsym=FLASH_LENGTH=0x00036000
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    --specs=nano.specs
    --specs=nosys.specs
)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
    COMMENT "Generating HEX/BIN and size report"
)

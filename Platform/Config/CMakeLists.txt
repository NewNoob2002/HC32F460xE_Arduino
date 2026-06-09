set(MCU_GENERATED_CONFIG_DIR
    "${CMAKE_BINARY_DIR}/generated/Platform/Config"
)

file(MAKE_DIRECTORY "${MCU_GENERATED_CONFIG_DIR}")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/mcu_version_config.h.in"
    "${MCU_GENERATED_CONFIG_DIR}/mcu_version_config.h"
    @ONLY
)

add_library(hc32_config INTERFACE)
target_include_directories(hc32_config INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${MCU_GENERATED_CONFIG_DIR}"
)

set(MCU_GENERATED_CONFIG_DIR
    "${CMAKE_BINARY_DIR}/generated/Platform/Config"
)

add_library(hc32_config INTERFACE)
target_include_directories(hc32_config INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${MCU_GENERATED_CONFIG_DIR}"
)

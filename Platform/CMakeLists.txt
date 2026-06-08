file(GLOB PLATFORM_CMSIS_SRC "CMSIS/Device/HDSC/hc32f4xx/Source/*.c")
file(GLOB PLATFORM_DRIVER_SRC "Driver/src/*.c")
file(GLOB PLATFORM_CORE_SRC "Core/src/*.c" "Core/src/*.cpp")
file(GLOB PLATFORM_HAL_SRC "HAL/*.c" "HAL/*.cpp")

# Syscalls_KEIL.c might not be needed for GCC
list(FILTER PLATFORM_CORE_SRC EXCLUDE REGEX "Syscalls_KEIL.c")

add_library(hc32_platform STATIC
    ${PLATFORM_CMSIS_SRC}
    ${PLATFORM_DRIVER_SRC}
    ${PLATFORM_CORE_SRC}
    ${PLATFORM_HAL_SRC}
    CMSIS/Device/HDSC/hc32f4xx/Source/GCC/startup_hc32f460.S
)

target_link_libraries(hc32_platform PUBLIC hc32_project_options)

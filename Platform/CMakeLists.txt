file(GLOB PLATFORM_CMSIS_SRC "CMSIS/Device/HDSC/hc32f4xx/Source/*.c")
file(GLOB PLATFORM_DRIVER_SRC "Driver/src/*.c")
file(GLOB PLATFORM_CORE_SRC "Core/src/*.c" "Core/src/*.cpp")
list(REMOVE_ITEM PLATFORM_CORE_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/Syscalls_GCC.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/Syscalls_KEIL.c"
)
file(GLOB PLATFORM_HAL_SRC "HAL/*.c" "HAL/*.cpp")

add_library(hc32_platform STATIC
    ${PLATFORM_CMSIS_SRC}
    ${PLATFORM_DRIVER_SRC}
    ${PLATFORM_CORE_SRC}
    ${PLATFORM_HAL_SRC}
    CMSIS/Device/HDSC/hc32f4xx/Source/GCC/startup_hc32f460.S
)

target_link_libraries(hc32_platform PUBLIC hc32_project_options)

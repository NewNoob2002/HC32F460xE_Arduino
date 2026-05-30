cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)
# 设置 C 和 C++ 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "")
message(STATUS "配置 Middleware中间件...")

# ============================================================================
# CMSIS-RTX RTOS 配置
# ============================================================================
set(CMSIS_RTX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS-RTX-5.9.1")
set(CMSIS_6_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_6-6.3.0")

message(STATUS "CMSIS-RTX 配置:")
message(STATUS "  - RTX: ${CMSIS_RTX_DIR}")
message(STATUS "  - CMSIS: ${CMSIS_6_DIR}")

add_library(CMSIS_RTX STATIC
    ${CMSIS_RTX_DIR}/Config/RTX_Config.c
    ${CMSIS_RTX_DIR}/Source/rtx_delay.c
    ${CMSIS_RTX_DIR}/Source/rtx_evflags.c
    ${CMSIS_RTX_DIR}/Source/rtx_kernel.c
    ${CMSIS_RTX_DIR}/Source/rtx_lib.c
    ${CMSIS_RTX_DIR}/Source/rtx_memory.c
    ${CMSIS_RTX_DIR}/Source/rtx_mempool.c
    ${CMSIS_RTX_DIR}/Source/rtx_msgqueue.c
    ${CMSIS_RTX_DIR}/Source/rtx_mutex.c
    ${CMSIS_RTX_DIR}/Source/rtx_semaphore.c
    ${CMSIS_RTX_DIR}/Source/rtx_system.c
    ${CMSIS_RTX_DIR}/Source/rtx_thread.c
    ${CMSIS_RTX_DIR}/Source/rtx_timer.c
    ${CMSIS_RTX_DIR}/Source/GCC/irq_armv7m.S
    ${CMSIS_6_DIR}/CMSIS/RTOS2/Source/os_systick.c
)

target_include_directories(CMSIS_RTX PUBLIC
    ${CMSIS_RTX_DIR}/Config
    ${CMSIS_RTX_DIR}/Include
    ${CMSIS_6_DIR}/CMSIS/RTOS2/Include
)

target_compile_definitions(CMSIS_RTX PRIVATE
    EVR_RTX_DISABLE
)

target_link_libraries(CMSIS_RTX PUBLIC stm32cubemx)

# ============================================================================
# 创建聚合库 Libraries（链接所有组件）
# ============================================================================
# 创建统一的 utils 库（聚合所有组件）
add_library(Middleware INTERFACE)

target_link_libraries(Middleware INTERFACE CMSIS_RTX)

message(STATUS "Middleware 聚合库已创建（链接所有组件）")
message(STATUS "")

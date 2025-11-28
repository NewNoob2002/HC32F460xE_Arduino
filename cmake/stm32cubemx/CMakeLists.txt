cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# STM32CubeMX generated symbols (macros)
set(Defines_Syms
    HC32F460
    USE_DDL_DRIVER
    $<$<CONFIG:Debug>:DEBUG>
)

# STM32CubeMX generated include paths
set(Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../App/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/CMSIS/Device/HDSC/hc32f4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/HAL/
)

# STM32CubeMX generated application sources
set(Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../App/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/addon_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/delay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/dwt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/hc32f4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Core/Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../startup_hc32f460.S
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/HAL/HAL.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/HAL/HAL_CORTEX.c
)

# STM32 HAL/LL Drivers
set(Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/CMSIS/Device/HDSC/hc32f4xx/Source/system_hc32f460.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_clk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_icg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_utility.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32f460_ll_interrupts_share.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_aos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_efm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_interrupts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_sram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Platform/Drivers/src/hc32_ll_pwc.c
)

# Drivers Midllewares



# Link directories setup
set(LINK_DIRS

)
# Project static libraries
set(LINK_LIBS
    Drivers
    ${TOOLCHAIN_LINK_LIBRARIES}

)
# Interface library for includes and symbols
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${Defines_Syms})

# Create STM32_Drivers static library
add_library(Drivers OBJECT)
target_sources(Drivers PRIVATE ${Drivers_Src})
target_link_libraries(Drivers PUBLIC stm32cubemx)


# Add STM32CubeMX generated application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()

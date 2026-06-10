add_subdirectory(Config)

file(GLOB PLATFORM_CMSIS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/HDSC/hc32f4xx/Source/*.c"
)
file(GLOB PLATFORM_DRIVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Driver/src/*.c"
)
file(GLOB PLATFORM_CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/src/*.cpp"
)
file(GLOB PLATFORM_HAL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL/*.cpp"
)

list(FILTER PLATFORM_DRIVER_SOURCES EXCLUDE REGEX "hc32_ll_icg\\.c$")
list(FILTER PLATFORM_CORE_SOURCES EXCLUDE REGEX "Syscalls_(GCC|KEIL)\\.c$")

add_library(hc32_platform OBJECT
    ${PLATFORM_CMSIS_SOURCES}
    ${PLATFORM_DRIVER_SOURCES}
    ${PLATFORM_CORE_SOURCES}
    ${PLATFORM_HAL_SOURCES}
    Core/src/Syscalls_GCC.c
    CMSIS/Device/HDSC/hc32f4xx/Source/GCC/startup_hc32f460.S
)

target_include_directories(hc32_platform PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Device/HDSC/hc32f4xx/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Driver/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/HAL"
)

target_include_directories(hc32_platform PRIVATE
    "${CMAKE_SOURCE_DIR}/Arduino"
    "${CMAKE_SOURCE_DIR}/Arduino/Core/avr"
    "${CMAKE_SOURCE_DIR}/Arduino/Core/drivers/i2c"
    "${CMAKE_SOURCE_DIR}/Arduino/Core/variants/hc32f460xx"
    "${CMAKE_SOURCE_DIR}/Libraries"
    "${CMAKE_SOURCE_DIR}/Libraries/Bq40z50"
    "${CMAKE_SOURCE_DIR}/Libraries/Mp2762a"
    "${CMAKE_SOURCE_DIR}/Libraries/MultiButton"
    "${CMAKE_SOURCE_DIR}/Libraries/MillisTaskManager"
    "${CMAKE_SOURCE_DIR}/Libraries/easylogger/inc"
    "${CMAKE_SOURCE_DIR}/Libraries/lwmem-2.2.4/lwmem/src/include"
)

target_link_libraries(hc32_platform PUBLIC
    hc32_project_options
    hc32_config
)

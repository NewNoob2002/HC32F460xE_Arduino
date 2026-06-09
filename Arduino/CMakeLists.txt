file(GLOB ARDUINO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/avr/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/avr/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/variants/hc32f460xx/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/variants/hc32f460xx/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/panic/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/panic/*.cpp"
)

add_library(hc32_arduino STATIC ${ARDUINO_SOURCES})

target_include_directories(hc32_arduino PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/avr"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/adc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/i2c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/irqn"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/panic"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/spi"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/sysclock"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/timera"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/drivers/usart"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/variants/hc32f460xx"
)

target_include_directories(hc32_arduino PRIVATE
    "${CMAKE_SOURCE_DIR}/Platform/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Platform/CMSIS/Device/HDSC/hc32f4xx/Include"
    "${CMAKE_SOURCE_DIR}/Platform/Core/inc"
    "${CMAKE_SOURCE_DIR}/Platform/Driver/inc"
    "${CMAKE_SOURCE_DIR}/Platform/HAL"
    "${CMAKE_SOURCE_DIR}/Libraries/easylogger/inc"
)

target_link_libraries(hc32_arduino PUBLIC
    hc32_project_options
    hc32_config
)

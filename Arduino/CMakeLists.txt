file(GLOB ARDUINO_SRC "*.c" "*.cpp")
file(GLOB ARDUINO_AVR_SRC "Core/avr/*.c" "Core/avr/*.cpp")
file(GLOB ARDUINO_VARIANT_SRC "Core/variants/hc32f460xx/*.c" "Core/variants/hc32f460xx/*.cpp")
file(GLOB ARDUINO_PANIC_SRC "Core/drivers/panic/*.c" "Core/drivers/panic/*.cpp")

add_library(hc32_arduino STATIC
    ${ARDUINO_SRC}
    ${ARDUINO_AVR_SRC}
    ${ARDUINO_VARIANT_SRC}
    ${ARDUINO_PANIC_SRC}
)

target_link_libraries(hc32_arduino PUBLIC hc32_project_options)

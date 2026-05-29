add_library(hc32_user OBJECT
    main.cpp
    src/message_decode.cpp
    src/slave_i2c.cpp
)

target_include_directories(hc32_user PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(hc32_user PUBLIC
    hc32_project_options
    hc32_platform_headers
    hc32_arduino_headers
    hc32_libraries_headers
    hc32_simulator_app_headers
)

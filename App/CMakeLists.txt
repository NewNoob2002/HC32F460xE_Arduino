file(GLOB APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(hc32_app STATIC ${APP_SOURCES})

target_include_directories(hc32_app PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_link_libraries(hc32_app PUBLIC
    hc32_project_options
    hc32_config
    hc32_platform
    hc32_arduino
    hc32_libraries
)

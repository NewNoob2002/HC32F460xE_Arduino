file(GLOB PARSER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Parser/src/*.cpp"
)
file(GLOB BQ40Z50_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Bq40z50/*.cpp"
)
file(GLOB MP2762A_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Mp2762a/*.cpp"
)
file(GLOB LWMEM_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/lwmem-2.2.4/lwmem/src/lwmem/*.c"
)
file(GLOB MILLIS_TASK_MANAGER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/MillisTaskManager/*.cpp"
)
file(GLOB EASYLOGGER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/easylogger/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/easylogger/port/*.c"
)
file(GLOB MULTIBUTTON_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/MultiButton/*.c"
)

add_library(hc32_libraries STATIC
    ${PARSER_SOURCES}
    ${BQ40Z50_SOURCES}
    ${MP2762A_SOURCES}
    ${LWMEM_SOURCES}
    ${MILLIS_TASK_MANAGER_SOURCES}
    ${EASYLOGGER_SOURCES}
    ${MULTIBUTTON_SOURCES}
)

target_include_directories(hc32_libraries PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/Parser/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lwmem-2.2.4/lwmem/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lwmem-2.2.4/lwmem/src/include/lwmem"
    "${CMAKE_CURRENT_SOURCE_DIR}/lwmem-2.2.4/lwmem/src/include/system"
    "${CMAKE_CURRENT_SOURCE_DIR}/easylogger/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/Bq40z50"
    "${CMAKE_CURRENT_SOURCE_DIR}/Mp2762a"
    "${CMAKE_CURRENT_SOURCE_DIR}/MultiButton"
    "${CMAKE_CURRENT_SOURCE_DIR}/MillisTaskManager"
)

target_include_directories(hc32_libraries PRIVATE
    "${CMAKE_SOURCE_DIR}/Arduino"
    "${CMAKE_SOURCE_DIR}/Arduino/Core/avr"
    "${CMAKE_SOURCE_DIR}/Arduino/Core/variants/hc32f460xx"
    "${CMAKE_SOURCE_DIR}/Platform/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Platform/CMSIS/Device/HDSC/hc32f4xx/Include"
    "${CMAKE_SOURCE_DIR}/Platform/Core/inc"
    "${CMAKE_SOURCE_DIR}/Platform/Driver/inc"
    "${CMAKE_SOURCE_DIR}/Platform/HAL"
)

target_link_libraries(hc32_libraries PUBLIC
    hc32_project_options
    hc32_config
)

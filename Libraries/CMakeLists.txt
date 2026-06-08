# Parser
file(GLOB PARSER_SRC "Parser/src/*.cpp")
# Bq40z50
file(GLOB BQ40Z50_SRC "Bq40z50/*.cpp")
# Mp2762a
file(GLOB MP2762A_SRC "Mp2762a/*.cpp")
# lwmem
file(GLOB LWMEM_SRC "lwmem-2.2.4/lwmem/src/lwmem/*.c")
# MillisTaskManager
file(GLOB MILLIS_TASK_MANAGER_SRC "MillisTaskManager/*.cpp")
# easylogger
file(GLOB EASYLOGGER_SRC "easylogger/src/*.c" "easylogger/port/*.c")
# MultiButton
file(GLOB MULTIBUTTON_SRC "MultiButton/*.c")

add_library(hc32_libraries STATIC
    ${PARSER_SRC}
    ${BQ40Z50_SRC}
    ${MP2762A_SRC}
    ${LWMEM_SRC}
    ${MILLIS_TASK_MANAGER_SRC}
    ${EASYLOGGER_SRC}
    ${MULTIBUTTON_SRC}
)

target_link_libraries(hc32_libraries PUBLIC hc32_project_options)

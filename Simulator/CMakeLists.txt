cmake_minimum_required(VERSION 3.10)
project(lvgl)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 CMake 策略以使用 <PackageName>_ROOT 变量
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE INCLUDES "lv_drivers/*.h" "lvgl/demos/*.h" "lvgl/*.h" "./*.h")
file(GLOB_RECURSE SOURCES "lv_drivers/*.c" "lvgl/demos/*.c" "lvgl/src/*.c")

file(GLOB_RECURSE UI_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/App/*.c
        ${CMAKE_CURRENT_LIST_DIR}/App/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/App/Utils/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/../Platform/support/support.c
)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/App
        ${CMAKE_CURRENT_LIST_DIR}/App/Pages
        ${CMAKE_CURRENT_LIST_DIR}/App/Utils
        ${CMAKE_CURRENT_LIST_DIR}/App/Resource
        ${CMAKE_CURRENT_LIST_DIR}/App/config
        ${CMAKE_CURRENT_LIST_DIR}/../Platform/support
        ${CMAKE_CURRENT_LIST_DIR}/../Platform/Config
        ${CMAKE_CURRENT_LIST_DIR}/../Platform/HAL
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# SDL2 配置
set(SDL2_ROOT E:/SDL2-2.32.10)
set(SDL2_INCLUDE_DIR ${SDL2_ROOT}/include)
set(SDL2_LIB_DIR ${SDL2_ROOT}/lib/x64)

# 手动配置 SDL2（因为 SDL2Config.cmake 不存在）
set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
set(SDL2_LIBRARIES ${SDL2_LIB_DIR}/SDL2.lib)

include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/lvgl)
link_directories(${SDL2_LIB_DIR})

add_executable(main main.cpp mouse_cursor_icon.c ${SOURCES} ${INCLUDES} ${UI_SOURCES}
        ../Platform/Config/mcu_define.h
        ../Platform/Config/mcu_define.h)
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE LV_INCLUDE_SIMPLE)

# 抑制 MSVC 编译器的警告
if(MSVC)
    # C5287: 不同枚举类型位运算警告
    # C4018: 有符号/无符号不匹配
    # C4996: 不安全的函数（如 strcpy, strcat）
    # C4267: size_t 到更小类型的转换
    target_compile_options(main PRIVATE /wd5287 /wd4018 /wd4996 /wd4267)
endif()

target_link_libraries(main PRIVATE ${SDL2_LIBRARIES})
add_custom_target(run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)

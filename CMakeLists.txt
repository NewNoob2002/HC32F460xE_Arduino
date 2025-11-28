cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#
# 必须放在最开始：声明交叉编译和目标系统
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# 工具链前缀
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

# MCU特定编译选项
set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS} -Wall -fdata-sections -ffunction-sections" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics" CACHE STRING "" FORCE)

# 调试/发布模式 flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-Os -g0" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0" CACHE STRING "" FORCE)

# 链接选项
set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS} -T \"${CMAKE_SOURCE_DIR}/HC32F460xE.ld\" --specs=nano.specs -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--print-memory-usage" CACHE STRING "" FORCE)
set(TOOLCHAIN_LINK_LIBRARIES "m")

# 【新增2】清除Windows默认链接参数
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_INIT "" CACHE STRING "" FORCE)

# 【新增3】指定测试程序的链接参数
set(CMAKE_TRY_COMPILE_LINK_FLAGS "${TARGET_FLAGS} -Wl,--gc-sections --specs=nano.specs" CACHE STRING "" FORCE)

# 禁用测试程序运行（嵌入式程序无法在Windows运行）
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 交叉编译查找规则
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME HC32F460xE)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Arduino/itoa.c
    Arduino/Print.cpp
    Arduino/Stream.cpp
    Arduino/wiring_digital.cpp
    Arduino/WString.cpp
    Arduino/yield.c
    Arduino/Core/variants/hc32f460xx/variant.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Arduino
    Arduino/Core/avr
    Arduino/Core/variants/hc32f460xx
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols

)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

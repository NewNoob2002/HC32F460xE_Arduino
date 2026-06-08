file(GLOB APP_SRC "src/*.cpp")

# We add sources directly to the elf target defined in the root
target_sources(${PROJECT_NAME}.elf PRIVATE
    ${APP_SRC}
)

# App specific includes
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    inc
)

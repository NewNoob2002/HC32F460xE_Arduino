add_library(host_parser STATIC
    ${PROJECT_SOURCE_DIR}/Libraries/Parser/src/SparkFun_Extensible_Message_Parser.cpp
    ${PROJECT_SOURCE_DIR}/Libraries/Parser/src/Parse_CUSTOM.cpp
)
target_include_directories(host_parser PUBLIC
    ${PROJECT_SOURCE_DIR}/Libraries/Parser/include
)

add_executable(parser_host_test parser_host_test.cpp)
target_link_libraries(parser_host_test PRIVATE host_parser)

add_executable(protocol_host_test
    protocol_host_test.cpp
    ${PROJECT_SOURCE_DIR}/USER/src/message_decode.cpp
)
target_include_directories(protocol_host_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/fakes
    ${PROJECT_SOURCE_DIR}/USER/inc
    ${PROJECT_SOURCE_DIR}/Platform/Config
)
target_link_libraries(protocol_host_test PRIVATE host_parser)

foreach(test_target IN ITEMS parser_host_test protocol_host_test)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_target} PRIVATE -Wall -Wextra -Werror -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${test_target} PRIVATE -fsanitize=address,undefined)
    endif()
    add_test(NAME ${test_target} COMMAND ${test_target})
    set_tests_properties(${test_target} PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
endforeach()

cmake_minimum_required(VERSION 3.20)
project(noisy-tx VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable("${CMAKE_PROJECT_NAME}")

add_subdirectory(src)

add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_BINARY_DIR}/../compile_commands.json"
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res/"
    "${CMAKE_BINARY_DIR}/res/"
)

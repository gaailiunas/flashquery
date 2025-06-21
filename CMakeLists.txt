cmake_minimum_required(VERSION 3.10)
project(flashquery)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(flashquery
    src/main.cpp
    src/document.cpp
    src/core/lexer.cpp
    src/core/parser.cpp
    src/core/selector.cpp
)

target_include_directories(flashquery PRIVATE include)

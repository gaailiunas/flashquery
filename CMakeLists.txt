cmake_minimum_required(VERSION 3.10)
project(flashquery)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(flashquery
    src/main.cc
    src/document.cc
    src/lexer.cc
    src/parser.cc
    src/selector.cc
)
target_include_directories(flashquery PRIVATE include)

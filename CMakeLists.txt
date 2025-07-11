cmake_minimum_required(VERSION 3.10)

project(subleq CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

add_executable(subleq src/main.cpp src/parser.cpp src/subleq.cpp)

add_executable(test_subleq tests/test.cpp src/parser.cpp src/subleq.cpp)
add_test(NAME run_tests COMMAND test_subleq)

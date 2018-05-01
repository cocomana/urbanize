cmake_minimum_required(VERSION 3.5)

project(urbanize C)
project(urbanise_test C)

set(CMAKE_C_STANDARD 99)

include_directories("src")
add_library(urbanize "src/list.c")

add_executable(urbanise_test "test/list_tests.c" "src/list.c")
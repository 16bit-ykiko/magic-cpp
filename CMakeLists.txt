cmake_minimum_required(VERSION 3.26)
project(magic)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-Werror -Wall -Waddress -Woverflow")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include) 

add_executable(main test/main.cpp)

add_executable(struct test/struct.cpp)

add_executable(type test/type.cpp)

add_executable(enum test/enum.cpp)
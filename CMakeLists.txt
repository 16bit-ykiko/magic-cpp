cmake_minimum_required(VERSION 3.22)
project(magic-cpp CXX)

add_library(MAGIC_CPP INTERFACE)
target_include_directories(MAGIC_CPP INTERFACE include)

option(MAGIC_CPP_INCLUDE_TESTS "Include tests" ON)
if(NOT MAGIC_CPP_INCLUDE_TESTS)
        return()
endif()

# 检查编译器版本支持
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpfullversion -std=c++20
        OUTPUT_VARIABLE GCC_VERSION
    )
    string(REGEX MATCH "[0-9]+\\.[0-9]+" GCC_VERSION ${GCC_VERSION})
    if(GCC_VERSION VERSION_LESS 10.0)
        message(FATAL_ERROR "Minimum required version of g++ is 10.0")
    endif()
    message(STATUS "Using g++ version ${GCC_VERSION}")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} --version
        OUTPUT_VARIABLE CLANG_VERSION_OUTPUT
    )
    string(REGEX MATCH "clang version ([0-9]+\\.[0-9]+)" CLANG_VERSION ${CLANG_VERSION_OUTPUT})
    string(REGEX REPLACE "clang version ([0-9]+\\.[0-9]+).*" "\\1" CLANG_VERSION ${CLANG_VERSION})
    if(CLANG_VERSION VERSION_LESS 10.0)
        message(FATAL_ERROR "Minimum required version of clang is 10.0")
    endif()
    message(STATUS "Using clang version ${CLANG_VERSION}")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.28)
        message(FATAL_ERROR "Minimum required version of MSVC is 19.28 (Visual Studio 2019 version 16.10)")
    endif()
    message(STATUS "Using MSVC version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# 检查C++的版本
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++20 HAS_CXX23_FLAG)
check_cxx_compiler_flag(-std=c++20 HAS_CXX20_FLAG)
if (HAS_CXX23_FLAG)
  set(CMAKE_CXX_STANDARD 23)
elseif (HAS_CXX20_FLAG)
  set(CMAKE_CXX_STANDARD 20)
else()
  message(FATAL_ERROR "C++20 standard is required!")
endif()

if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

# 优先寻找本地的GTest
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GoogleTest not found locally, downloading with FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
else()
    message(STATUS "GoogleTest found locally.")
endif()

enable_testing()

include_directories(include)

# 使用通配符一次性添加多个测试文件
file(GLOB TEST_SOURCES "test/*.cpp")

# 创建一个包含所有测试文件的可执行文件
add_executable(all_tests ${TEST_SOURCES})

target_link_libraries(all_tests GTest::gtest_main)
# 链接Google Test库到测试可执行文件

# 使用Google Test自动发现所有测试并将其添加到测试套件中
include(GoogleTest)
gtest_discover_tests(all_tests)

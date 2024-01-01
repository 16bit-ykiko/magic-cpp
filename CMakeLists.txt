cmake_minimum_required(VERSION 3.22)
project(magic-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)

# 使用通配符一次性添加多个测试文件
file(GLOB TEST_SOURCES "test/*.cpp")

# 创建一个包含所有测试文件的可执行文件
add_executable(all_tests ${TEST_SOURCES})
add_executable(main main.cpp)

target_link_libraries(all_tests GTest::gtest_main)
# 链接Google Test库到测试可执行文件

# 使用Google Test自动发现所有测试并将其添加到测试套件中
include(GoogleTest)
gtest_discover_tests(all_tests)

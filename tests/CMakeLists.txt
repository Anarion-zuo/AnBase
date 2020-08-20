cmake_minimum_required(VERSION 3.18)
project(AnBase)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(../include)
include_directories(../include/test)
add_subdirectory(../src binary_dir)
add_executable(tests src/main.cpp
        classes/TestString.cpp
        )
target_link_libraries(tests anbase antest)
cmake_minimum_required(VERSION 3.18)
project(AnBase)

include_directories(../include)
include_directories(../include/test)
add_subdirectory(../src binary_dir)
add_executable(tests src/main.cpp
        classes/TestVector.cpp
        classes/TestString.cpp
        )
target_link_libraries(tests anarionbase antest)
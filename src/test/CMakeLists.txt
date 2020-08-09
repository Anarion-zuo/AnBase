cmake_minimum_required(VERSION 3.10)
project (AnBase)

add_library(
        antest STATIC
        TestClass.cpp
)

target_link_libraries(antest anarionbase)

include_directories(../../include/test)
include_directories(../../include)
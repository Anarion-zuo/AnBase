cmake_minimum_required(VERSION 3.10)
project (AnBase)

add_library(
        antest STATIC
        TestClass.cpp
        BenchMarker.cpp
)

target_link_libraries(antest anbase)

include_directories(../../include/test)
include_directories(../../include)
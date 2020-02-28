cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

set(CMAKE_SOURCE_DIR .)

aux_source_directory(. DIR_SRCS)
add_subdirectory(src)
add_executable(MyCppBase src/main.cpp)
target_link_libraries(MyCppBase anarionbase)
include_directories(${CMAKE_SOURCE_DIR}/include)

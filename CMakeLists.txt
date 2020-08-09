cmake_minimum_required(VERSION 3.10)
project (AnBase)

set(CMAKE_SOURCE_DIR .)

aux_source_directory(. DIR_SRCS)
add_subdirectory(src)
add_executable(AnBase src/main.cpp)
target_link_libraries(AnBase anarionbase)
include_directories(include)

# tests
add_custom_target(test COMMAND
        
        )
cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_SRCS)
add_library(SRC ${DIR_SRCS})

# subdirectories
add_subdirectory(allocator)
add_subdirectory(concurrent)
add_subdirectory(container)
add_subdirectory(reflector)
add_subdirectory(context)
add_subdirectory(io)
target_link_libraries(SRC ALLOCATOR CONCURRENT CONTAINER REFLECTOR IO CONTEXT)

include_directories(../include)

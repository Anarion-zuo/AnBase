cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_CONCURRENT_SRCS)
add_library(CONCURRENT ${DIR_CONCURRENT_SRCS})
add_subdirectory(base)
add_subdirectory(pool)
target_link_libraries(CONCURRENT CONCURRENT_BASE CONCURRENT_POOL)

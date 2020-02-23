cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_CONCURRENT_POOL_SRCS)
add_library(CONCURRENT_POOL ${DIR_CONCURRENT_POOL_SRCS})
target_link_libraries(CONCURRENT_POOL CONCURRENT_BASE)

include_directories(../../../include/)
include_directories(../../../include/concurrent/pool/)

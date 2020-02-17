cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_CONTEXT_SRCS)
add_library(CONTEXT ${DIR_CONTEXT_SRCS})
target_link_libraries(CONTEXT)

include_directories(../../include)
include_directories(../../include/context)

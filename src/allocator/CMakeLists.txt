cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_ALLOCATOR_SRCS)
add_library(ALLOCATOR ${DIR_ALLOCATOR_SRCS})

include_directories(../../include)
include_directories(../../include/allocator)

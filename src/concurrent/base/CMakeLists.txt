cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_CONCURRENT_BASE_SRCS)
add_library(CONCURRENT_BASE ${DIR_CONCURRENT_BASE_SRCS})

include_directories(../../../include)
include_directories(../../../include/concurrent/base)

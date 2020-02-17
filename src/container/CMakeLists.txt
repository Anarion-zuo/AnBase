cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_CONTAINER_SRCS)
add_library(CONTAINER ${DIR_CONTAINER_SRCS})

include_directories(../../include/)
include_directories(../../include/container)

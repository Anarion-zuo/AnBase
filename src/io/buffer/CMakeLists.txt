cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_BUFFER_SRCS)
include_directories(../../../include)
include_directories(../../../include/io/buffer)
add_library(IO_BUFFER ${DIR_IO_BUFFER_SRCS})

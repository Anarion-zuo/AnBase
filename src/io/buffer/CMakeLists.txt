cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_BUFFER_SRCS)
add_library(IO_BUFFER ${DIR_IO_BUFFER_SRCS})
target_link_libraries(IO_BUFFER)

include_directories(../../include)
include_directories(../../include/io/buffer)

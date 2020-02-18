cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_SRCS)
add_library(IO_CHANNEL ${DIR_IO_CHANNEL_SRCS})
target_link_libraries(IO_CHANNEL)

include_directories(../../include)
include_directories(../../include/io/channel)

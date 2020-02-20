cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_NETWORK_SRCS)
add_library(IO_CHANNEL_NETWORK ${DIR_IO_CHANNEL_NETWORK_SRCS})
target_link_libraries(IO_CHANNEL_NETWORK)

include_directories(../../include)
include_directories(../../include/io/channel/network)

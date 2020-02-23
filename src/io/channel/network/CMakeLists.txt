cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_NETWORK_SRCS)

include_directories(../../../../include)
include_directories(../../../../include/io/channel/network)

add_library(IO_CHANNEL_NETWORK ${DIR_IO_CHANNEL_NETWORK_SRCS})
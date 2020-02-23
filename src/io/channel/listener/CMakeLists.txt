cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_LISTENER_SRCS)

include_directories(../../../../include)
include_directories(../../../../include/io/channel/listener)

add_library(IO_CHANNEL_LISTENER ${DIR_IO_CHANNEL_LISTENER_SRCS})
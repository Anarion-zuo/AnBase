cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_FILE_SRCS)
add_library(IO_CHANNEL_FILE ${DIR_IO_CHANNEL_FILE_SRCS})
target_link_libraries(IO_CHANNEL_FILE)

include_directories(../../include)
include_directories(../../include/io/channel/file)

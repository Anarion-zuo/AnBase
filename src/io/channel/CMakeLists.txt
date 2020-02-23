cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_CHANNEL_SRCS)

include_directories(../../../include)
include_directories(../../../include/io/channel)

add_library(IO_CHANNEL ${DIR_IO_CHANNEL_SRCS})

add_subdirectory(file)
add_subdirectory(network)
add_subdirectory(listener)
target_link_libraries(IO_CHANNEL IO_CHANNEL_FILE IO_CHANNEL_NETWORK IO_CHANNEL_LISTENER)

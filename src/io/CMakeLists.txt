cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_IO_SRCS)
add_subdirectory(buffer)
add_subdirectory(channel)
add_library(IO ${DIR_IO_SRCS})
target_link_libraries(IO IO_BUFFER IO_CHANNEL)

include_directories(../../include)
include_directories(../../include/io)

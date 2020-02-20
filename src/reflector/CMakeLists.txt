cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

aux_source_directory(. DIR_REFLECTOR)
add_library(REFLECTOR ${DIR_REFLECTOR})
# target_link_libraries(IO_CHANNEL IO_CHANNEL_FILE IO_CHANNEL_NETWORK)

include_directories(../../include)
include_directories(../../include/reflection)

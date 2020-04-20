cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

#aux_source_directory(. DIR_SRCS)
add_library(anarionbase STATIC
        allocator/Allocator.cpp
        allocator/ListConcurrentAllocator.cpp
        allocator/FixedLengthAllocator.cpp

        concurrent/base/Barrier.cpp
        concurrent/base/CondVar.cpp
        concurrent/base/Mutex.cpp
        concurrent/base/RwMutex.cpp
        concurrent/base/Thread.cpp

        concurrent/pool/ThreadPool.cpp
        concurrent/pool/AsyncInvoke.cpp

        container/String.cpp
        container/StringBuilder.cpp

        context/Callable.cpp
        context/Context.cpp
        context/LocalContext.cpp

        io/buffer/Buffer.cpp
        io/buffer/Buffer.cpp
        io/channel/Channel.cpp
        io/channel/InChannel.cpp
        io/channel/OutChannel.cpp
        io/channel/file/FileChannel.cpp
        io/channel/file/MemoryMappedFileChannel.cpp
        io/channel/file/FileEntry.cpp
        io/channel/file/Directory.cpp
        io/channel/listener/Listener.cpp
        io/channel/listener/EpollListener.cpp
        io/channel/network/ServerSocketChannel.cpp
        io/channel/network/SocketChannel.cpp
        io/channel/network/TcpServerSocketChannel.cpp
        io/channel/network/TcpSocketChannel.cpp
        io/channel/terminal/TerminalPrintChannel.cpp
        io/channel/terminal/TerminalScanChannel.cpp

        reflector/dynamic_reflect.cpp

        parser/MapParser.cpp
        parser/ListParser.cpp
        parser/xml/XmlNode.cpp

        logger/Logger.cpp

#        serialize/Serializable.cpp

        )
#add_library(anarionbase SHARED ${DIR_SRCS})

# subdirectories
#add_subdirectory(allocator)
#add_subdirectory(concurrent)
#add_subdirectory(container)
#add_subdirectory(reflector)
#add_subdirectory(context)
#add_subdirectory(io)
#target_link_libraries(anarionbase )#ALLOCATOR CONCURRENT CONTAINER REFLECTOR IO CONTEXT)

target_link_libraries(anarionbase pthread)
include_directories(${CMAKE_SOURCE_DIR}/include)

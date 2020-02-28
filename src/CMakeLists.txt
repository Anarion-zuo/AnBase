cmake_minimum_required(VERSION 3.10)
project (MyCppBase)

#aux_source_directory(. DIR_SRCS)
add_library(anarionbase STATIC
        allocator/Allocator.cpp
        allocator/DynamicListAllocator.cpp
        allocator/ListAllocator.cpp
        allocator/PoolAllocator.cpp

        concurrent/base/Barrier.cpp
        concurrent/base/CondVar.cpp
        concurrent/base/Mutex.cpp
        concurrent/base/RwMutex.cpp
        concurrent/base/Thread.cpp

        concurrent/pool/ThreadPool.cpp

        container/String.cpp

        context/Callable.cpp
        context/Context.cpp
        context/LocalContext.cpp

        io/buffer/Buffer.cpp
        io/channel/file/FileChannel.cpp
        io/channel/file/MemoryMappedFileChannel.cpp
        io/channel/listener/Listener.cpp
        io/channel/listener/EpollListener.cpp
        io/channel/network/ServerSocketChannel.cpp
        io/channel/network/SocketChannel.cpp
        io/channel/network/TcpServerSocketChannel.cpp
        io/channel/network/TcpSocketChannel.cpp

        reflector/dynamic_reflect.cpp
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

include_directories(${CMAKE_SOURCE_DIR}/include)

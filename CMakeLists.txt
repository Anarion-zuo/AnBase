cmake_minimum_required(VERSION 3.16)
project (AnBase)

# project settings
set(INC_DIR "include")
set(SRC_DIR "src")
set(TEST_DIR "test")

# all objects here
set(ANBASE_OBJS
        allocator/Allocator
        allocator/ListConcurrentAllocator
        allocator/FixedLengthAllocator

        allocator/page_alloc/SlotPage
        allocator/page_alloc/FreeSlotInfo
        allocator/page_alloc/PageAllocator

        concurrent/base/Barrier
        concurrent/base/CondVar
        concurrent/base/Mutex
        concurrent/base/RwMutex
        concurrent/base/Thread
        concurrent/base/BackgroundTask
        concurrent/base/Routine

        concurrent/pool/ThreadPool
        concurrent/pool/AsyncInvoke
        concurrent/pool/AsyncCaller
        concurrent/pool/DynamicThreadPool

        container/String
        container/StringBuilder
        container/Trie
        container/BitArray

        context/Callable
        context/Context
        context/LocalContext

        io/buffer/Buffer
        io/buffer/Buffer
        io/channel/Channel
        io/channel/InChannel
        io/channel/OutChannel
        io/channel/file/FileChannel
        io/channel/file/FileEntry
        io/channel/file/Directory
        io/channel/file/MemoryMappedFile
        io/channel/file/FileSystemNotifier
        #        io/channel/network/ServerSocketChannel.cpp
        #        io/channel/network/SocketChannel.cpp
        #        io/channel/network/TcpServerSocketChannel.cpp
        #        io/channel/network/TcpSocketChannel.cpp
        #        io/channel/terminal/TerminalPrintChannel.cpp
        #        io/channel/terminal/TerminalScanChannel.cpp
        io/channel/path/Path
        reflector/dynamic_reflect
        parser/MapParser
        parser/ListParser
        parser/xml/XmlElement
        parser/xml/xml-parse
        parser/xml/XmlLeaf
        parser/xml/XmlNode
        parser/json/JsonObject
        parser/json/JsonArray
        parser/json/JsonNumber
        parser/json/JsonMap
        parser/json/JsonString
        parser/json/JsonBoolean
        parser/json/parse-json
        logger/Logger
        time/Time
        time/Date
        pointers/PointerManager
        runtime/RunTimeData
        )
# derive source file paths from object names
set(ANBASE_SRCS ${ANBASE_OBJS})
set(SRCS_PREFIX "${SRC_DIR}/")
set(SRCS_SUFFIX ".cpp")
list(TRANSFORM ANBASE_SRCS PREPEND ${SRCS_PREFIX})
list(TRANSFORM ANBASE_SRCS APPEND ${SRCS_SUFFIX})
add_library(anbase STATIC ${ANBASE_SRCS})

target_link_libraries(anbase pthread)
include_directories(${INC_DIR})

# gtest framework
add_subdirectory(${TEST_DIR})
target_link_libraries(gtests anbase)

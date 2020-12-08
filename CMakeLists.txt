cmake_minimum_required(VERSION 3.16)
project (AnBase)

# project settings
set(INC_DIR "include")
set(SRC_DIR "src")
set(TEST_DIR "test")

# all objects here
set(ANBASE_OBJS
        allocator/Allocator.cpp
        allocator/SimpleAllocator.cpp
        allocator/ListConcurrentAllocator.cpp
        allocator/FixedLengthAllocator.cpp

        allocator/page_alloc/SlotPage.cpp
        allocator/page_alloc/FreeSlotInfo.cpp
        allocator/page_alloc/PageAllocator.cpp

        concurrent/base/Barrier.cpp
        concurrent/base/CondVar.cpp
        concurrent/base/Mutex.cpp
        concurrent/base/RwMutex.cpp
        concurrent/base/Thread.cpp
        concurrent/base/BackgroundTask.cpp
        concurrent/base/Routine.cpp

        concurrent/pool/ThreadPool.cpp
        concurrent/pool/AsyncInvoke.cpp
        concurrent/pool/AsyncCaller.cpp
        concurrent/pool/DynamicThreadPool.cpp

        container/String.cpp
        container/StringBuilder.cpp
        container/Trie.cpp
        container/BitArray.cpp

        context/Callable.cpp
        context/Context.cpp
        context/LocalContext.cpp

        io/buffer/Buffer.cpp
        io/buffer/Buffer.cpp
        io/channel/Channel.cpp
        io/channel/InChannel.cpp
        io/channel/OutChannel.cpp
        io/channel/file/FileChannel.cpp
#        io/channel/file/FileEntry
#        io/channel/file/Directory
        io/channel/file/MemoryMappedFile.cpp
#        io/channel/file/FileSystemNotifier
        #        io/channel/network/ServerSocketChannel.cpp
        #        io/channel/network/SocketChannel.cpp
        #        io/channel/network/TcpServerSocketChannel.cpp
        #        io/channel/network/TcpSocketChannel.cpp
        io/channel/terminal/TerminalPrintChannel.cpp
        io/channel/terminal/TerminalScanChannel.cpp
        io/fs/FileSystem.cpp
        io/fs/Path.cpp

        reflector/dynamic_reflect.cpp
        parser/MapParser.cpp
        parser/ListParser.cpp
        parser/xml/XmlElement.cpp
        parser/xml/xml-parse.cpp
        parser/xml/XmlLeaf.cpp
        parser/xml/XmlNode.cpp
        parser/json/JsonObject.cpp
        parser/json/JsonArray.cpp
        parser/json/JsonNumber.cpp
        parser/json/JsonMap.cpp
        parser/json/JsonString.cpp
        parser/json/JsonBoolean.cpp
        parser/json/parse-json.cpp
        parser/regular/RegularExpression.cpp
        parser/regular/CharNFA.cpp
        parser/regular/RegularParser.cpp
        parser/regular/NFANode.cpp

        serialize/Serializer.cpp
        serialize/logger/LogEntry.cpp
        serialize/logger/LogLevel.cpp

        logger/Logger.cpp
        time/Time.cpp
        time/Date.cpp
        pointers/PointerManager.cpp
        runtime/RunTimeData.cpp

        data/db/FileBlockManager.cpp
        data/db/PageManager.cpp
        data/db/BufferManager.cpp
        data/db/PageMappedObject.cpp
        data/db/IndexRangeRegistry.cpp
        data/db/loginfo/DataBaseLogEntry.cpp
        data/db/IdRegistry.cpp
        )
# derive source file paths from object names
set(ANBASE_SRCS ${ANBASE_OBJS})
set(SRCS_PREFIX "${SRC_DIR}/")
#set(SRCS_SUFFIX "")
list(TRANSFORM ANBASE_SRCS PREPEND ${SRCS_PREFIX})
#list(TRANSFORM ANBASE_SRCS APPEND ${SRCS_SUFFIX})
add_library(anbase STATIC ${ANBASE_SRCS})

target_link_libraries(anbase pthread)
include_directories(${INC_DIR})

# gtest framework
add_subdirectory(${TEST_DIR})
target_link_libraries(gtests anbase)

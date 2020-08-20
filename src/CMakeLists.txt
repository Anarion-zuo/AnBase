cmake_minimum_required(VERSION 3.10)
project (AnBase)

#aux_source_directory(. DIR_SRCS)
add_library(anbase STATIC
        allocator/Allocator.cpp
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
        io/channel/file/FileEntry.cpp
        io/channel/file/Directory.cpp
        io/channel/file/MemoryMappedFile.cpp
        io/channel/file/FileSystemNotifier.cpp
        io/channel/network/ServerSocketChannel.cpp
        io/channel/network/SocketChannel.cpp
        io/channel/network/TcpServerSocketChannel.cpp
        io/channel/network/TcpSocketChannel.cpp
        io/channel/terminal/TerminalPrintChannel.cpp
        io/channel/terminal/TerminalScanChannel.cpp
        io/channel/path/Path.cpp

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

        logger/Logger.cpp

        time/Time.cpp
        time/Date.cpp

        pointers/PointerManager.cpp

        )

target_link_libraries(anbase pthread)
include_directories(../include)

# tests
add_subdirectory(test)


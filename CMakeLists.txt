cmake_minimum_required(VERSION 3.16)
project (AnBase)
# platform derivation
if(${APPLE})
    message("On MacOSX")
    include_directories(/usr/local/Cellar/googletest/1.10.0/include)
elseif(${UNIX})
    message("On Unix except MacOSX, possibly linux")
elseif(${WIN32})
    message("On WIN32")
else()
    message("Unknown CMake Platform!!!")
endif()
# load google test
configure_file(GTestCmake.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()
# end googletest

# compiler flags
set(CLANG_FLAGS "-Wall -Xclang -fsized-deallocation -std=c++14 -stdlib=libc++")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CLANG_FLAGS}")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    set(CMAKE_CXX_FLAGS "${GCC_FLAGS}")
endif()

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
        concurrent/pool/AbstractThreadPool.cpp
        concurrent/pool/ArrayThreadPool.cpp

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
        parser/regular/NFA.cpp

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
include_directories(${SRC_DIR})

# gtest framework
add_subdirectory(${TEST_DIR})
target_link_libraries(gtests anbase)

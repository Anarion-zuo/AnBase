project(AnBase)

# project settings
set(INC_DIR "../include")
set(SRC_DIR "../src")

add_executable(gtests

        containers/TestBplusTreeSet.cpp
        containers/TestArray.cpp
        containers/TestVector.cpp
        containers/TestBplusTreeMap.cpp
        containers/TestTuple.cpp
        containers/TestHashSet.cpp
        containers/TestCopier.cpp
        containers/TestProxy.cpp
        containers/TestBplusTree.cpp
        containers/TestString.cpp
        containers/TestSoftArray.cpp
        containers/TestBitArray.cpp
        containers/TestNumLinkedList.cpp

        file/TestFileChannel.cpp

        concurrent/TestRoutine.cpp
        concurrent/TestThread.cpp
        concurrent/TestArrayThreadPool.cpp

        reflector/TestStaticReflector.cpp
        reflector/TestDynamicReflector.cpp

        db/storage/TestFreeList.cpp
        db/storage/TestLruList.cpp
        db/storage/TestPage.cpp
        db/TestIndexRangeRegistry.cpp
        db/TestNamedAllocate.cpp
        db/column/TestGeneralColumn.cpp

        log/TestLog.cpp

        time/TestTime.cpp

        function/TestFunction.cpp
        )

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

target_link_libraries(gtests gtest gtest_main)
include_directories(${INC_DIR})

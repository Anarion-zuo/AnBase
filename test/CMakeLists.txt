project(AnBase)

# project settings
set(INC_DIR "../include")
set(SRC_DIR "../src")

add_executable(gtests

        containers/TestBplusTreeSet.cpp
        containers/TestVector.cpp
        containers/TestBplusTreeMap.cpp
        containers/TestTuple.cpp

#        file/TestFileChannel.cpp

        concurrent/TestRoutine.cpp
        concurrent/TestThread.cpp
        )

target_link_libraries(gtests gtest gtest_main)
include_directories(${INC_DIR})

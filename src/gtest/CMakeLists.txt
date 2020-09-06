project(gtests)

include_directories(../../include)

add_executable(gtests

        containers/TestBplusTreeSet.cpp
        containers/TestVector.cpp
        containers/TestBplusTreeMap.cpp

#        file/TestFileChannel.cpp

        concurrent/TestRoutine.cpp
        )

target_link_libraries(gtests gtest gtest_main anbase)

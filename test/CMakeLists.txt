project(AnBase)

# project settings
set(INC_DIR "../include")
set(SRC_DIR "../src")

add_executable(gtests

        containers/TestBplusTreeSet.cpp
        containers/TestVector.cpp
        containers/TestBplusTreeMap.cpp
        containers/TestTuple.cpp
        containers/TestHashSet.cpp

        file/TestFileChannel.cpp

        concurrent/TestRoutine.cpp
        concurrent/TestThread.cpp

        reflector/TestStaticReflector.cpp
        reflector/TestDynamicReflector.cpp
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

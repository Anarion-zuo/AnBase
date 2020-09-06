//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <concurrent/base/Routine.h>
#include <concurrent/base/Thread.h>

using namespace anarion;

static void printHello() {
    printf("Hello Caller\n");
}

class HelloRoutine {
public:
    void run() {
        printf("Hello Routine\n");
    }

    void operator()() {
        run();
    }
};

static void printSeries(size_type begin, size_type end) {
    for (; begin < end; ++begin) {
        printf("%lu ", begin);
    }
    printf("\n");
}

//TEST(TestConcurrent, TestRoutine) {
//    // test basic utils
//    printHello();
//    HelloRoutine()();
//}

TEST(TestConcurrent, TestThread) {
    // call simple object
    Thread<HelloRoutine> thread;
    thread.start();
    thread.join();
}

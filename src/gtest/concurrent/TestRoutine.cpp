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

static void incrementOneReference(size_type &num) {
    ++num;
}
static void incrementOnePointer(size_type *num) {
    ++*num;
}

TEST(TestRoutine, NoReturnWithParams) {
    // pass by value
    auto tupleRoutine = makeBindRoutine(printSeries, 0, 100);
    tupleRoutine.run();

    // pass by pointer
    size_type beforeInc = 4, afterInc = beforeInc + 1;
    auto incrementOneRoutinePointer = makeBindRoutine(incrementOnePointer, &beforeInc);
    incrementOneRoutinePointer.run();
    ASSERT_EQ(beforeInc, afterInc);

    // pass by reference
    beforeInc = 4, afterInc = beforeInc + 1;
    auto incrementOneRoutineReference = makeBindRoutine(incrementOneReference, beforeInc);
    incrementOneRoutinePointer.run();
    ASSERT_EQ(beforeInc, afterInc);

}

static size_type incrementOneReturn(size_type num) {
    return num + 1;
}

TEST(TestRoutine, NoParamsWithturn) {
    size_type beforeInc = 4, afterInc = beforeInc + 1;
    auto incrementOneRoutine = makeBindRoutine<size_type>(incrementOneReturn, beforeInc);
    incrementOneRoutine.run();
    ASSERT_EQ(incrementOneRoutine.getReturn(), afterInc);
}

TEST(TestConcurrent, TestThread) {
    // call simple object
    Thread<HelloRoutine> thread;
    thread.start();
    thread.join();


}

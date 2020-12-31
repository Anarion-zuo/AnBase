//
// Created by anarion on 9/7/20.
//

#include <gtest/gtest.h>
#include <concurrent/base/Routine.h>
#include <concurrent/base/Thread.h>

using namespace anarion;

static void printHello() {
    printf("Hello Thread\n");
}

TEST(TestThread, TestThreadLoad) {
    auto thread = makeThread(makeBindRoutine(printHello));
    thread.start();
    thread.join();
}

static void incrementOne(size_type &num) {
    printf("In child thread: num %lx\n", &num);
    ++num;
}

TEST(TestThread, ThreadPassReference) {
    size_type beforeInc = 1, afterInc = beforeInc + 1;
    auto thread = makeThread(makeBindRoutine(incrementOne, beforeInc));
    printf("In parent thread: beforeInc 0x%lx parameter 0x%lx\n", &beforeInc, &thread.getRoutine().getArgs().get());
    thread.start();
    thread.join();
    ASSERT_EQ(beforeInc, afterInc);
}

static void incrementOnePointer(size_type *num) {
    printf("In child thread: num %lx\n", num);
    ++*num;
}

TEST(TestThread, ThreadPassPointer) {
    size_type beforeInc = 1, afterInc = beforeInc + 1;
    auto thread = makeThread(makeBindRoutine(incrementOnePointer, &beforeInc));
    printf("In parent thread: beforeInc 0x%lx parameter 0x%lx\n", &beforeInc, thread.getRoutine().getArgs().get());
    thread.start();
    thread.join();
    ASSERT_EQ(beforeInc, afterInc);
}

TEST(TestThread, Restart) {
    auto thread = makeThread([] {
        printf("hello there\n");
    });
    thread.start();
    thread.join();
    thread.start();
    thread.join();
}

TEST(TestThread, Cancel) {
    auto thread = makeThread([] {
        size_type times = 0;
        while (true) {
            printf("hello there for the %lu time\n", times);
            ++times;
            ThreadCore::sleep(Time(1, 0));
        }
    });
    thread.start();
    thread.cancel();
    thread.join();
}

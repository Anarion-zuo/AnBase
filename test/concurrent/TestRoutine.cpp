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

TEST(TestRoutine, NoParamsWithReturn) {
    size_type beforeInc = 4, afterInc = beforeInc + 1;
    auto incrementOneRoutine = makeBindRoutine<size_type>(incrementOneReturn, beforeInc);
    incrementOneRoutine.run();
    ASSERT_EQ(incrementOneRoutine.getReturn(), afterInc);
}

template <typename T, typename V>
size_type addTwo(T x, V y) {
    return x + y;
}

TEST(TestRoutine, TemplateFunction) {
    // must specify parameters
    auto tempRoutine = makeBindRoutine<size_type>(addTwo<int, int>, 1, 3);
    tempRoutine.run();
    ASSERT_EQ(tempRoutine.getReturn(), 1 + 3);
}

template <typename firstType, typename ...NumType>
size_type sumVari(firstType firstNum, NumType ...nums) {
    return sumVari(nums...) + firstNum;
}

template <typename ...NumType>
size_type sumVari() {
    return 0;
}

TEST(TestRoutine, VariadicWithReturn) {
    // best not use variadic function with routine
    /*
    auto sumVariRoutine = makeBindRoutine<size_type>(sumVari<size_type, size_type, size_type, size_type>, 1ul, 2ul, 3ul);
    sumVariRoutine.run();
    ASSERT_EQ(sumVariRoutine.getReturn(), 1 + 2 + 3);
     */
}

TEST(TestConcurrent, TestThread) {
    // call simple object
    Thread<HelloRoutine> thread;
    thread.start();
    thread.join();


}

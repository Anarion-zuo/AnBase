//
// Created by anarion on 8/6/20.
//

#include "test/TestClass.h"

anarion::TestClass::TestClass() {

}

void anarion::TestClass::runTest() {
    prePrint();
    callRunner();
    postPrint();
}

void anarion::TestClass::callRunner() {
    beginTime.setCurrent();
    runner();
    endTime.setCurrent();
    durationSeconds = Time {Time::difference(endTime, beginTime)}.getDouble();
}

void anarion::TestClass::postPrint() const {
    printf("=====Test Ends=====\n");
    printf("Duration time: %lf\n", durationSeconds);
}

void anarion::TestClass::prePrint() {
    printf("====Test Begins====\n");
}

void anarion::TestClass::throwTestFailed() {
    throw TestFailed();
}

const char *anarion::TestClass::TestFailed::what() const noexcept {
    printf("====Test Failed====\n");
    return "Test Failed...";
}

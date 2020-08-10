//
// Created by anarion on 8/6/20.
//

#include "test/TestClass.h"

anarion::TestClass::TestClass()
    : rootBenchMarker(this) {
    rootBenchMarker.testClass = this;
}

void anarion::TestClass::runTest() {
    prePrint();
    callRunner();
    postPrint();
}

void anarion::TestClass::callRunner() {
    runner();
}

void anarion::TestClass::postPrint() const {
    printf("=====Test Ends=====\n");
}

void anarion::TestClass::prePrint() {
    printf("====Test Begins====\n");
}

void anarion::TestClass::throwTestFailed() {
    throw TestFailed();
}

void anarion::TestClass::run() {
    rootBenchMarker.runBenchMark();
}

const char *anarion::TestClass::TestFailed::what() const noexcept {
    printf("====Test Failed====\n");
    return "Test Failed...";
}

void anarion::TestClass::RootBenchMarker::testee() {
    testClass->runTest();
}

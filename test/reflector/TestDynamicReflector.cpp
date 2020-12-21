//
// Created by anarion on 11/17/20.
//

#include <gtest/gtest.h>
#include "reflection/dynamic_reflect.h"

using namespace anarion;

class TestReflectClass {
public:
    int num;

    void hello() {
        printf("Hello TestReflectClass\n");
    }

    int getNum() {
        return num;
    }

    int numAdd(int inc) {
        return num + inc;
    }
};

TEST(TestDynamicReflector, TestOffsetOf) {
}

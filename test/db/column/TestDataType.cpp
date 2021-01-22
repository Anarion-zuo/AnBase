//
// Created by anarion on 1/22/21.
//

#include <gtest/gtest.h>
#include "data/db/types/Integers.hpp"

using namespace anarion;
using namespace anarion::db;

TEST(TestIntegers, TestInt8) {
    Int8 int8;
    char buf[1024];
    int8_t number = rand();
    int8.set(number);
    int8.writeBuffer(buf);
    int8.loadBuffer(buf);
    ASSERT_EQ(int8.get(), number);
}

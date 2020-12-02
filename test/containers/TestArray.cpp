//
// Created by anarion on 11/30/20.
//

#include <gtest/gtest.h>
#include "container/List/Array.h"

using namespace anarion;

TEST(TestArray, TestInit) {
    Array<uint16_t> array(10);
    array.pushBack(1);
    ASSERT_EQ(1, array.popBack());
}

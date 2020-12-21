//
// Created by anarion on 12/8/20.
//

#include <gtest/gtest.h>
#include <container/List/SoftArray.h>

using namespace anarion;

TEST(TestSoftArray, TestInit) {
    SoftArray<int16_t> array;
    size_type index= 1000;
    int16_t num = 99;
    array.set(index, num);
    ASSERT_EQ(num, array.get(index));
}

TEST(TestSoftArray, TestWideIndices) {
    SoftArray<int16_t> array;
    for (size_type time = 0; time < 10000; ++time) {
        size_type index = rand() % 10000;
        int16_t num = rand() % 1000;
        array.set(index, num);
        ASSERT_EQ(num, array.get(index));
    }
}

//
// Created by anarion on 12/9/20.
//

#include <gtest/gtest.h>
#include <container/List/BitArray.h>

using namespace anarion;

TEST(TestBitArray, TestInit) {
    BitArray array(100);
    ASSERT_EQ(array.getLength(), 100);
}

TEST(TestBitArray, TestSetCheck) {
    BitArray array;
    size_type setLength = 100;
    for (size_type index = 0; index < setLength; ++index) {
        array.set(index);
        ASSERT_TRUE(array.check(index));
    }
}

TEST(TestBitArray, TestRnage) {
    size_type setLength = 100;
    BitArray array(setLength);
    for (size_type index = 0; index < setLength; ++index) {
        array.set(index);
        ASSERT_TRUE(array.check(index));
    }
    ASSERT_FALSE(array.check(setLength + 1));
}

TEST(TestBitArray, TestClear) {
    size_type setLength = 100;
    BitArray array;
    for (size_type index = 0; index < setLength; ++index) {
        array.set(index);
        ASSERT_TRUE(array.check(index));
    }
    for (size_type index = 0; index < setLength; ++index) {
        array.clear(index);
        ASSERT_FALSE(array.check(index));
    }
}

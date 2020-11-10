//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <container/List/Vector.hpp>
using namespace anarion;

TEST(TestContainers, TestVector) {
    Vector<size_type> vector;

    // test push_back
    size_type pushSize = 1000000ul;
    for (size_type i = 0; i < pushSize; ++i) {
        vector.push_back(i);
    }
    ASSERT_EQ(vector.size(), pushSize);
    vector.clear();

    // test insert
    size_type insertSize = 200000ul;
    for (size_type i = 0; i < insertSize; ++i) {
        vector.insert(0ul, i);
    }
    ASSERT_EQ(vector.size(), insertSize);

    // test insert at half
    for (size_type i = 0; i < insertSize; ++i) {
        vector.insert(insertSize / 2, i);
    }
    ASSERT_EQ(vector.size(), insertSize * 2);

    // test remove at half
    for (size_type i = 0; i < insertSize; ++i) {
        vector.remove(insertSize / 2);
    }
    ASSERT_EQ(vector.size(), insertSize);
}

static bool isPowerOf2(int num) {
    return !(num & (num - 1));
}

/**
 * @test TestPush
 * @details Checking features:
 *      1) Vector expands only when size() == capacity(), making size() is a power of 2 true in all such time.
 *      2) the order of elements after pushing should be in accord as desired.
 */
TEST(TestVector, TestPush) {
    Vector<int> vector;
    const unsigned int length = 1000;
    // push_back
    // check expand action
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(i, vector.size());
        if (vector.capacity() == vector.size()) {
            printf("[TestPush] Expanding size at size %ld\n", vector.size());
            ASSERT_TRUE(isPowerOf2(vector.size()));
        }
        vector.push_back((int)i);
    }
    // check push action
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(vector.get(i), i);
    }
}
/**
 * @test TestPop
 * @details Checking features:
 *      1) Pop actions.
 */
TEST(TestVector, TestPop) {
    Vector<int> vector;
    const unsigned int length = 1000;
    // prepare a vector
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(i, vector.size());
        vector.push_back((int)i);
    }
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(vector.size(), length - i);
        ASSERT_EQ(vector.get(vector.size() - 1), length - i - 1);
        ASSERT_EQ(vector.pop_back(), length - 1 - i);
    }
}

/**
 * @details Checking features:
 *      1)
 */
TEST(TestVector, TestInsert) {

}

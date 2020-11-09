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

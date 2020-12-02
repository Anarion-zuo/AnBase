//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <container/List/Vector.hpp>
using namespace anarion;

static Vector<int> makeVector(size_type length) {
    Vector<int> vector (length);
    for (size_type index = 0; index < length; ++index) {
        vector.pushBack(index);
    }
    return move(vector);
}

TEST(TestContainers, TestVector) {
    Vector<size_type> vector;

    // test pushBack
    size_type pushSize = 1000000ul;
    for (size_type i = 0; i < pushSize; ++i) {
        vector.pushBack(i);
    }
    ASSERT_EQ(vector.size(), pushSize);
    vector.clear();

    // test insert
    size_type insertSize = 2000ul;
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
    const unsigned int length = 1000;
    Vector<int> vector = makeVector(length);
    // pushBack
    // check expand action
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(i, vector.size());
        if (vector.capacity() == vector.size()) {
            printf("[TestPush] Expanding size at size %ld\n", vector.size());
            ASSERT_TRUE(isPowerOf2(vector.size()));
        }
        vector.pushBack((int) i);
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
    const unsigned int length = 1000;
    Vector<int> vector = makeVector(length);
    // prepare a vector
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(i, vector.size());
        vector.pushBack((int) i);
    }
    for (unsigned int i = 0; i < length; ++i) {
        ASSERT_EQ(vector.size(), length - i);
        ASSERT_EQ(vector.get(vector.size() - 1), length - i - 1);
        ASSERT_EQ(vector.popBack(), length - 1 - i);
    }
}

static void checkInsertOneTest(Vector<int> &vector, const unsigned int length, const unsigned int insertIndex, int insertElement) {
    for (size_type index = 0; index < length + 1; ++index) {
        if (index == insertIndex) {
            ASSERT_EQ(insertElement, vector.get(index));
        } else if (index < insertIndex) {
            ASSERT_EQ(index, vector.get(index));
        } else {
            ASSERT_EQ(index, vector.get(index) + 1);
        }
    }
}

/**
 * @details Checking features:
 *      - Insert at several locations
 */
TEST(TestVector, TestInsertOne) {
    const unsigned int length = 1000;
    const int insertElement = 7589;
    // test front
    Vector<int> vector = makeVector(length);
    vector.insert(0ul, insertElement);
    checkInsertOneTest(vector, length, 0, insertElement);
    // test back
    vector = makeVector(length);
    vector.insert(vector.size(), insertElement);
    checkInsertOneTest(vector, length, vector.size() - 1, insertElement);
    // test middle
    vector = makeVector(length);
    vector.insert(vector.size() / 2, insertElement);
    checkInsertOneTest(vector, length, (vector.size() - 1) / 2, insertElement);
}

static void checkInsertSeriesTest(Vector<int> &vector, Vector<int> &insertVector, size_type insertIndex) {
    for (size_type index = 0; index < vector.size(); ++index) {
        if (index < insertIndex) {
            ASSERT_EQ(index, vector.get(index));
        } else if (index >= insertIndex + insertVector.size()) {
            ASSERT_EQ(index - insertVector.size(), vector.get(index));
        } else {
            ASSERT_EQ(index - insertIndex, vector.get(index));
        }
    }
}

/**
 * @details Checking features:
 *      - Insert at all possible locations.
 *      - Expand when out-of-space.
 */
TEST(TestVector, TestInsertSeries) {
    size_type insertLength = 50;
    Vector<int> insertVector = makeVector(insertLength), vector;
    // insert at front
    for (size_type length = insertLength; length < 100; length++) {
        for (size_type insertIndex = 0; insertIndex < length; ++insertIndex) {
            vector = makeVector(length);
            vector.insert(vector.beginIterator() + insertIndex, insertVector.beginIterator(), insertVector.endIterator());
            ASSERT_EQ(vector.capacity(), vector.size() * 2);
            checkInsertSeriesTest(vector, insertVector, insertIndex);
        }
    }
}

static void checkRemoveTest(Vector<int> &vector, size_type removeIndex, size_type removeLength) {
    for (size_type index = 0; index < vector.size(); ++index) {
        if (index < removeIndex) {
            ASSERT_EQ(index, vector.get(index));
        } else {
            ASSERT_EQ(index + removeLength, vector.get(index));
        }
    }
}

/**
 * @details Checking features:
 *      - Remove at all possible locations.
 *      - Shrink when size() too small w.r.t. capacity().
 */
TEST(TestVector, TestRemove) {
    size_type removeLength = 50;
    for (size_type length = removeLength; length < 100; ++length) {
        for (size_type removeIndex = 0; removeIndex + removeLength < length; ++removeIndex) {
            auto vector = makeVector(length);
            vector.remove(removeIndex, removeLength);
            if (length - removeLength < length / 3) {
                printf("Removing length %ld too small for vector length %ld\n", removeLength, length);
//                ASSERT_EQ(vector.capacity(), length / 3);
            } /*else {
                printf("Removing length %ld not too large for vector length %ld\n", removeLength, length);
            }*/
            checkRemoveTest(vector, removeIndex, removeLength);
        }
    }
}

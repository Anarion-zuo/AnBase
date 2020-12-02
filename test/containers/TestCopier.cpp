//
// Created by anarion on 11/26/20.
//

#include <gtest/gtest.h>
#include <container/base/Copier.hpp>

using namespace anarion;

TEST(TestCopier, TestCopy) {
    size_type arrLength = 30;
    int arr[arrLength], arr2[arrLength];
    for (size_type index = 0; index < arrLength; ++index) {
        arr[index] = index;
    }
    Copier<int>().copy(arr2, arr, arrLength);
    for (size_type index = 0; index < arrLength; ++index) {
        ASSERT_EQ(arr[index], arr2[index]);
    }
}

static void callMoveOverlap(int *oldarr, int *oparr, size_type arrLength, size_type subLength, size_type moveLength) {
    Copier<int>().moveOverLap(oparr + moveLength, oparr, subLength);
    for (size_type index = 0; index < arrLength; ++index) {
        if (index < moveLength) {
            ASSERT_EQ(oparr[index], oldarr[index]);
        } else {
            if (index < subLength + moveLength) {
                ASSERT_EQ(oldarr[index - moveLength], oparr[index]);
            } else {
                ASSERT_EQ(oparr[index], oldarr[index]);
            }
        }
    }
}

TEST(TestCopier, TestOverlap) {
    size_type arrLength = 30;
    int arr[arrLength], arr2[arrLength];
    for (size_type index = 0; index < arrLength; ++index) {
        arr[index] = index;
    }
    Copier<int>().copy(arr2, arr, arrLength);
    size_type subLength = 20, moveLength = 5;
    callMoveOverlap(arr, arr2, arrLength, subLength, moveLength);
}

static uint32_t *genArray(size_type length) {
    uint32_t *arr = new uint32_t [length];
    for (size_type index = 0; index < length; ++index) {
        arr[index] = index;
    }
    return arr;
}

TEST(TestCopier, TestMoveForward) {
    size_type arrLength = 100;
    uint32_t *arr = genArray(arrLength), *arr2 = genArray(arrLength);
    size_type beginIndex = 50, steps = 10;
    Copier<uint32_t>().moveForward(arr, arrLength, beginIndex, steps);
    for (size_type index = 0; index < arrLength - steps; ++index) {
        if (index < beginIndex - steps) {
            ASSERT_EQ(arr[index], index);
        } else {
            ASSERT_EQ(arr[index], index + steps);
        }
    }
    delete []arr;
    delete []arr2;
}

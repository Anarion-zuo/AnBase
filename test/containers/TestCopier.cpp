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

TEST(TestCopierm, TestOverlap) {
    size_type arrLength = 30;
    int arr[arrLength], arr2[arrLength];
    for (size_type index = 0; index < arrLength; ++index) {
        arr[index] = index;
    }
    Copier<int>().copy(arr2, arr, arrLength);
    size_type subLength = 20, moveLength = 5;
    callMoveOverlap(arr, arr2, arrLength, subLength, moveLength);
}

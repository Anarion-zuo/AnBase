//
// Created by anarion on 8/7/20.
//

#include "../classes/TestVector.h"
#include "../classes/TestString.h"
#include "../classes/TestHashSet.h"
#include "../../src/test/TestBitArray.h"
#include "../classes/TestBplusTreeSet.h"
#include "../classes/TestBplusTreeMap.h"
#include "../classes/TestFileChannel.h"
#include "../classes/TestPath.h"

bool binarySearch(int *arr, const int &obj, int &begin, int &end) {
    while (end - begin > 1) {
        int m = ((end - begin) >> 1) + begin;
        if (arr[m] < obj) {
            // begin indexed element is always smaller than obj
            begin = m;
        } else if (arr[m] == obj) {
            begin = m;
            end = m + 1;
            return true;
        } else {
            end = m;
        }
    }
    ++begin;
    ++end;
    return arr[begin] == obj;
}

int main() {
//    anarion::TestVector().run();
//    anarion::TestString().run();
//    anarion::TestHashSet().run();
//    anarion::TestBitArray().run();

    int arr[] = { 0, 1, 2, 3, 4, 5};
    int begin = 0, end = 4;
//    binarySearch(arr, 3, begin, end);

//    anarion::TestBplusTreeSet().run();
//    anarion::TestBplusTreeMap().run();
//    anarion::TestFileChannel().run();
    anarion::TestPath().run();
}
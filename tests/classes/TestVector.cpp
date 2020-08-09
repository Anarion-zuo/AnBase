//
// Created by anarion on 8/7/20.
//

#include "TestVector.h"

void anarion::TestVector::runner() {
    Vector<int> vector;

    for (int i = 0; i < 100; ++i) {
        vector.insert(0ul, i);
    }
    if (vector.size() != 100) {
        throw TestFailed();
    }
    for (int i = 0; i < 100; ++i) {
        if (vector.get(i) != 99 - i) {
            throw TestFailed();
        }
    }

    vector.clear();
    for (int i = 0; i < 100; ++i) {
        vector.insert((size_type)i, i);
    }
    if (vector.size() != 100) {
        throw TestFailed();
    }
    for (int i = 0; i < 100; ++i) {
        if (vector.get(i) != i) {
            throw TestFailed();
        }
    }

    for (int i = 0; i < 50; ++i) {
        vector.remove(0ul);
        if (vector.size() != 100 - i - 1) {
            throw TestFailed();
        }
    }
    if (vector.size() != 50) {
        throw TestFailed();
    }
    for (int i = 50; i < 100; ++i) {
        if (vector.get(i - 50) != i) {
            throw TestFailed();
        }
    }

    for (int i = 0; i < 4; ++i) {
        vector.remove(0ul, 4);
        if (vector.get(0) != 50 + 4 + i * 4 || vector.size() != 50 - i * 4 - 4) {
            throw TestFailed();
        }
    }

}

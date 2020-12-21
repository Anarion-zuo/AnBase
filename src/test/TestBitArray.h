//
// Created by anarion on 8/11/20.
//

#ifndef ANBASE_TESTBITARRAY_H
#define ANBASE_TESTBITARRAY_H

#include <container/List/BitArray.h>

namespace anarion {
    class TestBitArray : public TestClass {
    protected:
        void check(BitArray &bitArray, bool orAll, bool andAll) {
            if (bitArray.orAll() != orAll) {
                throwTestFailed();
            }
            if (bitArray.andAll() != andAll) {
                throwTestFailed();
            }
        }

        void runner() override {
            size_type bitArraySize = 999;
            BitArray bitArray(bitArraySize);

            if (bitArray.getLength() != bitArraySize) {
                throwTestFailed();
            }
            check(bitArray, false, false);

            bitArray.set(44);
            check(bitArray, true, false);

            for (size_type i = 0; i < 1000; ++i) {
                if (i % 2) {
                    bitArray.set(i);
                } else {
                    bitArray.clear(i);
                }
            }
            check(bitArray, true, false);
            for (size_type i = 0; i < 1000; ++i) {
                if (i % 2) {
                    if (!bitArray.check(i)) {
                        throwTestFailed();
                    }
                } else {
                    if (bitArray.check(i)) {
                        throwTestFailed();
                    }
                }
            }

            bitArray.resize(100);
            for (size_type i = 0; i < 100; ++i) {
                if (i % 2) {
                    if (!bitArray.check(i)) {
                        throwTestFailed();
                    }
                } else {
                    if (bitArray.check(i)) {
                        throwTestFailed();
                    }
                }
            }
            check(bitArray, true, false);
        }
    };
}

#endif //ANBASE_TESTBITARRAY_H

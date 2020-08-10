//
// Created by anarion on 8/10/20.
//

#ifndef ANBASE_TESTVECTOR_H
#define ANBASE_TESTVECTOR_H

#include <test/TestClass.h>
#include <vector>

namespace anarion {
    class TestVector : public TestClass {
    protected:
        void runner() override {
            Vector<size_type> vector;
            std::vector<size_type> stdVector;
            Time begin, end;
            double mydiff, stdDiff;

            // test insert
            size_type insertSize = 1e5;
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                vector.insert(0ul, i);
            }
            if (vector.size() != insertSize) {
                throw TestFailed();
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                stdVector.insert(stdVector.begin(), i);
            }
            if (stdVector.size() != insertSize) {
                throw TestFailed();
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("inserting at 0: mydiff %lf, stdDiff %lf\n", mydiff, stdDiff);

            // test insert at half
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                vector.insert(insertSize / 2, i);
            }
            if (vector.size() != insertSize * 2) {
                throw TestFailed();
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                stdVector.insert(stdVector.begin() + insertSize / 2, i);
            }
            if (stdVector.size() != insertSize * 2) {
                throw TestFailed();
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("inserting at insertSize / 2: mydiff %lf, stdDiff %lf\n", mydiff, stdDiff);

            // test remove at half
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                vector.remove(insertSize / 2);
            }
            if (vector.size() != insertSize) {
                throw TestFailed();
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < insertSize; ++i) {
                stdVector.erase(stdVector.begin() + insertSize / 2);
            }
            if (stdVector.size() != insertSize) {
                throw TestFailed();
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("removing at insertSize / 2: mydiff %lf, stdDiff %lf\n", mydiff, stdDiff);
        }

    };
}

#endif //ANBASE_TESTVECTOR_H

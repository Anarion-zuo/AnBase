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
            begin.setClockType(CLOCK_THREAD_CPUTIME_ID);
            end.setClockType(CLOCK_THREAD_CPUTIME_ID);
            double mydiff, stdDiff;

            // test push_back
            size_type pushSize = 1000000ul;
            begin.setCurrent();
            for (size_type i = 0; i < pushSize; ++i) {
                vector.push_back(i);
            }
            if (vector.size() != pushSize) {
                throw TestFailed();
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < pushSize; ++i) {
                stdVector.push_back(i);
            }
            if (stdVector.size() != pushSize) {
                throw TestFailed();
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("push_back: my vector %lf, std vector %lf\n", mydiff, stdDiff);
            vector.clear();
            stdVector.clear();

            // test insert
            size_type insertSize = 200000ul;
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
            printf("inserting at 0: my vector %lf, std vector %lf\n", mydiff, stdDiff);

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
            printf("inserting at insertSize / 2: my vector %lf, std vector %lf\n", mydiff, stdDiff);

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
            printf("removing at insertSize / 2: my vector %lf, std vector %lf\n", mydiff, stdDiff);
        }

    };
}

#endif //ANBASE_TESTVECTOR_H

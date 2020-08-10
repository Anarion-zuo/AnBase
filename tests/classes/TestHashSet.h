//
// Created by anarion on 8/10/20.
//

#ifndef ANBASE_TESTHASHSET_H
#define ANBASE_TESTHASHSET_H

#include <test/TestClass.h>
#include <unordered_set>

namespace anarion {
    class TestHashSet : public TestClass {
    protected:
        static size_type getRandom() {
            return rand() % 100000;
        }

        struct StdSStringHash {
        public:
            std::size_t operator()(const SString &obj) const {
                return obj.getHashVal();
            }
        };

        void runner() override {
            Time begin, end;
            HashSet<size_type> hashSet;
            std::unordered_set <size_type> stdSet;

            printf("\nBegins testing with sequential numbers 0-100000\n\n");
            // Time insert with respect to std
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                hashSet.insert(i);
            }
            end.setCurrent();
            double mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                stdSet.insert(i);
            }
            end.setCurrent();
            double stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("inserting: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            // Time find with respect to std
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = hashSet.find(i);
                if (it == hashSet.end_iterator()) {
                    throwTestFailed();
                }
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = stdSet.find(i);
                if (it == stdSet.end()) {
                    throw TestFailed();
                }
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("finding: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            // Time remove with respect to std
            begin.setCurrent();
            for (size_type i = 0; i < 10000; ++i) {
                hashSet.remove(i);
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 10000; ++i) {
                stdSet.erase(i);
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("removing: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            // Time find with respect to std
            begin.setCurrent();
            for (size_type i = 0; i < 10000; ++i) {
                auto it = hashSet.find(i);
                if (it != hashSet.end_iterator()) {
                    throwTestFailed();
                }
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 10000; ++i) {
                auto it = stdSet.find(i);
                if (it != stdSet.end()) {
                    throw TestFailed();
                }
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("finding: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            printf("\nBegins testing with random numbers 0-100000\n\n");
            // test clearing first
            begin.setCurrent();
            hashSet.clear();
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            stdSet.clear();
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("clearing: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            // test insert random
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                hashSet.insert(getRandom());
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                stdSet.insert(getRandom());
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("inserting: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            size_type valSum = 0;
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = hashSet.find(getRandom());
                if (it != hashSet.end_iterator()) {
                    size_type val = *it;
                    valSum += val;
                }
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = stdSet.find(getRandom());
                if (it != stdSet.end()) {
                    size_type val = *it;
                    valSum += val;
                }
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("finding: my duration: %lf, std duration: %lf, valsum: %lu\n", mydiff, stdDiff, valSum);

            // insert string
            HashSet<SString> hashSetString;
            std::unordered_set<SString, StdSStringHash> stdSetString;
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                hashSetString.insert(SString::parseDec(i));
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                stdSetString.insert(SString::parseDec(i));
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("inserting string: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);

            // finding string
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = hashSetString.find(SString::parseDec(i));
                if (it == hashSetString.end_iterator()) {
                    throwTestFailed();
                }
            }
            end.setCurrent();
            mydiff = Time(Time::difference(end, begin)).getDouble();
            begin.setCurrent();
            for (size_type i = 0; i < 100000; ++i) {
                auto it = stdSetString.find(SString::parseDec(i));
                if (it == stdSetString.end()) {
                    throwTestFailed();
                }
            }
            end.setCurrent();
            stdDiff = Time(Time::difference(end, begin)).getDouble();
            printf("finding string: my duration: %lf, std duration: %lf\n", mydiff, stdDiff);
        }
    };
}

#endif //ANBASE_TESTHASHSET_H

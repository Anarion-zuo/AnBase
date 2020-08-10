//
// Created by anarion on 8/10/20.
//

#ifndef ANBASE_BENCHMARKER_H
#define ANBASE_BENCHMARKER_H

#include <time/Time.h>

namespace anarion {
    class TestClass;
    class BenchMarker {
    public:
        explicit BenchMarker(TestClass *testClass) : testClass(testClass) {}

        void runBenchMark();

    protected:
        virtual void testee() = 0;
        virtual const char *testName() = 0;

        Time beginTime, endTime, diffTime;
        TestClass *testClass;
    };
}

#endif //ANBASE_BENCHMARKER_H

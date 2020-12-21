//
// Created by anarion on 8/6/20.
//

#ifndef ANBASE_TESTCLASS_H
#define ANBASE_TESTCLASS_H

#include <time/Time.h>
#include "BenchMarker.h"

namespace anarion {
    class TestClass {
    public:

        struct TestFailed : std::exception {
            const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
        };

        TestClass();

        void run();


        virtual ~TestClass() = default;

    protected:
        virtual void runner() = 0;

        void callRunner();
        static void prePrint();
        void postPrint() const;
        void runTest();

        static void throwTestFailed();

        class RootBenchMarker : public BenchMarker {
            friend class TestClass;
        protected:
            TestClass *testClass;
            void testee() override;
            const char *testName() override { return "RootBenchMarker"; }
        public:
            explicit RootBenchMarker(TestClass *testClass) : BenchMarker(testClass) {}
        } rootBenchMarker;
    };

#define Test(testName, testFunc, ...) \
    namespace anarion {\
        class testName : public TestClass {\
        protected:\
            void runner() override {  \
                printf("Test Name: "#testName"\n");                      \
                {testFunc};\
            } \
            __VA_ARGS__\
        };\
    }

#define TestMemberFunction(funcName, funcArgs, funcReturn, funcBody) \
    funcReturn funcName funcArgs {                      \
        funcBody                                                            \
    }

#define TimeMethod(method) (\
        Time beginMethodTime, endMethodTime;                       \
        beginMethodTime.setCurrent();\
        { method }\
        endMethodTime.setCurrent();                                \
        Time {Time::difference(endMethodTime, beginMethodTime)}.getDouble();\
    )
}

#endif //ANBASE_TESTCLASS_H

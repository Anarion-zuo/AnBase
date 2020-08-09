//
// Created by anarion on 8/6/20.
//

#ifndef ANBASE_TESTCLASS_H
#define ANBASE_TESTCLASS_H

#include <time/Time.h>
#include <io/channel/terminal/TerminalPrintChannel.h>

namespace anarion {
    class TestClass {
    public:

        struct TestFailed : std::exception {
            const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
        };

        TestClass();

        void runTest();

        virtual ~TestClass() = default;

    protected:
        virtual void runner() = 0;

        void callRunner();
        static void prePrint();
        void postPrint() const;

        static void throwTestFailed();

        Time beginTime, endTime;
        double durationSeconds = 0;
    };
}

#endif //ANBASE_TESTCLASS_H

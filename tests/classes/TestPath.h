//
// Created by anarion on 8/20/20.
//

#ifndef ANBASE_TESTPATH_H
#define ANBASE_TESTPATH_H

#include <test/TestClass.h>
#include <io/channel/path/Path.h>

namespace anarion {
    class TestPath : public TestClass {
    public:
    protected:
        void runner() override {
            Path absPath(SString("/home/anarion/testdir/"));
            Path relPath(SString("test1/test2/test3"));
            Path relPathStrong(SString("./test1/test2/test3"));
        }
    };
}

#endif //ANBASE_TESTPATH_H

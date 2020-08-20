//
// Created by anarion on 8/20/20.
//

#ifndef ANBASE_TESTFILECHANNEL_H
#define ANBASE_TESTFILECHANNEL_H

#include <test/TestClass.h>
#include <io/channel/file/FileChannel.h>
#include <io/channel/file/FileSystemNotifier.h>

namespace anarion {
    class TestFileChannel : public TestClass {
    protected:
        void runner() override {
            FileChannel file(SString("/home/anarion/testdir/TestFile.txt"));
            file.in("1234", 4);
            FileSystemNotifier notifier;
            notifier.addFileChannel(file);
            notifier.start();
            while (true) {}
            file.close();
        }
    };
}

#endif //ANBASE_TESTFILECHANNEL_H

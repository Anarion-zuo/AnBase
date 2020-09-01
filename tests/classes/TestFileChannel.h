//
// Created by anarion on 8/20/20.
//

#ifndef ANBASE_TESTFILECHANNEL_H
#define ANBASE_TESTFILECHANNEL_H

#include <test/TestClass.h>
#include <io/channel/file/FileChannel.h>
#include <io/channel/file/FileSystemNotifier.h>
#include <io/channel/file/Directory.h>

namespace anarion {
    class TestFileChannel : public TestClass {
    protected:
        void testReadWrite(FileChannel &file, size_type writeBegin, size_type writeLength) {
            file.set_cursor(writeBegin);
            for (size_type i = 0; i < writeLength; ++i) {
                file.in((const char *)&i, 1);
            }
            file.set_cursor(writeBegin);
            for (size_type i = 0; i < writeLength; ++i) {
                char outChar;
                file.out(&outChar, 1);
                if (outChar != (char)i) {
                    throwTestFailed();
                }
            }
        }

        void testDirectory(Directory &directory, size_type writeBegin, size_type writeLength) {
            while (directory.next()) {
                if (directory.curEnt()->d_type != DT_REG) { continue; }
                FileChannel subFile = directory.direntToFile(O_RDWR);
                subFile.set_cursor(writeBegin);
                char buffer[256];
                size_type len = subFile.out(buffer, 256);
                if (len != writeLength) {
                    throwTestFailed();
                }
                printf("%s\n", buffer);
                subFile.close();
            }
        }

        void runner() override {
            FileChannel file(SString("/home/anarion/testdir/TestFile.txt"));

            // test read/write
            size_type writeLength = 50, writeBegin = 0;
            file.open(0666);
            testReadWrite(file, writeBegin, writeLength);

            // test rename/move
            file.rename(SString("TestFile1.txt"));
            file.sync();
            FileChannel fileRenamed(SString("/home/anarion/testdir/TestFile1.txt"));
            try {
                fileRenamed.open();
            } catch (OpenFdFailed) {
                fileRenamed.close();
                throwTestFailed();
            }
            fileRenamed.close();

            file.close();

            // test directory
            Directory directory(SString("/home/anarion/testdir"));
            directory.open();
            testDirectory(directory, writeBegin, writeLength);

            // test create/remove directory
            Path removingPath = Path::combine(directory.getPath(), Path(SString("removingdir")));
            Directory removingDir(removingPath.getString());
            removingDir.create(0777);
            removingDir.open();
            testDirectory(removingDir, writeBegin, writeLength);
            removingDir.remove();
        }
    };
}

#endif //ANBASE_TESTFILECHANNEL_H

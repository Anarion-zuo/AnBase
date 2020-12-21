//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <io/channel/file/FileChannel.h>

using namespace anarion;

TEST(TestFileChannel, TestOpenClose) {
    FileChannel file(Path(SString()), O_RDONLY);
}
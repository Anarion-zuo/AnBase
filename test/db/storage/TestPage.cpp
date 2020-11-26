//
// Created by anarion on 11/26/20.
//

#include <gtest/gtest.h>
#include <data/db/PageManager.h>

using namespace anarion;
using namespace anarion::db;

TEST(TestPage, TestInit) {
    size_type blockSize = 40960, pageSize = 4096, pageCount = 10, bufferCount = 15;
    FileBlockManager *blockManager = new FileBlockManager(Path(SString("./")), blockSize, 1);
    PageManager pageManager(blockManager, {0, 0}, pageSize, pageCount, bufferCount);
}

TEST(TestPage, TestSimpleRW) {
    size_type blockSize = 40960, pageSize = 4096, pageCount = 10, bufferCount = 15;
    FileBlockManager *blockManager = new FileBlockManager(Path(SString("./")), blockSize, 1);
    PageManager pageManager(blockManager, {0, 0}, pageSize, pageCount, bufferCount);
    pageManager.write(0, 0, "12345", 5);
    char str[255] = {0};
    pageManager.read(0, 0, str, 5);
    ASSERT_TRUE(!strcmp(str, "12345"));
}

TEST(TestPage, TestHugeRW) {
    size_type blockSize = 40960, pageSize = 4096, pageCount = 10, bufferCount = 2;
    FileBlockManager *blockManager = new FileBlockManager(Path(SString("./TestPageDB")), blockSize, 1);
    PageManager pageManager(blockManager, {0, 0}, pageSize, pageCount, bufferCount);

    const size_type stringLength = 4096;
    char character = '9';
    char str[stringLength + 1] = {character}, str2[stringLength + 1] = {character};
    for (size_type index = 0; index < stringLength; ++index) {
        str[index] = character;
        str2[index] = character;
    }
    str[stringLength] = 0;
    str2[stringLength] = 0;
    pageManager.write(0, 0, str, stringLength);
    pageManager.read(0, 0, str, stringLength);
    ASSERT_TRUE(!strcmp(str, str2));
}

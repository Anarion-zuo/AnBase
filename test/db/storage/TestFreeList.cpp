//
// Created by anarion on 11/25/20.
//

#include <gtest/gtest.h>
#include <data/db/storage/FreeList.hpp>

using namespace anarion;
using namespace anarion::db;

TEST(TestFreeList, TestPushPop) {
    size_type listLength = 10;
    FreeList<uint16_t> freeList(listLength);
    ASSERT_EQ(listLength, freeList.size());
    for (size_t index = 0; index < listLength; ++index) {
        ASSERT_FALSE(freeList.isUsed(index));
    }
    for (size_type index = 0; index < listLength; ++index) {
        uint16_t fetchIndex;
        ASSERT_TRUE(freeList.fetch(fetchIndex));
        ASSERT_TRUE(freeList.isUsed(index));
    }
    ASSERT_TRUE(freeList.isEmpty());
    for (size_type index = 0; index < listLength; ++index) {
        ASSERT_TRUE(freeList.isUsed(index));
        ASSERT_TRUE(freeList.putBack(index));
        ASSERT_FALSE(freeList.isUsed(index));
        ASSERT_FALSE(freeList.isEmpty());
    }
}

TEST(TestFreeList, TestExpand) {
    size_type listLength = 5;
    FreeList<uint16_t> freeList(listLength);
    uint16_t fetchIndex;
    freeList.fetch(fetchIndex);
    freeList.fetch(fetchIndex);

    size_type moreLength = 4;
    freeList.append(moreLength);
    for (size_type index = listLength; index < listLength + moreLength; ++index) {
        ASSERT_FALSE(freeList.isUsed(index));
        ASSERT_TRUE(freeList.fetch(fetchIndex));
        ASSERT_EQ(fetchIndex, index);
        ASSERT_TRUE(freeList.isUsed(index));
    }
    for (size_type index = 0; index < listLength + moreLength; ++index) {
        if (freeList.isUsed(index)) {
            ASSERT_TRUE(freeList.putBack(index));
            ASSERT_FALSE(freeList.isUsed(index));
            ASSERT_FALSE(freeList.isEmpty());
        }
    }
}

TEST(TestFreeList, TestShrink) {
    size_type listLength = 20;
    FreeList<uint16_t> freeList(listLength);
    uint16_t fetchIndex;
    for (size_type index = 0; index < listLength; ++index) {
        freeList.fetch(fetchIndex);
    }
    for (size_type index = 0; index < 10;) {
        size_type randIndex = rand() % listLength;
        if (freeList.putBack(randIndex)) {
            ++index;
        }
    }
    bool usedMap[listLength];
    for (size_type index = 0; index < listLength; ++index) {
        usedMap[index] = freeList.isUsed(index);
    }
    size_type lessLength = 12;
    freeList.shrink(lessLength);
    for (size_type index = 0; index < listLength - lessLength; ++index) {
        ASSERT_EQ(usedMap[index], freeList.isUsed(index));
    }
}

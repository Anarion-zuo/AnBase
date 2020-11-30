//
// Created by anarion on 11/26/20.
//

#include <gtest/gtest.h>
#include <data/db/storage/LruList.h>

using namespace anarion;
using namespace anarion::db;

TEST(TestLruList, TestAddFetch) {
    LruList<uint16_t> lruList;
    size_type lruLength = 10;
    for (size_type index = 0; index < lruLength; ++index) {
        lruList.add(index);
    }
    for (size_type index = 0; index < lruLength; ++index) {
        ASSERT_TRUE(lruList.has(index));
    }
}

TEST(TestLruList, TestMark) {
    LruList<uint16_t> lruList;
    size_type lruLength = 10;
    for (size_type index = 0; index < lruLength; ++index) {
        lruList.add(index);
    }
    size_type markTimes = 66;
    for (size_type index = 0; index < markTimes; ++index) {
        uint16_t randIndex = rand() % lruLength;
        if (!lruList.mark(randIndex)) {
            continue;
        }
        ASSERT_EQ(randIndex, lruList.mostRecent());
        uint16_t object = lruList.popLeastRecent();
        ASSERT_FALSE(lruList.has(object));
        if (lruList.size() > 0) {
            ASSERT_TRUE(object != randIndex);
        } else {
            ASSERT_EQ(object, randIndex);
        }
        lruList.add(object);
        ASSERT_TRUE(lruList.has(object));
        ASSERT_EQ(lruLength, lruList.size());
    }
}

TEST(TestLruList, TestRemove) {
    LruList<uint16_t> lruList;
    size_type lruLength = 10;
    for (size_type index = 0; index < lruLength; ++index) {
        lruList.add(index);
    }
    size_type markTimes = 66;
    for (size_type index = 0; index < markTimes; ++index) {
        uint16_t randIndex = rand() % lruLength;
        ASSERT_TRUE(lruList.remove(randIndex));
        ASSERT_FALSE(lruList.has(randIndex));
        lruList.add(randIndex);
        ASSERT_TRUE(lruList.has(randIndex));
    }
}

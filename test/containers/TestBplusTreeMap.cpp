//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <container/Map/BplusTreeMap.hpp>

using namespace anarion;

TEST(TestContainers, TestBplusTreeMap) {
    BplusTreeMap<int, unsigned int, 100> map;
    size_type insertSize = 100;

    for (size_type index = 0; index < insertSize; ++index) {
        map.insert({index, index + 1});
    }

    for (size_type index = 0; index < insertSize; ++index) {
        auto it = map.find(index);
        ASSERT_EQ(it->get_val(), index + 1);
    }
}

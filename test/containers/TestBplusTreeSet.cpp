//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <container/Set/BplusTreeSet.h>

using namespace anarion;

TEST(TestContainers, TestBplusTreeSet) {
    BplusTreeSet<int, 2> treeSet;

    int insertSize = 100;

    // test insert
    for (unsigned long index = 0; index < insertSize; ++index) {
        printf("Tree Begins\n");
        treeSet.insert(index);
        ASSERT_TRUE(treeSet.testChildParentPointers());
        treeSet.print();
    }
    // test find
    for (size_type index = 0; index < insertSize; ++index) {
        ASSERT_TRUE(treeSet.has(index));
    }

    // test iterator
    for (auto it = treeSet.begin_iterator(); it != treeSet.end_iterator(); ++it) {
        printf("%d ", *it);
    }
    printf("\n");

    // test iterator increment operation
    size_type stepCount = 0;
    auto incrementIt = treeSet.begin_iterator();
    for (; stepCount < insertSize; incrementIt += 3, stepCount += 3) {
        ASSERT_EQ(*incrementIt, stepCount);
    }
    incrementIt = treeSet.end_iterator();
    --incrementIt;
    stepCount = *incrementIt;
    // test iterator decrement operation
    for (; stepCount < insertSize;
           incrementIt -= 3u,
                   stepCount -= 3) {
        ASSERT_EQ(*incrementIt, stepCount);
    }
}
//
// Created by anarion on 12/21/20.
//

#include <gtest/gtest.h>
#include <container/numtype/NumListSet.h>
#include <container/List/LinkedList.hpp>

using namespace anarion;

TEST(TestNumLinkedList, TestInit) {
    NumListSet<uint16_t> list(3);
    list.pushBack(1);
    ASSERT_EQ(list.popBack(),
              1);
    list.printElements();
}

TEST(TestNumLinkedList, TestPushPopInRange) {
    size_type length = 10;
    NumListSet<uint16_t> list(length);
    // empty list
    ASSERT_TRUE(list.empty());
    ASSERT_THROW(list.popBack(), NumListSet<uint16_t>::Underflow);
    // nonempty list
    for (size_type number = 1; number < length; ++number) {
        NumListSet<uint16_t> templist {list};
        for (size_type index = 0; index < number; ++index) {
            templist.pushBack(index);
            printf("%u pushed: ", index);
            templist.printElements();
            printf("\n");
        }
        for (size_type index = 0; index < number; ++index) {
            uint16_t popped = templist.popBack();
            ASSERT_EQ(popped, number - index - 1);
            printf("%u popped: ", popped);
            templist.printElements();
            printf("\n");
        }
    }
    // overflow
    ASSERT_THROW(list.pushBack(length), NumListSet<uint16_t>::Overflow);
}

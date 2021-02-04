//
// Created by anarion on 2/4/21.
//

#include <gtest/gtest.h>
#include "concurrent/base/WaitThread.h"

using namespace anarion;

TEST(TestWaitThread, Wait) {
    int number = rand(), store = number;
    auto thread = makeWaitThread([&] {
        ++number;
    });
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQ(number, store);
        thread.startWait();
        ASSERT_EQ(number, store);
        thread.launchWait();
        thread.join();
        ASSERT_EQ(number, 1 + store);
        ++store;
    }
}

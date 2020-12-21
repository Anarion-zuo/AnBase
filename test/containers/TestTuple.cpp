//
// Created by anarion on 9/6/20.
//

#include <gtest/gtest.h>
#include <container/Bind/Tuple.hpp>

using namespace anarion;

TEST(TestTuple, TestInit) {
    // constructors
    auto tuple = make_tuple(1, 2u, "hello tuple");
    auto out = tuple.get();
    ++out;
}

//
// Created by anarion on 12/15/20.
//

#include "function/Function.h"
#include <gtest/gtest.h>
#include <container/SString.h>

using namespace anarion;

static void hello() {
    printf("hello function\n");
};

TEST(TestFunction, TestInit) {
    Function<void()> function(hello);
    function();
}

static void print3Values(int x, int y, int z) {
    printf("x %d y %d z %d\n", x, y, z);
}

TEST(TestFunction, TestValuePass) {
    Function<void(int, int, int)> function(print3Values);
    function(1, 2, 3);
}

struct RValuePassed {
    int *p = nullptr;

    RValuePassed() = default;
    RValuePassed(const RValuePassed &rhs) : p(rhs.p) { printf("move not invoded...\n"); }
    RValuePassed(RValuePassed &&rhs) noexcept : p(rhs.p) {
        rhs.p = nullptr;
        printf("move invoked...\n");
    }
};

static void takeLValue(int *p, RValuePassed rv) {
    ASSERT_EQ(p, rv.p);
}

static void takeRValue(int *p, RValuePassed &&rv) {
    ASSERT_EQ(rv.p, p);
    RValuePassed rrv(forward<RValuePassed>(rv));
    ASSERT_EQ(p, rrv.p);
    ASSERT_EQ(nullptr, rv.p);
}

static void take2RValue(RValuePassed lv, RValuePassed &&rv) {
    ASSERT_EQ(rv.p, lv.p);
    RValuePassed rrv(forward<RValuePassed>(rv));
    ASSERT_EQ(lv.p, rrv.p);
    ASSERT_EQ(nullptr, rv.p);
}

TEST(TestFunction, TestRValuePass) {
    int num = 4;
    RValuePassed a, b, c;
    a.p = &num;
    b.p = &num;
    c.p = &num;

    printf("passing 1 rval\n");
    Function<void(int*, RValuePassed&&)> rpass(takeRValue);
    rpass(&num, move(a));
    ASSERT_EQ(nullptr, a.p);

    printf("\n");
    printf("passing 1 lval\n");

    Function<void(int *, RValuePassed)> lpass(takeLValue);
    lpass(&num, b);
    ASSERT_NE(b.p, nullptr);

    printf("\n");
    printf("passing 1 lval 1 rval\n");

    Function<void(RValuePassed, RValuePassed&&)> pass2(take2RValue);
    pass2(b, move(c));
    ASSERT_EQ(nullptr, c.p);
    ASSERT_NE(b.p, nullptr);
}

static void lv2rv2(int *p, RValuePassed lv1, RValuePassed lv2, RValuePassed &&rv1, RValuePassed &&rv2) {
    ASSERT_EQ(p, rv1.p);
    ASSERT_EQ(p, rv2.p);
    RValuePassed rrv1(forward<RValuePassed>(rv1)), rrv2(forward<RValuePassed>(rv2));
    ASSERT_EQ(rrv1.p, p);
    ASSERT_EQ(rrv2.p, p);
    ASSERT_EQ(lv1.p, p);
    ASSERT_EQ(lv2.p, p);
    ASSERT_EQ(nullptr, rv1.p);
    ASSERT_EQ(nullptr, rv2.p);
}

TEST(TestFunction, TestMultiPass) {
    int num = 4;
    RValuePassed a, b, c, d;
    a.p = &num;
    b.p = &num;
    c.p = &num;
    d.p = &num;

    printf("passing 2 lval 2 rval\n");
    Function<void(int *, RValuePassed, RValuePassed, RValuePassed &&, RValuePassed &&)> function(lv2rv2);
    function(&num, a, b, move(c), move(d));
    ASSERT_EQ(nullptr, c.p);
    ASSERT_EQ(nullptr, d.p);
    ASSERT_EQ(&num, a.p);
    ASSERT_EQ(&num, b.p);
}

static void lv1refrv1(int *p, RValuePassed &lv, RValuePassed &&rv) {
    ASSERT_EQ(lv.p, p);
    ASSERT_EQ(rv.p, p);
    RValuePassed rrv(forward<RValuePassed>(rv));
    ASSERT_EQ(rrv.p, p);
    ASSERT_EQ(rv.p, nullptr);
}

TEST(TestFunction, TestRefPass) {
    int num = 4;
    RValuePassed a, b, c, d;
    a.p = &num;
    b.p = &num;
    printf("passing 1 rval 1 lval ref\n");
    Function<void(int *p, RValuePassed &, RValuePassed &&)> function(lv1refrv1);
    function(&num, a, move(b));
}

TEST(TestFunction, TestLambdaInit) {
//    Function<void()> voidvoid { [](){
//        printf("hello lambda\n");
//    } };
    auto fp = [](){
        printf("hello lambda\n");
    };
    Function<void()> voidvoid { fp };
    voidvoid();
}

TEST(TestFunction, TestLambdaCapture) {
    int number = 999;
    auto fp = [number](int passedNum) {
        printf("captured number %d passed number %d\n", number, passedNum);
    };
    auto lambda = Lambda<decltype(fp), void, int>{ fp };
    lambda(444);
}

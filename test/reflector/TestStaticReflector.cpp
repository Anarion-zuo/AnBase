//
// Created by anarion on 11/17/20.
//

#include <gtest/gtest.h>
#include "reflection/static_reflector.hpp"

using namespace anarion;

class TestReflectClass {
public:
    int num;

    virtual void hello() {
        printf("Hello TestReflectClass\n");
    }

    int getNum() {
        return num;
    }

    int numAdd(int inc) {
        return num + inc;
    }
};

class TestReflectChild : public TestReflectClass {
    void hello() override {
        printf("Hello TestReflectChild\n");
    }
};

TEST(TestStaticReflector, TestCompile) {
//    auto offset = offsetofClass(TestReflectClass, num);
    TestReflectClass object;
    StaticReflector<TestReflectClass>::invoke(&TestReflectClass::hello, object);
    ASSERT_EQ(object.num, StaticReflector<TestReflectClass>::invoke(&TestReflectClass::getNum, object));
    ASSERT_EQ(object.num, StaticReflector<TestReflectClass>::attr(&TestReflectClass::num, object));
    ASSERT_EQ(object.num + 3, StaticReflector<TestReflectClass>::invoke(&TestReflectClass::numAdd, object, 3));
}

TEST(TestStaticReflector, TestOffsetOf) {
    auto offsetNum = offsetof(TestReflectClass, num);
    ASSERT_EQ(offsetNum, StaticReflector<TestReflectClass>::memberOffset(&TestReflectClass::num));

    TestReflectClass object;
    object.num = rand();
    auto numMemberPointer = StaticReflector<TestReflectClass>::getAttrPointer<int>(object, offsetNum);
    ASSERT_EQ(object.num, *numMemberPointer);
    *numMemberPointer = rand();
    ASSERT_EQ(object.num, *numMemberPointer);
    printf("object num %d\n", object.num);

    printf("Offset of method hello in parent %u, in child %u\n",
           StaticReflector<TestReflectClass>::memberOffset(&TestReflectClass::num),
           StaticReflector<TestReflectClass>::memberOffset(&TestReflectChild::num));

}

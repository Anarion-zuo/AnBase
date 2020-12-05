//
// Created by anarion on 12/5/20.
//

#include <gtest/gtest.h>
#include <container/SString.h>
#include <cmath>

using namespace anarion;

TEST(TestString, TestFloatConvert) {
    double number = 7.77;
    size_type integer = number;
}

TEST(TestString, TestDecBitCount) {
    for (size_type index = 0; index < 10; ++index) {
        size_type num = pow(10, index);
        num += rand() % num;
        ASSERT_EQ(index + 1,SString::decIntegerBitCount(num));
    }
}

TEST(TestString, TestParseFloat) {
    SString converted = SString::parseDec(1.222, 2);
    printf("%s\n", converted.cstr());
    converted = SString::parseDec(1/3.0);
    printf("%s\n", converted.cstr());
    converted = SString::parseDec(1/3.0, 4);
    printf("%s\n", converted.cstr());
    for (double num = 0; num < 100; num += 0.3) {
        char buf[1024];
        sprintf(buf, "%.15lf", num);
        ASSERT_EQ(0, strcmp(buf, SString::parseDec(num, 15).cstr()));
        printf("%s\n", buf);
    }
}

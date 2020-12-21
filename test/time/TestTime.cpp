//
// Created by anarion on 12/5/20.
//

#include <gtest/gtest.h>
#include <time/Time.h>
#include <time/Date.h>

using namespace anarion;

TEST(TestTime, TestConvert) {
    Time time = Time::now();
    Date date(time);
    printf("%s\n", date.print("%d %b %Y %T %Z").cstr());
}

TEST(TestTime, TestFieldExtract) {
    Time time = Time::now();
    printf("%s\n", SString::parseDec(time.getMsecField(), 2).cstr());
}

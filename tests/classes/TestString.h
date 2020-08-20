//
// Created by anarion on 8/7/20.
//

#ifndef ANBASE_TESTSTRING_H
#define ANBASE_TESTSTRING_H

#include <test/TestClass.h>

namespace anarion {
class TestString : public TestClass {
protected:
    void runner() override {
        SString string("1234567890");
        checkString(string);

        string.insert(1, 'p');
        checkString(string);

        string.insert(7, 'p');
        checkString(string);

        string.append("123456", 6);
        checkString(string);
        if (string.length() != 18) { throwTestFailed(); }

    }

    void checkString(const SString &string);
};
}

#endif //ANBASE_TESTSTRING_H

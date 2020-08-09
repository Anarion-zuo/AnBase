//
// Created by anarion on 8/7/20.
//

#ifndef ANBASE_TESTSTRING_H
#define ANBASE_TESTSTRING_H

#include <test/TestClass.h>

namespace anarion {
class TestString : public TestClass {
protected:
    void runner() override;

    void checkString(const SString &string);
};
}

#endif //ANBASE_TESTSTRING_H

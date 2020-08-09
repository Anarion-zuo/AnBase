//
// Created by anarion on 8/7/20.
//

#ifndef ANBASE_TESTVECTOR_H
#define ANBASE_TESTVECTOR_H

#include <test/TestClass.h>

namespace anarion {
class TestVector : public TestClass {
protected:
    void runner() override;
};
}

#endif //ANBASE_TESTVECTOR_H

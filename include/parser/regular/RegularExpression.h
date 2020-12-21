//
// Created by anarion on 11/18/20.
//

#ifndef ANBASE_REGULAREXPRESSION_H
#define ANBASE_REGULAREXPRESSION_H

#include <container/SString.h>

namespace anarion {
class RegularExpression {
protected:
    SString expression;

public:
    explicit RegularExpression(const SString &expression);
    explicit RegularExpression(SString &&expression);
    RegularExpression(const RegularExpression &) = default;
    RegularExpression(RegularExpression &&) noexcept = default;

    void compile();
};
}

#endif //ANBASE_REGULAREXPRESSION_H

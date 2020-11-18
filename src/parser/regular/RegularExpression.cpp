//
// Created by anarion on 11/18/20.
//

#include "parser/regular/RegularExpression.h"

anarion::RegularExpression::RegularExpression(const anarion::SString &expression)
    : expression(expression) {

}

anarion::RegularExpression::RegularExpression(anarion::SString &&expression)
    : expression(forward<SString>(expression)) {

}

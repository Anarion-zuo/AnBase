//
// Created by anarion on 4/6/20.
//

#include "parser/ListParser.h"

using namespace anarion;

static size_type indexOfCStr(const char *str, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (str[i] == c) {
            return i;
        }
    }
    return length;
}

anarion::LinkedList<anarion::SString> anarion::ListParser::parse(const anarion::SString &expression) {
    LinkedList<SString> ret;
    const char *cstr = expression.cstr();
    size_type length = expression.size();

    size_type leftIndex = 0;
    size_type segLen;
    size_type unParsedLen = length;

    while (unParsedLen) {
        segLen = indexOfCStr(cstr + leftIndex, unParsedLen, separator);
        unParsedLen -= segLen + 1;
        ret.push_back(SString(const_cast<char*>(cstr + leftIndex), segLen));
    }
    return move(ret);
}

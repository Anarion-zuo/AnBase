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

static size_type skipChar(const char *str, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (str[i] != c) {
            return i;
        }
    }
    return length;
}

anarion::LinkedList<anarion::SString> anarion::ListParser::parse(const anarion::SString &expression) {
    const char *cstr = expression.cstr();
    size_type length = expression.size();
    return parse(cstr, length);
}

LinkedList<SString> ListParser::parse(const char *cstr, size_type length) {
    size_type leftIndex = 0;
    size_type segLen;
    size_type unParsedLen = length;
    LinkedList<SString> ret;
    size_type headSpacesNum = skipChar(cstr + leftIndex, unParsedLen, separator);
    unParsedLen -= headSpacesNum;
    leftIndex += headSpacesNum;

    while (unParsedLen) {
        segLen = indexOfCStr(cstr + leftIndex, unParsedLen, separator);
        unParsedLen -= segLen;
        ret.push_back(SString(const_cast<char*>(cstr + leftIndex), segLen));
        leftIndex += segLen;

        size_type tailSpacesNum = skipChar(cstr + leftIndex, unParsedLen, separator);
        leftIndex += tailSpacesNum;
        unParsedLen -= tailSpacesNum;
    }
    return move(ret);
}

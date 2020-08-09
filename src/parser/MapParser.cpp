//
// Created by 左钰 on 2020/4/5.
//

#include "parser/MapParser.h"

using namespace anarion;

anarion::MapParser::MapParser(char equal, char separator) : equal(equal), separator(separator) {}

/*
 * The parsing process.
 *      1) Initialize variables
 *          1) Find the first valid character, skipping spaces
 *          2) begin to roll
 *      2) Parse each fragments
 *          The process is acted upon each part of the expression separated by the separator.
 *          max=3;timeout=90 --> max=3 and timeout=90
 *          The key and value of each part should be extracted from the separated expressions and forms a HashMap.
 *          max=3 --> { max, 3 }  timeout=90 --> { timeout, 90 }
 *          Being aware of potential spaces
 *          username =anarion  should be equivalent to username=anarion
 */

static char * getRidOfEndSpaces(const char *str, size_type &length) {
    // compute array length

    // head
    size_type headIndex = length;
    for (size_type i = 0; i < length & str[i] != 0; ++i) {
        if (str[i] != ' ') {
            headIndex = i;
            break;
        }
    }

    // tail
    size_type tailIndex = 0;
    for (size_type i = length - 1; i < 0ul - 1ul; --i) {
        if (str[i] != ' ') {
            tailIndex = i;
            break;
        }
    }

    length = tailIndex - headIndex + 1;
    if (length == 0) {
        return nullptr;
    }

    // copy memory
    char *retp = static_cast<char *>(operator new(length));
    memcpy(retp, str + headIndex, length);
    return retp;
}

static size_type indexOfCStr(const char *str, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (str[i] == c) {
            return i;
        }
    }
    return length;
}

anarion::HashMap<anarion::SString, anarion::SString>
anarion::MapParser::parse(const anarion::SString &inputExpression) {
    HashMap<SString, SString> ret;
    // initialize variables
    const char *cstr = inputExpression.cstr();
    size_type unParsedLength = inputExpression.length();
    size_type leftIndex = 0;
    // not indices, but offset with respect to present leftIndex
    size_type equalOffset;
    size_type separatorOffset;

    // iterate for each line
    while (unParsedLength <= inputExpression.length()) {
        equalOffset = indexOfCStr(cstr + leftIndex, unParsedLength, equal);
        separatorOffset = indexOfCStr(cstr + leftIndex, unParsedLength, separator);
        unParsedLength -= equalOffset + 1;
        unParsedLength -= separatorOffset - equalOffset;

        size_type keyLen = equalOffset;
        size_type valLen = separatorOffset - equalOffset - 1;
//        char *key = static_cast<char *>(operator new(keyLen));
//        char *val = static_cast<char *>(operator new(valLen));
//        memcpy(key, cstr + leftIndex, keyLen);
//        memcpy(val, cstr + leftIndex + equalOffset + 1, valLen);

        size_type oldKeyLen = keyLen;
        size_type oldValLen = valLen;
        char *cleanKey = getRidOfEndSpaces(cstr + leftIndex, keyLen);
        char *cleanVal = getRidOfEndSpaces(cstr + leftIndex + equalOffset + 1, valLen);
//        operator delete (key, oldKeyLen);
//        operator delete (val, oldValLen);

        ret.put(SString::move(cleanKey, keyLen), SString::move(cleanVal, valLen));

        leftIndex += separatorOffset + 1;
    }
    auto it = ret.find(SString("timeout"));
    return move(ret);
}

HashMap<SString, SString> MapParser::parse(const char *cstr, size_type length) {
    HashMap<SString, SString> ret;
    // initialize variables
    size_type unParsedLength = length;
    size_type leftIndex = 0;
    // not indices, but offset with respect to present leftIndex
    size_type equalOffset;
    size_type separatorOffset;

    // iterate for each line
    while (unParsedLength <= length) {
        equalOffset = indexOfCStr(cstr + leftIndex, unParsedLength, equal);
        separatorOffset = indexOfCStr(cstr + leftIndex, unParsedLength, separator);
        unParsedLength -= equalOffset + 1;
        unParsedLength -= separatorOffset - equalOffset;

        size_type keyLen = equalOffset;
        size_type valLen = separatorOffset - equalOffset - 1;
//        char *key = static_cast<char *>(operator new(keyLen));
//        char *val = static_cast<char *>(operator new(valLen));
//        memcpy(key, cstr + leftIndex, keyLen);
//        memcpy(val, cstr + leftIndex + equalOffset + 1, valLen);

        size_type oldKeyLen = keyLen;
        size_type oldValLen = valLen;
        char *cleanKey = getRidOfEndSpaces(cstr + leftIndex, keyLen);
        char *cleanVal = getRidOfEndSpaces(cstr + leftIndex + equalOffset + 1, valLen);
//        operator delete (key, oldKeyLen);
//        operator delete (val, oldValLen);

        ret.put(SString::move(cleanKey, keyLen), SString::move(cleanVal, valLen));

        leftIndex += separatorOffset + 1;
    }
    auto it = ret.find(SString("timeout"));
    return move(ret);
}

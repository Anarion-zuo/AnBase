//
// Created by anarion on 4/21/20.
//

#include <parser/json/JsonArray.h>
#include <parser/json/JsonBoolean.h>
#include <parser/json/JsonMap.h>
#include <parser/json/JsonNumber.h>
#include <parser/json/JsonString.h>

#include <exception>

using namespace anarion;

static size_type indexOf(const char *str, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (str[i] == c) {
            return i;
        }
    }
    return length;
}

static size_type lastIndexOf(const char *str, size_type length, char c) {
    for (size_type i = length - 1; i < length; --i) {
        if (str[i] == c) {
            return i;
        }
    }
    return length;
}

static size_type findLeftBracketOffset(const char *str, size_type length) {
    return indexOf(str, length, '{');
}

static size_type findRightBracketOffset(const char *str, size_type length) {
    return indexOf(str, length, '}');
}

static size_type findCommaOffset(const char *str, size_type length) {
    return indexOf(str, length, ',');
}

static size_type skipToValidChar(const char *expr, size_type length) {
    for (size_type i = 0; i < length; ++i) {
        if (expr[i] == ' ') {
            continue;
        }
        if (expr[i] == '\n') {
            continue;
        }
        if (expr[i] == '\r') {
            continue;
        }
        return i;
    }
    return length;
}

static bool isStringTrue(const char *str) {
    const char *true_c = "true";
    for (size_type i = 0; i < 4; ++i) {
        if (str[i] != true_c[i]) {
            return false;
        }
    }
    return true;
}

static bool isStringFalse(const char *str) {
    const char *false_c = "false";
    for (size_type i = 0; i < 5; ++i) {
        if (str[i] != false_c[i]) {
            return false;
        }
    }
    return true;
}

static bool isStringNull(const char *str) {
    const char *null_c = "null";
    for (size_type i = 0; i < 4; ++i) {
        if (str[i] != null_c[i]) {
            return false;
        }
    }
    return true;
}

static bool isNotInformativeChar(char c) {
    return
        c == ' ' ||
        c == '\n' ||
        c == '\r' ||
        c == ',' ;
}

static bool isNumberChar(char c) {
    return c >= '0' && c <= '9';
}

static bool isNumberString(const char *str, size_type length) {
    for (size_type i = 0; i < length; ++i) {
        if (isNumberChar(str[i]) == false) {
            if (i == 0 && *str == '-') {
                continue;
            }
            return false;
        }
    }
    return true;
}

struct JsonSyntaxError : public std::exception {
    const char *what() const noexcept override {
        return "Got JSON expression with wrong syntax.";
    }
};

static size_type findClosingRight(const char *expression, size_type length, char leftChar) {
    char rightChar;
    if (leftChar == '{') {
        rightChar = '}';
    } else if (leftChar == '[') {
        rightChar = ']';
    } else if (leftChar == '(') {
        rightChar = ')';
    } else if (leftChar == '"') {
        rightChar = '"';
    } else {
        throw JsonSyntaxError();
    }
    size_type count = 0;
    bool foundFirst = false;
    for (size_type i = 0; i < length; ++i) {
        if (expression[i] == leftChar) {
            ++count;
            foundFirst = true;
        }
        if (expression[i] == rightChar) {
            if (count == 0) {
                throw JsonSyntaxError();
            }
            --count;
        }
        if (count == 0 && foundFirst) {
            return i;
        }
    }
    return length;
}

JsonMap *parseJsonMap(const char *expression, size_type length);
JsonArray *parseJsonArray(const char *expression, size_type length) {
    if (expression == nullptr || length == 0) { return nullptr; }

    JsonArray *jarray = new JsonArray;
    size_type lineLeftOffset = 0, unParsedLength = length;
    while (true) {
        size_type leftBracketOffset = indexOf(expression + lineLeftOffset, unParsedLength, '{');
        if (leftBracketOffset == unParsedLength) { break; }
        lineLeftOffset += leftBracketOffset;
        unParsedLength -= leftBracketOffset;
        size_type rightBracketOffset = findClosingRight(expression + leftBracketOffset, unParsedLength, '{');
        if (rightBracketOffset == unParsedLength) {
            throw JsonSyntaxError();
        }
        ++lineLeftOffset;
        --unParsedLength;
        JsonMap *aMap = parseJsonMap(expression + lineLeftOffset, rightBracketOffset - 1);
        jarray->push_back(aMap);
        lineLeftOffset += rightBracketOffset;
        unParsedLength -= rightBracketOffset;
        size_type commaOffset = indexOf(expression + lineLeftOffset, unParsedLength, ',');
        if (commaOffset == unParsedLength) {
            break;
        }
        lineLeftOffset += 1 + commaOffset;
        unParsedLength -= 1 + commaOffset;
    }
    return jarray;
}

JsonMap *parseJsonMap(const char *expression, size_type length) {

    if (expression == nullptr || length == 0) {
        return nullptr;
    }

    JsonMap *jmap = new JsonMap();

    size_type unParsedLength = length;
    size_type lineLeftOffset = indexOf(expression, unParsedLength, '"');
    if (lineLeftOffset == unParsedLength) {
        throw JsonSyntaxError();
    }
    ++lineLeftOffset;
    unParsedLength -= lineLeftOffset;
    while (true) {
        // check length
        if (unParsedLength == 0) {
            break;
        }
        // key
        size_type keyRightOffset = indexOf(expression + lineLeftOffset, unParsedLength, '"');
        if (keyRightOffset == unParsedLength) {
            throw JsonSyntaxError();
        }
        size_type keyLen = keyRightOffset;
        char *key_c = static_cast<char *>(operator new(keyLen));
        memcpy(key_c, expression + lineLeftOffset, keyLen);
        unParsedLength -= keyLen + 1;
        lineLeftOffset += keyLen + 1;

        // mid
        size_type sepOffset = indexOf(expression + lineLeftOffset, unParsedLength, ':');
        if (sepOffset == unParsedLength) {
            throw JsonSyntaxError();
        }
        lineLeftOffset += sepOffset + 1;
        unParsedLength -= sepOffset + 1;

        // val
        size_type spaceLength = skipToValidChar(expression + lineLeftOffset, unParsedLength);
        if (spaceLength == unParsedLength) {
            throw JsonSyntaxError();
        }
        lineLeftOffset += spaceLength;
        unParsedLength -= spaceLength;
        if (expression[lineLeftOffset] == '"') {
            // string
            ++lineLeftOffset;
            --unParsedLength;
            size_type valLen = findClosingRight(expression + lineLeftOffset, unParsedLength, '"');
            char *val_c = static_cast<char *>(operator new(valLen));
            memcpy(val_c, expression + lineLeftOffset, valLen);
            jmap->put(SString::move(key_c, keyLen), new JsonString(SString::move(val_c, valLen)));

            lineLeftOffset += valLen + 1;
            unParsedLength -= valLen + 1;
        } else if (expression[lineLeftOffset] == '{') {
            // map
            size_type valLen = findClosingRight(expression + lineLeftOffset, unParsedLength, '{');
            if (valLen == unParsedLength) {
                throw JsonSyntaxError();
            }
            JsonMap *valMap = parseJsonMap(expression + lineLeftOffset + 1, valLen - 1);
            jmap->put(SString::move(key_c, keyLen), valMap);

            lineLeftOffset += valLen + 1;
            lineLeftOffset -= valLen + 1;
        } else if (expression[lineLeftOffset] == '[') {
            // array
            size_type valLen = findClosingRight(expression + lineLeftOffset, unParsedLength, '[');
            JsonArray *valArray = parseJsonArray(expression + lineLeftOffset + 1, valLen - 1);
            jmap->put(SString::move(key_c, keyLen), valArray);

            lineLeftOffset += valLen + 1;
            lineLeftOffset -= valLen + 1;
        } else if (expression[lineLeftOffset] == 't') {
            // true
            bool flag = isStringTrue(expression + lineLeftOffset);
            if (flag) {
                lineLeftOffset += 4;
                unParsedLength -= 4;
                if (!isNotInformativeChar(expression[lineLeftOffset])) {
                    throw JsonSyntaxError();
                }
            } else {
                throw JsonSyntaxError();
            }
            jmap->put(SString::move(key_c, keyLen), new JsonBoolean(true));
        } else if (expression[lineLeftOffset] == 'f') {
            // false
            bool flag = isStringFalse(expression + lineLeftOffset);
            if (flag) {
                lineLeftOffset += 5;
                unParsedLength -= 5;
                if (!isNotInformativeChar(expression[lineLeftOffset])) {
                    throw JsonSyntaxError();
                }
            } else {
                throw JsonSyntaxError();
            }
            jmap->put(SString::move(key_c, keyLen), new JsonBoolean(false));
        } else if (expression[lineLeftOffset] == 'n') {
            // null
            bool flag = isStringNull(expression + lineLeftOffset);
            if (flag) {
                lineLeftOffset += 4;
                unParsedLength -= 4;
                if (!isNotInformativeChar(expression[lineLeftOffset])) {
                    throw JsonSyntaxError();
                }
            } else {
                throw JsonSyntaxError();
            }
            jmap->put(SString::move(key_c, keyLen), nullptr);
        } else if (expression[lineLeftOffset] == ',') {
            // null
            jmap->put(SString::move(key_c, keyLen), nullptr);;
        } else {
            // chech if it is a number
            size_type commaOffset = indexOf(expression + lineLeftOffset, unParsedLength, ',');
            if (commaOffset == unParsedLength) {
                commaOffset = indexOf(expression + lineLeftOffset, unParsedLength, '}');
            }
            bool flag = isNumberString(expression + lineLeftOffset, commaOffset);
            if (!flag) {
                throw JsonSyntaxError();
            }
            char *number_str = static_cast<char *>(operator new(commaOffset + 1));
            memcpy(number_str, expression + lineLeftOffset, commaOffset);
            long number = atol(number_str);
            operator delete (number_str, commaOffset + 1);
            jmap->put(SString::move(key_c, keyLen), new JsonNumber(number));
            lineLeftOffset += commaOffset;
            unParsedLength -= commaOffset;
        }

        // end
        size_type commaOffset = indexOf(expression + lineLeftOffset, unParsedLength, ',');
        if (commaOffset == unParsedLength) { break; }
        lineLeftOffset += commaOffset + 1;
        unParsedLength -= commaOffset + 1;
        size_type nextQuoteOffset = indexOf(expression + lineLeftOffset, unParsedLength, '"');
        if (nextQuoteOffset == unParsedLength) { break; }
        lineLeftOffset += nextQuoteOffset + 1;
        unParsedLength -= nextQuoteOffset + 1;
    }
    return jmap;
}

namespace anarion {
    JsonMap *parseJson(const char *expression, size_type length) {
        size_type leftBracketOffset = indexOf(expression, length, '{');
        if (leftBracketOffset == length) {
            throw JsonSyntaxError();
        }
        size_type unParsedLength = length - leftBracketOffset - 1;
        size_type rightBracketOffset = findClosingRight(expression, length, '{');
        if (rightBracketOffset == length) {
            throw JsonSyntaxError();
        }
        return parseJsonMap(expression + leftBracketOffset + 1, rightBracketOffset - leftBracketOffset - 1);
    }
}

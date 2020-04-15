//
// Created by anarion on 4/6/20.
//

#include <parser/MapParser.h>
#include "parser/xml/XmlNode.h"

using namespace anarion;

char anarion::XmlNode::c_lt = '<', anarion::XmlNode::c_gt = '>', anarion::XmlNode::c_slash = '/', anarion::XmlNode::c_quote = '\"', anarion::XmlNode::c_equal = '=';

static size_type indexOf(const char *expr, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (expr[i] == c) {
            return i;
        }
    }
    return length;
}

static bool isInCstr(char c, const char *str) {
    while (*str != 0) {
        if (*str == c) { return true; }
        ++str;
    }
    return false;
}

static const char *skipChars(const char *begin, const char *cs) {
    while (isInCstr(*begin, cs)) {
        ++begin;
    }
    return begin;
}

static const char *skipChar(const char *begin, char c) {
    while (*begin == c) {
        ++begin;
    }
    return begin;
}

static const char *skipUntil(const char *begin, char c) {
    while (*begin != c) {
        ++ begin;
    }
    return begin;
}

static MapParser tagAttrMapParser ('=', ' ');

/*
 * Takes a string inside the tag as input.
 * The expr pointer should point to the first character after the tag name.
 * The input string should stop before the end of the tag header, either '/' or '>'.
 */
static HashMap<SString, SString> parseAttrs(const char *expr, size_type length) {
    return tagAttrMapParser.parse(expr, length);
}

anarion::XmlNode *anarion::XmlNode::parse(const char *expression, anarion::size_type length) {
    if (expression == nullptr || length == 0) {
        return nullptr;
    }

    bool isCollapsed = false;

    // tag head left and right index
    size_type tagHeadLeftIndex = indexOf(expression, length, c_lt);
    if (tagHeadLeftIndex == length) {
        return nullptr;
    }
    size_type tagHeadRightIndex = indexOf(expression + tagHeadLeftIndex + 1, length - 1 - tagHeadLeftIndex, c_gt) + tagHeadLeftIndex;
    if (tagHeadRightIndex == length) {
        throw TagRightBracketNotFound();
    }

    // ending / index
    size_type endingSlashIndex = tagHeadRightIndex + indexOf(expression + tagHeadRightIndex - 1, length + 1 - tagHeadRightIndex, c_slash);
    if (endingSlashIndex == length) {
        throw EndingCharNotFound();
    }

    // tag name
    const char *tagNameBegin = expression + tagHeadLeftIndex + 1;
    tagNameBegin = skipChar(tagNameBegin, ' ');
    const char *tagNameEnd = skipUntil(tagNameBegin, ' ');
    SString tagName (const_cast<char *>(tagNameBegin), tagNameEnd - tagNameBegin);

    // tag attributes
    const char *attrBegin = skipChar(tagNameEnd, ' ');
    const char *attrEnd;
    if (endingSlashIndex > tagHeadRightIndex) {
        attrEnd = expression + tagHeadRightIndex;
    } else {
        attrEnd = expression + endingSlashIndex;
        isCollapsed = true;
    }
    HashMap<SString, SString> attrs = tagAttrMapParser.parse(attrBegin, attrEnd - attrBegin);

    // inner texts
    if (isCollapsed) {
        // no inner texts
    }

}

const char *TagRightBracketNotFound::what() const noexcept {
    return "No '>' to pair with the previous '<'.";
}

const char *EndingCharNotFound::what() const noexcept {
    return "No '/' found for this tag.";
}

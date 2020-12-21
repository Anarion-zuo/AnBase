//
// Created by anarion on 4/26/20.
//

#include <parser/xml/XmlElement.h>
#include <parser/xml/XmlLeaf.h>

using namespace anarion;

static size_type indexOf(const char *str, size_type length, char c) {
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

static size_type skipCharBack(const char *str, size_type length, char c) {
    for (size_type i = 0; i < length; ++i) {
        if (str[-i] != c) {
            return i;
        }
    }
    return length;
}

static bool parseNodeTag(XmlElement *node, const char *expression, size_type length) {
    /*
     * Parse tag name
     */
    size_type leftOffset = skipChar(expression, length, ' ');
    if (leftOffset == length) {
        throw XmlParseFalied();
    }
    size_type unParsedLength = length - leftOffset;
    size_type nameLen = indexOf(expression + leftOffset, unParsedLength, ' ');
    node->setName(SString(const_cast<char*>(expression + leftOffset), nameLen));

    // move on for attributes
    leftOffset += nameLen;
    unParsedLength -= nameLen;

    /*
     * Flag signifying whether the tag is of style < ... />
     */
    bool hasPair = true;

    /*
     * Parse tag attributes
     */
    while (unParsedLength > 0) {
        /*
         * attribute key
         */
        // move to be at key string left boundary
        size_type leftBlankOffset = skipChar(expression + leftOffset, unParsedLength, ' ');
        leftOffset += leftBlankOffset;
        unParsedLength -= leftBlankOffset;
        if (unParsedLength == 0) { break; }

        /*
         * Possible ending
         */
        if (expression[leftOffset] == '/') {
            if (expression[leftOffset + 1] != '>') { throw XmlParseFalied(); }
            hasPair = false;
            break;
        }

        size_type keyOffset = leftOffset;
        // refCount to =
        size_type keyLen = indexOf(expression + leftOffset, unParsedLength, '=');
        if (keyLen == unParsedLength) { throw XmlParseFalied(); }
        // remove blanks between key string and =
        size_type rightBlankOffset = skipCharBack(expression + leftOffset + keyLen - 1, keyLen, ' ');
        if (rightBlankOffset == keyLen) { throw XmlParseFalied(); }
        leftOffset += keyLen + 1;
        unParsedLength -= keyLen + 1;
        keyLen -= rightBlankOffset;
        SString key {const_cast<char*>(expression + keyOffset), keyLen };

        /*
         * attribute value
         */
        size_type quoteLeftOffset = indexOf(expression + leftOffset, unParsedLength, '"');
        if (quoteLeftOffset == unParsedLength) { throw XmlParseFalied(); }
        leftOffset += quoteLeftOffset + 1;
        unParsedLength -= quoteLeftOffset + 1;
        size_type quoteRightOffset = indexOf(expression + leftOffset, unParsedLength, '"');
        if (quoteLeftOffset == unParsedLength) { throw XmlParseFalied(); }
        SString val {const_cast<char *>(expression + leftOffset), quoteRightOffset };
        leftOffset += quoteRightOffset + 1;
        unParsedLength -= quoteRightOffset + 1;

        node->addAttr(move(key), move(val));

    }
    return hasPair;
}


XmlElement *parseXml(const char *expression, size_type length) {
    if (expression == nullptr || length == 0) {
        return nullptr;
    }

    // initialize parameters
    size_type leftOffset = 0, unParsedLength = length;

    // special stack
    LinkedList<XmlElement*> stack;
    /*
     * The stack is to keep track of the latest parent of the current processing node.
     * The top of the stack is the parent node of the current node.
     * Pop the top if encounter a </ ... >.
     * Push the node into the stack if encounter a non-leaf part.
     * Using pushBack & popBack as push & pop
     */

    XmlElement *rootNode = nullptr;

    // roll
    while (true) {
        if (unParsedLength == 0) { break; }
        /*
         * Find left tag
         */

        // find left bracket
        size_type leftBracketOffset = indexOf(expression + leftOffset, unParsedLength, '<');
        if (leftBracketOffset == unParsedLength) { throw XmlParseFalied(); }
        // handle text node
        if (stack.size()) {
            if (leftBracketOffset != 0) {
                XmlElement *parent = stack.back();
                XmlLeaf *textNode = new XmlLeaf(SString(const_cast<char*>(expression + leftOffset), leftBracketOffset));
                parent->appendChild(textNode);
            }
        }
        leftOffset += leftBracketOffset + 1;
        unParsedLength -= leftBracketOffset + 1;

        // find right bracket
        size_type rightBracketOffset = indexOf(expression + leftOffset, unParsedLength, '>');
        if (rightBracketOffset == unParsedLength) { throw XmlParseFalied(); }

        // check if is a ending tag
        if (expression[leftOffset] == '/') {
            if (stack.empty()) { throw XmlParseFalied(); }
            ++leftOffset;
            --rightBracketOffset;
            --unParsedLength;
            XmlElement *last = stack.pop_back();
            size_type blankOffset = skipCharBack(expression + leftOffset + rightBracketOffset, rightBracketOffset, ' ');
            size_type tagNameLen = rightBracketOffset - blankOffset;
            SString endingName {const_cast<char*>(expression + leftOffset), tagNameLen };
            if (endingName != last->getTagName()) { throw XmlParseFalied(); }
            leftOffset += rightBracketOffset + 1;
            unParsedLength -= rightBracketOffset + 1;
            continue;
        }

        // create object
        XmlElement *newNode = new XmlElement;
        if (stack.empty()) {
            rootNode = newNode;
        }

        // run separate procedure to parse the tag name and attributes
        size_type tagLen = rightBracketOffset;
        bool hasPair = parseNodeTag(newNode, expression + leftOffset, tagLen);
        if (stack.size()) {
            XmlElement *parent = stack.back();
            parent->appendChild(newNode);
            if (hasPair) {
                stack.push_back(newNode);
            }
        } else {
            stack.push_back(newNode);
        }

        // move on
        leftOffset += rightBracketOffset + 1;
        unParsedLength -= rightBracketOffset + 1;
    }

    if (stack.size()) { throw XmlParseFalied(); }
    return rootNode;
}
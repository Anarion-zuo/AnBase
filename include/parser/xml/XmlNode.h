//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_XMLNODE_H
#define MYCPPBASE_XMLNODE_H

#include <container/SString.h>
#include <container/Set/HashSetList.hpp>
#include <container/Map/HashMap.hpp>
#include <container/List/LinkedList.hpp>

namespace anarion {
    class XmlNode {
    protected:
        SString nodeName;

        HashMap<SString, SString> attrs;
        LinkedList<XmlNode *> childs;

        static char c_lt, c_gt, c_slash, c_quote, c_equal;

    public:
        static XmlNode *parse(const char *expression, size_type length);
    };

    struct XmlParseFalied : public std::exception {};

    struct TagRightBracketNotFound : public XmlParseFalied {
        const char *what() const noexcept override;
    };

    struct EndingCharNotFound : public XmlParseFalied {
        const char *what() const noexcept override;
    };
}

#endif //MYCPPBASE_XMLNODE_H

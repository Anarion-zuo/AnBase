//
// Created by anarion on 4/6/20.
//

#ifndef MYCPPBASE_XMLELEMENT_H
#define MYCPPBASE_XMLELEMENT_H

#include <container/Set/HashSetList.hpp>
#include <container/List/LinkedList.hpp>
#include "XmlNode.h"

namespace anarion {
    class XmlElement : virtual public XmlNode {
    protected:
        SString nodeName;

        HashMap<SString, SString> attrs;
        LinkedList<XmlNode *> childs;

    public:
        static XmlElement *parse(const char *expression, size_type length);

        void setName(SString name) { nodeName = move(name); }

        void addAttr(SString key, SString val) { attrs.put(move(key), move(val)); }

        void appendChild(XmlNode * node) { childs.push_back(node); }

        SString &getTagName() { return nodeName; }
    };

    struct XmlParseFalied : public std::exception {};

}

#endif //MYCPPBASE_XMLELEMENT_H

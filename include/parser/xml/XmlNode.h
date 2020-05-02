//
// Created by anarion on 4/26/20.
//

#ifndef MYCPPBASE_XMLNODE_H
#define MYCPPBASE_XMLNODE_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>

namespace anarion {
    class XmlNode {
    public:

        virtual ~XmlNode() = default;

        virtual XmlNode *getChildByTagName(const SString &name) { return nullptr; }
        virtual XmlNode *getChildById(const SString &id) { return nullptr; }
        virtual size_type getChildCount() const { return 0; }

        virtual SString *getAttrVal(const SString &key) { return nullptr; }
        virtual size_type getAttrCount() const { return 0; }
    };
}

#endif //MYCPPBASE_XMLNODE_H

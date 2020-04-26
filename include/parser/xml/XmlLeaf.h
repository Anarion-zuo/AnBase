//
// Created by anarion on 4/26/20.
//

#ifndef MYCPPBASE_XMLLEAF_H
#define MYCPPBASE_XMLLEAF_H

#include "XmlNode.h"

namespace anarion {
    class XmlLeaf : virtual public XmlNode {
    protected:
        SString content;
    public:
        explicit XmlLeaf(SString &&content) : content(forward<SString>(content)) {}
    };
}

#endif //MYCPPBASE_XMLLEAF_H

//
// Created by anarion on 5/3/20.
//

#include "bean/Bean.h"

void *anarion::Bean::getAttrByName(const anarion::SString &name) {
    auto it = attrMap.find(name);
    if (it == attrMap.end_iterator()) {
        return nullptr;
    }
    return it->get_val();
}

//
// Created by anarion on 4/21/20.
//

#include "parser/json/JsonMap.h"

anarion::StringBuilder anarion::JsonMap::getStringBuilder() {
    StringBuilder ret;
    ret.cappend("{");
    for (auto it = begin_iterator(); it != end_iterator(); ++it) {
        ret.appendBuilder(makeKey(it->get_key()));
        ret.cappend(":");
        ret.appendBuilder(it->get_val()->getStringBuilder());
        ret.cappend(",");
    }
    ret.cappend("}");
    return move(ret);
}

anarion::StringBuilder anarion::JsonMap::makeKey(const anarion::SString &key) {
    StringBuilder ret;
    ret.cappend("\"");
    ret.cappend(key.getArr(), key.length());
    ret.cappend("\"");
    return move(ret);
}

anarion::JsonMap::~JsonMap() {
    for (auto it = begin_iterator(); it != end_iterator(); ++it) {
        delete it->get_val();
    }
}

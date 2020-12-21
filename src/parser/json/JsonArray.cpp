//
// Created by anarion on 4/21/20.
//

#include "parser/json/JsonArray.h"

anarion::StringBuilder anarion::JsonArray::getStringBuilder() {
    StringBuilder ret;
    ret.cappend("[");
    for (auto it = begin_iterator(); it != end_iterator(); ++it) {
        ret.appendBuilder((**it).getStringBuilder());
        ret.cappend(",");
    }
    ret.cappend("]");
}

anarion::JsonArray::~JsonArray() {
    list_node *node = head.next;
    while (node != &head) {
        delete node->obj;
        node = node->next;
    }
}

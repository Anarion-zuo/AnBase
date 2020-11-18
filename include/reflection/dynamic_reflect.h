//
// Created by anarion on 2020/2/20.
//

#ifndef MYCPPBASE_DYNAMIC_REFLECT_H
#define MYCPPBASE_DYNAMIC_REFLECT_H

#include <container/Map/HashMap.hpp>

namespace anarion {

template <typename T> struct true_t {};
template <typename T> struct false_t {};


template <typename T> struct reflector_trait {


    static void init() {

    }
};

template <typename Class, typename Attr>
unsigned int offsetofClass(typename Class::Attr *attrPointer) {
    char space[sizeof(Class)];
    Class &objRef = *static_cast<Class*>(space);
    return static_cast<char *>(&objRef.*attrPointer) - space;
}

class Reflected {

};
}
#endif //MYCPPBASE_DYNAMIC_REFLECT_H

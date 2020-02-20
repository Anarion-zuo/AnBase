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
}
#endif //MYCPPBASE_DYNAMIC_REFLECT_H

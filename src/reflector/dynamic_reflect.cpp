//
// Created by anarion on 2020/2/20.
//

#include "reflection/dynamic_reflect.h"

anarion::GlobalTypeInfo *anarion::GlobalTypeInfo::singleton;

anarion::hash_type anarion::GlobalTypeInfo::allocateId() {
    hash_type ret = lastId;
    ++lastId;
    return ret;
}

anarion::GlobalTypeInfo &anarion::GlobalTypeInfo::getInstance() {
    return *singleton;
}

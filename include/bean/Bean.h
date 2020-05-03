//
// Created by anarion on 5/3/20.
//

#ifndef ANBASE_BEAN_H
#define ANBASE_BEAN_H

#include <container/SString.h>
#include <container/Map/HashMap.hpp>

namespace anarion {
    class Bean {
    protected:
        HashMap<SString, void*> attrMap;

        void *getAttrByName(const SString &name);
        void registerAttr(const SString &name);
    public:
        template<typename T>
        T *getAttr(const SString &name) {

        }

        template<typename T>
        void setAttr(const SString &name, const T &val) {

        }

        template<typename T>
        void setAttr(const SString &name, T &&val) {

        }
    };
}

#endif //ANBASE_BEAN_H

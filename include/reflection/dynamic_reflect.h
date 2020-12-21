//
// Created by anarion on 2020/2/20.
//

#ifndef MYCPPBASE_DYNAMIC_REFLECT_H
#define MYCPPBASE_DYNAMIC_REFLECT_H

#include <container/Map/HashMap.hpp>
#include <container/SString.h>

namespace anarion {

    class AbstractNamedAttribute {
    public:
        virtual const SString &get_key() = 0;
    };

    template <typename T>
    class NamedAttribute : public AbstractNamedAttribute {
    protected:
        SString attributeName;
        T obj;

    public:
        NamedAttribute(SString &&attributeName, T &&obj)
            : attributeName(forward<SString>(attributeName)), obj(forward<T>(obj))
        {}
        const SString &getName() const {
            return attributeName;
        }
        T &getObject() {
            return obj;
        }
        const T &getObject() const {
            return obj;
        }

        const SString &get_key() override {
            return getName();
        }
    };

    template <typename T>
    struct hash_function<NamedAttribute<T>> {
        hash_type operator()(const NamedAttribute<T> &o) const {
            return hash_function<SString>()(o.getName());
        }
    };

    template <typename T>
    struct hash_function<NamedAttribute<T> *> {
        hash_type operator()(const NamedAttribute<T> *o) const {
            return hash_function<SString>()(o->getName());
        }
    };

    class GlobalTypeInfo {
    protected:
        static GlobalTypeInfo *singleton;
        hash_type lastId = 0;
    public:
        static GlobalTypeInfo &getInstance();

        hash_type allocateId();
    };

    template <typename T>
    struct Attribute {
        using type = T;
    };

    template <typename Class, typename Type>
    struct Attribute<Type Class::*> {
        using type = Type Class::*;
        using classType = Class;
        using objType = Type;

    private:
        static hash_type typeId;
    public:
        static hash_type getTypeId() {
            if (typeId == -1) {
                typeId = GlobalTypeInfo::getInstance().allocateId();
            }
            return typeId;
        }
    };
    template <typename Class, typename Type>
    hash_type Attribute<Type Class::*>::typeId = -1;

    template <typename T>
    class Reflected {
    private:
        HashMap<SString, AbstractNamedAttribute *> namedAttributes;
    };

#define Class(className) \
    class className : public anarion::Reflected \
    \

#define Attribute(access, type, attrName) \
private:                                  \
    void addReflect_##access_##type_##attrName() { \
                                              \
    }; \
access:                                       \
    type attrName
}
#endif //MYCPPBASE_DYNAMIC_REFLECT_H

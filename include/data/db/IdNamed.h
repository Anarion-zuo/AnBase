//
// Created by anarion on 12/8/20.
//

#ifndef ANBASE_IDNAMED_H
#define ANBASE_IDNAMED_H

#include <container/List/Vector.hpp>
#include <container/Set/HashSet.hpp>
#include <data/db/storage/FreeList.hpp>
#include <container/List/SoftArray.h>

namespace anarion {
namespace db {

template <typename IdNamed>
class NamedAllocator {
private:
    SoftArray<IdNamed *> array;
    using IdType = typename IdNamed::IdType;
    IdType curId = 0;
public:

    NamedAllocator() = default;

    IdNamed *get(IdType id) {
        if (id >= array.size()) {
            return nullptr;
        }
        return array.get(id);
    }
    const IdNamed *get(IdType id) const {
        if (id >= array.size()) {
            return nullptr;
        }
        return array.get(id);
    }
    IdType idOf(const IdNamed *obj) const {
        return obj->getId();
    }

private:

    void allocateHelper(IdNamed *obj) {
        obj->setId(curId);
        array.set(curId, obj);
        ++curId;
        while (true) {
            if (curId >= array.size() || array.get(curId) == nullptr) {
                break;
            }
            ++curId;
        }
    }

public:

    template <typename ...Args>
    IdNamed *allocate(Args ...args) {
        IdNamed *obj = new IdNamed(args...);
        allocateHelper(obj);
        return obj;
    }
    IdNamed *allocate() {
        IdNamed *obj = new IdNamed;
        allocateHelper(obj);
        return obj;
    }
    void deallocate(IdNamed *obj) {
        deallocate(idOf(obj));
    }
    void deallocate(IdType id) {
        if (id >= array.size()) {
            return;
        }
        if (id < curId) {
            curId = id;
        }
        delete array.get(id);
        array.set(id, nullptr);
    }

    struct Exception : std::exception {};
    struct OutOfNames : public Exception {};
    struct IdOutOfRange : public Exception {};
};
}
}
#endif //ANBASE_IDNAMED_H

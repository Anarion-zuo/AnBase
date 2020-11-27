//
// Created by anarion on 11/26/20.
//

#ifndef ANBASE_LRULIST_H
#define ANBASE_LRULIST_H

#include <container/List/LinkedList.hpp>
#include <container/Map/HashMap.hpp>

namespace anarion {
namespace db {

/**
 * @brief Implements LRU cache machenism.
 * @tparam T
 * @details Is implemented as a linked list. Every time an object is accessed, call mark to move it as the most-recent-accessed, as the object is moved to the front of the linked list. Every time
 */
template <typename T>
class LruList {
    LinkedList<T> list;
    HashMap<T, typename LinkedList<T>::iterator> iteratorMap;
public:
    LruList() = default;

    size_type size() const { return list.size(); }

    /**
     * @brief Includes a new object into this list.
     * @param o
     * @details Newly included objects are placed at the `most-recently accessed` place semantically.
     */
    void add(const T &o) {
        auto listIt = list.push_front(o);
        auto it = iteratorMap.find(o);
        if (it != iteratorMap.end_iterator()) {
            // the element exists
            list.remove(it->get_val());
            it->get_val() = listIt;
        } else {
            iteratorMap.insert({o, listIt});
        }
    }

    /**
     * @brief Pops the least-recently accessed object.
     * @return
     */
    T popLeastRecent() {
        iteratorMap.remove(list.back());
        return list.pop_back();
    }

    /**
     * @brief Removes certain object.
     * @param o
     * @return whether the object is included.
     */
    bool remove(const T &o) {
        auto mapIt = iteratorMap.find(o);
        if (mapIt == iteratorMap.end_iterator()) {
            return false;
        }
        list.remove(mapIt->get_val());
        iteratorMap.remove(mapIt);
        return true;
    }

    /**
     * @return the most-recently accessed object.
     */
    const T &mostRecent() const {
        return list.front();
    }

    /**
     * @brief Checks whether this list includes certain object.
     * @param o
     * @return
     */
    bool has(const T &o) const {
        return iteratorMap.find(o) != iteratorMap.end_iterator();
    }

    /**
     * @brief Marks certain object as `most-recently accessed`.
     * @param o
     * @return false if the object is not included.
     */
    bool mark(const T &o) {
        auto it = iteratorMap.find(o);
        if (it == iteratorMap.end_iterator()) {
            // object not exists
            return false;
        }
        list.move_to_front(it->get_val());
        return true;
    }
};

}
}

#endif //ANBASE_LRULIST_H

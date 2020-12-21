//
// Created by anarion on 12/21/20.
//

#ifndef ANBASE_NUMLISTSET_H
#define ANBASE_NUMLISTSET_H

#include "../List/Vector.hpp"

namespace anarion {
template <typename element_t>
class NumListSet {
    using index_t = element_t;
    struct Entry {
        index_t next = null;
        index_t prev = null;
    };
    Vector<Entry> array;
    static index_t null;

    void checkUnderflow() {
        if (empty()) {
            throw Underflow();
        }
    }

    void checkOverflow(index_t index) {
        if (index > capacity()) {
            throw Overflow();
        }
    }

    Entry &getEntry(index_t index) {
        return array.get(index);
    }
    const Entry &getEntry(index_t index) const {
        return array.get(index);
    }
    Entry &getHead() {
        return array.get(0);
    }
    const Entry &getHead() const {
        return array.get(0);
    }

    index_t element2index(element_t element) const {
        return element + 1;
    }

    element_t index2element(index_t index) const {
        return index - 1;
    }

public:

    explicit NumListSet(element_t initLength) : array(element2index(initLength), Entry{}) {
        getHead().next = 0;
        getHead().prev = 0;
    }

    NumListSet(const NumListSet &rhs) = default;

    bool empty() const {
        return getHead().next == 0 && getHead().prev == 0;
    }

    element_t capacity() const { return array.size() - 1; }

    void pushBack(element_t element) {
        index_t index = element2index(element);
        checkOverflow(index);
        if (getEntry(index).prev != null || getEntry(index).next != null) {
            // element already exists
            return;
        }
        index_t prev = getHead().prev;
        getEntry(prev).next = index;
        getEntry(index).next = 0;
        getHead().prev = index;
        getEntry(index).prev = prev;
    }

    element_t first() const {
        return index2element(getHead().next);
    }

    element_t back() const {
        return index2element(getHead().prev);
    }

    void remove(element_t element) {
        index_t index = element2index(element);
        index_t prev = getEntry(index).prev, next = getEntry(index).next;
        getEntry(prev).next = next;
        getEntry(next).prev = prev;
        getEntry(index).next = null;
        getEntry(index).prev = null;
    }

    element_t popBack() {
        checkUnderflow();
        element_t oldBack = index2element(getHead().prev);
        remove(oldBack);
        return oldBack;
    }

    bool has(element_t element) {
        index_t index = element2index(element);
        return getEntry(index).next != null || getEntry(index).prev != null;
    }

    void printElements() const {
        index_t index = getHead().next;
        while (index != 0) {
            printf("%u ", index2element(index));
            index = getEntry(index).next;
        }
    }

    struct Exception : public std::exception {};
    struct Underflow : public Exception {};
    struct Overflow : public Exception {};
};

template <typename index_t>
index_t NumListSet<index_t>::null = -1;
}

#endif //ANBASE_NUMLISTSET_H

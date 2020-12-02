//
// Created by anarion on 11/30/20.
//

#ifndef ANBASE_ARRAY_H
#define ANBASE_ARRAY_H

#include <container/base/Copier.hpp>
#include "../base/container_utility.hpp"

namespace anarion {
template <typename T/*, typename Allocator*/>
class Array {
protected:

struct Exception : public std::exception {};
struct OverFlow : public Exception {};
struct UnderFlow : public Exception {};
struct IndexOutOfRange : public Exception {};

    T *head, *cur;
    const size_type count;

    void checkIndexOutOfRange(size_type index) const {
        if (index >= size()) {
            throw IndexOutOfRange();
        }
    }

    void checkOverflow() const {
        if (size() == capacity()) {
            throw OverFlow();
        }
    }

    void checkUnderflow(size_type length) const {
        if (capacity() - size() < length) {
            throw UnderFlow();
        }
    }

    void checkUnderflow() const {
        if (size() == 0) {
            throw UnderFlow();
        }
    }

public:
    explicit Array(size_type count) :
    count(count), head(static_cast<T *>(operator new(count * sizeof(T)))) {
    }

    Array(size_type count, const T &obj) : Array(count) {
        for (size_type index = 0; index < count; ++index) {
            new (&this->head[index]) T(obj);
        }
    }

    ~Array() {
        clear();
        operator delete (head, count);
    }

    constexpr bool empty() const { return cur == head; }
    constexpr size_type size() const { return cur - head; }
    constexpr size_type capacity() const { return count; }

    void clear() {
        Copier<T>().clearSpace(head, size());
        cur = head;
    }

    T &get(size_type index) {
        checkIndexOutOfRange(index);
        return head[index];
    }

    const T &get(size_type index) const {
        checkIndexOutOfRange(index);
        return head[index];
    }

    T &operator[](size_type index) {
        return get(index);
    }

    const T &operator[](size_type index) const {
        return get(index);
    }

    void pushBack(const T &obj) {
        checkOverflow();
        new (cur) T(obj);
        ++cur;
    }

    void pushBack(T &&obj) {
        checkOverflow();
        new (cur) T(forward<T>(obj));
        ++cur;
    }

    using iterator = T *;

    constexpr iterator beginIterator() const { return head; }
    constexpr iterator endIterator() const { return cur; }
    constexpr bool hasIterator(iterator it) const { return it >= head && it < cur; }

protected:
    void insertPrepare(size_type index, size_type insertLength) {
        checkOverflow();
        Copier<T>().moveBackward(head, size(), index, insertLength);
    }

public:

    iterator insert(size_type index, const T &obj) {
        insertPrepare(index, 1);
        new (head + index) T(obj);
        return head + index;
    }

    iterator insert(size_type index, T &&obj) {
        insertPrepare(index, 1);
        new (head + index) T(forward<T>(obj));
        return head + index;
    }

    iterator insert(iterator it, const T &obj) {
        return insert(it - head, obj);
    }

    iterator insert(iterator it, T &&obj) {
        return insert(it - head, forward<T>(obj));
    }

    iterator insert(size_type index, iterator begin, size_type length) {
        insertPrepare(index, length);
        Copier<T>().copy(head + index, begin, length);
        return head + index;
    }

    iterator insert(iterator it, iterator begin, size_type length) {
        return insert(it - head, begin, length);
    }

    void pushFront(const T &obj) {
        insert(0, obj);
    }

    void pushFront(T &&obj) {
        insert(0, forward<T>(obj));
    }

protected:
    void removePrepare(size_type index, size_type removeLength) {
        checkUnderflow(removeLength);
        Copier<T>().moveForward(head, size(), index, removeLength);
    }

public:
    iterator remove(size_type index) {
        removePrepare(index, 1);
        return head + index;
    }

    iterator remove(iterator it) {
        return remove(it - head);
    }

    iterator remove(size_type begin, size_type length) {
        removePrepare(begin, length);
        return head + begin;
    }

    iterator remove(iterator begin, size_type length) {
        return remove(begin - head, length);
    }

    T popBack() {
        checkUnderflow();
        T ret = *(cur - 1);
        remove(cur - head - 1);
        return move(ret);
    }

    T popFront() {
        checkUnderflow();
        T ret = *head;
        remove(0);
        return move(ret);
    }
};
}

#endif //ANBASE_ARRAY_H

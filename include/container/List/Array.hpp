#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "../base/container_utility.hpp"
#include "../../exceptions/container/IndexOutOfRange.h"
#include "../../exceptions/container/EmptyContainer.h"

namespace anarion {
template <typename T, size_type arrLength = 0> class Array {
protected:
    T *p = operator new(arrLength * sizeof(T));
    size_type cur = 0;

    void clear_space_impl(T *first, size_type num, true_type) {
        // nothing here
    }

    void clear_space_impl(T *first, size_type num, false_type) {
        for (size_type i = 0; i < num; ++i) {
            first[i].~T();
        }
    }

    // call destructors
    void clear_space(T *first, size_type num) {
        clear_space_impl(first, num, typename type_trait<T>::is_pod());
    }

public:

    typedef T* iterator;

    iterator begin_iterator() const { return p; }
    iterator end_iterator() const { return p + cur; }

    Array() = default;

    ~Array() {
        clear_space(p, cur);
        operator delete(p, sizeof(T) * arrLength); 
    }

    Array(const Array &rhs) : p(operator new(arrLength * sizeof(T))), cur(rhs.cur) {
        copyCtorObjects(p, rhs.p, rhs.cur);
    }

    Array(Array &&rhs) noexcept : p(rhs.p), cur(rhs.cur) {
        rhs.p = nullptr;
        rhs.cur = 0;
    }

    Array &operator=(const Array &rhs) {
        if (&rhs == this) { return *this; }
        clear_space(p, cur);
        cur = rhs.cur;
        copyCtorObjects(p, rhs.p, cur);
        return *this;
    }

    Array &operator=(Array &&rhs) noexcept {
        clear_space(p, cur);
        operator delete(p, sizeof(T) * arrLength);
        p = rhs.p;
        cur = rhs.cur;
        rhs.p = nullptr;
        rhs.cur = 0;
        return *this;
    }

    T &get(size_type index) {
        if (index >= cur) {
            throw IndexOutOfRange();
        }
        return p[index];
    }

    T &operator[](size_type index) { return get(index); }

    void push_back(const T &o) {
        if (cur >= arrLength) {
            throw IndexOutOfRange();
        }
        copyCtorObjects(p + cur, &o, 1);
        ++cur;
    }

    void push_back(T &&o) {
        if (cur >= arrLength) {
            throw IndexOutOfRange();
        }
        moveCtorObjects(p + cur, &o, 1);
        ++cur;
    }

    T pop_back() {
        if (cur == 0) {
            throw EmptyContainer();
        }
        --cur;
        T &o = p[cur];
        return move(T);
    }

    size_type size() const { return cur; }
    size_type capacity() const { return arrLength; }
};
}

#endif //ARRAY_HPP
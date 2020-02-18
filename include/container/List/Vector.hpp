//
// Created by 左钰 on 2020/1/21.
//

#ifndef MYCPPLIB_VECTOR_HPP
#define MYCPPLIB_VECTOR_HPP

#include "../../exceptions/container/IndexOutOfRange.h"
#include "../base/type_trait.hpp"
#include "../base/container_utility.hpp"
#include "../../exceptions/container/EmptyContainer.h"

namespace anarion {
    template<typename T>
    class Vector {
    protected:

        T *begin = nullptr, *cur = nullptr, *end = nullptr;

        void expandWhenAdd() {
            resize(size() << 1u);
        }

        void copy_reverse_pod_impl(T *it, size_type num, true_type) {
            if (cur == it) { return; }
            memcpy(it + num, it, (cur - it) * sizeof(T));
        }

        void copy_reverse_pod_impl(T *it, size_type num, false_type) {
            if (cur == it) { return; }
            // copy from end to begin
            T *p = cur;
            --p;
            // made sure the destination is released
            for (; p >= it; --p) {
                T *np = p + num;
                new(np) T(move(*p));
                p->~T();   // release old space
            }
        }

        void copy_back_n(T *it, size_type num) {
            size_type oldsize = size(), newsize = oldsize + num, index = it - begin;
            // make for more space
            if (newsize >= capacity()) {
                newsize += newsize >> 1u;
                resize(newsize);
            }
            // old pointer may not be valid
            copy_reverse_pod_impl(begin + index, num, typename type_trait<T>::is_pod());
            // destruct remaining objects
            // unnecessary
            // update members
            cur += num;
        }

        void copy_forward_pod_impl(T *it, size_type num, true_type) {
            memcpy(it - num, it, (it - begin) * sizeof(T));
        }

        void copy_forward_pod_impl(T *it, size_type num, false_type) {
            // copy from begin to end
            T *p = it;
            for (; p < cur; ++p) {
                T *np = p - num;
                np->~T();   // release in advance
                new(np) T(std::move(*p));
            }
        }

        void copy_forward_n(T *it, size_type num) {
            copy_forward_pod_impl(it, num, type_trait<T>::is_pod());
            // destruct remaining objects
            T *p;
            p = cur - num;
            clear_space(p, num);
            // update members
            cur -= num;
        }

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

        T *new_space(size_type num) {
            return (T*)operator new(num * sizeof(T));
        }

        // optimized copy

        /*
            The reverse functions follow the convention of [) regardless of its copying order
            The input dst and src would not be copied
        */
        void reverse_copy(T *dst, T *src, size_type num) {
            reverse_copy_impl(dst, src, type_trait<T>::is_pod());
        }

        void reverse_copy_impl(T *dst, T *src, size_type num, true_type) {
            dst -= num;
            src -= num;
            memcpy(dst, src, num * sizeof(T));
        }

        void reverse_copy_impl(T *dst, T *src, size_type num, false_type) {
            for (size_type i = 0; i < num; ++i) {
                --dst;
                --src;
                new(dst) T(*src);
            }
        }

        void reverse_move(T *dst, T *src, size_type num) {
            reverse_move_impl(dst, src, type_trait<T>::has_move_ctor());
        }

        void reverse_move_impl(T *dst, T *src, size_type num, true_type) {
            for (size_type i = 0; i < num; ++i) {
                --dst;
                --src;
                new(dst) T(move(*src));
            }
        }

        void reverse_move_impl(T *dst, T *src, size_type num, false_type) {
            reverse_copy(dst, src, num);
        }

        void copy_back_expand(size_type index, size_type steps) {
            size_type oldsize = size(), oldcap = capacity(), newcap;
            if (index > oldsize) { throw IndexOutOfRange(); }
            T *new_cur = cur + steps;
            if (new_cur > end) {   // require expand
                newcap = (steps + oldsize) << 1u;
                T *n_space = new_space(newcap);
                // copy head
                moveCtorObjects(n_space, begin, index);
                // copy tail
                moveCtorObjects(n_space + index + steps, begin + index, oldsize - index);
                // clean up
                clear_space(begin, oldsize);
                operator delete(begin, sizeof(T) * oldcap);
                // reset
                begin = n_space;
                cur = begin + steps + oldsize;
                end = begin + newcap;
            } else {   // no expand
                reverse_copy(new_cur, begin + index + steps, steps);
                cur = new_cur;
            }
        }

        void seq_move(T *dst, T *src, size_type num) {
            seq_move_impl(dst, src, num, type_trait<T>::has_move_ctor());
        }

        void seq_move_impl(T *dst, T *src, size_type num, true_type) {
            for (size_type i = 0; i < num; ++i) {
                new (dst) T(move(*src));
                ++dst;
                ++src;
            }
        }

        void seq_move_impl(T *dst, T *src, size_type num, false_type) {
            seq_copy(dst, src, num);
        }

        void seq_copy(T *dst, T *src, size_type num) {
            seq_copy_impl(dst, src, num, type_trait<T>::is_pod());
        }

        void seq_copy_impl(T *dst, T *src, size_type num, true_type) {
            memcpy(dst, src, sizeof(T) * num);
        }

        void seq_copy_impl(T *dst, T *src, size_type num, false) {
            for (size_type i = 0; i < num; ++i) {
                new (dst) T(*src);
                ++dst;
                ++src;
            }
        }

        void copy_forward_expand(size_type index, size_type steps) {
            size_type oldsize = size();
            T *new_cur = cur - steps;
            if (new_cur < begin || index > oldsize) { throw IndexOutOfRange(); }
            size_type oldcap = capacity(),  newsize = oldsize - steps;
            if (newsize * 3 <= oldcap) {
                size_type newcap = newsize << 1;
                T *n_space = new_space(newcap);
                // copy head
                seq_move(n_space, begin, index);
                // copy tail
                seq_move(n_space, begin + index + steps, oldsize - index);
                // clean up
                clear_space(begin, oldsize);
                operator delete (begin, oldcap * sizeof(T));
                // reset
                begin = n_space;
                cur = n_space + newsize;
                end = n_space + newcap;
                return;
            }
            size_type new_index = index - steps;
            clear_space(begin + new_index, steps);
            seq_move(begin + new_index, begin + index, steps);
            cur -= steps;
        }

    public:

        typedef T *iterator;

        iterator begin_iterator() { return begin; }

        iterator end_iterator() { return cur; }

        bool has_iterator(iterator it) { return it < cur && it >= begin; }

        Vector() = default;

        explicit Vector(size_type initialSize) {
            if (initialSize == 0) {
                return;
            }
            begin = new_space(initialSize);
            end = (begin + initialSize);
            cur = begin;
        }

        Vector(size_type initialSize, const T &rhs) {
            if (initialSize == 0) {
                return;
            }
            begin = new_space(initialSize);
            end = (begin + initialSize);
            cur = begin + initialSize;
            for (size_type i = 0; i < initialSize; ++i) {
                new(&begin[i]) T(rhs);
            }
        }

        Vector(const Vector<T> &rhs) : begin(new_space(rhs.size())), cur(begin + rhs.size()), end(cur) {
            if (rhs.empty()) {
                return;
            }
            for (size_type i = 0; i < size(); ++i) {
                new(&begin[i]) T(rhs.begin[i]);
            }
        }

        Vector(Vector<T> &&rhs) noexcept : begin(rhs.begin), cur(rhs.cur), end(rhs.end) {
            rhs.begin = nullptr;
            rhs.end = nullptr;
            rhs.cur = nullptr;
        }

        Vector(T *p, size_type len) : begin(p), cur(begin + len), end(cur) {}   // move

        ~Vector() {
            clear();
        }

        Vector<T> &operator=(const Vector<T> &rhs) {
            if (&rhs == this) {
                return *this;
            }
            clear();
            // reset members
            begin = new_space(rhs.size());
            cur = begin + rhs.size();
            end = cur;
            // make copy
            copyCtorObjects(begin, rhs.begin, rhs.size());
            return *this;
        }

        Vector<T> &operator=(Vector<T> &&rhs) noexcept {
            clear();
            // reset members
            begin = rhs.begin;
            cur = rhs.cur;
            end = rhs.end;
            rhs.begin = nullptr;
            rhs.cur = nullptr;
            rhs.end = nullptr;
            return *this;
        }

        void clear() {
            clear_space(begin, size());
            operator delete(begin, capacity() * sizeof(T));
            begin = nullptr;
            end = nullptr;
            cur = nullptr;
        }

        bool empty() const {
            return !size();
        }

        size_type size() const {
            return cur - begin;
        }

        size_type capacity() const {
            return end - begin;
        }

        void resize(size_type new_size) {
            size_type oldsize = size(), oldcap = capacity(), newcap;
            if (new_size < oldsize) {
                newcap = new_size;
                // release the extra objects
                clear_space(&begin[new_size], oldsize - new_size);
            } else {
                newcap = oldsize;
            }
            // create new space
            T *newp = new_space(newcap);
            // move to new space
            seq_move(newp, begin, newcap);
            // release old space
            clear_space(begin, oldsize);
            operator delete(begin, oldcap * sizeof(T));
            // update members
            begin = newp;
            cur = newp + newcap;
            end = newp + new_size;
        }

        void push_back(const T &o) {
            if (begin == nullptr) {
                resize(1);
            }
            if (cur == end) {
                expandWhenAdd();
            }
            new(cur++) T(o);
        }

        void push_back(T &&o) {
            if (begin == nullptr) {
                resize(1);
            }
            if (cur == end) {
                expandWhenAdd();
            }
            new(cur++) T(forward<T>(o));
        }

        T pop_back() {
            if (empty()) { throw EmptyContainer(); }
            --cur;
            T &last = *cur;
            return move(last);
        }

        T &get(size_type index) {
            if (index > size()) { throw IndexOutOfRange(); }
            return begin[index];
        }

        T &operator[](size_type index) {
            return get(index);
        }

        iterator insert(iterator it, const T &o) {
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            copy_back_expand(index, 1);
            new(begin + index) T(o);
            return begin + index;
        }

        iterator insert(iterator it, T &&o) {
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            copy_back_n(it, 1);
            new(begin + index) T(forward<T>(o));
            return begin + index;
        }

        iterator insert(size_type index, const T &o) {
            if (index >= size()) { throw IndexOutOfRange(); }
            copy_back_expand(index, 1);
            new(begin + index) T(o);
            return begin + index;
        }

        iterator insert(size_type index, T &&o) {
            if (index >= size()) { throw IndexOutOfRange(); }
            copy_back_expand(index, 1);
            new(begin + index) T(forward<T>(o));
            return begin + index;
        }

        iterator insert(iterator it, iterator b, iterator e) {
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            while (b != e) {
                it = insert(it, *b);
                ++b;
                ++it;
            }
            return index + begin;
        }

        template <typename It>
        iterator insert(iterator it, It b, size_type num) {
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            copy_back_expand(index, num);
            size_type old_index = index;
            for (size_type i = 0; i < num; ++i, ++b, ++index) {
                new (begin + index) T(*b);
            }
            return begin + old_index;
        }

        iterator insert(iterator it, T *p, size_type num) {
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            copy_back_expand(index, num);
            size_type old_index = index;
            for (size_type i = 0; i < num; ++i, ++b, ++index) {
                seq_copy(begin + index, p, num);
            }
            return begin + old_index;
        }

        void assign(const T &o, size_type num) {
            clear();
            resize(num);
            for (size_type i = 0; i < num; ++i) {
                push_back(o);
            }
        }

        template<typename C>
        void assign(typename C::iterator begin, typename C::iterator end) {
            clear();
            while (begin != end) {
                push_back(*begin);
                ++begin;
            }
        }

        void remove(iterator it) {
            if (it > cur) { throw IndexOutOfRange(); }
            copy_forward_n(it, 1);
        }

        void remove(iterator b, iterator e) {
            if (e > cur) { throw IndexOutOfRange(); }
            size_type removing_num = e - b;
            remove(b, removing_num);
        }

        void remove(iterator b, size_type num) {
            if (b + num > cur) { throw IndexOutOfRange(); }
            if (num == 0) {
                return;
            }
            copy_forward_n(b + num, num);
        }

        T *getArr() const {
            return begin;
        }

        bool operator==(const Vector &rhs) const {
            if (&rhs == this) {
                return true;
            }
            if (rhs.size() != size()) {
                return false;
            }
            for (size_type i = 0; i < size(); ++i) {
                if (begin[i] != rhs.begin[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector &rhs) const {
            return !(rhs == *this);
        }

        iterator find(const T &o) const {
            T *p = begin;
            while (p != cur) {
                if (o == *p) {
                    return iterator(p);
                }
                ++p;
            }
            return end_iterator();
        }
    };

};

#endif //MYCPPLIB_VECTOR_HPP

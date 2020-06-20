//
// Created by 左钰 on 2020/1/21.
//

#ifndef MYCPPLIB_VECTOR_HPP
#define MYCPPLIB_VECTOR_HPP

#include "../base/type_trait.hpp"
#include "../base/container_utility.hpp"
#include <iostream>

namespace anarion {


#pragma region memory
    template<typename T>
    void clear_space_impl(T *first, size_type num, true_type) {
        // nothing here
    }

    template<typename T>
    void clear_space_impl(T *first, size_type num, false_type) {
        for (size_type i = 0; i < num; ++i) {
            first[i].~T();
        }
    }

    // call destructors
    template<typename T>
    void clear_space(T *first, size_type num) {
        clear_space_impl(first, num, typename type_trait<T>::is_pod());
    }

    template<typename T>
    T *new_space(size_type num) {
        if (num == 0) { return nullptr; }
        return (T*)operator new(num * sizeof(T));
    }
#pragma endregion

    // optimized copy
#pragma region optimized_copy
    /*
        The reverse functions follow the convention of [) regardless of its copying order
        The input dst and src would not be copied
    */
    template<typename T>
    void reverse_copy(T *dst, T *src, size_type num) {
        reverse_copy_impl(dst, src, num, typename type_trait<T>::is_pod());
    }

    template<typename T>
    void reverse_copy_impl(T *dst, T *src, size_type num, true_type) {
        dst -= num;
        src -= num;
        memcpy(dst, src, num * sizeof(T));
    }

    template<typename T>
    void reverse_copy_impl(T *dst, T *src, size_type num, false_type) {
        for (size_type i = 0; i < num; ++i) {
            --dst;
            --src;
            new(dst) T(*src);
            src->~T();
        }
    }

    template<typename T>
    void reverse_move(T *dst, T *src, size_type num) {
        reverse_move_impl(dst, src, typename type_trait<T>::has_move_ctor());
    }

    template<typename T>
    void reverse_move_impl(T *dst, T *src, size_type num, true_type) {
        for (size_type i = 0; i < num; ++i) {
            --dst;
            --src;
            new(dst) T(move(*src));
            src->~T();
        }
    }

    template<typename T>
    void reverse_move_impl(T *dst, T *src, size_type num, false_type) {
        reverse_copy(dst, src, num);
    }


    template<typename T>
    void seq_move(T *dst, T *src, size_type num) {
        seq_move_impl(dst, src, num, typename type_trait<T>::has_move_ctor());
    }

    template<typename T>
    void seq_move_impl(T *dst, T *src, size_type num, true_type) {
        for (size_type i = 0; i < num; ++i) {
            new (dst) T(move(*src));
            src->~T();
            ++dst;
            ++src;
        }
    }

    template<typename T>
    void seq_copy(T *dst, T *src, size_type num);
    template<typename T>
    void seq_move_impl(T *dst, T *src, size_type num, false_type) {
        seq_copy(dst, src, num);
    }

    template<typename T>
    void seq_copy(T *dst, T *src, size_type num) {
        seq_copy_impl(dst, src, num, typename type_trait<T>::is_pod());
    }

    template<typename T>
    void seq_copy_impl(T *dst, T *src, size_type num, true_type) {
        memcpy(dst, src, sizeof(T) * num);
    }

    template<typename T>
    void seq_copy_impl(T *dst, T *src, size_type num, false_type) {
        for (size_type i = 0; i < num; ++i) {
            new (dst) T(*src);
            src->~T();
            ++dst;
            ++src;
        }
    }
#pragma endregion

    template<typename T>
    class Vector {
    protected:

        T *begin = nullptr, *cur = nullptr, *end = nullptr;

        void expand_push() { resize(size() << 1u); }

        void copy_back_expand(size_type index, size_type steps) {
            size_type oldsize = size(), oldcap = capacity(), newcap;
            if (index > oldsize) { throw IndexOutOfRange(); }
            T *new_cur = cur + steps;
            if (new_cur > end) {   // require expand
                newcap = (steps + oldsize) << 1u;
                T *n_space = new_space<T>(newcap);
                // copy head
                seq_move(n_space, begin, index);
                // copy tail
                seq_move(n_space + index + steps, begin + index, oldsize - index);
                // clean up
                clear_space(begin, oldsize);
                operator delete(begin, sizeof(T) * oldcap);
                // reset
                begin = n_space;
                cur = begin + steps + oldsize;
                end = begin + newcap;
            } else {   // no expand
                reverse_copy(new_cur, cur, oldsize - index);
                cur = new_cur;
            }
        }

        void copy_forward_expand(size_type index, size_type steps) {
            size_type oldsize = size();
            T *new_cur = cur - steps;
            if (new_cur < begin || index > oldsize) { throw IndexOutOfRange(); }
            size_type oldcap = capacity(),  newsize = oldsize - steps;
            if (newsize == 0) {
                clear();
                return;
            }
            /*
            if (newsize * 3 <= oldcap) {
                size_type newcap = newsize << 1;
                T *n_space = new_space(newcap);
                // copy head
                seq_move(n_space, begin, index - steps + 1);
                // copy tail
                seq_move(n_space + index - steps, begin + index, oldsize - index);
                // clean up
                clear_space(begin, oldsize);
                operator delete (begin, oldcap * sizeof(T));
                // reset
                begin = n_space;
                cur = n_space + newsize;
                end = n_space + newcap;
                return;
            }
             */
            size_type new_index = index + 1ul - steps;
            clear_space(begin + new_index, steps);
            seq_move(begin + new_index, begin + index, oldsize - index);
            cur -= steps;
        }
        #pragma endregion

    public:

        typedef T *iterator;

        iterator begin_iterator() const { return begin; }
        iterator end_iterator() const { return cur; }
        bool has_iterator(iterator it) const { return it < cur && it >= begin; }

        #pragma region ctors_assigns_dtor
        Vector() = default;

        explicit Vector(size_type initialSize) {
            if (initialSize == 0) {
                return;
            }
            begin = new_space<T>(initialSize);
            end = (begin + initialSize);
            cur = begin;
        }

        Vector(size_type initialSize, const T &rhs) {
            if (initialSize == 0) {
                return;
            }
            begin = new_space<T>(initialSize);
            end = (begin + initialSize);
            cur = begin + initialSize;
            for (size_type i = 0; i < initialSize; ++i) {
                new(&begin[i]) T(rhs);
            }
        }

        Vector(const Vector<T> &rhs) : begin(new_space<T>(rhs.size())), cur(begin + rhs.size()), end(cur) {
            if (rhs.empty()) {
                return;
            }
            for (size_type i = 0; i < size(); ++i) {
                new(&begin[i]) T(rhs.begin[i]);
            }
        }

        Vector(Vector<T> &&rhs) noexcept : begin(rhs.begin), cur(rhs.cur), end(rhs.end) {
            rhs.clearMove();
        }

        Vector(std::initializer_list<T> &&initList) {
            size_type length = initList.size();
            if (length == 0) { return; }
            begin = new_space<T>(length);
            end = begin + length;
            cur = begin + length;
            size_type index = 0;
            for (auto &item : initList) {
                new (&begin[index]) T(move(item));
                ++index;
            }
        }

        void clearMove() {
            begin = nullptr;
            end = nullptr;
            cur = nullptr;
        }

        Vector(T *p, size_type len) : begin(p), cur(begin + len), end(cur) {}   // move

        virtual ~Vector() {
            clear();
        }

        Vector<T> &operator=(const Vector<T> &rhs) {
            if (&rhs == this) {
                return *this;
            }
            clear();
            // reset members
            begin = new_space<T>(rhs.size());
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
            operator delete(begin, (end - begin) * sizeof(T));
            begin = nullptr;
            end = nullptr;
            cur = nullptr;
        }
        #pragma endregion

        #pragma region sizes
        constexpr bool empty() const {
            return !size();
        }

        constexpr size_type size() const {
            return cur - begin;
        }

        constexpr size_type capacity() const {
            return end - begin;
        }
        
        void resize(size_type new_size) {
            if (new_size == 0) {
                clear();
                return;
            }
            size_type oldsize = size(), oldcap = capacity(), newcap, newsize;
            if (oldcap == 0) {
                begin = new_space<T>(new_size);
                cur = begin;
                end = begin + new_size;
                return;
            }
            if (new_size < oldsize) {
                newcap = new_size;
                newsize = new_size;
                // release the extra objects
//                clear_space(&begin[new_size], oldsize - new_size);
            } else {
                newcap = new_size;
                newsize = oldsize;
            }
            // create new space
            T *newp = new_space<T>(newcap);
            // move to new space
            seq_move(newp, begin, newsize);
            // release old space
            operator delete(begin, oldsize * sizeof(T));
            // update members
            begin = newp;
            cur = newp + newsize;
            end = newp + newcap;
        }
        #pragma endregion

        #pragma region back_front
        void push_back(const T &o) {
            if (begin == nullptr) {
                resize(1);
            }
            if (cur == end) {
                expand_push();
            }
            new(cur++) T(o);
        }

        void push_back(T &&o) {
            if (begin == nullptr) {
                resize(1);
            }
            if (cur == end) {
                expand_push();
            }
            new(cur++) T(forward<T>(o));
        }

        T pop_back() {
            if (empty()) { throw EmptyContainer(); }
            --cur;
            T &last = *cur;
            return move(last);
        }

        T pop_front() {
            if (empty()) { throw EmptyContainer(); }
            T ret = move(begin[0]);
            remove(0ul);
            return move(ret);
        }
        #pragma endregion

        T &get(size_type index) {
            if (index >= size()) { throw IndexOutOfRange(); }
            return begin[index];
        }

        const T &get(size_type index) const {
            if (index >= size()) { throw IndexOutOfRange(); }
            return begin[index];
        }

        T &operator[](size_type index) {
            return get(index);
        }

        const T &operator[](size_type index) const {
            return get(index);
        }

        #pragma region insert
        iterator insert(iterator it, const T &o) {
            if (empty()) {
                size_type oldIndex = it - begin;
                resize(1);
                it = begin + oldIndex;
            }
            if (it > cur) { throw IndexOutOfRange(); }
            if (it == cur) { push_back(o); return cur - 1; }
            size_type index = it - begin;
            copy_back_expand(index, 1);
            new(begin + index) T(o);
            return begin + index;
        }

        iterator insert(iterator it, T &&o) {
            if (empty()) {
                size_type oldIndex = it - begin;
                resize(1);
                it = begin + oldIndex;
            }
            if (it > cur) { throw IndexOutOfRange(); }
            if (it == cur) { push_back(forward<T>(o)); return cur - 1; }
            size_type index = it - begin;
            copy_back_expand(index, 1);
            new(begin + index) T(forward<T>(o));
            return begin + index;
        }

        iterator insert(size_type index, const T &o) {
            if (empty()) {
                resize(1);
            }
            if (index > size()) { throw IndexOutOfRange(); }
            if (index == size()) { push_back(o); return cur - 1; }
            copy_back_expand(index, 1);
            new(begin + index) T(o);
            return begin + index;
        }

        iterator insert(size_type index, T &&o) {
            if (empty()) {
                resize(1);
            }
            if (index > size()) { throw IndexOutOfRange(); }
            if (index == size()) { push_back(forward<T>(o)); return cur - 1; }
            copy_back_expand(index, 1);
            new(begin + index) T(forward<T>(o));
            return begin + index;
        }

        iterator insert(iterator it, iterator b, iterator e) {
            if (empty()) {
                size_type oldIndex = it - begin;
                resize(e - b + 1);
                it = begin + oldIndex;
            }
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
            if (empty()) {
                size_type oldIndex = it - begin;
                resize(num + 1);
                it = begin + oldIndex;
            }
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
            if (empty()) {
                size_type oldIndex = it - begin;
                resize(num + 1);
                it = begin + oldIndex;
            }
            if (it > cur) { throw IndexOutOfRange(); }
            size_type index = it - begin;
            copy_back_expand(index, num);
            size_type old_index = index;
            seq_copy(begin + index, p, num);
            return begin + old_index;
        }
        #pragma endregion

        #pragma region assign_remove
        void assign(const T &o, size_type num) {
            clear();
            resize(num);
            for (size_type i = 0; i < num; ++i) {
                push_back(o);
            }
        }

        template<typename I>
        void assign(I begin, I end) {
            clear();
            while (begin != end) {
                push_back(*begin);
                ++begin;
            }
        }

        void remove(size_type index) {
            remove(begin + index);
        }

        void remove(iterator it) {
            if (it > cur) { throw IndexOutOfRange(); }
            copy_forward_expand(it - begin, 1);
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
            copy_forward_expand(b - begin + num, num);
        }

        void remove(size_type index, size_type length) {
            remove(begin + index, length);
        }
        #pragma endregion

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

    template <typename T>
    void swap(typename Vector<T>::iterator x, typename Vector<T>::iterator y) {
        T o = move(*x);
        x->~T();
        new (x) T(move(*y));
        y->~T();
        new (y) T(move(o));
    }

};

#endif //MYCPPLIB_VECTOR_HPP

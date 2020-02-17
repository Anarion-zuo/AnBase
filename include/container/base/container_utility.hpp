//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_CONTAINER_UTILITY_HPP
#define MYCPPLIB_CONTAINER_UTILITY_HPP

#include "type_trait.hpp"
#include <cstring>

namespace anarion {
    typedef unsigned long size_type;

/*
 * type hacks
 */
    template<typename T>
    struct remove_const {
        typedef T type;
    };
    template<typename T>
    struct remove_const<const T> {
        typedef T type;
    };

    template<typename T>
    struct remove_ref {
        typedef T type;
    };
    template<typename T>
    struct remove_ref<T &> {
        typedef T type;
    };

    template<typename T>
    struct bare_type {
        typedef T type;
    };
    template<typename T>
    struct bare_type<T *> {
        typedef T type;
    };
    template <typename T>
    struct bare_type<const T> {
        typedef T type;
    };
    template<typename T>
    struct bare_type<T &> {
        typedef T type;
    };
    template<typename T>
    struct bare_type<const T &> {
        typedef T type;
    };
    template<typename T>
    struct bare_type<const T *> {
        typedef T type;
    };
    template<typename T>
    struct bare_type<T *const> {
        typedef T type;
    };
    template<typename T>
    struct bare_type<T &&> {
        typedef T type;
    };

    template<typename T>
    constexpr typename remove_ref<T>::type &&move(T &o) noexcept { return static_cast<typename remove_ref<T>::type &&>(o); }

    template<typename T>
    constexpr T&& forward(typename remove_ref<T>::type &&o) noexcept { return static_cast<T&&>(o); }

    template<typename T>
    constexpr T&& forward(typename remove_ref<T>::type &o) noexcept { return static_cast<T&&>(o); }

/*
 * copy and moves
 */

    template<typename T>
    void copyCtorObjectsImpl(T *dst, T *src, size_type num, true_type) {
        memcpy(dst, src, num * sizeof(T));
    }

    template<typename T>
    void copyCtorObjectsImpl(T *dst, T *src, size_type num, false_type) {
        for (size_type i = 0; i < num; ++i) {
            new(&dst[i]) T(src[i]);
        }
    }

    template<typename T>
    void copyCtorObjects(T *dst, T *src, size_type num) {
        copyCtorObjectsImpl(dst, src, num, typename type_trait<T>::is_pod());
    }

    template<typename T>
    void moveCtorObjectsImpl(T *dst, T *src, size_type num, true_type) {
        for (size_type i = 0; i < num; ++i) {
            new(&dst[i]) T(move(src[i]));
        }
    }

    template<typename T>
    void moveCtorObjectsImpl(T *dst, T *src, size_type num, false_type) {
        copyCtorObjects(dst, src, num);
    }

    template<typename T>
    void moveCtorObjects(T *dst, T *src, size_type num) {
        moveCtorObjectsImpl(dst, src, num, typename type_trait<T>::has_move_ctor());
    }

};

#endif //MYCPPLIB_CONTAINER_UTILITY_HPP

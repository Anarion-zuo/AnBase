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

    struct ContainerException : public std::exception {
        const char * what() const noexcept override { return "Container failed"; }
    };

    struct EmptyContainer : public ContainerException {
        const char *what() const noexcept override {
            return "The container has no element";
        }
    };

    struct IndexOutOfRange : public ContainerException {
        const char *what() const noexcept {
            return "Index out of range...";
        }
    };

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

};

#endif //MYCPPLIB_CONTAINER_UTILITY_HPP

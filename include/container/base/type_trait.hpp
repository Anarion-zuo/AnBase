//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_TYPE_TRAIT_HPP
#define MYCPPLIB_TYPE_TRAIT_HPP

#include <typeinfo>

/*
    POD -- plain old type
    clases requiring no special treatments when constructing, copying, and destructing
    copying can be performed by merely memcpy
    moving is no different from copying
    usually tiny

    Empty Ctor -- holding a constructor taking no attribute
*/

namespace anarion {

    struct true_type {
    };
    struct false_type {
    };

    template<typename T>
    struct type_trait {
        typedef false_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;

        static const char *name() { return typeid(T).name(); }
    };

    #pragma region partial specializations
    template<typename T>
    struct type_trait<T *> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;

        static const char *name() { return typename type_trait<T>::name(); }
    };

    template<typename T>
    struct type_trait<const T *> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;

        static const char *name() { return type_trait<T>::name(); }
    };

    template <typename T>
    struct type_trait<T&> {
        typedef typename type_trait<T>::is_pod is_pod;
        typedef typename type_trait<T>::has_empty_ctor has_empty_ctor;
        typedef typename type_trait<T>::has_move_ctor has_move_ctor;
        typedef typename type_trait<T>::has_move_assign has_move_assign;
        static const char *name() { return typename type_trait<T>::name(); }
    };

    template <typename T>
    struct type_trait<const T&> {
        typedef typename type_trait<T>::is_pod is_pod;
        typedef typename type_trait<T>::has_empty_ctor has_empty_ctor;
        typedef typename type_trait<T>::has_move_ctor has_move_ctor;
        typedef typename type_trait<T>::has_move_assign has_move_assign;
        static const char *name() { return typename type_trait<T>::name(); }
    };
    #pragma endregion

    #pragma region fundamental types
    template<>
    struct type_trait<char> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "char"; }
    };

    template<>
    struct type_trait<short> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "short"; }
    };

    template<>
    struct type_trait<int> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "int"; }
    };

    template<>
    struct type_trait<long> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "long"; }
    };

    template<>
    struct type_trait<unsigned char> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "unsigned char"; }
    };

    template<>
    struct type_trait<unsigned short> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "unsigned short"; }
    };

    template<>
    struct type_trait<unsigned int> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "unsigned int"; }
    };

    template<>
    struct type_trait<unsigned long> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "unsigned long"; }
    };

    template<>
    struct type_trait<float> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "float"; }
    };

    template<>
    struct type_trait<double> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
        static const char *name() { return "double"; }
    };

    #pragma endregion

};

#endif //MYCPPLIB_TYPE_TRAIT_HPP

//
// Created by anarion on 2020/2/2.
// https://github.com/Anarion-zuo untrac10100@gmail.com  welcome to chat
// 

#ifndef MYCPPLIB_TYPE_TRAIT_HPP
#define MYCPPLIB_TYPE_TRAIT_HPP

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
    };

    template<typename T>
    struct type_trait<T *> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };


    template<typename T>
    struct type_trait<const T *> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<char> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<short> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<int> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<long> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<unsigned char> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<unsigned short> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<unsigned int> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<unsigned long> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<float> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

    template<>
    struct type_trait<double> {
        typedef true_type is_pod;
        typedef false_type has_empty_ctor;
        typedef false_type has_move_ctor;
        typedef false_type has_move_assign;
    };

};

#endif //MYCPPLIB_TYPE_TRAIT_HPP
